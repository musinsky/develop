// 2007-03-18
// Jan Musinsky (decode designed by Dima Kirillov)
//
// root[] .L decode_dima.C+
// root[] decode_dima("alpha.dat")
// root[] decode_dima("alpha.dat", 0, kFALSE)

#include <TString.h>

UShort_t reencode_tdc(const UShort_t tdc_ch, const UShort_t tdc_id);

void decode_dima(const char *fname, const Int_t entries = 0,
                 const Bool_t reencode = kTRUE)
{
  FILE *fp = fopen(fname, "r");
  if (!fp) {
    Printf("Error opening your file");
    return;
  }

  // create 7 (2007-03)
  UChar_t Data[3000];
  Int_t i1 = 100 - 512, nhits = 0, iev7 = 0;
  UShort_t ampl, hit, delta;

  while(fread(Data, 1, 4, fp) == 4) {

    if((Data[3] & 0xF0) == 0x40) {
      // (Data[3] & 0xF0) == 0x40 => leading measurement
      printf("     hit = %3d", nhits);

      hit   = ((Data[2] & 0xF8)>>3) + (Data[3] & 0x0F)*32;
      delta = Data[2] & 0x07;
      ampl  = Data[0] + Data[1]*256; // ?! only 16 bit instead 19 ?!
      // (Data[2] & 0xF8)>>3 => tdc_ch => 0, 2, 4, 6, ..., 30
      // (Data[3] & 0x0F)    => tdc_id => 1, 2, 4, 8
      // TGepom::fgGemEvent->AddHit(i1 + hit, ampl, delta);

      if (reencode) {
        hit = reencode_tdc((Data[2] & 0xF8)>>3, Data[3] & 0x0F);
        hit = ((i1 - 100)/512)*64 + hit;
        printf("  %4d", hit);
      }
      else
        printf("  %4d", i1 + hit);
      Printf(" => %5d", ampl);
      nhits++;
    }

    if((Data[3] & 0xF0) == 0x80) {
      // (Data[3] & 0xF0) == 0x80) => module header
      i1 += 512; // in decode_ilja num of module increasing in module trailer
      if (reencode) Printf(" module = %d", (i1 - 100)/512);
      else Printf(" module = %d", i1);
    }

    if(Data[3] == 0xA0) {
      // (Data[3] == 0xA0) => event header ?! 8 bits instead first 4 ?!
      if (entries && iev7 >= entries) break;
      Printf("event = %d", iev7);
      i1    = 100 - 512;
      nhits = 0;
      iev7++;
    }
  }
  fclose(fp);
}
UShort_t reencode_tdc(const UShort_t tdc_ch, const UShort_t tdc_id)
{
  UShort_t norm_map[4][16] = // tdc_num, tdc_ch/2
    {
     { 0x17, 0x16, 0x07, 0x06,
       0x15, 0x14, 0x05, 0x04,
       0x13, 0x12, 0x03, 0x02,
       0x11, 0x10, 0x01, 0x00 },
     { 0x1F, 0x1E, 0x0F, 0x0E,
       0x1D, 0x1C, 0x0D, 0x0C,
       0x1B, 0x1A, 0x0B, 0x0A,
       0x19, 0x18, 0x09, 0x08 },
     { 0x37, 0x36, 0x27, 0x26,
       0x35, 0x34, 0x25, 0x24,
       0x33, 0x32, 0x23, 0x22,
       0x31, 0x30, 0x21, 0x20 },
     { 0x3F, 0x3E, 0x2F, 0x2E,
       0x3D, 0x3C, 0x2D, 0x2C,
       0x3B, 0x3A, 0x2B, 0x2A,
       0x39, 0x38, 0x29, 0x28 }
    };

  UShort_t tdc_num = 99;
  for(UInt_t i = 0; i < 4; i++) {
    if (tdc_id == ( ((UInt_t)1) << i )) {
      tdc_num = i; // 0, 1, 2, 3
      break;
    }
  }

  if (tdc_num > 3) {
    Printf("bad tdc_id %u", tdc_id);
    return 0;
  }

  return norm_map[tdc_num][tdc_ch/2];
}
