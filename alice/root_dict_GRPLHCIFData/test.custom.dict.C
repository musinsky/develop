// 2026-06-12

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
}
