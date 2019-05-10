// Author: Jan Musinsky
// 03/06/2015

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

#define BUF_SIZE 500 // same as in server

int main(int argc, char *argv[])
{
  if (argc !=4 ) {
    fprintf(stderr, "Usage:\n %s server port message\n", argv[0]);
    fprintf(stderr, "Example:\n %s strela.jinr.ru 7503 \"$(date +%T_%N)\"\n", argv[0]);
    return EXIT_FAILURE;
  }

  struct addrinfo hints;
  struct addrinfo *result, *rp;
  int status, sfd;

  // All the other fields in the structure pointed to by hints must contain either 0 or a null pointer
  memset(&hints, 0, sizeof(hints));
  hints.ai_family   = AF_UNSPEC;   // IPv4 OR IPv6
  hints.ai_socktype = SOCK_STREAM; // TCP

  status = getaddrinfo(argv[1], argv[2], &hints, &result);
  if (status != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return EXIT_FAILURE;
  }

  // result now points to a linked list of one or more struct addrinfo
  // try each address until first we successfully connect (in practice first one)
  for (rp = result; rp != NULL; rp = rp->ai_next) {
    sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sfd == -1) { // try next address
      perror("socket failed");
      continue;
    }

    printf("connecting ...\n");
    if (connect(sfd, rp->ai_addr, rp->ai_addrlen) == 0) break; // first success
    perror("connect failed");
    close(sfd);
  }

  if (rp == NULL) {
    fprintf(stderr, "no address succeeded connect\n");
    close(sfd);
    return EXIT_FAILURE;
  }

  char hbuf[NI_MAXHOST]; // 1025
  char sbuf[NI_MAXSERV]; // 32
  int flags = NI_NUMERICSERV; // no name (from /etc/services) only numeric

  //  // direct example
  //  status = getnameinfo(rp->ai_addr, rp->ai_addrlen, hbuf, NI_MAXHOST, sbuf, NI_MAXSERV, flags);
  //  if (status != 0) fprintf(stderr, "getnameinfo: %s\n", gai_strerror(status));
  //  else printf ("connected server %s:%s\n", hbuf, sbuf);
  //  // end direct example

  // example with getpeername()
  struct sockaddr_storage peer_addr; // for IPv4 only   struct sockaddr_in peer_addr;
  socklen_t peer_addr_len = sizeof(peer_addr);
  status = getpeername(sfd, (struct sockaddr *)&peer_addr, &peer_addr_len);
  if (status != 0) perror("getpeername failed");
  status = getnameinfo((struct sockaddr *)&peer_addr, peer_addr_len, hbuf, NI_MAXHOST, sbuf, NI_MAXSERV, flags);
  if (status != 0) fprintf(stderr, "getnameinfo: %s\n", gai_strerror(status));
  else printf ("connected server %s:%s\n", hbuf, sbuf);
  // end example with getpeername()

  freeaddrinfo(result); // rp (from result) needed for direct example

  char buf[BUF_SIZE];
  ssize_t bytes_recieved, bytes_sent;

  printf("sending ...\n");
  strcpy(buf, argv[3]); // now not needed memset(buf, 0, BUF_SIZE);
  if (strlen(buf) == 0) buf[0] = '\0';
  bytes_sent = send(sfd, buf, strlen(buf), 0);
  if (bytes_sent == -1) perror("send failed");
  else printf("to server sended %d bytes: %s\n", bytes_sent, buf);

  // !!!!!
  // send() might not send all the bytes (you want it to send 512 bytes, but it returns 412)
  // !!!!!

  memset(buf, 0, BUF_SIZE);
  printf("receiving ...\n");
  bytes_recieved = recv(sfd, buf, BUF_SIZE-1, 0);
  //  buf[bytes_recieved] = '\0';
  if (bytes_recieved == -1) perror("recv failed");
  else if (bytes_recieved == 0) fprintf(stderr, "server closed connection (shutdown)\n");
  else printf("from server received %d bytes: %s\n", bytes_recieved, buf);

  shutdown(sfd, SHUT_RD);
  close(sfd);
  return EXIT_SUCCESS;
}
