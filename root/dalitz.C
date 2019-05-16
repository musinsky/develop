// 2003-11-20
// https://root-forum.cern.ch/t/tgenphasespace-bug/234

{
  // Dalitz plot for B+ -> K+KsKs
  gStyle->SetPalette(1,0);
  gSystem->Load("libPhysics");
  TLorentzVector P(0,0,0,5.279);
  TLorentzVector *p1, *p2, *p3;
  TLorentzVector p12, p13, p23;

  Int_t n=3;
  Double_t m[3] = {0.493677,0.497672,0.497672};
  Double_t m12,m13,m23;

  TGenPhaseSpace S;
  S.SetDecay(P,n,m,"default");

  TH2F* dalitz = new TH2F("dalitz","Dalitz plot",100,0,25,100,0,25);
  Double_t w;
  for(int i=0;i<500000;i++) {
    w = S.Generate();
    p1 = S.GetDecay(0);
    p2 = S.GetDecay(1);
    p3 = S.GetDecay(2);
    p12 = *p1 + *p2;
    p13 = *p1 + *p3;
    p23 = *p2 + *p3;

    m12=p12.M2();
    m13=p13.M2();
    m23=p23.M2();

    dalitz->Fill(m12,m13,w);

    //cout<<w<<" ; "<<S.GetWtMax() <<endl;
  }
  dalitz->Draw("COLZ");
}
