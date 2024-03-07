<p align="right">last edit: 2024-03-07</p>

# Fortran
* https://en.wikipedia.org/wiki/Fortran
* https://wg5-fortran.org/
* https://gcc.gnu.org/wiki/GFortranStandards
* https://fortranwiki.org/fortran/show/Standards

---

* https://fortran-lang.org/
* https://fortran-lang.org/learn/quickstart/
* https://en.wikibooks.org/wiki/Fortran

## FORTRAN 77
* https://wg5-fortran.org/fearlier.html

An important practical extension to FORTRAN 77 was the release of MIL-STD-1753
in 1978. This specification, developed by the U.S. Department of Defense,
standardized a number of features implemented by most FORTRAN 77 compilers but
not included in the ANSI FORTRAN 77 standard. These features would eventually be
incorporated into the Fortran 90 standard.

[CERNLIB](https://en.wikipedia.org/wiki/CERN_Program_Library) modules are
written in the FORTRAN 77 language.

## GNU Fortran compiler
* https://gcc.gnu.org/wiki/GFortran
* https://gcc.gnu.org/wiki/GFortranStandards

gfortran is the name of the GNU Fortran project, developing a free Fortran
95/2003/2008/2018 compiler for GCC, the GNU Compiler Collection.

g77 was the predecessor to gfortran. As of gcc-4.0, it is no longer actively
supported. Why is g77 currently not supported? gcc-4.0 changed the front end
interface for all languages in gcc. No volunteer or company stepped up to update
g77 to the gcc-4.0 infrastructure, so it was dropped from the source tree. The
Fortran-95 front end for gcc was under active development, so it has taken g77's
place as the fortran compiler that is part of gcc.

GNU Fortran currently only supports the base Fortran 95 standard (not parts 2 or
3) as amended in the two corrigenda, the Fortran 95 TR 15581 extension, the
Fortran 77 standard with the MIL-STD 1753 extensions, and some portions of the
Fortran 2003 and 2008 standard.

## gfortran
* https://gcc.gnu.org/onlinedocs/gfortran/
* https://gcc.gnu.org/onlinedocs/gfortran/About-GNU-Fortran.html

The GNU Fortran compiler is the successor to `g77`, the Fortran 77 front end
included in GCC prior to version 4 (released in 2005). While it is
backward-compatible with most `g77` extensions and command-line options,
`gfortran` is a completely new implemention designed to support more modern
dialects of Fortran. GNU Fortran implements the Fortran 77, 90 and 95 standards
completely, most of the Fortran 2003 and 2008 standards, and some features from
the 2018 standard. It also implements several extensions including OpenMP and
OpenACC support for parallel programming.

* https://gcc.gnu.org/onlinedocs/gfortran/Standards.html

The GNU Fortran compiler implements ISO/IEC 1539:1997 (Fortran 95). As such, it
can also compile essentially all standard-compliant Fortran 90 and Fortran 77
programs. It also supports the ISO/IEC TR-15581 enhancements to allocatable
arrays.

* https://gcc.gnu.org/onlinedocs/gfortran/GNU-Fortran-and-GCC.html

- Source files with `.f`, `.for`, `.fpp`, `.ftn`, `.F`, `.FOR`, `.FPP`, and
`.FTN` extensions are treated as fixed form.
- Source files with `.f90`, `.f95`, `.f03`, `.f08`, `.F90`, `.F95`, `.F03` and
`.F08` extensions are treated as free form.

The capitalized versions of either form are run through preprocessing. Source
files with the lower case `.fpp` extension are also run through preprocessing.

* https://gcc.gnu.org/onlinedocs/gfortran/Option-Summary.html
* https://gcc.gnu.org/onlinedocs/gfortran/Fortran-Dialect-Options.html

* `-ffree-form` and `-ffixed-form`<br />
Specify the layout used by the source file. The free form layout was introduced
in Fortran 90. Fixed form was traditionally used in older Fortran programs. When
neither option is specified, the source form is determined by the file
extension.

* `-std=std`<br />
Specify the standard to which the program is expected to conform, which may be
one of `f95`, `f2003`, `f2008`, `f2018`, `gnu`, or `legacy`. The default value
for std is `gnu`, which specifies a superset of the latest Fortran standard that
includes all of the extensions supported by GNU Fortran, although warnings will
be given for obsolete extensions not recommended for use in new code. The
`legacy` value is equivalent but without the warnings for obsolete extensions,
and may be useful for old non-standard programs.

### notes 1
GFortran 4.2 (2007-05) [use
now](https://gcc.gnu.org/wiki/GFortran/News#gfortran_4.2) 4-byte record markers
by default for unformatted files to be compatible with g77 and most other
compilers. The implementation allows for records bigger than 2 GB, compatible
with several other compilers. Older versions of GFortran used by default 8-byte
record markers (on most systems); in order to change length of record markers,
e.g. to the read unformatted files created by older gfortran versions, the
`-frecord-marker=8` option can be used.


### notes 2
An INTEGER is any signed number that has no fractional part and no decimal
point. INTEGER numbers are also referred to as fixed point numbers. The other
three numerical types are called floating point numbers. A REAL number is a
signed number with a decimal point.

The integer data type, INTEGER, holds a signed integer. The default size for
INTEGER with no size specified is 4 bytes, and is aligned on 4-byte boundaries.
The usual default size for a REAL item with no size specified is 4 bytes, and is
aligned on 4-byte boundaries. However, these defaults can be changed by
compiling with certain special options. Data types are system dependent.

A REAL element has a sign bit, an 8-bit exponent, and a 23-bit fraction. These
REAL elements in f77 conform to the IEEE standard.

In practice, most computers use 32 bits to store INTEGER and REAL numbers. This
means that an INTEGER is limited to numbers between -2,147,483,648 and
+2,147,483,647 (a sign bit and 31 magnitude bits). If the IEEE standard is used,
then a REAL number will have about seven decimal digits and be within the
magnitude range of 10-38 to 10+38.
