#pragma once
#include <windows.h>
#include <string>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

#define INGAME_UPDATE_FILE_PATTERN  "file_update"
#define INGAME_UPDATE_EXE_PATTERN   "exe_update"
#define INGAME_UPDATE_INI_PATH      "Data\\GameLoad\\Config\\GameMain\\Config.ini"
#define INGAME_UPDATE_INI_EXE_PATH  "Data\\GameLoad\\Config\\Config.ini"

enum INGAME_UPDATE_STATE
{
    IUS_IDLE = 0,
    IUS_CHECKING,
    IUS_DOWNLOADING_FILES,
    IUS_EXTRACTING_FILES,
    IUS_NEED_EXE_UPDATE,
    IUS_LOADING_GAME,
    IUS_DONE,
    IUS_ERROR
};

struct InGameUpdateStatus
{
    INGAME_UPDATE_STATE state;
    float               progress;
    char                statusText[256];
    char                subText[256];
    int                 localVersion;
    int                 serverFileVer;
    int                 serverExeVer;
    int                 extractedFiles;
    int                 totalFiles;
    char                currentFile[MAX_PATH];
    char                currentDir[MAX_PATH];
};

void InGameUpdate_Start(HWND hWnd);
INGAME_UPDATE_STATE InGameUpdate_GetState();
void InGameUpdate_GetStatus(InGameUpdateStatus& out);
void InGameUpdate_RenderScreen(HDC hDC);
void InGameUpdate_Shutdown();

void InGameUpdate_SetLoadingProgress(float pct, const char* text);
void InGameUpdate_RunAsUpdater(const char* cmdLine);