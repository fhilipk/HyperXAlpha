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

	// headset state
	headset *m_headset{nullptr};
	connection_status status{DISCONNECTED};
	sleep_time sleep{S10};
	unsigned int battery{0};
	bool muted{false};
