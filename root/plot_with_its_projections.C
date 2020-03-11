// 2020-03-06
// https://root-forum.cern.ch/t/how-to-draw-2d-scattered-plot-with-its-projections-in-same-canvas-like-the-attached-figure/38166

{
  gStyle->SetFrameFillStyle(0);
  TCanvas *c1 = new TCanvas("c1", "c1",900,900);
  gStyle->SetOptStat(0);

  TPad *h2_pad = new TPad("h2_pad", "h2_pad",0.0,0.0,1.0,1.0);
  h2_pad->Draw();

  proj1_pad = new TPad("proj1_pad", "proj1_pad",0.0,0.0,1.0,1.0);
  proj1_pad->Draw();

  proj2_pad = new TPad("proj2_pad", "proj2_pad",0.0,0.0,1.0,1.0);
  proj2_pad->Draw();

  TH2F *h2 = new TH2F("h2","",40,-4,4,40,-20,20);
  Float_t px, py;
  for (Int_t i = 0; i < 25000; i++) {
    gRandom->Rannor(px,py);
    h2->Fill(px,5*py);
  }
  TH1D * projh2X = h2->ProjectionX();
  TH1D * projh2Y = h2->ProjectionY();

  h2_pad->cd();
  h2->Draw("COL");

  proj2_pad->cd();
  proj2_pad->SetFillStyle(4000);
  projh2X->SetLineColor(kRed);
  projh2X->SetFillStyle(0);
  projh2X->SetLineWidth(5);
  projh2X->Draw("HIST Y+");

  proj1_pad->cd();
  proj1_pad->SetFillStyle(4000);
  projh2Y->SetLineWidth(0);
  projh2Y->SetFillStyle(0);
  auto g = new TGraph();
  int n = projh2Y->GetNbinsX();
  double x, y;
  double bw = projh2Y->GetBinWidth(1);
  int i = 0;
  y = projh2Y->GetBinLowEdge(1);
  g->SetPoint(i, 0, y);
  for (int b=1; b<=n; b++) {
    x = projh2Y->GetBinContent(b);
    y = projh2Y->GetBinLowEdge(b);
    g->SetPoint(i, x, y); i++;
    g->SetPoint(i, x, y+bw); i++;
  }
  projh2Y->Draw("hbar X+");
  g->SetLineColor(kYellow);
  g->SetLineWidth(5);
  g->Draw("L");
}
