* http://man7.org/tlpi/code/online/all_files_by_chapter.html#ch57

* http://beej.us/guide/bgnet/output/html/multipage/index.html
* http://masandilov.ru/network/guide_to_network_programming
* http://codebase.eu/tutorial/linux-socket-programming-c/
* https://www.binarytides.com/socket-programming-c-linux-tutorial/

* http://citforum.ru/programming/unix/sockets/
* https://www.thegeekstuff.com/2011/12/c-socket-programming/
* http://www.linuxhowtos.org/C_C++/socket.htm

* http://rsdn.ru/article/unix/sockets.xml
* http://rus-linux.net/MyLDP/algol/sokets-API-1.html
* https://www.sallyx.org/sally/c/linux/socket

* https://habr.com/ru/post/122754/

* https://habr.com/ru/post/209144/
* https://habr.com/ru/post/209524/
* https://habr.com/ru/post/213559/


# NOTES
postupnost pre TCP client:
 socket (getaddrinfo)
 connect
 send/recv


postupnost pre TCP serve:
 socket (getaddrinfo)
 bind
 listen
    accept    (in loop)
    recv/send (in loop)


- predpokladam pripad bez multi, non-blocking a pod.
- jedno spojenie nie je dlhsie ako 300-500 microseconds, resp. vsetko co sa send-uje hned za sebou nie z tak vacsou pauzou je jedno spojenie
- socket can be connected only once in its life cycle
- pre kazde taketo spojenie musi byt prave jeden connected socket (na strane klienta, server ma stale iba jeden bind-nuty socket a v loope accept-uje prave socket connect-nuty zo strany klienta)
- max. pocet takychto spojeni (sucasne, naraz) je dany parametreom backlog funkcie listen(2) na serveri, t.j. aky moze byt max. velky pocet (rada) cakajucich spojeni
- SLOVENCINA (na serveri, ale vsetky webove servery)
- pre kazde spojenie so serverom preto vzdy postupnost (na strane klienta): novy socket, connect-nut ho (pouzivajuc getaddrinfo) a volat send (resp. recv)
- getaddrinfo(3) tak isto vzdy volat pre kazdy novy socket
- povodna idea, volat na strane klienta iba raz (na zaciatku) getaddrinfo, nechat si pointer na struct addrinfo, a potom stale ten isty pointer predavat pre kazdy novy socket/connect je zla
- pouzivajuc ten isty pointer na struct addrinfo (t.j. iba raz volana getaddrinfo) pre kazdy novy socket (na klientovi) bolo vidiet ako prave po backlog*2 (parameter funkcie listen(2) na serveri, *2 asi preto ze TCP je full-duplex) socket/connect spojeniach server "dlho rozmysla"
- po zavolani close/shutdown na klientovi, funkcia recv(2) na server strane vracia nulovu dlzku, t.j. ako keby klient volal send(2) s lenght retazca rovnym 0

`/proc/net/tcp` a list of open tcp sockets<br />
`/proc/net/udp` a list of open udp sockets<br />
`/proc/net/raw` a list all the raw sockets<br />

`lsof -c my_server_program`<br />
`lsof -c my_client_program`<br />


---
* [WebService c поддержкой gzip на C](https://habr.com/ru/post/133176/)
```
// Writes given data range to socket
// Write int value to socket
```

* https://stackoverflow.com/questions/2103565/how-to-determine-compressed-size-from-zlib-for-gzipped-data
