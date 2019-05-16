// -*- mode: c++ -*-
// Author: Jan Musinsky
// @(#) 07 Feb 2005

#ifndef TStrawTrack_H
#define TStrawTrack_H

#ifndef ROOT_TObject
#include <TObject.h>
#endif
#ifndef ROOT_TClonesArray
#include <TClonesArray.h>
#endif

class TStrawTrack : public TObject {

private:
  Double_t   fAz;         // Az for track
  Float_t    fBz;         // Bz for track
  Float_t    fChi2;       // Chi2 for track
  UShort_t   fNHitsTrack; // number of hits for this track

public:
  TStrawTrack();
  TStrawTrack(const Double_t az, const Float_t bz,
              const Float_t chi2, const UShort_t nh);
  virtual ~TStrawTrack();

  void       Clear(Option_t *option="");
  Double_t   GetAz() const { return fAz; }
  Float_t    GetBz() const { return fBz; }
  Float_t    GetChi2() const { return fChi2; }
  UShort_t   GetNHitsTrack() const { return fNHitsTrack; }

  ClassDef(TStrawTrack,1) // TStrawTrack class
};

#endif
