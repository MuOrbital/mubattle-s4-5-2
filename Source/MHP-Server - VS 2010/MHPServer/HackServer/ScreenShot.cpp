#include "stdafx.h"
#include "ScreenShot.h"
#include "ServerDisplayer.h"
#include "Util.h"

CScreenShot gScreenShot;

CScreenShot::CScreenShot()
{
	this->Init();
}


void CScreenShot::Init()
{
	this->buffer.clear();
	this->work_buffer.clear();
	this->TotalAdded = 0;
	CreateDirectoryA("ScreenShots", nullptr);
}


void CScreenShot::RecvInfo(PMSG_SCREEN_SHOT_SEND* lpMsg, int aIndex)
{
	static int lastindex = 0;

	if (lpMsg->index == 0)
	{
		lastindex = 0;
		this->Init();
	}

	lastindex++;

	for (int n = 0; n < lpMsg->count; n++)
	{
		PMSG_SCREEN* lpInfo = (PMSG_SCREEN*)(((BYTE*)lpMsg) + sizeof(PMSG_SCREEN_SHOT_SEND) + (sizeof(PMSG_SCREEN) * n));
		this->buffer.push_back(lpInfo->value);
		this->TotalAdded++;

		if (this->buffer.size() == lpMsg->bufferSize)
		{
			this->work_buffer.clear();
			this->work_buffer = this->buffer;

			this->SaveScreen("", "", aIndex);
		}
		else if (this->buffer.size() >= lpMsg->bufferSize)
		{
			LogAdd(LOG_GREEN, "ERROR! ¡La longitud del búfer es mayor que el tamaño esperado!");
		}
	}
}

void CScreenShot::SaveScreen(char* account, char* name, int aIndex)
{
	if (this->TotalAdded == 0)
	{
		return;
	}

	if (this->work_buffer.size() == 0)
	{
		return;
	}

	UINT len = this->work_buffer.size();
	HGLOBAL hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(hMem);
	memcpy(pmem, &this->work_buffer[0], len);
	IStream* stream = nullptr;
	CreateStreamOnHGlobal(hMem, FALSE, &stream);
	CImage image;
	image.Load(stream);
	stream->Release();
	GlobalUnlock(hMem);
	GlobalFree(hMem);

	//CImage LogoImage;
	//LogoImage.Load("logo.png");

	wchar_t text[50];
	wchar_t text1[50];
	wchar_t text2[50];
	wchar_t text3[50];

	swprintf(text, L"Account: %hs", account);
	swprintf(text1, L"Name: %hs", name);
	swprintf(text2, L"IP Address: %hs", GetIpByIndex(aIndex));
	swprintf(text3, L"Hwid: %hs", GetHwidByIndex(aIndex));

	Gdiplus::Graphics graphics(image.GetDC());

	auto font_family = new FontFamily(L"Arial");
	
	Gdiplus::Font font(font_family, 16);
	Gdiplus::SolidBrush brush(Color(255, 255, 255, 255));
	graphics.DrawString(text, -1, &font, PointF(0.0f, 0.0f), &brush);
	graphics.DrawString(text1, -1, &font, PointF(0.0f, 20.0f), &brush);
	graphics.DrawString(text2, -1, &font, PointF(0.0f, 40.0f), &brush);
	graphics.DrawString(text3, -1, &font, PointF(0.0f, 60.0f), &brush);
	image.ReleaseDC();

	SYSTEMTIME time;
	GetLocalTime(&time);

	char path[128];
	sprintf(path, "ScreenShots\\%s-%s\\%04d-%02d-%02d-%02d-%02d.jpg", account, name, time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute);

	char directory[128];
	sprintf(directory, "ScreenShots\\%s-%s", account, name);
	CreateDirectoryA(directory, nullptr);

	image.Save(path, ImageFormatJPEG);
	
	image.Destroy();
}
