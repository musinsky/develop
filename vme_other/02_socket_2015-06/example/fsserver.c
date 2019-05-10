#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#define SOCK_NAME "socket.file" // better /tmp/socket.file
#define BUF_SIZE 256

int main(int argc, char ** argv)
{
  struct sockaddr srvr_name, rcvr_name;
  char buf[BUF_SIZE];
  int sock;
  int namelen, bytes;

  // Домен, обозначенный константой AF_UNIX, соответствует сокетам в файловом пространстве имен
  // Второй параметр определяет тип сокета, значение SOCK_DGRAM указывает датаграммный сокет
  sock = socket(AF_UNIX, SOCK_DGRAM, 0);
  if (sock < 0) {
    perror("socket failed");
    return EXIT_FAILURE;
  }

  // семейство адресов файловых сокетов Unix AF_UNIX
  srvr_name.sa_family = AF_UNIX;
  // адрес семейства AF_UNIX представляет собой обычное имя файла сокета
  strcpy(srvr_name.sa_data, SOCK_NAME); // socket.file
  if (bind(sock, &srvr_name, strlen(srvr_name.sa_data) +
           sizeof(srvr_name.sa_family)) < 0) {
    perror("bind failed");
    return EXIT_FAILURE;
  }

  printf("BEFORE\n");
  bytes = recvfrom(sock, buf, sizeof(buf), 0, &rcvr_name, &namelen);
  if (bytes < 0) {
    perror("recvfrom failed");
    return EXIT_FAILURE;
  }
  printf("AFTER\n");

  buf[bytes] = 0;
  rcvr_name.sa_data[namelen] = 0;
  printf("sock = %d, bytes = %d\n", sock, bytes);
  printf("rcvr_name.sa_data = %s\n", rcvr_name.sa_data);
  printf("Client sent: %s\n", buf);
  close(sock);
  unlink(SOCK_NAME); // remove socket.file
}
