#pragma once

#include <map>
#include <string>

#include "Protocol.h"
#include "LuaState.h"

struct PMSG_CUSTOM_SOCKET_RECV
{
	PSWMSG_HEAD header; // 0xFE
	char m_PacketName[100];
	BYTE m_Packet[5000];
	int m_Size;
	int m_SizeGeting;
};

struct PMSG_CUSTOM_SOCKET_SEND
{
	PSWMSG_HEAD header; // 0xFE
	char m_PacketName[100];
	BYTE m_Packet[5000];
	int m_Size;
	int m_SizeGeting;
};

class CSocket {
public:
	CSocket();
	virtual ~CSocket();

	void RecvPacketCustomSocket(int aIndex, PMSG_CUSTOM_SOCKET_RECV * lpMsg, BYTE* packet, BYTE head, int size);
	void SetGlobalFunctions(lua_State * Lua);
	void ClearPackets();

public:
	std::map<std::string, PMSG_CUSTOM_SOCKET_RECV> m_Sockets;


};

extern CSocket gSocket;