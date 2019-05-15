// 2004-10-28

{
  Float_t fRStraw = 0.522;
  Float_t fDelX   = 1.095;
  Float_t fDelZ   = 0.95;
  Float_t fShiftX = 0.5475;

  Float_t fXWire[192], fZWire[192];
  Float_t fZMulti[4]={0,25,100,125};

  Float_t xshifter = 7*fDelX + fDelX/2;
  for (UShort_t j=0; j<192; j++) {
    for (UShort_t i = 0; i <96; i++) {
      fXWire[i] = xshifter - (i%16)*fDelX;
      if ( i/48*3+1 == i/16)  // shift middle layer
        fXWire[i] = fXWire[i] + fShiftX;
      fZWire[i] =  fZMulti[i/48]  + (i-(48*(i/48)))/16 * fDelZ;
    }
  }

  //  TPad x is our Z, TPad y is our X
  Float_t minx = fZWire[0];
  Float_t maxx = fZWire[95];
  Float_t miny = fXWire[15];
  Float_t maxy = fXWire[16]; // shift middle layer
  Float_t disx = TMath::Abs(maxx-minx);
  Float_t disy = TMath::Abs(maxy-miny);

  TCanvas *c = (TCanvas *)gROOT->GetListOfCanvases()->FindObject("c_lst");
  if (!c)
    c = new TCanvas("c_lst","Layout straws tubes");
  c->Range(minx-disx*0.15,miny-disy*0.15,maxx+disx*0.1,maxy+disy*0.1);
  c->Range(-1.0, 7.0, 2.9, 9.7 ); // "my zoom", urcuje aj "okruhlost"
  c->Clear();

  TEllipse *es;
  TMarker  *ma;
  for (UShort_t i = 0; i < 96; i++) {
    if (i== 0||i==16||i==32){//||i==i||i==1||i==33||i==2||i==18||i==34) {
      es = new TEllipse(fZWire[i],fXWire[i],fRStraw,fRStraw,0,360,0);
      ma = new TMarker(fZWire[i], fXWire[i], 2);
      ma->Draw();
      es->Draw();
    }
  }
}
