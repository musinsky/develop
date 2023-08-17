# 2023-08-18

GFC	= gfortran
FEXT	= f
GFCFLAGS=

all:
	@echo 'default do nothing, enter the name of the program'
	@echo 'for example: make program_name'

ascii2bin:
bin2ascii:
prc_dvo:
fortran.write:
fortran.read:

%:		%.$(FEXT)
		$(GFC) $(GFCFLAGS) $< -o $@

collider:	collider.$(FEXT)
		$(GFC) $(GFCFLAGS) $< -o $@ $$(cernlib packlib)

clean:
	rm -fv ascii2bin bin2ascii prc_dvo collider fortran.write fortran.read
	rm -fv fort.65 kinem.dat kinem.his   # collider
	rm -fv fortran.io.*   # fortran.write
