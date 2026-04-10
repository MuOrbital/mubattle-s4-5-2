// ServerDisplayer.cpp: implementation of the CServerDisplayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerDisplayer.h"
#include "ClientManager.h"
#include "Log.h"
#include "Util.h"
#include "Protection.h"

CServerDisplayer gServerDisplayer;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServerDisplayer::CServerDisplayer() // OK
{
	for(int n=0;n < MAX_LOG_TEXT_LINE;n++)
	{
		memset(&this->m_log[n],0,sizeof(this->m_log[n]));
	}

	this->m_font = CreateFontA(40,0,0,0,FW_REGULAR,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"Times");

	this->m_brush[0] = CreateSolidBrush(RGB(49,50,48,255));
	this->m_brush[1] = CreateSolidBrush(RGB(0,134,174,255));
	this->m_brush[2] = CreateSolidBrush(RGB(25,25,25));
	this->m_brush[3] = CreateSolidBrush(RGB(0,134,174,255));

	strcpy_s(this->m_DisplayerText[0],"Anti-Hack OffLine");
	strcpy_s(this->m_DisplayerText[1],"Anti-Hack Online");
}

CServerDisplayer::~CServerDisplayer() // OK
{
	DeleteObject(this->m_font);
	DeleteObject(this->m_brush[0]);
	DeleteObject(this->m_brush[1]);
	DeleteObject(this->m_brush[2]);
}

void CServerDisplayer::Init(HWND hWnd) // OK
{
	this->m_hwnd = hWnd;

	this->Interface(this->m_hwnd);

	gLog.AddLog(1,"LOG");

	gLog.AddLog(1,"LOG\\HACK_LOG");
}

void CServerDisplayer::Run() // OK
{
	this->Interface(this->m_hwnd);
	this->SetWindowName();
	this->LogTextPaint();
	this->PaintAllInfo();
}

void CServerDisplayer::SetWindowName() // OK
{
	char buff[256];

	wsprintf(buff,"Antihack Server (Version: %s)  (UserON : %d)",HACKSERVER_VERSION,GetUserCount());

	SetWindowText(this->m_hwnd,buff);
}

void CServerDisplayer::PaintAllInfo() // OK
{
	RECT rect;

	GetClientRect(this->m_hwnd,&rect);

	rect.top = 0;
	rect.bottom = 70;

	HDC hdc = GetDC(this->m_hwnd);

	int OldBkMode = SetBkMode(hdc,TRANSPARENT);

	HFONT OldFont = (HFONT)SelectObject(hdc,this->m_font);

	int state = 0;

	for(int n=0;n < MAX_CLIENT;n++)
	{
		if(gClientManager[n].CheckState() == 0)
		{
			continue;
		}

		if((GetTickCount()-gClientManager[n].m_PacketTime) <= 60000)
		{
			state = 1;
			break;
		}
	}

	if(state == 0)
	{
		SetTextColor(hdc,RGB(200,200,200));
		FillRect(hdc,&rect,this->m_brush[0]);
		TextOut(hdc,160,20,this->m_DisplayerText[0],strlen(this->m_DisplayerText[0]));
	}
	else
	{
		SetTextColor(hdc,RGB(250,250,250));
		FillRect(hdc,&rect,this->m_brush[1]);
		TextOut(hdc,160,20,this->m_DisplayerText[1],strlen(this->m_DisplayerText[1]));
	}

	SelectObject(hdc,OldFont);
	SetBkMode(hdc,OldBkMode);
	ReleaseDC(this->m_hwnd,hdc);
}

void CServerDisplayer::LogTextPaint() // OK
{ 
    HDC hdc = GetDC(this->m_hwnd);
	
	this->Interface(this->m_hwnd);
 
    int line = MAX_LOG_TEXT_LINE;
 
    int count = (((this->m_count-1)>=0)?(this->m_count-1):(MAX_LOG_TEXT_LINE-1));

	SetBkColor(hdc, BACKCOLOR);
 
    for(int n=0;n < MAX_LOG_TEXT_LINE;n++)
    {
        switch(this->m_log[count].color)
        {
            case LOG_BLACK:
                //SetTextColor(hdc, RGB(0, 0, 0));
                SetTextColor(hdc, RGB(127, 255, 212));
                SetBkColor(hdc, BACKCOLOR);
                break;
            case LOG_RED:
                SetTextColor(hdc, RGB(255, 0, 0));
                SetBkColor(hdc, BACKCOLOR);
                break;
            case LOG_GREEN:
                SetTextColor(hdc, RGB(0, 255, 0));
                SetBkColor(hdc, BACKCOLOR);
                break;
            case LOG_BLUE:
                SetTextColor(hdc, RGB(0, 100, 255));
                SetBkColor(hdc, BACKCOLOR);
                break;
            default: ;
        }
 
        int size = strlen(this->m_log[count].text);
 
        if(size > 1)
        {
            TextOut(hdc,0,(65+(line*15)),this->m_log[count].text,size);
            line--;
        }
 
        count = (((--count)>=0)?count:(MAX_LOG_TEXT_LINE-1));
    }
 
    ReleaseDC(this->m_hwnd,hdc);
}

void CServerDisplayer::LogAddText(eLogColor color,char* text,int size) // OK
{
	size = ((size>=MAX_LOG_TEXT_SIZE)?(MAX_LOG_TEXT_SIZE-1):size);

	memset(&this->m_log[this->m_count].text,0,sizeof(this->m_log[this->m_count].text));

	memcpy(&this->m_log[this->m_count].text,text,size);

	this->m_log[this->m_count].color = color;

	this->m_count = (((++this->m_count)>=MAX_LOG_TEXT_LINE)?0:this->m_count);

	gLog.Output(LOG_GENERAL,"%s",&text[9]);
}

void CServerDisplayer::Interface(HWND hWnd)
{
    HDC hDC = GetDC(this->m_hwnd);
    RECT rect;
    // ----
    GetClientRect(this->m_hwnd, &rect);
    FillRect(hDC, &rect, this->m_brush[2]);
    SetBkMode(hDC, TRANSPARENT);
    ReleaseDC(this->m_hwnd, hDC);
}