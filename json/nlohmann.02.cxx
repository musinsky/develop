// 2026-07-30

// $ g++ -g -O2 -pedantic -Wall -Wextra nlohmann.02.cxx -o nlohmann.02
//       -g # --debug for memory profiling tool (Massif)

// https://valgrind.org/docs/manual/ms-manual.html
//
// $ valgrind --tool=massif                ./nlohmann.02   // default time unit is instructions executed (i)
// $ valgrind --tool=massif --time-unit=B  ./nlohmann.02   // bytes allocated/deallocated on the heap and/or stack (B)
// $ valgrind --tool=massif --time-unit=ms ./nlohmann.02   // real (wallclock) time (ms, i.e. milliseconds)
// $ ms_print massif.out.PID >  massif.out.PID.print
//
// $ valgrind --tool=memcheck --leak-check=full ./nlohmann.02

// "ccdb.sample.big.json";     // 228 MB, ~ 245.000 objects (allocated ~ 910 MB)
// "ccdb.sample.normal.json"   //   2 MB, ~   2.000 objects (allocated ~ 7.7 MB)
// "ccdb.sample.small.json"    //  35 KB, ~      30 objects (allocated ~ 210 KB)
// "dummy.json"                //  empty                    (allocated ~  80 KB)
//
// # keep only the "id", "validFrom" and "validUntil" key/value pair and discard all other key/value pairs
// $ cp ccdb.sample.normal.json           ccdb.sample.normal.shrink.json
// $ sed -i 's/^.*,"id"/{"id"/g'          ccdb.sample.normal.shrink.json
// $ sed -i 's/,"initialValidity".*}/}/g' ccdb.sample.normal.shrink.json
//
// "ccdb.sample.normal.shrink.json" // 190 KB, ~ 2.000 objects (allocated ~ 960 KB)
//
// Pouzitie "shrink" JSON suboru moze byt vhodnejsie pre testovanie spotreby pamate v pripade ak si ukladame potrebne
// hodnoty vo vlastnej strukture. Rozdiel medzi alokovanou pamatou pre nlohmann::json objekt a alokovanou
// (spotrebovanou) pamatou pre vlastnu strukturu nie je v pripade "shrink" JSON suboru az taky dramaticky ako v pripade
// originalneho JSON suboru (rozdiel medzi peak-mi v Massif out file sa nestraca).
//
// valgrind --tool=massif --massif-out-file=massif.shrink --time-unit=ms ./nlohmann.02 ccdb.sample.normal.shrink.json
// ms_print massif.shrink > massif.shrink.print

// === Parsovanie JSON (CCDB) pomocou nlohmann::json a pamatova optimalizacia ===
//
// https://json.nlohmann.me/features/parsing
// https://simdjson.github.io/simdjson/md_doc_2ondemand__design.html   // !!!
// https://rapidjson.org/md_doc_features.html
//
//
// V pripade JSON suborov s velkym poctom objektov je potrebne davat pozor na to, ze nlohmann::json pouziva (by default)
// DOM parser a teda cely JSON subor sa nacita do pamate. Pre JSON CCDB subory (mnozstvo objektov s rovnakou strukturou)
// je faktor cca 4-5x ako velky je JSON subor a kolko pamate sa alokuje pre nlohmann::json objekt. Napr. pre subor
// 'ccdb.sample.big.json'  (228 MB, ~ 245.000 objects) sa alokuje ~ 910 MB pamate, resp. pre
// 'ccdb.sample.normal.json' (2 MB, ~   2.000 objects) sa alokuje ~   8 MB pamate.
//
// Treba si uvedomit, ze pre kazdy JSON "key/value" pair (prevazne string data type) nlohmann parser alokuje cast pamate
// (vnutorne asi v std::map a std::string). Pre JSON CCDB subory, kde je mnozstvo objektov s rovnakou strukturou, sa
// alokuje uplne zbytocne 245.000x ten isty "key" string (napr. "Last-Modified"), pricom realne potrebujeme 245.000x
// roznych "value" integer. Tychto opakujucich sa "key" strings (from "key/value" pair) byva v JSON CCDB suboroch cca
// 20-30 (zavisi od konkretnej CCDB databazy), takze je to znacne plytvanie pamatou. Realne su potrebne iba niektore
// "value" (from "key/value" pair) z JSON CCDB objektov (napr. "id", "validFrom", "validUntil").
//
// Existuje viac moznosti ako riesit tento problem.
//
// 1) Ignorujeme problem. Realne budeme parsovat JSON subory, ktore budu alokovat 10-100 MB pamate (no problem).
//
// 2) Pouzijeme callback parser, ktory umoznuje filtrovat objekty (vyberat len potrebne "key/value" pair) pocas
// parsovania a tym usetrit pamat vysledneho nlohmann::json objektu. Stale vsak budeme mat alokovanych 245.000x ten isty
// "key" string (napr. "validFrom") pre potrebny JSON CCDB objekt.
// Tato metoda je vseobecne vhodna ak potrebujeme vynechat niektore JSON "key/value" pair pocas parsovania, ale stale
// chceme mat nlohmann::json objekt (napr. pre dalsie spracovanie).
//
// 3) Pouzijeme callback parser, ale prefiltrovane objekty si ulozime do nejakej vlastnej struktury, ktora bude
// obsahovat iba potrebne "value" (from "key/value" pair). Samotny nlohmann::json objekt nebudeme zaplnat vobec, resp.
// neskor ho komplet vymazeme.
// Tato metoda je vhodna ak potrebujeme iba niektore JSON "key/value" a nepotrebujeme nlohmann::json objekt. Pracujeme
// iba s potrebnymi hodnotami alokovanymi vo vlastnej strukture.
//
// 4) Pouzijeme nlohmann SAX parser, ktory je stream parser. SAX parser je vhodny ak potrebujeme iba niektore JSON
// "key/value" a nepotrebujeme cely nlohmann::json objekt. Pracujeme iba s potrebnymi hodnotami alokovanymi vo vlastnej
// strukture (alebo triede). Je to pravdepodobne najvhodnejsia metoda pre JSON subory s velkym poctom objektov a ktore
// maju rovnaku strukturu (ako su prave JSON CCDB), ale zaroven najnarocnejsia na implementaciu. User class inheriting
// from json::json_sax_t nasobne ulahcuje implementaciu SAX parseru.
//
// 5) Pouzijeme iny JSON C++ parser: simdjson (rychly stream parser), rapidjson (DOM/SAX parser, pouziva sa v Alice O2).
//
// Pouzitie callback parser-u umonzuje relativne "jednoduchsie" filtrovanie JSON suborov pocas parsovania, ale vyzaduje
// istu mieru "opatrnosti" pri implementacii. Vhodnejsie je uz potom asi pouzit priamo SAX parser.
//
// Najjednoduchsia implementacia je pouzit cely nlohmann::json objekt (moznost 1) a z neho potom do vlastnej struktury
// nakopirovat len potrebne hodnoty. Ano, alokuje sa podstatne viac pamate, ale ihned po skopirovani potrebnych hodnot
// do vlastnej struktury, uz nepotrebnu alokovanu pamat (cely nlohmann::json objekt) uvolnime, zmazeme. Casovy rozdiel
// je minimalny.
//
// V pripade skopirovania hodnot z JSON suboru do vlastnej struktury sa ponuka aj moznost tuto strukturu ulozit do extra
// suboru (textovy, binarny) a potom ju rychlo a efektivne opakovane nacitavat. Vyhneme sa opakovanemu parsovaniu JSON.

#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <thread>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct ccdb_entry {
  long long validFrom;
  long long validUntil;
  std::string id;
};

void timer_chrono(bool start)
{
  static auto now = std::chrono::steady_clock::now();
  if (start) {
    now = std::chrono::steady_clock::now();
    return;
  }
  std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - now; // std::seconds by default
  std::cout << std::fixed << std::setprecision(2) << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;
}
void sleep_valgrind(bool verbose = true, int seconds = 1)
{
  // adding a sleep_for() allows Massif (valgrind) to measure the memory usage
  // of the nlohmann::json object before it is destroyed on program exit
  std::cout << "# sleep_for (" << seconds << " seconds) start (valgrind Massif)" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(seconds));
  std::cout << "# sleep_for (" << seconds << " seconds) stop  (valgrind Massif)" << std::endl;
  if (verbose) {
    std::cout << "No explicit cleanup is necessary at the end of the function.\n"
                 "The nlohmann::json object's destructor automatically frees all allocated memory."
              << std::endl;
  }
}
void parse_complete(const std::string& filename)
{
  std::cout << "\n===== " << __func__ << " =====\n";
  std::ifstream in(filename);
  if (!in) {
    std::cerr << "Cannot open file: '" << filename << "'\n";
    return;
  }
  std::cout << "Parsing JSON file: '" << filename << "'\n";

  timer_chrono(true);
  json j_complete = json::parse(in); // prefer DOM parsing method
  timer_chrono(false);

  std::cout << "Number of objects: " << j_complete["objects"].size() << std::endl;
  // std::cout << std::setw(2) << j_complete["objects"][0] << std::endl;
  sleep_valgrind();
}
void parse_callback(const std::string& filename, bool discard_arrays = false)
{
  std::cout << "\n===== " << __func__ << " =====\n";
  std::ifstream in(filename);
  if (!in) {
    std::cerr << "Cannot open file: '" << filename << "'\n";
    return;
  }
  std::cout << "Parsing JSON file: '" << filename << "'\n";
  if (discard_arrays) {
    std::cout << "Discarding array of objects, discarding all key/value pairs" << std::endl;
  } else {
    std::cout << "Keeping array of objects, discarding only ID key/value pair" << std::endl;
  }

  bool is_ID = false;
  std::vector<std::string> IDs;
  // https://json.nlohmann.me/features/parsing/parser_callbacks
  // https://json.nlohmann.me/api/basic_json/parser_callback_t/
  //
  // define parser callback
  json::parser_callback_t cb = [&is_ID, &IDs, &discard_arrays](int depth, json::parse_event_t event, json& parsed) {
    // std::cout << "DEPTH: " << depth << " EVENT: " << static_cast<int>(event) << " PARSED: " << parsed.dump() << '\n';

    if ((depth == 3) && (event == json::parse_event_t::key)) {
      const auto& key = parsed.get_ref<const std::string&>();
      if (key == "id") {
        is_ID = true;
        return false; // discard this key
      }
    }
    if (is_ID && (depth == 3) && (event == json::parse_event_t::value)) {
      const auto& value = parsed.get_ref<const std::string&>();
      // std::cout << "ID: " << value << std::endl;
      IDs.push_back(value);
      is_ID = false;
      return false; // discard this value
    }

    if (discard_arrays) { // discard array of objects (and array of subfolders) at any depth
      if ((event == json::parse_event_t::array_start) || (event == json::parse_event_t::array_end)) {
        return false;
      }
    }

    return true; // must be true to continue parsing
  };

  timer_chrono(true);
  json j_filtered = json::parse(in, cb);
  timer_chrono(false);

  std::cout << "Number of objects: " << j_filtered["objects"].size() << std::endl;
  std::cout << "Size of IDs (vector): " << IDs.size() << std::endl;
  // if (discard_arrays) {
  //   std::cout << std::setw(2) << j_filtered << std::endl;
  // } else {
  //   std::cout << std::setw(2) << j_filtered["objects"][0] << std::endl;
  // }
  sleep_valgrind();
}
std::vector<ccdb_entry> parse_complete_ccdb_entry(const std::string& filename)
{
  std::cout << "\n===== " << __func__ << " =====\n";
  std::vector<ccdb_entry> entries;
  std::ifstream in(filename);
  if (!in) {
    std::cerr << "Cannot open file: '" << filename << "'\n";
    return entries;
  }
  std::cout << "Parsing JSON file: '" << filename << "'\n";

  timer_chrono(true);
  json j_complete = json::parse(in); // prefer DOM parsing method
  timer_chrono(false);

  std::cout << "Number of objects: " << j_complete["objects"].size() << std::endl;
  sleep_valgrind(false);

  timer_chrono(true);
  entries.reserve(j_complete["objects"].size());
  for (const auto& obj : j_complete["objects"]) {
    ccdb_entry entry;
    // entry.validFrom = obj["validFrom"].get<long long>();
    // entry.validUntil = obj["validUntil"].get<long long>();
    // entry.id = obj["id"].get<std::string>();

    // using .value() with a default value is safer in case the key is missing
    entry.validFrom = obj.value("validFrom", 0LL);
    entry.validUntil = obj.value("validUntil", 0LL);
    entry.id = obj.value("id", "");

    entries.push_back(entry);
  }
  timer_chrono(false);

  sleep_valgrind();
  return entries;
}

int main(int argc, char* argv[])
{
  std::string fn = "ccdb.sample.normal.json";
  if (argc == 2) {
    fn = argv[1];
  }

  parse_complete(fn);
  parse_callback(fn, false); // keep array of objects, discard only ID key/value pair
  parse_callback(fn, true);  // discard array of objects, discard all key/value pairs
  parse_complete(fn);

  {
    std::vector<ccdb_entry> entries = parse_complete_ccdb_entry(fn);
    std::cout << "\n===== " << __func__ << " =====\n";
    std::cout << "Number of ccdb entries: " << entries.size() << std::endl;
    std::cout << "First ccdb entry: validFrom=" << entries[0].validFrom << ", validUntil=" << entries[0].validUntil
              << ", id=" << entries[0].id << std::endl;
    std::cout << "Last ccdb entry:  validFrom=" << entries.back().validFrom
              << ", validUntil=" << entries.back().validUntil << ", id=" << entries.back().id << std::endl;

    sleep_valgrind(false);
    // std::vector entries will be destroyed at the end of the scope
  }

  sleep_valgrind(false);
  return 0;
}
