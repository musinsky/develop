// 2005-07-06
// https://root-forum.cern.ch/t/fitting-a-graph/2113

#include <TF1.h>
#include <TAxis.h>

/*-----------------------------------------------------------------------------*/
double CalculateBeta(double KE, double M)
{
  //calculate beta given the kinetic energy of a particle

  double Ene=M+KE;
  double pc=sqrt(Ene*Ene - M*M);

  double beta=pc/Ene;

  return beta;
}
/*-----------------------------------------------------------------------------*/
double BetheBloch(double KE,double M)
{
  const double LArRho = 1.390; // * g/cm3; // LAr (liquid) mass density

  double dEdX;

  const double Z=19;
  const double A=39.938;
  const double z=1;//charge of incident particle in unit of e

  double beta;
  beta=CalculateBeta(KE,M);

  double gamma;
  gamma=1/sqrt(1-beta*beta);

  const double I=0.000188;//MeV
  const double me=0.511;//Mev/c2

  double Wmax;
  Wmax=(2*me*beta*beta*gamma*gamma) / (1 + 2*gamma*me/M + (me/M)*(me/M));

  const double C1=0.1535;//MeV cm2/g

  double X,X0,X1;
  X0=0.201;
  X1=3;
  X=log10(beta*gamma);
  double delta;
  double C,a;
  C=-5.217;
  a=0.196;

  if(X<=X0)
    delta=0;
  else if(X<=X1)
    delta=2*log(10.)*X+C+a*(X1-X)*(X1-X)*(X1-X);
  else
    delta=2*log(10.)*X+C;

  dEdX= ( C1*(Z/A) * LArRho  * z*z /(beta*beta)) *
    (log(2*me*beta*beta*gamma*gamma*Wmax / (I*I)) -
     2*beta*beta - delta); //-Dshell??

  return dEdX;
}
/*-----------------------------------------------------------------------------*/
double BetheBlochRange(double Range,double M)// cm
{
  double r(0);
  double kin(0);
  double en(0);
  while(r<Range)
    {
      kin+=0.1; //step di 0.1 MeV
      en=BetheBloch(kin,M);
      r+=0.1/en;
    }
  return en;
}
/*-----------------------------------------------------------------------------*/
double BetheBlochFunction(double *x,double *par)
{
  return BetheBloch(x[0],par[0]);
}
/*-----------------------------------------------------------------------------*/
double dEdX_Range(double *x, double *par)
{
  return BetheBlochRange(x[0],par[0]);
}
/*-----------------------------------------------------------------------------*/
void PlotBetheBloch(void)
{
  const double M = 105.658; //Muon mass

  TF1 *BBF = new TF1("BBF",BetheBlochFunction,0.,1000.,1);
  BBF->SetParameter(0,M);

  // Labels
  BBF->Draw();
  BBF->SetTitle("dE/dX Vs <Kin>");
  BBF->GetXaxis()->SetTitle("<Kin> (MeV)");
  BBF->GetYaxis()->SetTitle("dE/dX (MeV/cm)");
  BBF->GetYaxis()->SetLabelSize(0.02);
  BBF->GetXaxis()->SetLabelSize(0.02);
}
