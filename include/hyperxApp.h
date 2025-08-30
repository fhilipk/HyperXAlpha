#ifndef HYPERXAPP_H
#define HYPERXAPP_H

#include "hyperxFrame.h"
#include <wx/wx.h>

// Application entry point
class hyperxApp : public wxApp {
public:
  hyperxApp();
  ~hyperxApp() override = default;

  virtual bool OnInit() override;

private:
  hyperxFrame *m_frame{nullptr};
  bool systray{false};
};

#endif // HYPERXAPP_H
