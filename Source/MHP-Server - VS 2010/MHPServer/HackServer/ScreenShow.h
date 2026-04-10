#pragma once
#include <windows.h>
#include "HackServerProtocol.h"

struct PMSG_SCREEN_RECV
{
	PWMSG_HEAD header; //
	int count;
	int bufferSize;
	int index;
};

struct PMSG_SCREEN
{
	BYTE value;
};

struct PMSG_SCREENSTATE_SEND
{
	PBMSG_HEAD header;
	int value;
	int quality;
	BYTE display_index;
	short server_screen_width;
	short server_screen_height;
};

struct PMSG_SCREENDATA_RECV
{
	PBMSG_HEAD header;
	int Width;
	int Height;
	BYTE DisplayCount;
};

class CScreenShow
{
public:
	CScreenShow();
	virtual ~CScreenShow();
	// ----
	void Init();
	void RecvInfo(PMSG_SCREEN_RECV* lpMsg, int aIndex);
	static void DrawScreen(HWND hWnd, HDC hdc);
	void SendScreenState(int aIndex, int state);
	void RecvScreenData(PMSG_SCREENDATA_RECV* lpMsg,int aIndex);
	//BYTE TotalPicture[6220800];
	std::vector<unsigned char> buffer;
	std::vector<unsigned char> workbuffer;
	int TotalAdded;
	bool WindowOpen;
	HWND ScreenWnd;
	HDC ScreenDC;
	HINSTANCE hInst;
	bool isNeedReDraw;
	int WorkIndex;
	// ----
	int ScreenWidth;
	int ScreenHeight;
	// ----
	int FPS;
	int RealFPS;
	DWORD Tick;
	int ScreenQuality;
	int ScreenDiff;

	BYTE display_count;
	BYTE current_display;

	int display_width;
	int display_height;
private:
}; extern CScreenShow gScreenShow;
