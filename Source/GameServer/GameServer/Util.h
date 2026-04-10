#pragma once

#include "ServerDisplayer.h"
#include "User.h"

int SafeGetItem(int index);
flt GetRoundValue(float value);
BYTE GetNewOptionCount(BYTE NewOption);
BYTE GetSocketOptionCount(BYTE SocketOption[5]);
BYTE GetPathPacketDirPos(int px,int py);
void PacketArgumentDecrypt(char* out_buff,char* in_buff,int size);
void ErrorMessageBox(char* message,...);
void LogAdd(eLogColor color,char* text,...);
void LogAddConnect(eLogColor color, char* text, ...);
bool DataSend(int aIndex,BYTE* lpMsg,DWORD size);
void DataSendAll(BYTE* lpMsg,int size);
bool DataSendSocket(SOCKET socket,BYTE* lpMsg,DWORD size);
void MsgSendV2(LPOBJ lpObj,BYTE* lpMsg,int size);
void CloseClient(int aIndex);
void PostMessage1(char* name,char* message,char* text);
void PostMessage2(char* name,char* message,char* text);
void PostMessage3(char* name,char* message,char* text);
void PostMessage4(char* name,char* message,char* text);
void PostMessageUserON(char* name, char* message);
void SetLargeRand();
long GetLargeRand();
bool IsValidItem(int index);
void LogAddLua(eLogColor color, char* text, ...);
void ChatYellowMessage(int aIndex, char* String);
void ChatYellowMessageAll(char* String);
void ChatGreenMessage(int aIndex, char* String);
void ChatGreenMessageAll(char* String);
void ChatBlueMessage(int aIndex, char* String);
void ChatBlueMessageAll(char* String);
void ChatGoldMessage(int aIndex, char* String);
void ChatGoldMessageAll(char* String);
void FormatDamageNumber(int damage, char* outBuffer, size_t bufferSize);

extern short RoadPathTable[MAX_ROAD_PATH_TABLE];
