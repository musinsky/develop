// 2025-10-15

// see also '../c/read.values.fast.c'

#include <TFile.h>
#include <TTree.h>

void cc2root()
{
  TString fname = "20250726.cc"; // "cc" means CTP counters
  fname = "../c/read.values.file.sample";
  FILE *fin = fopen(fname.Data(), "r");
  if (!fin) {
    fprintf(stderr, "fopen('%s') failed: %s\n", fname.Data(), strerror(errno));
    return;
  }
  TFile ccFile(TString::Format("%s.root", fname.Data()), "RECREATE");
  // ccFile.SetCompressionLevel(ROOT::RCompressionSetting::ELevel::kUncompressed);
  TTree ccTree("cc", fname.Data());
  // write identical file as read file (only for compare, verify)
  // FILE *fcopy = fopen(TString::Format("/tmp/%s.copy", fname.Data()), "w");
  FILE *fcopy = nullptr;

  // on x86_64 platform (unsigned) long long = (unsigned) long = (u)int64_t = 8 bytes
  // ULLONG_MAX = ULONG_MAX = 18.446.744.073.709.551.615 => max. 20 digits
  //               UINT_MAX =              4.294.967.295 => max. 10 digits
  static_assert(sizeof(ULong_t) == 8, "code assumes 8-bytes unsigned long");

  char buffer[4096*16];         // 4096*16 = 65536 = 64 kB (most caches are 64 kB aligned)
  char word[20+1];              // max. 20 digits for uint64_t value
  std::uint64_t value64;        // 64-bit (temporary) value
  static constexpr std::uint64_t kLimit32 = 0xFFFFFFFFull;
  std::uint32_t values32[1085]; // 1085 (32-bit) values (NCOUNTERS = 1085, see bellow)
  std::uint64_t epoch;          // exactly 13 digits (ALICE CCDB millisecond resolution)
  std::uint64_t epochSec;       // exactly 10 digits (see bellow)
  char epochNano[9+1];          // max. 9 digits (see bellow)
  bool epochDot = false;

  // ccTree.Branch("epoch", &epoch, "epoch/g"); // epoch/1000 = epochSec
  ccTree.Branch("epoch", &epochSec, "epoch/g");
  ccTree.Branch("scaler", values32, TString::Format("scaler[%zu]/i", std::size(values32)));
  // 'i' 32-bit unsigned integer (UInt_t), 13                  => std::uint32_t or unsigned int
  // 'g' long unsigned integer, stored as 64 bit (ULong_t), 14 => std::uint64_t or unsigned long
  // 'l' 64-bit unsigned integer (ULong64_t), 17               => unsigned long long

  size_t nread, i, n;
  size_t wordSize = 0, countValues = 0, countLines = 0;

  do {
    nread = fread(buffer, sizeof(buffer[0]), std::size(buffer), fin);
    for (i = 0; i < nread; i++) {

      if (buffer[i] == '.')  { // word = timestamp epoch SECONDS.nanoseconds
        word[wordSize] = '\0'; // word before dot (first word)
        // epoch  999999999  (9 digits) => 2001-09-09 01:46:39 UTC
        // epoch 9999999999 (10 digits) => 2286-11-20 17:46:39 UTC
        // epoch 1753480801.035600400   => exactly 10 digits for seconds
        // std::from_chars(word, word+wordSize, epochSec); // #include <charconv>
        // epochSec = strtoull(word, nullptr, 10); // better choice than atoll()
        epochSec = atoll(word);
        if ((wordSize != 10) || (epochSec == 0))
          fprintf(stderr, "epochSec '%s' does not contain 10 digits\n", word);
        // printf("epochSec: '%s' (%zu-digit) => %lu\n", word, wordSize, epochSec);
        epochDot = true;
        wordSize = 0;
        continue;
      }

      if (buffer[i] == ' ')  {
        word[wordSize] = '\0'; // word before space

        if (epochDot == true) { // word = timestamp epoch seconds.NANOSECONDS
          // epoch 1753480801.035600400 => there should be 9 digits for nanoseconds
          //       1753566637.0356004   => python print truncates trailing zeros
          // don't use numeric data type: 0356004 != 356004
          if (wordSize >= std::size(epochNano)) {
            fprintf(stderr, "wordSize: %zu >= std::size(epochNano): %zu\n",
                    wordSize, std::size(epochNano));
            epochNano[0] = '\0';
          } else {
            for (n = 0; n < wordSize; n++) // manual memcpy
              epochNano[n] = word[n];
            epochNano[n] = '\0';
          }
          // printf("epochNano: '%s' (%zu-digit) => '%s'\n", word, wordSize, epochNano);
          epochDot = false;
          epoch = atoll(TString::Format("%lu%.3s", epochSec, epochNano).Data()); // 10+3 digit
          // printf("epoch: %lu\n", epoch);

        } else {                // word = value (scaler/counter)
          value64 = atoll(word);
          if (value64 > kLimit32) {
            // value of scaler/counter must always be 32-bit
            fprintf(stderr, "FATAL error, value64: %lu > kLimit32: %lu\n", value64, kLimit32);
            return;
          }
          values32[countValues++] = value64;
          // printf("value[%4zu]: '%s' (%zu-digit) => %u\n",
          //        countValues-1, word, wordSize, values32[countValues-1]);
          if (countValues >= std::size(values32)) {
            fprintf(stderr, "FATAL error, countValues: %zu >= std::size(values32): %zu\n",
                    countValues, std::size(values32));
            return;
          }
        }

        wordSize = 0;
        continue;
      }

      if (buffer[i] == '\n') { // word = value (scaler/counter)
        word[wordSize] = '\0'; // word before POSIX newline (last word)

        // if (countValues >= std::size(values32)) was checked before (see above)
        // and countValues is not changing anymore

        value64 = atoll(word);
        if (value64 > kLimit32) {
          // value of scaler/counter must always be 32-bit
          fprintf(stderr, "FATAL error, value64: %lu > kLimit32: %lu\n", value64, kLimit32);
          return;
        }
        values32[countValues] = value64;
        // printf("value[%4zu]: '%s' (%zu-digit) => %u\n\n",
        //        countValues, word, wordSize, values32[countValues]);

        // number of values (ALICE CTP counters) is exactly predetermined
        // AliceO2/DataFormats/Detectors/CTP/include/DataFormatsCTP/Scalers.h
        // static constexpr uint32_t NCOUNTERS = 1085;
        if ((countValues+1) != std::size(values32))
          fprintf(stderr, "(countValues+1): %zu != std::size(values32): %zu\n",
                  (countValues+1), std::size(values32));

        // write identical file as read file from parsed values (only for compare, verify)
        if (fcopy) {
          fprintf(fcopy, "%lu.%s", epochSec, epochNano);
          for (n = 0; n < (countValues+1); n++)
            fprintf(fcopy, " %u", values32[n]);
          fprintf(fcopy, "\n");
        }

        ccTree.Fill();
        countLines++;
        countValues = 0; // per line
        wordSize = 0;
        continue;
      }

      word[wordSize++] = buffer[i];
      if (wordSize >= std::size(word)) {
        fprintf(stderr, "FATAL error, wordSize: %zu >= std::size(word): %zu\n",
                wordSize, std::size(word));
        return;
      }

    }
  } while (nread == std::size(buffer));

  ccFile.Write();
  ccFile.Close();
  if (fcopy) fclose(fcopy);

  //  fread(3) does not distinguish between end-of-file and error, and callers
  //  must use feof(3) and ferror(3) to determine which occurred.
  if (feof(fin) == 0) fprintf(stderr, "end-of-file indicator was not reached\n");
  fclose(fin);

  printf("countLines: %zu\n", countLines);
}
