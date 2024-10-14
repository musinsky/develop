// 2024-10-14

// https://root.cern.ch/doc/master/classTString.html
//
// void TString::Form(const char *va_(fmt), ...)
// Formats a string using a printf style format descriptor. Existing string
// contents will be overwritten.
//
// TString TString::Format(const char *va_(fmt), ...)
// Static method which formats a string using a printf style format descriptor
// and return a TString. Similar to TString::Form() but it is not needed to
// first create a TString.
//
// ### global functions
// char *Form(const char *va_(fmt), ...)    // format in circular buffer
// Formats a string in a circular formatting buffer. Removes the need to create
// and delete short lived strings. Don't pass Form() pointers from user code
// down to ROOT functions as the circular buffer may be overwritten downstream.
// Use Form() results immediately or use TString::Format() instead.
//
// void Printf(const char *va_(fmt), ...)   // format and print
// Formats a string in a circular formatting buffer and prints the string.
// Appends a newline.

#include <TString.h>

const char *fun1() { // WRONG (compilation warning)
  return TString::Format("fun1 %d", 123).Data();
}
const char *fun2() { // WRONG (compilation warning)
  TString tmp;
  tmp.Form("fun2 %d", 123);
  printf("%s\n", tmp.Data());
  return tmp.Data();
}
const char *fun3() { // OK (circular buffer, use result immediately)
  return Form("fun3 %d", 123); // global function
}

void string_format_202410()
{
  TString str;
  str.Form("exam %d", 1);
  printf("%s\n", str.Data());

  printf("%s\n", TString::Format("exam %d", 2).Data());

  str.Form("exam %d", 3); // reset
  str.Form("next %s", str.Data()); // WRONG ('str' inside 'str')
  printf("%s\n", str.Data());
  str.Form("exam %d", 3); // reset
  str = TString::Format("next %s", str.Data()); // OK
  printf("%s\n", str.Data());

  printf("function1: %s\n", fun1());
  printf("function2: %s\n", fun2());
  printf("function3: %s\n", fun3());
}
