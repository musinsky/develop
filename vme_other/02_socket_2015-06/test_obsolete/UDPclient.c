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

  // UDP protocol. This type of socket is non-connection socket
  sock = socket(AF_INET, SOCK_DGRAM, 0); // UDP
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

  printf("sending ...\n");
  strcpy(buf, argv[3]); // now not needed memset(buf, 0, BUF_SIZE);
  msg_bytes = sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&serv_addr, addr_len);
  if (msg_bytes < 0) perror("sendto failed");
  else printf("to server sended %d bytes: %s\n", msg_bytes, buf);

  // receiving is unnecessary (only for example)
  printf("receiving ...\n");
  memset(buf, 0, BUF_SIZE);
  msg_bytes = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&serv_addr, &addr_len);
  if (msg_bytes < 0) perror("recvfrom failed");
  else printf("from server received %d bytes: %s\n", msg_bytes, buf);

  close(sock);
  return EXIT_SUCCESS;
}
