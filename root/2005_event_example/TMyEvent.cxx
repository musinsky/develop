// -*- mode: c++ -*-
// Author: Jan Musinsky
// @(#) 07 Feb 2005

#include "TMyEvent.h"
#include "TStrawTrack.h"

ClassImp(TMyEvent)

TClonesArray *TMyEvent::fgStrawTracks = 0;

//--------------------------------------------------------------------
TMyEvent::TMyEvent()
{
  Printf("TMyEvent -> Constructor");
  if (!fgStrawTracks) fgStrawTracks = new TClonesArray("TStrawTrack",100);
  fStrawTracks = fgStrawTracks;
  fNStrawTracks = 0;
}
//--------------------------------------------------------------------
TMyEvent::~TMyEvent()
{
  Printf("TMyEvent -> Destructor");
  fStrawTracks->Clear();
  if (fStrawTracks)
    delete fStrawTracks;
}
//--------------------------------------------------------------------
void TMyEvent::Clear(Option_t * /*option*/)
{
  //  Printf("TMyEvent -> Clear");
  fNStrawTracks=0;
  fStrawTracks->Clear();
}
//--------------------------------------------------------------------
TStrawTrack *TMyEvent::AddStrawTrack(const Double_t az, const Float_t bz,
                                     const Float_t chi2, const UShort_t nh)
{
  //Printf("TMyEvent -> AddStrawTrack");
  TClonesArray &sts = *fStrawTracks;
  TStrawTrack *track = new(sts[fNStrawTracks++]) TStrawTrack(az,bz,chi2,nh);
  //  Printf("In AddTrack tracks[%d]=%x, track=%x",fNStrawTracks-1,
  //     sts.At(fNStrawTracks-1),track);
  return track;
}
