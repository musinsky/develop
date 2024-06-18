// 2024-06-19

// $ g++ -std=c++20 -pedantic -Wall -Wextra data.type.cast.cxx -o data.type.cast
//       -fstrict-aliasing # (optional) strictest aliasing rules
// see also 'data.type.punning.c' for C punning (casting)

#include <bitset>
#include <cassert>
#include <climits>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <limits>

// https://en.cppreference.com/w/cpp/language/object#Strict_aliasing
// https://en.cppreference.com/w/cpp/language/reinterpret_cast
// https://en.cppreference.com/w/cpp/numeric/bit_cast
// https://en.cppreference.com/w/cpp/string/byte/memcpy
// https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#index-fstrict-aliasing

// Strict aliasing
//
// Accessing an object using an expression of a type other than the type with
// which it was created is undefined behavior in many cases.
//
// $ gcc/g++ -fstrict-aliasing   # option is enabled at levels -O2, -O3, -Os
// Allow the compiler to assume the strictest aliasing rules applicable to the
// language being compiled. For C (and C++), this activates optimizations based
// on the type of expressions. In particular, an object of one type is assumed
// never to reside at the same address as an object of a different type, unless
// the types are almost the same. For example, an 'unsigned int' can alias an
// 'int', but not a 'void*' or a 'double'.
//
// Strict aliasing prohibits examining the same memory as values of two
// different types. When it is needed to interpret the bytes of an object as a
// value of a different type, std::memcpy or std::bit_cast (since C++20) can be
// used to convert the values.
//
// reinterpret_cast (or equivalent explicit cast) between pointer or reference
// types shall not be used to reinterpret object representation in most cases
// because of the type aliasing rule (exception with std::intptr_t).

int main ()
{
  // // fixed width floating point types (since C++23)
  // // https://en.cppreference.com/w/cpp/types/floating-point
  // #include <stdfloat>
  // std::float32_t f32 = 3.14f32;
  // std::cout << "f32 = " << f32 << std::endl;

  const float pi = 3.14; // 3.14 = 1078523331 as integer, 0x4048f5c3 as hex
  float f32 = pi;
  std::uint32_t i32 = 0; // any 32-bit word as unsigned integer
  static_assert(sizeof(float) == sizeof(std::uint32_t));
  static_assert(std::numeric_limits<float>::is_iec559); // IEEE 754 standard

  // $ g++ -fstrict-aliasing => warning: dereferencing type-punned pointer will
  // break strict-aliasing rules [-Wstrict-aliasing]
  std::cout << "# reinterpret_cast, not recommended (break strict aliasing)\n";
  i32 = *reinterpret_cast<std::uint32_t*>(&f32); // undefined behavior, aliasing violation
  //  = *(uint32_t*)&f32; // C equivalent
  std::cout << "f32 = " << f32 << '\n'
            << "i32 = " << i32 << '\n'
            << "f32 (back) = " << *reinterpret_cast<float*>(&i32) << '\n';

  std::cout << "\n# reinterpret_cast exception with std::(u)intptr_t\n";
  // uintptr_t: unsigned integer type capable of holding a pointer to void
  f32 = pi;
  std::uintptr_t uip = reinterpret_cast<std::uintptr_t>(&f32); // static_cast is an error
  i32 = *reinterpret_cast<std::uint32_t*>(uip);
  std::cout << "f32 = " << f32 << '\n'
            << "i32 = " << i32 << '\n';
  uip = reinterpret_cast<std::uintptr_t>(&i32);
  std::cout << "f32 (back) = " << *reinterpret_cast<float*>(uip) << '\n';

  std::cout << "\n# std::bit_cast (since C++20)\n";
  f32 = pi;
  i32 = std::bit_cast<std::uint32_t>(f32);
  std::cout << "f32 = " << f32 << '\n'
            << "i32 = " << i32 << " = 0x" << std::hex << i32 << std::dec
            << " = " << std::bitset<CHAR_BIT*sizeof(i32)>(i32) << '\n'
            << "f32 (back) = " << std::bit_cast<float>(i32) << '\n';

  std::cout << "\n# std::memcpy, recommended type cast\n";
  // memcpy is best and safe method for type punning (type cast)
  f32 = pi;
  std::memcpy(&i32, &f32, sizeof(f32));
  std::cout << "f32 = " << f32 << '\n'
            << "i32 = " << i32 << " = 0x" << std::hex << i32 << std::dec
            << " = " << std::bitset<CHAR_BIT*sizeof(i32)>(i32) << '\n';
  float ff32 = 0.0;
  std::memcpy(&ff32, &i32, sizeof(float));
  std::cout << "f32 (back) = " << ff32 << '\n';

  // union type is an allowed form of type punning (type cast) in C, provided
  // that the member read is not larger than the one whose value was set
  // (otherwise the read has unspecified behavior). The same is syntactically
  // valid but has undefined behavior in C++.
  //
  // In C++ it works as a GNU gcc extension (but not as part of the C++
  // standard), see: https://stackoverflow.com/q/67206482
  std::cout << "\n# union, not safe in C++ (allowed in C)\n";
  union { float u_f32; std::int32_t u_i32; std::uint32_t u_ui32; } word32;
  word32.u_f32 = pi;
  std::cout << "u_f32  = " << word32.u_f32 << '\n'
            << "u_i32  = " << word32.u_i32 << '\n'
            << "u_ui32 = " << word32.u_ui32 << '\n';
}
