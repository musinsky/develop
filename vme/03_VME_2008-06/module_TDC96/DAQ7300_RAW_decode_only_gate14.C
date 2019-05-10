// 2008-06-07
// Musinsky Jan
//
// root[] .L DAQ7300_RAW_decode.C+
// root[] decode("sample.dat")

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void decode(const char *fname, const Int_t maxevents = 0)
{
  const Int_t NUM_MODULE = 5 - 1; // 2008_06
  Int_t fd, nspills = 0, nevents = 0, nmodules = 0, nhits = 0;
  UInt_t word;

  Int_t ga = 0;

  fd = open(fname, O_RDONLY);
  if (fd == -1) {
    printf("Error opening your file: %s\n", fname);
    return;
  }

  while(read(fd, &word, sizeof(word)) == sizeof(word)) {

    // 0xC Spill header
    if ((word >> 28) == 0xC) {
      printf("Spill #%d header\n", nspills);
      //      nevents = 0; // reset each burst(beam bunch)
      continue;
    }

    // 0xA Event header
    if ((word >> 28) == 0xA) {
      if (maxevents && nevents >= maxevents) break;
      printf("\tEvent #%d header", nevents);
      printf(", event number: %d\n", word & 0xFFFFF); // (bits 0 - 19)
      nmodules = 0; // reset each event
      nhits    = 0;
      continue;
    }


    // 0x8 Module header
    if ((word >> 28) == 0x8) {
      printf("\t\tModule #%d header", nmodules);
      // Custom VME Module Specifications (not default)
      printf(", event count: %d", word & 0xFFFF); // (bits 0  - 15)
      printf(", id: %d", (word >> 16) & 0x7F);    // (bits 16 - 22)
      printf(", ga: %d\n", (word >> 23) & 0x1F);  // (bits 23 - 27)
      ga = (word >> 23) & 0x1F;

      /* default, but unused
         printf("=== event number: %d\n", word & 0xFFFFF);    // (bits 0  - 19)
         printf("=== module type: %d\n", (word >> 20) & 0xF); // (bits 20 - 23)
      */
      continue;
    }

    // 0x4 TDC leading edge measurement
    if ((word >> 28) == 0x4) {
      if (ga != 14) continue;

      printf("\t\t\tTDC leading edge measurement #%d\n", nhits);
      printf("\t\t\ttdc_id:%2d", (word >> 24) & 0xF); // (bits 24 - 27)
      printf(", tdc_ch:%3d", (word >> 19) & 0x1F);    // (bits 19 - 23)
      printf(", time: %d\n", word & 0x7FFFF);          // (bits 0  - 18)
      nhits++;
      continue;
    }

    // 0x9 Module trailer
    if ((word >> 28) == 0x9) {
      printf("\t\tModule #%d trailer", nmodules);
      // Custom VME Module Specifications (not default)
      printf(", word count: %d", word & 0xFFFF); // (bits 0  - 15)
      if (((word >> 16) & 0xF) != 0xF)           // (bits 16 - 19)
        printf("\n\033[31m MTRL bits (16 - 19) are not 1111\033[m\n");
      printf(", flag: %d", (word >> 20) & 0x7);  // (bits 20 - 22)
      printf(", ga: %d\n", (word >> 23) & 0x1F); // (bits 23 - 27)
      /* default, but unused
         printf("=== module wordcount: %d\n", word & 0xFFF); // (bits 0 - 11)
      */
      nmodules++;
      continue;
    }

    // 0xB Event trailer
    if ((word >> 28) == 0xB) {
      printf("\tEvent #%d trailer", nevents);
      printf(", event wordcount: %d", word & 0xFFFF);    // (bits 0  - 15)
      printf(", xoff count: %d\n", (word >> 16) & 0xFF); // (bits 16 - 23)
      nevents++;
      if (nmodules != NUM_MODULE)
        printf("\033[31m incorrect number of modules: %d\033[m\n", nmodules);
      continue;
    }

    // 0xD Spill trailer
    if ((word >> 28) == 0xD) {
      printf("Spill #%d trailer", nspills);
      printf(", spill wordcount: %d\n", word & 0xFFFFFFF); // (bits 0 - 27)
      nspills++;
      continue;
    }
  }
  close(fd);
}
