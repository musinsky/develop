// 2019-05-29
// https://root-forum.cern.ch/t/custom-palette-color/34313

{
  TH2F *h1 = new TH2F("h1","h1",100,-4,4,100,-4,4);
  h1->SetStats(0);

  Double_t a, b;
  for (Int_t i = 0; i < 50000; i++) {
    gRandom->Rannor(a,b);
    h1->Fill(a-1.5,b-1.5,0.08);
  }

  Double_t max       = h1->GetMaximum();
  Double_t min       = h1->GetMinimum();
  Double_t val_white = 0.5;
  Double_t per_white = (val_white-min)/(max-min);

  const Int_t Number = 3;
  Double_t Red[Number]   = { 0., 1., 1.};
  Double_t Green[Number] = { 0., 1., 0.};
  Double_t Blue[Number]  = { 1., 1., 0.};
  Double_t Stops[Number] = { 0., per_white, 1. };

  Double_t val_cut   = 0.5;
  Double_t per_cut   = (val_cut-min)/(max-min);

  const Int_t Number2 = 4;
  Double_t Red2[Number2]   = { 0., 1., 0., 1.};
  Double_t Green2[Number2] = { 0., 0., 1., 0.};
  Double_t Blue2[Number2]  = { 1., 1., 0., 0.};
  Double_t Stops2[Number2] = { 0., per_cut, per_cut+0.0001, 1. };

  Int_t nb = 256;
  h1->SetContour(nb);

  //  TColor::CreateGradientColorTable(Number,Stops,Red,Green,Blue,nb);
  TColor::CreateGradientColorTable(Number2,Stops2,Red2,Green2,Blue2,nb);
  h1->Draw("colz");
}
