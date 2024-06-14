// 2024-06-14

// $ gcc -pedantic -Wall -Wextra -o data.type.cast data.type.cast.c && ./data.type.cast
//////////////////

#include <stdio.h>

// https://en.wikipedia.org/wiki/Type_conversion

int main() {
  // ToDo: as printf + is safe conversion small -> big ?!
  // char (1 byte) -> short (2 bytes) -> int (4 bytes) -> long (8 bytes)
  // on x86_64 platform long long = long (8 bytes)
  // float (4 byte) -> double (8 byte) -> long double (16 byte) // IEEE 754

  // https://stackoverflow.com/q/56690872
  printf("# float type cannot represent 16777217 exactly as IEEE 754 single precision"
         "\n# and to same for double 9007199254740993 as IEEE 754 double precision\n");
  printf("(float)16777216.0  =  %f\n", (float)16777216.0);
  printf("(float)16777217.0 !=! %f\n", (float)16777217.0);
  printf("(float)16777218.0  =  %f\n", (float)16777218.0);
  if ((float)16777216.0 == (int)16777217) printf("(float)16777216.0 == (int)16777217\n");
  if ((float)16777217.0 == (int)16777217) printf("(float)16777217.0 == (int)16777217\n");
  printf("(double)9007199254740993.0 !=! %f\n", (double)9007199254740993);

  printf("\n# long -> int type conversion\n");
  printf("# long to int causes dropping of excess higher order bits\n");
  // long_max = +9.223.372.036.854.775.807 (19 digit)
  // int_max  = +2.147.483.647 (10 digit)
  long l1 = 1234567890123456789;
  printf("long value %ld as int value %d\n", l1, (int)l1);
  printf("long value %ld as short value %hd\n", l1, (short)l1);
  int i1 = 1234567890;
  printf("int value %d as short value %hd\n", i1, (short)i1);

  printf("\n# double -> float type conversion\n");
  printf("# double to float causes rounding of digit\n");
  double d1 = 123456789.9876543;
  printf("1) double value %.7f as float value %.7f\n", d1, (float)d1);
  printf("1) double value %g as float value %g\n", d1, (float)d1);
  d1 = 100.123456789;
  printf("2) double value %.9f as float value %.9f\n", d1, (float)d1);
  printf("2) double value %g as float value %g\n", d1, (float)d1);
  d1 = 1234567890123456789.000;
  printf("3) double value %.3f as float value %.3f\n", d1, (float)d1);
  printf("3) double value %g as float value %g\n", d1, (float)d1);

  printf("\n# float -> int type conversion\n");
  printf("# float to int causes truncation, i.e. removal of the fractional part\n");
  float f1 = 3.14; float f2 = 9.99;
  printf("f1 = %f, f2 = %f\n", f1, f2);
  printf("(int)f1 = %d, (int)f2 = %d, (int)(f1+f2) = %d, (int)f1+(int)f2 = %d\n",
         (int)f1, (int)f2, (int)(f1+f2), (int)f1 + (int)f2);

  printf("\n# int -> float type conversion\n");
  printf("# int to float causes rounding and adding zero decimal point\n");
  i1 = 1234567890;
  printf("int value %d\tas float value %f\n", i1, (float)i1);
  i1 = 123456789;
  printf("int value %d\tas float value %f\n", i1, (float)i1);
  i1 = 12345678;
  printf("int value %d\tas float value %f\n", i1, (float)i1);
}
