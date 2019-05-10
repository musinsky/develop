CC		= gcc
CFLAGS		=
CEXT		= c

SOURCES		= $(wildcard *.$(CEXT))
TARGETS		= $(patsubst %.$(CEXT), %, $(SOURCES))

all:		default
default:	$(TARGETS)

$(TARGETS):	$(SOURCES)
		$(CC) $(CFLAGS) $@.$(CEXT) -o $@
clean:
		@rm -fv $(TARGETS)
