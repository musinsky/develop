// 2026-01-14

TCanvas *CanvasExactSize(const char* nt)
{
  // Exactly the graphics area size of a canvas
  int w = 1920/2, h = 1080/2; // widescreen (16:9)
  TCanvas *c = new TCanvas(nt, nt, 0, 0, w, h);
  c->SetWindowSize(w + (w - c->GetWw()), h + (h - c->GetWh()));
  c->SetGrid();
  return c;
}
TTree *GetCCTree(const char *fn)
{
  TFile *f = TFile::Open(fn, "READ");
  if (!f) return nullptr;
  TTree *t = f->Get<TTree>("cc");
  return t;
}
void fig_scalers()
{
  gStyle->SetGridColor(kGray);
  gStyle->SetOptStat(110010); // Entries, Overflow, Underflow

  // === proton-proton ===
  TTree *cc = GetCCTree("20250726.cc.root");
  if (!cc) return;
  int run = 564918;
  TString tmp = TString::Format("HasRun(%d)", run);

  TCanvas *c1 = CanvasExactSize("c1");
  cc->Draw("(GetScalerIncr(kLMB, 0) - GetScalerIncr(kLMA, 0))>>h1(10, -5, 5)", tmp.Data());
  TH1F *h1 = dynamic_cast<TH1F *>(gPad->FindObject("h1"));
  if (!h1) return;
  h1->SetMinimum(9.99);
  h1->GetXaxis()->SetNdivisions(110);
  h1->SetMarkerSize(1.25);
  h1->Draw("HIST, TEXT00");
  c1->SetLogy();
  c1->ModifiedUpdate();
  TDatime now;
  c1->Print(TString::Format("fig.scalers.%d.01.%.10s.pdf", run, now.AsSQLString()));

  TCanvas *c2 = CanvasExactSize("c2");
  cc->Draw("(GetScalerIncr(kLMB, 0) - GetScalerIncr(kInput, 2))>>h2(10, -5, 5)", tmp.Data());
  TH1F *h2 = dynamic_cast<TH1F *>(gPad->FindObject("h2"));
  if (!h2) return;
  h2->SetMinimum(9.99);
  h2->GetXaxis()->SetNdivisions(h1->GetXaxis()->GetNdivisions());
  h2->SetMarkerSize(h1->GetMarkerSize());
  h2->Draw("HIST, TEXT00");
  c2->SetLogy();
  c2->ModifiedUpdate();
  c2->Print(TString::Format("fig.scalers.%d.02.%.10s.pdf", run, now.AsSQLString()));

  TCanvas *c3 = CanvasExactSize("c3");
  tmp += " && GetEpochSec() > 1753508000 && GetEpochSec() < 1753509500";
  cc->Draw("(GetScalerIncr(kLMB, 0) - GetScalerIncr(kLMA, 0)):GetEpochSec()", tmp.Data());
  TGraph *g3 = dynamic_cast<TGraph *>(gPad->FindObject("Graph"));
  if (!g3) return;
  g3->SetMarkerStyle(7);
  g3->SetMarkerColor(kBlue+2);
  g3->SetLineStyle(3);
  g3->SetLineColor(kBlue-8);
  g3->SetDrawOption("LP");
  TH2F *htemp = dynamic_cast<TH2F *>(gPad->FindObject("htemp"));
  if (!htemp) return;
  htemp->GetXaxis()->SetNoExponent();
  htemp->GetXaxis()->SetTitleOffset(1.20);
  htemp->GetXaxis()->SetNdivisions(508);
  htemp->GetYaxis()->SetNdivisions(104);
  htemp->SetTitle(nullptr);
  TText info;
  info.SetTextAlign(22); // middle, middle
  info.SetTextFont(42);  // helvetica (default ROOT font)
  info.SetTextSize(0.03);
  tmp.Form("%s:%s", cc->GetVar1()->GetTitle(), cc->GetVar2()->GetTitle());
  info.DrawTextNDC(0.5, 0.97, tmp.Data());
  tmp.Form("{%s}", cc->GetSelect()->GetTitle());
  info.DrawTextNDC(0.5, 0.93, tmp.Data());
  c3->ModifiedUpdate();
  c3->Print(TString::Format("fig.scalers.%d.03.%.10s.pdf", run, now.AsSQLString()));

  // === Pb-Pb ===
  cc = GetCCTree("20241116.cc.root");
  if (!cc) return;
  run = 560089;
  tmp = TString::Format("HasRun(%d)", run);

  TCanvas *c10 = CanvasExactSize("c10");
  cc->Draw("(GetScalerIncr(kLMB, 16) - GetScalerIncr(kLMA, 16))>>h10(100, 0, 19e6)", tmp.Data());
  c10->Print(TString::Format("fig.scalers.%d.01.%.10s.pdf", run, now.AsSQLString()));

  TCanvas *c20 = CanvasExactSize("c20");
  cc->Draw("(GetScalerIncr(kLMB, 16) - GetScalerIncr(kInput, 2))>>h20(10, -5, 5)", tmp.Data());
  TH1F *h20 = dynamic_cast<TH1F *>(gPad->FindObject("h20"));
  if (!h20) return;
  h20->SetMinimum(9.99);
  h20->GetXaxis()->SetNdivisions(110);
  h20->SetMarkerSize(1.25);
  h20->Draw("HIST, TEXT00");
  c20->SetLogy();
  c20->ModifiedUpdate();
  c20->Print(TString::Format("fig.scalers.%d.02.%.10s.pdf", run, now.AsSQLString()));

  TCanvas *c30 = CanvasExactSize("c30");
  tmp += " && GetEpochSec() > 1731755700 && GetEpochSec() < 1731757200";
  cc->Draw("(GetScalerIncr(kLMB, 16) - GetScalerIncr(kInput, 2)):GetEpochSec()", tmp.Data());
  TGraph *g30 = dynamic_cast<TGraph *>(gPad->FindObject("Graph"));
  if (!g30) return;
  g30->SetMarkerStyle(7);
  g30->SetMarkerColor(kBlue+2);
  g30->SetLineStyle(3);
  g30->SetLineColor(kBlue-8);
  g30->SetDrawOption("LP");
  htemp = dynamic_cast<TH2F *>(gPad->FindObject("htemp"));
  if (!htemp) return;
  htemp->GetXaxis()->SetNoExponent();
  htemp->GetXaxis()->SetTitleOffset(1.20);
  htemp->GetXaxis()->SetNdivisions(507);
  htemp->GetYaxis()->SetNdivisions(104);
  htemp->SetTitle(nullptr);
  tmp.Form("%s:%s", cc->GetVar1()->GetTitle(), cc->GetVar2()->GetTitle());
  info.DrawTextNDC(0.5, 0.97, tmp.Data());
  tmp.Form("{%s}", cc->GetSelect()->GetTitle());
  info.DrawTextNDC(0.5, 0.93, tmp.Data());
  c30->ModifiedUpdate();
  c30->Print(TString::Format("fig.scalers.%d.03.%.10s.pdf", run, now.AsSQLString()));
}

// ROOT history 2025-11-11 with Roman
// cc->Draw("(GetScalerIncr(kLMB, 0) - GetScalerIncr(kLMA, 0))>>htemp(10,-5,5)",
//          "HasRun(560089)")
// cc->Draw("(GetScalerIncr(kLMB, 0) - GetScalerIncr(kInput, 2))>>htemp(100, -19e6, 0)",
//          "HasRun(560089)")
// cc->Draw("(GetScalerIncr(kLMB, 45) - GetScalerIncr(kLMA, 45))>>htemp(100, 0, 19e6)",
//          "HasRun(560089)");
// cc->Draw("(GetScalerIncr(kLMB, 45) - GetScalerIncr(kInput, 2))>>htemp(10, -5, 5)",
//          "HasRun(560089)");
