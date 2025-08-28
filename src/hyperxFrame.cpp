#include "hyperxFrame.h"
#include "dialog.h"
#include "hyperxApp.h"
#include <iomanip>
#include <iostream>

// Constructor
hyperxFrame::hyperxFrame(const wxChar *title, const wxPoint &pos,
						 const wxSize &size, const wxChar *runDir, wxApp *app,
						 bool useTray)
: wxFrame(nullptr, wxID_ANY, title, pos, size), m_headset(new headset),
m_runDir(runDir), running(true), app(app), useTray(useTray) {

	if (!m_headset->init()) {
		dialog *error = new dialog(_T("HyperX Cloud Alpha Unavailable"),
								   wxDefaultPosition, wxSize(440, 150),
								   m_runDir + _T("img/poweredOff.png"));
		delete m_headset;
		throw std::runtime_error("Failed to initialize headset");
	}

	t = std::thread(&hyperxFrame::read_loop, this);

	// headset polling
	timer = new wxTimer();
	timer->Bind(wxEVT_TIMER, &hyperxFrame::on_timer, this);

	if (wxTaskBarIcon::IsAvailable() && useTray) {
		taskAvailable = true;
		taskBarIcon = new wxTaskBarIcon();
		taskBarIcon->Bind(wxEVT_TASKBAR_RIGHT_DOWN, &hyperxFrame::showMenu, this);
		taskBarIcon->Bind(wxEVT_TASKBAR_LEFT_DOWN, &hyperxFrame::showWindow, this);
		setTaskIcon();
	}

	createFrame();

	m_headset->send_command(commands::CONNECTION_STATE);
}

// Timer event for periodic status update
void hyperxFrame::on_timer(wxTimerEvent &event) {
	m_headset->send_command(commands::STATUS_REQUEST);
	m_headset->send_command(commands::PING);
}

// Left click taskbar
void hyperxFrame::showWindow(wxTaskBarIconEvent &event) {
	(IsShown()) ? this->Hide() : this->Show();
}

// Right-click menu
void hyperxFrame::showMenu(wxTaskBarIconEvent &event) {
	enum { hOPEN = 2525, hQUIT };
	taskMenu = new wxMenu();
	taskMenu->Append(hOPEN, (IsShown()) ? _T("Hide") : _T("Show"), _T(""), wxITEM_NORMAL);
	taskMenu->AppendSeparator();
	taskMenu->Append(hQUIT, _T("Quit"), _T(""), wxITEM_NORMAL);
	taskMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, [this](wxCommandEvent &event) {
		switch (event.GetId()) {
			case hQUIT:
				this->quit(event);
				break;
			case hOPEN:
				(IsShown()) ? this->Hide() : this->Show();
				break;
			default:;
		}
	});
	taskBarIcon->PopupMenu(taskMenu);
}

// Mic and voice control handlers
void hyperxFrame::micSwitch(wxCommandEvent &event) {
	(micMonitor->GetValue())
	? m_headset->send_command(commands::MICROPHONE_MONITOR)
	: m_headset->send_command(commands::MICROPHONE_MONITOR_OFF);
}

void hyperxFrame::voiceSwitch(wxCommandEvent &event) {
	(voicePrompt->GetValue())
	? m_headset->send_command(commands::VOICE_PROMPTS)
	: m_headset->send_command(commands::VOICE_PROMPTS_OFF);
}

// Quit application
void hyperxFrame::quit(wxCommandEvent &event) {
	running = false;
	m_headset->send_command(commands::PING);
	if (timer->IsRunning()) timer->Stop();
	t.join();
	if (taskAvailable) {
		taskBarIcon->RemoveIcon();
		delete taskBarIcon;
	}
	delete m_headset;
	this->Destroy();
}

// Sleep timer choice handler
void hyperxFrame::sleepChoice(wxCommandEvent &event) {
	switch (sleepTimer->GetSelection()) {
		case 0: m_headset->send_command(commands::SLEEP_TIMER_10); break;
		case 1: m_headset->send_command(commands::SLEEP_TIMER_20); break;
		case 2: m_headset->send_command(commands::SLEEP_TIMER_30); break;
		default: break;
	}
}

// Update taskbar icon
void hyperxFrame::setTaskIcon() {
	if (taskAvailable) {
		if (status == connection_status::CONNECTED) {
			switch (battery) {
				case 0 ... 10:   wicon = wxIcon(wxIconLocation(m_runDir + _T("img/tray0.png"))); break;
				case 11 ... 30:  wicon = wxIcon(wxIconLocation(m_runDir + _T("img/tray20.png"))); break;
				case 31 ... 50:  wicon = wxIcon(wxIconLocation(m_runDir + _T("img/tray40.png"))); break;
				case 51 ... 70:  wicon = wxIcon(wxIconLocation(m_runDir + _T("img/tray60.png"))); break;
				case 71 ... 90:  wicon = wxIcon(wxIconLocation(m_runDir + _T("img/tray80.png"))); break;
				case 91 ... 100: wicon = wxIcon(wxIconLocation(m_runDir + _T("img/tray100.png"))); break;
			}
			taskBarIcon->SetIcon(wicon, std::to_string(battery * 3) + " Hours Remaining (" + std::to_string(battery) + "%)");
		} else {
			wicon = wxIcon(wxIconLocation(m_runDir + _T("img/traydc.png")));
			taskBarIcon->SetIcon(wicon, "Power Off");
		}
	}
}

// Create main GUI frame
void hyperxFrame::createFrame() {
	this->Bind(wxEVT_CLOSE_WINDOW, [this](wxCloseEvent &event) {
		if (taskAvailable) {
			this->Hide();
		} else {
			wxCommandEvent quitEvent;
			this->quit(quitEvent);
		}
	});
	this->Bind(wxEVT_ICONIZE, [this](wxIconizeEvent &event) { this->Hide(); });

	const auto margin = FromDIP(4);
	auto mainSizer = new wxBoxSizer(wxHORIZONTAL);
	wxPanel *panel = new wxPanel(this, wxID_ANY);
	auto sizer = new wxBoxSizer(wxVERTICAL);

	// Logo
	wxBitmapBundle logoImg(wxImage(m_runDir + "img/hyperx.png", wxBITMAP_TYPE_PNG));
	auto logo = new wxStaticBitmap(panel, wxID_ANY, logoImg, wxDefaultPosition, wxSize(200, 70));
	connectedLabel = new wxStaticText(panel, wxID_ANY, _T("Connected"));

	// Features box
	auto featureBox = new wxStaticBoxSizer(wxVERTICAL, panel, _T("Features"));
	sleepTimerLabel = new wxStaticText(panel, wxID_ANY, _T("Sleep Timer"));
	sleepTimer = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices);
	sleepTimer->SetSelection(0);
	sleepTimer->SetToolTip(_T("Set Sleep Timer"));
	sleepTimer->Bind(wxEVT_CHOICE, &hyperxFrame::sleepChoice, this);
	sleepTimer->Disable();

	voicePromptLabel = new wxStaticText(panel, wxID_ANY, _T("Voice Prompt"));
	voicePrompt = new wxSwitchCtrl(panel, wxID_ANY, false);
	voicePrompt->SetToolTip(_T("Enable Voice Prompt"));
	voicePrompt->Bind(wxEVT_SWITCH, &hyperxFrame::voiceSwitch, this);
	voicePrompt->Disable();

	micMonitorLabel = new wxStaticText(panel, wxID_ANY, _T("Mic Monitor"));
	micMonitor = new wxSwitchCtrl(panel, wxID_ANY, false);
	micMonitor->SetToolTip(_T("Enable Mic Monitor"));
	micMonitor->Bind(wxEVT_SWITCH, &hyperxFrame::micSwitch, this);
	micMonitor->Disable();

	featureBox->Add(sleepTimerLabel, 0, wxEXPAND | wxALL, margin);
	featureBox->Add(sleepTimer, 0, wxEXPAND | wxALL, margin);
	featureBox->Add(voicePromptLabel, 0, wxALIGN_RIGHT | wxALL, margin);
	featureBox->Add(voicePrompt, 0, wxALIGN_RIGHT | wxALL, margin);
	featureBox->Add(micMonitorLabel, 0, wxALIGN_RIGHT | wxALL, margin);
	featureBox->Add(micMonitor, 0, wxALIGN_RIGHT | wxALL, margin);

	// Buttons
	auto buttonBox = new wxBoxSizer(wxVERTICAL);
	quitButton = new wxButton(panel, wxID_EXIT, _T("Quit"));
	quitButton->Bind(wxEVT_BUTTON, &hyperxFrame::quit, this);

	buttonBox->Add(featureBox, 0, wxEXPAND | wxALL, margin);
	buttonBox->Add(quitButton, 0, wxEXPAND | wxALL, margin);
	if (taskAvailable) {
		hideButton = new wxButton(panel, wxID_ANY, _T("Minimize"));
		hideButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event) { this->Hide(); });
		buttonBox->Add(hideButton, 0, wxEXPAND | wxALL, margin);
	}

	sizer->Add(logo, 0, wxLEFT | wxRIGHT, margin);
	sizer->Add(connectedLabel, 0, wxLEFT | wxRIGHT, margin);
	sizer->Add(buttonBox, 0, wxEXPAND | wxALL, margin);

	panel->SetSizer(sizer);
	mainSizer->Add(panel, 1, wxALL, 8);
	this->SetSizerAndFit(mainSizer);

	if (!taskAvailable) this->Show();
}

// On headset connect
void hyperxFrame::onConnect() {
	m_headset->send_command(commands::STATUS_REQUEST);
	m_headset->send_command(commands::SLEEP_STATE);
	status = connection_status::CONNECTED;
	connectedLabel->SetLabel(_T("Connected"));
	sleepTimer->Enable();
	voicePrompt->Enable();
	micMonitor->Enable();
	setTaskIcon();
	timer->Start(30000);
}

// On headset disconnect
void hyperxFrame::onDisconnect() {
	status = connection_status::DISCONNECTED;
	connectedLabel->SetLabel(_T("Disconnected"));
	sleepTimer->Disable();
	voicePrompt->Disable();
	micMonitor->Disable();
	setTaskIcon();
	timer->Stop();
}

// Background read loop
void hyperxFrame::read_loop() {
	unsigned char buffer[32];
	while (running) {
		m_headset->read(buffer);
		app->CallAfter([this, &buffer]() {
			if (buffer[0] == 0x21 && buffer[1] == 0xbb) {
				switch (buffer[2]) {
					case 0x03:
						(buffer[3] == 0x01) ? onDisconnect() : onConnect();
						break;
					case 0x05: break;
					case 0x07: // Sleep state
						switch (buffer[3]) {
							case 0x0a: sleep = sleep_time::S10; sleepTimer->SetSelection(0); break;
							case 0x14: sleep = sleep_time::S20; sleepTimer->SetSelection(1); break;
							case 0x1e: sleep = sleep_time::S30; sleepTimer->SetSelection(2); break;
						}
						break;
							case 0x09: // Voice prompts
								voice = (buffer[3] == 0x01);
								voicePrompt->SetValue(voice);
								break;
							case 0x0a: break;
							case 0x0b: // Battery
								battery = (unsigned int)buffer[3];
								setTaskIcon();
								break;
							case 0x0c: break;
							case 0x0d: // Identifier
								identifier = (unsigned long)buffer[3] << 40 | (unsigned long)buffer[4] << 32 |
								(unsigned long)buffer[5] << 24 | (unsigned long)buffer[6] << 16 |
								(unsigned long)buffer[7] << 8  | (unsigned long)buffer[8];
								break;
							case 0x12: break; // Sleep set response
							case 0x13: voice = (buffer[3] == 0x01); break;
							case 0x22: mic_monitor = (buffer[3] == 0x01); micMonitor->SetValue(mic_monitor); break;
							case 0x23: muted = (buffer[3] == 0x01); micMute->SetValue(muted); break;
							case 0x24: (buffer[3] == 0x01) ? onDisconnect() : onConnect(); break;
				}
			}
		});
	}
}
