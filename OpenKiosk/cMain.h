#pragma once
#include "wx/wx.h"

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

public:
	int nFieldWidth = 10;
	int nFieldHeight = 10;
	wxButton** btn;
	int* nField = nullptr; // array of integers knowing whether a mine exists there or not.
	bool bFirstClick = true; // know if its the first click so the player doesnt loose immediately (this is when we will populate the mine field)

	void OnButtonClicked(wxCommandEvent &evt);


	wxDECLARE_EVENT_TABLE();
};

