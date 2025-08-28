#ifndef __HYPERXFRAME_H
#define __HYPERXFRAME_H

#include <atomic>
#include <thread>
#include <wx/taskbar.h>
#include <wx/wx.h>

#include "SwitchCtrl.h"
#include "alpha_w.h"

// main window
class hyperxFrame : public wxFrame {
public:
	// merge your fork constructor with upstream additions
	hyperxFrame(const wxChar *title, const wxPoint &pos, const wxSize &size,
				const wxChar *runDir, wxApp *app = nullptr, bool useTray = false);

private:
	// optional app pointer and tray flag from upstream
	wxApp *app;
	bool useTray = false;

	// Main layout
	wxTaskBarIcon *taskBarIcon;
	bool taskAvailable = false;
	wxMenu *taskMenu;
	wxIcon wicon;
	wxButton *quitButton;
	wxButton *hideButton;
	wxString m_runDir;
	wxStaticText *statusLabel;      // fork
	wxStaticText *connectedLabel;   // upstream

	// features box
	wxStaticText *sleepTimerLabel;
	wxChoice *sleepTimer;
	wxStaticText *voicePromptLabel;
	wxSwitchCtrl *voicePrompt;
	wxStaticText *micMonitorLabel;
	wxSwitchCtrl *micMonitor;

	// headset data
	headset *m_headset;
	sleep_time sleep;
	connection_status status;
	unsigned int battery;
	bool micMuted;
	bool muted;
	bool voice;
	bool mic_monitor;
	unsigned long identifier;
	const wxArrayString choices = {_T("10 Minutes"), _T("20 Minutes"),
		_T("30 Minutes"), _T("Never")};

		// callback functions for controls
		void createFrame();
		void setTaskIcon();
		void onConnect();
		void onDisconnect();           // upstream
		void showWindow(wxTaskBarIconEvent &event);
		void showMenu(wxTaskBarIconEvent &event);
		void on_micMute(wxCommandEvent &event);
		void on_micVolume(wxCommandEvent &event);
		void on_mute(wxCommandEvent &event);
		void on_volume(wxCommandEvent &event);
		void sleepChoice(wxCommandEvent &event);
		void voiceSwitch(wxCommandEvent &event); // hide button
		void micSwitch(wxCommandEvent &event);   // hide button
		void quit(wxCommandEvent &event);        // quit button

		// timer Event 5 seconds
		wxTimer *dialogTimer;          // fork
		wxTimer *timer;
		void on_timer(wxTimerEvent &event);

		// read loop for headset
		bool wanted;                   // fork
		std::atomic<bool> running;
		std::thread t;
		std::thread pt;                // fork
		void read_loop();
};

#endif
