#pragma once
#include "HackServerProtocol.h"

struct PMSG_SCREEN_SEND
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

struct PMSG_SCREENSTATE_RECV
{
	PBMSG_HEAD header;
	int value;
	int quality;
	BYTE display_index;
	short server_screen_width;
	short server_screen_height;
};

struct PMSG_SCREENDATA_SEND
{
	PBMSG_HEAD header;
	int Width;
	int Height;
	BYTE DisplayCount;
};

class CScreenCapture
{
public:
	CScreenCapture();
	virtual ~CScreenCapture();
	// ----
	static int get_encoder_clsid(WCHAR *format, CLSID *pClsid);
	static bool getDesktopScreenShot(std::vector<unsigned char> & buffer, ULONG Quality);
	void CaptureScreen();
	static void SendScreenShot(std::vector<unsigned char> & buffer);
	void RecvSetState(PMSG_SCREENSTATE_RECV* lpMsg);
	// ----
	int State;
	int m_Quality;
	int monitor_index;
	short server_screen_width;
	short server_screen_height;
	std::vector<unsigned char> screen_buffer;
	// ---
}; extern CScreenCapture gScreenCapture;