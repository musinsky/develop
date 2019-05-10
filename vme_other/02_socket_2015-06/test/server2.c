// Author: Jan Musinsky
// 29/06/2015

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

#define LISTEN_BACKLOG 5
#define BUF_SIZE 4096+1 // same (or bigger) as in client

int main(int argc, char *argv[])
{
  if (argc != 2) {
    fprintf(stderr, "Usage:\n %s port\n", argv[0]);
    fprintf(stderr, "Example:\n %s 7503\n", argv[0]);
    return EXIT_FAILURE;
  }

  struct addrinfo hints;
  struct addrinfo *result, *rp;
  int status, sfd;

  // All the other fields in the structure pointed to by hints must contain either 0 or a null pointer
  memset(&hints, 0, sizeof(hints));
  hints.ai_family   = AF_UNSPEC;   // IPv4 OR IPv6
  hints.ai_socktype = SOCK_STREAM; // TCP
  hints.ai_flags    = AI_PASSIVE;  // intended for bind (wildcard my IP address)
  //  hints.ai_protocol = 0;           // any protocol (0 = IPPROTO_IP) see too /etc/protocols

  status = getaddrinfo(NULL, argv[1], &hints, &result);
  if (status != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return EXIT_FAILURE;
  }

  // result now points to a linked list of one or more struct addrinfo
  // try each address until first we successfully bind (in practice first one)
  for (rp = result; rp != NULL; rp = rp->ai_next) {
    sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sfd == -1) { // try next address
      perror("socket failed");
      continue;
    }

    // bind failed: Address already in use
    // make sure the address (port) is not in use by a previous execution of our code
    int reuse = 1;
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
      perror("setsockopt failed");
      close(sfd);
      freeaddrinfo(result);
      return EXIT_FAILURE;
    }

    if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0) break; // first success
    perror("bind failed");
    close(sfd);
  }

  if (rp == NULL) {
    fprintf(stderr, "no address succeeded bind\n");
    close(sfd);
    return EXIT_FAILURE;
  }
  freeaddrinfo(result);

  //  // example with getsockopt
  //  int optvalue;
  //  socklen_t optvalue_len = sizeof(optvalue);
  //  getsockopt(sfd, SOL_SOCKET, SO_RCVBUF, &optvalue, &optvalue_len);
  //  printf("socket: %d with SO_RCVBUF: %d\n", sfd, optvalue); // /proc/sys/net/ipv4/tcp_rmem
  //  getsockopt(sfd, SOL_SOCKET, SO_SNDBUF, &optvalue, &optvalue_len);
  //  printf("socket: %d with SO_SNDBUF: %d\n", sfd, optvalue); // /proc/sys/net/ipv4/tcp_wmem
  //  // end example with getsockopt

  if (listen(sfd, LISTEN_BACKLOG) == -1) { // /proc/sys/net/core/somaxconn
    perror("listen failed");
    close(sfd);
    return EXIT_FAILURE;
  }

  // sockaddr_storage is designed to fit in both a struct sockaddr_in (IPv4) and struct sockaddr_in6 (IPv6)
  struct sockaddr_storage peer_addr; // for IPv4 only   struct sockaddr_in peer_addr;
  socklen_t peer_addr_len = sizeof(peer_addr);
  int newsfd;
  char buf[BUF_SIZE];
  ssize_t bytes_received, bytes_sent, tot_received;

  while (1) {
    printf("accepting ...\n");
    newsfd = accept(sfd, (struct sockaddr *)&peer_addr, &peer_addr_len);
    if (newsfd == -1) {
      perror("accept failed");
      //      close(sfd);
      //      return EXIT_FAILURE;
      continue;
    }

    char hbuf[NI_MAXHOST]; // 1025
    char sbuf[NI_MAXSERV]; // 32
    int flags = NI_NUMERICSERV; // no name (from /etc/services) only numeric
    status = getnameinfo((struct sockaddr *)&peer_addr, peer_addr_len, hbuf, NI_MAXHOST, sbuf, NI_MAXSERV, flags);
    if (status != 0) {
      fprintf(stderr, "getnameinfo: %s\n", gai_strerror(status));
      close(newsfd);
      continue;
    }
    printf("accepted client %s:%s\n", hbuf, sbuf);
    // order accepted ports (services) from F22 client: 60999 -> 61000 -> 32768 -> 32769 -> ...


    // Communicating with a socket is done using unix file descriptors (file handlers) and its associated
    // functions like write() and read(). For internet sockets send() and recv() are preferred.

    // The recv() call is normally used only on a connected socket (on client side in this example)
    // It is equivalent to the call: recvfrom(sockfd, buf, len, flags, NULL, 0);
    //
    // If fildes refers to a socket, read() shall be equivalent to recv() with no flags set.

    tot_received = 0;
    printf("receiving ...\n");
    while (1) {
      bytes_received = recv(newsfd, buf, BUF_SIZE-1, 0); // maybe set socket in non-blocking mode ?!
      tot_received += bytes_received;
      if (bytes_received == -1) perror("recv failed");
      if (bytes_received <= 0) break;
      // while bytes_received = 0 => jedno spojenie nie je dlhsie ako 300-500 microseconds
      // resp. vsetko co sa send-uje hned za sebou nie z tak vacsou pauzou je jedno spojenie
    }
    printf("from client received %d bytes\n", tot_received);

    // The send() call may be used only when the socket is in a connected state (on client side in this example)
    // It is equivalent to the call: sendto(sockfd, buf, len, flags, NULL, 0);
    //
    // The only difference between send() and write(2) is the presence of flags. With a zero flags argument,
    // send() is equivalent to write(2).
    printf("sending ...\n");
    //    bytes_sent = send(newsfd, "serverTCP_message", strlen("serverTCP_message"), 0);
    sprintf(buf, "%d", rand());
    bytes_sent = send(newsfd, buf, strlen(buf), 0);
    if (bytes_sent == -1) perror("send failed");
    else printf("to client sended %d bytes: %s\n", bytes_sent, buf);

    // Be sure to use shutdown(2) to shut down all parts the connection before closing the socket
    // sure that there is no recv(2).
    // Don't call shutdown() for a UDP socket.
    if (shutdown(newsfd, SHUT_RDWR) == -1) { // shutdown after send/recv from socket
      //      close(newsfd);
      //      close(sfd);
      //      return EXIT_FAILURE;
      perror("shutdown newsfd failed");
    }
    if (close(newsfd) == -1) perror("close newsfd failed");
    printf("-------------------------------------------------\n");
  }

  if (shutdown(sfd, SHUT_RDWR) == -1) perror("shutdown sfd failed");
  if (close(sfd) == -1) perror("close sfd failed");
  return EXIT_SUCCESS;
}
