// 2008-07-15
// https://root-forum.cern.ch/t/fitting-in-thnsparse/6833

#include "THnSparse.h"
#include "TMath.h"
#include "TAxis.h"
#include "TF3.h"
#include "TVirtualFitter.h"

THnSparse *h;
TF3 *f3;
Double_t gaus3(Double_t *x, Double_t *par)
{
  Double_t xx = TMath::Gaus(x[0], par[1], par[2]);
  Double_t yy = TMath::Gaus(x[1], par[3], par[4]);
  Double_t zz = TMath::Gaus(x[2], par[5], par[6]);
  return par[0]*xx*yy*zz;
}

void fcnsparse(Int_t &, Double_t *, Double_t &f, Double_t *par, Int_t iflag)
{
  static Int_t nt = 0;
  static Double_t xbins[100], ybins[100], zbins[100];
  f = 0;
  TAxis *xaxis = h->GetAxis(0);
  TAxis *yaxis = h->GetAxis(1);
  TAxis *zaxis = h->GetAxis(2);
  Int_t nx = xaxis->GetNbins();
  Int_t ny = yaxis->GetNbins();
  Int_t nz = zaxis->GetNbins();
  if (nt == 0) {
    for (Int_t bin = 1;bin <= nx;bin++) {
      xbins[bin] = xaxis->GetBinCenter(bin);
      ybins[bin] = yaxis->GetBinCenter(bin);
      zbins[bin] = zaxis->GetBinCenter(bin);
    }
  }
  Double_t x[3];
  Int_t idx[3];
  for (Int_t i = 1;i <= nx;i++) {
    x[0] = xbins[i];
    idx[0] = i;
    for (Int_t j = 1;j <= ny;j++) {
      x[1] = ybins[j];
      idx[1] = j;
      for (Int_t k = 1;k <= nz;k++) {
        idx[2] = k;
        Double_t e = h->GetBinError(idx);
        if (e <= 0) continue;
        x[2] = zbins[k];
        Double_t c = h->GetBinContent(idx);
        Double_t func = f3->EvalPar(x, par);
        Double_t df = (c - func) / e;
        f += df * df;
      }
    }
  }
  nt++;
  printf("nt = %5d, f = %g\n", nt, f);
}
void sparsefit()
{
  Int_t nbins[3] = {30, 30, 30};
  Double_t xmin[3] = { -3, -3, -3};
  Double_t xmax[3] = {3, 3, 3};
  h = new THnSparseF("h", "fit sparse", 3, nbins, xmin, xmax);
  f3 = new TF3("f3", gaus3, -3, 3, -3, 3, -3, 3, 7);
  f3->SetParameters(1, -0.5, 0.7, 0, 0.9, 0.5, 1.1);
  Double_t x[3];
  for (Int_t i = 0;i < 500000;i++) {
    f3->GetRandom3(x[0], x[1], x[2]);
    h->Fill(x);
  }
  printf("now fitting\n");
  TVirtualFitter *minuit = TVirtualFitter::Fitter(0, 5);
  minuit->SetFCN(fcnsparse);
  minuit->SetParameter(0, "constant", 1,   100,  0, 1000);
  minuit->SetParameter(1, "meanx",    0,     2, -1, 1);
  minuit->SetParameter(2, "rmsx",     1,   0.5, 0.5, 1.5);
  minuit->SetParameter(3, "meany",    0,     2, -1, 1);
  minuit->SetParameter(4, "rmsy",     1,   0.5, 0.5, 1.5);
  minuit->SetParameter(5, "meanz",    0,     2, -1, 1);
  minuit->SetParameter(6, "rmsz",     1,   0.5, 0.5, 1.5);
  Double_t arglist[10];
  arglist[0] = 1;
  minuit->ExecuteCommand("SET PRINT", arglist, 1);
  minuit->ExecuteCommand("MIGRAD", arglist, 0);
}
