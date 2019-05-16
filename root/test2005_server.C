// 2005-04-12
// https://root-forum.cern.ch/t/tmonitor-ready-tsocket-not-executed/1762

// 2019-05-16 update to ROOT6

#include <TApplication.h>
#include <TGClient.h>
#include <TGFrame.h>
#include <TGButton.h>
#include <TGTextEntry.h>
#include <TSocket.h>
#include <TServerSocket.h>
#include <TMonitor.h>
#include <TMessage.h>
#include <TH2.h>
#include <TStopwatch.h>
#include <TCanvas.h>
#include <TRandom.h>
#include <RQ_OBJECT.h>

#include <iostream>

class MyServer : public TQObject
{
public:
  MyServer();
  virtual ~MyServer();

  void send(TH2F * histo);
  bool handleSocket(TSocket * sock);
  void checkman();
  void checkMonitor();

private:
  TMonitor * mon;
  TServerSocket *servs;
  TList * socks;
};


class MyDaq
{
  RQ_OBJECT("MyDaq")

  public:
  MyDaq();
  virtual ~MyDaq();

  void start();
  void stop();
  void pause();
  void ratec(double); //*SIGNAL*
  void acquire();
  void hc(TH2F * hist); //*SIGNAL*
  void setServer(MyServer * Server);

private:
  TStopwatch * time;
  bool running;
  long RateCounter;
  double currentRate;
  long EventCounter;
  bool wait;
  TH2F *fHpxpy;
  TCanvas *c1;
  MyServer * ser;
};


class test : public TGMainFrame
{
public:
  test(const TGWindow *p, int w, int h);
  virtual ~test();

  void startacq();
  void showrate(double rate);
  void DoExit();

  TGTextButton * start;
  TGTextButton * pause;
  TGTextButton * stop;
  TGTextEntry * srate;
  MyDaq * daq;
  TGMainFrame* fmain;
  MyServer * server;
  TTimer * timer;

private:
  TGHorizontalFrame * hbframe1;
  TGLayoutHints * normallo;

};


MyServer::MyServer() : TQObject()
{
  servs = new TServerSocket(9090, kTRUE);
  mon = new TMonitor;
  mon->Add(servs);
  socks = new TList();

  mon->Connect("Ready(TSocket*)","MyServer",this,"handleSocket(TSocket*)");
}

MyServer::~MyServer()
{
  delete servs;
  delete mon;
  delete socks;
}

void MyServer::checkman()
{
  TSocket * s;
  if ((s = mon->Select(20)) != (TSocket*)-1)    //20ms timeout
    {
      handleSocket(s);
    }
}

void MyServer::send(TH2F * histo)
{

  TMessage mess(kMESS_OBJECT);
  mess.WriteObject(histo);

  TList remlist;

  for(int i=0; i< socks->GetSize();++i)
    {
      TSocket * s = (TSocket *)socks->At(i);
      //                cout <<"sending stuff to "<< s->GetInetAddress().GetHostName()<<endl;
      long status = s->Send(mess);
      //                cout <<"mess length: "<<mess.Length()<<" "<<status<<endl;

      if (status == -1)
        {
          cout <<"removing socket from list"<<endl;
          remlist.Add(s);
        }
      //                cout <<"done"<<endl;
    }

  for (int i=0; i < remlist.GetSize();++i)
    {
      TSocket * rs = (TSocket *)remlist.At(i);
      cout <<"removing "<< rs->GetInetAddress().GetHostName()<<endl;
      socks->Remove(rs);
    }

}

bool MyServer::handleSocket(TSocket * sock)
{
  if (sock->IsA() == TServerSocket::Class())
    {
      // accept new connection from client
      TSocket *s = ((TServerSocket*)sock)->Accept();
      s->SetOption(kNoBlock,1);
      socks->Add(s);
      cout <<"accepted connection from "<< s->GetInetAddress().GetHostName()<<" "<<endl;
      return 1;
    }
  else
    {
      cout <<" dont know what came in, but something came in"<<endl;
    }
  return 0;
}

void MyServer::checkMonitor()
{

  // Now check whether somebody asked for attention
  TList Ready;
  Int_t rc = mon->Select(&Ready,0,1);
  if (rc > 0) {
    TIter next(&Ready);
    TSocket *s = 0;
    while ((s = (TSocket *)next()) && (s != (TSocket *)(-1))) {
      if (handleSocket(s))
        return;
    }
  }
  return;
}


MyDaq::MyDaq()
{
  time=new TStopwatch();
  running = false;
  wait=true;
  RateCounter=0;
  currentRate=0.;
  EventCounter=0;
  fHpxpy  = new TH2F("hpxpy","py vs px",60,-6,6,60,-6,6);
  fHpxpy->SetOption("colz");
  c1 = new TCanvas("fCanvas","Hsimple Server",-200,10,250,250);
  fHpxpy->Draw();
}

MyDaq::~MyDaq()
{
  delete time;
  delete fHpxpy;
  delete c1;
}

void MyDaq::hc(TH2F * hist)
{
  Long_t args[1];
  args[0] = (Long_t) hist;

  Emit("hc(TH2F*)",args);
}

void MyDaq::ratec(double r)
{
  if (r != currentRate)
    {
      Emit("ratec(double)",r);
      currentRate = r;
    }
}

void MyDaq::setServer(MyServer * Server)
{
  ser = Server;
}

void MyDaq::pause()
{
  wait=!wait;
}

void MyDaq::stop()
{
  running=false;
  cout <<"----stop exe----"<<endl;
}

void MyDaq::start()
{
  wait=false;
  time->Start(true);
  acquire();
}

void MyDaq::acquire()
{
  Float_t px, py;

  running=true;
  while(running)
    {
      while(wait)
        {
          gSystem->ProcessEvents();
        }

      ++RateCounter;
      if (RateCounter > 10)
        {
          double ti =time->RealTime();
          if (ti)
            {
              double rate = (double) EventCounter/ti;
              ratec(rate);
            }
          RateCounter = 0;
          EventCounter=0;

          if (c1->TestBit(TObject::kNotDeleted))
            {
              c1->Modified();
              c1->Update();
            }
          else
            {
              gApplication->Terminate();
            }

          ser->send(fHpxpy);
          //                    ser->checkman();

          time->Start(true);
        }

      // Now check whether somebody asked for attention
      gSystem->ProcessEvents();

      ser->checkMonitor();

      if (wait || !running)
        continue;

      gRandom->Rannor(px,py);
      fHpxpy->Fill(px,py);
      ++EventCounter;
    }
  ratec(0);
}


test::test(const TGWindow *p, int w, int h)
  :TGMainFrame(p,w,h)
{
  normallo = new TGLayoutHints(kLHintsNormal);
  srate = new TGTextEntry(this,"0");
  srate->Resize(200,srate->GetDefaultHeight());
  this->AddFrame(srate,normallo);

  hbframe1 = new TGHorizontalFrame(this,100,100);
  start = new TGTextButton(hbframe1,"start");
  hbframe1->AddFrame(start,normallo);

  pause = new TGTextButton(hbframe1,"pause");
  hbframe1->AddFrame(pause,normallo);

  stop = new TGTextButton(hbframe1,"stop");
  hbframe1->AddFrame(stop,normallo);

  this->AddFrame(hbframe1,normallo);

  daq = new MyDaq();
  showrate(0.);

  server = new MyServer();
  daq->setServer(server);

  timer= new TTimer(100);
  //    timer->Start();

  timer->Connect("Timeout()","MyServer",server,"checkman()");

  daq->Connect("ratec(double)","test",this,"showrate(double)");
  //    daq->Connect("hc(TH2F*)","MyServer",server,"send(TH2F*)");

  start->Connect("Clicked()","test",this,"startacq()");
  stop->Connect("Clicked()","MyDaq",daq,"stop()");
  pause->Connect("Clicked()","MyDaq",daq,"pause()");

  this->Connect("CloseWindow()", "test", this, "DoExit()");

  this->SetWindowName("test");
  this->MapSubwindows();
  this->Resize(this->GetDefaultSize());
  this->MapWindow();
}

test::~test()
{
  delete normallo;
  delete start;
  delete stop;
  delete pause;
  delete srate;
  delete hbframe1;
  delete daq;
  delete fmain;
  delete timer;
}

void test::startacq()
{
  daq->start();
}

void test::DoExit()
{
  gApplication->Terminate();
}

void test::showrate(double r)
{
  char buf[32];
  if (r > 1050 && r <= 1000500)
    {
      r /= 1.e3;
      sprintf(buf, "%.2lf KHz", r);
    }
  else if (r > 1000500 && r <= 1000000500)
    {
      r /= 1.e6;
      sprintf(buf, "%.2lf MHz", r);
    }
  else
    {
      sprintf(buf, "%.2lf Hz", r);
    }

  srate->SetText(buf);
}

void test2005_server()
{
  new test(gClient->GetRoot(),400,400);
}
