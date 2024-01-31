// 2005-11-08 original
// 2019-05-14 update to ROOT6
// Musinsky Jan

void magnet_transport(const Double_t ptot, const Double_t field);

void transport01()
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
  Double_t p = TMath::Sqrt(vec[3]*vec[3] + vec[4]*vec[4] + vec[5]*vec[5]);
  vec[3] = vec[3]/p;
  vec[4] = vec[4]/p;
  vec[5] = vec[5]/p;
  Double_t fieldA = 3.0; // [Tesla]
  Double_t fieldB = -2.0;

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
      if (geom->GetCurrentVolume() == magnetA) magnet_transport(p,fieldA);
      if (geom->GetCurrentVolume() == magnetB) magnet_transport(p,fieldB);
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
void magnet_transport(const Double_t ptot, const Double_t field)
{
  TGeoNode *c_n = gGeoManager->GetCurrentNode();
  const Double_t *c_p = gGeoManager->GetCurrentPoint();
  const Double_t *c_d = gGeoManager->GetCurrentDirection();
  Double_t px = c_d[0]*ptot;
  Double_t py = c_d[1]*ptot;
  Double_t pz = c_d[2]*ptot;
  Double_t sp[3] = {c_p[0], c_p[1], c_p[2]};
  Double_t q = 1.0; // charge
  Double_t b3 = field*TMath::C()*10e-12;

  Double_t zc = sp[2] + (q*px)/b3;
  Double_t xc = sp[0] - (q*pz)/b3;
  Double_t angle_step = 0.01*TMath::DegToRad(); // ~ precision
  angle_step = TMath::Sign(angle_step, b3);    // +- field orientation

  Double_t sa, ca;
  Double_t np[3];
  Double_t full_angel = 0.0;
  Bool_t back_transport = kFALSE;

  cout << endl << "input magnet" << endl;
  cout << "point=>  "<<c_p[0]<<" "<<c_p[1]<<" "<<c_p[2]<<endl;
  cout << "impuls=> "<<c_d[0]*ptot<<" "<<c_d[1]*ptot<<" "<<c_d[2]*ptot<<endl;

  gGeoManager->AddTrack(1,9999);
  TVirtualGeoTrack *trackm = gGeoManager->GetTrack(1);
  trackm->SetName("trackm");
  trackm->SetLineColor(kBlue);

  for (Int_t i = 1; i < 9999; i++) {
    full_angel += angle_step;
    sa = TMath::Sin(full_angel);
    ca = q*(1.0-TMath::Cos(full_angel));
    np[2] = sp[2] + (sa*pz + ca*px)/b3;
    np[0] = sp[0] + (sa*px - ca*pz)/b3;
    np[1] = sp[1] + (full_angel*py)/b3;

    if (gGeoManager->FindNode(np[0], np[1], np[2]) == c_n) { // enlarge angel
      //      gGeoManager->GetTrack(0)->AddPoint(np[0],np[1],np[2],0);
      trackm->AddPoint(np[0],np[1],np[2],0);
      if (back_transport) {
        gGeoManager->SetCurrentDirection((-q*b3*(np[2]-zc))/ptot,py/ptot,
                                         ( q*b3*(np[0]-xc))/ptot);
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
      angle_step = -angle_step/100;
      back_transport = kTRUE;
    }
  }
}
