#include "stdafx.h"
#include "LuaReg.h"
#include "LuaStack.hpp"

#pragma comment (lib, "Advapi32.lib")

using namespace luaaa;

int getValueRegKey(char* keyValue) {
	HKEY key;

	int m_Value = -1;

	if (RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Webzen\\Mu\\Config", 0, KEY_ALL_ACCESS, &key) == ERROR_SUCCESS) {
		DWORD type = REG_DWORD, size = sizeof(m_Value);

		RegQueryValueExA(key, keyValue, 0, &type, (LPBYTE)&m_Value, &size);
	}

	RegCloseKey(key);

	return m_Value;
}

void setValueRegKey(char* keyValue, int value) {
	HKEY hKey;

	if (RegCreateKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Webzen\\Mu\\Config", 0, 0, 0, KEY_WRITE, 0, &hKey, NULL) == ERROR_SUCCESS)
	{
		RegSetValueExA(hKey, keyValue, 0, REG_DWORD, (LPBYTE)&value, sizeof(value));
	}

	RegCloseKey(hKey);
}

char* getStringRegKey(char* keyValue, int keySize) {
	HKEY key;

	char buffer[256];

	if (RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Webzen\\Mu\\Config", 0, KEY_ALL_ACCESS, &key) == ERROR_SUCCESS) {
		DWORD type = REG_SZ, size = keySize;

		RegQueryValueExA(key, keyValue, 0, &type, (LPBYTE)&buffer, &size);
	}

	RegCloseKey(key);

	return &buffer[0];
}

void setStringRegKey(char* keyValue, char* value) {
	HKEY hKey;

	char buffer[256];

	strcpy_s(buffer, value);

	if (RegCreateKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Webzen\\Mu\\Config", 0, 0, 0, KEY_WRITE, 0, &hKey, NULL) == ERROR_SUCCESS)
	{
		RegSetValueExA(hKey, keyValue, 0, REG_SZ, (LPBYTE)&buffer, strlen(value));
	}

	RegCloseKey(hKey);
}


void RegisterLuaReg(lua_State * lua)
{
	LuaModule(lua).fun("getValueRegKey", &getValueRegKey);
	LuaModule(lua).fun("setValueRegKey", &setValueRegKey);

	LuaModule(lua).fun("getStringRegKey", &getStringRegKey);
	LuaModule(lua).fun("setStringRegKey", &setStringRegKey);
}
