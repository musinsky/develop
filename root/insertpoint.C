// 2019-05-07
// https://root-forum.cern.ch/t/insert-points-at-the-mouse-position-in-a-tgraph/33928

TGraph *g;

void myexec()
{
  // get event information
  int event = gPad->GetEvent();
  int px    = gPad->GetEventX();
  int py    = gPad->GetEventY();

  // some magic to get the coordinates
  double xd = gPad->AbsPixeltoX(px);
  double yd = gPad->AbsPixeltoY(py);
  float   x = gPad->PadtoX(xd);
  float   y = gPad->PadtoY(yd);

  if (event==1) { // left mouse button click
    g->SetPoint(g->GetN(),x,y);
    g->Draw("L*");
    gPad->Update();
    return;
  }
}

void insertpoint()
{

  g = new TGraph();
  g->SetPoint(0,0,0);
  g->SetPoint(1,100,100);
  g->Draw("al*");

  // add exec
  gPad->AddExec("myexec","myexec()");
}
