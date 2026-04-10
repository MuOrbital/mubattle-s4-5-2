#pragma once

typedef unsigned __int64 QWORD;

#define WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT _WIN32_WINNT_WINXP

#define HACKCLIENT_VERSION "1.7.0"

#define PSAPI_VERSION 1

#define USE_SCREENSHOW 1

#define ENCRYPT_LOG 0

#define MAIN_SCREEN_STATE 0x00E609E8

// System Include
#include <windows.h>
#include <iostream>
#include <map>
#include <math.h>
#include <stdlib.h>
#include <winsock2.h>
#include <vector>
#include <Psapi.h>
#include "detours.h"
#include <Shlwapi.h>
#include <Dbghelp.h>
#include <wchar.h>
#include <Rpc.h>
#include <winternl.h>
#include <winioctl.h>
#include <fstream>

#include <atlimage.h>

#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Psapi.lib")
#pragma comment(lib,"Shlwapi.lib")
#pragma comment(lib,"Dbghelp.lib")
#pragma comment(lib,"detours.lib")
#pragma comment(lib,"Rpcrt4.lib")
