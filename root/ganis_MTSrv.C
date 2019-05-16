// 2005-08-11
// https://root-forum.cern.ch/t/tthread/2288

#include "TError.h"
#include "TMessage.h"
#include "TServerSocket.h"
#include "TSystem.h"
#include "TThread.h"

#include <iostream>

typedef struct {
  Int_t    nseq;
  TSocket *sock;
} thrdarg_t;

//
// Function to be run in the thread
//___________________________________________________
void StartupThread(void *args)
{

  thrdarg_t *arg = (thrdarg_t *)args;
  Int_t nseq = arg->nseq;
  TSocket *s = arg->sock;

  TMessage m;
  m << TString(Form("I am # %d",nseq));
  s->Send(m);

  cout << gSystem->GetPid()<< ": thread "<< (Int_t *)TThread::Self()
       <<": sending: I am # "<<nseq<< endl;
  delete arg;
  delete s;
  delete TThread::Self();
};

//
// Server class
//
class MTSrv {
  TServerSocket *fSSock;

public:
  MTSrv(Int_t port = 3000);
  virtual ~MTSrv() { if (fSSock) delete fSSock; }
};

//_________________________________________________________
MTSrv::MTSrv(Int_t port)
{
  // Constructor: starts the server
  Int_t nsrv = 0;

  fSSock = new TServerSocket(port);
  if (fSSock->IsValid()) {
    Info("MTSrv","waiting on port %d ...", port);
    while (1) {
      TSocket *s = fSSock->Accept();
      if (s->IsValid()) {
        nsrv++;
        thrdarg_t *args = new thrdarg_t;
        args->nseq = nsrv;
        args->sock = s;
        TThread *t = new TThread((TThread::VoidFunc_t)StartupThread, (void *)args);
        t->Run();
      }
    }
  } else {
    Info("MTSrv","could create the server socket");
  }
}

// add 2019-05-16
void ganis_MTSrv(Int_t port = 3000)
{
  MTSrv srv(port);
}
