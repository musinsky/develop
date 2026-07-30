// 2026-07-30

// $ g++     -O2 -pedantic -Wall -Wextra nlohmann.01.cxx -o nlohmann.01.gcc
// $ clang++ -O2 -pedantic -Wall -Wextra nlohmann.01.cxx -o nlohmann.01.clang

//          ccdb.sample.big.json     ccdb.sample.normal.json
// optimizations     GCC   CLANG                 GCC   CLANG
//           -O0   20.8s   22.9s               0.18s   0.20s
//           -O1    2.7s    2.3s               0.03s   0.02s
//           -O2    2.5s    2.3s               0.02s   0.02s
//           -O3    2.4s    2.1s               0.02s   0.02s
//
// nlohmann::json benefits significantly from compiler optimization. Enabling -O1 provides
// a dramatic performance improvement over -O0, mainly because the library is header-only
// and relies heavily on template inlining and other compile-time optimizations.

#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main(int argc, char* argv[])
{
  // "ccdb.sample.big.json";     // 228 MB, ~ 245.000 objects
  // "ccdb.sample.normal.json"   //   2 MB, ~   2.000 objects
  // "ccdb.sample.small.json"    //  35 KB, ~      30 objects

  std::string fn = "ccdb.sample.big.json";
  if (argc == 2) {
    fn = argv[1];
  }
  std::ifstream in(fn);
  if (!in) {
    std::cerr << "Cannot open file: '" << fn << "'\n";
    return 1;
  }
  std::cout << "Parsing JSON file: '" << fn << "'\n";

  // https://stackoverflow.com/q/33628250
  // https://json.nlohmann.me/features/parsing/
  // https://json.nlohmann.me/api/basic_json/parse/
  // https://json.nlohmann.me/api/operator_gtgt/

  auto start = std::chrono::steady_clock::now();

  json j_complete1 = json::parse(in); // prefer DOM parsing method
  std::cout << "Number of objects (1): " << j_complete1["objects"].size() << std::endl;

  auto stop = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = stop - start; // std::seconds by default
  std::cout << std::fixed << std::setprecision(2) << "Elapsed time (1): " << elapsed.count() << " seconds" << std::endl;

  in.clear();
  in.seekg(0, std::ios::beg); // in.seekg(0)
  start = std::chrono::steady_clock::now();

  json j_complete2;
  in >> j_complete2; // alternative DOM parsing method
  std::cout << "Number of objects (2): " << j_complete2["objects"].size() << std::endl;

  stop = std::chrono::steady_clock::now();
  elapsed = stop - start;
  std::cout << std::fixed << std::setprecision(2) << "Elapsed time (2): " << elapsed.count() << " seconds" << std::endl;

  std::cout << "\nNo explicit cleanup is necessary at the end of the program.\n"
               "The nlohmann::json object's destructor automatically frees all allocated memory."
            << std::endl;
  return 0;
}
