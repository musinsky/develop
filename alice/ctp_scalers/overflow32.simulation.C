// 2026-01-21

// $ root overflow32.simulation.C

// 2^32 - 1 =              4.294.967.295 =         0xFFFFFFFF (4 bytes)
// 2^64 - 1 = 18.446.744.073.709.551.615 = 0xFFFFFFFFFFFFFFFF (8 bytes)
//
// In C/C++ standards UNSIGNED integer arithmetic is performed modulo 2^n,
// so overflow is well defined and guaranteed behavior: UINT32_MAX + 1u = 0.
// For SIGNED integers, the result of overflow in C/C++ is in principle
// undefined behavior (compiler can offers options to control handling signed
// integer overflow).
// 4.294.967.295 -> 0 -> 1 # correct (expected) 32-bit unsigned integer overflow
// static_assert((0xFFFFFFFFu + 1u) == 0, "incorrect 32-bit overflow");

// !!! CAREFUL !!! with integer arithmetic and type definition
// ROOT Cling (C/C++ interpreter) on Linux x86_64 (32-bit int, 64-bit long):
// 4294967295u + 4294967295u => (unsigned int) 4294967294 // explicit type definition
// 4294967295  + 4294967295  =>         (long) 8589934590
// 4294967295u + 4294967295  =>         (long) 8589934590
// 4294967295  + 4294967295u =>         (long) 8589934590
// 0u - 4294967295u => (unsigned int) 1                   // explicit type definition
// 0  - 4294967295  =>         (long) -4294967295
// 0u - 4294967295  =>         (long) -4294967295
// 0  - 4294967295u => (unsigned int) 1
// Always should use !!! explicit type definition !!!

// IEEE 754 floating point format precision limitations on integer:
// binary32 (float)  2^24 =            16.777.216
// binary64 (double) 2^53 = 9.007.199.254.740.992
// Integers between 0 and 2^24 (float) or 2^53 (double) can be exactly
// represented, integers greater than 2^24 or 2^53 are rounded.
//
// TTree::Draw() use type casting to double or float. Be CAREFUL when
// displaying very large integer values (values will be rounded).

#include <iostream>
#include <iomanip>

void overflow32_simulation(std::ostream &stream = std::cout) {
  static_assert((std::uint32_t{0xFFFFFFFFu} + 1u) == 0, "incorrect 32-bit overflow");

  std::uint64_t ui64_x = 0, ui64_y = 0;
  std::uint32_t ui32_x = 0, ui32_y = 0, ui32_xPrev = 0, ui32_yPrev = 0;
  std::uint32_t ui32_xDelta = 800000000; //  ~5x8e08 to reach 0xFFFFFFFF
  std::uint32_t ui32_yDelta = 200000000; // ~22x2e08 to reach 0xFFFFFFFF
  std::size_t xCountOverflow = 0, yCountOverflow = 0, count = 0;

  stream << "'x' is always greater than 'y' (condition LMB >= LMA in CTP scalers)\n\n";

  while (xCountOverflow < 10) {
    ui32_x += ui32_xDelta; ui64_x += ui32_xDelta;
    ui32_y += ui32_yDelta; ui64_y += ui32_yDelta;

    stream << "x32: " << std::setw(10) << ui32_x
           << " (real x64: " << std::setw(20) << ui64_x << ")";
    if (ui32_xPrev > ui32_x) {
      xCountOverflow++;
      stream << "   # x32 count overflow: " << xCountOverflow;
    }
    stream << "\n";

    stream << "y32: " << std::setw(10) << ui32_y
           << " (real y64: " << std::setw(20) << ui64_y << ")";
    if (ui32_yPrev > ui32_y) {
      yCountOverflow++;
      stream << "   # y32 count overflow: " << yCountOverflow;
    }
    stream << "\n";

    stream << "x32 increment: " << std::setw(10) << ui32_x << " - " << std::setw(10)
           << ui32_xPrev << " = " << std::setw(10) << ui32_x - ui32_xPrev << "\n";
    stream << "y32 increment: " << std::setw(10) << ui32_y << " - " << std::setw(10)
           << ui32_yPrev << " = " << std::setw(10) << ui32_y - ui32_yPrev << "\n";
    ui32_xPrev = ui32_x;
    ui32_yPrev = ui32_y;

    stream << "epoch: " << std::setw(3) << count << " (condition x32 >= y32 is "
           << (ui32_x >= ui32_y ? "correct" : "INCORRECT") << ")\n\n";
    count++;
  }

  stream << "\n=== Manual restore (last values) ===\n"
         << "x32 = " << std::setw(10) << ui32_x << " + " << std::setw(3) << xCountOverflow
         << "*(0xFFFFFFFFull + 1) = "
         << (std::uint64_t)(ui32_x + xCountOverflow*(0xFFFFFFFFull + 1)) << "\n";
  stream << "y32 = " << std::setw(10) << ui32_y << " + " << std::setw(3) << yCountOverflow
         << "*(0xFFFFFFFFull + 1) = "
         << (std::uint64_t)(ui32_y + yCountOverflow*(0xFFFFFFFFull + 1)) << "\n";
  stream.flush();
}
