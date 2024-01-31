// 2006-05-11

{
  gROOT->Reset();
  // n + p -> n + p
  Float_t mass_p = 0.93827;
  Float_t mass_n = 0.93956;

  TVector3 beam3(0.0, 0.0, 1.680);
  TLorentzVector neuteron;
  neuteron.SetVectM(beam3, mass_n);
  TVector3 target3(0.0, 0.0, 0.0);
  TLorentzVector proton;
  proton.SetVectM(target3, mass_p);

  TLorentzVector cms = neuteron + proton;
  //  neuteron.Boost(-cms.BoostVector());
  //  Printf("impuls of neuteron in CMS = %g",neuteron[2]);
  proton.Boost(-cms.BoostVector()); // CMS
  Printf("impuls of proton in CMS = %g",proton[2]);
  Printf("pi / p*p in CMS = %g",TMath::Pi()/(proton[2]*proton[2]));
}
