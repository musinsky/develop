// 2026-05-18

#include <iostream>
#include <vector>
#include <format>
#include <filesystem>

#include <TFile.h>

#include "CCDB/CcdbApi.h"
#include "DataFormatsParameters/GRPLHCIFData.h"

struct SGROut {
  bool enabled;
  const char* bold()  const { return enabled ? "\033[1m"  : ""; }
  const char* red()   const { return enabled ? "\033[1;31m" : ""; }
  const char* green() const { return enabled ? "\033[1;32m" : ""; }
  const char* blue()  const { return enabled ? "\033[1;34m" : ""; }
  const char* reset() const { return enabled ? "\033[0m"  : ""; }
};

SGROut sgr{ isatty(fileno(stdout)) != 0 };
o2::ccdb::CcdbApi ccdb;

long findRunTimestamp(int run)
{
  std::map<std::string, std::string> headers;
  headers = ccdb.retrieveHeaders("RCT/Info/RunInformation", // Run Condition Table
                                 std::map<std::string, std::string>(), run);
  if (headers.empty()) {
    LOGP(error, "'RCT/Info/RunInformation' not in '{}', run: {}", ccdb.getURL(), run);
    return -1;
  }
  // for (const auto& [key, value] : headers) {
  //   std::cout << std::format("[ {} ] => {}\n", key, value);
  // }

  // BasicCCDBManager::getForRun() using the timestamp in the middle of the run
  // [STF, ETF] if missing => [SOX, EOX] => if missing [SOR, EOR]
  auto stf = headers.find("STF"); // Start of Time Frame
  long stfL = -1L;
  if (stf != headers.end()) {
    std::from_chars(stf->second.data(), stf->second.data() + stf->second.size(), stfL, 10);
  }
  auto etf = headers.find("ETF"); // End of Time Frame
  long etfL = -1L;
  if (etf != headers.end()) {
    std::from_chars(etf->second.data(), etf->second.data() + etf->second.size(), etfL, 10);
  }

  if ((stfL == -1) || (etfL == -1)) {
    cout << std::format("run: {} without Start/End of Time Frame\n", run);
    return -1;
  }

  cout << sgr.green() << std::format("run: {}", run) << sgr.reset();
  auto lm = headers.find("Last-Modified");
  if (lm != headers.end()) {
    cout << std::format(" ({} => {})\n", lm->first, lm->second);
  }
  else {
    cout << "\n";
  }
  auto stfT = std::chrono::system_clock::time_point(std::chrono::milliseconds(stfL));
  auto etfT = std::chrono::system_clock::time_point(std::chrono::milliseconds(etfL));
  cout << std::format("STF:       {} = {} UTC\n", stfL, stfT);
  cout << std::format("ETF:       {} = {} UTC\n", etfL, etfT);
  long runTimestamp = (stfL + etfL) / 2;
  auto runT = std::chrono::system_clock::time_point(std::chrono::milliseconds(runTimestamp));
  cout << sgr.green() << std::format("timestamp: {}", runTimestamp) << sgr.reset();
  cout << std::format(" = {} UTC = (STF+ETF)/2\n", runT);
  return runTimestamp;
}

std::string uniqueFileName(const std::string& originalName)
{
  // if exist '/path/to/file.txt' => '/path/to/file.2026-05-17_16-17-54.txt' (always unique name)
  std::filesystem::path p(originalName);
  if (!std::filesystem::exists(originalName)) {
    return originalName;
  }

  auto now = std::chrono::system_clock::now(); // current time in UTC time zone
  // long long nowLL = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
  auto now1 = std::chrono::current_zone()->to_local(now);
  // auto now1 = std::chrono::locate_zone("Asia/Tokyo")->to_local(now);
  auto now2 = std::chrono::floor<std::chrono::seconds>(now1);

  std::string newName = std::format("{}.{:%F_%H-%M-%S}{}", p.stem().string(), now2, p.extension().string());
  newName = (p.parent_path() / newName).string();
  // using std::filesystem function(s) is more more complex, efficient
  // 1st) if relative path (no parent path) => correct file name 'file.txt' (without '/' prefix)
  // 2nd) Linux '/path/to/file.txt', Windows 'C:\path\to\file.txt'

  return newName;
}

void findLHCData(long ts)
{
  std::map<std::string, std::string> metadata;
  // metadata["fillNumber"] = "9644"; // only as example, in our case no metadata requirements
  std::map<std::string, std::string> headers;
  // o2::parameters::GRPLHCIFData *ptrGRPLHCIFData = nullptr;
  // const auto ptrGRPLHCIFData => std::enable_if<true, o2::parameters::GRPLHCIFData *>::type
  const auto ptrGRPLHCIFData =
    ccdb.retrieveFromTFileAny<o2::parameters::GRPLHCIFData>("GLO/Config/GRPLHCIF", metadata, ts, &headers);
  // General/Global Run Parameters LHC InterFace data
  if (ptrGRPLHCIFData == nullptr) {
    LOGP(error, "'GLO/Config/GRPLHCIF' not in '{}', timestamp: {}", ccdb.getURL(), ts);
    return;
  }

  // cout << "metadata" << endl;
  // for (const auto& [key, value] : metadata) {
  //   std::cout << std::format("[ {} ] => {}\n", key, value);
  // }
  // cout << "headers" << endl;
  // for (const auto& [key, value] : headers) {
  //   std::cout << std::format("[ {} ] => {}\n", key, value);
  // }
  // cout << endl;
  // // TODO (print Valid-From -> Valid-Until, maybe Last-Modified)
  // ptrGRPLHCIFData->print();

  // const o2::BunchFilling& bunchFilling = ptrGRPLHCIFData->getBunchFilling();
  const auto& bunchFilling = ptrGRPLHCIFData->getBunchFilling();

  // AliceO2/DataFormats/common/include/CommonDataFormat/BunchFilling.h
  // dir=0,1: for C,A beams, dir=-1: for interacting BCs
  // bunchFilling.print();

  // POZOR
  // auto p =        bunchFilling.getBCPattern(); // vytvara KOPIU objektu
  // const auto& r = bunchFilling.getBCPattern(); // (const) referencia
  // std::cout << "p address: " << &p << std::endl;
  // std::cout << "r address: " << &r << std::endl;
  // std::cout << "address  : " << &bunchFilling.getBCPattern() << std::endl;

  //  std::cout << "FINAL\n";
  std::string fn = std::format("LHC.{}.{}", ptrGRPLHCIFData->getFillNumber(), ptrGRPLHCIFData->getInjectionScheme());

  auto filledBC = bunchFilling.getFilledBCs(-1); // std::vector (as "numbers") [0, 3563]
  const auto& patternBC = bunchFilling.getBCPattern(); // std::bitset (as "pattern"), OK we need only reference
  auto patternBCCopy =    bunchFilling.getBCPattern(); // wrong, unnecessary copy creation (we only need to read BC pattern)
  // to same logic as above: const auto& bunchFilling = ptrGRPLHCIFData->getBunchFilling();

  // as ROOT file (only std::vector, for std::bitset it would be necessary to create dictionary)
  TFile f(uniqueFileName(fn + ".root").data(), "NEW");
  f.WriteObject(&filledBC, "filledBC"); // auto* v = (std::vector<int>*)_file0->Get("filledBC");
  f.Close();

  // as stream file
  auto now = std::chrono::system_clock::now();
  auto local_time = std::chrono::current_zone()->to_local(now);
  std::string fn2 = uniqueFileName(fn + ".numbers");
  std::ofstream out(fn2);
  out << std::format("# {:%F}\n", local_time);
  out << std::format("# Fill            : {}\n", ptrGRPLHCIFData->getFillNumber());
  out << std::format("# Injection scheme: {}\n", ptrGRPLHCIFData->getInjectionScheme());
  bool first = true;
  for (auto i : filledBC) {
    if (!first)
      out << " ";
    out << i;
    first = false;
  }
  out << std::endl; // flush to file
  if (out.good()) {
    cout << sgr.green() << fn2 << sgr.reset() << " created" << std::endl;
  }
  // read from file => std::vector<int> v; int x; while (in >> x) { v.push_back(x); }

  std::ofstream out2(uniqueFileName(fn + ".bits"));
  out2 << bunchFilling.getBCPattern().to_string();

  delete ptrGRPLHCIFData; // ?! need delete ?! see AliceO2/CCDB/test/testCcdbApi.cxx
  // ?! BasicCCDBManager => user should not delete retrieved objects ?!
}

void ccdb_LHC_data()
{
  // gSystem->Load("libO2CCDB.so");
  // gSystem->Load("libO2DataFormatsParameters.so");

  // timestamp -1 or 0 = current (now) timestamp

  long mTimestamp = 1716040930304 + 1; // fill:  9644, run: 551731
  // => /alice/data/CCDB/GLO/Config/GRPLHCIF/07/14077/38376a05-151f-11ef-a37a-0aa202c71b9a

  // if "https://" or "alice-ccdb.cern.ch" => need alien token
  // o2::ccdb::CcdbApi ccdb;
  ccdb.init("alice-ccdb.cern.ch");
  // ccdb.isHostReachable();

  // findLHCData(mTimestamp);
  // findLHCData(findRunTimestamp(565118));
  findLHCData(findRunTimestamp(571771));
}

/**
 // ROOT copy paste code block
 //
 // long mTimestamp = 1716040930304 + 1; // alice-ccdb (need alien token)
 long mTimestamp = 1697170039223 + 1; // ccdb-test
 o2::ccdb::CcdbApi ccdb;
 // ccdb.init("alice-ccdb.cern.ch");
 ccdb.init("ccdb-test.cern.ch:8080");
 std::map<std::string, std::string> metadata;
 std::map<std::string, std::string> headers;
 const auto ptrGRPLHCIFData = ccdb.retrieveFromTFileAny<o2::parameters::GRPLHCIFData>("GLO/Config/GRPLHCIF", metadata, mTimestamp, &headers);

 ptrGRPLHCIFData->print();
 // const auto& bunchFilling = ptrGRPLHCIFData->getBunchFilling();
 // bunchFilling.print();
 **/
