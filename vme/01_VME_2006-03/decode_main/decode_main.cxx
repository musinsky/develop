// 2006-03-17

// make print_info

// needed for large files (> 4 GB)
//#define _LARGEFILE64_SOURCE
//#define _FILE_OFFSET_BITS 64

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include <TFile.h>
#include <TTree.h>

#include "THPTDCEvent.h"

const UShort_t norm_map[4][16] =
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

//______________________________________________________________________________
int main(int argc, char **argv)
{
  if (argc < 2) {
    Printf("%s: no input files",argv[0]);
    return -1;
  }

  UInt_t check_modules = 3;
  if (argc > 2) if (atoi(argv[2])) check_modules = atoi(argv[2]);

  struct stat statbuf;
  if (stat(argv[1],&statbuf)) {
    Printf("stat('%s') failed: %s",argv[1],strerror(errno));
    return -1;
  }

  Int_t fd = open(argv[1],O_RDONLY);
  if (fd < 0) {
    Printf("open('%s') failed: %s",argv[1],strerror(errno));
    return -1;
  }

  TFile *file = TFile::Open(Form("%s.root",argv[1]),"RECREATE");
  TTree *tree = new TTree("dp","strela tree");
  tree->SetAutoSave(1000000000);

  // TClonesArray in THPTDCEvent is not static (only once)
  THPTDCEvent *hptdc_event = new THPTDCEvent();
  tree->Branch("hptdc",&hptdc_event);

  UInt_t *words;
  off_t map_len, map_off, file_size, map_max;
  map_off   = 0;
  file_size = statbuf.st_size;
  map_max   = 10485760L; // 10 MB

  UInt_t nmodules, tdc_id, tdc_num, tdc_ch;
  UShort_t channel;
  UInt_t   time;
#ifdef PRINT_INFO
  UInt_t nevents = 0, nhits = 0;
#endif
  hptdc_event->Clear(); nmodules = 0; // for first event

  while (map_off < file_size) {
    map_len = file_size - map_off;
    if (map_len > map_max) map_len = map_max;

    words = (UInt_t*)mmap(0,map_len,PROT_READ,MAP_SHARED,fd,map_off);
    if (words == MAP_FAILED) {
      Printf("mmap('%s') failed: %s",argv[1],strerror(errno));
      return -1;
    }

    for (UInt_t iw = 0; iw < (map_len/sizeof(words)); iw++) {

#ifdef PRINT_INFO
      // 0xA Event header
      if ((words[iw] >> 28) == 0xA) {
        Printf("##### ehdr    %u #####",nevents);
        nhits = 0;
        nevents++;
        continue;
      }
#endif

      // 0xB Event trailer
      if ((words[iw] >> 28) == 0xB) {
        if (nmodules != check_modules) {
          Printf("ERROR, nmodules = %u",nmodules);
          return -2;
        }
        tree->Fill();
        hptdc_event->Clear();
        nmodules = 0;
        continue;
      }

#ifdef PRINT_INFO
      // 0x8 Module header
      if ((words[iw] >> 28) == 0x8) {
        Printf("  mhdr    %u",nmodules);
        continue;
      }
#endif

      // 0x9 Module trailer
      if ((words[iw] >> 28) == 0x9) {
        nmodules++; // must be in trailer
        continue;
      }

      // 0x4 TDC leading edge measurement
      if ((words[iw] >> 28) == 0x4) {
        tdc_id = (words[iw] >> 24)  & 0xF; // 1, 2, 4, 8 => "socket" ID
        // tdc_id is always < 16(4bit)
        tdc_num = 9999; // reset
        for(UInt_t i = 0; i < 4; i++) {    // one of all 4  "sockets"
          if (tdc_id == (((UInt_t)1) << i)) {
            tdc_num = i;                   // 0, 1, 2, 3 => "socket" number
            break;
          }
        }
        if (tdc_num > 3) {
          Printf("WARNING, bad tdc_id %u", tdc_id);
          continue;
        }

        tdc_ch = (words[iw] >> 19) & 0x1F; // 0, 2, 4, 6, ..., 30
        // tdc_ch is always < 32(5bit)

        channel = nmodules*64 + norm_map[tdc_num][tdc_ch/2];
        time    =  words[iw] & 0x7FFFF;
        hptdc_event->AddHit(channel,time);

#ifdef PRINT_INFO
        printf("    thit   %u",nhits);
        printf(" => %3hu",channel);
        Printf(" %u",time);
        nhits++;
#endif
      }
    } // end part of file

    munmap(words,map_len);
    map_off += map_len;
  } // end all file

  tree->Write();
  file->Close();
  close(fd);
  return 0;
}
