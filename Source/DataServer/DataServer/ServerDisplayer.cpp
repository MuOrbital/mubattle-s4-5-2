// ServerDisplayer.cpp: implementation of the CServerDisplayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ServerDisplayer.h"
#include "Log.h"
#include "Protect.h"
#include "ServerManager.h"
#include "SocketManager.h"

CServerDisplayer gServerDisplayer;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServerDisplayer::CServerDisplayer() // OK
{
	for (int n = 0; n < MAX_LOG_TEXT_LINE; n++)
	{
		memset(&this->m_log[n], 0, sizeof(this->m_log[n]));
	}

	this->m_font = CreateFont(50, 0, 0, 0, FW_THIN, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Times");

	this->m_brush[LOG_INACTIVE_BRUSH] = CreateSolidBrush(RGB(90, 90, 90));
	this->m_brush[LOG_ACTIVE_BRUSH] = CreateSolidBrush(RGB(0, 200, 120));
	this->m_brush[LOG_NAME_BRUSH] = CreateSolidBrush(RGB(30, 30, 36));
	this->m_brush[LOG_MAIN_BRUSH] = CreateSolidBrush(RGB(18, 18, 22));


	strcpy_s(this->m_DisplayerText[0], "STANDBY MODE");
	strcpy_s(this->m_DisplayerText[1], "ACTIVE MODE");
	m_hBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1));
}

CServerDisplayer::~CServerDisplayer() // OK
{
	DeleteObject(this->m_font);
	DeleteObject(this->m_brush[LOG_INACTIVE_BRUSH]);
	DeleteObject(this->m_brush[LOG_ACTIVE_BRUSH]);
	DeleteObject(this->m_brush[LOG_NAME_BRUSH]);
	DeleteObject(this->m_brush[LOG_MAIN_BRUSH]);

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

	PROTECT_FINAL

		gLog.AddLog(1, "LOG");
}

void CServerDisplayer::Run() // OK
{
	this->LogTextPaint();

	this->PaintName();

	this->SetWindowName();

	this->PaintAllInfo();
}

void CServerDisplayer::SetWindowName() // OK
{
	char buff[256];

	wsprintf(buff, "[%s] %s DataServer (QueueSize : %d)", DATASERVER_VERSION, DATASERVER_CLIENT, gSocketManager.GetQueueSize());

	SetWindowText(this->m_hwnd, buff);
}

void CServerDisplayer::PaintAllInfo() // OK
{
	RECT rect;

	GetClientRect(this->m_hwnd, &rect);

	int PosY = 150;
	rect.top = PosY;
	rect.bottom = PosY + 50;

	HDC hdc = GetDC(this->m_hwnd);

	int OldBkMode = SetBkMode(hdc, TRANSPARENT);

	HFONT OldFont = (HFONT)SelectObject(hdc, this->m_font);

	int state = 0;

	for (int n = 0; n < MAX_SERVER; n++)
	{
		if (gServerManager[n].CheckState() == 0)
		{
			continue;
		}

		if ((GetTickCount() - gServerManager[n].m_PacketTime) <= 60000)
		{
			state = 1;
			break;
		}
	}

	if (state == 0)
	{
		SetTextColor(hdc, RGB(200, 200, 200));
		FillRect(hdc, &rect, this->m_brush[0]);
		TextOut(hdc, 120, PosY, this->m_DisplayerText[0], strlen(this->m_DisplayerText[0]));
	}
	else
	{
		SetTextColor(hdc, RGB(250, 250, 250));
		FillRect(hdc, &rect, this->m_brush[1]);
		TextOut(hdc, 150, PosY, this->m_DisplayerText[1], strlen(this->m_DisplayerText[1]));
	}

	SelectObject(hdc, OldFont);
	SetBkMode(hdc, OldBkMode);
	ReleaseDC(this->m_hwnd, hdc);
}

void CServerDisplayer::PaintName() // OK
{
	if (m_hBitmap != nullptr)
	{
		HDC hdc = GetDC(this->m_hwnd);
		HDC  hdcMem = CreateCompatibleDC(hdc);
		HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMem, m_hBitmap); // Usar m_hBitmap

		StretchBlt(hdc, 0, 0, 600, 150, hdcMem, 0, 0, 600, 150, SRCCOPY);
		SelectObject(hdcMem, hBitmapOld);
		DeleteDC(hdcMem);
		ReleaseDC(this->m_hwnd, hdc);
	}
}

void CServerDisplayer::LogTextPaint() // OK
{
	RECT rect;

	GetClientRect(this->m_hwnd, &rect);

	rect.top = 0;

	HDC hdc = GetDC(this->m_hwnd);

	FillRect(hdc, &rect, m_brush[LOG_MAIN_BRUSH]);

	int line = 0;

	int count = (((this->m_count - 1) >= 0) ? (this->m_count - 1) : (MAX_LOG_TEXT_LINE - 1));

	int RenderFrameY = rect.bottom - 18;

	for (int n = 0; n < MAX_LOG_TEXT_LINE; n++)
	{
		SetBkMode(hdc, TRANSPARENT);
		switch (this->m_log[count].color)
		{
		case LOG_BLACK:
			SetTextColor(hdc, CLRREF_WHITE);
			break;
		case LOG_RED:
			SetTextColor(hdc, CLRREF_RED);
			break;
		case LOG_GREEN:
			SetTextColor(hdc, CLRREF_GREEN);
			break;
		case LOG_BLUE:
			SetTextColor(hdc, CLRREF_BLUE);
			break;
		}

		int size = strlen(this->m_log[count].text);

		if (size > 1 && (line * 15) <= (rect.bottom - 18) && count < MAX_LOG_TEXT_LINE)
		{
			TextOut(hdc, 0, RenderFrameY, this->m_log[count].text, size);
			RenderFrameY -= 15;
			line++;
		}

		count = (((--count) >= 0) ? count : (MAX_LOG_TEXT_LINE - 1));
	}

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
