#pragma once

#include "CustomJewel.h"

struct MAIN_FILE_INFO
{
	//MainInfo
	char CustomerName[32];
	char IpAddress[32];
	WORD IpAddressPort;
	char ClientVersion[8];
	char ClientSerial[17];
	char WindowName[32];
	char ScreenShotPath[50];
	int DebugConsole;

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

	//Interface
	int m_DisableClassSUM;
	int m_WideScreenType;
	int m_VideoLogin;

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