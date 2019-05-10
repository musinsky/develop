// Author: Jan Musinsky
// 28/05/2015

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

char *printhost(const struct sockaddr_in addr, int onlyhn)
{
  struct hostent *host = gethostbyaddr(
      (const char *)&addr.sin_addr.s_addr, sizeof(addr), AF_INET);
  if (!host) return "";
  if (onlyhn) return host->h_name;

  char **names;
  printf("\n=== host info ===\n");
  printf("name: %s\n", host->h_name);
  printf("length: %d\n", host->h_length);
  printf("addrtype: %d\n", host->h_addrtype);
  printf("addresses:");
  names = host->h_addr_list;
  while (*names) {
    printf(" %s", inet_ntoa(* (struct in_addr *)*names));
    names++;
  }
  printf("\naliases:");
  names = host->h_aliases;
  while (*names) {
    printf(" %s", *names);
    names++;
  }
  printf("\n=================\n");
  return "";
}

#define BUF_SIZE 256

int main(int argc, char *argv[])
{
  char buf[BUF_SIZE];
  int sock, serv_port, addr_len, client_len, msg_bytes;
  struct sockaddr_in serv_addr, client_addr;

  if (argc < 2) {
    fprintf(stderr, "Usage:\n %s <port_number>\n", argv[0]);
    fprintf(stderr, "Example:\n %s 54321\n", argv[0]);
    return EXIT_FAILURE;
  }

  serv_port = atoi(argv[1]);
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
  //  memset((char *)&serv_addr, 0, addr_len);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(serv_port);

  if (bind(sock, (struct sockaddr *)&serv_addr, addr_len) < 0) {
    perror("bind failed");
    close(sock);
    return EXIT_FAILURE;
  }

  client_len = sizeof(client_addr);
  while (1) {
    printf("receiving ...\n");
    memset(buf, 0, BUF_SIZE);
    //    msg_bytes = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&serv_addr, &addr_len);
    msg_bytes = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, &client_len);
    if (msg_bytes < 0) perror("recvfrom failed");
    else printf("from client(%s) received %d bytes: %s\n", printhost(client_addr, 1), msg_bytes, buf);

    // sending is unnecessary (only for example)
    printf("sending ...\n");
    //    msg_bytes = sendto(sock, "serverUDP_message", 17, 0, (struct sockaddr *)&serv_addr, addr_len);
    msg_bytes = sendto(sock, "serverUDP_message", 17, 0, (struct sockaddr *)&client_addr, client_len);
    if (msg_bytes < 0) perror("sendto failed");
    else printf("to client(%s) sended %d bytes: %s\n", printhost(client_addr, 1), msg_bytes, "serverUDP_message");
    //    printhost(client_addr, 0);
  }

  close(sock);
}
