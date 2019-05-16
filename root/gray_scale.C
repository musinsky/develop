// 2005-03-05
// https://root-forum.cern.ch/t/colz-color-scale/1652

{
  TF2 *f2 = new TF2("f2","0.1+1000*((1-(x-2)*(x-2))*(1-(y-2)*(y-2)))",1,3,1,3);
  const Int_t ncol = 100;
  Int_t colors[ncol];
  TColor *col;
  Double_t dg=1/(Double_t)ncol;
  Double_t grey=0;
  for (Int_t i=0; i<ncol; i++) {
    colors[i]= i;
    col = gROOT->GetColor(colors[i]);
    col->SetRGB(grey, grey, grey);
    grey = grey+dg;
  }
  f2->SetContour(ncol);
  gStyle->SetPalette(ncol,colors);
  f2->Draw("colz");
}
