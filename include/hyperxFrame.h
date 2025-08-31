#ifndef __HYPERXFRAME_H
#define __HYPERXFRAME_H

#include <thread>
#include <atomic>
#include <wx/wx.h>
#include <wx/taskbar.h>
#include <wx/timer.h>
#include <wx/choice.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include "SwitchCtrl.h"
#include "alpha_w.h"
#include "dialog.h"

// forward declare
class headset;
class hyperxApp;

class hyperxFrame : public wxFrame {
public:
	hyperxFrame(const wxChar *title, const wxPoint &pos, const wxSize &size,
				const wxChar *runDir, wxApp *app, bool useTray);

	void onConnect();
	void onDisconnect();

private:
	void createFrame();
	void setTaskIcon();
	void showWindow(wxTaskBarIconEvent &event);
	void showMenu(wxTaskBarIconEvent &event);

	void micSwitch(wxCommandEvent &event);
	void voiceSwitch(wxCommandEvent &event);
	void sleepChoice(wxCommandEvent &event);
	void quit(wxCommandEvent &event);
	void on_timer(wxTimerEvent &event);
	void read_loop();

	// app & tray
	wxApp *app{nullptr};
	bool useTray{false};
	bool taskAvailable{false};
	const wxChar *m_runDir{nullptr};

	wxTaskBarIcon *taskBarIcon{nullptr};
	wxMenu *taskMenu{nullptr};
	wxIcon wicon;

	// timers & threads
	wxTimer *timer{nullptr};
	std::thread t;

	// UI elements
	wxPanel *dialogPanel{nullptr};
	wxStaticBitmap *dialogLogo{nullptr};
	wxStaticText *connectedLabel{nullptr};
	wxStaticText *sleepTimerLabel{nullptr};
	wxStaticText *voicePromptLabel{nullptr};
	wxStaticText *micMonitorLabel{nullptr};
	wxStaticText *statusLabel{nullptr};
	wxChoice *sleepTimer{nullptr};
	wxSwitchCtrl *voicePrompt{nullptr};
	wxSwitchCtrl *micMonitor{nullptr};
	wxSwitchCtrl *micMute{nullptr};
	wxButton *quitButton{nullptr};
	wxButton *hideButton{nullptr};

	// cache file
	wxString m_cacheFile; // <-- added

	// headset state
	headset *m_headset{nullptr};
	connection_status status{DISCONNECTED};
	sleep_time sleep{S10};
	unsigned int battery{0};
	bool muted{false};
	bool voice{false};
	bool mic_monitor{false};
	bool running{true};
	unsigned long identifier{0};

	// choices for sleep timer
	const wxArrayString choices = {_T("10 Minutes"), _T("20 Minutes"), _T("30 Minutes")};
};

#endif // __HYPERXFRAME_H
