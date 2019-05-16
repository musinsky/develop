// 2007-05-25
// https://root-forum.cern.ch/t/inverted-grey-scale-pallete/4827

{
  TCanvas *c  = new TCanvas("c","Contours",0,0,1200,600);
  c->Divide(2, 1, 0.0001);
  c->cd(1);
  TH2* h = new TH2F("h","h",20,0,2,20,-2,2);

  for(int i=0; i<100000; i++) {
    double x=gRandom->Exp(1);
    double y=gRandom->Gaus(0,1);
    h->Fill(x,y);
  }

  const UInt_t Number = 2;
  Double_t Red[Number]   = { 0.00, 1.00};
  Double_t Green[Number] = { 0.00, 1.00};
  Double_t Blue[Number]  = { 0.00, 1.00};
  Double_t Stops[Number] = { 0.00, 1.00};

  Int_t nb=50;
  TColor::CreateGradientColorTable(Number,Stops,Red,Green,Blue,nb);
  h->Draw("colz");

  // 2008-07-15
  // https://root-forum.cern.ch/t/advise-on-tpalette-and-b-w-printers/6834

  c->cd(2);
  TF2 *f1 = new TF2("f2","0.1+(1-(x-2)*(x-2))*(1-(y-2)*(y-2))",1,3,1,3);

  f2->SetContour(nb);
  //  f2->Draw("surf1z");
  f2->Draw("surf2z");
}
