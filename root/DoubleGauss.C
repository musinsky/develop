// 2007-06-08
// https://root-forum.cern.ch/t/how-to-fit-a-unsymmetrical-gaus-distributing/4864

Double_t mydgauss(Double_t *x, Double_t *par) {
  Double_t arg1 =(x[0]-par[1])/par[2];
  Double_t arg2 =(x[0]-par[1])/par[3];
  Double_t r1 = TMath::Exp(-arg1*arg1);
  Double_t r2 = TMath::Exp(-arg2*arg2);
  return par[0]*(r1+r2);
}
void DoubleGauss() {
  TF1 *f1 = new TF1("f1",mydgauss,-3,5,4);
  f1->SetParameters(1,0,1,2);
  TH1F *h1 = new TH1F("h1","double gauss",100,-3,5);
  h1->FillRandom("f1",10000);
  h1->Fit(f1);
  h1->Draw();
}
