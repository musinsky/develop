// 2024-07-04

// $ gcc -pedantic -Wall -Wextra -o data.type.conversion data.type.conversion.c
//       -Wconversion # (optional) implicit conversions that may alter a value
// $ ./data.type.conversion

#include <stdio.h>
#include <assert.h>

// https://en.wikipedia.org/wiki/Type_conversion
// https://learn.microsoft.com/en-us/cpp/cpp/type-conversions-and-type-safety-modern-cpp
// https://learn.microsoft.com/en-us/cpp/cpp/standard-conversions

int main() {
  printf("============================================\n");
  printf("safe conversions and the value is unchanged:\n");
  printf("char (%lu byte) -> short (%lu bytes) -> int (%lu bytes) -> "
         "long (%lu bytes) => long long (%lu bytes)\n",
         sizeof(char), sizeof(short), sizeof(int), sizeof(long), sizeof(long long));
  printf("float (%lu bytes) -> double (%lu byte) -> long double (%lu bytes)\n",
         sizeof(float), sizeof(double), sizeof(long double));
  printf("============================================\n");

  printf("\n# long -> int (short, char) type conversion\n");
  printf("# long to int causes dropping of excess higher order bits\n");
  // long_max = +9.223.372.036.854.775.807 (19 digit)
  // int_max  =             +2.147.483.647 (10 digit)
  long l1 = 1234567890123456789;
  printf("long value %ld as int value %d\n", l1, (int)l1);
  printf("long value %ld as short value %hd\n", l1, (short)l1);
  printf("long value %ld as char value %d\n", l1, (char)l1);
  int i1 = 1234567890;
  printf("int value %d as short value %hd\n", i1, (short)i1);
  printf("int value %d as char value %d\n", i1, (char)i1);
  // i1 = l1;                       // gcc without warning
  // i1 = (int)1234567890123456789; // gcc without warning
  // i1 = 1234567890123456789;
  // // gcc warning: overflow in conversion from 'long int' to 'int' changes value
  // // from '1234567890123456789' to '2112454933'

  printf("\n# double -> float type conversion\n");
  printf("# double to float causes rounding of digit\n");
  double d1 = 123456789.1234567;
  printf("double value %.7f as float value %.7f\n", d1, (float)d1);
  printf("    or short %g as %g\n", d1, (float)d1);
  d1 = 100.12345678901234;
  printf("double value %.14f as float value %.14f\n", d1, (float)d1);
  printf("    or short %g as %g\n", d1, (float)d1);
  d1 = 1234567890123456.0;
  printf("double value %.2f as float value %.2f\n", d1, (float)d1);
  printf("    or short %g as %g\n", d1, (float)d1);
  d1 = 1.123e+123; // float_max = +3,40282e+38
  // printf("d) double value %f as float value %f\n", d1, (float)d1);
  printf("double value %g as float value %g\n", d1, (float)d1);

  // https://en.wikipedia.org/wiki/Single-precision_floating-point_format
  // https://en.wikipedia.org/wiki/Double-precision_floating-point_format
  // https://stackoverflow.com/q/56690872
  // https://stackoverflow.com/q/52267201
  // https://www.h-schmidt.net/FloatConverter/IEEE754.html
  printf("\n=====================================================================\n");
  printf("IEEE 754 single precision (4-byte, 32-bit word) floating point format\n");
  printf("binary32 = 1 bit: sign, 8 bits: exponent, 23 bits: fraction or mantissa\n");
  printf("precision limitations on integer: 2^23 (fraction) * 2 (sign/unsign) = 2^24 = "
         "16.777.216\n");
  printf("=> integers between 0 and 2^24 = 16.777.216 can be exactly represented\n");
  printf("=> integers between 2^24 and 2^25 round to a multiple of 2\n");
  printf("=> integers between 2^25 and 2^26 round to a multiple of 4\n");
  printf("=> integers between 2^n and 2^(n+1) round to a multiple of 2^(n-23)\n");
  printf("\nIEEE 754 double precision (8-byte, 64-bit word) floating point format\n");
  printf("binary64 = 1 bit: sign, 11 bits: exponent, 52 bits: fraction or mantissa\n");
  printf("precision limitations on integer: 2^52 (fraction) * 2 (sign/unsign) = 2^53 = "
         "9.007.199.254.740.992\n");
  printf("=> integers between 0 and 2^53 = 9.007.199.254.740.992 can be exactly represented\n");
  printf("=> integers between 2^n and 2^(n+1) round to a multiple of 2^(n-52)\n");
  printf("=====================================================================\n");

  assert(__STDC_IEC_559__ == 1); // IEEE 754 standard
  printf("\n# IEEE 754 single (double) precision limitations on integer values\n");
  printf("(float)16777215.0  =  %.1f", (float)16777215.0);
  printf(" | (float)1e07 = %17.1f\n", (float)1e07);
  printf("(float)16777216.0  =  %.1f", (float)16777216.0);
  printf(" | (float)1e08 = %17.1f\n", (float)1e08);
  printf("(float)16777217.0 !=! %.1f", (float)16777217.0);
  printf(" | (float)1e09 = %17.1f\n", (float)1e09);
  printf("(float)16777218.0  =  %.1f", (float)16777218.0);
  printf(" | (float)1e10 = %17.1f\n", (float)1e10);
  printf("(float)16777219.0 !=! %.1f", (float)16777219.0);
  printf(" | (float)1e11 = %17.1f\n", (float)1e11);
  printf("(float)16777220.0  =  %.1f", (float)16777220.0);
  printf(" | (float)1e12 = %17.1f\n", (float)1e12);
  printf("(float)16777221.0 !=! %.1f", (float)16777221.0);
  printf(" | (float)1e13 = %17.1f\n", (float)1e13);
  printf("(float)16777222.0  =  %.1f", (float)16777222.0);
  printf(" | (float)1e14 = %17.1f\n", (float)1e14);
  if ((float)16777216.0 == (int)16777217) printf("true: (float)16777216.0 == (int)16777217\n");
  if ((float)16777217.0 == (int)16777217) printf("true: (float)16777217.0 == (int)16777217\n");
  printf("(double)9007199254740993.0 !=! %.1f\n", (double)9007199254740993);

  printf("\n# float -> int type conversion (types with same size)\n");
  printf("# float to int causes truncation, i.e. removal of the fractional part\n");
  float f1 = 3.14; float f2 = 9.99;
  printf("f1 = %f, f2 = %f\n", f1, f2);
  printf("(int)f1 = %d, (int)f2 = %d, (int)(f1+f2) = %d, (int)f1+(int)f2 = %d\n",
         (int)f1, (int)f2, (int)(f1+f2), (int)f1 + (int)f2);
  f1 = 123456789.0;
  printf("value 123456789.0 stored in float %.1f and as int value %d\n", f1, (int)f1);
  f1 = 100000000.0;
  printf("value 100000000.0 stored in float %.1f and as int value %d\n", f1, (int)f1);
  f1 = 16777217.0;
  printf("value 16777217.0 stored in float %.1f and as int value %d\n", f1, (int)f1);
  f1 = 16777216.0;
  printf("value 16777216.0 stored in float %.1f and as int value %d\n", f1, (int)f1);

  printf("\n# int -> float type conversion (types with same size)\n");
  printf("# int to float causes rounding and adding zero decimal point\n");
  i1 = 123456789;
  printf("int value %d as float value %.1f\n", i1, (float)i1);
  i1 = 100000000;
  printf("int value %d as float value %.1f\n", i1, (float)i1);
  i1 = 16777217;
  printf("int value %d as float value %.1f\n", i1, (float)i1);
  i1 = 16777216;
  printf("int value %d as float value %.1f\n", i1, (float)i1);

  // https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#index-Wfloat-conversion
  // Warn for implicit conversions that reduce the precision of a real value.
  // This includes conversions from real to integer, and from higher precision
  // real to lower precision real values (also enabled by -Wconversion option).
#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-Wconversion"
  f1 = 0.123; // double to float changes value from 1.23e-1 to 1.23000003e-1f
  f1 = 1.230; // double to float changes value from 1.23e+0 to 1.23000002e+0f
  f1 = 3.14;  // double to float changes value from 3.1400000000000001e+0 to 3.1400001e+0f
  f1 = 3.14f;       // gcc without warning
  f1 = (float)3.14; // gcc without warning

  f1 = 16777216.0; // gcc without warning
  f1 = 16777217.0; // double to float changes value from 1.6777217e+7 to 1.6777216e+7f
  f1 = 16777218.0; // gcc without warning
  f1 = 16777219.0; // double to float changes value from 1.6777219e+7 to 1.677722e+7f

  f1 = 16777216;        // gcc without warning
  f1 = 16777217;        // int to float changes value from 16777217 to 1.6777216e+7f
  f1 = (float)16777217; // gcc without warning (f1 = 16777216.0)
  f1 = 123456789;       // int to float changes value from 123456789 to 1.23456792e+8f
  f1 = 123456789.0;     // double to float changes value from 1.23456789e+8 to 1.23456792e+8f

  i1 = 3.14;        // double to int changes value from 3.1400000000000001e+0 to 3
  i1 = (float)3.14; // float to int changes value from 3.1400001e+0f to 3
  i1 = (int)3.14;   // gcc without warning

  i1 = 16777217.0;        // gcc without warning (i1 = 16777217)
  i1 = (float)16777217.0; // gcc without warning (i1 = 16777216)
#pragma GCC diagnostic pop

  printf("\ninfo: GNU gcc %s\n",  __VERSION__);
  printf("info: GNU C Library (glibc) %d.%d\n", __GLIBC__, __GLIBC_MINOR__);
}
