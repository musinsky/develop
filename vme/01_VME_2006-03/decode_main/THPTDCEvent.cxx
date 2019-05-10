// 2006-03-16

#include <TClonesArray.h>

#include "THPTDCEvent.h"

ClassImp(THPTDCEvent)

//______________________________________________________________________________
THPTDCEvent::THPTDCEvent()
{
  fNEvents   = 0;
  fNHits     = 0;
  fHPTDCHits = new TClonesArray("THPTDCHit",100);
}
//______________________________________________________________________________
THPTDCEvent::~THPTDCEvent()
{
  Clear("C");
  delete fHPTDCHits;
}
//______________________________________________________________________________
void THPTDCEvent::Clear(Option_t *option)
{
  fNHits = 0;
  fHPTDCHits->Clear(option);
}
//______________________________________________________________________________
void THPTDCEvent::AddHit(const UShort_t channel, const UInt_t time)
{
  TClonesArray &hits = *fHPTDCHits;
  new(hits[fNHits++]) THPTDCHit(channel,time);
}
//==============================================================================

ClassImp(THPTDCHit)

//______________________________________________________________________________
THPTDCHit::THPTDCHit(const UShort_t ch, const UInt_t t)
{
  fChannel = ch;
  fTime    = t;
}
