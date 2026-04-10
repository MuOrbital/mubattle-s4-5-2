#pragma once
#include "ScreenCapture.h"

struct PMSG_SCREEN_SHOT_SEND
{
	PWMSG_HEAD header; //
	int count;
	int bufferSize;
	int index;
};

class CScreenShot : CScreenCapture
{
public:
	CScreenShot();
	void SendScreenShot();
	// ----
	DWORD delay;
}; extern CScreenShot gScreenShot;