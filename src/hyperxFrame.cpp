#include "hyperxFrame.h"
#include <wx/msgdlg.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/stdpaths.h>
#include <iostream>
#include <filesystem>

hyperxFrame::hyperxFrame(const wxChar *title,
						 const wxPoint &pos,
						 const wxSize &size,
						 const wxChar *runDir,
						 wxApp *appPtr,
						 bool useTrayFlag)
: wxFrame(nullptr, wxID_ANY, title, pos, size),
app(appPtr),
useTray(useTrayFlag),
m_runDir(runDir)
{
	createFrame();

	dialogLogo = new wxStaticBitmap(dialogPanel, wxID_ANY,
									wxBitmap(wxImage(wxString(m_runDir) + _T("img/poweredOff.png"), wxBITMAP_TYPE_PNG)),
									wxDefaultPosition,
								 wxSize(size.GetWidth(), size.GetHeight() - 50));

	timer = new wxTimer(this);
	timer->Start(1000);

	// Set cache path to writable location
	wxString cacheDir = wxStandardPaths::Get().GetUserDataDir() + _T("/hyperxalpha_cache");
	if (!std::filesystem::exists(cacheDir.ToStdString())) {
		std::filesystem::create_directories(cacheDir.ToStdString());
	}
	m_cacheFile = cacheDir + _T("/cache.dat");

	t = std::thread(&hyperxFrame::read_loop, this);
}

void hyperxFrame::setTaskIcon() {
	if (!taskAvailable) return;

	if (battery <= 10)
		wicon = wxIcon(wxString(m_runDir) + _T("img/tray0.png"));
	else if (battery <= 30)
		wicon = wxIcon(wxString(m_runDir) + _T("img/tray20.png"));
	else if (battery <= 50)
		wicon = wxIcon(wxString(m_runDir) + _T("img/tray40.png"));
	else if (battery <= 70)
		wicon = wxIcon(wxString(m_runDir) + _T("img/tray60.png"));
	else if (battery <= 90)
		wicon = wxIcon(wxString(m_runDir) + _T("img/tray80.png"));
	else
		wicon = wxIcon(wxString(m_runDir) + _T("img/tray100.png"));

	if (taskBarIcon)
		taskBarIcon->SetIcon(wicon);
}

void hyperxFrame::createFrame() {
	dialogPanel = new wxPanel(this, wxID_ANY);

	wxBitmapBundle logoImg(wxImage(wxString(m_runDir) + _T("img/hyperx.png"), wxBITMAP_TYPE_PNG));

	// Setup other UI elements (buttons, switches, sizers)
}

void hyperxFrame::read_loop() {
	unsigned char buffer[8];
	while (running) {
		// read headset buffer (omitted)
		switch (buffer[1]) {
			case 0x23:
				muted = (buffer[3] == 0x01);
				if (micMute) micMute->SetValue(muted);
				break;
			default: break;
		}
	}
}
