#include "cMain.h"
#include <sstream>
#include <string>



bool loggedIn = false;
bool HookKeys = false;
LRESULT CALLBACK LLKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_BUTTON(10001, OnButtonClicked)
    EVT_TEXT_ENTER(10002, OnButtonClicked)
wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "OpenKiosk", wxPoint(30,30), wxSize(800,600))
{
    m_pnl1 = new wxPanel(this,wxID_ANY,wxPoint(0,0));
    ShowFullScreen(true);
    SetWindowStyle(wxSTAY_ON_TOP);
    m_txt1 = new wxTextCtrl(this->m_pnl1, wxID_ANY, "", wxPoint(GetSize().GetWidth() / 2 - 150, GetSize().GetHeight() / 2), wxSize(300, 30));
    m_txt2 = new wxTextCtrl(this->m_pnl1, 10002, "", wxPoint(GetSize().GetWidth() / 2 - 150, GetSize().GetHeight() / 2 + 50), wxSize(300, 30));
    m_btn1 = new wxButton(this->m_pnl1, 10001, "Login", wxPoint(GetSize().GetWidth() / 2 - 150/2, GetSize().GetHeight() / 2 + 100), wxSize(150, 30));
	//m_list1 = new wxListBox(this, wxID_ANY, wxPoint(10, 110), wxSize(300, 300));
    
    HookKeyboard();
    m_txt2->SetWindowStyle(wxTE_PASSWORD);
    m_txt2->Refresh();
    m_txt1->SetFocus();
    wxPlatformInfo plat;
    if (plat.GetOperatingSystemFamilyName() == "Windows")
    {
        bool hidepwd = true;
        HWND hwnd = (HWND)m_txt2->GetHandle();
        SendMessage(hwnd, EM_SETPASSWORDCHAR, hidepwd ? 0x25cf : 0, 0); // 0x25cf is . character
        m_txt2->Refresh();
    }
}


cMain::~cMain()
{

}


void cMain::OnEnterPressed(wxCommandEvent& evt2)
{

}

void cMain::OnButtonClicked(wxCommandEvent& evt)
{
    if (!loggedIn)
    {
        if (m_txt1->GetValue() == "miguel" && m_txt2->GetValue() == "dino")
            ChangeSessionState();
    }
    else
        ChangeSessionState();

    m_txt1->SetValue("");
    m_txt2->SetValue("");
	evt.Skip(); // skip checking the rest of the parents events
}
void cMain::ChangeSessionState()
{

    if (loggedIn) { // you are logged in, process the log out
        SetWindowStyle(wxSTAY_ON_TOP);
        HookKeys = false;
        m_txt1->Show();
        m_txt2->Show();
        m_btn1->SetPosition(wxPoint(m_txt2->GetPosition().x + 150 / 2, m_txt2->GetPosition().y + 50));
        m_btn1->SetLabelText("Login");
        loggedIn = false;
        ShowFullScreen(true);
        m_txt1->SetFocus();
    }
    else { // you are logged out, process the log in
        ShowFullScreen(false);
        Hide();
        HookKeys = false;
        SetWindowStyle(wxDEFAULT_FRAME_STYLE);
        SetSize(wxRect(GetSize().GetWidth(), GetSize().GetHeight(), 400, 400));
        m_txt1->Hide();
        m_txt2->Hide();
        m_btn1->SetPosition(wxPoint(GetSize().GetWidth() / 2 - 150 / 2, GetSize().GetHeight() / 2 + 90));
        m_btn1->SetLabelText("Logout");
        ::InvalidateRect(NULL, NULL, true);
        Centre();
        Show();
        loggedIn = true;
    }
}

void cMain::HookKeyboard()
{

    static HHOOK hook_keys;
    hook_keys = SetWindowsHookEx(WH_KEYBOARD_LL, LLKeyboardProc, 0, 0);
}

LRESULT CALLBACK LLKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    PKBDLLHOOKSTRUCT hookstruct;

    if (nCode == HC_ACTION)
    {
        switch (wParam)
        {
        case WM_KEYDOWN: case WM_SYSKEYDOWN:
        case WM_KEYUP: case WM_SYSKEYUP:

            hookstruct = (PKBDLLHOOKSTRUCT)lParam;
            if (HookKeys)
                if (hookstruct->vkCode == 9) /* pesky Windows button */
                {
                    return 1;
                }
                else if (hookstruct->vkCode == 91)
                    return 1;
                else
                    return CallNextHookEx(NULL, nCode, wParam, lParam);
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}
