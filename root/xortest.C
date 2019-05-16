// 2019-03-28
// https://root-forum.cern.ch/t/3d-coordinate-from-mouse-cursor-position/33303

void xortest()
{
  TCanvas* c1 = new TCanvas("c1","Dynamic Slice Example",10,10,700,500);
  c1->DrawFrame(-4.,-4,4.,4);
  c1->AddExec("xortest","xortestExec()");
}

void xortestExec()
{

  gPad->GetCanvas()->FeedbackMode(kTRUE);

  int pyold = gPad->GetUniqueID();
  int px    = gPad->GetEventX();
  int py    = gPad->GetEventY();
  float uxmin = gPad->GetUxmin();
  float uxmax = gPad->GetUxmax();
  int pxmin   = gPad->XtoAbsPixel(uxmin);
  int pxmax   = gPad->XtoAbsPixel(uxmax);

  if(pyold) gVirtualX->DrawLine(pxmin,pyold,pxmax,pyold);
  gVirtualX->DrawLine(pxmin,py,pxmax,py);

  gPad->SetUniqueID(py);
  Float_t upy = gPad->AbsPixeltoY(py);
  Float_t y = gPad->PadtoY(upy);
}
