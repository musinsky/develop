// 2005-09-29

{
  gSystem->Load("libGeom");
  TGeoManager *mgr = new TGeoManager( "Geom", "TGeoPcon example" );
  TGeoVolume *top = mgr->MakeBox( "TOP", NULL, 100, 100, 250 );
  mgr->SetTopVolume(top);
  TGeoPcon *pcon = new TGeoPcon( 0, 360, 10 );
  Double_t z = 0;
  Double_t r = 0;
  for( UInt_t i = 0; i < 10; i++ )
    {
      pcon->DefineSection( i, z, r, r + 1 );
      z += 10;
      r += 2;
    }
  TGeoVolume *pvol = new TGeoVolume( "PVOL", pcon );
  top->AddNode( pvol, 1 );
  mgr->CloseGeometry();
  top->Draw();

  Double_t pos[3] = { 0, 0, 500 };
  Double_t dir[3] = { 0, 0, -1 };
  mgr->InitTrack( pos, dir );
  if( mgr->IsOutside() )
    {
      mgr->FindNextBoundary();
      std::cout << "\tStepping " << mgr->GetStep() << " cm to next "
        "boundary.\n";
      mgr->Step();
    }
  while( ! mgr->IsOutside() )
    {
      const Double_t *point = mgr->GetCurrentPoint();
      std::cout << "Current node: " << mgr->GetPath() << ": pos (" <<
        point[0] << ", " << point[1] << ", " << point[2] << ")\n";
      mgr->FindNextBoundary();
      std::cout << "\tStepping " << mgr->GetStep() << " cm to next "
        "boundary\n";
      mgr->Step();
    }
}
