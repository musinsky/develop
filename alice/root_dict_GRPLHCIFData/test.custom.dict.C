// 2026-05-25

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
  if (!TClassTable::GetDict("o2::parameters::GRPLHCIFData"))
    if (gSystem->Load("libGRPLHCIFData.only.dict.so") != 0) return;

  TFile f("o2-parameters-GRPLHCIFData_1778951539107.root");
  // 2026-05-16 19:12:19.107 CEST 2026, MD5: 212d6df86f20d28974c8a0b8695568ac
  if (f.IsZombie()) return;

  // class o2::parameters::GRPLHCIFData is not polymorphic class, it does not inherit from TObject
  o2::parameters::GRPLHCIFData* ptrGRPLHCIFData = nullptr; // non-TObject
  ptrGRPLHCIFData = f.Get<o2::parameters::GRPLHCIFData>("ccdb_object");      // OK, recommended
  // ptrGRPLHCIFData = (o2::parameters::GRPLHCIFData *)f.Get("ccdb_object"); // OK, old C-style cast
  // ptrGRPLHCIFData = reinterpret_cast<o2::parameters::GRPLHCIFData*>(f.Get("ccdb_object"));
  // ptrGRPLHCIFData = dynamic_cast<>();   // wrong (only for polymorphic object)
  if (!ptrGRPLHCIFData) return;
  std::cout << std::format("Fill number:      {}\n", ptrGRPLHCIFData->getFillNumber());
  std::cout << std::format("Injection scheme: {}\n", ptrGRPLHCIFData->getInjectionScheme());

  const o2::BunchFilling& refBunchFilling = ptrGRPLHCIFData->getBunchFilling(); // non-TObject
  std::cout << std::format("Number of bunches: {}\n", refBunchFilling.getNBunches());
  // refBunchFilling.getFilledBCs(); // !!! not implemented !!! we have (use) only header file
  const auto& refBCPattern = refBunchFilling.getBCPattern(); // std::bitset<o2::constants::lhc::LHCMaxBunches>
  // for (size_t i = 0; i < refBCPattern.size(); i++) { if (refBCPattern.test(i)) std::cout << i << std::endl; }
  // for (size_t i = 0; i < 70; i++)                  { if (refBCPattern.test(i)) std::cout << i << std::endl; }
}
