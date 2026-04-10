#include "stdafx.h"
#include "resource.h"
#include "HackServer.h"
#include "MiniDump.h"
#include "ProcessManager.h"
#include "ServerDisplayer.h"
#include "ServerInfo.h"
#include "SocketManager.h"
#include "ReadFiles.h"
#include "BlackList.h"
#include "Authenticate.h"
#include "HidManager.h"
#include "Util.h"
#include "ScreenShow.h"
#include "ClientManager.h"
#include "IpManager.h"
#include "Protection.h"
#include "ProcessListInfo.h"
#include "HelperPlayers.h"

HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
TCHAR szChildTitle[MAX_LOADSTRING];
TCHAR szChildClass[MAX_LOADSTRING];
HWND hWnd;
HWND hChildWnd;
HWND ScreenWnd;

HWND ProcessListWnd;


int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow) // OK
{
	ULONG_PTR gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
	CMiniDump::Start();

	LoadString(hInstance,IDS_APP_TITLE,szTitle,MAX_LOADSTRING);
	LoadString(hInstance,IDC_HACKSERVER,szWindowClass,MAX_LOADSTRING);

	MyRegisterClass(hInstance);

	if(InitInstance(hInstance,nCmdShow) == 0)
	{
		return 0;
	}

	gServerInfo.ReadStartupInfo("MHPServerInfo",".\\MHPServer.ini");

	#if(PROTECT_STATE == 1)

	gAuthenticate.Check();

	#endif

	gProtection.Auth(hInst, hWnd);

	char buff[256];

	wsprintf(buff, "Antihack Server (Version: %s)  (Usuarios : %d)", HACKSERVER_VERSION, GetUserCount());

	SetWindowText(hWnd,buff);

	gServerDisplayer.Init(hWnd);

	WSADATA wsa;

	if(WSAStartup(MAKEWORD(2,2),&wsa) == 0)
	{
		if(gSocketManager.Start(gServerInfo.m_HackServerPort) != 0)
		{
			gServerInfo.ReadInit();

			SetTimer(hWnd,TIMER_1000,1000,nullptr);

			SetTimer(hWnd,TIMER_5000,5000,nullptr);

			SetTimer(ScreenWnd, TIMER_5001, 1, nullptr);
		}
	}
	else
	{
		LogAdd(LOG_RED,"WSAStartup() fallу con el error: %d",WSAGetLastError());
	}

	gServerDisplayer.PaintAllInfo();

	SetTimer(hWnd,TIMER_2000,2000,nullptr);

	HACCEL h_accel_table = LoadAccelerators(hInstance,(LPCTSTR)IDC_HACKSERVER);

	MSG msg;

	while(GetMessage(&msg,nullptr,0,0) != 0)
	{
		if(TranslateAccelerator(msg.hwnd,h_accel_table,&msg) == 0)
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}

	CMiniDump::Clean();
	GdiplusShutdown(gdiplusToken);
	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) // OK
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)wnd_proc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance,(LPCTSTR)IDI_ICON1);
	wcex.hCursor = LoadCursor(nullptr,IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = (LPCSTR)IDC_HACKSERVER;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance,(LPCTSTR)IDI_ICON1);

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(const HINSTANCE h_instance, const int n_cmd_show) // OK
{
	hInst = h_instance;

	hWnd = CreateWindowEx(0,szWindowClass,szTitle, WS_OVERLAPPED + WS_CAPTION + 
		WS_SYSMENU + WS_VISIBLE + WS_MINIMIZEBOX,CW_USEDEFAULT,0,
		600,600,nullptr,nullptr,h_instance,nullptr);

	if(hWnd == nullptr)
		return 0;

	ShowWindow(hWnd,n_cmd_show);
	UpdateWindow(hWnd);
	return 1;
}

LRESULT CALLBACK wnd_proc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) // OK
{
	switch(message)
	{
		case WM_CREATE:
			{
				ScreenWnd = CreateDialogParamA(hInst, LPCTSTR(IDM_SCREEN), hWnd, (DLGPROC)Screen, 0);
				gScreenShow.ScreenWnd = ScreenWnd;
				gScreenShow.hInst = hInst;
			}
		break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDM_ABOUT:
					DialogBox(hInst,(LPCTSTR)IDD_ABOUTBOX,hWnd,(DLGPROC)About);
					break;
				case ID_OTHER_BANNED:
					DialogBox(hInst,(LPCTSTR)IDM_BANNED,hWnd,(DLGPROC)Banned);
					break;
				case ID_OTHER_SCREEN:
					DialogBox(hInst,(LPCTSTR)IDD_INFO_DIALOG,hWnd,(DLGPROC)PlayersList);
					break;
				case ID_OTHER_DELETETEMPBANS:
					gHidManager.DeleteAllHardwareId();
					break;
				case IDM_EXIT:
					if(MessageBox(nullptr,"їEstбs seguro de terminar MHPServer? "," Preguntar terminar servidor",MB_YESNO | MB_ICONQUESTION) == IDYES)
					{
						DestroyWindow(hWnd);
					}
					break;
				case IDM_RELOAD_CONFIG:
					gServerInfo.ReadConfig();
					break;
				case IDM_RELOAD_DUMP:
					gServerInfo.ReadDumpList();
					break;
				case IDM_RELOAD_CHECKSUM:
					gServerInfo.ReadChecksumList();
					break;
				case IDM_RELOAD_WINDOW:
					gServerInfo.ReadWindowList();
					break;
				case IDM_RELOAD_BLACKLIST:
					gServerInfo.ReadBlackList();
					break;
				case IDM_UPDATE:
					gReadFiles.UpdateInternalList();
					break;
				default:
					return DefWindowProc(hWnd,message,wParam,lParam);
			}
			break;
		case WM_TIMER:
			switch(wParam)
			{
				case TIMER_1000:
					gProcessManager.CheckProcess();
					break;
				case TIMER_2000:
					gServerDisplayer.Run();
					break;
				case TIMER_5000:
					TimeoutProc();
					gProcessManager.ClearProcessCache();
					break;
				default:
					break;
			}
			break;
		case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			break;
		default:
			return DefWindowProc(hWnd,message,wParam,lParam);
	}

	return 0;
}

LRESULT CALLBACK About(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam) // OK
{
	switch(message)
	{
		case WM_INITDIALOG:
			return 1;
		case WM_COMMAND:
			if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg,LOWORD(wParam));
				return 1;
			}
			break;
	}

	return 0;
}


LRESULT CALLBACK PlayersList(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) // OK
{
	switch (message)
	{
	case WM_INITDIALOG:
		{
			gHelperPlayers.init_dialog(hDlg);
			ProcessListWnd = CreateDialogParamA(hInst, LPCTSTR(IDM_PROCESS_LIST), hDlg, (DLGPROC)ProcessList, 0);
			gProcessListInfo.ScreenWnd = ProcessListWnd;
			gProcessListInfo.hInst = hInst;
		}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_SEARCH_IP: gHelperPlayers.search_ip(); break;
		case IDC_CLEAR_IP: gHelperPlayers.clear_ip(); break;
		case IDC_BAN_IP: gHelperPlayers.ban_ip(); break;
		case IDC_BAN_HWID: gHelperPlayers.ban_hwid(); break;
		case IDC_CLOSE_CLIENT: gHelperPlayers.close_client(); break;
		case IDC_RESET_PC: gHelperPlayers.reset_pc(); break;
		case IDC_LIST_IP: gHelperPlayers.list_ip(wParam); break;
		case IDC_LIST_HWID: gHelperPlayers.list_hwid(wParam); break;
		case ID_MAKE_SCREENSHOT: gHelperPlayers.make_screen_shot(); break;
		case ID_RELOAD: gHelperPlayers.reload(); break;
		case ID_PROCESSLIST: gHelperPlayers.process_list(); break;
		case ID_DESKTOP: gHelperPlayers.screen_show();	break;
		}
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			if (gScreenShow.WorkIndex != -1)
			{
				gScreenShow.SendScreenState(gScreenShow.WorkIndex, 0);
			}
			EndDialog(hDlg,LOWORD(wParam));
			return 1;
		}
		break;
	}

	return 0;
}

LRESULT CALLBACK Banned(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam) // OK
{
	switch(message)
	{
		case WM_INITDIALOG:
			{
				char WindowName[100];
				int UsersCount = gHidManager.m_HardwareIdInfo.size();
				sprintf(WindowName, "%d Users Prohibidos", UsersCount);
				SetWindowText(hDlg, WindowName);
				HWND accounts_list_box = GetDlgItem(hDlg, IDC_LIST1); 

				for(std::vector<HARDWARE_ID_INFO>::iterator it=gHidManager.m_HardwareIdInfo.begin();it != gHidManager.m_HardwareIdInfo.end();it++)
				{
					SendMessage(accounts_list_box,LB_ADDSTRING,NULL,LPARAM(it->HardwareId));
				}
			}
			break;

		case WM_COMMAND:
			if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg,LOWORD(wParam));
				return 1;
			}
			break;
	}

	return 0;
}

#include <windowsx.h>

LRESULT CALLBACK Screen(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam) // OK
{
    RECT rect;
    HDC hdc;	

	switch(message)
	{
		case WM_CREATE:
			{

			}
			break;
		case WM_INITDIALOG:
			{
				SetWindowText(hDlg, "SCREEN");
			}
			break;
		case WM_PAINT:
			{
			PAINTSTRUCT ps;
            hdc = BeginPaint(ScreenWnd, &ps);
			gScreenShow.DrawScreen(ScreenWnd,hdc);
            EndPaint(hWnd, &ps);
			}
            break;
		case WM_CLOSE:
			{
				ShowWindow(ScreenWnd, SW_HIDE);
				gScreenShow.WindowOpen = false;
			}
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_DISPLAYS:
                {
					if(HIWORD(wParam) == CBN_SELCHANGE)
					{
						HWND lpComboBox = GetDlgItem(hDlg, IDC_DISPLAYS);
						
						int index = (int)SendMessage(lpComboBox,CB_GETCURSEL,0,0);
						gScreenShow.current_display = index;
						gScreenShow.SendScreenState(gScreenShow.WorkIndex,1);
					}
				}
				break;
			}
			case WM_TIMER:
				switch ( wParam )
				{
				case TIMER_5001:
					{
						hdc = GetDC(hDlg);
						GetClientRect(hDlg, &rect);
						gScreenShow.DrawScreen(ScreenWnd,hdc);
						//Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
						ReleaseDC(hDlg, hdc);
					}
					break;
				}
				
			if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				gScreenShow.SendScreenState(gScreenShow.WorkIndex,0);
				EndDialog(hDlg,LOWORD(wParam));
				return 1;
			}
			break;
	}

	return 0;
}


#define IDC_LISTVIEW                      1111


 
int SetListViewColumns(HWND hWndLV, int colNum, int textMaxLen, char** header, int* sizes)
{
    RECT rcl;
    GetClientRect(hWndLV, &rcl);
 
    int index = -1;
 
    LVCOLUMN lvc;
    lvc.mask = LVCF_TEXT | LVCF_WIDTH;
    lvc.cx = (rcl.right - rcl.left) / colNum;
    lvc.cchTextMax = textMaxLen;
 
    for (int i = 0; i < colNum; i++)
    {
		lvc.cx = sizes[i];
        lvc.pszText = header[i];
        index = ListView_InsertColumn(hWndLV, i, &lvc);
        if (index == -1) break;
    }
 
    return index;
}

HWND CreateListView(HWND hWndParent, UINT uId)
{     
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC  = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);
 
    RECT rcl;
    GetClientRect(hWndParent, &rcl);
 
    HWND hWndLV = CreateWindow(WC_LISTVIEW, "",
        WS_CHILD | LVS_REPORT,
        0, 0, rcl.right - rcl.left, rcl.bottom - rcl.top - 150,
        hWndParent, (HMENU)uId, GetModuleHandle(NULL), NULL);
 
    // Чтобы определялись строка (item) и столбец (subitem) обязательно устанавливаем
    // расширенный стиль LVS_EX_FULLROWSELECT.
    ListView_SetExtendedListViewStyleEx(hWndLV, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
 
    return (hWndLV);
}

LRESULT CALLBACK ProcessList(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) // OK
{
	static HWND hWndLV = nullptr;

	int const colNum = 3;
	char* header[colNum] = {"Name", "PID", "Path"};
	int sizes[3] = {200, 70, 800};

	switch (message)
	{
		case WM_NOTIFY :
		{
			if (((LPNMHDR)lParam)->code == NM_CLICK || ((LPNMHDR)lParam)->code == NM_RCLICK)
			{
				int iSelect = SendMessage(hWndLV, LVM_GETNEXTITEM,
					-1, LVNI_FOCUSED); // return item selected

				if (iSelect == -1) // no items
				{
					MessageBox(hWnd, "No hay elementos en ListView",
						"Error", MB_OK | MB_ICONINFORMATION);
					break;
				}

				//MsgBox("IndeX: %d", iSelect);
				gProcessListInfo.SetInfoByIndex(iSelect);
				//flag = 1; // set flag so that we know there was a hit
			}
			//switch (((LPNMHDR)lParam)->code)
			//{
			//case LVN_ITEMCHANGED:
			//	NMLISTVIEW* plv;
			//	plv = (NMLISTVIEW*)lParam;
			//	plv.
			//	// вот искомая переменная.
			//	MsgBox("dasdasdsa");
			//	plv->iItem;
			//	break;
			//}
			//break;
		}
		break;

	case WM_INITDIALOG:
		{
			if ((hWndLV = CreateListView(hDlg, IDC_LISTVIEW)) == NULL)
			{
				MessageBox(nullptr, "No se puede crear el elemento ListView", "Error", MB_OK);
			}

			gProcessListInfo.hWndLV = hWndLV;
			gProcessListInfo.hDlg = hDlg;

			SetListViewColumns(hWndLV, colNum, 50, header, sizes);

			ShowWindow(hWndLV, SW_SHOWDEFAULT);

			SetWindowText(hDlg, "Lista de procesos");
		}
		break;
	case WM_CLOSE:
		{
			ShowWindow(hDlg, SW_HIDE);
		}
		break;
	case WM_COMMAND:
		int wmId = LOWORD(wParam);
		int wmEvent = HIWORD(wParam);

		switch (wmId)
		{
		case IDC_GET_DUMP:
			{
				int iPos = ListView_GetNextItem(gProcessListInfo.hWndLV, -1, LVNI_SELECTED);
				gProcessListInfo.SendRequest(iPos, RequestType::request_dump);
			}
			break;
		case IDC_GET_CLASS:
			{
				int iPos = ListView_GetNextItem(gProcessListInfo.hWndLV, -1, LVNI_SELECTED);
				gProcessListInfo.SendRequest(iPos, RequestType::request_window);
			}
			break;
		case IDC_DOWNLOAD_FILE:
			{
				int iPos = ListView_GetNextItem(gProcessListInfo.hWndLV, -1, LVNI_SELECTED);
				gProcessListInfo.SendRequest(iPos, RequestType::request_file);
			}
			break;
		}


		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			return 1;
		}
		break;
	}

	return 0;
}
