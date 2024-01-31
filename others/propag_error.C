// 2006-05-09 original
// 2019-05-14 update to ROOT6
// Musinsky Jan

{
  const Int_t n  = 10;
  Double_t x[n]  = {1,   5,   12,  16,  22,  26,  33,  35,  40,  44};
  Double_t y[n]  = {1.3, 1.8, 3.6, 4.4, 5.1, 6.2, 7.3, 8.0, 8.9, 9.7};
  Double_t ex[n] = {0};
  Double_t ey[n] = {0.3,  0.4,  0.2,  0.4,  0.5,  0.2,  0.2,  0.3,  0.4, 0.3};

  Double_t offset = 10000;
  Double_t mey    = 1; // try 100 or 1000
  for (Int_t i = 0; i < n; i++) {
    x[i]  += offset;
    ey[i] /= mey;
  }

  TGraph *gr = new TGraphErrors(n,x,y,ex,ey);
  gr->SetMarkerStyle(8);
  gr->Draw("AP");
  gr->Fit("pol1");

  // f(x) = a0 + x*a1
  TF1 *pol1 = gr->GetFunction("pol1");
  // propagation of error
  // sigma_f(x)^2 = sigma_a0^2 + (x*sigma_a1)^2
  Double_t sigma_a0 = pol1->GetParError(0);
  Double_t sigma_a1 = pol1->GetParError(1);
  Double_t value    = 50 + offset;
  Double_t sigma_f  = TMath::Sqrt(TMath::Power(sigma_a0, 2) +
                                  TMath::Power((value*sigma_a1), 2));
  Printf("\npropagation error, without covariance");
  Printf("pol1(%g) = %g +-  %g",value,pol1->Eval(value),sigma_f);

  // covariance matrix
  TVirtualFitter *fitter = TVirtualFitter::GetFitter();
  Int_t npar = pol1->GetNpar(); // = 2
  TMatrixD matrix(npar,npar,fitter->GetCovarianceMatrix());
  matrix.Print();
  // to same with Minuit (but only Minuit)
  // Double_t *covar = new Double_t[npar*npar]
  // gMinuit->mnemat(covar,npar);

  // propagation of error (with covariance between a0 and a1)
  // sigma_f(x)^2 = sigma_a0^2 + (x*sigma_a1)^2 + 2*cov*x
  sigma_a0 = TMath::Sqrt(fitter->GetCovarianceMatrixElement(0,0));
  sigma_a1 = TMath::Sqrt(fitter->GetCovarianceMatrixElement(1,1));
  Double_t cov = fitter->GetCovarianceMatrixElement(0,1); // == (1,0)
  sigma_f = TMath::Sqrt(TMath::Power(sigma_a0, 2) +
                        TMath::Power((value*sigma_a1), 2) + 2*cov*value);
  Printf("\npropagation error, with covariance");
  Printf("pol1(%g) = %g +-  %g",value,pol1->Eval(value),sigma_f);

  // manual calculate (without ROOT fitter)
  Double_t aa = 0, bb = 0, cc = 0, dd = 0, ee = 0, ff = 0;
  for (Int_t j = 0; j < n; j++) {
    aa += x[j]/(ey[j]*ey[j]);
    bb += 1.0 /(ey[j]*ey[j]);
    cc += y[j]/(ey[j]*ey[j]);
    dd += (x[j]*x[j])/(ey[j]*ey[j]);
    ee += (x[j]*y[j])/(ey[j]*ey[j]);
    ff += (y[j]*y[j])/(ey[j]*ey[j]);
  }
  Double_t den = dd*bb - aa*aa;
  Double_t a0  = (dd*cc - ee*aa)/den;
  Double_t a1  = (ee*bb - cc*aa)/den;
  sigma_a0     =  sqrt(dd/den);
  sigma_a1     =  sqrt(bb/den);
  cov          =  -aa/den;
  sigma_f      = TMath::Sqrt(TMath::Power(sigma_a0, 2) +
                             TMath::Power((value*sigma_a1), 2) + 2*cov*value);
  Printf("\npropagation error, manual calculate covariance");
  Printf("pol1(%g) = %g +-  %g",value,a0+a1*value,sigma_f);
}
