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
  int sock, newsock, serv_port, addr_len, client_len, msg_bytes;
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

  sock = socket(AF_INET, SOCK_STREAM, 0); // TCP, 0 = IPPROTO_IP
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

  if (listen(sock, 50) < 0) {
    perror("listen failed");
    close(sock);
    return EXIT_FAILURE;
  }

  client_len = sizeof(client_addr);
  while (1) {
    printf("accepting ...\n");
    memset(&client_addr, 0, client_len); // TREBA, NETREBA
    newsock = accept(sock, (struct sockaddr *)&client_addr, (socklen_t *)&client_len);
    if (newsock < 0) {
      perror("accept failed");
      // ALEBO NIC NEROBIT ?!
      close(sock);
      continue; // asi to lepsie ako return
      break;    // asi to lepsie ako return
      return EXIT_FAILURE;
    } else
      printf("accepted %s\n", printhost(client_addr, 0));

    memset(buf, 0, BUF_SIZE);
    printf("reading ...\n");
    msg_bytes = read(newsock, buf, BUF_SIZE-1);
    if (msg_bytes < 0) perror("read failed");
    else printf("from client(%s) readed %d bytes: %s\n", printhost(client_addr, 1), msg_bytes, buf);

    // writing is unnecessary (only for example)
    printf("writing ...\n");
    msg_bytes = write(newsock, "serverTCP_message", 17);
    if (msg_bytes < 0) perror("write failed");
    else printf("to client(%s) writed %d bytes: %s\n", printhost(client_addr, 1), msg_bytes, "serverTCP_message");

    // Kolisnichenko.2012.Razrabotka.Linux.prilozhenij.Rus.pdf
    // функции send() и recv(), предназначенные именно для работы с сокетами.
    // man 3 read
    // If fildes refers to a socket, read() shall be equivalent to recv() with no flags set.
    //
    // The recv() call is normally used only on a connected socket (see connect(2)).
    // It is equivalent to the call:      recvfrom(fd, buf, len, flags, NULL, 0));

    // Рекомендуется сначала вызвать функцию shutdown(), а затем уже вызывать close()
    if (shutdown(newsock, SHUT_RD) < 0) // ?!
      perror("shutdown failed");
    close(newsock);
  }

  shutdown(sock, SHUT_RD); // ?!
  close(sock);
}
