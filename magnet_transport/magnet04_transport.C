// 2005-11-07 original
// 2019-05-14 update to ROOT6
// Musinsky Jan

{
  TGeoManager *geom = new TGeoManager("world","");
  TGeoMaterial *mat = new TGeoMaterial("vac",0,0,0);
  TGeoMedium *med = new TGeoMedium("med",1,mat);

  TGeoVolume *top = geom->MakeBox("TOP",med,250,50,500);
  geom->SetTopVolume(top);

  TGeoVolume *magnet = geom->MakeBox ("magnet",med,50,30,151.0/2.0);
  TGeoTranslation *tMagnet = new TGeoTranslation(0,0,50+151.0/2.0);

  top->AddNode(magnet,1,tMagnet);
  geom->CloseGeometry();

  top->Draw();

  //---------------------------------------
  Bool_t back = kFALSE;

  TVector3 p(0.08843, 0.13416, 3.48055);
  Double_t point[3] = { 2.80654, 4.04591, 0.0};
  //TVector3 p(-0.2953857,0.13416,3.46921); back = kTRUE;
  //Double_t point[3] = {-1.6899549,9.8699561,151};

  //  TVector3 p(-0.05533,-0.15023, 3.48078);
  //  Double_t point[3] = {-1.22906,-5.51242, 0.0};
  //  TVector3 p(-0.4391937,-0.15023,3.453848); back = kTRUE;
  //  Double_t point[3] = {-11.996672,-12.048613,151.0};

  //  TVector3 p( 0.04026, 0.02749, 3.49812);
  //  Double_t point[3] = {-0.10443, 0.90350, 0.0};
  //  TVector3 p(-0.3435641,0.02749,3.481604); back = kTRUE;
  //  Double_t point[3] = {-6.6659448,2.0917733,151.0};

  //  TVector3 p(-0.07467, 0.10899, 3.48660);
  //  Double_t point[3] = {-4.27989, 5.37731, 0.0};
  //  TVector3 p(-0.4585465,0.10899,3.457647); back = kTRUE;
  //  Double_t point[3] = {-15.873853,10.112736,151.0};

  //  TVector3 p(-0.18530,-0.06083, 3.47090);
  //  Double_t point[3] = {-6.68293,-0.61275, 0.0 };
  //  TVector3 p(-0.5693104,-0.06083,3.430155); back = kTRUE;
  //  Double_t point[3] = {-23.192834,-3.2724869,151.0};

  point[2] = point[2] + 50.0; // + real geometry

  TVector3 mag_field(0.0,0.847845,0.0);
  Double_t tx = p.Px()/p.Pz();
  Double_t ty = p.Py()/p.Pz();
  Double_t ptran = p.Perp(mag_field);
  Double_t c = TMath::C()*10e-12;
  Double_t curvature =  c*mag_field.y()*1.0/ptran;
  //  Double_t curvature = c*mag_field.y()*1.0/p.Mag();
  Double_t radius = 1.0/curvature;
  Double_t length = 151.0;
  if (back) length = -length;

  Double_t z_c, x_c, new_point[3];
  Double_t temp = TMath::Sqrt(1+tx*tx);

  z_c = point[2] + radius*tx/temp;
  x_c = point[0] - TMath::Sqrt(radius*radius - (point[2]-z_c)*(point[2]-z_c));
  Printf("x_c = %10.8g, z_c = %10.8g",x_c,z_c);
  // verify equation of circle
  Printf("radius = %10.8g",sqrt((point[0]-x_c)*(point[0]-x_c) +
                                (point[2]-z_c)*(point[2]-z_c)  ));
  Printf("radius = %10.8g",sqrt(radius*radius));

  //  Double_t zeta  = curvature*(point[2]-z_c);
  Double_t zeta = -tx/temp;
  Double_t zeta2 = zeta + curvature*(point[2]+length-point[2]); // length

  new_point[0] = point[0] + (TMath::Sqrt(1-zeta2*zeta2) -
                             TMath::Sqrt(1-zeta*zeta)     )/curvature;
  new_point[1] = point[1] + ty*(TMath::ASin(zeta2) - TMath::ASin(zeta))/
    (curvature*temp);
  new_point[2] = point[2] + length;
  Printf("\nx   = %10.8g, y   = %10.8g, z   = %10.8g",
         new_point[0],new_point[1],new_point[2]);

  Double_t new_tx, new_ty, new_p[3];
  new_tx = -zeta2/TMath::Sqrt(1-zeta2*zeta2);
  new_ty = ty*TMath::Sqrt( (1+new_tx*new_tx)/(1+tx*tx));
  new_p[2] = p.Py()/new_ty; // p.Py = const
  new_p[0] = new_p[2]*new_tx;
  new_p[1] = p.Py();
  Printf("px  = %10.7g, py  = %10.7g, pz  = %10.7g",
         new_p[0], new_p[1], new_p[2]);
  Printf("radius = %10.8g",sqrt( (new_point[0]-x_c)*(new_point[0]-x_c) +
                                 (new_point[2]-z_c)*(new_point[2]-z_c) ));

  const UShort_t step = 100;
  Double_t sx[step], sy[step], sz[step];
  sx[0] = point[0], sy[0] = point[1], sz[0] = point[2];
  UShort_t nsteps = 1;
  Double_t current_length = 0;
  for (UShort_t i = 1; i < step; i++) {
    current_length = (i+1)*length/step;
    zeta2 = zeta + curvature*(current_length);
    sx[i] = point[0] + (TMath::Sqrt(1-zeta2*zeta2) -
                        TMath::Sqrt(1-zeta*zeta)     )/curvature;
    sy[i] = point[1] + ty*(TMath::ASin(zeta2) - TMath::ASin(zeta))/
      (curvature*temp);
    sz[i] = point[2] + current_length;
    nsteps++;
  }
  TPolyLine3D *line = new TPolyLine3D(nsteps,sx,sy,sz);
  line->SetLineColor(kRed);
  line->Draw();
}
