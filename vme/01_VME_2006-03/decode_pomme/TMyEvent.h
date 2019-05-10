// 2006-03-16

#include <TObject.h>

class TClonesArray;

class TGemEvent : public TObject {

private:
  UInt_t          fNhits;
  UInt_t          fNevent;
  TClonesArray   *fAdcHits;

public:
  TGemEvent();
  virtual ~TGemEvent();

  void  AddHit(UShort_t nadc, UShort_t adc);
  void  SetEventNumber(UInt_t nevent) {fNevent = nevent;}
  void  Clear();

  ClassDef(TGemEvent,1)  //Gem Event class
};

class TAdcHit : public TObject {

private:
  UShort_t          fNadc;
  UShort_t          fAdc;
public:
  TAdcHit() {;}
  virtual ~TAdcHit() {;}
  TAdcHit(UShort_t nadc, UShort_t adc);

  ClassDef(TAdcHit,1)  //Adc Hit class
};
