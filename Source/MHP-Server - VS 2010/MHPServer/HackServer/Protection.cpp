#include "stdafx.h"
#include "Protection.h"
#include "ThemidaSDK.h"
#include "resource.h"
#include "wininet.h"

#include <fstream>
#include <list>
#include <string>
#include <sstream>
#include "computerHash.h"
#include "base64.h"
#include "Util.h"

CProtection gProtection;

LRESULT CALLBACK LicenseErrorBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
			{
				SetWindowText(GetDlgItem(hDlg, IDC_EDIT1), gProtection.m_ComputerHardwareId);//strupr(gLicense.m_Data.Hash1));
				SetWindowText(GetDlgItem(hDlg, IDC_EDIT2), gProtection.ipAddr);//strupr(gLicense.m_Data.Hash1));
				return TRUE;
			}

		case WM_COMMAND:
			if ((wParam & 0xFFFF & 0xFFFF) == IDC_BTNCOPY)
			{
				if(OpenClipboard(nullptr))//открываем буфер обмена
				{
					
					char text[512];
					strcpy_s(text,"HardwareId: ");
					strcat_s(text,gProtection.m_ComputerHardwareId);
					strcat_s(text,"\r\n");
					strcat_s(text,"IpAddress: ");
					strcat_s(text,gProtection.ipAddr);

					copy_to_clipboard(text);
					
					EndDialog(hDlg, (wParam& 0xFFFF & 0xFFFF ));
					MessageBox(nullptr,"Registration data successfully copied to clipboard!","INFO",MB_OK | MB_ICONINFORMATION);
					return TRUE;
				}
			}
			else if ((wParam & 0xFFFF & 0xFFFF) == IDC_BTNCOPY_HARDWAREID)
			{
				if(OpenClipboard(nullptr))
				{
					char text[256];
					strcpy_s(text,gProtection.m_ComputerHardwareId);

					copy_to_clipboard(text);
					
					EndDialog(hDlg, (wParam& 0xFFFF & 0xFFFF ));
					MessageBox(nullptr,"Hardware ID successfully copied to clipboard!","INFO",MB_OK | MB_ICONINFORMATION);
					return TRUE;
				}
			}
			else if ((wParam & 0xFFFF & 0xFFFF) == IDC_BTNCOPY_IPADDRES)
			{
				if(OpenClipboard(nullptr))
				{
					char text[256];
					strcpy_s(text,gProtection.ipAddr);

					copy_to_clipboard(text);
					
					EndDialog(hDlg, (wParam& 0xFFFF & 0xFFFF ));
					MessageBox(nullptr,"IP address successfully copied to clipboard!","INFO",MB_OK | MB_ICONINFORMATION);
					return TRUE;
				}
			}
			else if ( ( wParam & 0xFFFF & 0xFFFF ) == IDOK || (wParam & 0xFFFF & 0xFFFF ) == IDCANCEL || (wParam & 0xFFFF & 0xFFFF) == IDC_BUTTON1) 
			{
				EndDialog(hDlg, (wParam& 0xFFFF & 0xFFFF ));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

CProtection::CProtection()
{
	CLEAR_START

	VM_START

	srand((DWORD)time(nullptr));

	memset(this->m_ComputerHardwareId,0,sizeof(this->m_ComputerHardwareId));

	this->m_EncKey = rand()%256;

	this->m_LicenseActive = 0;

	this->m_Debt = 0;

	memset(this->end_license_date,0,sizeof(this->end_license_date));

	this->m_CustomsData.clear();

	VM_END

	CLEAR_END
}

CProtection::~CProtection()
{
	
}


void CProtection::GetComputerHardwareId()
{
	VMProtectBegin("GetHWID");

	std::string hwid = cComputerHash::machineHash();

	for(int i=0;i<hwid.length();i++)
	{
		this->m_ComputerHardwareId[i] = hwid.c_str()[i];
	}

	VMProtectEnd();
}


void CProtection::Auth(HINSTANCE hIns, HWND hWnd)
{
	//strcpy(this->CustomerName,"WorkPC");
	//return;
	VMProtectBegin("Auth");

	this->offline_activation("OBBY");
	
	VMProtectEnd();
}

void CProtection::offline_activation(char* customer_name)
{
	this->m_LicenseActive = 1;
	this->m_Index = 0;

	strcpy_s(this->CustomerName,customer_name); // Читаем имя пользователя из главного узла.

	for(int i=0;i<CUSTOMS_COUNT;i++)
	{
		this->SetCustomState(i, true); // Устанавливаем статус кастому.
	}
}

void CProtection::ErrorMessageBox1()
{
	memset(this->m_ComputerHardwareId,0,sizeof(this->m_ComputerHardwareId));

	this->SafeExitProcess();
}

void CProtection::InfoBox(char* message,...)
{
	VM_START

	char buff[256];

	memset(buff,0,sizeof(buff));

	va_list arg;
	va_start(arg,message);
	vsprintf_s(buff,message,arg);
	va_end(arg);

	MessageBox(nullptr,buff,"Info",MB_OK | MB_ICONWARNING);

	VM_END
}

void CProtection::SafeExitProcess()
{
	while(true)
	{
		TerminateProcess(GetCurrentProcess(),0);
		CRASH_APPLICATION_MACRO
	}
}

char* CProtection::GetEncryptedString(BYTE* string, int size)
{
	static char buff[256] = {0};

	memset(buff,0,sizeof(buff));

	for(int n=0;n < size;n++)
	{
		buff[n] = string[n]^0xB0;
	}

	return buff;
}

void CProtection::SetCustomState(int index, bool value)
{
	if (index == CUSTOM_VOTEPARCER)
	{
		value = false;
	}

	std::map<int,bool>::iterator it = this->m_CustomsData.find(index);

	if (it == this->m_CustomsData.end())
	{
		this->m_CustomsData.insert(std::pair<int,bool>(index,value));
	}
	else
	{
		it->second = value;
	}
}

bool CProtection::GetCustomState(eCustomsType index)
{
	if (this->m_CustomsData.empty())
	{
		return false;
	}
	
	std::map<int,bool>::iterator it = this->m_CustomsData.find((int)index);

	if (it != this->m_CustomsData.end())
	{
		return (bool)it->second;
	}

	return false;
}

void CProtection::InitDialog(HWND hWnd)
{
	//this->ghWnd = hWnd;

	//std::stringstream str ;
	//str << "License [ " << this->CustomerName << " ] Information";

	//SetWindowText(hWnd,str.str().c_str());

	//SetWindowText(GetDlgItem(this->ghWnd, IDC_LICENSENAME), gProtection.CustomerName);
	//SetWindowText(GetDlgItem(this->ghWnd, IDC_LICENSEHWID), gProtection.m_ComputerHardwareId);
	//SetWindowText(GetDlgItem(this->ghWnd, IDC_LICENSEIP), gProtection.ipAddr);

	//SendMessage(GetDlgItem(this->ghWnd, IDC_MARRY), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_MARRY) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	//SendMessage(GetDlgItem(this->ghWnd, IDC_QUESTSYSTEM), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_QUEST_SYSTEM) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	//SendMessage(GetDlgItem(this->ghWnd, IDC_VOTEPARCER), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_VOTEPARCER) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	//SendMessage(GetDlgItem(this->ghWnd, IDC_TVTEVENT), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_TVTEVENT) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	//SendMessage(GetDlgItem(this->ghWnd, IDC_STATSADD), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_STATSADD) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	//SendMessage(GetDlgItem(this->ghWnd, IDC_EXPANDEDCHARS), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_EXPANDED_CHARACTER) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	//SendMessage(GetDlgItem(this->ghWnd, IDC_PARTYSEARCH), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_PARTY_SEARCH) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	//SendMessage(GetDlgItem(this->ghWnd, IDC_OFFLINEMODE), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_OFFLINE_MODE) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	//SendMessage(GetDlgItem(this->ghWnd, IDC_SMITHY), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_SMITHY) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	//SendMessage(GetDlgItem(this->ghWnd, IDC_BUFFICON), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_BUFFICON) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	//SendMessage(GetDlgItem(this->ghWnd, IDC_BUFFS), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_BUFFS) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	//SendMessage(GetDlgItem(this->ghWnd, IDC_NEWPETS), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_PETS) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	//SendMessage(GetDlgItem(this->ghWnd, IDC_EVENTTIMER), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_EVENTTIMER) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	//SendMessage(GetDlgItem(this->ghWnd, IDC_GUILDWARE), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_GUILD_WAREHOUSE) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	//SendMessage(GetDlgItem(this->ghWnd, IDC_ACHIVEMENTS), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_ACHIEVEMENTS) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	//SendMessage(GetDlgItem(this->ghWnd, IDC_RANKINGS), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_RANKINGS) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	//SendMessage(GetDlgItem(this->ghWnd, IDC_OPTION_REPLACE), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_OPTION_REPLACE) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	//SendMessage(GetDlgItem(this->ghWnd, IDC_NEW_PARTY), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_NEW_PARTY) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
}

bool CProtection::GetTeleportState()
{
	if(this->m_Index == eCustomers::WorkPC)
		return true;
	return false;
}
