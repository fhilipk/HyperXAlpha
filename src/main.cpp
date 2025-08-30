#include "hyperxApp.h"
#include <wx/wx.h>

int main(int argc, char **argv) {
  wxDISABLE_DEBUG_SUPPORT();  // avoids weird asserts in release builds

  // Initialize wxWidgets
  if ( !wxEntryStart(argc, argv) )
    return -1;

  // Create and hook up the app
  hyperxApp *app = new hyperxApp();
  wxTheApp = app;

  if ( !app->OnInit() )
    return -1;

  // Run the wxWidgets event loop
  int ret = wxTheApp->OnRun();

  // Cleanup
  app->OnExit();
  wxEntryCleanup();

  return ret;
}
