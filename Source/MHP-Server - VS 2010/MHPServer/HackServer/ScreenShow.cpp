#include "stdafx.h"
#include "ScreenShow.h"
#include "Util.h"
#include <fstream>
#include "SocketManager.h"
#include "Resource.h"
#include <windowsx.h>

CScreenShow gScreenShow;

CScreenShow::CScreenShow()
{
	this->Init();
	HANDLE h_thread = CreateThread(nullptr,0,LPTHREAD_START_ROUTINE(gScreenShow.DrawScreen),nullptr,0,nullptr);
	SetThreadPriority(h_thread,THREAD_PRIORITY_HIGHEST);
	this->WindowOpen = false;
	this->ScreenWnd = nullptr;
	this->buffer.clear();
	this->workbuffer.clear();
	this->isNeedReDraw = false;
	this->WorkIndex = -1;
	this->FPS = 0;
	this->RealFPS = 0;
	this->Tick = GetTickCount();
	this->ScreenQuality = 100;
	this->ScreenDiff = 0;
	this->display_count = 0;
	this->current_display = 0;

	HDC DevC = GetDC(nullptr);
	this->display_width = GetDeviceCaps(DevC, DESKTOPHORZRES);
	this->display_height = GetDeviceCaps(DevC, DESKTOPVERTRES);
}

CScreenShow::~CScreenShow()
{
	
}

void CScreenShow::Init()
{
	this->buffer.clear();
	this->TotalAdded = 0;
}

void CScreenShow::RecvInfo(PMSG_SCREEN_RECV* lpMsg, int aIndex)
{
	if (this->WorkIndex != aIndex)
	{
		return;
	}

	static int lastindex = 0;

	if (lpMsg->index == 0)
	{
		lastindex=0;
		this->Init();
	}

	lastindex++;
	
	for(int n=0;n < lpMsg->count;n++)
	{
		PMSG_SCREEN* lpInfo = (PMSG_SCREEN*)(((BYTE*)lpMsg)+sizeof(PMSG_SCREEN_RECV)+(sizeof(PMSG_SCREEN)*n));
		this->buffer.push_back(lpInfo->value);
		this->TotalAdded++;

		if (this->buffer.size() == lpMsg->bufferSize)
		{
			this->workbuffer.clear();
			this->workbuffer = this->buffer;

			this->isNeedReDraw = true;

			if (GetTickCount() - this->Tick < 1000)
			{
				this->FPS++;
			}
			else
			{
				this->Tick = GetTickCount();
				this->RealFPS = FPS;
				this->FPS = 0;

				/*if (RealFPS<20)
				{
					if (this->ScreenQuality>30)
					{
						this->ScreenQuality-=10;
						this->SendScreenState(this->WorkIndex,1);
					}
					
				}
				else if (RealFPS > 20)
				{
					if (this->ScreenQuality < 100)
					{
						this->ScreenQuality+=10;
						this->SendScreenState(this->WorkIndex,1);
					}
				}*/
			}
		}
		else if (this->buffer.size() >= lpMsg->bufferSize)
		{
			LogAdd(LOG_RED,"[ScreenShow] ¡ERROR! ¡La longitud del búfer es mayor que el tamaño esperado!");
			break;
		}
	}
}

bool load_img(std::vector<unsigned char> & buf, Bitmap *image)
{
    UINT len = buf.size();
    HGLOBAL hMem = GlobalAlloc(GMEM_FIXED, len);
    BYTE *pmem = (BYTE*)GlobalLock(hMem);
    memcpy(pmem, &buf[0], len);
    IStream    *stream = nullptr;
    CreateStreamOnHGlobal(hMem, FALSE, &stream);
    //image.FromStream(stream);
	//image = new Bitmap(stream);
    stream->Release();
    GlobalUnlock(hMem);
    GlobalFree(hMem);
    return true;
}  

int GetEncoderClsid(WCHAR *format, CLSID *pClsid) {
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

IStream* CreateStreamOnResource(LPCTSTR lpName, LPCTSTR lpType)
{
    IStream * ipStream = nullptr;

    HRSRC hrsrc = FindResource(nullptr, lpName, lpType);
    if (hrsrc == nullptr)
        goto Return;

    DWORD dwResourceSize = SizeofResource(nullptr, hrsrc);
    HGLOBAL hglb_image = LoadResource(nullptr, hrsrc);
    if (hglb_image == nullptr)
        goto Return;

    LPVOID pvSourceResourceData = LockResource(hglb_image);
    if (pvSourceResourceData == nullptr)
        goto Return;

    HGLOBAL hgbl_resource_data = GlobalAlloc(GMEM_MOVEABLE, dwResourceSize);
    if (hgbl_resource_data == nullptr)
        goto Return;

    LPVOID pvResourceData = GlobalLock(hgbl_resource_data);

    if (pvResourceData == nullptr)
        goto FreeData;

    CopyMemory(pvResourceData, pvSourceResourceData, dwResourceSize);

    GlobalUnlock(hgbl_resource_data);

    if (SUCCEEDED(CreateStreamOnHGlobal(hgbl_resource_data, TRUE, &ipStream)))
        goto Return;

FreeData:
    GlobalFree(hgbl_resource_data);

Return:
    return ipStream;
}

void CScreenShow::DrawScreen(HWND hWnd, HDC hdc)
{
	if (!gScreenShow.WindowOpen)
	{
		return;
	}

	if (gScreenShow.TotalAdded==0)
	{
		return;
	}

	if (gScreenShow.workbuffer.size() == 0)
	{
		return;
	}

	if (gScreenShow.isNeedReDraw == false)
	{
		return;
	}

	const int start_y = 30;
	
	gScreenShow.ScreenWnd = hWnd;
	gScreenShow.ScreenDC = hdc;

	UINT len = gScreenShow.workbuffer.size();
	HGLOBAL hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE *pmem = (BYTE*)GlobalLock(hMem);
	memcpy(pmem, &gScreenShow.workbuffer[0], len);
	IStream    *stream = nullptr;
	CreateStreamOnHGlobal(hMem, FALSE, &stream);
	CImage image;
	image.Load(stream);
	stream->Release();
	GlobalUnlock(hMem);
	GlobalFree(hMem);

	//image.Save("image.jpg",ImageFormatJPEG);

	CImage LogoImage;
	LogoImage.Load("logo.png");

	wchar_t text[50];
	wchar_t text1[50];

	swprintf(text,L"FPS: %d, Conteo de pantalla: %d" ,gScreenShow.RealFPS, gScreenShow.display_count);
	swprintf(text1,L"Calidad de imagen: %d",gScreenShow.ScreenQuality);

	Gdiplus::Graphics graphics ( image.GetDC () );
	FontFamily font_family(L"Arial");
	Gdiplus::Font font ( &font_family, 16 );
	Gdiplus::SolidBrush brush ( Color ( 255, 0, 0, 255 ) );
	graphics.DrawString ( text, -1, &font, PointF(0.0f, 0.0f+start_y), &brush );
	graphics.DrawString ( text1, -1, &font, PointF(0.0f, 19.0f+start_y), &brush );
	image.ReleaseDC();
	//image.Draw(hdc,0,0,gScreenShow.ScreenWidth,gScreenShow.ScreenHeight,0,0,gScreenShow.ScreenWidth,gScreenShow.ScreenHeight+gScreenShow.ScreenDiff);
	//image.Draw(hdc,0,0,gScreenShow.ScreenWidth,gScreenShow.ScreenHeight,0,0,2560,1440);

	image.Draw(hdc,0,start_y,gScreenShow.ScreenWidth,gScreenShow.ScreenHeight,0,0,gScreenShow.ScreenWidth,gScreenShow.ScreenHeight+gScreenShow.ScreenDiff);


//	RECT    rcCli;          
//GetClientRect(WindowFromDC(hdc), &rcCli);
//// then you might have: 
//long nWidth = rcCli.right-rcCli.left; 
//long nHeight  = rcCli.bottom-rcCli.top;
//	
//	image.Draw(hdc,0,0,nWidth,nHeight,0,0,nWidth,nHeight);

	//image.StretchBlt(hdc,0,0,gScreenShow.ScreenWidth,gScreenShow.ScreenHeight/*-30*//*image.GetWidth(),image.GetHeight()*/,SRCCOPY);
	LogoImage.StretchBlt(hdc,gScreenShow.ScreenWidth - LogoImage.GetWidth()-6-10,10+start_y,LogoImage.GetWidth(),LogoImage.GetHeight()/*image.GetWidth(),image.GetHeight()*/,SRCCOPY);

	image.Destroy();
	LogoImage.Destroy();

	gScreenShow.isNeedReDraw = false;
}

void CScreenShow::SendScreenState(int aIndex, int state)
{
	if (state == 1)
	{
		this->WorkIndex = aIndex;
	}
	else
	{
		this->current_display = 0;
		this->WorkIndex = -1;
		this->ScreenQuality = 100;
		this->ScreenDiff = 0;
	}

	PMSG_SCREENSTATE_SEND pMsg;

	pMsg.header.set(0x10,sizeof(pMsg));

	pMsg.value = state;

	pMsg.quality = this->ScreenQuality;

	pMsg.display_index = this->current_display;

	pMsg.server_screen_width = GetDeviceCaps(GetDC(nullptr), DESKTOPHORZRES);
	pMsg.server_screen_height = GetDeviceCaps(GetDC(nullptr), DESKTOPVERTRES);

	gSocketManager.DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CScreenShow::RecvScreenData(PMSG_SCREENDATA_RECV* lpMsg, int aIndex)
{
	if (aIndex != this->WorkIndex)
	{
		return;
	}

	HDC hDCScreen =  GetDC(NULL);
	int Horres = GetDeviceCaps(hDCScreen,  HORZRES);
	int Vertres = GetDeviceCaps(hDCScreen,  VERTRES);
	ReleaseDC(NULL, hDCScreen);

	if (Horres > lpMsg->Width && Vertres > lpMsg->Height)
	{
		this->ScreenWidth = lpMsg->Width;
		this->ScreenHeight = lpMsg->Height;
	}
	else
	{
		this->ScreenWidth = Horres;
		this->ScreenHeight = Vertres;
		this->ScreenDiff = 100;
	}

	this->display_count = lpMsg->DisplayCount;

	HWND hCombo = GetDlgItem(this->ScreenWnd, IDC_DISPLAYS);
	//SendMessage(hCombo, CB_SETMINVISIBLE, 10, NULL);

	SendMessage(hCombo, CB_RESETCONTENT, 0, NULL);
	
	for(int i=0;i<gScreenShow.display_count;i++)
	{
		char text[64];
		sprintf(text,"Mostrar: #%d",i+1);
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)text);
	}

	ComboBox_SetCurSel(hCombo,0);

	this->WindowOpen = true;
	MoveWindow(this->ScreenWnd,0,0,this->ScreenWidth,this->ScreenHeight+30,1);
	ShowWindow(ScreenWnd, SW_SHOWNORMAL);
}
