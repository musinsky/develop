// 2026-05-25

#ifdef __CLING__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// https://github.com/AliceO2Group/AliceO2/blob/dev/DataFormats/Parameters/src/ParametersDataLinkDef.h
#pragma link C++ class o2::parameters::GRPLHCIFData + ;
// https://github.com/AliceO2Group/AliceO2/blob/dev/DataFormats/common/src/CommonDataFormatLinkDef.h
#pragma link C++ class o2::BunchFilling + ;
#pragma link C++ class std::pair < long, o2::BunchFilling> + ;

#endif
