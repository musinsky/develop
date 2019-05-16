// 2005-09-15

// try zoom with mouse
{
  TCanvas *c1 = new TCanvas("c1");
  TH2F *h1 = new TH2F("h1","h1",40,-4,4,40,-4,4);
  TH2F *h2 = new TH2F("h2","h2",40,-4,4,40,-4,4);
  Double_t a,b;
  for (Int_t i=0;i<5000;i++) {
    gRandom->Rannor(a,b);
    h1->Fill(a-1.5,b-1.5);
    h2->Fill(a+1.5,b+1.5);
  }
  TExec *ex1 = new TExec("ex1","gStyle->SetPalette(0);");
  TExec *ex2 = new TExec("ex2","gStyle->SetPalette(1);");
  h1->Draw("colz");
  ex1->Draw();
  h1->Draw("col same");
  ex2->Draw();
  h2->Draw("col same");
}
