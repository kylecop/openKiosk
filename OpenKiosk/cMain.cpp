#include "cMain.h"

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_BUTTON(10001, OnButtonClicked)

wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "The Title", wxPoint(30,30), wxSize(800,600))
{
	btn = new wxButton * [nFieldWidth * nFieldHeight];
	wxGridSizer* grid = new wxGridSizer(nFieldWidth, nFieldHeight, 0, 0);

	nField = new int[nFieldWidth * nFieldHeight]; // create the array of mines

	// change the font size
	wxFont font(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);


	for (int x = 0; x < nFieldWidth; x++)
		for (int y = 0; y < nFieldHeight; y++)
		{
			btn[y * nFieldWidth + x] = new wxButton(this, 10000 + (y * nFieldWidth + x));
			btn[y * nFieldWidth + x]->SetFont(font);
			grid->Add(btn[y * nFieldWidth + x], 1, wxEXPAND | wxALL);


			btn[y * nFieldWidth + x]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnButtonClicked, this);// if the button is clicked, bind it to the onbuttonclicked event
			nField[y * nFieldWidth + x] = 0; //set the default value for each location
		}

	this->SetSizer(grid);
	grid->Layout();
}




cMain::~cMain()
{
	delete[]btn;
}

void cMain::OnButtonClicked(wxCommandEvent& evt)
{
	// get coordinate of button in field array
	int x = (evt.GetId() - 10000) % nFieldWidth;
	int y = (evt.GetId() - 10000) / nFieldWidth;

	if (bFirstClick)
	{
		int mines = 30;

		//randomly generate mines in the minefield
		while (mines)
		{
			int rx = rand() % nFieldWidth;
			int ry = rand() % nFieldHeight;

			//if this location is valid, put a mine there
			if (nField[ry * nFieldWidth + rx] == 0 && rx != x && ry != y)
			{
				nField[ry * nFieldWidth + rx] = -1; // -1 indicates the presence of a mine
				mines--;
			}
		}
		bFirstClick = false; //because it has been clicked now
	}

	// disable this button from being pressed again
	btn[y * nFieldWidth + x]->Enable(false);

	// check if player hit mine
	if (nField[y * nFieldWidth + x] == -1)
	{
		wxMessageBox("BOOOOOOM !! - Game Over :(");

		// reset game
		bFirstClick = true;
		for (int x = 0; x < nFieldWidth; x++)
			for (int y = 0; y < nFieldHeight; y++)
			{
				nField[y * nFieldWidth + x] = 0;
				btn[y * nFieldWidth + x]->SetLabel("");
				btn[y * nFieldWidth + x]->Enable(true);
			}
	}
	else
	{
		int mine_count = 0;
		for (int i = -1; i < 2; i++)
			for (int j = -1; j < 2; j++)
			{
				if (x + i >= 0 && x + i < nFieldWidth * y + j >= 0 && y + j < nFieldHeight)
					if (nField[(y + j) * nFieldWidth + (x + i)] == -1) // keep track of how many mines are around the button clicked
						mine_count++;
			}

		// update buttons label to show mine count if > 0
		if (mine_count > 0)
			btn[y * nFieldWidth + x]->SetLabel(std::to_string(mine_count));
		else
		{
			btn[y * nFieldWidth + x]->SetLabel("");

		}
	}


	evt.Skip(); // skip checking the rest of the parents events
}
