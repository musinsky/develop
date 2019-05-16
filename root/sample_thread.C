// 2008-08-19
// https://root-forum.cern.ch/t/using-a-tthread-to-call-a-method/6999

#include <TROOT.h>
#include <TGButton.h>
#include <TGFrame.h>
#include <TTimer.h>
#include <TThread.h>
#include <Riostream.h>

class MyMainFrame : public TGMainFrame {

private:
  TTimer *timer;
  TThread *thread;

public:
  MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h);
  virtual ~MyMainFrame();
  void DoDraw();
  void EnableTimer();
  void DisableTimer();
  static void *PrintStuff(void *arg);

  ClassDef(MyMainFrame, 0) // My Main Frame
};

MyMainFrame::MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h) :
  TGMainFrame(p,w,h), timer(0), thread(0)
{
  TGTextButton *start = new TGTextButton(this,"&Start ");
  start->Connect("Clicked()","MyMainFrame",this,"EnableTimer()");

  TGTextButton *stop = new TGTextButton(this,"&Stop ");
  stop->Connect("Clicked()","MyMainFrame",this,"DisableTimer()");

  AddFrame(start, new TGLayoutHints(kLHintsCenterY,5,5,3,4));
  AddFrame(stop, new TGLayoutHints(kLHintsCenterY,5,5,3,4));

  thread = new TThread("thread", PrintStuff, (void*) 0);

  MapSubwindows();
  Layout();
  MapWindow();
}

void MyMainFrame::DoDraw()
{
  thread->Run();
}

void MyMainFrame::EnableTimer()
{
  if (!timer) {
    timer = new TTimer(this, 500, kTRUE); //synchronous or asynch?
    timer->Connect("Timeout()", "MyMainFrame", this, "DoDraw()");
    timer->Reset();
    timer->TurnOn();
  }
  else
    timer->TurnOn();
}

void MyMainFrame::DisableTimer()
{
  if (timer)
    timer->TurnOff();
  if (thread && (thread->GetState() == TThread::kRunningState)) {
    thread->Kill();
  }
}

void *MyMainFrame::PrintStuff(void *arg)
{
  cout << "Did it work?" << endl;
  return 0;
}

MyMainFrame::~MyMainFrame() {
  Cleanup();
  if (thread)
    delete thread;
  if (timer) {
    timer->TurnOff();
    delete timer;
  }
}

void sample_thread()
{
  // Popup the GUI
  new MyMainFrame(gClient->GetRoot(),600,400);
}
