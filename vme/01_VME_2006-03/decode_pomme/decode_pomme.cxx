// 2006-03-16

#include <TFile.h>
#include <TTree.h>

#include "TMyEvent.h"

int main(int argc, char **argv)
{
  if (argc < 2) {
    Printf("usage: %s file.data number_modules",argv[0]);
    return -1;
  }
  FILE *fp = fopen(argv[1],"r");

  TFile *file = TFile::Open("dima.root","RECREATE");
  TTree *tree = new TTree("dp","strela tree");
  //tree->SetAutoSave(1000000000);

  TGemEvent *gem_event = new TGemEvent();
  //TGemEvent *gem_event = new TGemEvent();
  tree->Branch("gem",&gem_event);

  UChar_t Data[3000];
  Int_t i1;
  UShort_t ampl, hit, delta;

  for (Int_t ee=0; ee<1051598; ee++) { // ?!?!?!
    gem_event->Clear();

    i1 = 100 - 512;
    while(fread(Data,1,4,fp) == 4  && (Data[3] != 0xB0)) {
      if((Data[3] & 0xF0) == 0x40) {
        hit   = ((Data[2] & 0xF8)>>3) + (Data[3] & 0x0F)*32;
        delta = 0;
        ampl  = Data[0] + Data[1]*256 + (Data[2] & 0x07)*256*256;
        gem_event->AddHit(i1 + hit, ampl);
      }
      if((Data[3] & 0xF0) == 0x80) i1 += 512;
    }
    tree->Fill();
  }

  tree->Write();
  file->Close();
  fclose(fp);
  return 0;
}
