// 2005-07-06
// https://root-forum.cern.ch/t/fitting-a-graph/2113

{
  gROOT->ProcessLine(".L BetheBlock.h");

  const Int_t Np = 38;
  Float_t x[Np] =
    {0.51,  1.08,  2.14,  3.09,  4.03,  4.99,  5.81,  6.62,  7.45,  8.25,
     9.07,  9.90, 10.69, 11.48, 12.32, 13.14, 13.96, 14.80, 15.59, 16.38,
     17.17, 17.95, 18.71, 19.46, 20.25, 21.04, 21.83, 22.62, 23.41, 24.17,
     24.92, 25.71, 26.50, 27.23, 28.02, 28.81, 29.58, 29.97};
  Float_t y[Np] =
    {6.45,  9.95,  7.97,  8.47,  7.94,  6.34,  7.13,  6.56,  7.48,  6.86,
     6.43,  5.20,  6.63,  5.73,  6.15,  6.69,  5.78,  4.65,  5.55,  5.70,
     5.66,  5.12,  4.87,  6.22,  5.64,  4.98,  5.72,  5.08,  5.12,  4.81,
     5.07,  4.99,  5.75,  4.60,  4.88,  4.68,  5.05,  4.08};

  Float_t yp[Np],ym[Np],yk[Np],ypi[Np];

  for(int i=0;i<Np;i++)
    {
      yp[i] = BetheBlochRange(x[i],938.27);
      ym[i] = BetheBlochRange(x[i],105.65);
      yk[i] = BetheBlochRange(x[i],493.68);
      ypi[i] = BetheBlochRange(x[i],139.57);
    }

  // Plot the data
  TGraph *gr = new TGraph(Np,x,y);
  gr->Draw("A*");

  gr->SetTitle("dE/dX Vs Range");
  gr->GetXaxis()->SetTitle("dE/dX Vs Range");
  gr->GetYaxis()->SetTitle("dE/dX (MeV/cm)");
  gr->GetXaxis()->SetTitle("Range (cm)");
  gr->GetYaxis()->SetLabelSize(0.02);
  gr->GetXaxis()->SetLabelSize(0.02);

  // Print theoretical line for protons
  TGraph *grp = new TGraph(Np,x,yp);
  grp->SetLineColor(kMagenta);
  grp->SetLineWidth(2);
  grp->Draw("C");

  // Print theoretical line for muons
  TGraph *grm = new TGraph(Np,x,ym);
  grm->SetLineColor(kBlue);
  grm->SetLineWidth(2);
  grm->Draw("C");

  // Print theoretical line for Kaons
  TGraph *grk = new TGraph(Np,x,yk);
  grk->SetLineColor(kCyan);
  grk->SetLineWidth(2);
  grk->Draw("C");

  // Print theoretical line for pions
  TGraph *grpi = new TGraph(Np,x,ypi);
  grpi->SetLineColor(kGreen);
  grpi->SetLineWidth(2);
  grpi->Draw("C");

  // Fitting the data
  gStyle->SetOptFit(1111);

  // TF1 *fitFcn = new TF1("user",dEdX_Range,x[0],x[Np-1],1);
  TF1 *fitFcn = new TF1("user", "dEdX_Range", x[0], x[Np-1], 1);
  fitFcn->SetLineWidth(4);
  fitFcn->SetLineStyle(4);
  fitFcn->SetLineColor(kRed);
  fitFcn->SetParameter(0,900.);
  fitFcn->SetParName(0,"Mass");

  gr->Fit("user","M");

  // The Legend
  TLegend *leg = new TLegend(0.7,0.6,0.9,0.8);
  leg->AddEntry(gr,"Data","p");
  leg->AddEntry(fitFcn,"Fitted Data","l");
  leg->AddEntry(grp,"Proton","l");
  leg->AddEntry(grm,"Muon","l");
  leg->AddEntry(grk,"Kaon","l");
  leg->AddEntry(grpi,"Pion","l");
  leg->Draw();
}
