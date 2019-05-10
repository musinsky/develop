// 2006-03-16

#include <TObject.h>

class TClonesArray;

class THPTDCEvent : public TObject {

private:
  UShort_t      fNEvents;
  UShort_t      fNHits;
  TClonesArray *fHPTDCHits;

public:
  THPTDCEvent();
  virtual      ~THPTDCEvent();

  void          Clear(Option_t *option="");
  void          AddHit(const UShort_t channel, const UInt_t time);
  void          SetEventNumber(const UShort_t nevent) { fNEvents = nevent; }
  UShort_t      GetNEvents() const { return fNEvents; }
  UShort_t      GetNHits() const { return fNHits; }
  TClonesArray *HPTDCHits() const { return fHPTDCHits; }

  ClassDef(THPTDCEvent,1) // HPTDC class
};

//______________________________________________________________________________

class THPTDCHit : public TObject {

private:
  UShort_t      fChannel;
  UInt_t        fTime;

public:
  THPTDCHit() {;}
  THPTDCHit(const UShort_t ch, const UInt_t t);
  virtual      ~THPTDCHit() {;}

  ClassDef(THPTDCHit,1) // HPTDCHit class
};
