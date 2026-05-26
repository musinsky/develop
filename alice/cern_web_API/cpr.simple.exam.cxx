// -*- mode: c++-ts -*-
// 2026-05-26

// https://github.com/libcpr/cpr
// https://docs.libcpr.dev/
//
// $ sudo dnf install cpr-devel

// export TOKEN='xxx'
// $ g++ cpr.simple.exam.cxx -o cpr.simple.exam.exe -lcpr && ./cpr.simple.exam.exe

#include <cstdlib>
#include <iostream>
#include <cpr/cpr.h>

int main()
{
  const char* token = std::getenv("TOKEN");
  if (!token) {
    std::cout << "$TOKEN variable is not set" << std::endl;
    return 1;
  }

  cpr::Response r = cpr::Get(cpr::Url{"https://ali-bookkeeping.cern.ch/api/lhcFills"},
                             cpr::Parameters{{"filter[fillNumbers]", "11745"},
                                             {"filter[hasStableBeams]", "true"},
                                             {"token", token}});
  std::cout << r.text << std::endl;
  std::cout << "\nHTTP status code: " << r.status_code << std::endl;
  std::cout << "header[\"content-type\"]: " << r.header["content-type"] << std::endl;

  return 0;
}
