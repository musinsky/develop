// 2008-06-03
// https://root-forum.cern.ch/t/multiple-tgraph-or-th1-in-perspective/6613

// Author: Jan Musinsky
// 01/06/2008

#include <TH1.h>
#include <TF1.h>
#include <TROOT.h>
#include <TRandom.h>
#include <TCanvas.h>
#include <TColor.h>

void MakeHistos(TList *hlist, Int_t nh)
{
  Double_t min = -10, max = 10, mean;
  TH1F *h;
  TF1 *g = (TF1 *)gROOT->GetFunction("gaus");
  for (Int_t i = 0; i < nh; i++) {
    h = new TH1F(Form("h_%d", i), "", 100, min*1.2, max*1.2);
    hlist->Add(h);

    // first peak
    g->SetParameters(1, 0, 1);
    h->FillRandom("gaus", 100000);

    // second peak
    mean = min + (max - min)/(nh - 1)*i;
    g->SetParameters(1, mean, 2);
    h->FillRandom("gaus", h->GetEntries()/1.5);
  }
}

void DrawHistos(TList *hlist, Double_t xsize, Double_t ysize)
{
  TCanvas *c = new TCanvas("c_per", " perspective histograms");
  c->SetFillColor(kWhite);

  Int_t nh = hlist->GetSize();
  Double_t xdel = (1.0 - xsize)/nh, ydel = (1.0 - ysize)/nh;
  TPad *pad;
  TH1F *h;
  //  for (Int_t i = 0; i < nh; i++) {
  for (Int_t i = nh-1; i >= 0; i--) {
    // make pad
    c->cd(0);
    pad = new TPad(Form("pad_%d", i + 1), "", i*xdel, i*ydel,
                   (i + 1)*xdel + xsize, (i + 1)*ydel + ysize);
    pad->SetNumber(i + 1);
    pad->SetFrameFillStyle(4000);
    pad->SetFillStyle(4000);
    pad->SetFrameLineColor(kWhite);
    pad->SetFrameLineWidth(0);
    pad->SetFrameBorderMode(0);
    pad->SetFrameBorderSize(0);
    pad->Draw();
    pad->cd();

    // draw histo
    h = (TH1F *)hlist->FindObject(Form("h_%d", i));
    //    h->SetFillStyle(3001);
    Int_t c1 = 50, c2 = 200, c0 = (c2 - c1)/nh;
    h->SetFillColor(TColor::GetColor(c1 + i*c0, 0, c2 - i*c0));
    h->SetStats(0);
    if (i != 0) {
      h->GetXaxis()->SetLabelOffset(9999);
      h->GetYaxis()->SetLabelOffset(9999);
    }
    h->Draw();
  }
  c->cd(0);
  //  c->Print(Form("%s.eps", c->GetName()));
}

void hperspective()
{
  TList *histograms = new TList();
  MakeHistos(histograms, 7);
  DrawHistos(histograms, 0.35, 0.20);
}
