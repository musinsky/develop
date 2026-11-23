// 2024-06-14

// $ gcc -pedantic -Wall -Wextra -o data.types data.types.c && ./data.types
//       -std=c23 # (optional) experimental mode for C23 standard

#include <stdio.h>
#include <locale.h>
#include <limits.h>
#include <inttypes.h>
#include <float.h>

// https://en.wikipedia.org/wiki/C_data_types
// https://en.wikipedia.org/wiki/Printf
// https://en.cppreference.com/w/c/types/integer
// https://en.cppreference.com/w/c/types/limits
// https://en.cppreference.com/w/c/language/arithmetic_types
// https://en.cppreference.com/w/cpp/language/types
// https://en.cppreference.com/w/cpp/types/integer
// https://en.cppreference.com/w/cpp/types/floating-point
// https://en.cppreference.com/w/cpp/io/c/fprintf

int main() {
  // en_US.UTF8  => 1,000.23   de_DE.UTF8 => 1.000,23
  // mfe_MU.UTF8 => 1 000.23   sk_SK.UTF8 => 1 000,23
  setlocale(LC_NUMERIC, "de_DE.UTF8");
  struct lconv *lc = localeconv(); // read only locale settings
  printf("# locale '%s' => '%s' as thousands_sep and '%s' as decimal_point\n\n",
         setlocale(LC_NUMERIC, NULL), lc->thousands_sep, lc->decimal_point);

  // gcc -pedantic => warning: ISO C does not support the ''' printf flag [-Wformat=]
  // man 3 printf => Flag characters, thousands' grouping characters: Note that many
  // versions of gcc cannot parse ''' (apostrophe) option and will issue a warning.
#pragma GCC diagnostic ignored "-Wformat"
  printf("## C data types ##\n");
  printf("char          // smallest addressable unit (integer type), "
         "%lu byte = %d bits, [%'+d, %'+d]\n",
         sizeof(char), CHAR_BIT, CHAR_MIN, CHAR_MAX);
  printf("signed char   // as char but guaranteed to be   signed, %lu byte, [%'+d, %'+d]\n",
         sizeof(signed char), SCHAR_MIN, SCHAR_MAX);
  printf("unsigned char // as char but guaranteed to be unsigned, %lu byte, [%'d, %'d]\n",
         sizeof(unsigned char), 0, UCHAR_MAX);
  printf("\n");
  printf("short          // short   signed integer type, %lu bytes, [%'+hd, %'+hd]\n",
         sizeof(short), SHRT_MIN, SHRT_MAX);
  printf("unsigned short // short unsigned integer type, %lu bytes, [%'d, %'hu]\n",
         sizeof(unsigned short), 0, USHRT_MAX);
  printf("int            // basic   signed integer type, %lu bytes, [%'+d, %'+d]\n",
         sizeof(int), INT_MIN, INT_MAX);
  printf("unsigned int   // basic unsigned integer type, %lu bytes, [%'d, %'u]\n",
         sizeof(unsigned int), 0, UINT_MAX);
  printf("long           // long    signed integer type, %lu bytes, [%'+ld, %'+ld]\n",
         sizeof(long), LONG_MIN, LONG_MAX);
  printf("unsigned long  // long  unsigned integer type, %lu bytes, [%'d, %'lu]\n",
         sizeof(unsigned long), 0, ULONG_MAX);
  printf("long long          // long long   signed integer type, %lu bytes, [%'+lld, %'+lld]\n",
         sizeof(long long), LLONG_MIN, LLONG_MAX);
  printf("unsigned long long // long long unsigned integer type, %lu bytes, [%'d, %'llu]\n",
         sizeof(unsigned long long), 0, ULLONG_MAX);
  printf("                   // on x86_64 platform: (unsigned) long long = (unsigned) long\n");
  printf("\n");
  printf("float       // single-precision   floating-point type, %lu bytes, [%'+g, %'+g]\n",
         sizeof(float), FLT_MIN, FLT_MAX);
  printf("double      // double-precision   floating-point type, %lu bytes, [%'+g, %'+g]\n",
         sizeof(double), DBL_MIN, DBL_MAX);
  printf("long double // extended precision floating-point number format, "
         "%lu bytes, [%'+Lg, %'+Lg]\n", sizeof(long double), LDBL_MIN, LDBL_MAX);
  printf("\n");
  printf("## Fixed width integer types ##\n");
  printf("int8_t   //   signed integer type, exact  8-bit width, %lu bytes, "
         "[%'+" PRId8 ", %'+" PRId8 "]\n", sizeof(int8_t), INT8_MIN, INT8_MAX);
  printf("uint8_t  // unsigned integer type, exact  8-bit width, %lu bytes, "
         "[%'d, %'" PRIu8 "]\n", sizeof(int8_t), 0, UINT8_MAX);
  printf("int16_t  //   signed integer type, exact 16-bit width, %lu bytes, "
         "[%'+" PRId16 ", %'+" PRId16 "]\n", sizeof(int16_t), INT16_MIN, INT16_MAX);
  printf("uint16_t // unsigned integer type, exact 16-bit width, %lu bytes, "
         "[%'d, %'" PRIu16 "]\n", sizeof(int16_t), 0, UINT16_MAX);
  printf("int32_t  //   signed integer type, exact 32-bit width, %lu bytes, "
         "[%'+" PRId32 ", %'+" PRId32 "]\n", sizeof(int32_t), INT32_MIN, INT32_MAX);
  printf("uint32_t // unsigned integer type, exact 32-bit width, %lu bytes, "
         "[%'d, %'" PRIu32 "]\n", sizeof(int32_t), 0, UINT32_MAX);
  printf("int64_t  //   signed integer type, exact 64-bit width, %lu bytes, "
         "[%'+" PRId64 ", %'+" PRId64 "]\n", sizeof(int64_t), INT64_MIN, INT64_MAX);
  printf("uint64_t // unsigned integer type, exact 64-bit width, %lu bytes, "
         "[%'d, %'" PRIu64 "]\n", sizeof(int64_t), 0, UINT64_MAX);
#pragma GCC diagnostic warning "-Wformat"
  printf("### printf and scanf format specifiers ###\n");
  printf("# printf/scanf macros are in the format PRI/SCN{fmt}{type}\n");
  printf("PRId8:   %s, PRIx8:   %s, PRIo8:   %s, PRIu8:   %s, PRIi8:   %s\n",
         PRId8,        PRIx8,       PRIo8,       PRIu8,       PRIi8);
  printf("PRId16:  %s, PRIx16:  %s, PRIo16:  %s, PRIu16:  %s, PRIi16:  %s\n",
         PRId16,       PRIx16,      PRIo16,      PRIu16,      PRIi16);
  printf("PRId32:  %s, PRIx32:  %s, PRIo32:  %s, PRIu32:  %s, PRIi32:  %s\n",
         PRId32,       PRIx32,      PRIo32,      PRIu32,      PRIi32);
  printf("PRId64: %s, PRIx64: %s, PRIo64: %s, PRIu64: %s, PRIi64: %s\n",
         PRId64,      PRIx64,     PRIo64,     PRIu64,     PRIi64);
  // printf("usage example: INT64_MAX = %"PRId64"\n", INT64_MAX);

#if __STDC_VERSION__ > 201710L
  // An unspecified value larger than 201710L (C17) is used for the experimental
  // and incomplete -std=c23 and -std=gnu23 modes.
  // 2024-06, gcc 14.1 -std=c23 => 202000L (final C23 will be 202311L)

  printf("\n");
  printf("## Bit precise integer types (since C23) ##\n");
  // still experimental, not recommended
  // CLANG: _ExtInt(N) is now a deprecated alias for _BitInt(N)
  printf("BITINT_MAXWIDTH = %d and ULLONG_WIDTH = %d\n", BITINT_MAXWIDTH, ULLONG_WIDTH);
  _BitInt(3) i3bit = -4; // or signed _BitInt(3); 3-bit values from interval <-4, 3>
  printf("The 3-bit example variable has a value of %d and sizeof %lu byte\n",
         (int)i3bit, sizeof(i3bit));
  printf("sizeof _BitInt(15) = %lu bytes\n", sizeof(_BitInt(15))); // padding bits
  printf("sizeof _BitInt(16) = %lu bytes\n", sizeof(_BitInt(16)));
  printf("sizeof _BitInt(17) = %lu bytes\n", sizeof(_BitInt(17))); // padding bits

  printf("\n");
  printf("## Fixed width floating point types (since C23) ##\n");
  // still experimental, not recommended
  // C type _FloatN in C++ as std::floatN_t type (N = 16, 32, 64, 128)
  printf("sizeof _Float16 = %lu, _Float32 = %lu, _Float64 = %lu, _Float128 = %lu bytes\n",
         sizeof(_Float16), sizeof(_Float32), sizeof(_Float64), sizeof(_Float128));

  printf("\n");
  printf("## Decimal floating point types (since C23) ##\n");
  // https://en.wikipedia.org/wiki/Decimal_floating_point
  // C type _DecimalN (N = 32, 64, 128)
  printf("sizeof _Decimal32 = %lu, _Decimal64 = %lu, _Decimal128 = %lu bytes\n",
         sizeof(_Decimal32), sizeof(_Decimal64), sizeof(_Decimal128));
#endif

  printf("\ninfo: GNU gcc %s\n",  __VERSION__);
  printf("info: GNU C Library (glibc) %d.%d\n", __GLIBC__, __GLIBC_MINOR__);
  printf("info: __STDC_VERSION__ = %ld\n", __STDC_VERSION__);
}
