#include "hyperxApp.h"
#include <wx/image.h>
#include <iostream>
#include <limits.h>
#include <stdlib.h>

hyperxApp::hyperxApp() : m_frame(nullptr), systray(false) {}

bool hyperxApp::OnInit() {
  wxImage::AddHandler(new wxPNGHandler);

  char resolved_path[PATH_MAX];
  if (!realpath(argv[0], resolved_path)) {
    std::cerr << "Error resolving path" << std::endl;
    return false;
  }
  wxString c(resolved_path);
  if (c.length() > 6) {
    c.erase(c.end() - 6, c.end());
  }

  try {
    m_frame = new hyperxFrame(
      _T("HyperX Alpha"),
                              wxDefaultPosition,
                              wxSize(400, 400),
                              c,
                              this,
                              systray
    );
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    return false;
  }

  SetTopWindow(m_frame);
  return true;
}

// wxWidgets app declaration
wxIMPLEMENT_APP(hyperxApp);
