// 2010-06-16

{
  Double_t Red[2]   = { 1.0, 0.0};
  Double_t Green[2] = { 0.0, 0.0};
  Double_t Blue[2]  = { 0.0, 1.0};
  Double_t Stops[2] = { 0.0, 1.0};
  Int_t MyPalette[5];
  Int_t FI = TColor::CreateGradientColorTable(2, Stops, Red,Green,Blue, 15);
  Int_t ccc = 0;
  for (int i = 5; i < 10; i++)
    MyPalette[ccc++] = FI+i;

  //  Int_t FI = TColor::CreateGradientColorTable(2, Stops, Red,Green,Blue, 5);
  //  for (int i = 0; i < 5; i++) MyPalette[i] = FI+i;

  h1 = new TH1F("h1", "", 100, -4, 12);
  h2 = new TH1F("h2", "", 100, -4, 12);
  h3 = new TH1F("h3", "", 100, -4, 12);
  h4 = new TH1F("h4", "", 100, -4, 12);
  h5 = new TH1F("h5", "", 100, -4, 12);

  h1->FillRandom("gaus");
  gaus = (TF1 *)gROOT->FindObject("gaus");
  gaus->SetParameter(1,2);
  h2->FillRandom("gaus");
  gaus->SetParameter(1,4);
  h3->FillRandom("gaus");
  gaus->SetParameter(1,6);
  h4->FillRandom("gaus");
  gaus->SetParameter(1,8);
  h5->FillRandom("gaus");

  h1->SetFillColor(MyPalette[0]);
  h2->SetFillColor(MyPalette[1]);
  h3->SetFillColor(MyPalette[2]);
  h4->SetFillColor(MyPalette[3]);
  h5->SetFillColor(MyPalette[4]);

  h1->Draw();
  h2->Draw("same"); h3->Draw("same"); h4->Draw("same"); h5->Draw("same");
}
