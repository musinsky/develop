// 2007-10-09

#include <Riostream.h>
#include <TString.h>

void seg_read_files(Int_t q1, Int_t q2)
{
  ifstream inputFile;
  inputFile.open(TString::Format("seg%d%d.txt", q1, q2));
  if (!inputFile.is_open()) return;
  TString inputLine;
  Double_t x1[4], y1[4], x2[4], y2[4];
  Int_t nLine = 0, nTangent = 0;

  while (1) {
    if (inputFile.eof()) break;
    inputLine.ReadLine(inputFile);
    if (!inputLine.Contains(TString::Format("Line %d Tangent", nLine + 1))) continue;
    //    Printf("%s", inputLine.Data());
    inputLine.Replace(0, 18 + 1, "");
    //    Printf("%s", inputLine.Data());
    //    Printf("%20.14f", inputLine.Atof());
    if (nTangent == 0) x1[nLine] = inputLine.Atof();
    if (nTangent == 1) y1[nLine] = inputLine.Atof();
    if (nTangent == 2) x2[nLine] = inputLine.Atof();
    if (nTangent == 3) y2[nLine] = inputLine.Atof();
    if ((nTangent++) == 3) { nLine++; nTangent = 0; } // next line & reset
  }
  inputFile.close();

  Double_t a, b;
  Int_t order[4] = {1, 0, 3, 2};
  for (Int_t i = 0; i < 4; i++) {
    //    Printf("%20.14f", x1[order[i]]);
    //    Printf("%20.14f", y1[order[i]]);
    //    Printf("%20.14f", x2[order[i]]);
    //    Printf("%20.14f", y2[order[i]]);
    a = (y2[order[i]] - y1[order[i]])/(x2[order[i]] - x1[order[i]]);
    b = y1[order[i]] - a*x1[order[i]];
    Printf("%20.14f, %20.14f", a, b);
  }
}
