// 2004-11-04

{
  Float_t fRStraw = 0.522;
  Float_t fDelX   = 1.095;
  Float_t fDelZ   = 0.95;
  Float_t fShiftX = 0.5475;

  Float_t fXWire[192], fZWire[192];
  //Float_t fZMulti[4]={0,25.5,0,29.0};
  Float_t fZMulti[4]={0,6,13,20};

  Float_t xshifter = 7*fDelX + fDelX/2;
  for (UShort_t i = 0; i <192; i++) {
    fXWire[i] = xshifter - (i%16)*fDelX;
    if ( i/48*3+1 == i/16)  // shift middle layer
      fXWire[i] = fXWire[i] + fShiftX;
    fZWire[i] =  fZMulti[i/48]  + (i-(48*(i/48)))/16 * fDelZ;

    //fXWire[i]=fXWire[i]*3.0;
    //fZWire[i]=fZWire[i]*3.0;
  }


  //  TPad x is our Z, TPad y is our X
  Float_t minx = fZWire[0];
  //Float_t maxx = fZWire[95];
  Float_t maxx = fZWire[191];
  Float_t miny = fXWire[15];
  Float_t maxy = fXWire[16]; // shift middle layer
  Float_t disx = TMath::Abs(maxx-minx);
  Float_t disy = TMath::Abs(maxy-miny);

  TCanvas *c = (TCanvas *)gROOT->GetListOfCanvases()->FindObject("c_straws");
  if (!c)
    c = new TCanvas("c_straws","Schematic straws tubes");
  c->Range(minx-disx*0.15,miny-disy*0.15,maxx+disx*0.1,maxy+disy*0.1);
  //c->Range(-1.0, 7.0, 2.9, 9.7 ); // "my zoom", urcuje aj "okruhlost"
  c->Clear();

  TEllipse *es;
  TMarker  *ma;
  TLatex   *latex;
  char tmp[10];

  for (UShort_t i = 0; i < 192; i++) {
    if (i < 96)
      sprintf(tmp,"%i",i+96);
    else
      sprintf(tmp,"%i",i-96);

    //if (i== 0||i==16||i==32)
    es = new TEllipse(fZWire[i],fXWire[i],fRStraw,fRStraw,0,360,0);
    es->Draw();
    ma = new TMarker(fZWire[i], fXWire[i], 2);
    //ma->Draw();
    latex = new TLatex(fZWire[i], fXWire[i], tmp);
    latex->SetTextSize(0.025);
    latex->SetTextAlign(22);
    latex->Draw();
  }
}
