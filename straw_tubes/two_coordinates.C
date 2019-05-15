// 2004-10-27

void two_coordinates(Float_t alf0 = 0, Float_t b0 = 0,
                     Float_t alf1 = 999, Float_t b1 = 0)
{
  // sustava ZX1 je v 4 kvadrante sustavy ZX0
  // preveruje len tuto konfiguraciu
  // !!! alf1 nie je potrebne !!!

  /*
    X0                    X1
    |                     /
    |                    /
    +-------------------+-------------Z0
    |                  /
    |           \     /
    |             \  /
    |               +
    |              /  \
    |             /     \
    |            /        \ Z1
  */

  Float_t dis   = 18.4;    // prienik osy X1 a Z0
  Float_t theta = -19.5;   // uhol  medzi Z1 a Z0
  theta = TMath::DegToRad()*theta;

  Float_t a = -1.0/TMath::Tan(theta);
  Float_t b = - a*dis;
  cout << "rovnica osy X1 v systeme ZX0 je: " << endl;
  cout << " X = " << a << " * Z + " << b << endl;

  Float_t a0 = TMath::Tan(TMath::DegToRad()*alf0);
  Float_t a1 = TMath::Tan(TMath::DegToRad()*alf1);

  // prienik osy X1 a track-u

  Float_t tmp   = ( a - a0);
  Float_t tmp_z = (b0 - b );
  Float_t tmp_x = (a*b0 - a0*b);

  Float_t z = tmp_z/tmp;
  Float_t x = tmp_x/tmp;

  cout << "prienik priamky: " << endl;
  cout << " X = " << a0 << " * Z + " << b0 << endl;
  cout << "s vyssie uvedenou osou je v bode: " << endl;
  cout << "     Z = " << z << "    a   X = " << x << endl;
  cout << " const =  " << x - (b1*TMath::Cos(theta)) << endl;
}
