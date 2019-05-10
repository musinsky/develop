// Author: Jan Musinsky
// 03/06/2015

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
  struct addrinfo *result;
  struct addrinfo *rp;
  int ret;

  if ((argc < 2) || (argc > 3)) {
    fprintf(stderr, "Usage:\n %s host [port]\n", argv[0]);
    fprintf(stderr, "Example:\n %s www.example.com\n", argv[0]);
    fprintf(stderr, " %s alice.saske.sk 23\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  ret = getaddrinfo(argv[1], argv[2], NULL, &result);
  if (ret != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
    exit(EXIT_FAILURE);
  }

  for (rp = result; rp != NULL; rp = rp->ai_next) {
    //    printf("ai_flags: %d\n", rp->ai_flags);
    printf("ai_flags: 0x%04X =>", rp->ai_flags);
    if (rp->ai_flags & AI_PASSIVE)    printf(" PASSIVE[0x%04X]", AI_PASSIVE);
    if (rp->ai_flags & AI_CANONNAME)  printf(" CANONNAME[0x%04X]", AI_CANONNAME);
    if (rp->ai_flags & AI_V4MAPPED)   printf(" V4MAPPED[0x%04X]", AI_V4MAPPED);
    if (rp->ai_flags & AI_ALL)        printf(" ALL[0x%04X]", AI_ALL);
    if (rp->ai_flags & AI_ADDRCONFIG) printf(" ADDRCONFIG[0x%04X]", AI_ADDRCONFIG);
    printf("\n");
    printf("ai_family: %d => ", rp->ai_family);
    if      (rp->ai_family == AF_INET)  printf("IPv4\n");
    else if (rp->ai_family == AF_INET6) printf("IPv6\n");
    else                                printf("other\n");
    printf("ai_socktype: %d => ", rp->ai_socktype);
    if      (rp->ai_socktype == SOCK_STREAM) printf("SOCK_STREAM\n");
    else if (rp->ai_socktype == SOCK_DGRAM)  printf("SOCK_DGRAM\n");
    else if (rp->ai_socktype == SOCK_RAW)    printf("SOCK_RAW\n");
    else                                     printf("other\n");
    printf("ai_protocol: %d => ", rp->ai_protocol); // /etc/protocols
    struct protoent *proto = getprotobynumber(rp->ai_protocol);
    printf("%s\n", proto->p_name);
    printf("ai_addrlen: %d\n", rp->ai_addrlen);
    printf("ai_canonname: %s\n", rp->ai_canonname);
    //    printf("ai_addr->sa_data: %s\n", rp->ai_addr->sa_data);
    printf("ai_addr->sa_family: %d\n", rp->ai_addr->sa_family);

    char hbuf[NI_MAXHOST]; // 1025
    char sbuf[NI_MAXSERV]; // 32
    int flags = NI_NUMERICHOST | NI_NUMERICSERV;
    flags = 0;
    ret = getnameinfo(rp->ai_addr, rp->ai_addrlen, hbuf, NI_MAXHOST, sbuf, NI_MAXSERV, flags);
    if (ret != 0) {
      fprintf(stderr, "getnameinfo: %s\n", gai_strerror(ret));
      continue;
    }
    printf("host: %s\n", hbuf);
    printf("serv: %s\n\n", sbuf); // see /etc/services
  }

  freeaddrinfo(result);
  return EXIT_SUCCESS;
}
