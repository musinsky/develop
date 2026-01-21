// 2026-01-15

void PrintInfo(TVirtualPad *pad, TObject *obj, Int_t x, Int_t y)
{
  TGraph *g = dynamic_cast<TGraph *>(obj);
  if (!g) return;
  TH2F *htemp = dynamic_cast<TH2F *>(pad->FindObject("htemp"));
  if (!htemp) return;

  std::string info = std::format("[{:4d}] x = {:10.0f}, y = {:10.0f}",
                                 x, g->GetPointX(x), g->GetPointY(x));
  htemp->SetTitle(info.c_str());
  // pad->Update();
  std::cout << info << std::endl;
}

void hlprint()
{
  if (!gPad) return;
  gPad->GetCanvas()->HighlightConnect("PrintInfo(TVirtualPad*,TObject*,Int_t,Int_t)");
}
