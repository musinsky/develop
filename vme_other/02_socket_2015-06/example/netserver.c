#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_SIZE 256

int main(int argc, char ** argv)
{
  int sock, newsock, port, clen;
  char buf[BUF_SIZE];
  struct sockaddr_in serv_addr, cli_addr;
  if (argc < 2) {
    fprintf(stderr,"usage: %s <port_number>\n", argv[0]);
    return EXIT_FAILURE;
  }

  // В первом параметре функции socket() мы передаем константу AF_INET,
  // указывающую на то, что открываемый сокет должен быть сетевым.
  // Значение второго параметра требует, чтобы сокет был потоковым.
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    printf("socket() failed: %d\n", errno);
    return EXIT_FAILURE;
  }

  memset((char *) &serv_addr, 0, sizeof(serv_addr));
  port = atoi(argv[1]);
  // семейство адресов Интернета AF_INET
  serv_addr.sin_family = AF_INET;
  // в качестве самого адреса – специальную константу INADDR_ANY,
  // наша программа сервер зарегистрируется на всех адресах той машины, на которой она выполняется.
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY;
  // htons() переписывает двухбайтовое значение порта так,
  // чтобы порядок байтов соответствовал принятому в Интернете
  serv_addr.sin_port = htons(port);
  if (bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("bind failed");
    printf("bind() failed: %d\n", errno);
    return EXIT_FAILURE;
  }

  // Сетевой сервер должен уметь выполнять запросы множества клиентов одновременно,
  // наш сервер netserver.c фактически может обработать запрос только одного клиента, но в общем случае.
  // При этом в соединениях «точка-точка», например, при использовании потоковых сокетов, для каждого клиента
  // у сервера должен быть открыт отдельный сокет. Из этого следует, что мы не должны устанавливать соединение
  // с клиентом через сам сокет sock, предназначенный для прослушивания входящих запросов (обычно, при использовании
  // сетевых сокетов мы и не можем этого сделать), иначе все другие попытки соединиться с сервером
  // по указанному адресу и порту будут заблокированы. Вместо этого мы вызываем функцию listen(2),
  // которая переводит сервер в режим ожидания запроса на соединение
  listen(sock, 1);
  clen = sizeof(cli_addr);

  printf("BEFORE\n");
  // вызываем функцию accept(2), которая устанавливает соединение в ответ на запрос клиента
  // Получив запрос на соединение, функция accept() возвращает новый сокет, открытый для обмена данными с клиентом,
  // запросившим соединение. Сервер как бы перенаправляет запрошенное соединение на другой сокет, оставляя сокет sock
  // свободным для прослушивания запросов на установку соединения.
  newsock = accept(sock, (struct sockaddr *)&cli_addr, &clen);
  if (newsock < 0) {
    printf("accept() failed: %d\n", errno);
    return EXIT_FAILURE;
  }
  printf("AFTER\n");
  printf("sock = %d, newsock = %d\n", sock, newsock);
  printf("cli_addr.sin_port = %d, cli_addr.sin_addr = %d\n", cli_addr.sin_port, cli_addr.sin_addr);

  memset(buf, 0, BUF_SIZE);
  printf("BEFORE 2\n");
  read(newsock, buf, BUF_SIZE-1);
  printf("AFTER 2\n");
  buf[BUF_SIZE] = 0;
  printf("MSG: %s\n", buf);
  write(newsock, "OK from server", 2+12);
  close(newsock);
  close(sock);
}
