#pragma once

#include "Lua.h"
#include <map>
#include <string>
#include "ProtocolSend.h"
#include "Packets.h"

struct PMSG_CUSTOM_SOCKET_SEND
{
	NEW_PSWMSG_HEAD header; // 0xFE
	char m_PacketName[100];
	BYTE m_Packet[5000];
	int m_Size;
	int m_SizeGeting;
};

class CSocket {
public:
	CSocket();
	virtual ~CSocket();

	void SetGlobalFunctions(lua_State * Lua);
	void RecvCustomPacket(BYTE head, PMSG_CUSTOM_SOCKET_SEND * lpMsg, int size);

	static BOOL CreatePacket(lua_State * Lua);
	static BOOL SetDwordPacket(lua_State * Lua);
	static BOOL GetDwordPacket(lua_State * Lua);
	static BOOL SetWordPacket(lua_State * Lua);
	static BOOL GetWordPacket(lua_State * Lua);
	static BOOL SetBytePacket(lua_State * Lua);
	static BOOL GetBytePacket(lua_State * Lua);
	static BOOL SetCharPacket(lua_State * Lua);
	static BOOL GetCharPacket(lua_State * Lua);
	static BOOL SetCharPacketLength(lua_State * Lua);
	static BOOL GetCharPacketLength(lua_State * Lua);
	static BOOL SendPacket(lua_State * Lua);
	static BOOL GetClearPacket(lua_State * Lua);
	void DataSend(BYTE* lpMsg, int size);

public:
	std::map<std::string, PMSG_CUSTOM_SOCKET_SEND> m_Sockets;
};

extern CSocket gSocket;