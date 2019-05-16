// -*- mode: c++ -*-
// Author: Jan Musinsky
// @(#) 07 Feb 2005

void macro()
{
  gSystem->Load("lib/libMyEvent");

  TFile *f = new TFile("exam.root","RECREATE","File with data");
  TTree *t = new TTree("et","TTree with data");

  cout << "before TMyEvent *my_event = new TMyEvent()" << endl;
  TMyEvent *my_event = new TMyEvent();
  cout << "after  TMyEvent *my_event = new TMyEvent()" << endl << endl;

  cout << "before TBranch *bra = ..." << endl;
  // v skutocnosti pouzivam TBronch
  TBranch *bra = t->Branch("MyEventBranch","TMyEvent",&my_event);
  cout << "after  TBranch *bra = ..." << endl << endl;

  UInt_t ne = 10000;

  for(Int_t ev = 0; ev < ne; ev++) {
    //    cout << "########## new event ##########"<<endl;
    //    cout << "event      = " << ev << endl;
    UShort_t ntrack = (UShort_t)(gRandom->Rndm(ev)*10);
    for(UShort_t i = 0; i < ntrack; i++) {
      //      cout << "----- new track -----"<<endl;
      //      cout << "track      = "<< i << endl;
      Double_t az  = gRandom->Rndm(i);
      Float_t bz   = gRandom->Rndm(i)*10;
      Float_t chi2 = gRandom->Rndm(i)*100;
      UShort_t nch =  (UShort_t)(gRandom->Rndm(i)*100);
      TStrawTrack *st = my_event->AddStrawTrack(az,bz,chi2,nch);
      //      cout << "Check Add st = " << st << endl;
    }
    //    TClonesArray *cst = my_event->GetStrawTracks();
    //    Int_t jn = cst->GetEntries();
    //    cout << "Number of non-zero object = " << jn <<endl;
    //    cout << "number of tracks = " << my_event->GetNStrawTracks() << endl;
    t->Fill();
    my_event->Clear();
  }
  t->Write();
  f->Close();
}
