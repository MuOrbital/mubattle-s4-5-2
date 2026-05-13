///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include <locale.h>
#include <zmouse.h>
#include "UIWindows.h"
#include "UIManager.h"
#include "ZzzOpenglUtil.h"
#include "ZzzTexture.h"
#include "ZzzOpenData.h"
#include "ZzzScene.h"
#include "ZzzBMD.h"
#include "ZzzInfomation.h"
#include "ZzzObject.h"
#include "ZzzCharacter.h"
#include "ZzzInterface.h"
#include "ZzzInventory.h"
#include "zzzLodTerrain.h"
#include "DSPlaySound.h"
#include "wsclientinline.h"
#include "Resource.h"
#include <imm.h>
#include "zzzpath.h"
#include "Nprotect.h"
#include "Local.h"
#include "PersonalShopTitleImp.h"
#include "./Utilities/Log/ErrorReport.h"
#include "UIMapName.h"		// rozy
#include "./ExternalObject/leaf/ExceptionHandler.h"
#include "./Utilities/Dump/CrashReporter.h"
#include "./Utilities/Log/muConsoleDebug.h"
#include "ProtocolSend.h"
#include "ProtectSysKey.h"
#include "CBTMessageBox.h"
#include "./ExternalObject/leaf/regkey.h"
#include "CSChaosCastle.h"
#include "GMHellas.h"
#include <io.h>
#include "Input.h"
#include "XboxInput.h"
#include "./Time/Timer.h"
#include "UIMng.h"
#ifdef MOVIE_DIRECTSHOW
#include <dshow.h>
#include "MovieScene.h"
#endif // MOVIE_DIRECTSHOW
#include "GameCensorship.h"
#include "w_MapHeaders.h"
#include "w_PetProcess.h"
#include <ThemidaInclude.h>
#include "MultiLanguage.h"
#include "Widescreen.h"
#include "Controller.h"
#include "HackCheck.h"
#include "Descriptions.h"
#include "CustomJewel.h"
#include "ServerName.h"
#pragma comment(lib, "glew32.lib")
#include "AutoClick.h"
#include "CustomWing.h"
#include "CustomEffects.h"
#include "MonsterGlow.h"
#include "MonsterName.h"
#include "RenderModel.h"
#include <rpc.h>
#include <rpcdce.h>
#pragma comment(lib, "rpcrt4.lib")
#include <dxgi.h>
#include <string>
#include <intrin.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <psapi.h>
#pragma comment(lib, "psapi.lib")

#pragma comment(lib,"ws2_32.lib")
#include <CustomSetEffect.h>
#include <DisableExcellent.h>
#include <ItemPosition.h>
#include <NewOptions.h>
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "wzAudio.lib")
#include <wzAudio.h>
#include <ServerListManager.h>
#include "Update/InGameUpdater.h"

#define WM_TRAYICON (WM_USER + 100)
#define ID_TRAYICON 1001
NOTIFYICONDATA g_nid = { 0 };
HICON g_hTrayIcon = NULL;
bool g_bInTray = false;
DWORD g_dwLastTrayFrameTime = 0;
DWORD g_dwLastTrayLogicTime = 0;
CUIMercenaryInputBox* g_pMercenaryInputBox = NULL;
CUITextInputBox* g_pSingleTextInputBox = NULL;
CUITextInputBox* g_pSinglePasswdInputBox = NULL;
int g_iChatInputType = 1;
extern BOOL g_bIMEBlock;
CChatRoomSocketList* g_pChatRoomSocketList = NULL;
CMultiLanguage* pMultiLanguage = NULL;
extern DWORD g_dwTopWindow;
#ifdef MOVIE_DIRECTSHOW
CMovieScene* g_pMovieScene = NULL;
#endif // MOVIE_DIRECTSHOW
CUIManager* g_pUIManager = NULL;
CUIMapName* g_pUIMapName = NULL;
int EffectDisable = 0;
int Time_Effect = 0;
bool ashies = false;
int weather = rand() % 3;
HWND      g_hWnd = NULL;
HINSTANCE g_hInst = NULL;
HDC       g_hDC = NULL;
HGLRC     g_hRC = NULL;
HFONT     g_hFont = NULL;
HFONT     g_hFontBold = NULL;
HFONT     g_hFontBig = NULL;
HFONT     g_hFixFont = NULL;
CTimer* g_pTimer = NULL;
bool      Destroy = false;
bool      ActiveIME = false;
BYTE* RendomMemoryDump;
ITEM_ATTRIBUTE* ItemAttRibuteMemoryDump;
CHARACTER* CharacterMemoryDump;
float DepthGeral = 4.2f;
int       RandomTable[100];
char TextMu[] = "mu.exe";
CErrorReport g_ErrorReport;
BOOL g_bMinimizedEnabled = FALSE;
int g_iScreenSaverOldValue = 60 * 15;
extern float g_fScreenRate_x;
extern float g_fScreenRate_y;
#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
BOOL g_bUseWindowMode = TRUE;
int g_bBorderless = 0;
#endif
char Mp3FileName[256];



void StopMp3(char* Name, BOOL bEnforce)
{
	if (!m_MusicOnOff && !bEnforce) return;

	if (Mp3FileName[0] != NULL)
	{
		if (strcmp(Name, Mp3FileName) == 0) {
			wzAudioStop();
			Mp3FileName[0] = NULL;
		}
	}
}

void PlayMp3(char* Name, BOOL bEnforce)
{
	if (Destroy) return;
	if (!m_MusicOnOff && !bEnforce) return;

	if (strcmp(Name, Mp3FileName) == 0)
	{
		return;
	}
	else
	{
		if (g_pOption->m_Music == false)
		{
			wzAudioPlay(Name, 1);
			strcpy(Mp3FileName, Name);
		}
	}
}

bool IsEndMp3()
{
	if (100 == wzAudioGetStreamOffsetRange())
		return true;
	return false;
}

int GetMp3PlayPosition()
{
	return wzAudioGetStreamOffsetRange();
}

extern int  LogIn;
extern char LogInID[];

void CheckHack(void)
{
#ifdef NEW_PROTOCOL_SYSTEM
	gProtocolSend.SendCheckOnline();
#else
	SendCheck();
#endif
}

GLvoid KillGLWindow(GLvoid)
{
	if (g_hRC)
	{
		if (!wglMakeCurrent(NULL, NULL))
		{
			g_ErrorReport.Write("GL - Release Of DC And RC Failed\r\n");
			MessageBox(NULL, "Release Of DC And RC Failed.", "Error", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(g_hRC))
		{
			g_ErrorReport.Write("GL - Release Rendering Context Failed\r\n");
			MessageBox(NULL, "Release Rendering Context Failed.", "Error", MB_OK | MB_ICONINFORMATION);
		}

		g_hRC = NULL;
	}

	if (g_hDC && !ReleaseDC(g_hWnd, g_hDC))
	{
		g_ErrorReport.Write("GL - OpenGL Release Error\r\n");
		MessageBox(NULL, "OpenGL Release Error.", "Error", MB_OK | MB_ICONINFORMATION);
		g_hDC = NULL;
	}

#if (defined WINDOWMODE)
	if (g_bUseWindowMode == FALSE)
	{
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}
#else
#ifdef ENABLE_FULLSCREEN
#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
	if (g_bUseWindowMode == FALSE)
#endif	// USER_WINDOW_MODE
	{
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}
#endif //ENABLE_FULLSCREEN
#endif	//WINDOWMODE(#else)
}


BOOL GetFileNameOfFilePath(char* lpszFile, char* lpszPath)
{
	int iFind = (int)'\\';
	char* lpFound = lpszPath;
	char* lpOld = lpFound;
	while (lpFound)
	{
		lpOld = lpFound;
		lpFound = strchr(lpFound + 1, iFind);
	}

	if (strchr(lpszPath, iFind))
	{
		strcpy(lpszFile, lpOld + 1);
	}
	else
	{
		strcpy(lpszFile, lpOld);
	}

	BOOL bCheck = TRUE;
	for (char* lpTemp = lpszFile; bCheck; ++lpTemp)
	{
		switch (*lpTemp)
		{
		case '\"':
		case '\\':
		case '/':
		case ' ':
			*lpTemp = '\0';
		case '\0':
			bCheck = FALSE;
			break;
		}
	}

	return (TRUE);
}

HANDLE g_hMainExe = INVALID_HANDLE_VALUE;
HANDLE g_hLimitSemaphore = NULL;

BOOL OpenMainExe(void)
{
#ifdef _DEBUG
	return (TRUE);
#endif
	char lpszFile[MAX_PATH];
	char* lpszCommandLine = GetCommandLine();
	GetFileNameOfFilePath(lpszFile, lpszCommandLine);

	g_hMainExe = CreateFile((char*)lpszFile, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	return (INVALID_HANDLE_VALUE != g_hMainExe);
}

void CloseMainExe(void)
{
	CloseHandle(g_hMainExe);
}

WORD DecryptCheckSumKey(WORD wSource)
{
	WORD wAcc = wSource ^ 0xB479;
	return ((wAcc >> 10) << 4) | (wAcc & 0xF);
}

DWORD GenerateCheckSum(BYTE* pbyBuffer, DWORD dwSize, WORD wKey)
{
	DWORD dwKey = (DWORD)wKey;
	DWORD dwResult = dwKey << 9;
	for (DWORD dwChecked = 0; dwChecked <= dwSize - 4; dwChecked += 4)
	{
		DWORD dwTemp;
		memcpy(&dwTemp, pbyBuffer + dwChecked, sizeof(DWORD));

		switch ((dwChecked / 4 + wKey) % 3)
		{
		case 0:
			dwResult ^= dwTemp;
			break;
		case 1:
			dwResult += dwTemp;
			break;
		case 2:
			dwResult <<= (dwTemp % 11);
			dwResult ^= dwTemp;
			break;
		}

		if (0 == (dwChecked % 4))
		{
			dwResult ^= ((dwKey + dwResult) >> ((dwChecked / 4) % 16 + 3));
		}
	}

	return (dwResult);
}

DWORD GetCheckSum(WORD wKey)
{
	wKey = DecryptCheckSumKey(wKey);

	char lpszFile[MAX_PATH];

	strcpy(lpszFile, "data\\local\\Gameguard.csr");

	HANDLE hFile = CreateFile((char*)lpszFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return (0);
	}

	DWORD dwSize = GetFileSize(hFile, NULL);
	BYTE* pbyBuffer = new BYTE[dwSize];
	DWORD dwNumber;
	ReadFile(hFile, pbyBuffer, dwSize, &dwNumber, 0);
	CloseHandle(hFile);

	DWORD dwCheckSum = GenerateCheckSum(pbyBuffer, dwSize, wKey);
	delete[] pbyBuffer;

	return (dwCheckSum);
}


BOOL GetFileVersion(char* lpszFileName, WORD* pwVersion)
{
	DWORD dwHandle;
	DWORD dwLen = GetFileVersionInfoSize(lpszFileName, &dwHandle);
	if (dwLen <= 0)
	{
		return (FALSE);
	}

	BYTE* pbyData = new BYTE[dwLen];
	if (!GetFileVersionInfo(lpszFileName, dwHandle, dwLen, pbyData))
	{
		delete[] pbyData;
		return (FALSE);
	}

	VS_FIXEDFILEINFO* pffi;
	UINT uLen;
	if (!VerQueryValue(pbyData, "\\", (LPVOID*)&pffi, &uLen))
	{
		delete[] pbyData;
		return (FALSE);
	}

	pwVersion[0] = HIWORD(pffi->dwFileVersionMS);
	pwVersion[1] = LOWORD(pffi->dwFileVersionMS);
	pwVersion[2] = HIWORD(pffi->dwFileVersionLS);
	pwVersion[3] = LOWORD(pffi->dwFileVersionLS);

	delete[] pbyData;
	return (TRUE);
}

extern PATH* path;

void DestroyWindow()
{
	leaf::CRegKey regkey;
	regkey.SetKey(leaf::CRegKey::_HKEY_CURRENT_USER, "SOFTWARE\\MuOnline\\Config");
	regkey.WriteDword("VolumeLevel", g_pOption->GetVolumeLevel());

	CUIMng::Instance().Release();

#ifdef MOVIE_DIRECTSHOW
	if (g_pMovieScene)
	{
		g_pMovieScene->Destroy();
	}
#endif // MOVIE_DIRECTSHOW

	//. release font handle
	if (g_hFont)
		DeleteObject((HGDIOBJ)g_hFont);

	if (g_hFontBold)
		DeleteObject((HGDIOBJ)g_hFontBold);

	if (g_hFontBig)
		DeleteObject((HGDIOBJ)g_hFontBig);

	if (g_hFixFont)
		::DeleteObject((HGDIOBJ)g_hFixFont);

	ReleaseCharacters();

	if (path != NULL)
	{
		delete path;
	}
	SAFE_DELETE(GateAttribute);

	for (int i = 0; i < MAX_SKILLS; ++i)
	{
	}
	SAFE_DELETE(SkillAttribute);

	SAFE_DELETE(CharacterMachine);

	DeleteWaterTerrain();

#ifdef MOVIE_DIRECTSHOW
	if (SceneFlag != MOVIE_SCENE)
#endif // MOVIE_DIRECTSHOW
	{
		gMapManager.DeleteObjects();

		for (int i = MODEL_LOGO; i < MAX_MODELS; i++)
		{
			Models[i].Release();
		}

		Bitmaps.UnloadAllImages();
	}

	SAFE_DELETE_ARRAY(CharacterMemoryDump);
	SAFE_DELETE_ARRAY(ItemAttRibuteMemoryDump);
	SAFE_DELETE_ARRAY(RendomMemoryDump);
	SAFE_DELETE_ARRAY(ModelsDump);

#ifdef DYNAMIC_FRUSTRUM
	DeleteAllFrustrum();
#endif //DYNAMIC_FRUSTRUM

	SAFE_DELETE(g_pMercenaryInputBox);
	SAFE_DELETE(g_pSingleTextInputBox);
	SAFE_DELETE(g_pSinglePasswdInputBox);

	SAFE_DELETE(g_pChatRoomSocketList);
	SAFE_DELETE(g_pUIMapName);	// rozy
	SAFE_DELETE(g_pTimer);
	SAFE_DELETE(g_pUIManager);

#ifdef MOVIE_DIRECTSHOW
	SAFE_DELETE(g_pMovieScene);
#endif // MOVIE_DIRECTSHOW

	SAFE_DELETE(pMultiLanguage);
	BoostRest(g_BuffSystem);
	BoostRest(g_MapProcess);
	BoostRest(g_petProcess);

	g_ErrorReport.Write("Destroy");

	HWND shWnd = FindWindow(NULL, "MuPlayer");
	if (shWnd)
		SendMessage(shWnd, WM_DESTROY, 0, 0);

	if (g_hLimitSemaphore)
	{
		ReleaseSemaphore(g_hLimitSemaphore, 1, NULL);
		CloseHandle(g_hLimitSemaphore);
		g_hLimitSemaphore = NULL;
	}
}
void DestroySound()
{
	for (int i = 0; i < MAX_BUFFER; i++)
		ReleaseBuffer(i);

	FreeDirectSound();
	wzAudioDestroy();
}

int g_iInactiveTime = 0;
int g_iNoMouseTime = 0;
int g_iInactiveWarning = 0;
bool g_bWndActive = false;
bool HangulDelete = false;
int Hangul = 0;
bool g_bEnterPressed = false;

int g_iMousePopPosition_x = 0;
int g_iMousePopPosition_y = 0;

extern int TimeRemain;
extern bool EnableFastInput;
void MainScene(HDC hDC);

void TrayAddIcon(HWND hWnd)
{
	if (g_hTrayIcon == NULL)
		g_hTrayIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ICON1));

	ZeroMemory(&g_nid, sizeof(g_nid));
	g_nid.cbSize = sizeof(NOTIFYICONDATA);
	g_nid.hWnd = hWnd;
	g_nid.uID = ID_TRAYICON;
	g_nid.uCallbackMessage = WM_TRAYICON;
	g_nid.hIcon = g_hTrayIcon;

	std::string tooltip = std::string(gProtect->m_MainInfo.WindowName) + "\r\n";

	if (Hero && CharacterAttribute && CharacterAttribute->Name[0] != '\0')
	{
		tooltip += std::string("Personagem: ") + CharacterAttribute->Name + "\r\n";
		tooltip += std::string("Level: ") + std::to_string(CharacterAttribute->Level) + "\r\n";
		tooltip += std::string("Resets: ") + std::to_string(CharacterAttribute->ViewResets) + "\r\n";

		if (gProtect->m_MainInfo.m_GetHWID != 0 && gProtect->m_MainInfo.m_ShowHWID != 0)
		{
			char LocalHardwareID[36] = { 0 };
			if (GetLocalComputerHardwareId(LocalHardwareID, sizeof(LocalHardwareID)))
			{
				tooltip += std::string("HWID: ") + LocalHardwareID + "\r\n";
			}
		}
	}
	else
	{
		tooltip += "Aguardando login...\r\n";
	}

	strncpy_s(g_nid.szTip, sizeof(g_nid.szTip), tooltip.c_str(), _TRUNCATE);

	std::string balloon = "";

	if (Hero && CharacterAttribute && CharacterAttribute->Name[0] != '\0')
	{
		balloon += std::string("Personagem: ") + CharacterAttribute->Name + "\r\n";
		balloon += std::string("Level: ") + std::to_string(CharacterAttribute->Level) + "\r\n";
		balloon += std::string("Resets: ") + std::to_string(CharacterAttribute->ViewResets) + "\r\n";

		if (gProtect->m_MainInfo.m_GetHWID != 0 && gProtect->m_MainInfo.m_ShowHWID != 0)
		{
			char LocalHardwareID[36] = { 0 };
			if (GetLocalComputerHardwareId(LocalHardwareID, sizeof(LocalHardwareID)))
			{
				balloon += std::string("HWID: ") + LocalHardwareID + "\r\n";
			}
		}
	}
	else
	{
		balloon = "Aguardando login...";
	}

	strncpy_s(g_nid.szInfoTitle, sizeof(g_nid.szInfoTitle), gProtect->m_MainInfo.WindowName, _TRUNCATE);
	strncpy_s(g_nid.szInfo, sizeof(g_nid.szInfo), balloon.c_str(), _TRUNCATE);

	g_nid.uTimeout = 7000;
	g_nid.dwInfoFlags = NIIF_INFO;
	g_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;

	Shell_NotifyIcon(NIM_ADD, &g_nid);
}

void TrayRemoveIcon()
{
	if (g_nid.hWnd)
		Shell_NotifyIcon(NIM_DELETE, &g_nid);
}

void TrayUpdateTooltip()
{
	if (!g_bInTray) return;

	std::string tooltip = std::string(gProtect->m_MainInfo.WindowName) + "\r\n";

	if (Hero && CharacterAttribute && CharacterAttribute->Name[0] != '\0')
	{
		tooltip += std::string("Personagem: ") + CharacterAttribute->Name + "\r\n";
		tooltip += std::string("Level: ") + std::to_string(CharacterAttribute->Level) + "\r\n";
		tooltip += std::string("Resets: ") + std::to_string(CharacterAttribute->ViewResets) + "\r\n";

		if (gProtect->m_MainInfo.m_GetHWID != 0 && gProtect->m_MainInfo.m_ShowHWID != 0)
		{
			char LocalHardwareID[36] = { 0 };
			if (GetLocalComputerHardwareId(LocalHardwareID, sizeof(LocalHardwareID)))
			{
				tooltip += std::string("HWID: ") + LocalHardwareID + "\r\n";
			}
		}
	}
	else
	{
		tooltip += "Aguardando login...\r\n";
	}

	strncpy_s(g_nid.szTip, sizeof(g_nid.szTip), tooltip.c_str(), _TRUNCATE);
	g_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;

	Shell_NotifyIcon(NIM_MODIFY, &g_nid);
}
LONG FAR PASCAL WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SYSKEYDOWN:
	{
		if (wParam == VK_F4 && (GetKeyState(VK_MENU) & 0x8000))
		{
			PostMessage(hwnd, WM_CLOSE, 0, 0);
			return 0;
		}

		if (wParam == VK_RETURN && (GetKeyState(VK_MENU) & 0x8000))
		{
			static bool isBorderless = (g_bBorderless == 1);
			isBorderless = !isBorderless;

			g_bBorderless = isBorderless ? 1 : 0;

			char szIniFilePath[256 + 20] = "";
			char szCurrentDir[256];
			GetCurrentDirectory(256, szCurrentDir);
			strcpy(szIniFilePath, szCurrentDir);
			if (szCurrentDir[strlen(szCurrentDir) - 1] == '\\')
				strcat(szIniFilePath, "config.ini");
			else
				strcat(szIniFilePath, "\\Data\\Custom\\config.ini");

			WritePrivateProfileString("AntiLagOptions", "Borderless",
				g_bBorderless ? "1" : "0", szIniFilePath);

			LONG windowStyle = GetWindowLong(hwnd, GWL_STYLE);
			RECT rcClient;
			GetClientRect(hwnd, &rcClient);

			if (isBorderless)
			{
				windowStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
				windowStyle |= WS_POPUP | WS_CLIPCHILDREN;
				AdjustWindowRect(&rcClient, WS_POPUP | WS_CLIPCHILDREN, FALSE);
			}
			else
			{
				windowStyle &= ~WS_POPUP;
				windowStyle |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER | WS_CLIPCHILDREN;
				AdjustWindowRect(&rcClient, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER | WS_CLIPCHILDREN, FALSE);
			}

			SetWindowLong(hwnd, GWL_STYLE, windowStyle);

			SetWindowPos(hwnd, HWND_TOP,
				(GetSystemMetrics(SM_CXSCREEN) - (rcClient.right - rcClient.left)) / 2,
				(GetSystemMetrics(SM_CYSCREEN) - (rcClient.bottom - rcClient.top)) / 2,
				rcClient.right - rcClient.left,
				rcClient.bottom - rcClient.top,
				SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);

			return 0;
		}
		return 0;
	}
	break;

	case WM_KEYDOWN:
	{
		if (wParam == VK_F12)
		{
			if (!g_bInTray)
			{
				ShowWindow(hwnd, SW_HIDE);
				TrayAddIcon(hwnd);
				g_bInTray = true;
				g_dwLastTrayFrameTime = GetTickCount();
				g_dwLastTrayLogicTime = GetTickCount();
			}
			else
			{
				ShowWindow(hwnd, SW_RESTORE);
				SetForegroundWindow(hwnd);
				TrayRemoveIcon();
				g_bInTray = false;
			}
			return 0;
		}
	}
	break;

	case WM_TRAYICON:
	{
		if (lParam == WM_LBUTTONDBLCLK || lParam == WM_LBUTTONUP)
		{
			if (g_bInTray)
			{
				ShowWindow(hwnd, SW_RESTORE);
				SetForegroundWindow(hwnd);
				TrayRemoveIcon();
				g_bInTray = false;
			}
		}
		return 0;
	}

#if defined PROTECT_SYSTEMKEY && defined NDEBUG
#ifndef FOR_WORK
	case WM_SYSCOMMAND:
	{
		if (wParam == SC_KEYMENU || wParam == SC_SCREENSAVE)
		{
			return 0;
		}
	}
	break;
#endif // !FOR_WORK
#endif // PROTECT_SYSTEMKEY && NDEBUG
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			// Não desativa o jogo quando perde foco (fullscreen continua rodando)
			// Apenas reseta os botões do mouse no Window Mode
#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
			if (g_bUseWindowMode == TRUE)
			{
				MouseLButton = false;
				MouseLButtonPop = false;
				MouseRButton = false;
				MouseRButtonPop = false;
				MouseRButtonPush = false;
				MouseLButtonDBClick = false;
				MouseMButton = false;
				MouseMButtonPop = false;
				MouseMButtonPush = false;
				MouseWheel = 0;
			}
#endif
		}
		else
		{
			g_bWndActive = true;
		}
		break;
	case WM_TIMER:
		//MessageBox(NULL,GlobalText[16],"Error",MB_OK);
		switch (wParam)
		{
		case HACK_TIMER:
			// PKD_ADD_BINARY_PROTECTION
			VM_START
				CheckHack();
			VM_END
				break;
		case WINDOWMINIMIZED_TIMER:
			PostMessage(g_hWnd, WM_CLOSE, 0, 0);
			break;
		case CHATCONNECT_TIMER:
			g_pFriendMenu->SendChatRoomConnectCheck();
			break;
		case SLIDEHELP_TIMER:
			if (g_bWndActive)
			{
				if (g_pSlideHelpMgr)
					g_pSlideHelpMgr->CreateSlideText();
			}
			break;
		}
		break;
	case WM_USER_MEMORYHACK:
		//SetTimer( g_hWnd, WINDOWMINIMIZED_TIMER, 1*1000, NULL);
		KillGLWindow();
		break;
	case WM_NPROTECT_EXIT_TWO:
		SendHackingChecked(0x04, 0);
		SetTimer(g_hWnd, WINDOWMINIMIZED_TIMER, 1 * 1000, NULL);
		MessageBox(NULL, GlobalText[16], "Error", MB_OK);
		break;
	case WM_ASYNCSELECTMSG:
		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_CONNECT:
			break;
		case FD_READ:
			SocketClient.nRecv();
			break;
		case FD_WRITE:
			SocketClient.FDWriteSend();
			break;
		case FD_CLOSE:
			g_pChatListBox->AddText("", GlobalText[3], SEASON3B::TYPE_SYSTEM_MESSAGE);
#ifdef CONSOLE_DEBUG
			switch (WSAGETSELECTERROR(lParam))
			{
			case WSAECONNRESET:
				g_ConsoleDebug->Write(MCD_ERROR, "The connection was reset by the remote side.");
				g_ErrorReport.Write("The connection was reset by the remote side.\r\n");
				g_ErrorReport.WriteCurrentTime();
				break;
			case WSAECONNABORTED:
				g_ConsoleDebug->Write(MCD_ERROR, "The connection was terminated due to a time-out or other failure.");
				g_ErrorReport.Write("The connection was terminated due to a time-out or other failure.\r\n");
				g_ErrorReport.WriteCurrentTime();
				break;
			}
#endif // CONSOLE_DEBUG
			SocketClient.Close();

#ifdef NEW_PROTOCOL_SYSTEM
			gProtocolSend.DisconnectServer();
#endif	

			CUIMng::Instance().PopUpMsgWin(MESSAGE_SERVER_LOST);
			break;
		}
		break;
	case WM_CTLCOLOREDIT:
		SetBkColor((HDC)wParam, RGB(0, 0, 0));
		SetTextColor((HDC)wParam, RGB(255, 255, 255));
		return (LRESULT)GetStockObject(BLACK_BRUSH);
		break;
	case WM_ERASEBKGND:
		return TRUE;
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
	}
	return 0;
	break;
	case WM_CLOSE:
	{
		Destroy = true;
		if (gProtect->m_MainInfo.m_AutoUpdateCpanel != 0)
			InGameUpdate_Shutdown();
		TerminateProcess(GetCurrentProcess(), 0);
		return 0;
	}
	break;
	case WM_DESTROY:
	{
		Destroy = true;
		if (gProtect->m_MainInfo.m_AutoUpdateCpanel != 0)
			InGameUpdate_Shutdown();
		SocketClient.Close();

#ifdef NEW_PROTOCOL_SYSTEM
		gProtocolSend.DisconnectServer();
#endif	
		TrayRemoveIcon();
		DestroySound();
		//DestroyWindow();
		KillGLWindow();
		CloseMainExe();
		PostQuitMessage(0);
	}
	break;
	case WM_SETCURSOR:
		ShowCursor(false);
		break;
#if (defined WINDOWMODE)
	case WM_SIZE:
		if (SIZE_MINIMIZED == wParam && g_bUseWindowMode == FALSE)
		{
			if (!(g_bMinimizedEnabled))
			{
				DWORD dwMess[SIZE_ENCRYPTION_KEY];
				for (int i = 0; i < SIZE_ENCRYPTION_KEY; ++i)
				{
					dwMess[i] = GetTickCount();
				}
				g_SimpleModulusCS.LoadKeyFromBuffer((BYTE*)dwMess, FALSE, FALSE, FALSE, TRUE);
			}
		}
		break;
#else
#ifdef NDEBUG
#ifndef FOR_WORK
#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
	case WM_SIZE:
		if (SIZE_MINIMIZED == wParam
#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
			&& g_bUseWindowMode == FALSE
#endif
			)
		{
			if (!(g_bMinimizedEnabled))
			{
				SendHackingChecked(0x05, 0);
				DWORD dwMess[SIZE_ENCRYPTION_KEY];
				for (int i = 0; i < SIZE_ENCRYPTION_KEY; ++i)
				{
					dwMess[i] = GetTickCount();
				}
				g_SimpleModulusCS.LoadKeyFromBuffer((BYTE*)dwMess, FALSE, FALSE, FALSE, TRUE);
			}
		}
		break;
#endif
#endif
#endif
#endif
	default:
		if (msg >= WM_CHATROOMMSG_BEGIN && msg < WM_CHATROOMMSG_END)
			g_pChatRoomSocketList->ProcessSocketMessage(msg - WM_CHATROOMMSG_BEGIN, WSAGETSELECTEVENT(lParam));
		break;
	}

	MouseLButtonDBClick = false;
	if (MouseLButtonPop == true && (g_iMousePopPosition_x != MouseX || g_iMousePopPosition_y != MouseY))
		MouseLButtonPop = false;
	switch (msg)
	{
	case WM_MOUSEMOVE:
	{
		static int WindowLeft = 640;
		WindowLeft = WindowWidth / g_fScreenRate_y;
		MouseX = (float)LOWORD(lParam) / g_fScreenRate_y;
		MouseY = (float)HIWORD(lParam) / g_fScreenRate_y;
		if (MouseX < 0)
			MouseX = 0;
		if (MouseX > GetWindowsX)
			MouseX = GetWindowsX;
		if (MouseY < 0)
			MouseY = 0;
		if (MouseY > GetWindowsY)
			MouseY = GetWindowsY;
	}
	break;
	case WM_LBUTTONDOWN:
		g_iNoMouseTime = 0;
		MouseLButtonPop = false;
		if (!MouseLButton)
			MouseLButtonPush = true;
		MouseLButton = true;
#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
		SetCapture(g_hWnd);
#endif
		break;
	case WM_LBUTTONUP:
		g_iNoMouseTime = 0;
		MouseLButtonPush = false;
		//if(MouseLButton) MouseLButtonPop = true;
		MouseLButtonPop = true;
		MouseLButton = false;
		g_iMousePopPosition_x = MouseX;
		g_iMousePopPosition_y = MouseY;
#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
		ReleaseCapture();
#endif
		break;
	case WM_RBUTTONDOWN:
		g_iNoMouseTime = 0;
		MouseRButtonPop = false;
		if (!MouseRButton) MouseRButtonPush = true;
		MouseRButton = true;
#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
		SetCapture(g_hWnd);
#endif
		break;
	case WM_RBUTTONUP:
		g_iNoMouseTime = 0;
		MouseRButtonPush = false;
		if (MouseRButton) MouseRButtonPop = true;
		MouseRButton = false;
#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
		ReleaseCapture();
#endif
		break;
	case WM_LBUTTONDBLCLK:
		g_iNoMouseTime = 0;
		MouseLButtonDBClick = true;
		break;
	case WM_MBUTTONDOWN:
		g_iNoMouseTime = 0;
		MouseMButtonPop = false;
		if (!MouseMButton) MouseMButtonPush = true;
		MouseMButton = true;
#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
		SetCapture(g_hWnd);
#endif
		break;
	case WM_MBUTTONUP:
		g_iNoMouseTime = 0;
		MouseMButtonPush = false;
		if (MouseMButton) MouseMButtonPop = true;
		MouseRButton = false;
#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
		ReleaseCapture();
#endif
		break;
	case WM_MOUSEWHEEL:
	{
		MouseWheel = (short)HIWORD(wParam) / WHEEL_DELTA;
	}
	break;
	case WM_IME_NOTIFY:
	{
		if (g_iChatInputType == 1)
		{
			switch (wParam)
			{
			case IMN_SETCONVERSIONMODE:
				if (GetFocus() == g_hWnd)
				{
					CheckTextInputBoxIME(IME_CONVERSIONMODE);
				}
				break;
			case IMN_SETSENTENCEMODE:
				if (GetFocus() == g_hWnd)
				{
					CheckTextInputBoxIME(IME_SENTENCEMODE);
				}
				break;
			default:
				break;
			}
		}
	}
	break;
	case WM_CHAR:
	{
		switch (wParam)
		{
		case VK_RETURN:
		{
			SetEnterPressed(true);
		}
		break;
		}
	}
	break;
	}

	if (g_BuffSystem) {
		LRESULT result;
		TheBuffStateSystem().HandleWindowMessage(msg, wParam, lParam, result);
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool CreateOpenglWindow()
{
	PIXELFORMATDESCRIPTOR pfd;

	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 16;
	pfd.cDepthBits = 16;

	if (!(g_hDC = GetDC(g_hWnd)))
	{
		g_ErrorReport.Write("OpenGL Get DC Error - ErrorCode : %d\r\n", GetLastError());
		KillGLWindow();
		MessageBox(NULL, GlobalText[4], "OpenGL Get DC Error.", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	GLuint PixelFormat;

	if (!(PixelFormat = ChoosePixelFormat(g_hDC, &pfd)))
	{
		g_ErrorReport.Write("OpenGL Choose Pixel Format Error - ErrorCode : %d\r\n", GetLastError());
		KillGLWindow();
		MessageBox(NULL, GlobalText[4], "OpenGL Choose Pixel Format Error.", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!SetPixelFormat(g_hDC, PixelFormat, &pfd))
	{
		g_ErrorReport.Write("OpenGL Set Pixel Format Error - ErrorCode : %d\r\n", GetLastError());
		KillGLWindow();
		MessageBox(NULL, GlobalText[4], "OpenGL Set Pixel Format Error.", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!(g_hRC = wglCreateContext(g_hDC)))
	{
		g_ErrorReport.Write("OpenGL Create Context Error - ErrorCode : %d\r\n", GetLastError());
		KillGLWindow();
		MessageBox(NULL, GlobalText[4], "OpenGL Create Context Error.", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!wglMakeCurrent(g_hDC, g_hRC))
	{
		g_ErrorReport.Write("OpenGL Make Current Error - ErrorCode : %d\r\n", GetLastError());
		KillGLWindow();
		MessageBox(NULL, GlobalText[4], "OpenGL Make Current Error.", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (glewInit() != GLEW_OK)
	{
		MessageBox(NULL, "Erro ao carregar glew32.dll", "Erro", MB_OK | MB_ICONERROR);
		return false;
	}

	ShowWindow(g_hWnd, SW_SHOW);
	SetForegroundWindow(g_hWnd);
	SetFocus(g_hWnd);
	return true;
}

HWND StartWindow(HINSTANCE hCurrentInst, int nCmdShow)
{
	char* windowName = gProtect->m_MainInfo.WindowName;
	WNDCLASS wndClass;
	HWND hWnd;

	wndClass.style = CS_OWNDC | CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hCurrentInst;
	HICON hBigIcon = (HICON)LoadImage(hCurrentInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	HICON hSmallIcon = (HICON)LoadImage(hCurrentInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	wndClass.hIcon = hBigIcon;	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = windowName;

	RegisterClass(&wndClass);

	RECT rc = { 0, 0, WindowWidth, WindowHeight };

	if (g_bBorderless == 1)
	{
		AdjustWindowRect(&rc, WS_POPUP | WS_CLIPCHILDREN, FALSE);

		hWnd = CreateWindowEx(
			WS_EX_APPWINDOW,
			windowName,
			windowName,
			WS_POPUP | WS_CLIPCHILDREN | WS_VISIBLE,
			(GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) / 2,
			(GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) / 2,
			rc.right - rc.left,
			rc.bottom - rc.top,
			NULL,
			NULL,
			hCurrentInst,
			NULL);
	}
	else if (g_bUseWindowMode == TRUE)
	{
		AdjustWindowRect(&rc, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER | WS_CLIPCHILDREN, FALSE);

		hWnd = CreateWindowEx(
			WS_EX_APPWINDOW,
			windowName,
			windowName,
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER | WS_CLIPCHILDREN,
			(GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) / 2,
			(GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) / 2,
			rc.right - rc.left,
			rc.bottom - rc.top,
			NULL,
			NULL,
			hCurrentInst,
			NULL);
	}
	else
	{
		hWnd = CreateWindowEx(
			WS_EX_TOPMOST | WS_EX_APPWINDOW,
			windowName,
			windowName,
			WS_POPUP,
			0,
			0,
			WindowWidth,
			WindowHeight,
			NULL,
			NULL,
			hCurrentInst,
			NULL);
	}

	return hWnd;
}

char m_ID[11];
char m_Version[11];
char m_ExeVersion[11];
int  m_SoundOnOff;
int  m_MusicOnOff;
int  m_Resolution;
int	m_nColorDepth;
int	g_iRenderTextType = 0;
int	m_CameraOnOff;

char g_aszMLSelection[MAX_LANGUAGE_NAME_LENGTH] = { '\0' };
std::string g_strSelectedML = "";

BOOL OpenInitFile()
{
	gController.Load();

	char szIniFilePath[256 + 20] = "";
	char szCurrentDir[256];
	GetCurrentDirectory(256, szCurrentDir);
	strcpy(szIniFilePath, szCurrentDir);
	if (szCurrentDir[strlen(szCurrentDir) - 1] == '\\')
		strcat(szIniFilePath, "config.ini");
	else
		strcat(szIniFilePath, "\\Data\\Custom\\config.ini");

	GetPrivateProfileString("LOGIN", "Version", "", m_Version, 11, szIniFilePath);

	szServerIpAddress = new char[32];
	memset(szServerIpAddress, 0, 32);

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	addrinfo hints = { 0 };
	addrinfo* result = nullptr;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(gProtect->m_MainInfo.IpAddress, NULL, &hints, &result) == 0)
	{
		sockaddr_in* addr = (sockaddr_in*)result->ai_addr;
		const char* ip = inet_ntoa(addr->sin_addr);
		strcpy(szServerIpAddress, ip);
		freeaddrinfo(result);
	}
	else
	{
		strcpy(szServerIpAddress, gProtect->m_MainInfo.IpAddress);
	}

	g_ServerPort = gProtect->m_MainInfo.IpAddressPort;

	Version[0] = gProtect->m_MainInfo.ClientVersion[0] + 1;
	Version[1] = gProtect->m_MainInfo.ClientVersion[2] + 2;
	Version[2] = gProtect->m_MainInfo.ClientVersion[3] + 3;
	Version[3] = gProtect->m_MainInfo.ClientVersion[5] + 4;
	Version[4] = gProtect->m_MainInfo.ClientVersion[6] + 5;

	memcpy(Serial, gProtect->m_MainInfo.ClientSerial, sizeof(Serial));

	char* lpszCommandLine = GetCommandLine();
	char lpszFile[MAX_PATH];
	if (GetFileNameOfFilePath(lpszFile, lpszCommandLine))
	{
		WORD wVersion[4];
		if (GetFileVersion(lpszFile, wVersion))
		{
			sprintf(m_ExeVersion, "%d.%02d", wVersion[0], wVersion[1]);
			if (wVersion[2] > 0)
			{
				char lpszMinorVersion[3] = "a";
				if (wVersion[2] > 26)
				{
					lpszMinorVersion[0] = 'A';
					lpszMinorVersion[0] += (wVersion[2] - 27);
					lpszMinorVersion[1] = '+';
				}
				else
				{
					lpszMinorVersion[0] += (wVersion[2] - 1);
				}
				strcat(m_ExeVersion, lpszMinorVersion);
			}
		}
		else
		{
			strcpy(m_ExeVersion, m_Version);
		}
	}
	else
	{
		strcpy(m_ExeVersion, m_Version);
	}

	m_ID[0] = '\0';
	m_SoundOnOff = 1;
	m_MusicOnOff = 1;
	m_Resolution = 0;
	m_nColorDepth = 0;

	HKEY hKey;
	DWORD dwDisp;
	DWORD dwSize;
	if (ERROR_SUCCESS == RegCreateKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\MuOnline\\Config", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisp))
	{
		dwSize = 11;
		RegQueryValueEx(hKey, "ID", 0, NULL, (LPBYTE)m_ID, &dwSize);

		dwSize = sizeof(int);
		if (RegQueryValueEx(hKey, "SoundOnOff", 0, NULL, (LPBYTE)&m_SoundOnOff, &dwSize) != ERROR_SUCCESS)
			m_SoundOnOff = 1;

		dwSize = sizeof(int);
		if (RegQueryValueEx(hKey, "MusicOnOff", 0, NULL, (LPBYTE)&m_MusicOnOff, &dwSize) != ERROR_SUCCESS)
			m_MusicOnOff = 1;

		dwSize = sizeof(int);
		if (RegQueryValueEx(hKey, "Resolution", 0, NULL, (LPBYTE)&m_Resolution, &dwSize) != ERROR_SUCCESS)
			m_Resolution = 1;
		if (m_Resolution < 0 || m_Resolution > 100)
			m_Resolution = 1;

		if (RegQueryValueEx(hKey, "ColorDepth", 0, NULL, (LPBYTE)&m_nColorDepth, &dwSize) != ERROR_SUCCESS)
			m_nColorDepth = 0;

		dwSize = sizeof(int);
		if (RegQueryValueEx(hKey, "TextOut", 0, NULL, (LPBYTE)&g_iRenderTextType, &dwSize) != ERROR_SUCCESS)
			g_iRenderTextType = 0;

		g_iChatInputType = 1;

#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
		dwSize = sizeof(int);
		if (RegQueryValueEx(hKey, "WindowMode", 0, NULL, (LPBYTE)&g_bUseWindowMode, &dwSize) != ERROR_SUCCESS)
			g_bUseWindowMode = TRUE;
#endif

		dwSize = MAX_LANGUAGE_NAME_LENGTH;
		if (RegQueryValueEx(hKey, "LangSelection", 0, NULL, (LPBYTE)g_aszMLSelection, &dwSize) != ERROR_SUCCESS)
			strcpy(g_aszMLSelection, "Eng");

		g_strSelectedML = g_aszMLSelection;
	}
	RegCloseKey(hKey);

	g_bBorderless = GetPrivateProfileInt("AntiLagOptions", "Borderless", 0, szIniFilePath);

	switch (m_Resolution)
	{
	case 1: WindowWidth = 800; WindowHeight = 600; break;
	case 2: WindowWidth = 1024; WindowHeight = 768; break;
	case 3: WindowWidth = 1280; WindowHeight = 960; break;
	case 4: WindowWidth = 1366; WindowHeight = 768; break;
	case 5: WindowWidth = 1440; WindowHeight = 900; break;
	case 6: WindowWidth = 1600; WindowHeight = 900; break;
	case 7: WindowWidth = 1680; WindowHeight = 1050; break;
	case 8: WindowWidth = 1910; WindowHeight = 970; break;
	case 9: WindowWidth = 1920; WindowHeight = 1080; break;
	case 10: WindowWidth = 2560; WindowHeight = 1440; break;
	default: WindowWidth = 800; WindowHeight = 600; break;
	}

	if (gProtect->m_MainInfo.m_WideScreenType != 0)
	{
		if (m_Resolution > 3)
		{
			g_fScreenRate_x = 1.6f;
			g_fScreenRate_y = 1.6f;
		}
		else
		{
			g_fScreenRate_x = (float)WindowHeight / 480;
			g_fScreenRate_y = (float)WindowHeight / 480;
		}
	}
	else
	{
		if (m_Resolution > 3)
		{
			g_fScreenRate_x = (float)WindowHeight / 480;
			g_fScreenRate_y = (float)WindowHeight / 480;
		}
		else
		{
			g_fScreenRate_x = (float)WindowHeight / 480;
			g_fScreenRate_y = (float)WindowHeight / 480;
		}
	}

	GWidescreen.Init();
	return TRUE;
}
BOOL Util_CheckOption(char* lpszCommandLine, unsigned char cOption, char* lpszString)
{
	unsigned char cComp[2];
	cComp[0] = cOption; cComp[1] = cOption;
	if (islower((int)cOption))
	{
		cComp[1] = toupper((int)cOption);
	}
	else if (isupper((int)cOption))
	{
		cComp[1] = tolower((int)cOption);
	}

	int nFind = (int)'/';
	unsigned char* lpFound = (unsigned char*)lpszCommandLine;
	while (lpFound)
	{
		lpFound = (unsigned char*)strchr((char*)(lpFound + 1), nFind);
		if (lpFound && (*(lpFound + 1) == cComp[0] || *(lpFound + 1) == cComp[1]))
		{	// ¹ß°ß
			if (lpszString)
			{
				int nCount = 0;
				for (unsigned char* lpSeek = lpFound + 2; *lpSeek != ' ' && *lpSeek != '\0'; lpSeek++)
				{
					nCount++;
				}

				memcpy(lpszString, lpFound + 2, nCount);
				lpszString[nCount] = '\0';
			}
			return (TRUE);
		}
	}

	return (FALSE);
}

BOOL UpdateFile(char* lpszOld, char* lpszNew)
{
	SetFileAttributes(lpszOld, FILE_ATTRIBUTE_NORMAL);
	SetFileAttributes(lpszNew, FILE_ATTRIBUTE_NORMAL);

	DWORD dwStartTickCount = ::GetTickCount();
	while (::GetTickCount() - dwStartTickCount < 5000) {
		if (CopyFile(lpszOld, lpszNew, FALSE))
		{	// ¼º°ø
			DeleteFile(lpszOld);
			return (TRUE);
		}
	}
	g_ErrorReport.Write("%s to %s CopyFile Error : %d\r\n", lpszNew, lpszOld, GetLastError());
	return (FALSE);
}

#include <tlhelp32.h>

BOOL KillExeProcess(char* lpszExe)
{
	HANDLE hProcessSnap = NULL;
	BOOL bRet = FALSE;
	PROCESSENTRY32 pe32 = { 0 };

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return (FALSE);

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hProcessSnap, &pe32))
	{
		do
		{
			if (stricmp(pe32.szExeFile, lpszExe) == 0)
			{
				HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);

				if (process)
				{
					TerminateProcess(process, 0);
				}
			}
		} while (Process32Next(hProcessSnap, &pe32));
		bRet = TRUE;
	}
	else
		bRet = FALSE;

	CloseHandle(hProcessSnap);

	return bRet;
}

char g_lpszCmdURL[50];
BOOL GetConnectServerInfo(PSTR szCmdLine, char* lpszURL, WORD* pwPort)
{
	char lpszTemp[256] = { 0, };
	if (Util_CheckOption(szCmdLine, 'y', lpszTemp))
	{
		BYTE bySuffle[] = { 0x0C, 0x07, 0x03, 0x13 };

		for (int i = 0; i < (int)strlen(lpszTemp); i++)
			lpszTemp[i] -= bySuffle[i % 4];
		strcpy(lpszURL, lpszTemp);

		if (Util_CheckOption(szCmdLine, 'z', lpszTemp))
		{
			for (int j = 0; j < (int)strlen(lpszTemp); j++)
				lpszTemp[j] -= bySuffle[j % 4];
			*pwPort = atoi(lpszTemp);
		}
		g_ErrorReport.Write("[Virtual Connection] Connect IP : %s, Port : %d\r\n", lpszURL, *pwPort);
		return (TRUE);
	}
	if (!Util_CheckOption(szCmdLine, 'u', lpszTemp))
	{
		return (FALSE);
	}
	strcpy(lpszURL, lpszTemp);
	if (!Util_CheckOption(szCmdLine, 'p', lpszTemp))
	{
		return (FALSE);
	}
	*pwPort = atoi(lpszTemp);

	return (TRUE);
}


extern int TimeRemain;
BOOL g_bInactiveTimeChecked = FALSE;
void MoveObject(OBJECT* o);


#include <dbghelp.h>
#include <EventEntryLevel.h>
#include <Monsters.h>
#include <MonsterEffect.h>
#include <ItemSize.h>

#pragma comment(lib,"dbghelp.lib")

bool ExceptionCallback(_EXCEPTION_POINTERS* pExceptionInfo)
{
	char path[MAX_PATH];

	SYSTEMTIME SystemTime;

	GetLocalTime(&SystemTime);

	//wsprintf(path, "%d-%d-%d_%dh%dm%ds.dmp", SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);

	HANDLE file = CreateFile(path, GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (file != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION mdei;

		mdei.ThreadId = GetCurrentThreadId();

		mdei.ExceptionPointers = pExceptionInfo;

		mdei.ClientPointers = 0;

		if (MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file, (MINIDUMP_TYPE)(MiniDumpScanMemory + MiniDumpWithIndirectlyReferencedMemory), &mdei, 0, 0) != 0)
		{
			CloseHandle(file);
			return EXCEPTION_EXECUTE_HANDLER;
		}
	}
	CloseHandle(file);

	return EXCEPTION_CONTINUE_SEARCH;
}

DWORD GetParentProcessId(DWORD processId)
{
	DWORD parentPid = 0;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	PROCESSENTRY32 pe;
	ZeroMemory(&pe, sizeof(pe));
	pe.dwSize = sizeof(pe);

	if (Process32First(hSnapshot, &pe))
	{
		do
		{
			if (pe.th32ProcessID == processId)
			{
				parentPid = pe.th32ParentProcessID;
				break;
			}
		} while (Process32Next(hSnapshot, &pe));
	}

	CloseHandle(hSnapshot);
	return parentPid;
}

bool GetProcessFileNameByPid(DWORD processId, char* outName, DWORD outSize)
{
	if (outName == NULL || outSize == 0)
	{
		return false;
	}

	outName[0] = '\0';

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
	if (hProcess == NULL)
	{
		return false;
	}

	char fullPath[MAX_PATH] = { 0 };

	if (GetModuleFileNameExA(hProcess, NULL, fullPath, MAX_PATH) == 0)
	{
		CloseHandle(hProcess);
		return false;
	}

	CloseHandle(hProcess);

	char* lastSlash = strrchr(fullPath, '\\');
	if (lastSlash != NULL)
	{
		strcpy_s(outName, outSize, lastSlash + 1);
	}
	else
	{
		strcpy_s(outName, outSize, fullPath);
	}

	return true;
}

bool WasStartedByLauncher()
{
	DWORD currentPid = GetCurrentProcessId();
	DWORD parentPid = GetParentProcessId(currentPid);

	if (parentPid == 0)
	{
		return false;
	}

	char parentExeName[MAX_PATH] = { 0 };

	if (!GetProcessFileNameByPid(parentPid, parentExeName, sizeof(parentExeName)))
	{
		return false;
	}

	if (_stricmp(parentExeName, gProtect->m_MainInfo.m_LauncherName) == 0)
	{
		return true;
	}

	return false;
}

bool IsLauncherAlreadyRunning()
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	PROCESSENTRY32 pe;
	ZeroMemory(&pe, sizeof(pe));
	pe.dwSize = sizeof(pe);

	if (Process32First(hSnapshot, &pe))
	{
		do
		{
			if (_stricmp(pe.szExeFile, gProtect->m_MainInfo.m_LauncherName) == 0)
			{
				CloseHandle(hSnapshot);
				return true;
			}
		} while (Process32Next(hSnapshot, &pe));
	}

	CloseHandle(hSnapshot);
	return false;
}

void RunLauncherFromSameFolder()
{
	char szCurrentPath[MAX_PATH] = { 0 };
	char szLauncherPath[MAX_PATH] = { 0 };

	GetModuleFileNameA(NULL, szCurrentPath, MAX_PATH);

	strcpy_s(szLauncherPath, szCurrentPath);

	char* lastSlash = strrchr(szLauncherPath, '\\');
	if (lastSlash != NULL)
	{
		*(lastSlash + 1) = '\0';
		strcat_s(szLauncherPath, gProtect->m_MainInfo.m_LauncherName);
	}

	ShellExecuteA(NULL, "open", szLauncherPath, NULL, NULL, SW_SHOWNORMAL);
}

bool IsRunningAsAdmin()
{
	BOOL isAdmin = FALSE;
	PSID adminGroup;
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	if (AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup)) {
		CheckTokenMembership(NULL, adminGroup, &isAdmin);
		FreeSid(adminGroup);
	}
	return isAdmin;
}

void RestartAsAdmin()
{
	char szPath[MAX_PATH];
	if (GetModuleFileNameA(NULL, szPath, MAX_PATH)) {
		SHELLEXECUTEINFO execInfo = { 0 };
		execInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		execInfo.fMask = SEE_MASK_FLAG_DDEWAIT | SEE_MASK_FLAG_NO_UI;
		execInfo.hwnd = NULL;
		execInfo.lpVerb = "runas";
		execInfo.lpFile = szPath;
		execInfo.nShow = SW_SHOWNORMAL;

		if (!ShellExecuteEx(&execInfo)) {
			MessageBox(NULL, "Falha ao iniciar o programa como Administrador.", "Erro", MB_OK | MB_ICONERROR);
		}
		ExitProcess(0);
	}
}

typedef LONG(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
bool GetWindowsVersion(DWORD& majorVersion, DWORD& minorVersion, DWORD& buildNumber) {
	HMODULE hMod = GetModuleHandleW(L"ntdll.dll");
	if (hMod) {
		RtlGetVersionPtr fn = (RtlGetVersionPtr)GetProcAddress(hMod, "RtlGetVersion");
		if (fn != nullptr) {
			RTL_OSVERSIONINFOW versionInfo = { 0 };
			versionInfo.dwOSVersionInfoSize = sizeof(versionInfo);
			if (fn(&versionInfo) == 0) {
				majorVersion = versionInfo.dwMajorVersion;
				minorVersion = versionInfo.dwMinorVersion;
				buildNumber = versionInfo.dwBuildNumber;
				return true;
			}
		}
	}
	return false;
}

bool IsRunningInVM()
{
	int cpuInfo[4] = { 0 };

	bool hypervisorPresent = false;
	char hypervisorVendor[13] = { 0 };

	__cpuid(cpuInfo, 1);

	if (cpuInfo[2] & (1 << 31))
	{
		hypervisorPresent = true;
	}

	__cpuid(cpuInfo, 0x40000000);
	memcpy(hypervisorVendor + 0, &cpuInfo[1], 4);
	memcpy(hypervisorVendor + 4, &cpuInfo[2], 4);
	memcpy(hypervisorVendor + 8, &cpuInfo[3], 4);

	if (strstr(hypervisorVendor, "VMware") ||
		strstr(hypervisorVendor, "VBox") ||
		strstr(hypervisorVendor, "KVM") ||
		strstr(hypervisorVendor, "Xen"))
	{
		return true;
	}

	if (strstr(hypervisorVendor, "Microsoft Hv") || hypervisorPresent)
	{
		HKEY hKey;
		char manufacturer[256] = { 0 };
		char productName[256] = { 0 };
		DWORD size = 0;

		if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\BIOS", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			size = sizeof(manufacturer);
			RegQueryValueExA(hKey, "SystemManufacturer", NULL, NULL, (LPBYTE)manufacturer, &size);

			size = sizeof(productName);
			RegQueryValueExA(hKey, "SystemProductName", NULL, NULL, (LPBYTE)productName, &size);

			RegCloseKey(hKey);

			if (_stricmp(manufacturer, "Microsoft Corporation") == 0 &&
				_stricmp(productName, "Virtual Machine") == 0)
			{
				return true;
			}

			if (strstr(manufacturer, "VMware") ||
				strstr(manufacturer, "VirtualBox") ||
				strstr(manufacturer, "Xen") ||
				strstr(manufacturer, "QEMU") ||
				strstr(manufacturer, "KVM") ||
				strstr(productName, "Virtual") ||
				strstr(productName, "VMware") ||
				strstr(productName, "VirtualBox") ||
				strstr(productName, "KVM") ||
				strstr(productName, "QEMU") ||
				strstr(productName, "HVM domU"))
			{
				return true;
			}
		}
	}

	if (GetFileAttributesA("C:\\Windows\\System32\\drivers\\VBoxMouse.sys") != INVALID_FILE_ATTRIBUTES ||
		GetFileAttributesA("C:\\Windows\\System32\\drivers\\VBoxGuest.sys") != INVALID_FILE_ATTRIBUTES ||
		GetFileAttributesA("C:\\Windows\\System32\\drivers\\VBoxSF.sys") != INVALID_FILE_ATTRIBUTES ||
		GetFileAttributesA("C:\\Windows\\System32\\drivers\\vmmouse.sys") != INVALID_FILE_ATTRIBUTES ||
		GetFileAttributesA("C:\\Windows\\System32\\drivers\\vmhgfs.sys") != INVALID_FILE_ATTRIBUTES)
	{
		return true;
	}

	return false;
}

bool IsWindowsServer()
{
	OSVERSIONINFOEXA osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEXA));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);

	if (!GetVersionExA((OSVERSIONINFOA*)&osvi))
	{
		return false;
	}

	return (osvi.wProductType != VER_NT_WORKSTATION);
}

bool CheckSystemRequirements()
{
	if (gProtect->m_MainInfo.m_CheckRequeriments != 0)
	{
		DWORD majorVersion, minorVersion, buildNumber;

		if (!GetWindowsVersion(majorVersion, minorVersion, buildNumber)) {
			return false;
		}

		if (majorVersion < gProtect->m_MainInfo.m_CheckWindowsVersion) {
			char msg[256];
			wsprintf(msg, "Your operating system is not compatible with the game. Minimum requirement: Windows %d or higher.", gProtect->m_MainInfo.m_CheckWindowsVersion);
			MessageBox(NULL, msg, "Compatibility Error", MB_OK | MB_ICONERROR);
			return false;
		}

		if (IsWindowsServer()) {
			MessageBox(NULL, "Windows Server editions are not supported by the game.", "Compatibility Error", MB_OK | MB_ICONERROR);
			return false;
		}

		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memInfo);

		DWORDLONG totalPhysMem = memInfo.ullTotalPhys;

		if (totalPhysMem < (DWORDLONG)(gProtect->m_MainInfo.m_MinMemoryRAM - 1) * 1024ULL * 1024ULL * 1024ULL)
		{
			char ramMsg[256];
			wsprintf(ramMsg, "Your system does not meet the minimum requirements to run the game. Minimum Requirement %d GB RAM.", gProtect->m_MainInfo.m_MinMemoryRAM);
			MessageBox(NULL, ramMsg, "Minimum Requirements Error", MB_OK | MB_ICONERROR);
			return false;
		}

		SYSTEM_INFO sysInfo;
		GetSystemInfo(&sysInfo);

		if (sysInfo.dwNumberOfProcessors < gProtect->m_MainInfo.m_MinCPUCore) {

			const char* coreType;

			switch (gProtect->m_MainInfo.m_MinCPUCore) {
			case 1: coreType = "1 core"; break;
			case 2: coreType = "dual-core"; break;
			case 3: coreType = "tri-core"; break;
			case 4: coreType = "quad-core"; break;
			case 6: coreType = "hexa-core"; break;
			case 8: coreType = "octa-core"; break;
			case 12: coreType = "dodeca-core"; break;
			case 16: coreType = "hexadeca-core"; break;
			case 24: coreType = "24 cores"; break;
			case 32: coreType = "32 cores"; break;
			default:
				char tempCore[32];
				wsprintf(tempCore, "%d cores", gProtect->m_MainInfo.m_MinCPUCore);
				coreType = tempCore;
				break;
			}

			char cpuMsg[256];
			wsprintf(cpuMsg, "Your system does not meet the minimum requirements to run the game. A %s or higher processor is required.", coreType);
			MessageBox(NULL, cpuMsg, "Minimum Requirements Error", MB_OK | MB_ICONERROR);
			return false;
		}

		bool gpuValid = false;

		if (gProtect->m_MainInfo.m_MinVideoMemory == 0)
		{
			gpuValid = true;
		}
		else
		{
			IDXGIFactory* factory = nullptr;
			IDXGIAdapter* adapter = nullptr;

			HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);

			if (SUCCEEDED(hr) && factory != nullptr) {

				for (UINT i = 0; factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i) {

					DXGI_ADAPTER_DESC desc;

					HRESULT adapterHr = adapter->GetDesc(&desc);

					if (SUCCEEDED(adapterHr)) {

						DWORDLONG requiredVRAM = (DWORDLONG)(gProtect->m_MainInfo.m_MinVideoMemory - 1) * 1024ULL * 1024ULL * 1024ULL;

						DWORDLONG totalVideoMemory = desc.DedicatedVideoMemory;

						if (totalVideoMemory == 0)
						{
							totalVideoMemory = desc.SharedSystemMemory;
						}

						if (totalVideoMemory >= requiredVRAM) {
							gpuValid = true;
						}
					}

					adapter->Release();
				}

				factory->Release();
			}
			else {
				gpuValid = true;
			}
		}

		if (!gpuValid) {
			char gpuMsg[256];
			wsprintf(gpuMsg, "Your system does not meet the minimum requirements to run the game. %d GB or more of VRAM is required.", gProtect->m_MainInfo.m_MinVideoMemory);
			MessageBox(NULL, gpuMsg, "Minimum Requirements Error", MB_OK | MB_ICONERROR);
			return false;
		}
	}

	return true;
}

bool IsHardwareIdBlocked(const char* BlockList, const char* HardwareID)
{
	if (gProtect->m_MainInfo.m_GetHWID != 0)
	{
		if (BlockList == nullptr || HardwareID == nullptr)
		{
			return false;
		}

		char Buffer[50000];
		strcpy_s(Buffer, BlockList);

		char* context = nullptr;
		char* token = strtok_s(Buffer, ",", &context);

		while (token != nullptr)
		{
			if (_stricmp(token, HardwareID) == 0)
			{
				return true;
			}

			token = strtok_s(nullptr, ",", &context);
		}
	}
	return false;
}

bool GetLocalComputerHardwareId(char* outHardwareId, int bufferSize)
{
	if (gProtect->m_MainInfo.m_GetHWID != 0)
	{
		if (bufferSize < 36) return false;
		ZeroMemory(outHardwareId, bufferSize);

		DWORD VolumeSerialNumber = 0;
		if (GetVolumeInformationA("C:\\", NULL, 0, &VolumeSerialNumber, NULL, NULL, NULL, 0) == 0)
		{
			GetVolumeInformationA(NULL, NULL, 0, &VolumeSerialNumber, NULL, NULL, NULL, 0);
		}

		UUID uuid = { 0 };
		if (UuidCreateSequential(&uuid) != RPC_S_OK)
		{
			UuidCreate(&uuid);
		}

		SYSTEM_INFO SystemInfo;
		ZeroMemory(&SystemInfo, sizeof(SystemInfo));
		GetSystemInfo(&SystemInfo);

		DWORD ComputerHardwareId1 = VolumeSerialNumber ^ 0x12B586FE;
		DWORD ComputerHardwareId2 = *(DWORD*)(&uuid.Data4[2]) ^ 0x5D78A569;
		DWORD ComputerHardwareId3 = ((*(WORD*)(&uuid.Data4[6]) & 0xFFFF) | (SystemInfo.wProcessorArchitecture << 16)) ^ 0xF45BC123;
		DWORD ComputerHardwareId4 = ((SystemInfo.wProcessorLevel & 0xFFFF) | (SystemInfo.wProcessorRevision << 16)) ^ 0xB542D8E1;

		wsprintfA(outHardwareId, "%08X-%08X-%08X-%08X",
			ComputerHardwareId1,
			ComputerHardwareId2,
			ComputerHardwareId3,
			ComputerHardwareId4);
	}
	return true;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nCmdShow)
{
	MSG msg;

	if (szCmdLine && strstr(szCmdLine, "--apply-update"))
	{
		InGameUpdate_RunAsUpdater(szCmdLine);
		return 0;
	}

	gController.Instance = hInstance;

	leaf::AttachExceptionHandler(ExceptionCallback);

	gProtect = new CProtect;

	if (gProtect->ReadMainFile("Data//Local//info.bmd") == 0)
	{
		MessageBox(0, "Erro de configuração. Consulte o suporte! #1", "Erro de inicialização!", MB_OK | MB_ICONERROR);
		delete gProtect;
		gProtect = nullptr;
		ExitProcess(0);
	}

	if (gProtect->m_MainInfo.m_OpenLauncher != 0)
	{
		if (!WasStartedByLauncher())
		{
			if (!IsLauncherAlreadyRunning())
			{
				RunLauncherFromSameFolder();

				delete gProtect;
				gProtect = nullptr;
				return 0;
			}
		}
	}

	if (gProtect->m_MainInfo.m_RequireAdmin != 0)
	{
		if (!IsRunningAsAdmin())
		{
			if (gProtect->m_MainInfo.m_RequireAdminMessage != 0)
			{
				int resposta = MessageBoxA(NULL,
					"Este jogo requer execução de Administrador.\n\n"
					"Deseja reiniciar e executar como administrador?",
					"Requer Execução como Administrador",
					MB_YESNO | MB_ICONQUESTION);

				if (resposta == IDYES)
				{
					RestartAsAdmin();
				}
			}
			else
			{
				RestartAsAdmin();
			}

			delete gProtect;
			gProtect = nullptr;
			return 0;
		}
	}

	if (IsRunningInVM())
	{
		if (gProtect->m_MainInfo.m_BlockVirtualMachine != 0)
		{
			MessageBox(NULL, "The game cannot be run inside a virtual machine.", "Security Error", MB_OK | MB_ICONERROR);
			return false;
		}
	}

	DWORD MaxInstances = gProtect->m_MainInfo.m_MaxInstance;

	if (MaxInstances == 0)
		MaxInstances = 10;

	const char* SEMAPHORE_NAME = "Global\\MuOnlineClientLimit_v2026";

	g_hLimitSemaphore = CreateSemaphoreA(NULL, MaxInstances, MaxInstances, SEMAPHORE_NAME);

	if (g_hLimitSemaphore == NULL)
	{
		MessageBoxA(NULL, "Erro ao criar limitador de instâncias.", "Erro Fatal", MB_OK | MB_ICONERROR);
		return 0;
	}

	if (WaitForSingleObject(g_hLimitSemaphore, 0) != WAIT_OBJECT_0)
	{
		char msg[256];
		wsprintfA(msg,
			"Você já atingiu o limite máximo de clientes abertos!\n\n"
			"Máximo permitido: %d clientes simultâneos.",
			MaxInstances);

		MessageBoxA(NULL, msg, "Limite de Instâncias", MB_OK | MB_ICONWARNING);

		CloseHandle(g_hLimitSemaphore);
		g_hLimitSemaphore = NULL;
		return 0;
	}

	//if (GetFileAttributesA("Data\\Local\\hw.id") == INVALID_FILE_ATTRIBUTES)
	//{
	//	char HardwareID[36] = { 0 };
	//
	//	if (GetLocalComputerHardwareId(HardwareID, sizeof(HardwareID)))
	//	{
	//		char msg[512];
	//		wsprintfA(msg,
	//			"SEU CUSTOMER HARDWARE ID:\n\n"
	//			"%s\n\n",
	//			HardwareID);
	//
	//		MessageBoxA(NULL, msg, "Hardware ID - Verificação de Segurança", MB_OK | MB_ICONINFORMATION);
	//
	//		HANDLE hFile = CreateFileA("Data\\Local\\hw.id", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//		if (hFile != INVALID_HANDLE_VALUE)
	//		{
	//			DWORD written;
	//			WriteFile(hFile, HardwareID, strlen(HardwareID), &written, NULL);
	//			CloseHandle(hFile);
	//		}
	//	}
	//}

	if (gProtect->m_MainInfo.m_GetHWID != 0)
	{
		char LocalHardwareID[36] = { 0 };
		if (GetLocalComputerHardwareId(LocalHardwareID, sizeof(LocalHardwareID)))
		{
			if (IsHardwareIdBlocked(gProtect->m_MainInfo.m_CheckBlockHWID, LocalHardwareID))
			{
				MessageBoxA(
					NULL,
					"Este computador foi BLOQUEADO pela Administração.\n\n"
					"e não pode ser usado para acessar o Servidor!\n",
					"Aviso da Administração",
					MB_OK | MB_ICONERROR
				);

				ExitProcess(0);
			}
		}
	}

	if (gProtect->m_MainInfo.m_CheckRequeriments != 0)
	{
		if (!CheckSystemRequirements())
		{
			delete gProtect;
			gProtect = nullptr;
			ExitProcess(1);
		}
	}

	gCustomJewel->Init();

	gCustomWing.Init();

	gCustomEffects.Init();

	gMonsters.Init();

	gMonsterName.Init();

	gMonsterGlow.Init();

	gMonsterEffect.Init();

	gEventEntryLevel.Init();

	gItemManager->Init();

	gDescriptions->Init();

	gCustomServerName.Init();

	gRenderModel.Init();

	gCCustomSetEffect.Init();

	gDisableExcellent.Init();

	gCustomItemPosition.Init();

	gItemNewOption.Init();

	gCustomItemSize.Init();

	gProtect->CheckPlugin1File();
	gProtect->CheckPlugin2File();
	gProtect->CheckPlugin3File();
	gProtect->CheckPlugin4File();
	gProtect->CheckPlugin5File();

	InitHackCheck();

	InitAutoClick();
	//InitAutoCombo();

	char lpszExeVersion[256] = "unknown";

	char* lpszCommandLine = GetCommandLine();
	char lpszFile[MAX_PATH];
	WORD wVersion[4] = { 0, };
	if (GetFileNameOfFilePath(lpszFile, lpszCommandLine))
	{
		if (GetFileVersion(lpszFile, wVersion))
		{
			sprintf(lpszExeVersion, "%d.%02d", wVersion[0], wVersion[1]);
			if (wVersion[2] > 0)
			{
				char lpszMinorVersion[2] = "a";
				lpszMinorVersion[0] += (wVersion[2] - 1);
				strcat(lpszExeVersion, lpszMinorVersion);
			}
		}
	}

	Console.Init();

	g_ErrorReport.Write("\r\n");
	g_ErrorReport.WriteLogBegin();
	g_ErrorReport.AddSeparator();
	g_ErrorReport.Write("Mu online %s (%s) executed. (%d.%d.%d.%d)\r\n", lpszExeVersion, "Eng", wVersion[0], wVersion[1], wVersion[2], wVersion[3]);

	g_ConsoleDebug->Write(MCD_NORMAL, "Mu Online (Version: %d.%d.%d.%d)", wVersion[0], wVersion[1], wVersion[2], wVersion[3]);

	g_ErrorReport.WriteCurrentTime();
	ER_SystemInfo si;
	ZeroMemory(&si, sizeof(ER_SystemInfo));
	GetSystemInfo(&si);
	g_ErrorReport.AddSeparator();
	g_ErrorReport.WriteSystemInfo(&si);
	g_ErrorReport.AddSeparator();

	// PKD_ADD_BINARY_PROTECTION
	VM_START
		WORD wPortNumber;
	if (GetConnectServerInfo(szCmdLine, g_lpszCmdURL, &wPortNumber))
	{
		szServerIpAddress = g_lpszCmdURL;
		g_ServerPort = wPortNumber;
	}
	VM_END

		if (!OpenMainExe())
		{
			return false;
		}

	// PKD_ADD_BINARY_PROTECTION
	VM_START
		g_SimpleModulusCS.LoadEncryptionKey("Data\\Enc1.dat");
	g_SimpleModulusSC.LoadDecryptionKey("Data\\Dec2.dat");
	VM_END

		g_ErrorReport.Write("> To read config.ini.\r\n");

	if (OpenInitFile() == FALSE)
	{
		g_ErrorReport.Write("config.ini read error\r\n");
		return false;
	}

	pMultiLanguage = new CMultiLanguage(g_strSelectedML);

	if (g_iChatInputType == 1)
		ShowCursor(FALSE);

	g_ErrorReport.Write("> Enum display settings.\r\n");
	DEVMODE DevMode;
	DEVMODE* pDevmodes;
	int nModes = 0;
	while (EnumDisplaySettings(NULL, nModes, &DevMode)) nModes++;
	pDevmodes = new DEVMODE[nModes + 1];
	nModes = 0;
	while (EnumDisplaySettings(NULL, nModes, &pDevmodes[nModes])) nModes++;

	DWORD dwBitsPerPel = 16;
	for (int n1 = 0; n1 < nModes; n1++)
	{
		if (pDevmodes[n1].dmBitsPerPel == 16 && m_nColorDepth == 0) {
			dwBitsPerPel = 16; break;
		}
		if (pDevmodes[n1].dmBitsPerPel == 24 && m_nColorDepth == 1) {
			dwBitsPerPel = 24; break;
		}
		if (pDevmodes[n1].dmBitsPerPel == 32 && m_nColorDepth == 1) {
			dwBitsPerPel = 32; break;
		}
	}

#ifdef ENABLE_FULLSCREEN
#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
	if (g_bUseWindowMode == FALSE)
#endif	// USER_WINDOW_MODE
	{
		for (int n2 = 0; n2 < nModes; n2++)
		{
			if (pDevmodes[n2].dmPelsWidth == WindowWidth && pDevmodes[n2].dmPelsHeight == WindowHeight && pDevmodes[n2].dmBitsPerPel == dwBitsPerPel)
			{
				g_ErrorReport.Write("> Change display setting %dx%d.\r\n", pDevmodes[n2].dmPelsWidth, pDevmodes[n2].dmPelsHeight);
				ChangeDisplaySettings(&pDevmodes[n2], 0);
				break;
			}
		}
	}
#endif //ENABLE_FULLSCREEN

	delete[] pDevmodes;

	g_ErrorReport.Write("> Screen size = %d x %d.\r\n", WindowWidth, WindowHeight);

	g_hInst = hInstance;
	g_hWnd = StartWindow(hInstance, nCmdShow);
	g_ErrorReport.Write("> Start window success.\r\n");

	if (!CreateOpenglWindow())
	{
		return FALSE;
	}

	g_ErrorReport.Write("> OpenGL init success.\r\n");
	g_ErrorReport.AddSeparator();
	//g_ErrorReport.WriteOpenGLInfo();
	g_ErrorReport.AddSeparator();
	g_ErrorReport.WriteSoundCardInfo();

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	if (gProtect->m_MainInfo.m_AutoUpdateCpanel != 0)
	{
		InGameUpdate_Start(g_hWnd);
		{
			MSG msgUpd;

			while (InGameUpdate_GetState() == IUS_CHECKING)
			{
				while (PeekMessage(&msgUpd, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msgUpd);
					DispatchMessage(&msgUpd);
				}
				InGameUpdate_RenderScreen(g_hDC);
				SwapBuffers(g_hDC);
				Sleep(16);
			}

			while (true)
			{
				INGAME_UPDATE_STATE updState = InGameUpdate_GetState();
				if (updState == IUS_DONE || updState == IUS_ERROR)
					break;

				while (PeekMessage(&msgUpd, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msgUpd);
					DispatchMessage(&msgUpd);
				}

				InGameUpdate_RenderScreen(g_hDC);
				SwapBuffers(g_hDC);
				Sleep(16);
			}

			DWORD dwEnd = GetTickCount() + 1500;
			while (GetTickCount() < dwEnd)
			{
				while (PeekMessage(&msgUpd, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msgUpd);
					DispatchMessage(&msgUpd);
				}
				InGameUpdate_RenderScreen(g_hDC);
				SwapBuffers(g_hDC);
				Sleep(16);
			}
		}
	}

	//g_ErrorReport.WriteImeInfo( g_hWnd);
	g_ErrorReport.AddSeparator();

	switch (WindowWidth)
	{
	case 800:  FontHeight = 13; break;
	case 1024: FontHeight = 13; break;
	case 1200: FontHeight = 14; break;
	case 1280: FontHeight = 14; break;
	case 1366: FontHeight = 15; break;
	case 1400: FontHeight = 15; break;
	case 1600: FontHeight = 16; break;
	case 1910: FontHeight = 16; break;
	case 1920: FontHeight = 16; break;
	case 2560: FontHeight = 16; break;
	default: FontHeight = 15; break;
	}

	int nFixFontHeight = FontHeight;
	int nFixFontSize;
	int iFontSize;

	iFontSize = FontHeight - 1;
	nFixFontSize = nFixFontHeight - 1;

	if (g_hFont) DeleteObject(g_hFont);
	if (g_hFontBold) DeleteObject(g_hFontBold);
	if (g_hFontBig) DeleteObject(g_hFontBig);
	if (g_hFixFont) DeleteObject(g_hFixFont);

	g_hFont = CreateFont(iFontSize, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, GlobalText[0][0] ? GlobalText[0] : NULL);
	g_hFontBold = CreateFont(iFontSize, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, GlobalText[0][0] ? GlobalText[0] : NULL);
	g_hFontBig = CreateFont(iFontSize * 2, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, GlobalText[0][0] ? GlobalText[0] : NULL);
	g_hFixFont = CreateFont(nFixFontSize, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, GlobalText[18][0] ? GlobalText[18] : NULL);

	if (g_pMercenaryInputBox) g_pMercenaryInputBox->SetFont(g_hFont);
	if (g_pSingleTextInputBox) g_pSingleTextInputBox->SetFont(g_hFont);
	if (g_pSinglePasswdInputBox) g_pSinglePasswdInputBox->SetFont(g_hFont);

	if (g_pRenderText) g_pRenderText->SetFont(g_hFont);

	setlocale(LC_ALL, "english");

	CInput::Instance().Create(g_hWnd, WindowWidth, WindowHeight);

	g_pNewUISystem->Create();

	if (m_MusicOnOff)
	{
		wzAudioCreate(g_hWnd);
		wzAudioOption(WZAOPT_STOPBEFOREPLAY, 1);
	}

	if (m_SoundOnOff)
	{
		InitDirectSound(g_hWnd);
		leaf::CRegKey regkey;
		regkey.SetKey(leaf::CRegKey::_HKEY_CURRENT_USER, "SOFTWARE\\MuOnline\\Config");
		DWORD value;
		if (!regkey.ReadDword("VolumeLevel", value))
		{
			value = 1;
			regkey.WriteDword("VolumeLevel", value);
		}
		if (value < 0 || value >= 10)
			value = 5;

		g_pOption->SetVolumeLevel(int(value));
		SetEffectVolumeLevel(g_pOption->GetVolumeLevel());
	}

	SetTimer(g_hWnd, HACK_TIMER, 20 * 1000, NULL);

	srand((unsigned)time(NULL));
	for (int i = 0; i < 100; i++)
		RandomTable[i] = rand() % 360;

	//memorydump[0]
	RendomMemoryDump = new BYTE[rand() % 100 + 1];


	GateAttribute = new GATE_ATTRIBUTE[MAX_GATES];
	SkillAttribute = new SKILL_ATTRIBUTE[MAX_SKILLS];

	//memorydump[1]
	ItemAttRibuteMemoryDump = new ITEM_ATTRIBUTE[MAX_ITEM + 1024];
	ItemAttribute = ((ITEM_ATTRIBUTE*)ItemAttRibuteMemoryDump) + rand() % 1024;

	//memorydump[2]
	CharacterMemoryDump = new CHARACTER[MAX_CHARACTERS_CLIENT + 1 + 128];
	CharactersClient = ((CHARACTER*)CharacterMemoryDump) + rand() % 128;
	CharacterMachine = new CHARACTER_MACHINE;

	memset(GateAttribute, 0, sizeof(GATE_ATTRIBUTE) * (MAX_GATES));
	memset(ItemAttribute, 0, sizeof(ITEM_ATTRIBUTE) * (MAX_ITEM));
	memset(SkillAttribute, 0, sizeof(SKILL_ATTRIBUTE) * (MAX_SKILLS));
	memset(CharacterMachine, 0, sizeof(CHARACTER_MACHINE));

	CharacterAttribute = &CharacterMachine->Character;
	CharacterMachine->Init();
	Hero = &CharactersClient[0];

	if (g_iChatInputType == 1)
	{
		g_pMercenaryInputBox = new CUIMercenaryInputBox;
		g_pSingleTextInputBox = new CUITextInputBox;
		g_pSinglePasswdInputBox = new CUITextInputBox;
	}

	g_pChatRoomSocketList = new CChatRoomSocketList;
	g_pUIManager = new CUIManager;
	g_pUIMapName = new CUIMapName;	// rozy
	g_pTimer = new CTimer();

#ifdef MOVIE_DIRECTSHOW
	g_pMovieScene = new CMovieScene;
#endif // MOVIE_DIRECTSHOW

	g_BuffSystem = BuffStateSystem::Make();

	g_MapProcess = MapProcess::Make();

	g_petProcess = PetProcess::Make();

	CUIMng::Instance().Create();

	if (g_iChatInputType == 1)
	{
		g_pMercenaryInputBox->Init(g_hWnd);
		g_pSingleTextInputBox->Init(g_hWnd, 200, 20);
		g_pSinglePasswdInputBox->Init(g_hWnd, 200, 20, 9, TRUE);
		g_pSingleTextInputBox->SetState(UISTATE_HIDE);
		g_pSinglePasswdInputBox->SetState(UISTATE_HIDE);

		g_pMercenaryInputBox->SetFont(g_hFont);
		g_pSingleTextInputBox->SetFont(g_hFont);
		g_pSinglePasswdInputBox->SetFont(g_hFont);

		g_bIMEBlock = FALSE;
		HIMC  hIMC = ImmGetContext(g_hWnd);
		ImmSetConversionStatus(hIMC, IME_CMODE_ALPHANUMERIC, IME_SMODE_NONE);
		ImmReleaseContext(g_hWnd, hIMC);
		SaveIMEStatus();
		g_bIMEBlock = TRUE;
	}
#if (defined WINDOWMODE)
	if (g_bUseWindowMode == FALSE)
	{
		int nOldVal;
		SystemParametersInfo(SPI_SCREENSAVERRUNNING, 1, &nOldVal, 0);
		SystemParametersInfo(SPI_GETSCREENSAVETIMEOUT, 0, &g_iScreenSaverOldValue, 0);
		SystemParametersInfo(SPI_SETSCREENSAVETIMEOUT, 300 * 60, NULL, 0);
	}
#else
#ifdef NDEBUG
#ifndef FOR_WORK
#ifdef ACTIVE_FOCUS_OUT
	if (g_bUseWindowMode == FALSE)
	{
#endif	// ACTIVE_FOCUS_OUT
		int nOldVal; // °ªÀÌ µé¾î°¥ ÇÊ¿ä°¡ ¾øÀ½
		SystemParametersInfo(SPI_SCREENSAVERRUNNING, 1, &nOldVal, 0);  // ´ÜÃàÅ°¸¦ ¸ø¾²°Ô ÇÔ
		SystemParametersInfo(SPI_GETSCREENSAVETIMEOUT, 0, &g_iScreenSaverOldValue, 0);  // ½ºÅ©¸°¼¼ÀÌ¹ö Â÷´Ü
		SystemParametersInfo(SPI_SETSCREENSAVETIMEOUT, 300 * 60, NULL, 0);  // ½ºÅ©¸°¼¼ÀÌ¹ö Â÷´Ü
#ifdef ACTIVE_FOCUS_OUT
	}
#endif	// ACTIVE_FOCUS_OUT
#endif
#endif
#endif	//WINDOWMODE(#else)

#ifdef SAVE_PACKET
	DeleteFile(PACKET_SAVE_FILE);
#endif

#if defined PROTECT_SYSTEMKEY && defined NDEBUG
#ifndef FOR_WORK
	ProtectSysKey::AttachProtectSysKey(g_hInst, g_hWnd);
#endif // !FOR_WORK
#endif // PROTECT_SYSTEMKEY && NDEBUG

	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0))
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (g_bInTray)
			{
				DWORD currentTime = GetTickCount();
				if (currentTime - g_dwLastTrayFrameTime >= 5000)
				{
					g_dwLastTrayFrameTime = currentTime;
					g_dwLastTrayLogicTime = currentTime;

#if (defined WINDOWMODE)
					if (g_bUseWindowMode == TRUE)
					{
						Scene(g_hDC);           // Window Mode
					}
					else
					{
						Scene(g_hDC);           // Fullscreen - agora roda SEMPRE (mesmo sem foco)
					}
#else
					Scene(g_hDC);               // Versão antiga
#endif
				}
				else
				{
					Sleep(650);
				}
			}
			else
			{
#if (defined WINDOWMODE)
				if (g_bUseWindowMode == TRUE)
				{
					Scene(g_hDC);
				}
				else if (g_bWndActive)
				{
					Scene(g_hDC);
				}
#ifndef FOR_WORK
				else if (g_bUseWindowMode == FALSE)
				{
					// seu código antigo de minimized (deixe como estava)
				}
#endif
#else
				if (g_bWndActive)
					Scene(g_hDC);
#endif
			}
		}

#ifdef NEW_PROTOCOL_SYSTEM
		if (SceneFlag < CHARACTER_SCENE)
			ProtocolCompiler();

		g_pChatRoomSocketList->ProtocolCompile();
		gProtocolSend.RecvMessage();
#else
		ProtocolCompiler();
		g_pChatRoomSocketList->ProtocolCompile();
#endif


	} // while( 1 )

	DestroyWindow();

	ExitProcess(0);
	return msg.wParam;
}

