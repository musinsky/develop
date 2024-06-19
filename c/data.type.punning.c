// 2024-06-19

// $ gcc -pedantic -Wall -Wextra -o data.type.punning data.type.punning.c && ./data.type.punning
//       -fstrict-aliasing # (optional) strictest aliasing rules
// see also 'data.type.cast.cxx' for C++ casting (punning)

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <limits.h>
#include <string.h>

// https://en.wikipedia.org/wiki/Type_conversion
// https://en.wikipedia.org/wiki/Type_punning
// https://en.wikipedia.org/wiki/Union_type
// https://learn.microsoft.com/en-us/cpp/cpp/type-conversions-and-type-safety-modern-cpp
// https://learn.microsoft.com/en-us/cpp/cpp/standard-conversions

int main() {
  assert(CHAR_BIT == 8);                    // 1 byte = 8 bits
  assert(sizeof(int32_t) == sizeof(float)); // types with same size (4 bytes, 32 bits)
  assert(__STDC_IEC_559__ == 1);            // IEEE 754 standard
  // https://en.cppreference.com/w/c/23
  // __STDC_IEC_559__ (since C99) deprecated (since C23)
  // macro __STDC_IEC_60559_BFP__ (since C23) supersedes __STDC_IEC_559__
  const float pi = 3.14; // 3.14 = 1078523331 as integer, 0x4048f5c3 as hex

  // The declared type of 'x' is 'float' but it is read through an expression of
  // type 'unsigned int'. On many common platforms, this use of pointer punning
  // can create problems if different pointers are aligned in machine specific
  // ways. Furthermore, pointers of different sizes can alias accesses to the
  // same memory, causing problems that are unchecked by the compiler.
  //
  // Fixed width integer types (u)intN_t have no padding (no aligned problems).
  float f32 = pi;
  int32_t i32 = 0;

  // $ gcc -fstrict-aliasing => warning: dereferencing type-punned pointer will
  // break strict-aliasing rules [-Wstrict-aliasing]   # -fno-strict-aliasing
  //#pragma GCC diagnostic push
  //#pragma GCC diagnostic ignored "-Wstrict-aliasing"
  printf("# use of pointers, not recommended (break strict aliasing), undefined behavior\n");
  // The C standard's aliasing rules state that an object shall have its stored
  // value accessed only by an lvalue expression of a compatible type. The types
  // 'float' and 'int32_t' are not compatible, therefore this code's behavior is
  // undefined. Although on GCC and LLVM this particular program compiles and
  // runs as expected.
  i32 = *(int32_t*)&f32; // undefined behavior, aliasing violation
  //  = *reinterpret_cast<int32_t*>(&f32); // C++ equivalent
  printf("f32 = %f\n", f32);
  printf("i32 = %d\n", i32);
  printf("f32 (back) = %f\n", *(float*)&i32);
  //#pragma GCC diagnostic pop

  // $ gcc -fstrict-aliasing => no warning, but still aliasing violation
  printf("# the same in a different way\n");
  f32 = pi;
  int32_t *i32ptr = (int32_t *)&f32;
  float   *f32ptr = (float *)i32ptr; // = (float *)&(*i32ptr)
  printf("f32 = %f\n", f32);
  printf("*i32ptr = %d\n", *i32ptr);
  printf("*f32ptr (back) = %f\n", *f32ptr);
  f32 = -pi;
  printf("f32 = %f (and without any expressions)\n", f32);
  printf("*i32ptr = %d\n", *i32ptr);
  printf("*f32ptr (back) = %f\n", *f32ptr);

  printf("\n# memcpy, recommended type punning\n");
  // memcpy is best and safe method for type punning (type cast)
  f32 = pi;
  memcpy(&i32, &f32, sizeof(f32));
  printf("f32 = %f\n", f32);
  // printf("f32 = %a\n", f32); // double in hexadecimal notation (mantissa + exponent)
  printf("i32 = %d = 0x%x\n", i32, i32); // PRIx32 = 'x'
  float ff32 = 0.0;
  memcpy(&ff32, &i32, sizeof(i32));
  printf("f32 (back) = %f\n", ff32);

  printf("\n# union, allowed in C (not safe in C++)\n");
  union { float u_f32; int32_t u_i32; uint32_t u_ui32; } word32;
  word32.u_f32 = pi;
  printf("u_f32  = %f\n", word32.u_f32);
  printf("u_i32  = %d\n", word32.u_i32);
  printf("u_ui32 = %u\n", word32.u_ui32); // be careful with '%d' (unsign print as sign)
}
