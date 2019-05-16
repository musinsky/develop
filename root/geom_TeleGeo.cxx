// 2002-09-27
// https://root.cern.ch/root/roottalk/roottalk02/4133.html

{
  //
  // Make Telescope and set up the geometry
  //
  gSystem->Load("libGeom");
  TGeoManager *geom = new TGeoManager("MARIE","MARIE Geometry");
  TGeoMaterial *mat;
  TGeoMixture *mix;
  air = new TGeoMaterial("air",14.61,7.3,0.0012);
  //  mat2 = new TGeoMaterial("air",14.61,7.3,0.0012);
  //mix = new TGeoMaterial("air",14.61,7.3,0.0012);
  //  mat3 = new TGeoMaterial("air",14.61,7.3,0.0012);

  TGeoMedium *mat1 = new TGeoMedium("Air material",2,air);
  TGeoMedium *mat2 = new TGeoMedium("Air material",2,air);
  TGeoMedium *mat3 = new TGeoMedium("Air material",2,air);


  TGeoVolume *top = geom->MakeBox("TOP",mat1,800.,800.,800.);
  geom->SetTopVolume(top);

  TGeoRotation *rot1 = new TGeoRotation("rot1",0,0,45);
  TGeoCombiTrans *combi1 = new TGeoCombiTrans(6.353,6.353,0,
                                              new TGeoRotation("rot1",0,0,-45));
  TGeoCombiTrans *combi2 = new TGeoCombiTrans(6.353,6.353,8.88,
                                              new TGeoRotation("rot1",0,0,-45));
  TGeoCombiTrans *combi3 = new TGeoCombiTrans(-8.984,0,8.88,rot1);
  TGeoCombiTrans *combi4 = new TGeoCombiTrans(-8.984,0,7.597,rot1);
  TGeoCombiTrans *combi5 = new TGeoCombiTrans(-8.984,0,5.692,rot1);
  //TGeoCombiTrans *combi6 = new TGeoCombiTrans(-8.984,0,4.435,rot1);
  TGeoCombiTrans *combi6 = new TGeoCombiTrans(-8.984,0,0,rot1);
  TGeoTranslation *tr0 = new TGeoTranslation(0,0,0);
  TGeoTranslation *tr1 = new TGeoTranslation(-8.984,0,8.98);
  TGeoTranslation *tr2 = new TGeoTranslation(-8.984,0,0);
  TGeoTranslation *tr3 = new TGeoTranslation(-8.984,0,8.78);
  TGeoTranslation *tr4 = new TGeoTranslation(-8.984,0,7.597);
  TGeoTranslation *tr5 = new TGeoTranslation(-8.984,0,5.692);
  TGeoTranslation *tr6 = new TGeoTranslation(-8.984,0,4.535);
  TGeoTranslation *tr7 = new TGeoTranslation(-8.984,0,4.435);
  TGeoTranslation *tr8 = new TGeoTranslation(-8.984,0,4.335);
  TGeoTranslation *tr9 = new TGeoTranslation(-8.984,0,3.31);
  TGeoTranslation *tr10 = new TGeoTranslation(-8.984,0,2.652);
  TGeoTranslation *tr11 = new TGeoTranslation(-8.984,0,1.913);
  TGeoTranslation *tr12 = new TGeoTranslation(-8.984,0,1.255);

  TGeoVolume *marie = geom->MakeBox("MARIE",mat1,25.,25.,25.);
  marie->SetVisibility(kFALSE);
  //marie->SetLineColor(kBlue);
  TGeoVolume *Housing = geom->MakeBox("Housing",mat3,15.,6.,11.);
  Housing->SetVisibility(kFALSE);
  TGeoVolume *OuterHousing = geom->MakeBox("Outer",mat3,14.7,5.1,10.15);
  OuterHousing->SetLineColor(kGreen);
  //OuterHousing->SetVisibility(kFALSE);
  TGeoVolume *InnerHousing = geom->MakeBox("Inner",mat1,14.5095,4.9095,9.9595);
  InnerHousing->SetLineColor(kGreen);
  InnerHousing->SetVisibility(kTRUE);
  TGeoVolume *Brass = geom->MakeTube("Brass",mat3,0,1.8,0.00635);
  Brass->SetLineColor(kYellow);
  TGeoVolume *B1 = geom->MakeTube("B1",mat2,0,3.175,0.25);
  B1->SetLineColor(kBlack);
  TGeoVolume *B2 = geom->MakeTube("B2",mat2,0,3.175,0.25);
  B2->SetLineColor(kBlack);
  TGeoVolume *B3 = geom->MakeTube("B3",mat2,0,3.175,0.25);
  B3->SetLineColor(kBlack);
  TGeoVolume *B4 = geom->MakeTube("B4",mat2,0,3.175,0.25);
  B4->SetLineColor(kBlack);
  TGeoVolume *A1 = geom->MakeBox("A1",mat2,1.27,1.27,0.05);
  A1->SetLineColor(kBlack);
  Double_t orig[3] = {0,0,4.445};
  TGeoBBox *A = new TGeoBBox(1.27,1.27,0.05,orig);
  TGeoVolume *A2 = new TGeoVolume("A2",A,mat2);
  //TGeoVolume *A2 = geom->MakeBox("A2","mat2",1.27,1.27,0.05);
  A2->SetLineColor(kBlack);
  TGeoVolume *PSD1 = geom->MakeBox("PSD1",mat2,1.27,1.27,0.01575);
  PSD1->SetLineColor(kBlack);
  TGeoVolume *PSD2 = geom->MakeBox("PSD2",mat2,1.27,1.27,0.01575);
  PSD2->SetLineColor(kBlack);

  InnerHousing->AddNode(Brass,1,tr1);
  InnerHousing->AddNode(A1,1,combi3);
  InnerHousing->AddNode(Brass,2,tr3);
  InnerHousing->AddNode(PSD1,1,combi4);
  InnerHousing->AddNode(PSD2,1,combi5);
  InnerHousing->AddNode(Brass,3,tr6);
  InnerHousing->AddNode(A2,1,combi6);
  InnerHousing->AddNode(Brass,4,tr8);
  InnerHousing->AddNode(B1,1,tr9);
  InnerHousing->AddNode(B2,1,tr10);
  InnerHousing->AddNode(B3,1,tr11);
  InnerHousing->AddNode(B4,1,tr12);

  OuterHousing->AddNode(InnerHousing,1,tr0);
  marie->AddNode(OuterHousing,1,tr0);
  top->AddNode(marie,1,tr0);

  // add from another macro file
  geom->CloseGeometry();
  geom->SetVisLevel(4);
  top->Draw();
  Double_t point[3], dircos[3];
  point[0] = -8.984;
  point[1] = 0.0;
  point[2] = 7.8;
  Double_t theta = 180.;
  Double_t phi = 0.;
  Double_t degrad = TMath::Pi()/180.;
  // better this way
  dircos[0] = TMath::Sin(theta*degrad)*TMath::Cos(phi*degrad);
  dircos[1] = TMath::Sin(theta*degrad)*TMath::Sin(phi*degrad);
  dircos[2] = TMath::Cos(theta*degrad);
  printf("direction : %f %f %f\n", dircos[0], dircos[1], dircos[2]);
  geom->InitTrack(point,dircos);
  while (!geom->IsOutside()) {   // you should use this to propagate till outside
    printf("current : %s\n", geom->GetPath());
    geom->FindNextBoundary();
    cout<<"Distance to Boundary is "<<geom->GetStep()<<" cm."<<endl;
    geom->Step();
  }
}
