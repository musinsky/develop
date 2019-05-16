// 2007-09-04
// https://root-forum.cern.ch/t/socket-communication/5251

#include "TList.h"
#include "TMonitor.h"
#include "TServerSocket.h"

const char *buf = "ASimpleBuffer";

void net_server(Int_t port = 3000)
{
  // Accept for connections on 'port'
  Printf("Starting a server socket on port %d", port);
  TServerSocket *ss = new TServerSocket(port);

  // Wait for the client
  TSocket *s = ss->Accept();

  // Monitor this socket in read and write
  TMonitor *m = new TMonitor();
  m->Add(s, TMonitor::kRead|TMonitor::kWrite);
  m->Activate(s);

  // Now Loop sending over info until the client sends a stop signal
  Int_t len, typ = 1;
  TList rd, wr;
  Bool_t done = kFALSE;
  while (!done) {
    len = strlen(buf);
    // Wait for the socket being ready
    Int_t nr = m->Select(&rd, &wr, -1);
    // If there is something to read from the client we are done
    if (nr > 0 && rd.GetSize() > 0 && s == (TSocket *)rd.First()) {
      Printf("Got stop signal form the client");
      done = kTRUE;
    } else if (wr.GetSize() > 0 && s == (TSocket *)wr.First()) {
      // Send the length
      s->Send(len, typ);
    }
    if (!done) {
      // Wait for the socket being ready
      Int_t nr = m->Select(&rd, &wr, -1);
      // If there is something to read from the client we are done
      if (nr > 0 && rd.GetSize() > 0 && s == (TSocket *)rd.First()) {
        Printf("Got stop signal form the client");
        done = kTRUE;
      } else if (wr.GetSize() > 0 && s == (TSocket *)wr.First()) {
        s->SendRaw((const void *)buf, len);
      }
    }
  }

  // Cleanup
  m->DeActivate(s);
  delete m;
  delete s;
  delete ss;
  // Done
  return;
}
