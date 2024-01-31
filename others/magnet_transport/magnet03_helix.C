// 2004-12-02 original
// 2005-11-07 update to vvg values
// 2019-05-14 update to ROOT6
// Musinsky Jan

void Helix(const Double_t *v_in, Double_t *v_out, const Double_t v_step);

void magnet03_helix()
{
  //    +y ( = -x)  +z ( = +y) px ( = +pz)  py ( = -px) pz ( = +py) p
  //    -2.80654    4.04591    3.48055      -0.08843    0.13416     3.484257
  //    1.22906     -5.51242   3.48078      0.05533     -0.15023    3.484460
  //    0.10443     0.90350    3.49812      -0.04026    0.02749     3.498460
  //    4.27989     5.37731    3.48660      0.07467     0.10899     3.489102
  //    6.68293     -0.61275   3.47090      0.18530     -0.06083    3.476375

  Double_t in[5]={2.80654,0.0,0.08843/3.484257,3.48055/3.484257,3.484257};
  Double_t step = 151.2555;
  Double_t in_b[5]={-1.68993, 151, -0.0847749, 0.995656, 3.48426 };
  Double_t step_b = -151.2555;

  //  Double_t in[5]={-6.68293,100.0,-0.1853/3.476375,3.47090/3.476375,3.476375};
  //  Double_t step = 152.001;
  //  Double_t in_b[5]={-23.1928, 251, -0.163707, 0.986354, 3.47637};
  //  Double_t step_b = -152.001;

  Double_t out[5]={0};

  Helix(in,out,step);
  cout << "x  = " << out[0] << endl;
  cout << "z  = " << out[1] << endl;
  cout << endl;
  cout << "px = " << out[2]*out[4] << endl;
  cout << "pz = " << out[3]*out[4] << endl;

  Helix(in_b,out,step_b);
  cout << "x  = " << out[0] << endl;
  cout << "z  = " << out[1] << endl;
  cout << endl;
  cout << "px = " << out[2]*out[4] << endl;
  cout << "pz = " << out[3]*out[4] << endl;
}
void Helix(const Double_t *v_in, Double_t *v_out, const Double_t v_step)
{
  // units Tesla, cm, GeV/c
  Double_t mag_field = 0.847845;
  Double_t h4  = 0.00299792458*mag_field;
  Double_t rho = -h4/v_in[4];

  ////////
  Double_t calculate_arc;
  calculate_arc = 2*TMath::ASin(151.0*rho/2.0)/rho;
  cout << "calculate_arc = " << calculate_arc << endl << endl;
  ///////

  Double_t tet = rho*v_step;
  Double_t sint, sintt, tsint, cos1t, sin2;

  //  if ( TMath::Abs(tet) > 0.15) {
  sint  = TMath::Sin(tet);
  sintt = sint/tet;
  tsint = (tet-sint)/tet;
  sin2  = TMath::Sin(0.5*tet);
  cos1t = 2.0*(sin2*sin2)/tet;
  //  }
  //  else {
  //    tsint = tet*tet/6.0;
  //    sintt = (1.0 - tsint);
  //    sint  = tet*sintt;
  //    cos1t = 0.5*tet;
  //  }

  Double_t f1 = v_step*sintt;
  Double_t f2 = v_step*cos1t;
  Double_t f4 = -tet*cos1t;
  Double_t f5 = sint;

  // 0, 1, 2,  3,  4
  // x, z, px, pz, p

  Double_t hxp0 =   v_in[3]; // !!!  + !!!
  Double_t hxp1 =  -v_in[2]; // !!!  - !!!

  v_out[0] = v_in[0] + (f1*v_in[2] + f2*hxp0);
  v_out[1] = v_in[1] + (f1*v_in[3] + f2*hxp1);

  v_out[2] = v_in[2] + (f4*v_in[2] + f5*hxp0);
  v_out[3] = v_in[3] + (f4*v_in[3] + f5*hxp1);
  v_out[4] = v_in[4];
}
