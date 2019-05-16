// 2005-08-11
// https://root-forum.cern.ch/t/tthread/2288

#include "TMessage.h"
#include "TSocket.h"

#include <iostream>

void ganis_Clnt(Int_t port = 3000)
{
  TSocket *s = new TSocket("localhost", port);
  if (s->IsValid()) {
    TMessage *msg;
    s->Recv(msg);

    TString ss;
    (*msg) >> ss;
    cout << "Got: " << ss.Data() << endl;

    delete msg;
  }

  if (s) delete s;
}
