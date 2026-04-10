#pragma once

#include "ServerDisplayer.h"

#define CRASH_APPLICATION_MACRO _asm _emit 0xFF _asm _emit 0x25 _asm _emit 0x00 _asm _emit 0x00 _asm _emit 0x00 _asm _emit 0x00

enum eClientDisconnectType
{
	CLIENT_DISCONNECT_NONE = 0,
	CLIENT_DISCONNECT_DUMP_DETECTION = 1,
	CLIENT_DISCONNECT_EXECUTABLE_DETECTION = 2,
	CLIENT_DISCONNECT_FILE_DETECTION = 3,
	CLIENT_DISCONNECT_FILE_MAPPING_DETECTION = 4,
	CLIENT_DISCONNECT_LIBRARY_DETECTION = 5,
	CLIENT_DISCONNECT_REGISTRY_DETECTION = 6,
	CLIENT_DISCONNECT_MEMORY_DETECTION = 7,
	CLIENT_DISCONNECT_WINDOW_DETECTION = 8,
	CLIENT_DISCONNECT_MACRO_DETECTION = 9,
	CLIENT_DISCONNECT_DEBUGGER_DETECTION = 10,
};

template <typename T>
T get_listbox_data(HWND control)
{
	int selected_index = (int)SendMessage(control, LB_GETCURSEL, 0, 0);
	if (selected_index == LB_ERR)
		return NULL;

	T ip_text = (T)SendMessage(control, LB_GETITEMDATA, selected_index, 0);
	if ((int)ip_text == LB_ERR)
		return NULL;

	return ip_text;
}

template <typename T>
T get_listbox_data(HWND control, int index)
{
	T ip_text = (T)SendMessage(control, LB_GETITEMDATA, index, 0);
	if ((int)ip_text == LB_ERR)
		return nullptr;

	return ip_text;
}

int get_count_hwid_by_ip(char* ip_addr);
void copy_to_clipboard(std::string text);
bool CheckListBox(HWND hDlg, int Item, char* text);
int get_index_of_symbol(char* arr, char symbol);
bool get_listbox_text(HWND control, char* row_text);
bool isNumber(const string& str);
vector<string> split(const string& str, char delim);
bool validateIP(string ip);
BOOL WINAPI AddListViewItems(HWND hWndLV, int colNum, int textMaxLen, char** item);

void MsgBox(char* message,...);
void ErrorMessageBox(char* message,...);
void LogAdd(eLogColor color,char* text,...);
void TimeoutProc();
int GetUserCount();
char* GetIpByIndex(int aIndex);
char* GetHwidByIndex(int aIndex);
int GetIndexByIpAndHwid(char* IP, char* HWID);
bool GetSelectedListBoxItem(HWND hDlg, int Item, char* text);
int GetFreeClientIndex();
int SearchFreeClientIndex(int* index,int MinIndex,int MaxIndex,DWORD MinTime);
void PacketDecryptData(BYTE* lpMsg,int size,BYTE key);
void PacketEncryptData(BYTE* lpMsg,int size,BYTE key);
void GetHardwareId(char* hid);
char* RemoveCharacter(char* Input, char Character);
void AuthenticateError();
DWORD GetCurrentVersion();
DWORD ConvertHackVersion(char* version);

extern int gClientCount;
