// Author: Jan Musinsky
// 28/05/2015

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
#include <netdb.h>

#define BUF_SIZE 256

int main(int argc, char *argv[])
{
  char buf[BUF_SIZE];
  int sock, serv_port, addr_len, msg_bytes;
  struct sockaddr_in serv_addr;
  struct hostent *serv_info;

  if (argc !=4 ) {
    fprintf(stderr, "Usage:\n %s <server> <port_number> <message>\n", argv[0]);
    fprintf(stderr, "Example:\n %s strela.jinr.ru 54321 \"$(date +%T_%N)\"\n", argv[0]);
    return EXIT_FAILURE;
  }

  serv_info = gethostbyname(argv[1]); // !!! OBSOLETE !!!
  if (!serv_info) {
    fprintf(stderr, "unknown host: %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  serv_port = atoi(argv[2]);
  if (serv_port < 1024 || serv_port > 65535) { // see /etc/services
    fprintf(stderr, "wrong port range: %d\n", serv_port);
    return EXIT_FAILURE;
  }

  sock = socket(AF_INET, SOCK_STREAM, 0); // TCP, 0 = IPPROTO_IP
  if (sock < 0) {
    perror("socket failed");
    return EXIT_FAILURE;
  }

  addr_len = sizeof(serv_addr);
  memset(&serv_addr, 0, addr_len);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr = *(struct in_addr *)*serv_info->h_addr_list;
  //  strncpy((char *)&serv_addr.sin_addr.s_addr, (char *)host_ser->h_addr, host_ser->h_length);
  serv_addr.sin_port = htons(serv_port);

  // Kolisnichenko.2012.Razrabotka.Linux.prilozhenij.Rus.pdf
  // v tomto priklade neopodstatnene
  // #define CLIENT_PORT 3334
  //
  /* Связываем сокет клиента с портом клиента */
  // bind (sock, (struct sockaddr *)&client, sizeof(client));
  // memset ((char *)&client, '\0', sizeof(server));

  /*
  int optval;
  int optlen;
  // Читаем длину буфера отправки
  optlen = sizeof(optval);
  getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &optval, &optlen);
  printf("value of SO_SNDBUF: %d\n", optval);
   */


  printf("connecting ...\n");
  if (connect(sock, (struct sockaddr *)&serv_addr, addr_len) < 0) {
    perror("connect failed");
    close(sock);
    return EXIT_FAILURE;
  } else
    printf("connected %s:%d\n", argv[1], serv_port);

  printf("writing ...\n");
  strcpy(buf, argv[3]); // now not needed memset(buf, 0, BUF_SIZE);
  msg_bytes = write(sock, buf, strlen(buf));
  if (msg_bytes < 0) perror("write failed");
  else printf("to server writed %d bytes: %s\n", msg_bytes, buf);

  // reading is unnecessary (only for example)
  printf("reading ...\n");
  memset(buf, 0, BUF_SIZE);
  msg_bytes = read(sock, buf, BUF_SIZE-1);
  if (msg_bytes < 0) perror("read failed");
  else printf("from server readed %d bytes: %s\n", msg_bytes, buf);

  shutdown(sock, 0); // ?!
  close(sock);
  return EXIT_SUCCESS;
}
