// 2019-05-14
// https://root-forum.cern.ch/t/how-to-set-axis-label/34065

{
  auto h = new TH1F("h","h",100,0.,1.);
  h->Draw();
  auto Xaxis = h->GetXaxis();
  Xaxis->SetNdivisions(-9);
  Xaxis->ChangeLabel(1,-1,-1,-1,-1,-1,"20");
  Xaxis->ChangeLabel(2,-1,-1,-1,-1,-1,"35");
  Xaxis->ChangeLabel(3,-1,-1,-1,-1,-1,"50");
  Xaxis->ChangeLabel(4,-1,-1,-1,-1,-1,"65");
  Xaxis->ChangeLabel(5,-1,-1,-1,-1,-1,"80");
  Xaxis->ChangeLabel(6,-1,-1,-1,-1,-1,"95");
  Xaxis->ChangeLabel(7,-1,-1,-1,-1,-1,"110");
  Xaxis->ChangeLabel(8,-1,-1,-1,-1,-1,"140");
  Xaxis->ChangeLabel(9,-1,-1,-1,-1,-1,"180");
  Xaxis->ChangeLabel(10,-1,-1,-1,-1,-1,"300");
}
