# 2023-09-25

GFC	= gfortran
FEXT	= f
GFCFLAGS=
GCC	= gcc
CEXT	= c
GCCFLAGS=


all:
	@echo 'default do nothing, enter the name of the program'
	@echo 'for example: make program_name'

ascii2bin:
bin2ascii:
prc_dvo:
fortran.write:
fortran.read:
fortran.c.read:

%:		%.$(FEXT)
		$(GFC) $(GFCFLAGS) $< -o $@

%:		%.$(CEXT)
		$(GCC) $(GCCFLAGS) $< -o $@

collider:	collider.$(FEXT)
		$(GFC) $(GFCFLAGS) $< -o $@ $$(cernlib packlib)

clean:
	rm -fv ascii2bin bin2ascii prc_dvo collider
	rm -fv fortran.write fortran.read fortran.c.read
	rm -fv fort.65 kinem.dat kinem.his   # collider
	rm -fv fortran.io.*   # fortran.write
