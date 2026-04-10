#pragma once
#include "ScreenShow.h"

struct PMSG_SCREEN_SHOT_SEND
{
	PWMSG_HEAD header; //
	//char account[11];
	//char name[11];
	int count;
	int bufferSize;
	int index;
};

class CScreenShot
{
public:
	CScreenShot();
	void Init();
	void RecvInfo(PMSG_SCREEN_SHOT_SEND* lpMsg, int aIndex);
	void SaveScreen(char* account, char* name, int aIndex);
	// ----
	std::vector<unsigned char> buffer;
	std::vector<unsigned char> work_buffer;
	int TotalAdded;
}; extern CScreenShot gScreenShot;