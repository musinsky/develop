// 2020-05-06
// https://root-forum.cern.ch/t/project-th3-on-walls-as-th2/39283

#include "TCanvas.h"
#include "TH2.h"
#include "TH3.h"
#include "TMarker3DBox.h"
#include "TPolyLine3D.h"
#include "TGLViewer.h"
#include "TView3D.h"
#include "TStyle.h"
#include "TRandom.h"

#include <iostream>
using namespace std;

void proj3d(TH3F *inh3 = NULL)
{
  // Draw 3d hist in a box together with its 2d projections
  // on the walls using TView3D
  // For nicer look one should dive into the features
  // offered by the 3d viewers, especially the TGLViewer
  // Note: This is meant as a guide, no guarantee for correctness
  //
  // Author: OS
  //
  gStyle->SetCanvasPreferGL(kFALSE);
  auto *c1 = new TCanvas("c1","Project 3d",250,10,800,800);
  TH3F *h3;
  if (inh3 != NULL) {
    h3 = inh3;
  } else {
    // build an example if no TH3 is provided
    h3 = new TH3F("hp33", "3dim ", 40,-4,4, 40,-4,4, 40,-4,4);
    h3->GetXaxis()->SetTitle("px");
    h3->GetYaxis()->SetTitle("py");
    h3->GetZaxis()->SetTitle("pz");
    gRandom->SetSeed();
    Double_t px, py, pz, px1, py1;
    for (Int_t i = 0; i < 1000; i++) {
      gRandom->Rannor(px,py);
      gRandom->Rannor(px1,pz);

      px = 1+ 0.8* px;
      py = -2.5 + 0.4*py;
      pz = -1.5 + 0.2*pz;
      h3->Fill(px, py,pz);

      px = -2+ px1;
      py = 2.5 + 0.6*py;
      pz = 1. + 0.25*pz;
      h3->Fill(px, py,pz);
    }
  }
  // draw hist + projections in normal way
  c1->Divide(2,2);
  c1->cd(1);
  h3->Draw("BOX2");

  // build projections on planes
  auto projxy = (TH2D*)h3->Project3D("xy");
  auto projyz = (TH2D*)h3->Project3D("yz");
  auto projxz = (TH2D*)h3->Project3D("xz");
  c1->cd(2);
  projxy->Draw("col");
  c1->cd(3);
  projyz->Draw("col");
  c1->cd(4);
  projxz->Draw("col");
  c1->Modified();
  c1->Update();

  // now the new part
  gStyle->SetCanvasPreferGL(kTRUE);
  auto *c2 = new TCanvas("c2","Project 3d",600,100,800,800);
  c2->cd();
  Double_t xymin = projxy->GetMinimum();
  Double_t xymax = projxy->GetMaximum();
  Double_t yzmin = projyz->GetMinimum();
  Double_t yzmax = projyz->GetMaximum();
  Double_t xzmin = projxz->GetMinimum();
  Double_t xzmax = projxz->GetMaximum();

  TAxis *xa3 = h3->GetXaxis();
  TAxis *ya3 = h3->GetYaxis();
  TAxis *za3 = h3->GetZaxis();
  // assume fixed binwidth
  Double_t xw = 0.5*xa3->GetBinWidth(1);
  Double_t yw = 0.5*ya3->GetBinWidth(1);
  Double_t zw = 0.5*za3->GetBinWidth(1);
  // create view with axis
  Double_t rmin[3], rmax[3];
  // draw the marker boxes just outside the 3d hist volume
  rmin[0] = xa3->GetXmin() - 2 * xw;
  rmin[1] = ya3->GetXmin() - 2 * yw;
  rmin[2] = za3->GetXmin() - 2 * zw;
  rmax[0] = xa3->GetXmax() + 2 * xw;
  rmax[1] = ya3->GetXmax() + 2 * yw;
  rmax[2] = za3->GetXmax() + 2 * zw;

  TView3D *view = new TView3D(1, rmin, rmax);
  view->ShowAxis();
  //  view->Draw("gl");
  TMarker3DBox *pm3d;
  // a box around the scene
  TPolyLine3D *pl3d;
  Double_t xl[5] = {rmin[0], rmax[0], rmax[0], rmin[0], rmin[0]};
  Double_t yl[5] = {rmin[1], rmin[1], rmax[1], rmax[1], rmin[1]};
  Double_t zl[5] = {rmin[2],rmin[2],rmin[2],rmin[2],rmin[2]};
  pl3d = new TPolyLine3D(5, xl, yl, zl);
  pl3d->SetLineColor(1);
  pl3d->SetLineWidth(2);
  pl3d->Draw();
  Double_t xu[5] = {rmin[0], rmax[0], rmax[0], rmin[0], rmin[0]};
  Double_t yu[5] = {rmin[1], rmin[1], rmax[1], rmax[1], rmin[1]};
  Double_t zu[5] = {rmax[2], rmax[2],rmax[2],rmax[2],rmax[2]};
  pl3d = new TPolyLine3D(5, xu, yu, zu);
  pl3d->SetLineColor(1);
  pl3d->SetLineWidth(2);
  pl3d->Draw();
  Double_t xr[2] = {rmax[0], rmax[0]};
  Double_t yr[2] = {rmin[1], rmin[1]};
  Double_t zr[2] = {rmin[2], rmax[2]};
  pl3d = new TPolyLine3D(2, xr, yr, zr);
  pl3d->SetLineColor(1);
  pl3d->SetLineWidth(2);
  pl3d->Draw();
  Double_t xb[2] = {rmax[0], rmax[0]};
  Double_t yb[2] = {rmax[1], rmax[1]};
  Double_t zb[2] = {rmin[2], rmax[2]};
  pl3d = new TPolyLine3D(2, xb, yb, zb);
  pl3d->SetLineColor(1);
  pl3d->SetLineWidth(2);
  pl3d->Draw();
  Double_t xf[2] = {rmin[0], rmin[0]};
  Double_t yf[2] = {rmin[1], rmin[1]};
  Double_t zf[2] = {rmin[2], rmax[2]};
  pl3d = new TPolyLine3D(2, xf, yf, zf);
  pl3d->SetLineColor(1);
  pl3d->SetLineWidth(2);
  pl3d->Draw();
  Double_t xk[2] = {rmin[0], rmin[0]};
  Double_t yk[2] = {rmax[1], rmax[1]};
  Double_t zk[2] = {rmin[2], rmax[2]};
  pl3d = new TPolyLine3D(2, xk, yk, zk);
  pl3d->SetLineColor(1);
  pl3d->SetLineWidth(2);
  pl3d->Draw();
  //
  Int_t ncolors  = gStyle->GetNumberOfColors();
  TAxis *xa = projxy->GetXaxis();
  TAxis *ya = projxy->GetYaxis();

  Double_t xc, yc, zc, bwidth, range;
  // draw xy proj at bottom, fixed z
  zc = za3->GetXmin() - zw;
  // thickness of the boxes
  bwidth = 0.25*zw;
  range = xymax-xymin;
  for(int xb=1; xb<=xa->GetNbins(); xb++) {
    for(int yb=1; yb<=ya->GetNbins(); yb++) {
      int binc = projxy->GetBinContent(xb, yb);
      if (binc > 0) {
        xc = xa->GetBinCenter(xb);
        yc = ya->GetBinCenter(yb);
        pm3d = new TMarker3DBox(yc,xc,zc,yw,xw,bwidth, 0, 0);
        // color number
        int nc = ncolors * binc / range;
        pm3d->SetLineColor(gStyle->GetColorPalette(nc));
        pm3d->SetFillColor(gStyle->GetColorPalette(nc));
        pm3d->SetFillStyle(1);
        pm3d->Draw();
      }
    }
  }
  // draw zy proj at right side, fixed x
  xc = xa3->GetXmax() + xw;
  bwidth = 0.25*xw;
  range = yzmax-yzmin;
  for(int xb=1; xb<=xa->GetNbins(); xb++) {
    for(int yb=1; yb<=ya->GetNbins(); yb++) {
      int binc = projyz->GetBinContent(xb, yb);
      if (binc > 0) {
        yc = xa->GetBinCenter(xb);
        zc = ya->GetBinCenter(yb);
        pm3d = new TMarker3DBox(xc,zc,yc,bwidth,zw,yw, 0, 0);
        // color number
        int nc = ncolors * binc / range;
        pm3d->SetLineColor(gStyle->GetColorPalette(nc));
        pm3d->SetFillColor(gStyle->GetColorPalette(nc));
        pm3d->SetFillStyle(1);
        pm3d->Draw();
      }
    }
  }
  // draw zx proj at back side, fixed y
  yc = ya3->GetXmax() + yw;
  bwidth = 0.25*yw;
  range = xzmax-xzmin;
  for(int xb=1; xb<=xa->GetNbins(); xb++) {
    for(int yb=1; yb<=ya->GetNbins(); yb++) {
      int binc = projxz->GetBinContent(xb, yb);
      if (binc > 0) {
        xc = xa->GetBinCenter(xb);
        zc = ya->GetBinCenter(yb);
        pm3d = new TMarker3DBox(zc,yc,xc,zw,bwidth,xw, 0, 0);
        // color number
        int nc = ncolors * binc / range;
        pm3d->SetLineColor(gStyle->GetColorPalette(nc));
        pm3d->SetFillColor(gStyle->GetColorPalette(nc));
        pm3d->SetFillStyle(1);
        pm3d->Draw();
      }
    }
  }
  // now the 3d hist itself
  range = h3->GetMaximum();
  for(int xb=0; xb<=h3->GetNbinsX(); xb++) {
    for(int yb=0; yb<=h3->GetNbinsY(); yb++) {
      for(int zb=0; zb<=h3->GetNbinsZ(); zb++) {
        int bin = h3->GetBin(xb, yb, zb);
        int binc = h3->GetBinContent(bin);
        if (binc > 0) {
          xc = xa3->GetBinCenter(xb);
          yc = ya3->GetBinCenter(yb);
          zc = za3->GetBinCenter(zb);
          xw = 0.5*xa3->GetBinWidth(xb);
          yw = 0.5*ya3->GetBinWidth(yb);
          zw = 0.5*za3->GetBinWidth(zb);
          pm3d = new TMarker3DBox(xc,yc,zc,xw,yw,zw, 0, 0);
          int nc = ncolors * binc / range;
          pm3d->SetLineColor(gStyle->GetColorPalette(nc));
          pm3d->SetFillColor(gStyle->GetColorPalette(nc));
          pm3d->SetFillStyle(1);
          pm3d->Draw();
        }
      }
    }
  }
  view->Draw();
  TGLViewer *glv = (TGLViewer*)c2->GetViewer3D("ogl");
  cout << "TGLViewer *glv =(TGLViewer*)" << glv << endl;
  glv->DoDraw();
}
