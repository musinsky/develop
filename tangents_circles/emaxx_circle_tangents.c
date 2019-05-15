// 2011-03-29
// http://e-maxx.ru/algo/circle_tangents
// http://e-maxx.ru/algo/src_circle_tangents
// http://e-maxx.ru/forum/viewtopic.php?id=348
// 2019-05-14
// https://cp-algorithms.com/geometry/tangents-to-two-circles.html
// https://www.geogebra.org/classic
// https://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Tangents_between_two_circles
// https://github.com/FunctionReturn/Moon/blob/master/frontend/examples/rope/third_party/wikibooks/circle-circle-tangents.js
// http://atlas-computing.web.cern.ch/atlas-computing/links/distDirectory/1.0.1/atrig/LVL2/t2muon/bmc_trig/c_tan.F
// http://atlas-computing.web.cern.ch/atlas-computing/links/AtlasReconstructionDir/Tracking/TrkUtilityPackages/TrkDriftCircleMath/src/TangentToCircles.cxx
//     /afs/cern.ch/atlas/software/builds/nightlies/copy/AtlasReconstruction/rel_2/Tracking/TrkUtilityPackages/TrkDriftCircleMath/src/TangentToCircles.cxx   # latest -> rel_2
//
// https://habr.com/ru/post/451642/

// # g++ emaxx_circle_tangents.c

#include <vector>
#include <math.h>
#include <stdio.h>

using std::vector;

// original 2011-03-29
struct pt {
  double x, y;

  pt operator- (pt p) {
    pt res = { x-p.x, y-p.y };
    return res;
  }
};

struct circle : pt {
  double r;
};

struct line {
  double a, b, c;
};

const double EPS = 1E-9;

double sqr (double a) {
  return a * a;
}

void tangents (pt c, double r1, double r2, vector<line> & ans) {
  double r = r2 - r1;
  double z = sqr(c.x) + sqr(c.y);
  double d = z - sqr(r);
  if (d < -EPS)  return;
  d = sqrt (abs (d));
  line l;
  l.a = (c.x * r + c.y * d) / z; // ? or - ?
  l.b = (c.y * r - c.x * d) / z; // ? or + ?
  l.c = r1;
  ans.push_back (l);
}

vector<line> tangents (circle a, circle b) {
  vector<line> ans;
  for (int i=-1; i<=1; i+=2)
    for (int j=-1; j<=1; j+=2)
      tangents (b-a, a.r*i, b.r*j, ans);
  for (size_t i=0; i<ans.size(); ++i)
    ans[i].c -= ans[i].a * a.x + ans[i].b * a.y;
  return ans;
}

// added 2019-05-14
int main() {
  circle first;
  first.x = 3;
  first.y = 3;
  first.r = 2;
  circle second;
  second.x = 7;
  second.y = 7;
  second.r = 0.5;
  vector<line> tng;
  tng = tangents(first, second);
  printf("original ax + by + c = 0\n");
  for (size_t i = 0; i < tng.size(); i++)
    printf("[%d] %.2f, %.2f, %.2f\n", i, tng[i].a, tng[i].b, tng[i].c);
  printf("geogebra ax + by + c = 0 (?! multiply by 2 ?!)\n");
  for (size_t i = 0; i < tng.size(); i++)
    printf("[%d] %.2f, %.2f, %.2f\n", i, 2*tng[i].a, 2*tng[i].b, 2*tng[i].c);
  printf("y = mx + b\n");
  for (size_t i = 0; i < tng.size(); i++)
    printf("[%d] %20.14f, %20.14f\n", i, -tng[i].a/tng[i].b, -tng[i].c/tng[i].b);

  return 1;
}
