// 2006-04-04

{
  gROOT->Reset();
  Float_t mass_p = 0.93826;
  Float_t mass_d = 1.87549;
  TVector3 l3(0, 0,  3.31702);
  TVector3 a3(0, 0, -1.65942);
  TVector3 c3(0, 0,  0.91582);

  TVector3 p3(-0.14108, -0.24591, 0.58972);
  TLorentzVector proton;
  proton.SetVectM(p3, mass_p);
  // in CMS   -0.29245
  // in ALAB  -0.82493

  TLorentzVector lab;
  lab.SetVectM(l3, mass_d);
  //  TLorentzVector alab;
  //  alab.SetVectM(a3, mass_d);
  //  TLorentzVector cms;
  //  cms.SetVectM(c3, mass_d);

  TLorentzVector target(0, 0, 0, mass_p);
  TLorentzVector cms =  target + lab;

  lab.Boost(-cms.BoostVector());    // CMS
  proton.Boost(-cms.BoostVector()); // CMS

  //  lab.SetVectM(l3, mass_d);
  //  cms = lab;
  //  target.Boost(-cms.BoostVector()); // ALAB
  //  proton.Boost(-cms.BoostVector()); // ALAB
}
