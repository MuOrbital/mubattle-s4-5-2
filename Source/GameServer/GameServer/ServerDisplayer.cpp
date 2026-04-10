// ServerDisplayer.cpp: implementation of the CServerDisplayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerDisplayer.h"
#include "CustomArena.h"
#include "GameMain.h"
#include "Log.h"
#include "Protect.h"
#include "resource.h"
#include "ServerInfo.h"
#include "SocketManager.h"
#include "User.h"
#include "InvasionManager.h"

CServerDisplayer gServerDisplayer;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServerDisplayer::CServerDisplayer() // OK
{
	this->EventBc = -1;

	for (int n = 0; n < MAX_LOG_TEXT_LINE; n++)
	{
		memset(&this->m_log[n], 0, sizeof(this->m_log[n]));
	}

	this->m_font = CreateFont(50, 0, 0, 0, FW_THIN, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Times");
	this->m_font2 = CreateFont(24, 0, 0, 0, FW_THIN, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Times");
	this->m_font3 = CreateFont(15, 0, 0, 0, FW_THIN, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");
	this->m_font4 = CreateFont(20, 0, 0, 0, FW_THIN, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Times");


#if(GAMESERVER_NOMBRE == 0)
	this->m_brush[0] = CreateSolidBrush(RGB(0, 200, 120)); //-- cuando esta activo | Verde elegante (status ON) | #00C878
	this->m_brush[1] = CreateSolidBrush(RGB(90, 90, 90)); //-- cuando esta desactivado | Cinza neutro | #5A5A5A
	this->m_brush[2] = CreateSolidBrush(RGB(0, 120, 215)); //-- Encabezado | Azul moderno (Windows style premium) | #0078D7
	this->m_brush[3] = CreateSolidBrush(RGB(18, 18, 22)); //-- Fondo Principal | Preto profundo | #121216
	this->m_brush[4] = CreateSolidBrush(RGB(30, 30, 36)); //-- fondo de eventos e informacion | Cinza escuro suave | #1E1E24
#elif(GAMESERVER_NOMBRE == 1)
	this->m_brush[0] = CreateSolidBrush(RGB(60, 255, 51)); //-- cuando esta activo
	this->m_brush[1] = CreateSolidBrush(RGB(120, 120, 120)); //-- cuando esta desactivado
	this->m_brush[2] = CreateSolidBrush(RGB(0, 152, 239)); //-- Encabezado
	this->m_brush[3] = CreateSolidBrush(RGB(255, 255, 255)); //-- Fondo Principal
	this->m_brush[4] = CreateSolidBrush(RGB(210, 210, 210)); //-- fondo de eventos e informacion
#else
	this->m_brush[0] = CreateSolidBrush(RGB(201, 214, 223)); //-- cuando esta activo
	this->m_brush[1] = CreateSolidBrush(RGB(201, 214, 223)); //-- cuando esta desactivado
	this->m_brush[2] = CreateSolidBrush(RGB(82, 97, 107)); //-- Encabezado
	this->m_brush[3] = CreateSolidBrush(RGB(27, 38, 44)); //-- Fondo Principal
	this->m_brush[4] = CreateSolidBrush(RGB(48, 56, 65)); //-- fondo de eventos e informacion
#endif

	strcpy_s(this->m_DisplayerText[0], "STANDBY MODE");
	strcpy_s(this->m_DisplayerText[1], "ACTIVE MODE");

	m_hBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP2));
}

CServerDisplayer::~CServerDisplayer() // OK
{
	DeleteObject(this->m_font);
	DeleteObject(this->m_brush[0]);
	DeleteObject(this->m_brush[1]);
	DeleteObject(this->m_brush[2]);
	DeleteObject(this->m_brush[3]);
	DeleteObject(this->m_brush[4]);
	if (m_hBitmap != nullptr)
	{
		DeleteObject(m_hBitmap);
		m_hBitmap = nullptr;
	}
}

void CServerDisplayer::Init(HWND hWnd) // OK
{
	PROTECT_START

		this->m_hwnd = hWnd;

	CreateDirectoryA("Logs", NULL);

	PROTECT_FINAL

		gLog.AddLog(1, "Logs/LOG");
	gLog.AddLog(gServerInfo.m_WriteChatLog, "Logs/CHAT_LOG");
	gLog.AddLog(gServerInfo.m_WriteCommandLog, "Logs/COMMAND_LOG");
	gLog.AddLog(gServerInfo.m_WriteTradeLog, "Logs/TRADE_LOG");
	gLog.AddLog(gServerInfo.m_WriteConnectLog, "Logs/CONNECT_LOG");
	gLog.AddLog(gServerInfo.m_WriteHackLog, "Logs/HACK_LOG");
	gLog.AddLog(gServerInfo.m_WriteCashShopLog, "Logs/CASH_SHOP_LOG");
	gLog.AddLog(gServerInfo.m_WriteChaosMixLog, "Logs/CHAOS_MIX_LOG");
	gLog.AddLog(1, "Logs/LUA_LOG");
}

void CServerDisplayer::Run() // OK
{
	this->LogTextPaint();
	this->PaintName();
	this->SetWindowName();
	this->PaintAllInfo();
	this->PaintOnline();
	//this->PaintPremium();
	this->PaintSeason();
	this->PaintEventTime();
	this->PaintInvasionTime();
	this->PaintCustomArenaTime();
	this->LogTextPaintConnect();
	this->LogTextPaintGlobalMessage();
}

void CServerDisplayer::SetWindowName() // OK
{
	char buff[256];

	if (Conectar == 0)
	{
		wsprintf(buff, "[%s] %s (ON: %d) %s", GAMESERVER_VERSION, gServerInfo.m_ServerName, gObjTotalUser, GAMESERVER_CLIENT);
	}

	SetWindowText(this->m_hwnd, buff);

	HWND hWndStatusBar = GetDlgItem(this->m_hwnd, IDC_STATUSBAR);

	RECT rect;

	GetClientRect(this->m_hwnd, &rect);

	RECT rect2;

	GetClientRect(hWndStatusBar, &rect2);

	MoveWindow(hWndStatusBar, 0, rect.bottom - rect2.bottom + rect2.top, rect.right, rect2.bottom - rect2.top, true);

	int iStatusWidths[] = { 190,270,360,450,580, -1 };

	char text[256];

	SendMessage(hWndStatusBar, SB_SETPARTS, 6, (LPARAM)iStatusWidths);

	wsprintf(text, "Gameserver %s - Update %d ", GAMESERVER_NAME, UPDATE_GAMING);

	SendMessage(hWndStatusBar, SB_SETTEXT, 0, (LPARAM)text);

	wsprintf(text, "OffStore: %d", gObjOffStore);

	SendMessage(hWndStatusBar, SB_SETTEXT, 1, (LPARAM)text);

	wsprintf(text, "OffAttack: %d", gObjOffAttack);

	SendMessage(hWndStatusBar, SB_SETTEXT, 2, (LPARAM)text);

	wsprintf(text, "Bots Buffer: %d", gObjTotalBot);

	SendMessage(hWndStatusBar, SB_SETTEXT, 3, (LPARAM)text);

	wsprintf(text, "Monsters: %d/%d", gObjTotalMonster, MAX_OBJECT_MONSTER);

	SendMessage(hWndStatusBar, SB_SETTEXT, 4, (LPARAM)text);

	SendMessage(hWndStatusBar, SB_SETTEXT, 5, (LPARAM)NULL);

	ShowWindow(hWndStatusBar, SW_SHOW);

}

void CServerDisplayer::PaintAllInfo() // OK activo y desactivado
{
	RECT rect;
	GetClientRect(this->m_hwnd, &rect);

	long Medida = rect.right - 450;
	Medida = Medida / 3;
	rect.right = rect.right - 450 - Medida - Medida;

	// ABAIXADO - Mudei de +60 para +170
	rect.top = rect.top + 150;
	rect.bottom = rect.top + 25;

	HDC hdc = GetDC(this->m_hwnd);
	int OldBkMode = SetBkMode(hdc, TRANSPARENT);
	HFONT OldFont = (HFONT)SelectObject(hdc, this->m_font4);

	SetTextColor(hdc, RGB(250, 250, 250));
	FillRect(hdc, &rect, this->m_brush[1]);

	if (gJoinServerConnection.CheckState() == 0 || gDataServerConnection.CheckState() == 0)
	{
		DrawText(hdc, GAMESERVER_STATUS, sizeof(GAMESERVER_STATUS), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		DrawText(hdc, GAMESERVER_STATUS_MODE, sizeof(GAMESERVER_STATUS_MODE), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	SelectObject(hdc, OldFont);
	SetBkMode(hdc, OldBkMode);
	ReleaseDC(this->m_hwnd, hdc);
}

void CServerDisplayer::PaintOnline() // OK
{
	RECT rect;
	GetClientRect(this->m_hwnd, &rect);

	long Medida = rect.right - 450;
	long Media = Medida / 2;
	Medida = Medida / 3;

	rect.right = rect.right - 450 - Medida;
	rect.left = Medida + 2;

	// ABAIXADO - Mudei de +60 para +170
	rect.top = rect.top + 150;
	rect.bottom = rect.top + 25;

	HDC hdc = GetDC(this->m_hwnd);
	int OldBkMode = SetBkMode(hdc, TRANSPARENT);
	HFONT OldFont = (HFONT)SelectObject(hdc, this->m_font4);

	SetTextColor(hdc, RGB(250, 250, 250));
	FillRect(hdc, &rect, this->m_brush[1]);

	char text[25];
	if (Conectar == 1)
	{
		if (gServerInfo.m_ServerMaxUserNumber <= 100)
			wsprintf(text, "ONLINE: %d/%d", gObjTotalUser, gServerInfo.m_ServerMaxUserNumber);
		else
			wsprintf(text, "ONLINE: %d/%d", gObjTotalUser, 100);
	}
	else
	{
		wsprintf(text, "ONLINE: %d/%d", gObjTotalUser, gServerInfo.m_ServerMaxUserNumber);
	}

	TextOut(hdc, Media - 60, rect.top + 2, text, strlen(text));

	SelectObject(hdc, OldFont);
	SetBkMode(hdc, OldBkMode);
	ReleaseDC(this->m_hwnd, hdc);
}

void CServerDisplayer::PaintSeason() // OK Season6
{
	RECT rect;
	GetClientRect(this->m_hwnd, &rect);

	long Medida = rect.right - 450;
	Medida = Medida / 3;

	rect.right = rect.right - 450;
	rect.left = Medida + Medida + 2;

	// ABAIXADO - Mudei de +60 para +170
	rect.top = rect.top + 150;
	rect.bottom = rect.top + 25;

	HDC hdc = GetDC(this->m_hwnd);
	int OldBkMode = SetBkMode(hdc, TRANSPARENT);
	HFONT OldFont = (HFONT)SelectObject(hdc, this->m_font4);

	SetTextColor(hdc, RGB(250, 250, 250));

#if(GAMESERVER_NOMBRE == 0)
	FillRect(hdc, &rect, this->m_brush[0]);
#else
	FillRect(hdc, &rect, this->m_brush[1]);
#endif

	DrawText(hdc, GAMESERVER_SEASON, sizeof(GAMESERVER_SEASON), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	SelectObject(hdc, OldFont);
	SetBkMode(hdc, OldBkMode);
	ReleaseDC(this->m_hwnd, hdc);
}
void CServerDisplayer::PaintPremium() // OK
{
	RECT rect;

	GetClientRect(this->m_hwnd, &rect);

	rect.left = rect.right - 180;
	rect.top = rect.bottom - 65;
	rect.bottom = rect.bottom - 20;

	HDC hdc = GetDC(this->m_hwnd);

	int OldBkMode = SetBkMode(hdc, TRANSPARENT);

	HFONT OldFont = (HFONT)SelectObject(hdc, this->m_font2);
	SetTextColor(hdc, RGB(250, 250, 250));//this->m_brush[2]

#if(GAMESERVER_NOMBRE == 0)
	FillRect(hdc, &rect, this->m_brush[0]);
#else
	FillRect(hdc, &rect, this->m_brush[2]);
#endif

	if (Conectar == 0)
	{
		TextOut(hdc, rect.right - 170, rect.top + 14, "GAME SERVER", 7);
	}

	SelectObject(hdc, OldFont);
	SetBkMode(hdc, OldBkMode);
	ReleaseDC(this->m_hwnd, hdc);
}


void CServerDisplayer::PaintName() // OK - Desenha a imagem de fundo
{
	if (m_hBitmap != nullptr)
	{
		RECT Rect;
		GetClientRect(this->m_hwnd, &Rect);

		HDC hdc = GetDC(this->m_hwnd);
		HDC hdcMem = CreateCompatibleDC(hdc);

		HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMem, m_hBitmap);

		// Ajuste aqui o tamanho da imagem conforme sua bitmap
		StretchBlt(hdc, 0, 0, 600, 150, hdcMem, 0, 0, 600, 150, SRCCOPY);

		SelectObject(hdcMem, hBitmapOld);
		DeleteDC(hdcMem);
		ReleaseDC(this->m_hwnd, hdc);
	}
}

void CServerDisplayer::PaintEventTime() // OK
{
#if(GAMESERVER_TYPE==0)
	RECT rect;

	GetClientRect(this->m_hwnd, &rect);

	int posX1 = rect.right - 295;
	int posX2 = rect.right - 200;

	rect.left = rect.right - 300;
	rect.right = rect.right - 150;
	rect.bottom = 290;

	HDC hdc = GetDC(this->m_hwnd);

	int OldBkMode = SetBkMode(hdc, TRANSPARENT);

	FillRect(hdc, &rect, this->m_brush[4]);

	HFONT OldFont = (HFONT)SelectObject(hdc, this->m_font3);

	char text1[20];
	char text2[30];
	int totalseconds;
	int hours;
	int minutes;
	int seconds;
	int days;

	SetTextColor(hdc, RGB(0, 173, 181));
	TextOut(hdc, rect.left + 5, rect.top + 2, "EVENTS:", 7);

	SetTextColor(hdc, RGB(0, 102, 204));
	wsprintf(text1, "Blood Castle: ");

	if (this->EventBc == -1)
	{
		wsprintf(text2, "Disabled");
	}
	else if (this->EventBc == 0)
	{
		wsprintf(text2, "Online");
	}
	else
	{
		totalseconds = this->EventBc;
		hours = totalseconds / 3600;
		minutes = (totalseconds / 60) % 60;
		seconds = totalseconds % 60;
		wsprintf(text2, "%02d:%02d:%02d", hours, minutes, seconds);
	}

	TextOut(hdc, posX1, rect.top + 2 + 15, text1, strlen(text1));
	if (this->EventBc == -1)
	{
		SetTextColor(hdc, RGB(255, 0, 0));
	}
	else if (this->EventBc == 0)
	{
		SetTextColor(hdc, RGB(0, 190, 0));
	}
	else if (this->EventBc < 300)
	{
		SetTextColor(hdc, RGB(0, 190, 0));
	}
	else
	{
#if(GAMESERVER_NOMBRE == 1)
		SetTextColor(hdc, RGB(0, 0, 0));
#else
		SetTextColor(hdc, RGB(255, 255, 255));
#endif
	}
	TextOut(hdc, posX2, rect.top + 2 + 15, text2, strlen(text2));

	//--
	SetTextColor(hdc, RGB(0, 102, 204));
	wsprintf(text1, "Devil Square: ");

	if (this->EventDs == -1)
	{
		wsprintf(text2, "Disabled");
	}
	else if (this->EventDs == 0)
	{
		wsprintf(text2, "Online");
	}
	else
	{
		totalseconds = this->EventDs;
		hours = totalseconds / 3600;
		minutes = (totalseconds / 60) % 60;
		seconds = totalseconds % 60;
		wsprintf(text2, "%02d:%02d:%02d", hours, minutes, seconds);
	}

	TextOut(hdc, posX1, rect.top + 2 + 30, text1, strlen(text1));
	if (this->EventDs == -1)
	{
		SetTextColor(hdc, RGB(255, 0, 0));
	}
	else if (this->EventDs == 0)
	{
		SetTextColor(hdc, RGB(0, 190, 0));
	}
	else if (this->EventDs < 300)
	{
		SetTextColor(hdc, RGB(0, 190, 0));
	}
	else
	{
#if(GAMESERVER_NOMBRE == 1)
		SetTextColor(hdc, RGB(0, 0, 0));
#else
		SetTextColor(hdc, RGB(255, 255, 255));
#endif
	}

	TextOut(hdc, posX2, rect.top + 2 + 30, text2, strlen(text2));

	//--
	SetTextColor(hdc, RGB(0, 102, 204));
	wsprintf(text1, "Chaos Castle: ");

	if (this->EventCc == -1)
	{
		wsprintf(text2, "Disabled");
	}
	else if (this->EventCc == 0)
	{
		wsprintf(text2, "Online");
	}
	else
	{
		totalseconds = this->EventCc;
		hours = totalseconds / 3600;
		minutes = (totalseconds / 60) % 60;
		seconds = totalseconds % 60;
		wsprintf(text2, "%02d:%02d:%02d", hours, minutes, seconds);
	}

	TextOut(hdc, posX1, rect.top + 2 + 45, text1, strlen(text1));
	if (this->EventCc == -1)
	{
		SetTextColor(hdc, RGB(255, 0, 0));
	}
	else if (this->EventCc == 0)
	{
		SetTextColor(hdc, RGB(0, 190, 0));
	}
	else if (this->EventCc < 300)
	{
		SetTextColor(hdc, RGB(0, 190, 0));
	}
	else
	{
#if(GAMESERVER_NOMBRE == 1)
		SetTextColor(hdc, RGB(0, 0, 0));
#else
		SetTextColor(hdc, RGB(255, 255, 255));
#endif
	}
	TextOut(hdc, posX2, rect.top + 2 + 45, text2, strlen(text2));

	//--
	SetTextColor(hdc, RGB(0, 102, 204));
	wsprintf(text1, "Illus.Temple");

	if (this->EventIt == -1)
	{
		wsprintf(text2, "Disabled");
	}
	else if (this->EventIt == 0)
	{
		wsprintf(text2, "Online");
	}
	else
	{
		totalseconds = this->EventIt;
		hours = totalseconds / 3600;
		minutes = (totalseconds / 60) % 60;
		seconds = totalseconds % 60;

		if (hours > 23)
		{
			days = hours / 24;
			wsprintf(text2, "%d day(s)+", days);
		}
		else
		{
			wsprintf(text2, "%02d:%02d:%02d", hours, minutes, seconds);
		}
	}

	TextOut(hdc, posX1, rect.top + 2 + 60, text1, strlen(text1));
	if (this->EventIt == -1)
	{
		SetTextColor(hdc, RGB(255, 0, 0));
	}
	else if (this->EventIt == 0)
	{
		SetTextColor(hdc, RGB(0, 190, 0));
	}
	else if (this->EventIt < 300)
	{
		SetTextColor(hdc, RGB(0, 190, 0));
	}
	else
	{
#if(GAMESERVER_NOMBRE == 1)
		SetTextColor(hdc, RGB(0, 0, 0));
#else
		SetTextColor(hdc, RGB(255, 255, 255));
#endif
	}
	TextOut(hdc, posX2, rect.top + 2 + 60, text2, strlen(text2));

	//--
//	SetTextColor(hdc, RGB(0, 102, 204));
//	wsprintf(text1, "Online Lottery: ");
//
//	if (this->EventCustomLottery == -1)
//	{
//		wsprintf(text2, "Disabled");
//	}
//	else if (this->EventCustomLottery == 0)
//	{
//		wsprintf(text2, "Online");
//	}
//	else
//	{
//		totalseconds = this->EventCustomLottery;
//		hours = totalseconds / 3600;
//		minutes = (totalseconds / 60) % 60;
//		seconds = totalseconds % 60;
//
//		if (hours > 23)
//		{
//			days = hours / 24;
//			wsprintf(text2, "%d day(s)+", days);
//		}
//		else
//		{
//			wsprintf(text2, "%02d:%02d:%02d", hours, minutes, seconds);
//		}
//	}
//
//	TextOut(hdc, posX1, rect.top + 2 + 75, text1, strlen(text1));
//	if (this->EventCustomLottery == -1)
//	{
//		SetTextColor(hdc, RGB(255, 0, 0));
//	}
//	else if (this->EventCustomLottery == 0)
//	{
//		SetTextColor(hdc, RGB(0, 190, 0));
//	}
//	else if (this->EventCustomLottery < 300)
//	{
//		SetTextColor(hdc, RGB(0, 190, 0));
//	}
//	else
//	{
//#if(GAMESERVER_NOMBRE == 1)
//		SetTextColor(hdc, RGB(0, 0, 0));
//#else
//		SetTextColor(hdc, RGB(255, 255, 255));
//#endif
//	}
//	TextOut(hdc, posX2, rect.top + 2 + 75, text2, strlen(text2));

	//--
//	SetTextColor(hdc, RGB(0, 102, 204));
//	wsprintf(text1, "Custom Quiz: ");
//
//	if (this->EventCustomQuiz == -1)
//	{
//		wsprintf(text2, "Disabled");
//	}
//	else if (this->EventCustomQuiz == 0)
//	{
//		wsprintf(text2, "Online");
//	}
//	else
//	{
//		totalseconds = this->EventCustomQuiz;
//		hours = totalseconds / 3600;
//		minutes = (totalseconds / 60) % 60;
//		seconds = totalseconds % 60;
//		wsprintf(text2, "%02d:%02d:%02d", hours, minutes, seconds);
//
//		if (hours > 23)
//		{
//			days = hours / 24;
//			wsprintf(text2, "%d day(s)+", days);
//		}
//		else
//		{
//			wsprintf(text2, "%02d:%02d:%02d", hours, minutes, seconds);
//		}
//	}
//
//	TextOut(hdc, posX1, rect.top + 2 + 90, text1, strlen(text1));
//	if (this->EventCustomQuiz == -1)
//	{
//		SetTextColor(hdc, RGB(255, 0, 0));
//	}
//	else if (this->EventCustomQuiz == 0)
//	{
//		SetTextColor(hdc, RGB(0, 190, 0));
//	}
//	else if (this->EventCustomQuiz < 300)
//	{
//		SetTextColor(hdc, RGB(0, 190, 0));
//	}
//	else
//	{
//#if(GAMESERVER_NOMBRE == 1)
//		SetTextColor(hdc, RGB(0, 0, 0));
//#else
//		SetTextColor(hdc, RGB(255, 255, 255));
//#endif
//	}
//	TextOut(hdc, posX2, rect.top + 2 + 90, text2, strlen(text2));

	//--
	SetTextColor(hdc, RGB(0, 102, 204));
	wsprintf(text1, "Custom Bonus: ");

	if (this->EventCustomBonus == -1)
	{
		wsprintf(text2, "Disabled");
	}
	else if (this->EventCustomBonus == 0)
	{
		wsprintf(text2, "Online");
	}
	else
	{
		totalseconds = this->EventCustomBonus;
		hours = totalseconds / 3600;
		minutes = (totalseconds / 60) % 60;
		seconds = totalseconds % 60;

		if (hours > 23)
		{
			days = hours / 24;
			wsprintf(text2, "%d day(s)+", days);
		}
		else
		{
			wsprintf(text2, "%02d:%02d:%02d", hours, minutes, seconds);
		}
	}

	TextOut(hdc, posX1, rect.top + 2 + 75, text1, strlen(text1));
	if (this->EventCustomBonus == -1)
	{
		SetTextColor(hdc, RGB(255, 0, 0));
	}
	else if (this->EventCustomBonus == 0)
	{
		SetTextColor(hdc, RGB(0, 190, 0));
	}
	else if (this->EventCustomBonus < 300)
	{
		SetTextColor(hdc, RGB(0, 190, 0));
	}
	else
	{
#if(GAMESERVER_NOMBRE == 1)
		SetTextColor(hdc, RGB(0, 0, 0));
#else
		SetTextColor(hdc, RGB(255, 255, 255));
#endif
	}
	TextOut(hdc, posX2, rect.top + 2 + 75, text2, strlen(text2));
	//--
	SetTextColor(hdc, RGB(0, 102, 204));
	wsprintf(text1, "Custom Drop: ");

	if (this->EventDrop == -1)
	{
		wsprintf(text2, "Disabled");
	}
	else if (this->EventDrop == 0)
	{
		wsprintf(text2, "Online");
	}
	else
	{
		totalseconds = this->EventDrop;
		hours = totalseconds / 3600;
		minutes = (totalseconds / 60) % 60;
		seconds = totalseconds % 60;

		if (hours > 23)
		{
			days = hours / 24;
			wsprintf(text2, "%d day(s)+", days);
		}
		else
		{
			wsprintf(text2, "%02d:%02d:%02d", hours, minutes, seconds);
		}
	}

	TextOut(hdc, posX1, rect.top + 2 + 90, text1, strlen(text1));
	if (this->EventDrop == -1)
	{
		SetTextColor(hdc, RGB(255, 0, 0));
	}
	else if (this->EventDrop == 0)
	{
		SetTextColor(hdc, RGB(0, 190, 0));
	}
	else if (this->EventDrop < 300)
	{
		SetTextColor(hdc, RGB(0, 190, 0));
	}
	else
	{
#if(GAMESERVER_NOMBRE == 1)
		SetTextColor(hdc, RGB(0, 0, 0));
#else
		SetTextColor(hdc, RGB(255, 255, 255));
#endif
	}
	TextOut(hdc, posX2, rect.top + 2 + 90, text2, strlen(text2));

	//--
//	SetTextColor(hdc, RGB(0, 102, 204));
//	wsprintf(text1, "King of Mu: ");
//
//	if (this->EventKing == -1)
//	{
//		wsprintf(text2, "Disabled");
//	}
//	else if (this->EventKing == 0)
//	{
//		wsprintf(text2, "Online");
//	}
//	else
//	{
//		totalseconds = this->EventKing;
//		hours = totalseconds / 3600;
//		minutes = (totalseconds / 60) % 60;
//		seconds = totalseconds % 60;
//
//		if (hours > 23)
//		{
//			days = hours / 24;
//			wsprintf(text2, "%d day(s)+", days);
//		}
//		else
//		{
//			wsprintf(text2, "%02d:%02d:%02d", hours, minutes, seconds);
//		}
//	}
//
//	TextOut(hdc, posX1, rect.top + 2 + 135, text1, strlen(text1));
//
//	if (this->EventKing == -1)
//	{
//		SetTextColor(hdc, RGB(255, 0, 0));
//	}
//	else if (this->EventKing == 0)
//	{
//		SetTextColor(hdc, RGB(0, 190, 0));
//	}
//	else if (this->EventKing < 300)
//	{
//		SetTextColor(hdc, RGB(0, 190, 0));
//	}
//	else
//	{
//#if(GAMESERVER_NOMBRE == 1)
//		SetTextColor(hdc, RGB(0, 0, 0));
//#else
//		SetTextColor(hdc, RGB(255, 255, 255));
//#endif
//	}
//
//	TextOut(hdc, posX2, rect.top + 2 + 135, text2, strlen(text2));

	//--
//	SetTextColor(hdc, RGB(0, 102, 204));
//	wsprintf(text1, "TvT event: ");
//
//	if (this->EventTvT == -1)
//	{
//		wsprintf(text2, "Disabled");
//	}
//	else if (this->EventTvT == 0)
//	{
//		wsprintf(text2, "Online");
//	}
//	else
//	{
//		totalseconds = this->EventTvT;
//		hours = totalseconds / 3600;
//		minutes = (totalseconds / 60) % 60;
//		seconds = totalseconds % 60;
//
//		if (hours > 23)
//		{
//			days = hours / 24;
//			wsprintf(text2, "%d day(s)+", days);
//		}
//		else
//		{
//			wsprintf(text2, "%02d:%02d:%02d", hours, minutes, seconds);
//		}
//	}
//
//	TextOut(hdc, posX1, rect.top + 2 + 150, text1, strlen(text1));
//	if (this->EventTvT == -1)
//	{
//		SetTextColor(hdc, RGB(255, 0, 0));
//	}
//	else if (this->EventTvT == 0)
//	{
//		SetTextColor(hdc, RGB(0, 190, 0));
//	}
//	else if (this->EventTvT < 300)
//	{
//		SetTextColor(hdc, RGB(0, 190, 0));
//	}
//	else
//	{
//#if(GAMESERVER_NOMBRE == 1)
//		SetTextColor(hdc, RGB(0, 0, 0));
//#else
//		SetTextColor(hdc, RGB(255, 255, 255));
//#endif
//	}
//
//	TextOut(hdc, posX2, rect.top + 2 + 150, text2, strlen(text2));

#if(GAMESERVER_UPDATE>=601)
	//--
	SetTextColor(hdc, RGB(0, 102, 204));
	wsprintf(text1, "Moss Merch.: ");

	if (this->EventMoss == -1)
	{
		wsprintf(text2, "Disabled");
	}
	else if (this->EventMoss == 0)
	{
		wsprintf(text2, "Online");
	}
	else
	{
		totalseconds = this->EventMoss;
		hours = totalseconds / 3600;
		minutes = (totalseconds / 60) % 60;
		seconds = totalseconds % 60;

		if (hours > 23)
		{
			days = hours / 24;
			wsprintf(text2, "%d day(s)+", days);
		}
		else
		{
			wsprintf(text2, "%02d:%02d:%02d", hours, minutes, seconds);
		}
	}

	TextOut(hdc, posX1, rect.top + 2 + 165, text1, strlen(text1));

	if (this->EventMoss == -1)
	{
		SetTextColor(hdc, RGB(255, 0, 0));
	}
	else if (this->EventMoss == 0)
	{
		SetTextColor(hdc, RGB(0, 190, 0));
	}
	else if (this->EventMoss < 300)
	{
		SetTextColor(hdc, RGB(0, 190, 0));
	}
	else
	{
#if(GAMESERVER_NOMBRE == 1)
		SetTextColor(hdc, RGB(0, 0, 0));
#else
		SetTextColor(hdc, RGB(255, 255, 255));
#endif
	}

	TextOut(hdc, posX2, rect.top + 2 + 165, text2, strlen(text2));

#endif

	SelectObject(hdc, OldFont);
	SetBkMode(hdc, OldBkMode);
	ReleaseDC(this->m_hwnd, hdc);

#endif
}

void CServerDisplayer::PaintInvasionTime() // OK
{
//#if(GAMESERVER_TYPE==0)
	RECT rect;

	GetClientRect(this->m_hwnd, &rect);
	int posX1 = rect.right - 140;
	int posX2 = rect.right - 60;

	rect.left = rect.right - 145;
	rect.right = rect.right - 5;
	rect.bottom = rect.bottom - 20;

	HDC hdc = GetDC(this->m_hwnd);

	int OldBkMode = SetBkMode(hdc, TRANSPARENT);

	FillRect(hdc, &rect, this->m_brush[4]);

	HFONT OldFont = (HFONT)SelectObject(hdc, this->m_font3);

	char text1[20];
	char text2[30];
	int totalseconds;
	int hours;
	int minutes;
	int seconds;
	int days;

	SetTextColor(hdc, RGB(0, 173, 181));
	TextOut(hdc, rect.left + 5, rect.top + 2, "INVASION:", 9);

	for (int n = 0; n < 14; n++)
	{

		SetTextColor(hdc, RGB(0, 102, 204));
		if (n == 0) { wsprintf(text1, "%d - %s", n, "Skeleton King: "); }
		if (n == 1) { wsprintf(text1, "%d - %s", n, "Red Dragon: "); }
		if (n == 2) { wsprintf(text1, "%d - %s", n, "Golden Dragon: "); }
		if (n == 3) { wsprintf(text1, "%d - %s", n, "White Wizard: "); }
		if (n == 4) { wsprintf(text1, "%d - %s", n, "New Year: "); }
		if (n == 5) { wsprintf(text1, "%d - %s", n, "Rabbits: "); }
		if (n == 6) { wsprintf(text1, "%d - %s", n, "Summer: "); }
		if (n == 7) { wsprintf(text1, "%d - %s", n, "Cursed Santa: "); }
		if (n == 8) { wsprintf(text1, "%d - %s", n, "Custom 1: "); }
		if (n == 9) { wsprintf(text1, "%d - %s", n, "Custom 2: "); }
		if (n == 10) { wsprintf(text1, "%d- %s", n, "Custom 3: "); }
		if (n == 11) { wsprintf(text1, "%d- %s", n, "Custom 4: "); }
		if (n == 12) { wsprintf(text1, "%d- %s", n, "Custom 5: "); }
		if (n == 13) { wsprintf(text1, "%d- %s", n, "Custom 6: "); }

		if (this->EventInvasion[n] == -1)
		{
			wsprintf(text2, "Disabled");
		}
		else if (this->EventInvasion[n] == 0)
		{
			wsprintf(text2, "Online");
		}
		else
		{
			totalseconds = this->EventInvasion[n];
			hours = totalseconds / 3600;
			minutes = (totalseconds / 60) % 60;
			seconds = totalseconds % 60;

			if (hours > 23)
			{
				days = hours / 24;
				wsprintf(text2, "%d day(s)+", days);
			}
			else
			{
				wsprintf(text2, "%02d:%02d:%02d", hours, minutes, seconds);
			}
		}

		TextOut(hdc, posX1, rect.top + 20 + (15 * n), text1, strlen(text1));
		if (this->EventInvasion[n] == -1)
		{
			SetTextColor(hdc, RGB(255, 0, 0));
		}
		else if (this->EventInvasion[n] == 0)
		{
			SetTextColor(hdc, RGB(0, 190, 0));
		}
		else if (this->EventInvasion[n] < 300)
		{
			SetTextColor(hdc, RGB(0, 190, 0));
		}
		else
		{
#if(GAMESERVER_NOMBRE == 1)
			SetTextColor(hdc, RGB(0, 0, 0));
#else
			SetTextColor(hdc, RGB(255, 255, 255));
#endif
		}
		TextOut(hdc, posX2, rect.top + 20 + (15 * n), text2, strlen(text2));
	}

	SelectObject(hdc, OldFont);
	SetBkMode(hdc, OldBkMode);
	ReleaseDC(this->m_hwnd, hdc);
//#endif
}

void CServerDisplayer::PaintCustomArenaTime() // OK
{
#if(GAMESERVER_TYPE==0)
	RECT rect;

	GetClientRect(this->m_hwnd, &rect);

	int posX1 = rect.right - 445;
	int posX2 = rect.right - 355;

	rect.left = rect.right - 450;
	rect.right = rect.right - 305;
	//rect.top	= 60;
	rect.bottom = 290;

	HDC hdc = GetDC(this->m_hwnd);

	int OldBkMode = SetBkMode(hdc, TRANSPARENT);

	FillRect(hdc, &rect, this->m_brush[4]);

	HFONT OldFont = (HFONT)SelectObject(hdc, this->m_font3);

	char text1[20];
	char text2[30];
	int totalseconds;
	int hours;
	int minutes;
	int seconds;
	int days;

	SetTextColor(hdc, RGB(0, 173, 181));
	TextOut(hdc, rect.left + 5, rect.top + 2, "CUSTOM ARENA:", 13);

	for (int n = 0; n < 14; n++)
	{

		SetTextColor(hdc, RGB(0, 102, 204));

		//if (strcmp("", gCustomArena.GetArenaName(n)) == 0)
		//{
		//	wsprintf(text1, "%d - Empty", n);
		//}
		//else
		//{
		//	wsprintf(text1, "%d - %s", n, gCustomArena.GetArenaName(n));
		//}

		if (this->EventCustomArena[n] == -1)
		{
			wsprintf(text2, "Disabled");
		}
		else if (this->EventCustomArena[n] == 0)
		{
			wsprintf(text2, "Online");
		}
		else
		{
			totalseconds = this->EventCustomArena[n];
			hours = totalseconds / 3600;
			minutes = (totalseconds / 60) % 60;
			seconds = totalseconds % 60;

			if (hours > 23)
			{
				days = hours / 24;
				wsprintf(text2, "%d day(s)+", days);
			}
			else
			{
				wsprintf(text2, "%02d:%02d:%02d", hours, minutes, seconds);
			}
		}

		TextOut(hdc, posX1, rect.top + 20 + (15 * n), text1, strlen(text1));
		if (this->EventCustomArena[n] == -1)
		{
			SetTextColor(hdc, RGB(255, 0, 0));
		}
		else if (this->EventCustomArena[n] == 0)
		{
			SetTextColor(hdc, RGB(0, 190, 0));
		}
		else if (this->EventCustomArena[n] < 300)
		{
			SetTextColor(hdc, RGB(0, 190, 0));
		}
		else
		{
#if(GAMESERVER_NOMBRE == 1)
			SetTextColor(hdc, RGB(0, 0, 0));
#else
			SetTextColor(hdc, RGB(255, 255, 255));
#endif
		}
		TextOut(hdc, posX2, rect.top + 20 + (15 * n), text2, strlen(text2));
	}

	SelectObject(hdc, OldFont);
	SetBkMode(hdc, OldBkMode);
	ReleaseDC(this->m_hwnd, hdc);

#else

	RECT rect;

	GetClientRect(this->m_hwnd, &rect);

	int posX1 = rect.right - 440;
	int posX2 = rect.right - 370;

	rect.left = rect.right - 450;
	rect.right = rect.right - 5;
	rect.bottom = 290;

	HDC hdc = GetDC(this->m_hwnd);

	int OldBkMode = SetBkMode(hdc, TRANSPARENT);

	FillRect(hdc, &rect, this->m_brush[4]);

	HFONT OldFont = (HFONT)SelectObject(hdc, this->m_font3);

	char text1[20];
	char text2[30];
	char text3[30];
	char text4[30];
	int totalseconds;
	int hours;
	int minutes;
	int seconds;
	int days;

	SetTextColor(hdc, RGB(0, 173, 181));
	TextOut(hdc, rect.left + 5, rect.top + 2, "EVENTS:", 7);

	SetTextColor(hdc, RGB(0, 102, 204));
	wsprintf(text1, "Loren Deep: ");

	if (this->EventCastleDeep == -1)
	{
		wsprintf(text2, "Disabled");
	}
	else if (this->EventCastleDeep == 0)
	{
		wsprintf(text2, "Online");
	}
	else
	{
		totalseconds = this->EventCastleDeep;
		hours = totalseconds / 3600;
		minutes = (totalseconds / 60) % 60;
		seconds = totalseconds % 60;

		if (hours > 23)
		{
			days = hours / 24;
			wsprintf(text2, "%d day(s)+", days);
		}
		else
		{
			wsprintf(text2, "%02d:%02d:%02d", hours, minutes, seconds);
		}
	}

	TextOut(hdc, posX1, 30, text1, strlen(text1));
	if (this->EventCastleDeep == -1)
	{
		SetTextColor(hdc, RGB(255, 0, 0));
	}
	else if (this->EventCastleDeep == 0)
	{
		SetTextColor(hdc, RGB(0, 190, 0));
	}
	else if (this->EventCastleDeep < 300)
	{
		SetTextColor(hdc, RGB(0, 190, 0));
	}
	else
	{
		SetTextColor(hdc, RGB(0, 0, 0));
	}
	TextOut(hdc, posX2, 30, text2, strlen(text2));

	SetTextColor(hdc, RGB(0, 102, 204));
	wsprintf(text1, "CryWolf: ");

	if (this->EventCryWolf == -1)
	{
		wsprintf(text2, "Disabled");
	}
	else if (this->EventCryWolf == 0)
	{
		wsprintf(text2, "Online");
	}
	else
	{
		totalseconds = this->EventCryWolf;
		hours = totalseconds / 3600;
		minutes = (totalseconds / 60) % 60;
		seconds = totalseconds % 60;

		if (hours > 23)
		{
			days = hours / 24;
			wsprintf(text2, "%d day(s)+", days);
		}
		else
		{
			wsprintf(text2, "%02d:%02d:%02d", hours, minutes, seconds);
		}
	}

	TextOut(hdc, posX1, 50, text1, strlen(text1));
	if (this->EventCryWolf == -1)
	{
		SetTextColor(hdc, RGB(255, 0, 0));
	}
	else if (this->EventCryWolf == 0)
	{
		SetTextColor(hdc, RGB(0, 190, 0));
	}
	else if (this->EventCryWolf < 300)
	{
		SetTextColor(hdc, RGB(0, 190, 0));
	}
	else
	{
#if(GAMESERVER_NOMBRE == 1)
		SetTextColor(hdc, RGB(0, 0, 0));
#else
		SetTextColor(hdc, RGB(255, 255, 255));
#endif
	}
	TextOut(hdc, posX2, 50, text2, strlen(text2));

	SetTextColor(hdc, RGB(0, 102, 204));
	wsprintf(text1, "Castle Siege: ");

	if (this->EventCs == -1)
	{
		wsprintf(text2, "Disabled");
	}
	else if (this->EventCs == 0)
	{
		wsprintf(text2, "Adjust Date");
		wsprintf(text3, " ");
		wsprintf(text4, " ");
	}
	else
	{
		totalseconds = this->EventCs;
		hours = totalseconds / 3600;
		minutes = (totalseconds / 60) % 60;
		seconds = totalseconds % 60;

		if (hours > 23)
		{
			days = hours / 24;
			wsprintf(text4, "- Next Stage: %d day(s)+", days);
		}
		else
		{
			wsprintf(text4, "- Next Stage: %02d:%02d:%02d", hours, minutes, seconds);
		}

		if (this->EventCsState == -1)
			wsprintf(text3, "- Stage %d: None", this->EventCsState);
		if (this->EventCsState == 0)
			wsprintf(text3, "- Stage %d: Idle 1", this->EventCsState);
		if (this->EventCsState == 1)
			wsprintf(text3, "- Stage %d: Guild Register", this->EventCsState);
		if (this->EventCsState == 2)
			wsprintf(text3, "- Stage %d: Idle 2", this->EventCsState);
		if (this->EventCsState == 3)
			wsprintf(text3, "- Stage %d: Mark Register", this->EventCsState);
		if (this->EventCsState == 4)
			wsprintf(text3, "- Stage %d: Idle 3", this->EventCsState);
		if (this->EventCsState == 5)
			wsprintf(text3, "- Stage %d: Notify", this->EventCsState);
		if (this->EventCsState == 6)
			wsprintf(text3, "- Stage %d: Ready Siege", this->EventCsState);
		if (this->EventCsState == 7)
			wsprintf(text3, "- Stage %d: Started Siege", this->EventCsState);
		if (this->EventCsState == 8)
			wsprintf(text3, "- Stage %d: Ended Siege", this->EventCsState);
		if (this->EventCsState == 9)
			wsprintf(text3, "- Stage %d: End All", this->EventCsState);

		if (this->EventCs)
			wsprintf(text2, "Stage %d", this->EventCsState);
	}

	TextOut(hdc, posX1, 70, text1, strlen(text1));
	if (this->EventCs == -1)
	{
		SetTextColor(hdc, RGB(255, 0, 0));
	}
	else
	{
#if(GAMESERVER_NOMBRE == 1)
		SetTextColor(hdc, RGB(0, 0, 0));
#else
		SetTextColor(hdc, RGB(255, 255, 255));
#endif
	}
	TextOut(hdc, posX2, 70, text2, strlen(text2));

	TextOut(hdc, posX1 + 5, 155, text3, strlen(text3));

	TextOut(hdc, posX1 + 5, 175, text4, strlen(text4));

	SelectObject(hdc, OldFont);
	SetBkMode(hdc, OldBkMode);
	ReleaseDC(this->m_hwnd, hdc);

#endif

}

void CServerDisplayer::LogTextPaint() // OK
{
	RECT rect;

	GetClientRect(this->m_hwnd, &rect);

	HDC hdc = GetDC(this->m_hwnd);

	int OldBkMode = SetBkMode(hdc, TRANSPARENT);

	FillRect(hdc, &rect, this->m_brush[3]);

	HFONT OldFont = (HFONT)SelectObject(hdc, this->m_font3);

	int line = MAX_LOG_TEXT_LINE;

	int count = (((this->m_count - 1) >= 0) ? (this->m_count - 1) : (MAX_LOG_TEXT_LINE - 1));

	for (int n = 0; n < MAX_LOG_TEXT_LINE; n++)
	{
		switch (this->m_log[count].color)
		{
		case LOG_BLACK:
#if(GAMESERVER_NOMBRE == 1)
			SetTextColor(hdc, RGB(0, 0, 0));
#else
			SetTextColor(hdc, RGB(255, 255, 255));
#endif
			break;
		case LOG_RED:
			SetTextColor(hdc, RGB(239, 0, 0));
			break;
		case LOG_GREEN:
			SetTextColor(hdc, RGB(0, 255, 0));
			break;
		case LOG_BLUE:
			SetTextColor(hdc, RGB(0, 152, 239));
			break;
		case LOG_BOT:
			SetTextColor(hdc, RGB(255, 0, 64));
			break;
		case LOG_USER:
			SetTextColor(hdc, RGB(254, 154, 46));
			break;
		case LOG_EVENT:
			SetTextColor(hdc, RGB(0, 102, 204));
			break;
		case LOG_ALERT:
			SetTextColor(hdc, RGB(0, 173, 181));
			break;
		}

		int size = strlen(this->m_log[count].text);

		if (size > 1)
		{
			TextOut(hdc, 0, (37 + (line * 15)), this->m_log[count].text, size);
			line--;
		}

		count = (((--count) >= 0) ? count : (MAX_LOG_TEXT_LINE - 1));
	}

	SelectObject(hdc, OldFont);
	ReleaseDC(this->m_hwnd, hdc);
}

void CServerDisplayer::LogTextPaintConnect() // OK
{
	RECT rect;

	GetClientRect(this->m_hwnd, &rect);

	rect.left = rect.right - 450;
	rect.right = rect.right - 150;
	rect.top = rect.bottom - 245;
	rect.bottom = rect.bottom - 20;

	HDC hdc = GetDC(this->m_hwnd);

	int OldBkMode = SetBkMode(hdc, TRANSPARENT);

	HFONT OldFont = (HFONT)SelectObject(hdc, this->m_font3);

	FillRect(hdc, &rect, this->m_brush[4]);

	SetTextColor(hdc, RGB(0, 173, 181));
	TextOut(hdc, rect.left + 5, rect.top + 2, "CONNECTION LOG:", 15);

	int line = MAX_LOGCONNECT_TEXT_LINE;

	int count = (((this->m_countConnect - 1) >= 0) ? (this->m_countConnect - 1) : (MAX_LOGCONNECT_TEXT_LINE - 1));

	for (int n = 0; n < MAX_LOGCONNECT_TEXT_LINE; n++)
	{
		switch (this->m_logConnect[count].color)
		{
		case LOG_BLACK:
#if(GAMESERVER_NOMBRE == 1)
			SetTextColor(hdc, RGB(0, 0, 0));
#else
			SetTextColor(hdc, RGB(255, 255, 255));
#endif
			break;
		case LOG_RED:
			SetTextColor(hdc, RGB(255, 0, 0));
			break;
		case LOG_GREEN:
			SetTextColor(hdc, RGB(0, 190, 0));
			break;
		case LOG_BLUE:
			SetTextColor(hdc, RGB(0, 0, 255));
			break;
		case LOG_BOT:
			SetTextColor(hdc, RGB(255, 0, 64));
			SetBkMode(hdc, TRANSPARENT);
			break;
		case LOG_USER:
			SetTextColor(hdc, RGB(254, 154, 46));
			SetBkMode(hdc, TRANSPARENT);
			break;
		case LOG_EVENT:
			SetTextColor(hdc, RGB(0, 102, 204));
			SetBkMode(hdc, TRANSPARENT);
			break;
		case LOG_ALERT:
			SetTextColor(hdc, RGB(0, 173, 181));
			SetBkMode(hdc, TRANSPARENT);
			break;
		}

		int size = strlen(this->m_logConnect[count].text);

		if (size > 1)
		{
			TextOut(hdc, rect.left + 10, (rect.top + 5 + (line * 15)), this->m_logConnect[count].text, size);
			line--;
		}

		count = (((--count) >= 0) ? count : (MAX_LOGCONNECT_TEXT_LINE - 1));
	}

	SelectObject(hdc, OldFont);
	SetBkMode(hdc, OldBkMode);
	ReleaseDC(this->m_hwnd, hdc);
}

void CServerDisplayer::LogTextPaintGlobalMessage() // OK
{
	RECT rect;

	GetClientRect(this->m_hwnd, &rect);

	rect.left = rect.right - 450;
	rect.right = rect.right - 150;
	rect.top = 295;
	rect.bottom = 440;

	HDC hdc = GetDC(this->m_hwnd);

	int OldBkMode = SetBkMode(hdc, TRANSPARENT);

	HFONT OldFont = (HFONT)SelectObject(hdc, this->m_font3);

	FillRect(hdc, &rect, this->m_brush[4]);

	SetTextColor(hdc, RGB(0, 173, 181));

	TextOut(hdc, rect.left + 5, rect.top + 2, "GLOBAL MESSAGE:", 15);

	int line = MAX_LOGGLOBAL_TEXT_LINE;

	int count = (((this->m_countGlobal - 1) >= 0) ? (this->m_countGlobal - 1) : (MAX_LOGGLOBAL_TEXT_LINE - 1));

	for (int n = 0; n < MAX_LOGGLOBAL_TEXT_LINE; n++)
	{

		SetTextColor(hdc, RGB(0, 190, 0));

		int size = strlen(this->m_logGlobal[count].text);

		if (size > 1)
		{
			TextOut(hdc, rect.left + 10, (rect.top + 5 + (line * 15)), this->m_logGlobal[count].text, size);
			line--;
		}

		count = (((--count) >= 0) ? count : (MAX_LOGGLOBAL_TEXT_LINE - 1));
	}

	SelectObject(hdc, OldFont);
	SetBkMode(hdc, OldBkMode);
	ReleaseDC(this->m_hwnd, hdc);
}

void CServerDisplayer::LogAddText(eLogColor color, char* text, int size) // OK
{
	PROTECT_START

		size = ((size >= MAX_LOG_TEXT_SIZE) ? (MAX_LOG_TEXT_SIZE - 1) : size);

	memset(&this->m_log[this->m_count].text, 0, sizeof(this->m_log[this->m_count].text));

	memcpy(&this->m_log[this->m_count].text, text, size);

	this->m_log[this->m_count].color = color;

	this->m_count = (((++this->m_count) >= MAX_LOG_TEXT_LINE) ? 0 : this->m_count);

	PROTECT_FINAL

		gLog.Output(LOG_GENERAL, "%s", &text[9]);
}

void CServerDisplayer::LogAddTextConnect(eLogColor color, char* text, int size) // OK
{
	PROTECT_START

		size = ((size >= MAX_LOGCONNECT_TEXT_SIZE) ? (MAX_LOGCONNECT_TEXT_SIZE - 1) : size);

	memset(&this->m_logConnect[this->m_countConnect].text, 0, sizeof(this->m_logConnect[this->m_countConnect].text));

	memcpy(&this->m_logConnect[this->m_countConnect].text, text, size);

	this->m_logConnect[this->m_countConnect].color = color;

	this->m_countConnect = (((++this->m_countConnect) >= MAX_LOGCONNECT_TEXT_LINE) ? 0 : this->m_countConnect);

	PROTECT_FINAL

		gLog.Output(LOG_GENERAL, "%s", &text[9]);
}

void CServerDisplayer::LogAddTextGlobal(eLogColor color, char* text, int size) // OK
{
	PROTECT_START

		size = ((size >= MAX_LOGGLOBAL_TEXT_SIZE) ? (MAX_LOGGLOBAL_TEXT_SIZE - 1) : size);

	memset(&this->m_logGlobal[this->m_countGlobal].text, 0, sizeof(this->m_logGlobal[this->m_countGlobal].text));

	memcpy(&this->m_logGlobal[this->m_countGlobal].text, text, size);

	this->m_logGlobal[this->m_countGlobal].color = color;

	this->m_countGlobal = (((++this->m_countGlobal) >= MAX_LOGGLOBAL_TEXT_LINE) ? 0 : this->m_countGlobal);

	PROTECT_FINAL
}