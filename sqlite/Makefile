# 2023-11-08

GCC	= gcc
CEXT	= c
GCCFLAGS= -pedantic -Wall -Wextra

SOURCES	= $(wildcard *.$(CEXT))
TARGETS	= $(patsubst %.$(CEXT), %, $(SOURCES))

all:		default

default:	$(TARGETS)

$(TARGETS):	$(SOURCES)
		$(GCC) $(GCCFLAGS) -lsqlite3 $@.$(CEXT) -o $@

clean:
	rm -fv $(TARGETS)
	rm -fv test.db
