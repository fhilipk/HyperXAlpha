#include "hyperxApp.h"
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc == 1 || std::string(argv[1]) == "--systray") {
    wxApp *pApp = new hyperxApp();
    hyperxApp::SetInstance(pApp);
    wxEntry(argc, argv);
    wxEntryCleanup();
  } else {
    std::cout << "HyperX Alpha Help" << std::endl;
    std::cout << "  Use --systray to start GUI with legacy systray support!" << std::endl;
  }
  return 0;
}
