#include "hyperxApp.h"
#include "hyperxFrame.h"

#include <wx/image.h>
#include <wx/filename.h>
#include <iostream>
#include <limits.h>
#include <stdlib.h>

hyperxApp::hyperxApp() : m_frame(nullptr), systray(false) {}

bool hyperxApp::OnInit() {
  // Add PNG support
  wxImage::AddHandler(new wxPNGHandler);

  // Resolve executable path
  char resolved_path[PATH_MAX];
  if (!realpath(argv[0], resolved_path)) {
    std::cerr << "Error resolving path" << std::endl;
    return false;
  }
  wxString exePath(resolved_path);

  // Strip trailing part if needed (matches original logic)
  if (exePath.length() > 6) {
    exePath.erase(exePath.end() - 6, exePath.end());
  }

  // Create the main frame
  try {
    m_frame = new hyperxFrame(
      _T("HyperX Alpha"),    // window title
                              wxDefaultPosition,     // default position
                              wxSize(400, 400),      // default size
                              exePath,               // path to resources
                              this,                  // pointer to app
                              systray                // system tray flag
    );
  } catch (std::exception &e) {
    std::cout << "Exception: " << e.what() << std::endl;
    return false;
  }

  SetTopWindow(m_frame);
  return true;
}

// wxWidgets will automatically generate main()
wxIMPLEMENT_APP(hyperxApp);
