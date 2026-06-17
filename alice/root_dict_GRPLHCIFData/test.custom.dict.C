// 2026-06-17

// root [0] TFile f("o2-parameters-GRPLHCIFData_1778951539107.root")
// Warning in <TClass::Init>: no dictionary for class o2::parameters::GRPLHCIFData is available
// Warning in <TClass::Init>: no dictionary for class pair<long,o2::BunchFilling> is available
// Warning in <TClass::Init>: no dictionary for class o2::BunchFilling is available
// root [1] f.ls()
//   KEY: o2::parameters::GRPLHCIFData  ccdb_object;1
// root [2] f.ShowStreamerInfo()
// StreamerInfo for class: o2::parameters::GRPLHCIFData, version=1
// StreamerInfo for class: pair<long,int>, version=1
// StreamerInfo for class: pair<long,string>, version=1
// StreamerInfo for class: pair<long,float>, version=1
// StreamerInfo for class: pair<long,o2::BunchFilling>, version=1
// StreamerInfo for class: o2::BunchFilling, version=2

#include <iostream>
#include <format>
#include <TClassTable.h>
#include <TSystem.h>
#include <TFile.h>
#include "AliceO2_GRPLHCIFData/GRPLHCIFData.h"

void test_custom_dict()
{
  if (!TClassTable::GetDict("o2::parameters::GRPLHCIFData")) {
    // or TClass::GetClass() function at a higher level
    const char* dictLib = "libGRPLHCIFData.only.dict.so";
    std::cout << "Loading '" << dictLib << "'" << std::endl;
    if (gSystem->Load(dictLib) != 0) {
      std::cout << "Failed to load '" << dictLib << "'" << std::endl;
      return;
    }
  }

  TFile f("o2-parameters-GRPLHCIFData_1778951539107.root");
  // 2026-05-16 19:12:19.107 CEST 2026, MD5: 212d6df86f20d28974c8a0b8695568ac
  if (f.IsZombie()) {
    return;
  }

  o2::parameters::GRPLHCIFData* pGRPLHCIFData = nullptr; // is not derived from TObject
  // class o2::parameters::GRPLHCIFData is not polymorphic class (without virtual functions)
  // pGRPLHCIFData = (o2::parameters::GRPLHCIFData*)f.Get("ccdb_object");                   // C style cast (not safe)
  // pGRPLHCIFData = dynamic_cast<o2::parameters::GRPLHCIFData*>(f.Get("ccdb_object"));     // runtime error
  // dynamic_cast is only for polymorphic classes (with virtual functions)
  // pGRPLHCIFData = static_cast<o2::parameters::GRPLHCIFData*>(f.Get("ccdb_object"));      // compilation error
  // pGRPLHCIFData = reinterpret_cast<o2::parameters::GRPLHCIFData*>(f.Get("ccdb_object")); // low level cast (not safe)

  pGRPLHCIFData = f.Get<o2::parameters::GRPLHCIFData>("ccdb_object"); // recommended
  // TFile::Get<T> = TDirectoryFile::Get<T> = TDirectory::Get<T>
  // https://root.cern.ch/doc/master/classTFile.html#a311492f3cfc76dad3cd5781ab8090749
  // https://root.cern.ch/doc/master/TDirectory_8h_source.html#l00206
  // template <class T> inline T* Get(const char* namecycle)
  // {
  //   return static_cast<T*>(GetObjectChecked(namecycle, TClass::GetClass<T>()));
  // }
  // pGRPLHCIFData = static_cast<o2::parameters::GRPLHCIFData*>(
  //   f.GetObjectChecked("ccdb_object", TClass::GetClass<o2::parameters::GRPLHCIFData>()));
  if (!pGRPLHCIFData) {
    return;
  }
  std::cout << std::format("Fill number:      {}\n", pGRPLHCIFData->getFillNumber());
  std::cout << std::format("Injection scheme: {}\n", pGRPLHCIFData->getInjectionScheme());

  const o2::BunchFilling& rBunchFilling = pGRPLHCIFData->getBunchFilling(); // reference to avoid create copy
  // const auto& rBunchFilling = pGRPLHCIFData->getBunchFilling();          // reference to avoid create copy
  //
  // const o2::BunchFilling BunchFilling = pGRPLHCIFData->getBunchFilling(); // create (local) copy of o2::BunchFilling
  // const auto BunchFilling = pGRPLHCIFData->getBunchFilling();             // create (local) copy of o2::BunchFilling
  std::cout << std::format("Number of bunches: {}\n", rBunchFilling.getNBunches());
  // rBunchFilling.getFilledBCs(); // !!! not implemented !!! we have (use) only header files

  const std::bitset<o2::constants::lhc::LHCMaxBunches>& rBCPattern = rBunchFilling.getBCPattern();
  // const auto& rBCPattern = rBunchFilling.getBCPattern();
  // for (size_t i = 0; i < rBCPattern.size(); i++) { if (rBCPattern.test(i)) std::cout << i << std::endl; }
  // for (size_t i = 0; i < 70; i++)                { if (rBCPattern.test(i)) std::cout << i << std::endl; }

  // https://github.com/AliceO2Group/AliceO2/blob/dev/Common/Constants/include/CommonConstants/LHCConstants.h
  // https://github.com/AliceO2Group/AliceO2/blob/dev/DataFormats/common/include/CommonDataFormat/BunchFilling.h
  // getPattern(-1) = getBCPattern()    = pattern of interacting BCs at P2, -1 for InteractingBC
  // getPattern(0)  = getBeamPattern(0) = pattern of beam bunches at P2, 0 for A beam (from ATLAS side, clockwise)
  // getPattern(1)  = getBeamPattern(1) = pattern of beam bunches at P2, 1 for C beam (from CMS side, anticlockwise)
  //
  // testBC(bcID, -1) = testInteractingBC(bcID) = test interacting (-1) beams bunch    // Interacting
  // testBC(bcID, 0)  = testBeamBunch(bcID, 0)  = test clockwise (0, A) beam bunch     // Beam-A
  // testBC(bcID, 1)  = testBeamBunch(bcID, 1)  = test anticlockwise (1, C) beam bunch // Beam-C

  // const std::bitset<o2::constants::lhc::LHCMaxBunches>& rBCPattern2 = rBunchFilling.getPattern(-1);
  // const std::bitset<o2::constants::lhc::LHCMaxBunches>& rBeamAPattern = rBunchFilling.getPattern(0);
  // const std::bitset<o2::constants::lhc::LHCMaxBunches>& rBeamCPattern = rBunchFilling.getPattern(1);
  std::cout << std::endl;
  std::cout << std::format("Interacting beams, number of bunches: {}\n", rBunchFilling.getNBunches(-1));
  std::cout << std::format("Beam-A, number of bunches:            {}\n", rBunchFilling.getNBunches(0));
  std::cout << std::format("Beam-C, number of bunches:            {}\n", rBunchFilling.getNBunches(1));
  std::cout << "       I A C" << std::endl;
  for (size_t i = 90; i < 110; i++) {
    std::cout << std::format("[{:4}] ", i);
    if (rBunchFilling.testBC(i, -1)) {
      std::cout << "1 ";
    } else {
      std::cout << "0 ";
    }
    if (rBunchFilling.testBC(i, 0)) {
      std::cout << "1 ";
    } else {
      std::cout << "0 ";
    }
    if (rBunchFilling.testBC(i, 1)) {
      std::cout << "1";
    } else {
      std::cout << "0";
    }
    std::cout << std::endl;
  }
}

// ### WARNING ###
// https://en.cppreference.com/cpp/utility/bitset
//
// [] std::bitset<5> bits{"10110"}
// [] cout << bits << std::endl
// 10110   // !!!
// [] cout << bits[0] << bits[1] << bits[2] << bits[3] << bits[4] << std::endl
// 01101   // !!!
//
// bits[0] is LSB (least significant bit)
// std::bitset<> is not like a "character array", it is like a "binary number"
//
// [] std::bitset<5> Bits{}
// [] Bits.set(1, true)           // use set() function to avoid mistake, confusion
// [] cout << Bits << std::endl
// 00010
// [] cout << Bits[0] << Bits[1] << Bits[2] << Bits[3] << Bits[4] << std::endl
// 01000
// [] Bits.test(0)                // use test() function to avoid mistake, confusion
// (bool) false
// [] Bits.test(1)                // use test() function to avoid mistake, confusion
// (bool) true
