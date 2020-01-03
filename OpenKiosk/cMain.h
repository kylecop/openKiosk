#pragma once
#include "wx/wx.h"

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

public:
	wxButton *m_btn1 = nullptr;
	wxTextCtrl* m_txt1 = nullptr;
	wxTextCtrl* m_txt2 = nullptr;
	wxPanel* m_pnl1 = nullptr;
	//wxListBox *m_list1 = nullptr;

	void OnEnterPressed(wxCommandEvent& evt2);
	void OnButtonClicked(wxCommandEvent &evt);
	void ChangeSessionState();
	void HookKeyboard();


	wxDECLARE_EVENT_TABLE();
};

