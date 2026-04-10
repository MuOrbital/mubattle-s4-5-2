#pragma once

typedef unsigned __int64 QWORD;

#define WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT _WIN32_WINNT_WINXP

#define HACKSERVER_VERSION "1.7.0"

#define PROTECT_STATE 0

#define USE_SCREENSHOW 0

// System Include
#include <windows.h>
#include <winsock2.h>
#include <process.h>
#include <iostream>
using namespace std;
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <Rpc.h>
#include <dbghelp.h>
#include <Psapi.h>
#include <fstream>
#include <sstream>



//#include "detours.h"
#include <Shlwapi.h>
#include <wchar.h>
#include <winternl.h>
#include <winioctl.h>

#include <commctrl.h>


#include <atlimage.h>
#include <atltime.h>


#include "..\\..\\Util\\libpugixml\\pugixml\\pugixml.hpp"

#include "..\\..\\Util\\VMProtect\\VMProtectSDK.h"

#pragma comment(lib, "comctl32.lib")

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Psapi.lib")
#pragma comment(lib,"Shlwapi.lib")
//#pragma comment(lib,"detours.lib")
#pragma comment(lib,"Rpcrt4.lib")

#include <Gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

#pragma comment(lib,"dbghelp.lib")
//#pragma comment(lib, "libcurl.lib")

#pragma comment(lib, "crypt32.lib")
#pragma comment(lib,"wldap32.lib")
#pragma comment(lib,"Normaliz.lib")

#define CURL_STATICLIB

#ifdef _DEBUG
#pragma comment(lib,"..\\..\\..\\Util\\curl\\libcurl_a_debug.lib")
#else
#pragma comment(lib,"..\\..\\Util\\curl\\libcurl_a.lib")
#endif