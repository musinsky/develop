// 2021-03-24
// https://root-forum.cern.ch/t/shading-under-a-tgraph-with-color-as-a-function-of-another-variable/44126

void graph2col() {
  TCanvas *c = new TCanvas("c","2 graphs with colors palette",700,500);

  // Create two graphs. The first one has a low frequency and the second one an high one
  const Int_t n = 200;
  TGraph *gr1 = new TGraph(n);
  TGraph *gr2 = new TGraph(n);

  Double_t min = 11111.;
  Double_t max = -11111.;

  Double_t x, y1, y2;
  for (Int_t i=0;i<n;i++) {
    x = i*0.02;
    y1 = 10*sin(x);
    gr1->SetPoint(i,x,y1);
    y2 = 10*sin(10*x);
    gr2->SetPoint(i,x,y2);
    if (y2 < min) min = y2;
    if (y2 > max) max = y2;
  }

  // Draw the first graph as a line
  gr1->Draw("AL");
  c->Update();

  // Retrieve the min value of the plot
  Double_t minp = gr1->GetHistogram()->GetMinimum();

  // Retrive the number of colors in the current palette
  Int_t nb = TColor::GetNumberOfColors();

  // Draw the second graph as colors below the first one.
  Int_t ic;
  for (Double_t x0 =0; x0<4.; x0 = x0+0.02) {
    ic = ((max - gr2->Eval(x0)) / (max-min))*nb;
    TLine *l = new TLine(x0 , minp, x0, gr1->Eval(x0));
    l->SetLineColor(TColor::GetColorPalette(ic));
    l->SetLineWidth(5);
    l->Draw();
  }

  // Draw again the first graph and redraw the axis
  gr1->SetLineWidth(5);
  gr1->SetLineColor(kRed);
  gr1->Draw("L");
  c->RedrawAxis();
}
