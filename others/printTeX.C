// Author: Jan Musinsky
// 29/07/2009

void printTeX(const char *name, TPad *pad = 0)
{
  if (!gPad) return;
  if (pad == 0) pad = gPad;
  pad->Modified();
  pad->Update();
  pad->Print(Form("%s.eps", name));
  gSystem->Exec(Form("epstopdf %s.eps", name));
}
