#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#define SOCK_NAME "socket.file" // better /tmp/socket.file
#define BUF_SIZE 256

int main(int argc, char ** argv)
{
  int sock;
  sock = socket(AF_UNIX, SOCK_DGRAM, 0);
  char buf[BUF_SIZE];
  struct sockaddr srvr_name;

  if (sock < 0) {
    perror("socket failed");
    return EXIT_FAILURE;
  }
  srvr_name.sa_family = AF_UNIX;
  strcpy(srvr_name.sa_data, SOCK_NAME); // socket.file
  strcpy(buf, "Hello, Unix sockets!");
  int bytes = sendto(sock, buf, strlen(buf), 0, &srvr_name,
                     strlen(srvr_name.sa_data) + sizeof(srvr_name.sa_family));

  printf("sock = %d, bytes = %d\n", sock, bytes);
  printf("srvr_name.sa_data = %s\n", srvr_name.sa_data);
  printf("Client sending: %s\n", buf);
}
