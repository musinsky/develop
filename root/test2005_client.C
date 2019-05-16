// 2005-04-07
// https://root-forum.cern.ch/t/tmonitor-ready-tsocket-not-executed/1762

// 2019-05-16 update to ROOT6

#include <TApplication.h>
#include <TGClient.h>
#include <TGFrame.h>
#include <TGButton.h>
#include <TGTextEntry.h>
#include <TSocket.h>
#include <TMonitor.h>
#include <TMessage.h>
#include <TH2.h>
#include <TCanvas.h>
#include <RQ_OBJECT.h>

#include <iostream>

class MyClient
{
  RQ_OBJECT("MyClient")

  public:
  MyClient();
  virtual ~MyClient();

  void recv(TSocket * sock);
  void received(TMessage * mess);       //*SIGNAL*
  void checkman();

private:
  TMonitor * mon;
};


class testshow
{
public:
  testshow();
  virtual ~testshow();

  void showMessage(TMessage * Mess);

private:
  TH2F * fHpxpy;
  TCanvas * c1;
  MyClient * client;

};


MyClient::MyClient()
{
  mon = new TMonitor();
  TSocket * s = new TSocket("localhost", 9090);

  cout <<"connected to: "<< s->GetInetAddress().GetHostName()<<" "<<endl;


  mon->Add(s);
  mon->Connect("Ready(TSocket*)","MyClient",this,"recv(TSocket*)");

}

MyClient::~MyClient()
{
  delete mon;
}

void MyClient::received(TMessage * Mess)
{
  Long_t args[1];
  args[0] = (Long_t) Mess;

  Emit("received(TMessage*)",args);
}

void MyClient::checkman()
{
  TSocket * s;
  if ((s = mon->Select(20)) != (TSocket*)-1)    //20ms timeout
    {
      recv(s);
    }
}

void MyClient::recv(TSocket * s)
{
  TMessage *mess;
  if (s->Recv(mess) <= 0)
    {
      return;
    }

  received(mess);
  delete mess;
}


testshow::testshow()
{
  fHpxpy  = 0;
  c1 = new TCanvas("fCanvas","Hsimple Client",-200,200,500,500);
  client = new MyClient();

  client->Connect("received(TMessage*)","testshow",this,"showMessage(TMessage*)");
}

testshow::~testshow()
{
  delete c1;
  delete fHpxpy;
}

void testshow::showMessage(TMessage * mess)
{
  if (fHpxpy) delete fHpxpy;
  if (mess->GetClass()->InheritsFrom(TH1::Class()))
    {
      fHpxpy = (TH2F *) mess->ReadObject(mess->GetClass());

      if (!c1->TestBit(TObject::kNotDeleted))
        {
          gApplication->Terminate();
        }
      fHpxpy->Draw();
      c1->Modified();
      c1->Update();
    }

}


void test2005_client()
{
  new testshow();
}
