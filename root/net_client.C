// 2007-09-04
// https://root-forum.cern.ch/t/socket-communication/5251

#include "TMonitor.h"
#include "TSocket.h"

char buf[256];

void net_client(const char *host = "localhost", Int_t port = 3000)
{
  // Accept for connections on 'port'
  Printf("Connecting to server at %s:%d", host, port);
  TSocket *s = new TSocket(host, port);

  Int_t len, typ;
  Int_t nr = 10;
  while (nr--) {
    s->Recv(len, typ);
    s->RecvRaw(buf, len);
    buf[len] = 0;
    Printf("Received from server %s (%d)", buf, nr);
  }
  // Send something to stop
  s->Send(len);
  delete s;
  // Done
  return;
}
