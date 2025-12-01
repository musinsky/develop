// Date:   2025-11-19
// Author: Musinsky Jan

#include <iostream>
#include <iomanip>
#include <cinttypes>
#include <fstream>
#include <algorithm>
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include "CTPScalers.h"

//______________________________________________________________________________
CTPScalerEntry::CTPScalerEntry()
{
  // Info("CTPScalerEntry", "Default constructor");
}
//______________________________________________________________________________
CTPScalerEntry::~CTPScalerEntry()
{
  // Info("CTPScalerEntry", "Destructor");
}
//______________________________________________________________________________
void CTPScalerEntry::CheckScalers(const CTPScalerEntry* prev)
{
  if (!prev) {
    Warning("CheckScalers", "prev is nullptr");
    return;
  }

  std::uint64_t epochPrev = prev->GetEpoch();
  fEpochIncr = 0;                                          // incorrect
  if (fEpoch > epochPrev) fEpochIncr = fEpoch - epochPrev; // correct

  std::uint32_t sCurr, sPrev; // CTP scalers always 32-bit values
  std::uint64_t del64, sum64;
  EState state;
  for (std::size_t i = 0; i < NCOUNTERS; i++) {
    sCurr = fScalers[i];
    sPrev = prev->GetScalers()[i];
    fScalersIncr[i] = sCurr - sPrev; // for UNSIGNED integer overflow is guaranteed behavior

    state = EState::kDecreasing;                        // incorrect
    if (sCurr >= sPrev) state = EState::kNonDecreasing; // correct
    else {
      del64 = static_cast<std::uint64_t>(sPrev) - static_cast<std::uint64_t>(sCurr);
      sum64 = static_cast<std::uint64_t>(sPrev) + del64;
      //    = 2*previous - current
      if (sum64 > LIMIT32) state = EState::kOverflow;   // correct
    }
    fScalersStat[i] = static_cast<std::uint8_t>(state);   // due to issue ROOT-20181
  }
  // cc->Draw("Iteration$>>(1200, 0, 1200)", "fScalersStat==3")
}
//______________________________________________________________________________
bool CTPScalerEntry::HasRun(std::uint32_t run) const
{
  // First 16 scalers are run number (max. 16 runs at same time)

  if (run == 0) { // HasRun(0) = without any run, !HasRun(0) = has any run number
    if (fScalers[0] == run) return true;
    return false;
  }
  for (std::size_t r = 0; r < 16; r++)
    if (fScalers[r] == run) return true; // run was found
  return false;                          // run not found
}

//______________________________________________________________________________
std::uint64_t CTPScalers::QuickStrToULL(const char* nptr) const
{
  // Simplified standard strtoull() function (see glibc/stdlib/strtol_l.c)
  // that skip many test: whitespace, sign, recognize base, locale support,
  // wide char, overflow, etc.
  //
  // ui64 = strtoull(string, nullptr, 10);         // #include <stdlib.h>
  // std::from_chars(string, nullptr, ui64, 10);   // #include <charconv>

  if (!nptr) {
    Warning("QuickStrToULL", "nullptr");
    return 0;
  }

  const char *s = nptr;
  unsigned char c = *s;
  std::uint64_t i = 0;

  for (; c != '\0'; c = *++s) {
    if (c >= '0' && c <= '9') c -= '0';
    else {
      Warning("QuickStrToULL", "string '%s' contains non digit char '%c'", nptr, c);
      return 0;
    }
    i = i * 10ull + c;
  }
  return i;
}
//______________________________________________________________________________
void CTPScalers::CCToRoot(const char* ccfname, const char* outdn, bool skipFirst) const
{
  FILE *fin = fopen(ccfname, "r");
  if (!fin) {
    Error("CCToRoot", "fopen('%s') failed: %s", ccfname, strerror(errno));
    return;
  }
  ccfname = gSystem->BaseName(ccfname);
  TFile ccFile(TString::Format("%s/%s.root", outdn, ccfname), "RECREATE");
  if (ccFile.IsZombie()) return;
  // ccFile.SetCompressionLevel(ROOT::RCompressionSetting::ELevel::kUncompressed);
  TTree ccTree(CCTN, ccfname);   // "cc" means CTP counters
  CTPScalerEntry *entry = new CTPScalerEntry();
  ccTree.Branch("CTPScalerEntry", &entry);

  // UINT64_MAX = 18.446.744.073.709.551.615 => max. 20 digits
  // UINT32_MAX =              4.294.967.295 => max. 10 digits

  // standard Unix epoch time in seconds precision
  // epoch  999999999  (9 digits) => 2001-09-09 01:46:39 UTC
  // epoch 4294967295 (10 digits) => 2106-02-07 06:28:15 UTC
  // epoch 9999999999 (10 digits) => 2286-11-20 17:46:39 UTC
  //
  // epoch 1753480801 (10 digits) => 2025-07-26 00:00:01 CEST
  // epoch time in nanoseconds precision
  // 1753480801.035600400 = seconds (10 digits) + nanoseconds (9 digits) < UINT64_MAX
  //
  // In "cc" files epoch time is written as 64-bit float number using Python time.time()
  // with an accuracy of 7 decimal places = 100 ns = 0.1 us = 0.0001 ms
  // NOTE: print of floating-point number truncates trailing zeros
  // https://docs.python.org/library/time.html
  // time.time()    as float   (64-bit)  1760957300.6160468  = (10 exactly + max. 7)    digits
  // time.time_ns() as integer (dynamic) 1760957300616046834 = (10 exactly + 9 exactly) digits
  //
  // 1760957300616046834 [19]   1 ns precision, epoch time from Python time.time_ns() function
  // 17609573006160468   [17] 100 ns precision, epoch time from Python time.time() function
  // 1760957300616       [13]   1 ms precision, epoch time in ALICE CCDB
  // 1760957300          [10]   1 s  precision, epoch time as standard Unix time

  char buffer[4096*16];         // 4096*16 = 65536 = 64 kB (most caches are 64 kB aligned)
  char word[20+1];              // max. 20 digits for any uint64_t value
  std::uint64_t value64;        // 64-bit (temporary) value
  std::array<std::uint32_t, NCOUNTERS> values32 = {}; // 32-bit [NCOUNTERS = 1085] values
  char epochCharNano[10+9+1];   // exactly 19 digits, nanoseconds precision
  std::uint64_t epochNano = 0;  // exactly 19 digits, nanoseconds precision
  std::uint64_t epochSec = 0;   // exactly 10 digits, seconds precision

  std::size_t nread, i, n;
  std::size_t wordSize = 0, countValues = 0, countLines = 0;
  bool epochDot = false;
  epochCharNano[10+9] = '\0';

  CTPScalerEntry entryPrev;
  bool firstEntry = false;
  if (skipFirst) firstEntry = true;

  do {
    nread = fread(buffer, sizeof(buffer[0]), std::size(buffer), fin);
    for (i = 0; i < nread; i++) {

      if (buffer[i] == '.') {   // word = timestamp epoch SECONDS.nanoseconds
        word[wordSize] = '\0';  // word before dot (first word)

        if (wordSize != 10) { // exactly 10 digits, seconds part
          Warning("CCToRoot", "epoch (seconds) '%s' does not contain exactly 10 digits", word);
          epochSec = 0;
        } else {
          epochSec = QuickStrToULL(word); // exactly 10 digits < UINT64_MAX
          for (n = 0; n < wordSize; n++)
            epochCharNano[n] = word[n]; // manual memcpy
        }
        epochDot = true;
        // printf("epochSec: '%s' (%zu-digit) => %" PRIu64 "\n", word, wordSize, epochSec);

        wordSize = 0;
        continue;
      }

      if (buffer[i] == ' ') {
        word[wordSize] = '\0';  // word before space (next word)

        if (epochDot == true) { // word = timestamp epoch seconds.NANOSECONDS
          if (wordSize > 7) { // max. 7 digits, nanoseconds (100 ns) part
            Warning("CCToRoot", "epoch (nanoseconds) '%s' contains more than 7 digits", word);
            epochNano = 0;
          } else {
            // don't use numeric data type for store nanoseconds part: 0356004 != 356004
            for (n = 0; n < wordSize; n++)
              epochCharNano[10+n] = word[n]; // merge with seconds + manual memcpy
            for (n = (10+wordSize); n < (10+9); n++)
              epochCharNano[n] = '0';        // add trailing zeros
            // epochCharNano[10+9] = '\0';
            epochNano = QuickStrToULL(epochCharNano); // exactly 19 digits < UINT64_MAX
            if (epochSec == 0) epochNano = 0;
          }
          epochDot = false;
          // printf("epochNano: '%s' (%zu-digit) => %" PRIu64 "\n", word, wordSize, epochNano);
        } else {                // word = value (scaler/counter)
          value64 = QuickStrToULL(word); // max. 20 digits (as 32-bit value max. 10 digits)
          if (value64 > LIMIT32) {
            Error("CCToRoot", "scaler/counter value must always be 32-bit\n"
                  "value64: %" PRIu64 " > LIMIT32: %" PRIu64 "\n", value64, LIMIT32);
            return;
          }
          values32[countValues++] = static_cast<std::uint32_t>(value64);
          // printf("value32[%4zu]: '%s' (%zu-digit) => %" PRIu32 "\n",
          //        countValues-1, word, wordSize, values32[countValues-1]);
          if (countValues >= std::size(values32)) {
            Error("CCToRoot", "countValues: %zu >= std::size(values32): %zu",
                  countValues, std::size(values32));
            return;
          }
        }

        wordSize = 0;
        continue;
      }

      if (buffer[i] == '\n') {  // word = value (scaler/counter)
        word[wordSize] = '\0';  // word before POSIX newline (last word)

        value64 = QuickStrToULL(word); // max. 20 digits (as 32-bit value max. 10 digits)
        if (value64 > LIMIT32) {
          Error("CCToRoot", "scaler/counter value must always be 32-bit\n"
                "value64: %" PRIu64 " > LIMIT32: %" PRIu64 "\n", value64, LIMIT32);
          return;
        }
        // if (countValues >= std::size(values32)) was checked before (see above)
        // and countValues is not changing anymore
        values32[countValues] = static_cast<std::uint32_t>(value64);
        // printf("value32[%4zu]: '%s' (%zu-digit) => %" PRIu32 "\n\n",
        //        countValues, word, wordSize, values32[countValues]);
        if ((countValues+1) != std::size(values32)) { // NCOUNTERS = 1085
          Error("CCToRoot", "number of scalers/counters is exactly predetermined\n"
                "(countValues+1): %zu != std::size(values32): %zu (line: %zu)\n",
                (countValues+1), std::size(values32), countLines);
          return;
        }

        entry->SetEpoch(epochNano);
        entry->AssignScalers(values32);
        entry->CheckScalers(&entryPrev);
        entryPrev = *entry; // default C++ assignment operator
        if (!firstEntry) ccTree.Fill();

        firstEntry = false;
        countLines++;
        countValues = 0; // per line
        wordSize = 0;
        continue;
      }

      word[wordSize++] = buffer[i];
      if (wordSize >= std::size(word)) {
        Error("CCToRoot", "wordSize: %zu >= std::size(word): %zu",
              wordSize, std::size(word));
        return;
      }

    }
  } while (nread == std::size(buffer));

  ccTree.Print("toponly");
  printf("\nROOT file '%s' created", ccFile.GetName());
  if (skipFirst)
    printf(" (without first entry)\n");
  else
    printf(" (with first entry)\n");
  ccFile.Write();
  ccFile.Close();
  delete entry;

  // fread(3) does not distinguish between end-of-file and error
  if (feof(fin) == 0) Error("CCToRoot", "end-of-file indicator was not reached");
  fclose(fin);
  printf("countLines: %zu\n", countLines);
}
//______________________________________________________________________________
void CTPScalers::RootToCC(const char* rootfn, const char* outdn) const
{
  // Write identical (byte by byte) replica file as "cc" file from parsed values
  // stored in root tree file (only for compare, verify).
  // If root file skip first entry: $ diff YYYYMMDD.cc.replica <(tail -n +2 YYYYMMDD.cc)

  TFile *f = TFile::Open(rootfn, "READ");
  if (!f) return;
  TTree *cc = f->Get<TTree>(CCTN);
  if (!cc) return;
  CTPScalerEntry *entry = new CTPScalerEntry();
  cc->SetBranchAddress("CTPScalerEntry", &entry);

  TString rfname = TString::Format("%s/%s.replica", outdn, cc->GetTitle());
  FILE *freplica = fopen(rfname.Data(), "w");
  if (!freplica) {
    Error("RootToCC", "fopen('%s') failed: %s", rfname.Data(), strerror(errno));
    return;
  }

  std::array<std::uint32_t, NCOUNTERS> values32 = {}; // 32-bit [NCOUNTERS = 1085] values
  std::uint64_t epochNano;      // exactly 19 digits, nanoseconds precision
  // epoch: 1753480801.0356004  => 1753480801035600400 # CCToRoot()
  // epoch: 1753480801035600400 => 1753480801.0356004  # RootToCC()
  // don't use numeric data type for store nanoseconds part: 0356004 != 356004
  std::uint64_t nano = 1000000000;
  int exp = 9;

  for (std::int64_t i = 0; i < cc->GetEntries(); i++) {
    cc->GetEntry(i);

    epochNano = entry->GetEpoch();
    nano = 1000000000; exp = 9;
    fprintf(freplica, "%" PRIu64 ".", epochNano/nano);      // epoch seconds part
    if (epochNano != 0)
      while (epochNano%10 == 0) {
        epochNano /= 10; // remove trailing zeros
        nano /= 10; exp--;
      }
    if (exp > 0)
      fprintf(freplica, "%0*" PRIu64, exp, epochNano%nano); // epoch nanoseconds part

    values32 = entry->GetScalers();
    for (std::size_t n = 0; n < std::size(values32); n++)
      fprintf(freplica, " %" PRIu32, values32[n]);
    fprintf(freplica, "\n");
  }

  f->Close();
  delete entry;
  fclose(freplica);
  printf("Replica file '%s' created\n", rfname.Data());
}
// void CTPScalers::RootToCC() const
// {
//   std::uint64_t epochNano;      // exactly 19 digits, nanoseconds precision
//   char epochCharNano[10+9+1+1]; // exactly 19 digits + 1 'dot' char, nanoseconds precision
//   // don't use numeric data type for store nanoseconds part: 0356004 != 356004
//   // epoch: 1753480801.0356004  => 1753480801035600400 # CCToRoot()
//   // epoch: 1753480801035600400 => 1753480801.0356004  # RootToCC()

//   for (std::int64_t i = 0; i < cc->GetEntries(); i++) {
//     cc->GetEntry(i);

//     epochNano = entry->GetEpoch();
//     if (epochNano != 0)
//       while (epochNano%10 == 0) epochNano /= 10; // remove trailing zeros
//     int nanoLen = sprintf(epochCharNano, "%" PRIu64, epochNano);
//     if ((nanoLen < 0) || ((nanoLen+1) >= (int)std::size(epochCharNano))) {
//       Error("RootToCC", "wrong lenght: %d of epochCharNano", nanoLen);
//       return;
//     }
//     for (int j = (nanoLen - 1); j >= 10; j--)
//       epochCharNano[j+1] = epochCharNano[j];     // shift nanoseconds part by one char
//     epochCharNano[10] = '.';                     // add 'dot' char after seconds part
//     epochCharNano[nanoLen+1] = '\0';             // terminating null byte
//     fprintf(freplica, "%s", epochCharNano);
//     // printf("epochNano: %" PRIu64 ", epochCharNano: %s\n", entry->GetEpoch(), epochCharNano);
//   }
// }
//______________________________________________________________________________
std::vector<std::size_t> CTPScalers::ParseActiveClasses(std::uint32_t run,
                                                        const char* cfgdir) const
{
  // Very simplified parser (only finds all active classes)
  // AliceO2/DataFormats/Detectors/CTP/include/DataFormatsCTP/Configuration.h

  std::vector<std::size_t> classesIdx;
  std::string fn = TString::Format("%s/%u.rcfg2", cfgdir, run).Data(); // since C++20 std::format
  std::ifstream file(fn);
  if (!file.is_open()) {
    Error("ParseActiveClasses", "file('%s') failed", fn.c_str());
    return classesIdx;
  }

  std::string line;
  bool afterCluster = false;
  while (std::getline(file, line)) {
    if (!afterCluster) {
      if (line.find("cluster") != std::string::npos) afterCluster = true;
      continue;
    }

    if (line.find('{') != std::string::npos && line.find('}') != std::string::npos) {
      std::istringstream iss(line);
      std::string word;
      std::size_t number;
      iss >> word; // skip 1st word
      if (iss >> number) classesIdx.push_back(number);
      else Error("ParseActiveClasses", "2nd word is not number");
    }
  }

  // file.close(); // RAII (destructor automatically closes file)
  return classesIdx;
}
//______________________________________________________________________________
void CTPScalers::ClassesConditions(const char* rootfn, std::uint32_t run,
                                   std::ostream& stream) const
{
  TFile *f = TFile::Open(rootfn, "READ");
  if (!f) return;
  TTree *cc = f->Get<TTree>(CCTN);
  if (!cc) return;
  CTPScalerEntry *entry = new CTPScalerEntry();
  cc->SetBranchAddress("CTPScalerEntry", &entry);

  // 64 classes x 5 conditions (LMB >= LMA >= L0B >= L0A >= L1B >= L1A)
  std::size_t cnt[64][5] = {};
  std::size_t cnt0 = 0;
  std::vector<std::size_t> clsIdx = ParseActiveClasses(run);
  bool isActiveClass = false;

  for (std::int64_t i = 0; i < cc->GetEntries(); i++) {
    cc->GetEntry(i);
    if (!entry->HasRun(run)) continue;

    cnt0++;
    for (std::size_t cls = 0; cls < std::size(cnt); cls++) {
      if (entry->GetScalerIncr(kLMB, cls) < entry->GetScalerIncr(kLMA, cls)) cnt[cls][0]++;
      if (entry->GetScalerIncr(kLMA, cls) < entry->GetScalerIncr(kL0B, cls)) cnt[cls][1]++;
      if (entry->GetScalerIncr(kL0B, cls) < entry->GetScalerIncr(kL0A, cls)) cnt[cls][2]++;
      if (entry->GetScalerIncr(kL0A, cls) < entry->GetScalerIncr(kL1B, cls)) cnt[cls][3]++;
      if (entry->GetScalerIncr(kL1B, cls) < entry->GetScalerIncr(kL1A, cls)) cnt[cls][4]++;
    }
  }

  stream << "#                     Causality conditions for CTP classes\n"
         << "#                    LMB >= LMA >= L0B >= L0A >= L1B >= L1A\n"
         << "#                       (all values are in percentages)\n"
         << "#         "
         << "run: " << run << ", file: " << gSystem->BaseName(f->GetName())
         << ", entries: " << cnt0 << "\n" << "#\n"
         << "#         "
         << "| LMB < LMA | LMA < L0B | L0B < L0A | L0A < L1B | L1B < L1A |\n"
         << "#         "
         << "|-----------|-----------|-----------|-----------|-----------|\n";
  stream << std::fixed << std::setprecision(2);
  for (std::size_t cls = 0; cls < std::size(cnt); cls++) {
    if (std::find(clsIdx.begin(), clsIdx.end(), cls) != clsIdx.end()) isActiveClass = true;
    else isActiveClass = false;
    stream << "class " << std::setw(2) << cls << (isActiveClass ? " +|  " : "  |  ")
           << std::setw(6) << cnt[cls][0]*100.0/cnt0 << (cnt[cls][0] > 0 ? "  *|  " : "   |  ")
           << std::setw(6) << cnt[cls][1]*100.0/cnt0 << (cnt[cls][1] > 0 ? "  *|  " : "   |  ")
           << std::setw(6) << cnt[cls][2]*100.0/cnt0 << (cnt[cls][2] > 0 ? "  *|  " : "   |  ")
           << std::setw(6) << cnt[cls][3]*100.0/cnt0 << (cnt[cls][3] > 0 ? "  *|  " : "   |  ")
           << std::setw(6) << cnt[cls][4]*100.0/cnt0 << (cnt[cls][4] > 0 ? "  *|" : "   |")
           << std::endl;
  }

  f->Close();
  delete entry;
}
