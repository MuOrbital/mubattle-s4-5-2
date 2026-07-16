#pragma once

#include "CustomJewel.h"

#define UPDATE_COLOR_COUNT 26
#define UPDATE_TIP_MAX 32
#define UPDATE_TIP_LENGTH 256

struct UPDATE_COLOR_INFO
{
	BYTE Red;
	BYTE Green;
	BYTE Blue;
};

struct MAIN_FILE_INFO
{
	//MainInfo
	char CustomerName[32];
	char IpAddress[32];
	WORD IpAddressPort;
	WORD MHPServerPort;
	int m_UseIntegratedAntiHack;
	char ClientVersion[8];
	char ClientSerial[17];
	char WindowName[32];
	char ScreenShotPath[50];
	int m_OpenLauncher;
	char m_LauncherName[260];
	int DebugConsole;
	int m_MaxInstance;
	int m_RequireAdmin;
	int m_RequireAdminMessage;
	int m_BlockVirtualMachine;

	int m_AutoUpdateCpanel;
	char m_AutoUpdateURL[150];
	char m_TitleName[150];
	char m_SubTitleName[150];
	char m_WebSiteURL[150];
	UPDATE_COLOR_INFO m_UpdateColors[UPDATE_COLOR_COUNT];
	int m_UpdateTipsCount;
	char m_UpdateTips[UPDATE_TIP_MAX][UPDATE_TIP_LENGTH];

	//PluginSystem
	int m_AntiBypassDLL;
	char PluginName1[32];
	char PluginName2[32];
	char PluginName3[32];
	char PluginName4[32];
	char PluginName5[32];
	DWORD Plugin1CRC32;
	DWORD Plugin2CRC32;
	DWORD Plugin3CRC32;
	DWORD Plugin4CRC32;
	DWORD Plugin5CRC32;

	//LuaSystemClient
	BYTE ReloadLua;
	BYTE LuaCrypt;
	char m_PrivateCode[125];

	//SystemRequirements
	DWORD m_CheckRequeriments;
	DWORD m_CheckWindowsVersion;
	DWORD m_MinMemoryRAM;
	DWORD m_MinCPUCore;
	DWORD m_MinVideoMemory;

	//Interface
	int m_DisableClassSUM;
	int m_WideScreenType;
	int m_VideoLogin;
	int m_VisualBugHPFerrarezi;
	int m_SelectServerCustom;
	// Official MuHelper port: controls the client UI/AI without replacing the legacy coordinate HUD.
	int m_MuHelperEnabled;
	int m_HelperActiveAlert;
	int m_HelperActiveLevel;

	//Custom
	int LoadingLegend;
	int LoadingImageCount;
	int m_GMGiftPlayerTrade;
	int m_TCAAllowTrade;
	int m_LuckyTalismanAllowTrade;
	int m_SkillDisableEffect;
	int ShowFPSWindow;
	int m_BlockSellTemporayNPC;
	int m_MountMiniSafeZone;
	int m_TooltipAnimation;
	int m_NoDelayPotion;

	//Custom
	int m_GetHWID;
	int m_ShowHWID;
	char m_CheckBlockHWID[50000];
};


class CProtect
{
public:
	CProtect();
	virtual ~CProtect();
	bool ReadMainFile(char* Path);
	void CheckPlugin1File();
	void CheckPlugin2File();
	void CheckPlugin3File();
	void CheckPlugin4File();
	void CheckPlugin5File();
public:
	MAIN_FILE_INFO m_MainInfo;
	DWORD m_ClientFileCRC;
};

extern CProtect* gProtect;
