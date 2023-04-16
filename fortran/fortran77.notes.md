<p align="right">last edit: 2023-04-16</p>

# FORTRAN 77
* https://en.wikipedia.org/wiki/CERN_Program_Library

CERNLIB modules are written in the FORTRAN 77 language.

## Fortran
* https://en.wikipedia.org/wiki/Fortran
* https://fortranwiki.org/fortran/show/Standards
* https://wg5-fortran.org/fearlier.html

An important practical extension to FORTRAN 77 was the release of MIL-STD-1753
in 1978. This specification, developed by the U.S. Department of Defense,
standardized a number of features implemented by most FORTRAN 77 compilers but
not included in the ANSI FORTRAN 77 standard. These features would eventually be
incorporated into the Fortran 90 standard.

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

### notes
GFortran 4.2 and 4.3 [use
now](https://gcc.gnu.org/wiki/GFortran/News#gfortran_4.2) 4-byte record markers
by default for unformatted files to be compatible with g77 and most other
compilers. The implementation allows for records bigger than 2 GB, compatible
with several other compilers. Older versions of GFortran used by default 8-byte
record markers (on most systems); in order to change length of record markers,
e.g. to the read unformatted files created by older gfortran versions, the
`-frecord-marker=8` option can be used.
