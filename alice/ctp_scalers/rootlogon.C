// 2025-11-02

{
  if (TClassTable::GetDict("CTPScalerEntry")) {
    std::cout << "CTPScalers library was already loaded" << std::endl;
    return;
  }
  gROOT->ProcessLine(".L CTPScalers.cxx+");

  gStyle->SetOptStat(111111);
  gStyle->SetGridColor(kGray);
  gStyle->SetMarkerStyle(kFullDotMedium);
  gStyle->SetMarkerColor(kBlue+2);
}
