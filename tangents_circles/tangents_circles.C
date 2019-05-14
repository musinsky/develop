// 2007-10-09

#include <TString.h>
#include <TMath.h>

void swap(Double_t *value)
{
  Double_t temp = value[0];
  value[0] = value[1];
  value[1] = temp;
}

void quad(const Int_t q, Int_t &sx, Int_t &sy)
{
  switch (q) {
  case (1): { sx =  1; sy =  1; break; }
  case (2): { sx = -1; sy =  1; break; }
  case (3): { sx = -1; sy = -1; break; }
  case (4): { sx =  1; sy = -1; break; }
  default:  { sx =  0; sy =  0; break; }
  }
}

void tangents_circles(Int_t q1, Int_t q2)
{
  Int_t signx[2] = {0}, signy[2] = {0};
  quad(q1, signx[0], signy[0]);
  quad(q2, signx[1], signy[1]);

  Double_t x[2] = {signx[0]*3.0, signx[1]*7.0 };
  Double_t y[2] = {signy[0]*3.0, signy[1]*7.0 };
  Double_t r[2] = {  2.0, 0.5 };
  if (x[0] > x[1]) { swap(x); swap(y); swap(r); }

  Double_t xd     = x[0] - x[1];
  Double_t yd     = y[0] - y[1];
  Double_t rm     = r[0] - r[1];
  Double_t rp     = r[0] + r[1];
  Double_t xd2    = xd*xd;
  Double_t yd2    = yd*yd;
  Double_t xdyd   = xd*yd;
  Double_t sxdyd2 = xd2 + yd2;
  Double_t rm2    = rm*rm;
  Double_t rp2    = rp*rp;

  Double_t a11    = xdyd + rm*TMath::Sqrt(sxdyd2 - rm2);
  Double_t a12    = xdyd - rm*TMath::Sqrt(sxdyd2 - rm2);
  Double_t b1     = rm2 - xd2;
  Double_t a21    = xdyd + rp*TMath::Sqrt(sxdyd2 - rp2);
  Double_t a22    = xdyd - rp*TMath::Sqrt(sxdyd2 - rp2);
  Double_t b2     = rp2 - xd2;

  Double_t ax[4] = {0}, bx[4] = {0};
  ax[0] = -a11/b1;
  ax[1] = -a12/b1;
  ax[2] = -a21/b2;
  ax[3] = -a22/b2;

  /*
  // calculate with trigonometric functions is ~2x slowly
  Double_t dis = TMath::Sqrt(sxdyd2);
  Double_t an0 = TMath::ATan2(yd, xd);
  Double_t an1 = TMath::ASin(rm/dis);
  Double_t an2 = TMath::ASin(rp/dis);
  ax[0] = TMath::Tan(an0 + an1);
  ax[1] = TMath::Tan(an0 - an1);
  ax[2] = TMath::Tan(an0 + an2);
  ax[3] = TMath::Tan(an0 - an2);
  */

  bx[0] = -ax[0]*x[1]+y[1]-r[1]*TMath::Sqrt(1+ax[0]*ax[0]);
  bx[1] = -ax[1]*x[1]+y[1]+r[1]*TMath::Sqrt(1+ax[1]*ax[1]);
  bx[2] = -ax[2]*x[1]+y[1]+r[1]*TMath::Sqrt(1+ax[2]*ax[2]);
  bx[3] = -ax[3]*x[1]+y[1]-r[1]*TMath::Sqrt(1+ax[3]*ax[3]);

  if (xd == 0) {
    Printf("something wrong");
    bx[0]=-ax[0]*x[1]+y[1]+r[1]*TMath::Sqrt(1+ax[0]*ax[0]);
    bx[3]=-ax[3]*x[1]+y[1]+r[1]*TMath::Sqrt(1+ax[3]*ax[3]);
  }

  for (Int_t s = 0; s < 4; s++)
    Printf("%20.14f, %20.14f", ax[s], bx[s]);
}
