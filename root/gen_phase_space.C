// 2005-01-25

//  An Example how to generate reaction
//      pp->pp (eta->2gamma) in root
//
{
  // example of use of TGenPhaseSpace
  gSystem->Load("libPhysics.so");
  Double_t fMEta=0.547;
  Double_t fEkin=1.36;
  Double_t fMProt=0.938;
  Double_t fMGamma=0.0;
  Double_t fPmom=TMath::Sqrt(fEkin*(fEkin+2*fMProt));

  TLorentzVector target(0.0, 0.0, 0.0, fMProt);
  TLorentzVector beam(0.0, 0.0, fPmom,fEkin+fMProt);
  TLorentzVector W = beam + target;

  Double_t masses[3] = { fMProt,fMProt ,fMEta} ;
  Double_t mas1[2] = { fMGamma ,fMGamma } ;
  Double_t pi = TMath::Pi();
  TGenPhaseSpace prod;
  prod.SetDecay(W, 3, masses);
  TGenPhaseSpace decay;

  TH2F hh("hh","#theta #gamma1 vs #theta #gamma2 LAB",100,0,pi,100,0,pi);
  TH1F hg("hg","#theta #eta",100,0,pi);
  for (Int_t n=0;n<100000;n++) {
    //generate production reaction
    Double_t weight = prod.Generate();

    TLorentzVector *pProt1  = prod.GetDecay(0);

    TLorentzVector *pProt2  = prod.GetDecay(1);
    TLorentzVector *pEta    = prod.GetDecay(2);
    hg.Fill(pEta->Theta() ,weight);
    //generate dacay reaction
    decay.SetDecay(*pEta, 2, mas1);
    weight *= decay.Generate();
    TLorentzVector *pG1 = decay.GetDecay(0);
    TLorentzVector *pG2 = decay.GetDecay(1);

    hh.Fill(pG1->Theta() ,pG2->Theta() ,weight);
  }
  //correlation plot between two gammas
  hh.Draw("col");
}
