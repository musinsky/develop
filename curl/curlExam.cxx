// 2026-08-20

// $ g++ -O2 -pedantic -Wall -Wextra -lcurl -o curlExam curlExam.cxx

#include <curl/curl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <utility>

#if LIBCURL_VERSION_NUM < 0x075300
#error "libcurl >= 7.83.0 is required (curl_easy_header() function)"
#endif

template <typename... Args>
std::string colorFormat(int colorCode, std::format_string<Args...> fmt, Args&&... args)
{
  std::string formattedText = std::format(fmt, std::forward<Args>(args)...);
  if (!isatty(fileno(stdout))) {
    return formattedText;
  }

  const bool bold = colorCode >= 30;
  const int sgr = std::abs(colorCode);
  return std::format("\033[{};{}m{}\033[0m", bold ? 1 : 0, sgr, formattedText);
}
void coutColorStart(int colorCode)
{
  if (!isatty(fileno(stdout))) {
    return;
  }
  const bool bold = colorCode >= 30;
  const int sgr = std::abs(colorCode);
  std::cout << "\033[" << (bold ? 1 : 0) << ";" << sgr << "m";
}
void coutColorStop()
{
  if (!isatty(fileno(stdout))) {
    return;
  }
  std::cout << "\033[0m" << std::endl; // std::flush;
}

CURLUcode curlParseURL(const char* fullUrl = "https://example.com:123/path/to/example?report=true#section321")
{
  // https://curl.se/libcurl/c/curl_url.html
  // https://curl.se/libcurl/c/curl_url_set.html
  // https://curl.se/libcurl/c/curl_url_get.html

  // curl terminology:   url = scheme://host:port/path?query#fragment
  // CURLU (nezamienat s CURL) je handle pre URL parser. Uzivatel nemusi pisat vlastny parser pre URL. CURLU je
  // samostatny handle pre URL parser a je nezavisly od CURL easy handle, t.j. nie je potrebne volat curl_easy_init().
  // Added in curl 7.62.0 (released in 2018-10).

  // curl_url_set(url, CURLUPART_QUERY, "filter=proton", CURLU_APPENDQUERY);
  // curl_url_set(url, CURLUPART_QUERY, "limit=50",      CURLU_APPENDQUERY);
  //
  // curl_url_set(url, CURLUPART_URL, "http://ali-qcdb-gpn.cern.ch:8083/qc/CTP/MO/RawData/input02/1784877935784", 0);
  // curl_url_set(url, CURLUPART_PATH, "/download/a60702af-8731-11f1-b68f-0aa14520a1a2", 0);
  // curl_url_set(url, CURLUPART_URL,  "/download/a60702af-8731-11f1-b68f-0aa14520a1a2", 0); // also works (relative URL)
  // curl_url_get(url, CURLUPART_URL, &fullResolvedUrl, 0)
  // => fullResolvedUrl = "http://ali-qcdb-gpn.cern.ch:8083/download/a60702af-8731-11f1-b68f-0aa14520a1a2"

  CURLU* url = curl_url();
  if (!url) {
    std::cerr << "curl_url() failed" << std::endl;
    return CURLUE_OUT_OF_MEMORY;
  }
  CURLUcode rc = curl_url_set(url, CURLUPART_URL, fullUrl, 0);
  if (rc == CURLUE_OK) {
    char* get = nullptr;
    if (curl_url_get(url, CURLUPART_URL, &get, 0) == CURLUE_OK) {
      std::cout << std::format("CURLUPART_URL:      {}\n", (get ? get : "n/a"));
      curl_free(get);
    }
    if (curl_url_get(url, CURLUPART_SCHEME, &get, 0) == CURLUE_OK) {
      std::cout << std::format("CURLUPART_SCHEME:   {}\n", (get ? get : "n/a"));
      curl_free(get);
    }
    if (curl_url_get(url, CURLUPART_HOST, &get, 0) == CURLUE_OK) {
      std::cout << std::format("CURLUPART_HOST:     {}\n", (get ? get : "n/a"));
      curl_free(get);
    }
    if (curl_url_get(url, CURLUPART_PORT, &get, CURLU_DEFAULT_PORT) == CURLUE_OK) {
      std::cout << std::format("CURLUPART_PORT:     {}\n", (get ? get : "n/a"));
      curl_free(get);
    }
    if (curl_url_get(url, CURLUPART_PATH, &get, 0) == CURLUE_OK) {
      std::cout << std::format("CURLUPART_PATH:     {}\n", (get ? get : "n/a"));
      curl_free(get);
    }
    if (curl_url_get(url, CURLUPART_QUERY, &get, 0) == CURLUE_OK) {
      std::cout << std::format("CURLUPART_QUERY:    {}\n", (get ? get : "n/a"));
      curl_free(get);
    }
    if (curl_url_get(url, CURLUPART_FRAGMENT, &get, 0) == CURLUE_OK) {
      std::cout << std::format("CURLUPART_FRAGMENT: {}\n", (get ? get : "n/a"));
      curl_free(get);
    }
  } else {
    std::cerr << colorFormat(31, "curl_url_set(CURLUPART_URL) failed: {}\n", curl_url_strerror(rc));
    std::cerr << colorFormat(31, "invalid fullUrl: '{}'\n", fullUrl);
  }

  curl_url_cleanup(url);
  return rc;
}
CURLcode curlDNSCache(CURL* curl, struct curl_slist*& dnsCache)
{
  // https://curl.se/libcurl/c/curl_slist_append.html
  // https://curl.se/libcurl/c/CURLOPT_RESOLVE.html
  // https://curl.se/libcurl/c/CURLOPT_IPRESOLVE.html

  if (!curl) {
    std::cerr << "CURL pointer is null" << std::endl;
    return CURLE_BAD_FUNCTION_ARGUMENT;
  }

  // V jednoduchom procese je prinos od DNS cache minimalny. Realny (a vyrazny) prinos moze byt pri spracovani velkeho
  // mnozstva requestov, resp. pri spracovani requestov v paralelnom rezime (testovat s CURLOPT_VERBOSE=1L).
  // https://curl.se/libcurl/c/curl_easy_getinfo.html   # CURLINFO_CONNECT_TIME vs CURLINFO_TOTAL_TIME
  // see section: Times, An overview of the time values available from curl_easy_getinfo

  CURLcode result;
  // dnsCache = curl_slist_append(dnsCache, "alice-ccdb.cern.ch:80:128.142.249.94");
  // dnsCache = curl_slist_append(dnsCache, "alice-ccdb.cern.ch:80:128.142.249.95");
  //    * Added alice-ccdb.cern.ch:80:128.142.249.94 to DNS cache
  //    * RESOLVE alice-ccdb.cern.ch:80 - old addresses discarded
  //    * Added alice-ccdb.cern.ch:80:128.142.249.95 to DNS cache
  dnsCache = curl_slist_append(dnsCache, "alice-ccdb.cern.ch:80:128.142.249.94,128.142.249.95");
  //    * Added alice-ccdb.cern.ch:80:128.142.249.94,128.142.249.95 to DNS cache
  dnsCache = curl_slist_append(dnsCache, "ali-qcdb-gpn.cern.ch:8083:128.141.20.157");
  //    * Added ali-qcdb-gpn.cern.ch:8083:128.141.20.157 to DNS cache
  result = curl_easy_setopt(curl, CURLOPT_RESOLVE, dnsCache);
  result = curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4); // only IPv4

  // result = curl_easy_setopt(curl, CURLOPT_RESOLVE, nullptr);   // reset DNS cache
  // result = curl_easy_setopt(curl, CURLOPT_IPRESOLVE, nullptr); // reset IP resolve option

  // nezabudnut na konci uvolnit alokovane zdroje: curl_slist_free_all(dnsCache);
  return result;
}
CURLcode curlEasyGetinfo(CURL* curl)
{
  // https://curl.se/libcurl/c/curl_easy_getinfo.html
  //
  // curl_easy_getinfo() vracia informacie o poslednom request-e, t.j. v pripade CURLOPT_FOLLOWLOCATION=1L (redirect on)
  // vracia informacie o druhom (poslednom) redirect request-e (prvy request vracia 3xx redirect response code).

  if (!curl) {
    std::cerr << "CURL pointer is null" << std::endl;
    return CURLE_BAD_FUNCTION_ARGUMENT;
  }

  coutColorStart(46);
  std::cout << "\n############ curl_easy_getinfo() (begin) ############\n";

  CURLcode result;
  long responseCode;
  result = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
  if (result == CURLE_OK) {
    std::cout << std::format("CURLINFO_RESPONSE_CODE: {}", responseCode);
    if ((responseCode / 100) == 3) {
      std::cout << " (3xx => redirection)";
    } else if ((responseCode / 100) == 2) {
      std::cout << " (2xx => success)";
    }
    std::cout << std::endl;
  } else {
    std::cerr << colorFormat(31, "curl_easy_getinfo(CURLINFO_RESPONSE_CODE) failed: {}\n", curl_easy_strerror(result));
  }
  double connectTime;
  result = curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &connectTime);
  if (result == CURLE_OK) {
    std::cout << std::format("CURLINFO_CONNECT_TIME: {:.5f} seconds\n", connectTime);
  } else {
    std::cerr << colorFormat(31, "curl_easy_getinfo(CURLINFO_CONNECT_TIME) failed: {}\n", curl_easy_strerror(result));
  }
  double totalTime;
  result = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &totalTime);
  if (result == CURLE_OK) {
    std::cout << std::format("CURLINFO_TOTAL_TIME: {:.3f} seconds\n", totalTime);
  } else {
    std::cerr << colorFormat(31, "curl_easy_getinfo(CURLINFO_TOTAL_TIME) failed: {}\n", curl_easy_strerror(result));
  }
  curl_off_t speedDownload;
  result = curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD_T, &speedDownload);
  if (result == CURLE_OK) {
    std::cout << std::format("CURLINFO_SPEED_DOWNLOAD_T: {} bytes/sec\n", speedDownload);
    // printf("Download Speed %" CURL_FORMAT_CURL_OFF_T " bytes/sec\n", speedDownload);
  } else {
    std::cerr << colorFormat(31, "curl_easy_getinfo(CURLINFO_SPEED_DOWNLOAD_T) failed: {}\n",
                             curl_easy_strerror(result));
  }
  char* effectiveUrl = nullptr;
  result = curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effectiveUrl);
  if (result == CURLE_OK) {
    std::cout << std::format("CURLINFO_EFFECTIVE_URL: {}\n", (effectiveUrl ? effectiveUrl : "none"));
  } else {
    std::cerr << colorFormat(31, "curl_easy_getinfo(CURLINFO_EFFECTIVE_URL) failed: {}\n", curl_easy_strerror(result));
  }
  char* redirectUrl = nullptr;
  result = curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &redirectUrl);
  if (result == CURLE_OK) {
    std::cout << std::format("CURLINFO_REDIRECT_URL: {}\n", (redirectUrl ? redirectUrl : "none"));
    // curlParseURL(redirectUrl ? redirectUrl : "none");
  } else {
    std::cerr << colorFormat(31, "curl_easy_getinfo(CURLINFO_REDIRECT_URL) failed: {}\n", curl_easy_strerror(result));
  }
  long redirects;
  result = curl_easy_getinfo(curl, CURLINFO_REDIRECT_COUNT, &redirects);
  if (result == CURLE_OK) {
    std::cout << std::format("CURLINFO_REDIRECT_COUNT: {}\n", redirects);
  } else {
    std::cerr << colorFormat(31, "curl_easy_getinfo(CURLINFO_REDIRECT_COUNT) failed: {}\n", curl_easy_strerror(result));
  }
  double redirectTime;
  result = curl_easy_getinfo(curl, CURLINFO_REDIRECT_TIME, &redirectTime);
  if (result == CURLE_OK) {
    std::cout << std::format("CURLINFO_REDIRECT_TIME: {:.3f} seconds\n", redirectTime);
  } else {
    std::cerr << colorFormat(31, "curl_easy_getinfo(CURLINFO_REDIRECT_TIME) failed: {}\n", curl_easy_strerror(result));
  }
  long fileTime;
  // fileTime (a rozne ine info) mozno ziskat pomocou curl_easy_header() ako 'Last-Modified' header
  result = curl_easy_getinfo(curl, CURLINFO_FILETIME, &fileTime); // must ask libcurl to collect this information
  if (result == CURLE_OK) {
    std::cout << std::format("CURLINFO_FILETIME: {}\n", fileTime);
  } else {
    std::cerr << colorFormat(31, "curl_easy_getinfo(CURLINFO_FILETIME) failed: {}\n", curl_easy_strerror(result));
  }

  std::cout << "\n############ curl_easy_getinfo()  (end)  ############\n";
  coutColorStop();
  return result;
}
CURLcode curlDB(CURL* curl, const char* fullUrl, bool redirect = false, bool verbose = true)
{
  // https://curl.se/libcurl/c/curl_easy_setopt.html
  // https://curl.se/libcurl/c/curl_easy_perform.html

  if (!curl) {
    std::cerr << "CURL pointer is null" << std::endl;
    return CURLE_BAD_FUNCTION_ARGUMENT;
  }

  CURLcode result;

  result = curl_easy_setopt(curl, CURLOPT_URL, fullUrl);
  if (result != CURLE_OK) {
    std::cerr << colorFormat(31, "curl_easy_setopt(CURLOPT_URL) failed: {}\n", curl_easy_strerror(result));
    return result;
  }
  // curl_easy_setopt(curl, CURLOPT_USERAGENT, "CCDBDownloader");
  result = curl_easy_setopt(curl, CURLOPT_TIMEOUT, 2L); // in seconds, 0L by default (no timeout)
  if (result != CURLE_OK) {
    std::cerr << colorFormat(31, "curl_easy_setopt(CURLOPT_TIMEOUT) failed: {}\n", curl_easy_strerror(result));
    return result;
  }
  result = curl_easy_setopt(curl, CURLOPT_FILETIME, 1L); // ask libcurl to collect 'Last-Modified' from header
  if (result != CURLE_OK) {
    std::cerr << colorFormat(31, "curl_easy_setopt(CURLOPT_FILETIME) failed: {}\n", curl_easy_strerror(result));
    return result;
  }
  if (verbose) {
    result = curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // 0L by default (disabled)
    if (result != CURLE_OK) {
      std::cerr << colorFormat(31, "curl_easy_setopt(CURLOPT_VERBOSE) failed: {}\n", curl_easy_strerror(result));
      return result;
    }
  } else {
    result = curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L); // 0L by default (disabled)
    if (result != CURLE_OK) {
      std::cerr << colorFormat(31, "curl_easy_setopt(CURLOPT_VERBOSE) failed: {}\n", curl_easy_strerror(result));
      return result;
    }
  }

  FILE* objectFile = nullptr;
  const char* objectFileName = "/tmp/object.root"; // in most cases in CCDB/QCDB it is the ROOT file
  std::cout << colorFormat(32, "URL: {}\n", fullUrl) << std::endl;
  // Nie prilis korektna logika pre 'ccdb-test' server, ktory vracia priamo 200 response code (no 303 redirect).
  // if redirect true =>  object (http:// ROOT file) stiahne na disk
  // if redirect false => object (http:// ROOT file) fwrite to stdout
  if (redirect) {
    result = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    std::cout << colorFormat(42, "CURLOPT_FOLLOWLOCATION=1L (redirect on)\n");
    if (result == CURLE_OK) {
      objectFile = fopen(objectFileName, "wb"); // writing binary data
      if (objectFile) {
        // CURLOPT_WRITEFUNCTION by default is fwrite() to write directly to file
        result = curl_easy_setopt(curl, CURLOPT_WRITEDATA, objectFile);
        std::cout << colorFormat(42, "CURLOPT_WRITEDATA={}\n", objectFileName);
        if (result != CURLE_OK) {
          std::cerr << colorFormat(31, "curl_easy_setopt(CURLOPT_WRITEDATA) failed: {}\n", curl_easy_strerror(result));
        }
      } else {
        std::cerr << colorFormat(31, "Failed to open file for writing: {} (redirect to stdout)\n", objectFileName);
      }
    }
  } else {
    result = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L); // 0L by default, but let it be explicitly set (reset)
    std::cout << colorFormat(-42, "CURLOPT_FOLLOWLOCATION=0L (redirect off)\n");
  }
  if (result != CURLE_OK) {
    std::cerr << colorFormat(31, "curl_easy_setopt(CURLOPT_FOLLOWLOCATION) failed: {}\n", curl_easy_strerror(result));
    return result;
  }

  if (verbose) {
    coutColorStart(47);
    std::cout << "\n############ CURL_VERBOSE (begin) ############\n";
  }
  result = curl_easy_perform(curl);
  if (verbose) {
    std::cout << "\n############ CURL_VERBOSE  (end)  ############\n";
    coutColorStop();
  }
  if (result != CURLE_OK) {
    std::cerr << colorFormat(31, "curl_easy_perform() failed: {}\n", curl_easy_strerror(result));
    if (result == CURLE_OPERATION_TIMEDOUT) {
      if (strstr(fullUrl, "ali-qcdb-gpn") != nullptr) {
        std::cerr << colorFormat(41, "QCDB only from CERN GPN (CERN internal network), try VPN or SSH tunnel\n");
      }
    }
  }

  if (objectFile) { // if redirect and if file was opened successfully
    fclose(objectFile);
    if (std::filesystem::exists(objectFileName)) {
      std::cout << colorFormat(42, "Downloaded object file: {}\n", objectFileName);
      std::cout << colorFormat(42, "             file size: {}\n", std::filesystem::file_size(objectFileName));
      auto ft = std::filesystem::last_write_time(objectFileName);
      // auto ct = std::chrono::clock_cast<std::chrono::system_clock>(ft);
      auto ct = std::chrono::file_clock::to_sys(ft);
      auto lt = std::chrono::zoned_time{ std::chrono::current_zone(), ct };
      std::cout << colorFormat(42, "  file last write time: {}\n", lt);
    } else {
      std::cerr << colorFormat(31, "Failed to download (redirect) object file: {}\n", objectFileName);
    }
    std::cout << std::endl;
  }

  curlEasyGetinfo(curl);
  return result;
}

// curl_easy_perform() (request) na CCDB alebo QCDB server vracia HTTP/1.1 303 redirect status (response) code.
//
// # CCDB server vracia HTTP/1.1 303 redirect na alien:// URL (URL from 'Location' header)
// > GET /GLO/Config/GRPLHCIF/1782529277570 HTTP/1.1
// Host: alice-ccdb.cern.ch
// < HTTP/1.1 303
// < Content-Location: alien:///alice/data/CCDB/GLO/Config/GRPLHCIF/09/03799/776b570c-71d4-11f1-b15c-0aa202c71b9a
// < Content-Location: http://alice-ccdb.cern.ch/download/776b570c-71d4-11f1-b15c-0aa202c71b9a
// < Location: alien:///alice/data/CCDB/GLO/Config/GRPLHCIF/09/03799/776b570c-71d4-11f1-b15c-0aa202c71b9a
//
// # QCDB server vracia HTTP/1.1 303 redirect na http:// URL (URL from 'Location' header)
// > GET /qc/CTP/MO/RawData/input02/1784877935784 HTTP/1.1
// Host: ali-qcdb-gpn.cern.ch:8083
// < HTTP/1.1 303
// < Content-Location: /download/a60702af-8731-11f1-b68f-0aa14520a1a2
// < Location: /download/a60702af-8731-11f1-b68f-0aa14520a1a2
//
// Ak je aktivny redirect (CURLOPT_FOLLOWLOCATION=1L), tak curl_easy_perform() vykona zaroven aj redirect request
// na URL ziskane z 'Location' header.
//
// # CCDB redirect na alien:// URL
// * The redirect target URL could not be parsed: Unsupported URL scheme
//
// To je ocakavane spravanie, kedze curl nepodporuje alien:// URL scheme (navrhovane pre AliEn Grid default protocol).
// curl_easy_perform() vracia error: Unsupported protocol. V tomto pripade je potrebne "manualne" ziskat http:// URL
// z 'Content-Location' header a potom vykonat dalsi request na tuto HTTP URL.
//
// # QCDB redirect na http:// URL
// * Issue another request to this URL: 'http://ali-qcdb-gpn.cern.ch:8083/download/a60702af-8731-11f1-b68f-0aa14520a1a2'
// * Reusing existing http: connection with host ali-qcdb-gpn.cern.ch
// > GET /download/a60702af-8731-11f1-b68f-0aa14520a1a2 HTTP/1.1
// Host: ali-qcdb-gpn.cern.ch:8083
// < HTTP/1.1 200
//
// Kedze QCDB ma redirect na standardne podporovany http:// URL, tak curl uspesne vykona redirect request (by default
// fwrite() on stdout, resp. zavisi od CURLOPT_WRITEFUNCTION) a QCDB server vracia HTTP/1.1 200 success response code.
// Inymi slovami, jedno volanie curl_easy_perform() (jeden request) vykona dva sub-requests:
//    1st) HTTP/1.1 303 redirect: curl ziska URL z 'Location' header     # OK pre CCDB a QCDB
//    2nd) HTTP/1.1 200 success: "object download" zo ziskanej URL       # Error (alien) pre CCDB, OK (http) pre QCDB
//
// ===================================================================================================================
//
// Dalej budeme predpokladat, ze nepozadujeme stiahnutie objektu (ROOT file) z CCDB/QCDB na disk, ale pozadujeme
// ziskanie jeho HTTP URL. Uzivatel potom sam rozhodne, ci chce objekt (ROOT file) neskor stiahnut alebo pouzit tuto
// HTTP URL priamo napr. v TFile::Open(). Pretoze nepozadujeme stiahnutie objektu, tak nebudeme pouzivat redirect
// option CURLOPT_FOLLOWLOCATION=1L (redirect on).
//
// CCDB server vracia 303 response code, redirect na alien:// URL, ziskany z 'Location' header
//                    + header 'Content-Location' obsahuje alien:// aj http:// URL
// QCDB server vracia 303 response code, redirect na http://  URL, ziskany z 'Location' header
//                    + header 'Content-Location' obsahuje http:// URL
// Samotny redirect request na URL z 'Location' header sa nebude vykonavat (CURLOPT_FOLLOWLOCATION=0L, redirect off).
//
// Na ziskanie HTTP URL z CCDB/QCDB servera mame v principe dve moznosti:
// 1st) pouzijeme CURLINFO_REDIRECT_URL
//      HTTP URL pre CCDB ziskame jednoduchou zamenou alien:// na http:// + modifikacia cesty (path)
//      HTTP URL pre QCDB je uz priamo http://
// 2nd) "manualne" ziskat HTTP URL z 'Content-Location' header, ODPORUCANA mnoznost
//
// Pri 1st) moznosti su neskor mozne problemy, napr. zmena URL schemy v buducnosti a pod. 2nd) moznost je menej
// chybova a hlavne univerzalna pre CCDB aj QCDB. Okrem toho, z header sa budu "manualne" ziskavat aj rozne dalsie
// informacie, napr. 'Valid-From'.
//
// "Manualne" ziskanie HTTP URL z 'Content-Location' header je mozne dvoma sposobmi:
// 1st) starsi sposob pomocou CURLOPT_HEADERFUNCTION (header_callback funkcie) a vlastneho parsovania header bufferu
// 2nd) novsi a preferovany sposob pomocou curl_easy_header(), added in curl 7.83.0 (released in 2022-04)

CURLcode curlEasyHeader(CURL* curl)
{
  // https://curl.se/libcurl/c/curl_easy_header.html
  // https://curl.se/libcurl/c/curl_easy_nextheader.html
  //
  // https://developer.mozilla.org/en-US/docs/Web/HTTP/Reference/Headers
  // https://www.iana.org/assignments/http-fields

  // curl_easy_header() je vhodne a moderne (since curl 7.83.0, released in 2022-04) riesenie na ziskavanie jednotlivych
  // HTTP headers z HTTP response, napr. 'Content-Location', 'Valid-From', 'Created', 'ETag' a pod. Na rozdiel od
  // CURLOPT_HEADERFUNCTION (header_callback() function) nemusime sami parsovat HTTP header lines ani riesit ich
  // rozdelenie medzi jednotlive callbacky. curl_easy_header() a curl_easy_nextheader() pouziva HTTP header parser
  // priamo v libcurl. Vyhladavanie je case-insensitive ('Valid-From' vs 'valid-from') a pri duplicitnych hlavickach
  // (napr. 'Content-Location' pre CCDB) rozlisujeme podla indexu: index [0] => alien://, index [1] => http://.

  if (!curl) {
    std::cerr << "CURL pointer is null" << std::endl;
    return CURLE_BAD_FUNCTION_ARGUMENT;
  }

  coutColorStart(43);
  std::cout << "\n############ curlEasyHeader() (begin) ############\n";

  std::cout << "\n=== curl_easy_nextheader() ===\n";
  struct curl_header* hLine = nullptr;
  while ((hLine = curl_easy_nextheader(curl, CURLH_HEADER, -1, hLine))) {
    //                                           int request | (-1 for last request, 0 for first request and so on)
    std::cout << std::format("{}[{}] {}: {}\n", hLine->amount, hLine->index, hLine->name, hLine->value);
    // std::cout << std::format("name: {}, value: {}, amount: {}, index: {}, origin: {}, anchor: {}\n",
    //                          hLine->name, hLine->value, hLine->amount, hLine->index, hLine->origin, hLine->anchor);
    //
    // Moznost vyhladavania (parsovania) konkretneho header podla "name" a ziskat jeho "value" pre dalsie spracovanie,
    // ale na to je vhodnejsie pouzit "predurceny" curl_easy_header() function.
  }

  std::cout << "\n=== curl_easy_header() ===\n";
  CURLHcode hResult = curl_easy_header(curl, "ETag", 0, CURLH_HEADER, -1, &hLine);
  //                              const char *name | (case-insensitive, e.g. "ETag" or "etag")
  //                                 size_t nameindex | (nameindex [0], [1], ..., [amount-1] of this instance)
  //                                                        int request | (-1 for last request, 0 for first request)
  //
  // size_t nameindex = 0 oznacuje prvy vyskyt (instance) hlavicky. 'ETag' line sa v CCDB/QCDB HTTP header response
  // vyskutuje prave 1x, preto nameindex moze mat prave len jednu hodnotu = 0, ziaden iny vyskyt tejto hlavicky
  // neexistuje. Kedze nameindex je typu size_t, nemozno pouzit specialnu hodnotu -1 pre posledny vyskyt. Celkovy pocet
  // vyskytov (instances) nam udava hodnota hLine->amount.
  //
  // int request = -1 oznacuje posledny request. V nasom pripade mame len jeden request, takze -1 je ekvivalentne 0.
  // V pripade viacerych requestov (napr. pri CURLOPT_FOLLOWLOCATION=1L, redirect on) by sme mohli ziskat hlavicku
  // z konkretneho requestu zadanim jeho request indexu (CURLINFO_REDIRECT_COUNT udava pocet redirect requestov).

  // ! POZOR ! CCDB/QCDB header pre request 0 a request 1 (redirect on) je, resp. moze byt mierne odlisny. Redirect
  // request vrati nie len iny HTTP response code ale aj mierne iny HTTP header.
  //
  // QCDB header request 1 (after success http:// redirect) uz neobsahuje 'Content-Location' alebo 'Location' header,
  // ale obsahuje navyse napr. 'Content-MD5' alebo'path' header.
  // CCDB header request 1 (after unsuccessful alien:// redirect) neexistuje, CURLHcode: CURLHE_MISSING (2).

  if (hResult == CURLHE_OK) {
    std::cout << std::format("{}[{}] {}: {}\n", hLine->amount, hLine->index, hLine->name, hLine->value);
  } else {
    // curl_easy_strerror() only for CURLcode, not for CURLHcode
    // https://curl.se/libcurl/c/libcurl-errors.html#CURLHcode
    std::cerr << "curl_easy_header(\"ETag\") failed with CURLHcode: " << hResult << std::endl;
    // do not try colorFormat(31, ...) compilation error
  }

  hResult = curl_easy_header(curl, "cOntent-diSposition", 0, CURLH_HEADER, -1, &hLine);
  if (hResult == CURLHE_OK) {
    std::cout << std::format("{}[{}] {}: {}\n", hLine->amount, hLine->index, hLine->name, hLine->value);
  } else {
    std::cerr << "curl_easy_header(\"Content-Disposition\") failed with CURLHcode: " << hResult << std::endl;
  }
  hResult = curl_easy_header(curl, "Location", 0, CURLH_HEADER, -1, &hLine);
  if (hResult == CURLHE_OK) {
    std::cout << std::format("{}[{}] {}: {}\n", hLine->amount, hLine->index, hLine->name, hLine->value);
  } else {
    std::cerr << "curl_easy_header(\"Location\") failed with CURLHcode: " << hResult << std::endl;
  }

  hResult = curl_easy_header(curl, "Content-Location", 0, CURLH_HEADER, -1, &hLine);
  if (hResult == CURLHE_OK) {
    std::cout << std::format("{}[{}] {}: {}\n", hLine->amount, hLine->index, hLine->name, hLine->value);
  } else {
    std::cerr << "curl_easy_header(\"Content-Location[0]\") failed with CURLHcode: " << hResult << std::endl;
  }
  hResult = curl_easy_header(curl, "Content-Location", 1, CURLH_HEADER, -1, &hLine);
  if (hResult == CURLHE_OK) {
    std::cout << std::format("{}[{}] {}: {}\n", hLine->amount, hLine->index, hLine->name, hLine->value);
  } else {
    std::cerr << "curl_easy_header(\"Content-Location[1]\") failed with CURLHcode: " << hResult << std::endl;
  }

  std::cout << "\n############ curlEasyHeader()  (end)  ############\n";
  coutColorStop();
  return CURLE_OK;
}

int main()
{
  // https://curl.se/libcurl/c/libcurl.html
  // https://curl.se/libcurl/c/example.html
  // https://github.com/curl/curl/tree/master/docs/examples
  // https://daniel.haxx.se/blog/2021/09/27/common-mistakes-when-using-libcurl/
  // https://everything.curl.dev/libcurl/cplusplus.html
  //
  // Alice O2: curl 7.70.0-DEV, RHEL9: curl 7.76.1, Fedora 44: curl 8.18.0   # (2026-08)

  // https://curl.se/libcurl/c/curl_global_init.html
  // https://curl.se/libcurl/c/curl_easy_init.html
  //
  // curl_easy_init() automatically calls curl_global_init() if it has not been called yet. You are strongly advised
  // to not allow this automatic behavior, by calling curl_global_init() yourself properly.
  //
  // curl_global_init() volat iba jeden krat v ramci celeho programu a na jeho konci volat curl_global_cleanup().
  //
  // curl_easy_init() vytvara samostatny easy CURL* handle. Pre sekvencne requesty zvycajne staci jeden CURL* handle,
  // ktory mozeme opakovane pouzivat. Viac CURL* handles ma zmysel najma pri paralelnom stahovani viacerych objektov
  // alebo pri viacerych nezavislych requestoch, ktore maju bezat sucasne.
  //
  // Pri pouziti viacerych threads nesmie byt ten isty CURL* handle pouzivany sucasne vo viacerych threads. Kazdy thread
  // by mal mat vlastny CURL* handle vytvoreny pomocou curl_easy_init() a na konci thread-u volat curl_easy_cleanup().

  CURLcode result = curl_global_init(CURL_GLOBAL_DEFAULT); // actually equals to CURL_GLOBAL_ALL mask
  if (result != CURLE_OK) {
    std::cerr << colorFormat(31, "curl_global_init() failed: {}\n", curl_easy_strerror(result));
    // do not call curl_global_cleanup() if curl_global_init() failed, because it was not initialized properly
    return result;
  }

  // curlParseURL(); // example with full URL
  // std::cout << "\n\n";
  // curlParseURL("http://wrong address");
  // std::cout << "\n\n";
  // curlParseURL("http://alice-ccdb.cern.ch/GLO/Config/GRPLHCIF/1782529277570");
  // std::cout << "\n\n";

  CURL* curl = curl_easy_init();
  if (!curl) {
    std::cerr << colorFormat(31, "curl_easy_init() failed\n");
    curl_global_cleanup();
    return CURLE_FAILED_INIT;
  }

  // struct curl_slist* dnsList = nullptr;
  // curlDNSCache(curl, dnsList);

  const char* ccdbUrl = "http://alice-ccdb.cern.ch/GLO/Config/GRPLHCIF/1782529277570";
  // const char* ccdbUrl = "http://ccdb-test.cern.ch:8080/GLO/Config/GRPLHCIF/1697170039223";
  // ! POZOR ! ccdb-test server vracia 200 response code (no 303 redirect) a object (http:// ROOT file, not alien://)
  const char* qcdbUrl = "http://ali-qcdb-gpn.cern.ch:8083/qc/CTP/MO/RawData/input02/1784877935784";
  // const char* qcdbUrl = "http://localhost:8083/qc/CTP/MO/RawData/input02/1784877935784"; // ssh tunnel

  std::cout << "\n##########################################################\n";
  curlDB(curl, ccdbUrl, false, true);
  curlEasyHeader(curl);
  std::cout << "\n##########################################################\n";
  curlDB(curl, ccdbUrl, true, true);
  curlEasyHeader(curl);
  std::cout << "\n##########################################################\n";
  curlDB(curl, qcdbUrl, false, true);
  curlEasyHeader(curl);
  std::cout << "\n##########################################################\n";
  curlDB(curl, qcdbUrl, true, true);
  curlEasyHeader(curl);

  // if (dnsList) {
  //   curl_slist_free_all(dnsList);
  //   dnsList = nullptr;
  // }
  curl_easy_cleanup(curl);
  curl_global_cleanup();
  return 0;
}

// https://curl.se/libcurl/c/example.html   # see for examples of write/header_callback() functions
//
// https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
// https://curl.se/libcurl/c/CURLOPT_WRITEDATA.html
//
// size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
// CURLcode curl_easy_setopt(CURL *handle, CURLOPT_WRITEFUNCTION, write_callback);
// CURLcode curl_easy_setopt(CURL *handle, CURLOPT_WRITEDATA, void *pointer);
//
// https://curl.se/libcurl/c/CURLOPT_HEADERFUNCTION.html
// https://curl.se/libcurl/c/CURLOPT_HEADERDATA.html
//
// size_t header_callback(char* buffer, size_t size, size_t nitems, void* userdata);
// CURLcode curl_easy_setopt(CURL* handle, CURLOPT_HEADERFUNCTION, header_callback);
// CURLcode curl_easy_setopt(CURL *handle, CURLOPT_HEADERDATA, void *pointer);

// ToDo: URL with browse/ or latest/ (text/plain, text/html, text/xml, application/json)
// --header "Accept: application/json" vs URL query "?Accept=application/json"

/*

$ curl http://alice-ccdb.cern.ch/GLO/Config/GRPLHCIF/1782529277570 --show-headers
HTTP/1.1 303
Access-Control-Allow-Origin: *
Access-Control-Allow-Headers: range
Access-Control-Expose-Headers: content-range,content-length,accept-ranges
Access-Control-Allow-Methods: HEAD,GET
Cache-Control: no-cache
Content-Location: alien:///alice/data/CCDB/GLO/Config/GRPLHCIF/09/03799/776b570c-71d4-11f1-b15c-0aa202c71b9a
Content-Location: http://alice-ccdb.cern.ch/download/776b570c-71d4-11f1-b15c-0aa202c71b9a
Date: Mon, 17 Aug 2026 12:16:12 GMT
Valid-Until: 1785121277570
Valid-From: 1782529277570
Created: 1782529277572
ETag: "776b570c-71d4-11f1-b15c-0aa202c71b9a"
Last-Modified: Sat, 27 Jun 2026 03:01:17 GMT
Content-Disposition: filename="o2-parameters-GRPLHCIFData_1782529277570.root"
ObjectType: o2::parameters::GRPLHCIFData
adjustableEOV: true
created_by: dpl
partName: send
Location: alien:///alice/data/CCDB/GLO/Config/GRPLHCIF/09/03799/776b570c-71d4-11f1-b15c-0aa202c71b9a
Content-Type: application/root
Content-Length: 0

$ curl http://alice-ccdb.cern.ch/download/776b570c-71d4-11f1-b15c-0aa202c71b9a --show-headers
HTTP/1.1 200
Access-Control-Allow-Origin: *
Access-Control-Allow-Headers: range
Access-Control-Expose-Headers: content-range,content-length,accept-ranges
Access-Control-Allow-Methods: HEAD,GET
Date: Mon, 17 Aug 2026 12:16:35 GMT
Valid-Until: 1785121277570
Valid-From: 1782529277570
Created: 1782529277572
ETag: "776b570c-71d4-11f1-b15c-0aa202c71b9a"
Last-Modified: Sat, 27 Jun 2026 03:01:17 GMT
Content-Disposition: inline;filename="o2-parameters-GRPLHCIFData_1782529277570.root"
ObjectType: o2::parameters::GRPLHCIFData
adjustableEOV: true
created_by: dpl
partName: send
path: GLO/Config/GRPLHCIF
Accept-Ranges: bytes
Content-MD5: ff06822fbef697260098d7bfa5e21f2e
Content-Type: application/root
Content-Length: 2048

Warning: Binary output can mess up your terminal. Use "--output -" to tell curl to output it to your terminal anyway,
or consider "--output <FILE>" to save to a file.

*/
