// ServerDisplayer.h: interface for the CServerDisplayer class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_LOG_TEXT_LINE								28
#define MAX_LOG_TEXT_SIZE								80

#define	CLRREF_BLACK									RGB(0, 0, 0)
#define	CLRREF_RED										RGB(255, 0, 0)
#define	CLRREF_GREEN									RGB(0, 190, 0)
#define	CLRREF_BLUE										RGB(0,  102, 204)
#define	CLRREF_WHITE									RGB(255, 255, 255)
#define LOG_INACTIVE_BRUSH								0
#define LOG_ACTIVE_BRUSH								1
#define LOG_NAME_BRUSH									2
#define LOG_CONTAINER_BRUSH								3
#define LOG_MAIN_BRUSH									3


enum eLogColor
{
	LOG_BLACK = 0,
	LOG_RED = 1,
	LOG_GREEN = 2,
	LOG_BLUE = 3,
};

struct LOG_DISPLAY_INFO
{
	char text[MAX_LOG_TEXT_SIZE];
	eLogColor color;
};

class CServerDisplayer
{
public:
	CServerDisplayer();
	virtual ~CServerDisplayer();
	void Init(HWND hWnd);
	void Run();
	void SetWindowName();
	void PaintAllInfo();
	void PaintName();
	void LogTextPaint();
	void LogAddText(eLogColor color, char* text, int size);
private:
	HWND m_hwnd;
	HFONT m_font;
	HBRUSH m_brush[4];
	HBITMAP m_hBitmap;
	LOG_DISPLAY_INFO m_log[MAX_LOG_TEXT_LINE];
	int m_count;
	char m_DisplayerText[2][64];
};

extern CServerDisplayer gServerDisplayer;
