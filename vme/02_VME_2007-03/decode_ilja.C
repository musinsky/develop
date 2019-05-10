// 2007-03-18
// Jan Musinsky (decode designed by Ilja Slepnev)
//
// root[] .L decode_ilja.C+
// root[] decode_ilja("alpha.dat")

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <TString.h>

void decode_ilja(const char *fname, const UInt_t entries = 0)
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

  Int_t fd;
  UInt_t word;
  UInt_t nevents = 0, nmodules = 0, nhits = 0;
  UInt_t tdc_id = 0, tdc_num = 0, tdc_ch = 0;
  const UInt_t nmod = 3;

  UShort_t fNtdc;
  UInt_t   fTdc;

  fd = open(fname, O_RDONLY);
  if (fd == -1) {
    Printf("Error opening your file");
    return;
  }

  nevents = 0;
  while(read(fd, &word, sizeof(word)) == sizeof(word)) {

    // 0xA Event header
    if ((word >> 28) == 0xA) {
      if (entries && nevents >= entries) break;
      Printf("event = %u", nevents);
      if (nevents > 0 && nmodules != nmod) {
        Printf(" !!! ERROR !!! ");
        return;
      }
      nmodules = 0;
      nhits    = 0;
      nevents++;
      continue;
    }

    // 0x8 Module header
    if ((word >> 28) == 0x8) {
      Printf(" module = %u", nmodules);
      continue;
    }

    // 0x9 Module trailer
    if ((word >> 28) == 0x9) {
      nmodules++;
      continue;
    }

    // 0x4 TDC leading edge measurement
    if ((word >> 28) == 0x4) {
      printf("     hit = %3u", nhits);

      tdc_id = (word >> 24)  & 0xF; // 1, 2, 4, 8
      // tdc_id is always < 16

      tdc_num = 99;
      for(UInt_t i = 0; i < 4; i++) {
        if (tdc_id == ( ((UInt_t)1) << i )) {
          tdc_num = i;             // 0, 1, 2, 3
          break;
        }
      }

      if (tdc_num > 3) {
        Printf("bad tdc_id %u", tdc_id);
        continue;
      }

      tdc_ch = (word >> 19) & 0x1F; // 0, 2, 4, 6, ..., 30
      // tdc_ch is always < 32

      fNtdc = nmodules*64 + norm_map[tdc_num][tdc_ch/2];
      fTdc  = word & 0x7FFFF;

      printf("  %4hu", fNtdc);
      Printf(" => %5u", fTdc);
      nhits++;
    }
  }
  close(fd);
}
