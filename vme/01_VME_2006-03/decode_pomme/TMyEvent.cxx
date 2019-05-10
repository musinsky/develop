// 2006-03-16

#include <TClonesArray.h>

#include "TMyEvent.h"

ClassImp(TGemEvent)

//______________________________________________________________________________
TGemEvent::TGemEvent()
{
  fAdcHits  = new TClonesArray("TAdcHit",100);
  fNhits = 0;
  fNevent = 0;
}
//______________________________________________________________________________
TGemEvent::~TGemEvent()
{
  Clear();
}
//______________________________________________________________________________
void TGemEvent::Clear()
{
  fAdcHits->Clear();
  fNhits = 0;
}
//______________________________________________________________________________
void TGemEvent::AddHit(UShort_t nadc, UShort_t adc)
{
  TClonesArray &hits = *fAdcHits;
  new(hits[fNhits++]) TAdcHit(nadc,adc);
}

ClassImp(TAdcHit)

//______________________________________________________________________________
TAdcHit::TAdcHit(UShort_t nadc, UShort_t adc)
{
  fNadc = nadc;
  fAdc = adc;
}
