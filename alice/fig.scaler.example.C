// 2026-01-13

void fig_scaler_example()
{
  TFile *f = TFile::Open("20250726.cc.root", "READ");
  if (!f) return;
  TTree *cc = f->Get<TTree>("cc");
  if (!cc) return;
  // CTPScalerEntry *entry = nullptr;
  // cc->SetBranchAddress("CTPScalerEntry", &entry);

  // setenv("TZ", "Asia/Tokyo", 1); tzset(); // only for testing
  setenv("TZ", "Europe/Zurich", 1); tzset(); // set CERN time as local time
  int scaler = 647, run = 564918;
  cc->Draw(TString::Format("GetScaler(%d):GetEpochSec()", scaler),
           TString::Format("HasRun(%d)", run));
  TGraph *g = dynamic_cast<TGraph *>(gPad->FindObject("Graph"));
  if (!g) return;
  g->SetMarkerStyle(6);
  g->SetMarkerColor(kGreen+2);
  TH2F *htemp = dynamic_cast<TH2F *>(gPad->FindObject("htemp"));
  if (!htemp) return;
  // htemp->GetXaxis()->SetNoExponent();
  htemp->GetXaxis()->SetTitleOffset(1.20);
  htemp->GetXaxis()->SetTimeDisplay(1); // switch (*TOGGLE*) time display
  htemp->GetXaxis()->SetTimeFormat("%H:%M");
  // htemp->GetXaxis()->SetTitle("time, H:M");
  // htemp->GetXaxis()->SetTitle(TString::Format("%s, H:M", cc->GetVar(1)->GetTitle()));
  htemp->GetXaxis()->SetTitle(TString::Format("%s, H:M", htemp->GetXaxis()->GetTitle()));
  htemp->GetYaxis()->SetTitle("raw value of scaler");
  htemp->GetYaxis()->SetTitleColor(g->GetMarkerColor());
  htemp->GetYaxis()->SetLabelColor(g->GetMarkerColor());
  htemp->GetYaxis()->CenterTitle();
  htemp->GetYaxis()->SetNdivisions(409);
  TText info;
  info.SetTextAlign(32); // middle, right
  info.SetTextFont(102); // courier (mono) bold
  info.SetTextSize(0.03);
  info.SetTextColor(g->GetMarkerColor());
  info.DrawTextNDC(0.65, 0.97, htemp->GetTitle());
  htemp->SetTitle(nullptr);

  TLine line;
  line.SetLineStyle(2);
  line.SetLineWidth(1);
  line.SetLineColor(g->GetMarkerColor());
  line.DrawLine(htemp->GetXaxis()->GetXmin(), 0xFFFFFFFF,
                htemp->GetXaxis()->GetXmax(), 0xFFFFFFFF);
  TLatex latex;
  latex.SetTextAlign(12); // middle, left
  latex.SetTextSize(0.04);
  latex.SetTextColor(g->GetMarkerColor());
  latex.DrawLatex(htemp->GetXaxis()->GetXmin()-2000, 0xFFFFFFFF*1.01, "#bf{ 2^{32}} #Rightarrow");

  cc->Draw(TString::Format("GetScalerIncr(%d):GetEpochSec()", scaler),
           TString::Format("HasRun(%d)", run), "same");
  g = dynamic_cast<TGraph *>(gPad->GetListOfPrimitives()->Last()); // ?!
  if (!g) return;
  g->SetMarkerStyle(6);
  g->SetMarkerColor(kBlue+2);
  gPad->GetListOfPrimitives()->Remove(g); // no draw
  TPad *pad2 = new TPad("pad2", "", 0.00, 0.00, 1.00, 1.00);
  pad2->SetFillStyle(4000);   // transparent
  pad2->SetFrameFillStyle(0); // transparent
  pad2->Draw();
  pad2->cd();
  if (scaler == 647) g->SetMinimum(4000000); // !!! specific for scaler[647] !!!
  g->Draw("APY+");
  g->GetXaxis()->SetLimits(htemp->GetXaxis()->GetXmin(), htemp->GetXaxis()->GetXmax());
  // g->GetXaxis()->SetNoExponent();
  g->GetXaxis()->SetTimeDisplay(1); // switch (*TOGGLE*) time display
  g->GetXaxis()->SetLabelOffset(999.9); // no double label
  g->GetYaxis()->SetTitle("increment value of scaler");
  g->GetYaxis()->SetTitleColor(g->GetMarkerColor());
  g->GetYaxis()->SetLabelColor(g->GetMarkerColor());
  g->GetYaxis()->CenterTitle();
  g->GetYaxis()->SetNdivisions(409);
  g->GetYaxis()->SetTitleOffset(1.25);
  g->SetTitle(nullptr);
  info.SetTextColor(g->GetMarkerColor());
  info.DrawTextNDC(0.65, 0.93,
                   TString::Format("%s:%s {%s}", cc->GetVar(0)->GetTitle(),
                                   cc->GetVar(1)->GetTitle(), cc->GetSelect()->GetTitle()));
  info.SetTextColor(kBlack);
  info.SetTextAlign(12); // middle, left
  info.SetTextSize(0.035);
  info.DrawTextNDC(0.70, 0.95, TString::Format("file: %s", cc->GetTitle()));

  // unsetenv("TZ"); tzset();
  TCanvas *c = gPad->GetCanvas();
  c->SetWindowSize(1920/2, 1080/2); // widescreen (16:9)
  std::uint32_t w = c->GetWindowWidth(), h = c->GetWindowHeight();
  // exactly the graphics area size of a canvas
  c->SetWindowSize(w + (w - c->GetWw()), h + (h - c->GetWh()));
  gStyle->SetGridColor(kGray);
  c->SetGrid();
  c->ModifiedUpdate();
  TDatime now;
  c->Print(TString::Format("fig.scaler.example.%.10s.pdf", now.AsSQLString()));
}
