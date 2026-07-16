#include "stdafx.h"
#include "CCRC32.h"
#include "ThemidaSDK.h"
#include "CustomJewel.h"
#include "EventEntryLevel.h"
#include "LuaProtect.h"
#include "CustomMessage.h"
#include "MemScript.h"

#define MACRO_CONFIG_TXT STRING_CRIPT(".\\Configs.txt")
#define MCRO_DEFINE_CONFIG STRING_CRIPT("Configs")
#define UPDATE_COLOR_COUNT 26
#define UPDATE_TIP_MAX 32
#define UPDATE_TIP_LENGTH 256

void GerateConfig();
//void GerateCustomJewels();
void EncryptLua();
void DecryptLua();
void LoadUpdateColors(struct UPDATE_COLOR_INFO* colors);
void LoadUpdateTips(char tips[UPDATE_TIP_MAX][UPDATE_TIP_LENGTH], int* count);

static BYTE bBuxCode[3] = { 0xFC, 0xCF, 0xAB };

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

	//Launcher Updater
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
	// Must stay byte-for-byte aligned with Source/Main/source/Protect.h.
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

struct LOAD_CUSTOM_JEWEL_INFO
{
	CUSTOM_JEWEL_INFO CustomJewelInfo[MAX_CUSTOM_JEWEL];
};

static BYTE ClampColorValue(int value)
{
	if (value < 0)
	{
		return 0;
	}

	if (value > 255)
	{
		return 255;
	}

	return (BYTE)value;
}

void LoadUpdateColors(UPDATE_COLOR_INFO* colors)
{
	static const BYTE DefaultColors[UPDATE_COLOR_COUNT][3] =
	{
		{ 3, 6, 18 },
		{ 3, 13, 71 },
		{ 179, 217, 255 },
		{ 5, 26, 140 },
		{ 0, 140, 230 },
		{ 13, 64, 204 },
		{ 0, 102, 230 },
		{ 5, 153, 255 },
		{ 13, 51, 191 },
		{ 13, 26, 204 },
		{ 0, 140, 230 },
		{ 13, 64, 179 },
		{ 20, 89, 230 },
		{ 23, 31, 255 },
		{ 51, 153, 255 },
		{ 204, 235, 255 },
		{ 0, 0, 0 },
		{ 13, 18, 31 },
		{ 0, 51, 191 },
		{ 26, 140, 255 },
		{ 51, 153, 255 },
		{ 26, 140, 255 },
		{ 255, 255, 255 },
		{ 255, 255, 255 },
		{ 255, 255, 255 },
		{ 255, 255, 255 },
	};

	for (int n = 0; n < UPDATE_COLOR_COUNT; n++)
	{
		colors[n].Red = DefaultColors[n][0];
		colors[n].Green = DefaultColors[n][1];
		colors[n].Blue = DefaultColors[n][2];
	}

	CMemScript script;

	if (script.SetBuffer(".\\UpdateColors.txt") == 0)
	{
		return;
	}

	try
	{
		while (true)
		{
			eTokenResult token = script.GetToken();

			if (token == TOKEN_END)
			{
				break;
			}

			if (token == TOKEN_STRING)
			{
				if (strcmp(script.GetString(), "end") == 0)
				{
					break;
				}

				continue;
			}

			if (token != TOKEN_NUMBER)
			{
				continue;
			}

			int id = script.GetNumber();
			int red = script.GetAsNumber();
			int green = script.GetAsNumber();
			int blue = script.GetAsNumber();

			if (id < 1 || id > UPDATE_COLOR_COUNT)
			{
				continue;
			}

			colors[id - 1].Red = ClampColorValue(red);
			colors[id - 1].Green = ClampColorValue(green);
			colors[id - 1].Blue = ClampColorValue(blue);
		}
	}
	catch (...)
	{
		printf("UpdateColors.txt possui uma configuracao invalida. As cores validas anteriores foram mantidas.\n");
	}
}

void LoadUpdateTips(char tips[UPDATE_TIP_MAX][UPDATE_TIP_LENGTH], int* count)
{
	static const char* const DefaultTips[] =
	{
		"Classe BK - Use Control + Z para combar de Fenrir.",
		"ALT + ENTER para habilitar modo borda infinita.",
		"F10 para habilitar a Camera3D - F11 restaura.",
		"Shift 2x para habilitar AutoPotion interno.",
		"Control 2x para habilitar Modo PVP.",
		"Tecla L para abrir o Marketplace.",
		"Chaos Machine: use TCA para nao perder itens de +10 a +13.",
		"Chaos Machine: use TOL para aumentar a chance de sucesso.",
		"Comando: use /selupan para ir ao Boss Selupan.",
		"Comando: use /refinaria para ir a torre de Refinaria.",
		"Comando: use /treta para ir para Devias 4.",
		"FREE: 1.000rr = 1mr + 1 PontoMR | VIP: 900rr = 1mr + 1 PontoMR.",
		"Lucky Wheel por WCoinC no NPC em frente ao Bar de Lorencia.",
		"Bosses tem 50% de chance de drop para o TOP 1, 2 e 3 Damage.",
		"Quem joga sempre evolui, quem chora fica para tras.",
		"Elfa ou Summoner pedindo itens dizendo que e menina? Confia.",
	};

	memset(tips, 0, UPDATE_TIP_MAX * UPDATE_TIP_LENGTH);

	int defaultCount = (int)(sizeof(DefaultTips) / sizeof(DefaultTips[0]));

	for (int n = 0; n < defaultCount; n++)
	{
		strncpy_s(tips[n], UPDATE_TIP_LENGTH, DefaultTips[n], _TRUNCATE);
	}

	(*count) = defaultCount;

	CMemScript script;

	if (script.SetBuffer(".\\UpdateColors.txt") == 0)
	{
		return;
	}

	char parsedTips[UPDATE_TIP_MAX][UPDATE_TIP_LENGTH] = {};
	bool tipsSection = false;
	bool hasParsedTips = false;

	try
	{
		while (true)
		{
			eTokenResult token = script.GetToken();

			if (token == TOKEN_END)
			{
				break;
			}

			if (token == TOKEN_STRING)
			{
				if (strcmp(script.GetString(), "tips") == 0)
				{
					tipsSection = true;
					continue;
				}

				if (tipsSection && strcmp(script.GetString(), "end") == 0)
				{
					break;
				}

				continue;
			}

			if (tipsSection == false || token != TOKEN_NUMBER)
			{
				continue;
			}

			int id = script.GetNumber();
			char* text = script.GetAsString();

			if (id < 1 || id > UPDATE_TIP_MAX || text[0] == 0)
			{
				continue;
			}

			strncpy_s(parsedTips[id - 1], UPDATE_TIP_LENGTH, text, _TRUNCATE);
			hasParsedTips = true;
		}
	}
	catch (...)
	{
		return;
	}

	if (hasParsedTips == false)
	{
		return;
	}

	memset(tips, 0, UPDATE_TIP_MAX * UPDATE_TIP_LENGTH);
	(*count) = 0;

	for (int n = 0; n < UPDATE_TIP_MAX; n++)
	{
		if (parsedTips[n][0] == 0)
		{
			continue;
		}

		strncpy_s(tips[*count], UPDATE_TIP_LENGTH, parsedTips[n], _TRUNCATE);
		(*count)++;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	printf("MuOnline: \n");
	printf("Escolha uma opcao de 1 a 3: \n");

	printf("1 - Gerar Config \n");

	printf("2 - Encript Lua \n");

	printf("3 - Gerar Tudo \n");

	int execute = 0;

	scanf_s("%i", &execute);

	switch (execute)
	{
		case 1:
			GerateConfig();
			printf("Arquivo gerado com sucesso! \n");
		break;
		case 2:
			EncryptLua();
			printf("Scripts Lua encriptados em LuaScripts/Decripted! \n");
		break;
		case 3:
			GerateConfig();
			EncryptLua();
			printf("Arquivo gerado com sucesso! \n");
			printf("Scripts Lua encriptados em LuaScripts/Decripted! \n");
		break;

	default:
		printf("Opcao invalida, escolha de 1 a 3 \n");
		break;
	}

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
	info.MHPServerPort = GetPrivateProfileInt("MainInfo", "MHPServerPort", 55999, ".\\MainInfo.ini");
	info.m_UseIntegratedAntiHack = GetPrivateProfileInt("MainInfo", "UseIntegratedAntiHack", 1, ".\\MainInfo.ini");
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

	info.m_AutoUpdateCpanel = GetPrivateProfileInt("IntegratedLauncher", "AutoUpdateCpanel", 1, ".\\MainInfo.ini");
	GetPrivateProfileString("IntegratedLauncher", "AutoUpdateURL", "", info.m_AutoUpdateURL, sizeof(info.m_AutoUpdateURL), ".\\MainInfo.ini");
	GetPrivateProfileString("IntegratedLauncher", "TitleName", "", info.m_TitleName, sizeof(info.m_TitleName), ".\\MainInfo.ini");
	GetPrivateProfileString("IntegratedLauncher", "SubTitleName", "", info.m_SubTitleName, sizeof(info.m_SubTitleName), ".\\MainInfo.ini");
	GetPrivateProfileString("IntegratedLauncher", "WebSiteURL", "", info.m_WebSiteURL, sizeof(info.m_WebSiteURL), ".\\MainInfo.ini");
	LoadUpdateColors(info.m_UpdateColors);
	int loadedUpdateTips = 0;
	LoadUpdateTips(info.m_UpdateTips, &loadedUpdateTips);
	info.m_UpdateTipsCount = GetPrivateProfileInt("IntegratedLauncher", "MaxUpdateTips", 16, ".\\MainInfo.ini");

	if (info.m_UpdateTipsCount < 0)
	{
		info.m_UpdateTipsCount = 0;
	}

	if (info.m_UpdateTipsCount > UPDATE_TIP_MAX)
	{
		info.m_UpdateTipsCount = UPDATE_TIP_MAX;
	}

	if (info.m_UpdateTipsCount > loadedUpdateTips)
	{
		info.m_UpdateTipsCount = loadedUpdateTips;
	}

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
	info.m_VisualBugHPFerrarezi = GetPrivateProfileInt("Interface", "VisualBugHPFerrarezi", 0, ".\\MainInfo.ini");
	info.m_SelectServerCustom = GetPrivateProfileInt("Interface", "SelectServerCustom", 0, ".\\MainInfo.ini");
	// MuHelper is opt-in so existing clients keep the current coordinate interface by default.
	info.m_MuHelperEnabled = GetPrivateProfileInt("HelperInfo", "MuHelperEnabled", 0, ".\\MainInfo.ini");
	info.m_HelperActiveAlert = GetPrivateProfileInt("HelperInfo", "HelperActiveAlert", 0, ".\\MainInfo.ini");
	info.m_HelperActiveLevel = GetPrivateProfileInt("HelperInfo", "HelperActiveLevel", 80, ".\\MainInfo.ini");

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

void DecryptLua()
{
	GetPrivateProfileString("LuaSystemClient", "PrivateCode", "", gFileProtectLua.m_PrivateCode, sizeof(gFileProtectLua.m_PrivateCode), ".\\MainInfo.ini");

	Sleep(20);

	gFileProtectLua.FindFileDecrypt("LuaScripts\\Encrypted\\", "LuaScripts\\Decrypted\\");
}
