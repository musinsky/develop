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

# http://citforum.ru/programming/unix/sockets/
# Сокеты в файловом пространстве имен (Unix Domain)
# fsserver
# fsclient

# Парные сокеты
# sockpair

# Сетевые сокеты
# netserver
# netclient

# http://www.thegeekstuff.com/2011/12/c-socket-programming/
# 1server
# 1client

# http://www.linuxhowtos.org/C_C++/socket.htm
# 2server
# 2server_loop
# 2client

# datagram socket (UDP)
# 3server_udp
# 3client_udp

# Sockets in the Unix Domain
# 4server_U
# 4client_U
