#include "hyperxFrame.h"
#include <wx/msgdlg.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/stdpaths.h>
#include <filesystem>
#include <thread>
#include <iostream>

hyperxFrame::hyperxFrame(const wxChar *title,
						 const wxPoint &pos,
						 const wxSize &size,
						 const wxChar *runDir,
						 wxApp *appPtr,
						 bool useTrayFlag)
: wxFrame(nullptr, wxID_ANY, title, pos, size),
app(appPtr),
useTray(useTrayFlag),
m_runDir(runDir),
running(true)
{
	createFrame();

	// Use wxStandardPaths for images
	wxString imgPath = wxStandardPaths::Get().GetResourcesDir() + _T("/img/poweredOff.png");
	if (!std::filesystem::exists(imgPath.ToStdString())) {
		wxMessageBox("Missing image: poweredOff.png", "Error", wxICON_ERROR);
	}
	dialogLogo = new wxStaticBitmap(dialogPanel, wxID_ANY,
									wxBitmap(wxImage(imgPath, wxBITMAP_TYPE_PNG)),
									wxDefaultPosition,
								 wxSize(size.GetWidth(), size.GetHeight() - 50));

	timer = new wxTimer(this);
	timer->Start(1000);

	// Cache directory
	wxString cacheDir = wxStandardPaths::Get().GetUserDataDir() + _T("/hyperxalpha_cache");
	if (!std::filesystem::exists(cacheDir.ToStdString())) {
		std::filesystem::create_directories(cacheDir.ToStdString());
	}
	m_cacheFile = cacheDir + _T("/cache.dat");

	// Start headset read loop
	t = std::thread(&hyperxFrame::read_loop, this);
}

void hyperxFrame::setTaskIcon() {
	if (!taskAvailable) return;

	wxString trayImg;
	if (battery <= 10) trayImg = "tray0.png";
	else if (battery <= 30) trayImg = "tray20.png";
	else if (battery <= 50) trayImg = "tray40.png";
	else if (battery <= 70) trayImg = "tray60.png";
	else if (battery <= 90) trayImg = "tray80.png";
	else trayImg = "tray100.png";

	wxString imgPath = wxStandardPaths::Get().GetResourcesDir() + _T("/img/") + trayImg;
	if (taskBarIcon) {
		taskBarIcon->SetIcon(wxIcon(imgPath));
	}
}

void hyperxFrame::createFrame() {
	dialogPanel = new wxPanel(this, wxID_ANY);

	wxString logoPath = wxStandardPaths::Get().GetResourcesDir() + _T("/img/hyperx.png");
	if (!std::filesystem::exists(logoPath.ToStdString())) {
		wxMessageBox("Missing image: hyperx.png", "Error", wxICON_ERROR);
	}
	wxBitmapBundle logoImg(wxImage(logoPath, wxBITMAP_TYPE_PNG));

	// TODO: Add buttons, switches, sizers, etc.
}

void hyperxFrame::read_loop() {
	unsigned char buffer[8];
	while (running) {
		// Read headset buffer
		// Example logic:
		switch (buffer[1]) {
			case 0x23:
				muted = (buffer[3] == 0x01);
				if (micMute) micMute->SetValue(muted);
				break;
			default:
				break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
