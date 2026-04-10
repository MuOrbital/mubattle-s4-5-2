#include "stdafx.h"
#include "HelperPlayers.h"
#include "ClientManager.h"
#include "resource.h"
#include "Util.h"
#include "BlackList.h"
#include "HackServerProtocol.h"
#include "ProcessListInfo.h"
#include "ScreenShow.h"
#include "Protection.h"

CHelperPlayers gHelperPlayers;

void CHelperPlayers::init_dialog(const HWND hwnd)
{
	this->set_hwnd(hwnd);

	HICON hIcon = (HICON)LoadImage(GetModuleHandle(nullptr),
	                                MAKEINTRESOURCE(IDI_ICON1),
	                                IMAGE_ICON,
	                                GetSystemMetrics(SM_CXSMICON),
	                                GetSystemMetrics(SM_CYSMICON),
	                                0);
	if (hIcon)
		SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

	this->reload();
}

void CHelperPlayers::close_client()
{
	HWND ip_lb_control = GetDlgItem(this->window_hwnd_, IDC_LIST_IP);

	HWND hwid_lb_control = GetDlgItem(this->window_hwnd_, IDC_LIST_HWID);
	
	/*char* ip_text = get_listbox_data<char*>(ip_lb_control);
	char* hwid_text = get_listbox_data<char*>(hwid_lb_control);

	if (ip_text == nullptr || hwid_text == nullptr)
		return;

	int aIndex = GetIndexByIpAndHwid(ip_text, hwid_text);*/

	int aIndex = get_listbox_data<int>(hwid_lb_control);

	if (aIndex == -1)
	{
		LogAdd(LOG_RED, "[CloseClient] Error. ¡No Puedo Obtener El Indice De Usuarios!");
		return;
	}

	HCClientDisconnectSend(aIndex, 1488);
}

void CHelperPlayers::search_ip()
{
	HWND hwid_lb_control = GetDlgItem(this->window_hwnd_, IDC_LIST_HWID);
	HWND ip_lb_control = GetDlgItem(this->window_hwnd_, IDC_LIST_IP);
	HWND control = GetDlgItem(this->window_hwnd_, IDC_IP_ADDRESS);
	DWORD IPAddress;
	SendMessage(control, IPM_GETADDRESS, 0, (LPARAM)&IPAddress);
	char Buff[64];
	sprintf(Buff, "%d.%d.%d.%d", (int)FIRST_IPADDRESS(IPAddress), (int)SECOND_IPADDRESS(IPAddress), (int)THIRD_IPADDRESS(IPAddress), (int)FOURTH_IPADDRESS(IPAddress));

	int lb_count = SendMessage(ip_lb_control, LB_GETCOUNT, 0, 0);

	for (int i = 0; i < lb_count; i++)
	{
		char* ip_text = get_listbox_data<char*>(ip_lb_control,i);
		
		if(ip_text == nullptr)
			continue;

		if (!strcmp(Buff, ip_text))
		{
			SendMessage(ip_lb_control, LB_SETCURSEL, i, 0);

			HWND ip_control = GetDlgItem(this->window_hwnd_, IDC_STATIC_IP);
			HWND hwid_control = GetDlgItem(this->window_hwnd_, IDC_STATIC_HWID);

			SetWindowText(hwid_control, "XXXXXXX-XXXXXXX-XXXXXXX-XXXXXXX");

			SetWindowText(ip_control, ip_text);

			int count = 0;
			char text[64];

			for (int n = 0; n < MAX_CLIENT; n++)
			{
				if (gClientManager[n].CheckState() != 0)
				{
					if (!strcmp(ip_text, gClientManager[n].m_IpAddr))
					{
						count++;
						sprintf(text, "%d). %s", count, gClientManager[n].m_HardwareId);
						SendMessage(hwid_lb_control, LB_ADDSTRING, NULL, LPARAM(text));
						SendMessage(hwid_lb_control, LB_SETITEMDATA, count - 1, (LPARAM)gClientManager[n].m_HardwareId);
					}
				}
			}
		}
	}
}

void CHelperPlayers::clear_ip()
{
	HWND control = GetDlgItem(this->window_hwnd_, IDC_IP_ADDRESS);

	SendMessage(control, IPM_CLEARADDRESS, 0, 0);
}

void CHelperPlayers::ban_ip()
{
	HWND ip_control = GetDlgItem(this->window_hwnd_, IDC_STATIC_IP);

	char ip_text[17];
	GetWindowText(ip_control, ip_text, sizeof(ip_text));

	if (!validateIP(ip_text))
		return;

	BLACKLIST_IPADDR_INFO IpInfo;

	strcpy_s(IpInfo.IpAddress, ip_text);

	gBlackList.gIpAddressBlackList.push_back(IpInfo);

	gBlackList.WriteNewBlackList("BlackList.txt");

	this->close_client();
}

void CHelperPlayers::ban_hwid()
{
	HWND hwid_control = GetDlgItem(this->window_hwnd_, IDC_STATIC_HWID);

	char hwid_text[36];
	GetWindowText(hwid_control, hwid_text, sizeof(hwid_text));

	const int hwid_length = 35;

	if (strlen(hwid_text) != hwid_length)
		return;

	BLACKLIST_HARDID_INFO IpInfo;

	strcpy_s(IpInfo.HardwareId, hwid_text);

	gBlackList.gHardwareIdBlackList.push_back(IpInfo);

	gBlackList.WriteNewBlackList("BlackList.txt");

	close_client();
}

void CHelperPlayers::reset_pc()
{
	HWND ip_lb_control = GetDlgItem(this->window_hwnd_, IDC_LIST_IP);

	HWND hwid_lb_control = GetDlgItem(this->window_hwnd_, IDC_LIST_HWID);

	/*char* ip_text = get_listbox_data<char*>(ip_lb_control);
	char* hwid_text = get_listbox_data<char*>(hwid_lb_control);

	if (ip_text == nullptr || hwid_text == nullptr)
		return;

	int aIndex = GetIndexByIpAndHwid(ip_text, hwid_text);*/

	int aIndex = get_listbox_data<int>(hwid_lb_control);

	if (aIndex == -1)
	{
		LogAdd(LOG_RED, "[RebootClient] Error. ¡No Puedo Obtener El Indice De Usuarios!");
		return;
	}

	HCClientRebootSend(aIndex, 0);
}


void CHelperPlayers::list_ip(WPARAM wParam)
{
	if (HIWORD(wParam) == LBN_SELCHANGE)
	{
		HWND ip_lb_control = GetDlgItem(this->window_hwnd_, IDC_LIST_IP);
		HWND hwid_lb_control = GetDlgItem(this->window_hwnd_, IDC_LIST_HWID);

		HWND ip_control = GetDlgItem(this->window_hwnd_, IDC_STATIC_IP);
		HWND hwid_control = GetDlgItem(this->window_hwnd_, IDC_STATIC_HWID);

		SetWindowText(hwid_control, "XXXXXXX-XXXXXXX-XXXXXXX-XXXXXXX");

		char* ip_text = get_listbox_data<char*>(ip_lb_control);
		if (ip_text == nullptr)
		{
			LogAdd(LOG_RED,"No puedo obtener texto");
			return;
		}
		
		SetWindowText(ip_control, ip_text);
		
		SendMessage(hwid_lb_control, LB_RESETCONTENT, 0, NULL);
		
		int count = 0;
		char text[64];

		for (int n = 0; n < MAX_CLIENT; n++)
		{
			if (gClientManager[n].CheckState() != 0)
			{
				if (!strcmp(ip_text, gClientManager[n].m_IpAddr))
				{
					count++;
					sprintf(text, "%d). %s", count, gClientManager[n].m_HardwareId);
					SendMessage(hwid_lb_control, LB_ADDSTRING, NULL, LPARAM(text));

					/*hardware_id_data data;
					data.client_index = gClientManager[n].m_index;
					strcpy_s(data.hardware_id, gClientManager[n].m_HardwareId);*/
					
					SendMessage(hwid_lb_control, LB_SETITEMDATA, count-1, (LPARAM)gClientManager[n].m_index);
				}
			}
		}
	}
}

void CHelperPlayers::list_hwid(WPARAM wParam)
{
	if (HIWORD(wParam) == LBN_SELCHANGE)
	{
		HWND ip_lb_control = GetDlgItem(this->window_hwnd_, IDC_LIST_IP);
		
		HWND hwid_control = GetDlgItem(this->window_hwnd_, IDC_STATIC_HWID);
		HWND hwid_lb_control = GetDlgItem(this->window_hwnd_, IDC_LIST_HWID);

		/*char* ip_text = get_listbox_data<char*>(ip_lb_control);

		char* hwid_text = get_listbox_data<char*>(hwid_lb_control);

		if (hwid_text == nullptr)
			return;

		SetWindowText(hwid_control, hwid_text);

		int index = GetIndexByIpAndHwid(ip_text, hwid_text);*/

		int index = get_listbox_data<int>(hwid_lb_control);

		SetWindowText(hwid_control, GetHwidByIndex(index));


		if (index == -1)
		{
			LogAdd(LOG_RED, "[UserAccount] Error. ¡No Puedo Obtener El Indice De Usuarios!");
			return;
		}
		
		HCGetAccountDataSend(index);
	}
}

void CHelperPlayers::make_screen_shot()
{
	HWND ip_lb_control = GetDlgItem(this->window_hwnd_, IDC_LIST_IP);
	HWND hwid_lb_control = GetDlgItem(this->window_hwnd_, IDC_LIST_HWID);

	/*char* ip_text = get_listbox_data<char*>(ip_lb_control);
	char* hwid_text = get_listbox_data<char*>(hwid_lb_control);

	if (ip_text == nullptr || hwid_text == nullptr)
		return;

	int index = GetIndexByIpAndHwid(ip_text, hwid_text);*/

	int index = get_listbox_data<int>(hwid_lb_control);

	if (index == -1)
	{
		LogAdd(LOG_RED, "[GetScreenShot] Error. ¡No Puedo Obtener El Indice De Usuarios!");
		return;
	}

	GCClientScreenShotSend(index);
}

void CHelperPlayers::reload()
{
	HWND ip_list_control = GetDlgItem(this->window_hwnd_, IDC_LIST_IP);
	HWND hwid_list_control = GetDlgItem(this->window_hwnd_, IDC_LIST_HWID);
	SendMessage(ip_list_control, LB_RESETCONTENT, 0, NULL);
	SendMessage(hwid_list_control, LB_RESETCONTENT, 0, NULL);

	char window_name[100];
	sprintf(window_name, "Información de los jugadores (Connected %d Users)", GetUserCount());
	SetWindowText(this->window_hwnd_, window_name);

	int count = 0;
	char text[64];

	for (int n = 0; n < MAX_CLIENT; n++)
	{
		if (gClientManager[n].CheckState() != 0)
		{
			if (CheckListBox(this->window_hwnd_, IDC_LIST_IP, gClientManager[n].m_IpAddr) == 0)
			{
				continue;
			}
			
			count++;
			
			sprintf(text, "%d). %s [%d]", count, gClientManager[n].m_IpAddr, get_count_hwid_by_ip(gClientManager[n].m_IpAddr));
			SendMessage(ip_list_control, LB_ADDSTRING, NULL, LPARAM(text));
			SendMessage(ip_list_control, LB_SETITEMDATA, count - 1, (LPARAM)gClientManager[n].m_IpAddr);
		}
	}
}

void CHelperPlayers::process_list()
{
	HWND ip_lb_control = GetDlgItem(this->window_hwnd_, IDC_LIST_IP);

	HWND hwid_lb_control = GetDlgItem(this->window_hwnd_, IDC_LIST_HWID);

	/*char* ip_text = get_listbox_data<char*>(ip_lb_control);
	char* hwid_text = get_listbox_data<char*>(hwid_lb_control);

	if (ip_text == nullptr || hwid_text == nullptr)
		return;

	int index = GetIndexByIpAndHwid(ip_text, hwid_text);*/

	int index = get_listbox_data<int>(hwid_lb_control);

	if (index == -1)
	{
		LogAdd(LOG_RED, "[ProcessList] Error. ¡No puedo obtener el índice!");
		return;
	}

	gProcessListInfo.SendProcessRequest(index);
}

void CHelperPlayers::screen_show()
{
	if(!gProtection.GetCustomState(CUSTOM_SCREEN_SHOW))
		MsgBox("¡No tienes acceso a esta función!");
	
	HWND ip_lb_control = GetDlgItem(this->window_hwnd_, IDC_LIST_IP);

	HWND hwid_lb_control = GetDlgItem(this->window_hwnd_, IDC_LIST_HWID);

	/*char* ip_text = get_listbox_data<char*>(ip_lb_control);
	char* hwid_text = get_listbox_data<char*>(hwid_lb_control);

	if (ip_text == nullptr || hwid_text == nullptr)
		return;

	int index = GetIndexByIpAndHwid(ip_text, hwid_text);*/

	int index = get_listbox_data<int>(hwid_lb_control);

	if (index == -1)
	{
		LogAdd(LOG_RED, "[DesktopCapture] Error. ¡No puedo obtener el índice!");
		return;
	}

	gScreenShow.SendScreenState(gScreenShow.WorkIndex, 0);
	gScreenShow.SendScreenState(index, 1);
}

void CHelperPlayers::set_account_info(int index)
{
    HWND hwid_lb_control = GetDlgItem(this->window_hwnd_, IDC_LIST_HWID);

    // Usamos apenas o método atual (get_listbox_data do HWID), que é o que o resto do código usa
    int current_index = get_listbox_data<int>(hwid_lb_control);

    if (current_index == -1)
    {
        LogAdd(LOG_RED, "[AccountInfo] Error. ¡No puedo obtener el índice!");
        return;
    }

    if (current_index != index)
    {
        LogAdd(LOG_RED, "[AccountInfo] Error. ¡Índice incorrecto!");
        return;
    }

    HWND account_control = GetDlgItem(this->window_hwnd_, IDC_STATIC_ACCOUNT);
    HWND name_control = GetDlgItem(this->window_hwnd_, IDC_STATIC_CHARACTER_NAME);

    SetWindowText(account_control, gClientManager[index].account);
    SetWindowText(name_control, gClientManager[index].name);
}
