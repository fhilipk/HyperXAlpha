#ifndef HYPERXAPP_H
#define HYPERXAPP_H

#include "hyperxFrame.h"
#include <wx/wx.h>

class hyperxApp : public wxApp {
public:
  hyperxApp();          // original constructor
  hyperxApp(bool);      // upstream constructor
  ~hyperxApp();

  virtual bool OnInit();

private:
  hyperxFrame *m_frame;
  bool systray;         // upstream addition
};

#endif
