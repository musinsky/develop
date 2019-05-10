#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUF_SIZE 256

int main(int argc, char ** argv)
{
  int sock, port;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char buf[BUF_SIZE];

  if (argc < 3) {
    fprintf(stderr,"usage: %s <hostname> <port_number>\n", argv[0]);
    return EXIT_FAILURE;
  }
  port = atoi(argv[2]);
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    printf("socket() failed: %d", errno);
    return EXIT_FAILURE;
  }

  // преобразование доменного имени сервера в его сетевой адрес
  server = gethostbyname(argv[1]);
  if (server == NULL) {
    perror("host failed");
    printf("Host not found\n");
    return EXIT_FAILURE;
  }

  memset((char *) &serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  strncpy((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
  serv_addr.sin_port = htons(port);

  printf("BEFORE\n");
  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("connect failed");
    printf("connect() failed: %d\n", errno);
    return EXIT_FAILURE;
  }
  printf("AFTER\n");

  printf(">");
  memset(buf, 0, BUF_SIZE);
  fgets(buf, BUF_SIZE-1, stdin);
  write(sock, buf, strlen(buf));
  memset(buf, 0, BUF_SIZE);
  read(sock, buf, BUF_SIZE-1);
  printf("%s\n",buf);
  close(sock);
  return 0;
}
