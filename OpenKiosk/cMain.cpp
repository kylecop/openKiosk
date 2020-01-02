#include "cMain.h"
#include <sstream>
#include <string>


LONG GetDWORDRegKey(HKEY hKey, const std::wstring& strValueName, DWORD& nValue, DWORD nDefaultValue);
LONG GetBoolRegKey(HKEY hKey, const std::wstring& strValueName, bool& bValue, bool bDefaultValue);
LONG GetStringRegKey(HKEY hKey, const std::wstring& strValueName, std::wstring& strValue, const std::wstring& strDefaultValue);
LONG SetDWORDRegKey(HKEY hKey, const std::wstring& strValueName, DWORD nValue);
bool TaskManagerIsEnabled();
void DisableTaskManager();
void EnableTaskManager();

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_BUTTON(10001, OnButtonClicked)

wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "The Title", wxPoint(30,30), wxSize(800,600))
{
	m_btn1 = new wxButton(this, 10001, "Click Me", wxPoint(10, 10), wxSize(150, 50));
	m_txt1 = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 70), wxSize(300, 30));
	m_list1 = new wxListBox(this, wxID_ANY, wxPoint(10, 110), wxSize(300, 300));
    if (TaskManagerIsEnabled())
    {
        DisableTaskManager();
        if (TaskManagerIsEnabled()) 
        {
            HKEY hKey;//HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\System\DisableTaskMgr = 1(DWORD)
            LONG lRes = RegOpenKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies", 0, KEY_READ, &hKey);
            RegCreateKeyEx(hKey, L"System", NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
            DisableTaskManager();

            if (TaskManagerIsEnabled())
                wxMessageBox("Tried to disable task manager and failed. Do you need administrative privileges?");
            else
                wxMessageBox("Disabled Task Manager");
        }
        else
            wxMessageBox("Disabled Task Manager");
    }
}

cMain::~cMain()
{

    EnableTaskManager();
    if (TaskManagerIsEnabled())
        wxMessageBox("Enabled Task Manager");

    HKEY hKey;//HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\System\DisableTaskMgr = 1(DWORD)
    LONG lRes = RegOpenKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies", 0, KEY_READ, &hKey);
    RegCloseKey(hKey);
    lRes = RegOpenKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", 0, KEY_READ, &hKey);
    RegCloseKey(hKey);
}


void cMain::OnButtonClicked(wxCommandEvent& evt)
{

	//m_list1->AppendString(std::to_string(strValueOfBinDir));
	evt.Skip(); // skip checking the rest of the parents events
}



void DisableTaskManager()
{
    HKEY hKey;//HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\System\DisableTaskMgr = 1(DWORD)
    LONG lRes = RegOpenKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", 0, KEY_WRITE, &hKey);
    SetDWORDRegKey(hKey, L"DisableTaskMgr", (DWORD)1);
}

void EnableTaskManager()
{
    HKEY hKey;//HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\System\DisableTaskMgr = 1(DWORD)
    LONG lRes = RegOpenKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", 0, KEY_WRITE, &hKey);
    SetDWORDRegKey(hKey, L"DisableTaskMgr", (DWORD)0);

}
LONG GetDWORDRegKey(HKEY hKey, const std::wstring& strValueName, DWORD& nValue, DWORD nDefaultValue)
{
    nValue = nDefaultValue;
    DWORD dwBufferSize(sizeof(DWORD));
    DWORD nResult(0);
    LONG nError = ::RegQueryValueExW(hKey, strValueName.c_str(), 0, NULL, reinterpret_cast<LPBYTE>(&nResult), &dwBufferSize);

    if (ERROR_SUCCESS == nError)
    {
        nValue = nResult;
    }
    return nError;
}
LONG SetDWORDRegKey(HKEY hKey, const std::wstring& strValueName, DWORD nValue)
{
    DWORD nResult(0);
    LONG nError = RegSetValueEx(hKey, strValueName.c_str(), NULL, REG_DWORD, (LPBYTE)&nValue, sizeof(DWORD));

    if (ERROR_SUCCESS == nError)
    {
        nValue = nResult;
    }
    return nError;
}

LONG GetBoolRegKey(HKEY hKey, const std::wstring& strValueName, bool& bValue, bool bDefaultValue)
{
    DWORD nDefValue((bDefaultValue) ? 1 : 0);
    DWORD nResult(nDefValue);
    LONG nError = GetDWORDRegKey(hKey, strValueName.c_str(), nResult, nDefValue);
    if (ERROR_SUCCESS == nError)
    {
        bValue = (nResult != 0) ? true : false;
    }
    return nError;
}


LONG GetStringRegKey(HKEY hKey, const std::wstring& strValueName, std::wstring& strValue, const std::wstring& strDefaultValue)
{
    strValue = strDefaultValue;
    WCHAR szBuffer[512];
    DWORD dwBufferSize = sizeof(szBuffer);
    ULONG nError;
    nError = RegQueryValueExW(hKey, strValueName.c_str(), 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);
    if (ERROR_SUCCESS == nError)
    {
        strValue = szBuffer;
    }
    return nError;
}

bool TaskManagerIsEnabled()
{
    bool result = false;

    HKEY hKey;//HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\System\DisableTaskMgr = 1(DWORD)
    LONG lRes = RegOpenKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", 0, KEY_READ, &hKey);
    bool bExistsAndSuccess(lRes == ERROR_SUCCESS);
    bool bDoesNotExistsSpecifically(lRes == ERROR_FILE_NOT_FOUND);
    DWORD strValueOfBinDir;
    std::wstring strKeyDefaultValue;
    GetDWORDRegKey(hKey, L"DisableTaskMgr", strValueOfBinDir, (DWORD)"1"); //check if it is a 1, returns 1 if true, 0 if false    
    if (std::to_string(strValueOfBinDir) != '1')
        result = true;
    return result;
}
