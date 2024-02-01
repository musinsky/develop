// Author: Jan Musinsky
// 30/06/2010

void strela_setup_anim()
{
  TGeoManager *geom = new TGeoManager("strela", "STRELA geometry setup");
  TGeoMedium *m0 = new TGeoMedium("m0", 0, new TGeoMaterial("m00", 0, 0, 0));
  TGeoMedium *m1 = new TGeoMedium("m1", 0, new TGeoMaterial("m01", 0, 0, 0));
  TGeoMedium *m2 = new TGeoMedium("m2", 0, new TGeoMaterial("m02", 0, 0, 0));
  TGeoMedium *m3 = new TGeoMedium("m3", 0, new TGeoMaterial("m03", 0, 0, 0));
  TGeoVolume *top = geom->MakeBox("top", m0, 100, 100, 100);
  geom->SetTopVolume(top);

  Double_t sx = 12.5, sy = 12.5, sz = 1.5*4;
  TGeoVolume *small_4p = geom->MakeBox("small_4p", m1, sx/2, sy/2, sz/2);
  TGeoVolume *small_1p = small_4p->Divide("small_1p", 3, 4, -1, -1);
  small_1p->SetLineColor(kGray+1);
  TGeoVolume *big_4p = geom->MakeBox("big_4p", m1, sx/2*2, sy/2*2, sz/2);
  TGeoVolume *big_1p = big_4p->Divide("big_1p", 3, 4, -1, -1);
  big_1p->SetLineColor(kGray+1);
  //  TGeoVolume *target = geom->MakeBox("target", m2, 3, 3, 1);
  TGeoVolume *target = geom->MakeTube("target", m2, 0, 3, 1);
  target->SetLineColor(kGray+2);
  TGeoVolume *magnet = geom->MakeBox("magnet", m0, 15, 2, 30);
  magnet->SetLineColor(kWhite);
  TGeoVolume *S1 = geom->MakeBox("S1", m3, 10./2, 10./2, 0.3);
  TGeoVolume *S2 = geom->MakeBox("S2", m3, 7./2, 7./2, 0.3);
  TGeoVolume *S3 = geom->MakeBox("S3", m3, 25./2, 22./2, 0.6);
  S1->SetLineColor(kWhite);
  S2->SetLineColor(kWhite);
  S3->SetLineColor(kWhite);

  Double_t z = -20;
  TGeoTranslation *t_S1 = new TGeoTranslation(0, 0, z);
  top->AddNode(S1, 1, t_S1);
  TGeoTranslation *t_S2 = new TGeoTranslation(0, 0, z+=7);
  top->AddNode(S2, 2, t_S2);
  TGeoTranslation *t_D1y = new TGeoTranslation(0, 0, z+=7);
  top->AddNode(small_4p, 3, t_D1y);
  TGeoTranslation *t_D1x = new TGeoTranslation(0, 0, z+=sz);
  top->AddNode(small_4p, 4, t_D1x);
  TGeoTranslation *t_T = new TGeoTranslation(0, 0, z+=10);
  top->AddNode(target, 5, t_T);
  TGeoTranslation *t_D2 = new TGeoTranslation(0, 0, z+=12);
  top->AddNode(small_4p, 6, t_D2);
  TGeoRotation *r_D3 = new TGeoRotation();
  r_D3->RotateZ(22.5);
  TGeoCombiTrans *t_D3 = new TGeoCombiTrans(0, 0, z+=10, r_D3);
  top->AddNode(small_4p, 7, t_D3);
  TGeoTranslation *t_D4 = new TGeoTranslation(0, 0, z+=10);
  top->AddNode(small_4p, 8, t_D4);
  TGeoTranslation *t_D5 = new TGeoTranslation(0, 0, z+=10);
  top->AddNode(small_4p, 9, t_D5);

  TGeoTranslation *t_Mu = new TGeoTranslation(0, 6, z+=45);
  top->AddNode(magnet, 10, t_Mu);
  TGeoTranslation *t_Md = new TGeoTranslation(0, -6, z);
  top->AddNode(magnet, 11, t_Md);
  TGeoVolume *after_m= new TGeoVolumeAssembly("after_m");
  TGeoRotation *r_am = new TGeoRotation();
  r_am->RotateY(-33);
  TGeoCombiTrans *t_am = new TGeoCombiTrans(0, 0, z+=45, r_am);
  top->AddNode(after_m, 12, t_am);

  TGeoTranslation *t_D6y = new TGeoTranslation(0, 0, z=10);
  after_m->AddNode(big_4p, 1, t_D6y);
  TGeoTranslation *t_D6x = new TGeoTranslation(0, 0, z+=sz);
  after_m->AddNode(big_4p, 2, t_D6x);
  TGeoRotation *r_D7 = new TGeoRotation();
  r_D7->RotateZ(22.5);
  TGeoCombiTrans *t_D7 = new TGeoCombiTrans(0, 0, z+=10, r_D7);
  after_m->AddNode(big_4p, 3, t_D7);
  TGeoTranslation *t_S3 = new TGeoTranslation(0, 0, z+=7);
  after_m->AddNode(S3, 4, t_S3);

  geom->CloseGeometry();

  top->Draw();
  gPad->SetFillColor(kWhite);
  gPad->Update();
  top->Draw("ogl");
  TGLViewer *v = (TGLViewer *)gPad->GetViewer3D();
  v->SetStyle(TGLRnrCtx::kOutline);

  TTimer *timer = new TTimer(20);
  timer->SetCommand("Animate()");
  timer->TurnOn();
}

void Animate()
{
  TGLViewer *v = (TGLViewer *)gPad->GetViewer3D();
  v->CurrentCamera().RotateRad(0.001, 0.01);
  v->DoDraw();
}
