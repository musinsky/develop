// -*- mode: c++ -*-
// Author: Jan Musinsky
// @(#) 07 Feb 2005

#ifndef TStrawEvent_H
#define TStrawEvent_H

#ifndef ROOT_TObject
#include <TObject.h>
#endif
#ifndef ROOT_TClonesArray
#include <TClonesArray.h>
#endif

class TStrawTrack;

class TMyEvent : public TObject {

private:
  UShort_t             fNStrawTracks;   // number of tracks
  TClonesArray        *fStrawTracks;    // -> array with all tracks

  static TClonesArray *fgStrawTracks;

public:
  TMyEvent();
  virtual ~TMyEvent();

  void          Clear(Option_t *option="");
  void          SetNStrawTracks(const UShort_t nt) { fNStrawTracks = nt; }
  UShort_t      GetNStrawTracks() const { return fNStrawTracks; }

  TStrawTrack  *AddStrawTrack(const Double_t az, const Float_t bz,
                              const Float_t chi2, const UShort_t nh);
  TClonesArray *GetStrawTracks() const { return fStrawTracks; }

  ClassDef(TMyEvent,1) // TMyEvent
};

#endif
