// 2007-03-19
// Jan Musinsky
//
// root[] .L decode_table.C+
// root[] decode_table()

#include <TString.h>
#include <TMath.h>

#define SOCKETS  4
#define CHANNELS 16
/*
  +---+---+
  47  |  63
  46  |  62
  .   |  .
  .   |  .
  33  |  49
  32  |  48
  +---+---+
  15  |  31
  14  |  30
  .   |  .
  .   |  .
  1   |  17
  0   |  16
  +---+---+
*/

UShort_t decode_tdc(const UShort_t tdc_id, const UShort_t tdc_ch)
{
  UShort_t norm_map[SOCKETS][CHANNELS] = // tdc_num, tdc_ch/2
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

  // tdc_id = 1, 2, 4, 8
  UShort_t tdc_num = 9999;
  for(UShort_t i = 0; i < SOCKETS; i++) {
    if (tdc_id == (1<<i)) {
      tdc_num = i; // 0, 1, 2, 3
      break;
    }
  }

  if (tdc_num >= SOCKETS) {
    Printf("bad tdc_id %u", tdc_id);
    return 0;
  }

  // tdc_ch = 0, 2, 4, 6, ..., 30
  return norm_map[tdc_num][tdc_ch/2];
}

void decode_table()
{
  UShort_t id, ch;
  Int_t total = SOCKETS*CHANNELS;
  Int_t ilja[total], dima[total], seek[total];
  Int_t count = 0;

  for (UShort_t i = 0; i < SOCKETS; i++) { // tdc_num
    id = 1<<i;

    for (UShort_t j = 0; j < CHANNELS; j++) { // tdc_ch
      ch = j*2;

      ilja[count] = decode_tdc(id, ch);
      dima[count] = 100 + (ch + id*32); // see in pomme.cxx
      // Printf("[%3d] %4d", ilja[count], dima[count]);
      count++;
    }
  }

  // 2  3
  // 0  1
  Int_t i1, i2;
  TMath::Sort(count, ilja, seek, kFALSE);
  for (Int_t column = (SOCKETS - 1); column >=0; column -= 2) {
    Printf("\nGND\t\tGND");
    for (Int_t row = (CHANNELS - 1); row >=0; row--) {
      i1 = (column - 1)*CHANNELS + row;
      i2 = column*CHANNELS + row;
      printf("[%3d] %4d\t", ilja[seek[i1]], dima[seek[i1]]);
      Printf("[%3d] %4d",   ilja[seek[i2]], dima[seek[i2]]);
    }
  }
}
