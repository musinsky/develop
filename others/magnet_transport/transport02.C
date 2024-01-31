// 2005-11-08 original
// 2019-05-14 update to ROOT6
// Musinsky Jan

void magnet_transport(const Double_t curvature, const Double_t ptot);

void transport02()
{
  // create geometry
  TGeoManager *geom = new TGeoManager("world","");
  TGeoMaterial *mat = new TGeoMaterial("vac",0,0,0);
  TGeoMedium *med0  = new TGeoMedium("med0",0,mat);
  TGeoMedium *med1  = new TGeoMedium("med1",1,mat); // magnet medium

  TGeoVolume *top = geom->MakeBox("TOP",med0,1000,1000,1000);
  geom->SetTopVolume(top);

  TGeoVolume *detA = geom->MakeBox("detA",med0,20,20,10);
  TGeoTranslation *tA = new TGeoTranslation(0,0,-50);
  TGeoVolume *magnetA = geom->MakeBox ("magnetA",med1,50,50,100);
  TGeoTranslation *tMagnetA = new TGeoTranslation(0,0,150);

  TGeoVolume *afterMagnetA = new TGeoVolumeAssembly("afterMagnetA");
  TGeoRotation *rotA = new TGeoRotation();
  rotA->RotateY(-30);
  TGeoCombiTrans *combiA = new TGeoCombiTrans("combiA",-50,0,300,rotA);
  TGeoVolume *detB = geom->MakeBox("detB",med0,30,30,10);
  TGeoTranslation *tB = new TGeoTranslation(0,0,50);
  detB->SetLineColor(kBlue);
  TGeoVolume *magnetB = geom->MakeBox ("magnetB",med1,40,40,120);
  TGeoTranslation *tMagnetB = new TGeoTranslation(0,0,200);
  magnetB->SetLineColor(kBlue);

  TGeoVolume *afterMagnetB = new TGeoVolumeAssembly("afterMagnetB");
  TGeoRotation *rotB = new TGeoRotation();
  rotB->RotateY(20);
  TGeoCombiTrans *combiB = new TGeoCombiTrans("combiB",0,0,200,rotB);
  TGeoVolume *detC = geom->MakeBox("detC",med0,30,30,10);
  TGeoTranslation *tC = new TGeoTranslation(0,0,200);
  detC->SetLineColor(kGreen);

  afterMagnetB->AddNode(detC,1,tC);

  afterMagnetA->AddNode(detB,1,tB);
  afterMagnetA->AddNode(magnetB,1,tMagnetB);
  afterMagnetA->AddNode(afterMagnetB,1,combiB);

  top->AddNode(detA,1,tA);
  top->AddNode(magnetA,1,tMagnetA);
  top->AddNode(afterMagnetA,1,combiA);

  geom->CloseGeometry();

  // tracking

  // generate particle(s), start from center detA
  geom->cd("/TOP_1/detA_1");
  Double_t *detAp = geom->GetCurrentMatrix()->GetTranslation();
  Double_t vec[6];
  vec[0] = gRandom->Uniform(detAp[0]-5,detAp[0]+5); // x
  vec[1] = gRandom->Uniform(detAp[1]-1,detAp[1]+1); // y
  vec[2] = gRandom->Uniform(detAp[2]-1,detAp[2]+1); // z
  vec[3] = gRandom->Gaus(0.5,0.1); // px
  vec[4] = gRandom->Gaus(0.0,0.1); // py
  vec[5] = gRandom->Gaus(3.0,0.2); // pz, direct of beam [GeV/c]
  Double_t p  = TMath::Sqrt(vec[3]*vec[3] + vec[4]*vec[4] + vec[5]*vec[5]);
  Double_t pt = TMath::Sqrt(vec[3]*vec[3] + vec[5]*vec[5]);
  vec[3] = vec[3]/p;
  vec[4] = vec[4]/p;
  vec[5] = vec[5]/p;
  Double_t fieldA = 3.0; // [Tesla]
  Double_t fieldB = -2.0;
  Double_t curvatureA = (fieldA*TMath::C()*10e-12)/pt; // [cm]
  Double_t curvatureB = (fieldB*TMath::C()*10e-12)/pt;

  geom->InitTrack(vec[0],vec[1],vec[2],vec[3],vec[4],vec[5]);
  geom->AddTrack(0,999);
  TVirtualGeoTrack *track = geom->GetTrack(0);
  track->SetName("track");

  const Double_t *currentp = geom->GetCurrentPoint();
  track->AddPoint(currentp[0],currentp[1],currentp[2],0); // start point

  // transport
  while (!geom->IsOutside()) {
    geom->FindNextBoundaryAndStep();
    track->AddPoint(currentp[0],currentp[1],currentp[2],0);
    cout << "-> "<<gGeoManager->GetCurrentNode()->GetName() << endl;
    if (geom->GetCurrentVolume()->GetMedium() == med1) { // magnet
      if (geom->GetCurrentVolume() == magnetA) magnet_transport(curvatureA,p);
      if (geom->GetCurrentVolume() == magnetB) magnet_transport(curvatureB,p);
    }
  }

  // draw
  geom->GetMasterVolume()->Draw();
  geom->DrawTracks();
  TView *view = gPad->GetView();
  //  if (view) view->SetView(-270,140,0,0);
  //  if (view) view->SetView(90,90,0,0);
  Int_t irep;
  if (view) view->SetView(90,90,0,irep);
  gPad->Modified();
  gPad->Update();
}
//______________________________________________________________________________
void magnet_transport(const Double_t curvature, const Double_t ptot)
{
  const char *path = gGeoManager->GetPath(); // remember current path
  const Double_t *c_p  = gGeoManager->GetCurrentPoint();
  const Double_t *c_d  = gGeoManager->GetCurrentDirection();
  Double_t sp[3] = {c_p[0], c_p[1], c_p[2]};
  Double_t tx    = c_d[0]/c_d[2]; // tx = px/pz
  Double_t ty    = c_d[1]/c_d[2]; // ty = py/pz
  Double_t tmp   = TMath::Sqrt(1+tx*tx);
  Double_t zeta  = -tx/tmp;
  Double_t zetad;

  // !!!!! direction of field ,problem with - !!!!!
  //  Double_t zc   = sp[2] + (1.0/curvature)*(tx/tmp);
  //  Double_t xc   = sp[0] - TMath::Sqrt((1.0/curvature)*(1.0/curvature) -
  //                                  (sp[2] - zc)*(sp[2] - zc));

  Double_t np[3];
  Bool_t   back_transport = kFALSE;
  // !!!!! step must be depend of mag. field orientation, +- !!!!!
  Double_t normal_step    = 0.05; // [cm]
  Double_t exactly_step   = normal_step/100;
  Double_t current_step   = normal_step;
  Double_t full_step      = 0;
  Double_t new_tx, new_ty, new_dir[3];

  cout << endl << "input magnet" << endl;
  cout << "point=>  "<<c_p[0]<<" "<<c_p[1]<<" "<<c_p[2]<<endl;
  cout << "impuls=> "<<c_d[0]*ptot<<" "<<c_d[1]*ptot<<" "<<c_d[2]*ptot<<endl;
  gGeoManager->AddTrack(1,999);
  TVirtualGeoTrack *trackm = gGeoManager->GetTrack(1);
  trackm->SetName("trackm");
  trackm->SetLineColor(kBlue);

  for (Int_t i = 1; i < 99999; i++) {
    full_step += current_step;
    zetad = zeta + curvature*(full_step);
    np[0] = sp[0] + (TMath::Sqrt(1-zetad*zetad) - TMath::Sqrt(1-zeta*zeta))/
      curvature;
    np[1] = sp[1] + ty*(TMath::ASin(zetad) - TMath::ASin(zeta))/
      (curvature*tmp);
    np[2] = sp[2] + full_step; // !!!!! full_step = new_z - old_z, problem +-

    gGeoManager->FindNode(np[0], np[1], np[2]);
    if (gGeoManager->IsSameLocation()) { // enlarge full_step
      //      gGeoManager->GetTrack(0)->AddPoint(np[0],np[1],np[2],0);
      trackm->AddPoint(np[0],np[1],np[2],0);
      if (back_transport) {
        new_tx = -zetad/TMath::Sqrt(1-zetad*zetad);
        new_ty = ty*TMath::Sqrt((1+new_tx*new_tx)/(1+tx*tx));
        new_dir[1] = c_d[1]; // <=> py = new_py
        new_dir[2] = new_dir[1]/new_ty;
        new_dir[0] = new_dir[2]*new_tx;
        gGeoManager->SetCurrentDirection(new_dir);

        cout << "output magnet" << endl;
        cout << "point=>  "<<c_p[0]<<" "<<c_p[1]<<" "<<c_p[2]<<endl;
        cout << "impuls=> "<<c_d[0]*ptot<<" "<<c_d[1]*ptot<<" "<<c_d[2]*ptot<<endl;
        cout << "steps=   " << i << endl;
        cout << "sum2=    " <<c_d[0]*c_d[0]+c_d[1]*c_d[1]+c_d[2]*c_d[2]<< endl;
        cout << endl;
        return;
      }
    }
    else if (!back_transport) { // oops, little back
      gGeoManager->cd(path); // CurrentNode must be magnet
      current_step = -exactly_step;
      back_transport = kTRUE;
    }
  }
}
