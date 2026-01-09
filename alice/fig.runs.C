// 2026-01-09

void runs(const char *fn, int nScl)
{
  TFile *f = TFile::Open(fn, "READ");
  if (!f) return;
  TTree *cc = f->Get<TTree>("cc");
  if (!cc) return;

  // setenv("TZ", "Asia/Tokyo", 1); tzset(); // only for testing
  setenv("TZ", "Europe/Zurich", 1); tzset(); // set CERN time as local time
  cc->GetEntry(0);
  TTimeStamp tsFirst(cc->GetLeaf("fEpoch")->GetValueLong64()/1000000000, kFALSE, 0);
  cc->GetEntry(cc->GetEntries() - 1);
  TTimeStamp  tsLast(cc->GetLeaf("fEpoch")->GetValueLong64()/1000000000, kFALSE, 0);

  cc->Draw(TString::Format("GetScaler(%d):(GetEpochSec()-%ld)", 0, tsFirst.GetSec()),
           TString::Format("GetScaler(%d)!=0", 0), "");
  TGraph *g = dynamic_cast<TGraph *>(gPad->FindObject("Graph"));
  // TPad::GetPrimitive() is obsolete, use TPad::FindObject() instead
  if (!g) return;
  g->SetMarkerStyle(7);
  g->SetMarkerColor(kBlue+2);
  TH2F *htemp = dynamic_cast<TH2F *>(gPad->FindObject("htemp"));
  if (!htemp) return;
  htemp->GetXaxis()->SetLimits(0, tsLast.GetSec() - tsFirst.GetSec());
  htemp->GetXaxis()->SetTimeDisplay(1); // switch (*TOGGLE*) time display
  htemp->GetXaxis()->SetTimeOffset(tsFirst.GetSec());
  htemp->GetXaxis()->SetTimeFormat("%H:%M");
  htemp->GetXaxis()->SetTitle("time, H:M");
  htemp->GetYaxis()->SetTitle("scaler, run number");
  htemp->GetYaxis()->CenterTitle();
  htemp->GetYaxis()->SetLabelSize(0.0325);
  htemp->GetYaxis()->SetNoExponent();
  htemp->GetYaxis()->SetNdivisions(214);

  htemp->SetTitle(nullptr);
  TString tmp1, tmp2;
  tmp1 = tsFirst.AsString("l");
  tmp1.Remove(tmp1.Index(") +") + 1);
  tmp2.Form("first epoch: %ld = %s", tsFirst.GetSec(), tmp1.Data());
  TText info;
  info.SetTextAlign(12); // middle, left
  info.SetTextFont(102); // courier (mono) bold
  info.SetTextColor(kBlack);
  info.SetTextSize(0.025);
  info.DrawTextNDC(0.15, 0.97, tmp2.Data());
  tmp1 = tsLast.AsString("l");
  tmp1.Remove(tmp1.Index(") +") + 1);
  tmp2.Form("last  epoch: %ld = %s", tsLast.GetSec(), tmp1.Data());
  info.DrawTextNDC(0.15, 0.93, tmp2.Data());
  tmp2.Form("file: %s", cc->GetTitle());
  info.SetTextSize(0.035);
  info.DrawTextNDC(0.75, 0.95, tmp2.Data());

  info.SetTextSize(0.035);
  info.SetTextColor(g->GetMarkerColor());
  info.DrawTextNDC(0.80, 0.17 + (0.80/16*0), "scaler[0]"); // max. 16 run scalers

  if (nScl > 0) {
    cc->Draw(TString::Format("GetScaler(%d):(GetEpochSec()-%ld)", 1, tsFirst.GetSec()),
             TString::Format("GetScaler(%d)!=0", 1), "same");
    g = dynamic_cast<TGraph *>(gPad->GetListOfPrimitives()->Last()); // ?!
    if (!g) return;
    g->SetMarkerStyle(7);
    g->SetMarkerColor(kRed+2);

    info.SetTextColor(g->GetMarkerColor());
    info.DrawTextNDC(0.80, 0.17 + (0.80/16*1), "scaler[1]"); // max. 16 run scalers
  }

  // unsetenv("TZ"); tzset();
  TCanvas *c = gPad->GetCanvas();
  c->SetWindowSize(1920/2, 1080/2); // widescreen (16:9)
  std::uint32_t w = c->GetWindowWidth(), h = c->GetWindowHeight();
  // exactly the graphics area size of a canvas
  c->SetWindowSize(w + (w - c->GetWw()), h + (h - c->GetWh()));
  gStyle->SetGridColor(kGray);
  c->SetGrid();
  c->SetLeftMargin(0.10 + 0.01);
  c->SetRightMargin(0.10 - 0.05);
  c->ModifiedUpdate();
  c->ModifiedUpdate(); // ?! 2x
  TDatime now;
  c->Print(TString::Format("fig.runs.%s.%.10s.pdf", cc->GetTitle(), now.AsSQLString()));
}

void fig_runs()
{
  runs("20241116.cc.root", 0);
  runs("20250726.cc.root", 1);
}
