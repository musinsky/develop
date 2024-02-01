// 2005-01-28 original
// 2019-05-13 update to ROOT6
// Musinsky Jan

{
  gStyle->SetOptStat(111111);

  const UInt_t ne = 300000; // distribution
  const UInt_t nb = 100000; // background
  Float_t a[ne+nb];

  // pomocou tejto funkcie si dole vygenerujem histogram
  TF1 *f_pol6 = new TF1("f_pol6","pol6");
  f_pol6->SetParameters(-9.93401e+06, 248902, -2587.79, 14.2902, -0.0442101,
                        7.26706e-05, -4.95998e-08);
  f_pol6->SetRange(190, 300);

  // generujem histogram s danym rozdelenim + background
  // !!! pozor na bining !!! (-10,400 => musi byt 410, inak zle normuje)
  TH1F *h_pol6 = new TH1F("h_pol6","Monte Carlo distribution",410,-10,400);
  for (UInt_t i=0; i<ne; i++) {
    a[i] = f_pol6->GetRandom()-190; // zaciname s T0 !!!
    h_pol6->Fill(a[i]);
  }
  for (UInt_t i=ne; i<(ne+nb); i++) {
    a[i] = gRandom->Rndm(i)*600-100; // (-100,500)
    h_pol6->Fill(a[i]);
  }
  h_pol6->Draw();

  // 1111111111111111111111
  // fitnem nas histogram polynomom 6 stupna
  // predpokladam, ze viem kde je T0 a Tmax
  Float_t tmin = 0;
  Float_t tmax = 110;

  h_pol6->Fit("pol6","","",tmin,tmax);
  h_pol6->GetFunction("pol6")->SetLineColor(kRed);
  Float_t p0 = pol6->GetParameter(0);
  Float_t p1 = pol6->GetParameter(1);
  Float_t p2 = pol6->GetParameter(2);
  Float_t p3 = pol6->GetParameter(3);
  Float_t p4 = pol6->GetParameter(4);
  Float_t p5 = pol6->GetParameter(5);
  Float_t p6 = pol6->GetParameter(6);

  // teraz "analiticky integrujem" funkciu pol6
  // a prepocitam kazdu funkcnu hodnotu takejto funkcie
  // a zaplnim vysledny histogram
  TH1F *h_r_1 = new TH1F("h_r_1","Final uniform distribution",1000,-0.1,0.9);

  Int_t bmin = h_pol6->FindBin(tmin);
  Int_t bmax = h_pol6->FindBin(tmax);
  Double_t sum = h_pol6->Integral(bmin,bmax);
  Double_t k = 0.75 / sum; // !!! zavisi od nbins h_pol6 !!!

  for (UInt_t i=0; i<(ne+nb); i++) {
    if ( (a[i]<tmin) || (a[i]>tmax) ) continue;
    //    Float_t r = p0*a[i]+p1*((a[i]**2)/2)+p2*((a[i]**3)/3)+
    //      p3*((a[i]**4)/4)+p4*((a[i]**5)/5)+p5*((a[i]**6)/6)+p6*((a[i]**7)/7);
    Float_t r = p0*a[i]            + p1*(TMath::Power(a[i], 2)/2) +
      p2*(TMath::Power(a[i], 3)/3) + p3*(TMath::Power(a[i], 4)/4) +
      p4*(TMath::Power(a[i], 5)/5) + p5*(TMath::Power(a[i], 6)/6) +
      p6*(TMath::Power(a[i], 7)/7);
    r = r*k;
    h_r_1->Fill(r);
  }
  h_r_1->Draw();

  // 2222222222222222222222
  // zintegrujem, zosumujem histogram

  TH1F *h_int6 = new TH1F("h_int6","Summed Monte Carlo",410,-10,400);
  sum = 0.0;
  for (UInt_t i=1; i<410; i++) {
    sum += h_pol6->GetBinContent(i);
    h_int6->SetBinContent(i,sum);
  }
  h_int6->Draw();

  // teraz takyto zosumovany integral fitnem
  // a v podstate uz mam hotovo funkciu (zintegrovanu) !!!
  // nemusim uz analyticky integrovat (ako vyssi pripad)
  // a takouto funkciou znova zaplnim vysledny histogram

  tmin = 0;
  tmax = 110;

  h_int6->Fit("pol4","","",tmin,tmax);
  h_int6->GetFunction("pol4")->SetLineColor(kBlue);
  p0 = pol4->GetParameter(0);
  p1 = pol4->GetParameter(1);
  p2 = pol4->GetParameter(2);
  p3 = pol4->GetParameter(3);
  p4 = pol4->GetParameter(4);

  TH1F *h_r_2 = new TH1F("h_r_2","Final uniform distribution",1000,-0.1,0.9);
  k = 0.75 / pol4->Eval(tmax);

  for (UInt_t i=0; i<(ne+nb); i++) {
    if ( (a[i]<tmin) || (a[i]>tmax) ) continue;
    //    Float_t r = p0 + p1*a[i] + p2*a[i]**2 + p3*a[i]**3 + p4*a[i]**4;
    Float_t r = p0 + p1*a[i]   + p2*TMath::Power(a[i], 2) +
      p3*TMath::Power(a[i], 3) + p4*TMath::Power(a[i], 4);
    r = r*k;
    h_r_2->Fill(r);
  }
  h_r_2->Draw();
}

// from JLab
// this bit of code is for 85% Ar 15% CO2
// uses the x-t relation of Mark Jones derived from a Garfield simulation
// if (time > 2.0)
//   TF1 *f_jlab1 = new TF1("f_jlab1", "0.01868 + 0.00518*x + 0.000015775*x**2 - 0.00000018735*x**3", 0, 110);
// below is the result for integral method (EJB)
// a single fit for all planes was used
// if ( (time >  0.0) && (time <= 110.0) )
//   TF1 *f_jlab2 = new TF1("f_jlab2", "15.199 - 0.32085*x + 2.9064*x**2 - 0.059774*x**3 + 0.00054390*x**4 - 0.0000018513*x**5", 0, 110);
// dist=dist*0.522/5350.0
