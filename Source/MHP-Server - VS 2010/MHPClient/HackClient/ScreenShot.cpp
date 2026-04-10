#include "stdafx.h"
#include "ScreenShot.h"
#include "Util.h"
#include "Log.h"
#include "Connection.h"
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

CScreenShot gScreenShot;

CScreenShot::CScreenShot()
{
	this->delay = GetTickCount() - 580000;
}

void CScreenShot::SendScreenShot()
{
	if(*(DWORD*)(MAIN_SCREEN_STATE) != 5)
	{
		return;
	}

	std::vector<unsigned char> buffer;

	ULONG_PTR gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	
	//this->getDesktopScreenShot(buffer,100);
	if (!this->getDesktopScreenShot(buffer,100) || buffer.size() == 0)
	{
		gLog.Output(LOG_DEBUG,"[ScreenShot] SOMETHING WENT WRONG. Terrible sorry...");
		return;
	}

	//char characterName[11];

	//gLog.Output(LOG_DEBUG,"[ScreenShot] size = %d ",buffer.size());

	GdiplusShutdown(gdiplusToken);

	int SendIndex = 0;
	int i=0;
	for(i = 0; i < buffer.size();)
	{
		BYTE send[8192*2];

		PMSG_SCREEN_SHOT_SEND pMsg;

		char *acc = (char*)0x0E8C260;

		//memcpy(pMsg.name,(void*)(*(DWORD*)(gUserStruct)+0x00),sizeof(pMsg.name));
		//memcpy(pMsg.account,acc,sizeof(pMsg.account));

		pMsg.header.set(0x15/*+(rand()%10)*/,0);

		int size = sizeof(pMsg);

		pMsg.count = 0;
		pMsg.bufferSize = buffer.size();
		pMsg.index = SendIndex++;

		PMSG_SCREEN info;

		for(int j = 0; j < 8192*2-50; j ++)
		{
			if (i>=buffer.size())
			{
				break;
			}

			info.value = buffer[i];

			memcpy(&send[size],&info,sizeof(info));
			size += sizeof(info);

			pMsg.count++;
			i++;
		}

		pMsg.header.size[0] = SET_NUMBERHB(size);

		pMsg.header.size[1] = SET_NUMBERLB(size);

		memcpy(send,&pMsg,sizeof(pMsg));

		while(true)
		{
			if (gConnection.CheckSendSize() > 0)
			{
				Sleep(100);
			}
			else
			{
				break;
			}
		}

		gLog.Output(LOG_DEBUG,"send index: %d",pMsg.index);

		bool state = gConnection.DataSend(send,size);

		gLog.Output(LOG_DEBUG,"send state: %d",state);
		
		//gLog.Output(LOG_DEBUG,"Size = %d!",size);
		//Sleep(3);
	}
}
