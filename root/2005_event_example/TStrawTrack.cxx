// -*- mode: c++ -*-
// Author: Jan Musinsky
// @(#) 07 Feb 2005

#include "TStrawTrack.h"

ClassImp(TStrawTrack)

//--------------------------------------------------------------------
TStrawTrack::TStrawTrack()
{
  Printf("TStrawTrack -> Constructor");
  fAz         = 9999;
  fBz         = 9999;
  fChi2       = 9999;
  fNHitsTrack = 9999;
}
//--------------------------------------------------------------------
TStrawTrack::TStrawTrack(const Double_t az, const Float_t bz,
                         const Float_t chi2, const UShort_t nh)
{
  // Printf("TStrawTrack -> Constructor with init value");
  fAz         = az;
  fBz         = bz;
  fChi2       = chi2;
  fNHitsTrack = nh;
}
//--------------------------------------------------------------------
TStrawTrack::~TStrawTrack()
{
  Printf("TStrawTrack -> Destructor, call Clear()");
  Clear();
}
//--------------------------------------------------------------------
void TStrawTrack::Clear(Option_t * /*option*/)
{
  Printf("TStrawTrack -> Clear");
}
