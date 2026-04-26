#include "stdafx.h"
#include "CCRC32.h"
#include "ThemidaSDK.h"
#include "CustomJewel.h"
#include "EventEntryLevel.h"
#include "LuaProtect.h"
#include "CustomMessage.h"

#define MACRO_CONFIG_TXT STRING_CRIPT(".\\Configs.txt")
#define MCRO_DEFINE_CONFIG STRING_CRIPT("Configs")

void GerateConfig();
//void GerateCustomJewels();
void EncryptLua();

static BYTE bBuxCode[3] = { 0xFC, 0xCF, 0xAB };

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
	int m_OpenLauncher;
	char m_LauncherName[260];
	int DebugConsole;
	int m_MaxInstance;
	int m_RequireAdmin;
	int m_RequireAdminMessage;
	int m_BlockVirtualMachine;

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

struct LOAD_CUSTOM_JEWEL_INFO
{
	CUSTOM_JEWEL_INFO CustomJewelInfo[MAX_CUSTOM_JEWEL];
};

int _tmain(int argc, _TCHAR* argv[])
{
	printf("What do you want to do?: \n");

	printf("1 - Gerar Config \n");

	printf("2 - Encript Lua \n");

	printf("3 - Gerar Tudo \n");

	int execute = 0;

	scanf_s("%i", &execute);

	if (execute == 1)
	{
		GerateConfig();
	}

	else if (execute == 2)
	{
		EncryptLua();
	}
	else
	{
		GerateConfig();
		EncryptLua();
	}

	printf("Arquivo gerado com sucesso! \n");

	system("pause");
}


void GerateConfig()// OK
{
	CLEAR_START

		ENCODE_START

		MAIN_FILE_INFO info;

	memset(&info, 0, sizeof(info));
	//MainInfo
	info.IpAddressPort = GetPrivateProfileInt("MainInfo", "IpAddressPort", 44405, ".\\MainInfo.ini");
	GetPrivateProfileString("MainInfo", "CustomerName", "", info.CustomerName, sizeof(info.CustomerName), ".\\MainInfo.ini");
	GetPrivateProfileString("MainInfo", "IpAddress", "", info.IpAddress, sizeof(info.IpAddress), ".\\MainInfo.ini");
	GetPrivateProfileString("MainInfo", "ClientVersion", "", info.ClientVersion, sizeof(info.ClientVersion), ".\\MainInfo.ini");
	GetPrivateProfileString("MainInfo", "ClientSerial", "", info.ClientSerial, sizeof(info.ClientSerial), ".\\MainInfo.ini");
	GetPrivateProfileString("MainInfo", "WindowName", "", info.WindowName, sizeof(info.WindowName), ".\\MainInfo.ini");
	GetPrivateProfileString("MainInfo", "ScreenShotPath", "", info.ScreenShotPath, sizeof(info.ScreenShotPath), ".\\MainInfo.ini");

	info.m_OpenLauncher = GetPrivateProfileInt("MainInfo", "OpenLauncher", 1, ".\\MainInfo.ini");
	GetPrivateProfileString("MainInfo", "LauncherName", "Launcher.exe", info.m_LauncherName, sizeof(info.m_LauncherName), ".\\MainInfo.ini");

	info.DebugConsole = GetPrivateProfileInt("MainInfo", "DebugConsole", 1, ".\\MainInfo.ini");
	info.m_MaxInstance = GetPrivateProfileInt("MainInfo", "MaxInstance", 2, ".\\MainInfo.ini");
	info.m_RequireAdmin = GetPrivateProfileInt("MainInfo", "RequireAdmin", 1, ".\\MainInfo.ini");
	info.m_RequireAdminMessage = GetPrivateProfileInt("MainInfo", "RequireAdminMessage", 1, ".\\MainInfo.ini");
	info.m_BlockVirtualMachine = GetPrivateProfileInt("MainInfo", "BlockVirtualMachine", 1, ".\\MainInfo.ini");

	//PluginSystem
	GetPrivateProfileString("PluginSystem", "PluginName1", "", info.PluginName1, sizeof(info.PluginName1), ".\\MainInfo.ini");
	GetPrivateProfileString("PluginSystem", "PluginName2", "", info.PluginName2, sizeof(info.PluginName2), ".\\MainInfo.ini");
	GetPrivateProfileString("PluginSystem", "PluginName3", "", info.PluginName3, sizeof(info.PluginName3), ".\\MainInfo.ini");
	GetPrivateProfileString("PluginSystem", "PluginName4", "", info.PluginName4, sizeof(info.PluginName4), ".\\MainInfo.ini");
	GetPrivateProfileString("PluginSystem", "PluginName5", "", info.PluginName5, sizeof(info.PluginName5), ".\\MainInfo.ini");
	info.m_AntiBypassDLL = GetPrivateProfileInt("PluginSystem", "AntiBypassDLL", 1, ".\\MainInfo.ini");
	//LuaSystemClient
	info.ReloadLua = GetPrivateProfileInt("LuaSystemClient", "ReloadLua", 0, ".\\MainInfo.ini");
	info.LuaCrypt = GetPrivateProfileInt("LuaSystemClient", "OnlyCryptedLua", 0, ".\\MainInfo.ini");
	GetPrivateProfileString("LuaSystemClient", "PrivateCode", "", info.m_PrivateCode, sizeof(info.m_PrivateCode), ".\\MainInfo.ini");
	//SystemRequirements
	info.m_CheckRequeriments = GetPrivateProfileInt("SystemRequirements", "CheckRequeriments", 0, ".\\MainInfo.ini");
	info.m_CheckWindowsVersion = GetPrivateProfileInt("SystemRequirements", "CheckWindowsVersion", 7, ".\\MainInfo.ini");
	info.m_MinMemoryRAM = GetPrivateProfileInt("SystemRequirements", "MinMemoryRAMCheck", 2, ".\\MainInfo.ini");
	info.m_MinCPUCore = GetPrivateProfileInt("SystemRequirements", "MinCPUCoreCheck", 2, ".\\MainInfo.ini");
	info.m_MinVideoMemory = GetPrivateProfileInt("SystemRequirements", "MinVideoMemory", 2, ".\\MainInfo.ini");

	//Interface
	info.m_DisableClassSUM = GetPrivateProfileInt("Interface", "DisableClassSUM", 0, ".\\MainInfo.ini");
	info.m_WideScreenType = GetPrivateProfileInt("Interface", "WideScreenType", 0, ".\\MainInfo.ini");
	info.m_VideoLogin = GetPrivateProfileInt("Interface", "VideoLogin", 0, ".\\MainInfo.ini");

	//Custom
	info.LoadingLegend = GetPrivateProfileInt("Custom", "LoadingLegend", 1, ".\\MainInfo.ini");
	info.LoadingImageCount = GetPrivateProfileInt("Custom", "LoadingImageCount", 1, ".\\MainInfo.ini");
	info.m_GMGiftPlayerTrade = GetPrivateProfileInt("Custom", "GMGiftPlayerTrade", 0, ".\\MainInfo.ini");
	info.m_TCAAllowTrade = GetPrivateProfileInt("Custom", "TCAAllowTrade", 0, ".\\MainInfo.ini");
	info.m_LuckyTalismanAllowTrade = GetPrivateProfileInt("Custom", "LuckyTalismanAllowTrade", 0, ".\\MainInfo.ini");
	info.ShowFPSWindow = GetPrivateProfileInt("Custom", "ShowFPSWindow", 0, ".\\MainInfo.ini");
	info.m_SkillDisableEffect = GetPrivateProfileInt("Custom", "SkillDisableEffect", 0, ".\\MainInfo.ini");
	info.m_BlockSellTemporayNPC = GetPrivateProfileInt("Custom", "BlockSellTemporayNPC", 0, ".\\MainInfo.ini");
	info.m_MountMiniSafeZone = GetPrivateProfileInt("Custom", "MountMiniSafeZone", 0, ".\\MainInfo.ini");
	info.m_TooltipAnimation = GetPrivateProfileInt("Custom", "TooltipAnimation", 0, ".\\MainInfo.ini");
	info.m_NoDelayPotion = GetPrivateProfileInt("Custom", "NoDelayPotion", 0, ".\\MainInfo.ini");

	//HardwareID
	info.m_GetHWID = GetPrivateProfileInt("HardwareID", "GetHWID", 0, ".\\MainInfo.ini");
	info.m_ShowHWID = GetPrivateProfileInt("HardwareID", "ShowHWID", 0, ".\\MainInfo.ini");
	GetPrivateProfileString("HardwareID", "CheckBlockHWID", "", info.m_CheckBlockHWID, sizeof(info.m_CheckBlockHWID), ".\\MainInfo.ini");

	CCRC32 CRC32;

	if (CRC32.FileCRC(info.PluginName1, &info.Plugin1CRC32, 1024) == 0)
	{
		info.Plugin1CRC32 = 0;
	}

	if (CRC32.FileCRC(info.PluginName2, &info.Plugin2CRC32, 1024) == 0)
	{
		info.Plugin2CRC32 = 0;
	}

	if (CRC32.FileCRC(info.PluginName3, &info.Plugin3CRC32, 1024) == 0)
	{
		info.Plugin3CRC32 = 0;
	}

	if (CRC32.FileCRC(info.PluginName4, &info.Plugin4CRC32, 1024) == 0)
	{
		info.Plugin4CRC32 = 0;
	}

	if (CRC32.FileCRC(info.PluginName5, &info.Plugin5CRC32, 1024) == 0)
	{
		info.Plugin5CRC32 = 0;
	}

	for (int n = 0; n < sizeof(MAIN_FILE_INFO); n++)
	{
		((BYTE*)&info)[n] ^= bBuxCode[n % 3];
	}

	CreateDirectory("Data", NULL);
	CreateDirectory("Data\\Local", NULL);

	DeleteFile("Data\\Local\\info.bmd");
	HANDLE file = CreateFile("Data\\Local\\info.bmd", GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, 0);


	if (file == INVALID_HANDLE_VALUE)
	{
		return;
	}

	DWORD OutSize = 0;

	if (WriteFile(file, &info, sizeof(MAIN_FILE_INFO), &OutSize, 0) == 0)
	{
		CloseHandle(file);
		return;
	}

	CloseHandle(file);

	ENCODE_END

		CLEAR_END
}

void EncryptLua() {
	GetPrivateProfileString("LuaSystemClient", "PrivateCode", "", gFileProtectLua.m_PrivateCode, sizeof(gFileProtectLua.m_PrivateCode), ".\\MainInfo.ini");

	Sleep(20);

	gFileProtectLua.FindFile("LuaScripts\\Decrypted\\", "LuaScripts\\Encrypted\\");
}