#include "StdAfx.h"
#include "LuaSocket.h"
#include "Protocol.h"
#include "Util.h"
#include "LuaStack.hpp"
#include "LuaGameServer.h"

CSocket gSocket;

CSocket::CSocket() {
	m_Sockets.clear();
}

CSocket::~CSocket() {
	m_Sockets.clear();
}

void CSocket::RecvPacketCustomSocket(int aIndex, PMSG_CUSTOM_SOCKET_RECV* lpMsg, BYTE* packet, BYTE head, int size)
{
	std::map<std::string, PMSG_CUSTOM_SOCKET_RECV>::iterator it = m_Sockets.find(std::string(lpMsg->m_PacketName));

	if (it == gSocket.m_Sockets.end()) {
		PMSG_CUSTOM_SOCKET_RECV info;

		std::memset(info.m_Packet, 0, sizeof(info.m_Packet));

		std::memcpy(info.m_PacketName, lpMsg->m_PacketName, sizeof(info.m_PacketName));

		std::memcpy(info.m_Packet, lpMsg->m_Packet, sizeof(info.m_Packet));

		info.m_Size = size;

		info.m_SizeGeting = 0;

		gSocket.m_Sockets.insert(std::pair<std::string, PMSG_CUSTOM_SOCKET_RECV>(std::string(lpMsg->m_PacketName), info));
	}
	else {
		std::memset(it->second.m_Packet, 0, sizeof(it->second.m_Packet));

		std::memcpy(it->second.m_PacketName, lpMsg->m_PacketName, sizeof(it->second.m_PacketName));

		std::memcpy(it->second.m_Packet, lpMsg->m_Packet, sizeof(it->second.m_Packet));

		it->second.m_Size = size;

		it->second.m_SizeGeting = 0;
	}

	gLuaGameServer.GameServerProtocol(aIndex, head, lpMsg->m_PacketName);
}

void CreatePacket(std::string packetName, int packet) {
	static PMSG_CUSTOM_SOCKET_RECV pMsg;

	pMsg.header.set(0xFA, (BYTE)packet, 0);

	pMsg.m_Size = 0;

	pMsg.m_SizeGeting = 0;

	std::memcpy(pMsg.m_PacketName, packetName.c_str(), sizeof(pMsg.m_PacketName));

	std::map<std::string, PMSG_CUSTOM_SOCKET_RECV>::iterator it = gSocket.m_Sockets.find(packetName);

	if (it == gSocket.m_Sockets.end()) {
		gSocket.m_Sockets.insert(std::pair<std::string, PMSG_CUSTOM_SOCKET_RECV>(packetName, pMsg));
	}
}

void SetDwordPacket(std::string packetName, DWORD number) {
	std::map<std::string, PMSG_CUSTOM_SOCKET_RECV>::iterator it = gSocket.m_Sockets.find(packetName);

	if (it != gSocket.m_Sockets.end()) {
		if ((it->second.m_Size + 4) >= sizeof(it->second.m_Packet)) {
			return;
		}

		it->second.m_Packet[it->second.m_Size++] = SET_NUMBERHB(SET_NUMBERHW(number));
		it->second.m_Packet[it->second.m_Size++] = SET_NUMBERLB(SET_NUMBERHW(number));
		it->second.m_Packet[it->second.m_Size++] = SET_NUMBERHB(SET_NUMBERLW(number));
		it->second.m_Packet[it->second.m_Size++] = SET_NUMBERLB(SET_NUMBERLW(number));
	}
}

DWORD GetDwordPacket(std::string packetName, int m_PosNumb) {
	std::map<std::string, PMSG_CUSTOM_SOCKET_RECV>::iterator it = gSocket.m_Sockets.find(packetName);

	if (it != gSocket.m_Sockets.end()) {
		DWORD PosNumber = m_PosNumb;

		if (PosNumber == -1) {
			DWORD m_Pos1 = it->second.m_SizeGeting++;
			DWORD m_Pos2 = it->second.m_SizeGeting++;
			DWORD m_Pos3 = it->second.m_SizeGeting++;
			DWORD m_Pos4 = it->second.m_SizeGeting++;

			DWORD GetDwordSizing = (it->second.m_Packet[m_Pos1] << 24) + (it->second.m_Packet[m_Pos2] << 16) + (it->second.m_Packet[m_Pos3] << 8) + (it->second.m_Packet[m_Pos4]);

			return GetDwordSizing;
		}
		else {
			DWORD GetDword = (it->second.m_Packet[PosNumber] << 24) + (it->second.m_Packet[PosNumber + 1] << 16) + (it->second.m_Packet[PosNumber + 2] << 8) + (it->second.m_Packet[PosNumber + 3]);

			return GetDword;
		}
	}

	return 0;
}

void SetWordPacket(std::string packetName, WORD Number) {
	std::map<std::string, PMSG_CUSTOM_SOCKET_RECV>::iterator it = gSocket.m_Sockets.find(std::string(packetName));

	if (it != gSocket.m_Sockets.end()) {
		WORD m_Number = Number;

		if ((it->second.m_Size + 2) >= sizeof(it->second.m_Packet)) {
			return;
		}

		it->second.m_Packet[it->second.m_Size++] = SET_NUMBERHB(m_Number);
		it->second.m_Packet[it->second.m_Size++] = SET_NUMBERLB(m_Number);
	}
}

WORD GetWordPacket(std::string packetName, int m_PosNumb) {
	std::map<std::string, PMSG_CUSTOM_SOCKET_RECV>::iterator it = gSocket.m_Sockets.find(packetName);

	if (it != gSocket.m_Sockets.end()) {
		DWORD PosNumber = m_PosNumb;

		if (PosNumber == -1) {
			DWORD m_Pos1 = it->second.m_SizeGeting++;
			DWORD m_Pos2 = it->second.m_SizeGeting++;

			WORD GetWordSizing = ((int)(it->second.m_Packet[m_Pos1]) << 8) + it->second.m_Packet[m_Pos2];

			return GetWordSizing;
		}
		else {
			WORD GetWord = ((int)(it->second.m_Packet[PosNumber]) << 8) + it->second.m_Packet[PosNumber + 1];

			return GetWord;
		}
	}

	return 0;
}

void SetBytePacket(std::string packetName, short Number) {
	std::map<std::string, PMSG_CUSTOM_SOCKET_RECV>::iterator it = gSocket.m_Sockets.find(packetName);

	if (it != gSocket.m_Sockets.end()) {
		if ((it->second.m_Size + 1) >= sizeof(it->second.m_Packet)) {
			return;
		}

		it->second.m_Packet[it->second.m_Size++] = (unsigned char)Number;
	}
}

short GetBytePacket(std::string namePacket, int m_PosN) {
	std::map<std::string, PMSG_CUSTOM_SOCKET_RECV>::iterator it = gSocket.m_Sockets.find(namePacket);

	if (it != gSocket.m_Sockets.end()) {
		DWORD PosNumber = m_PosN;

		if (PosNumber == -1) {
			DWORD m_Pos1 = it->second.m_SizeGeting++;
			BYTE GetByteSizing = it->second.m_Packet[m_Pos1];
			return GetByteSizing;
		}
		else {
			BYTE GetByte = it->second.m_Packet[PosNumber];
			return GetByte;
		}
	}

	return 0;
}

void SetCharPacket(std::string packetName, std::string string) {
	std::map<std::string, PMSG_CUSTOM_SOCKET_RECV>::iterator it = gSocket.m_Sockets.find(packetName);

	if (it != gSocket.m_Sockets.end()) {
		if ((it->second.m_Size + 10) >= sizeof(it->second.m_Packet)) {
			return;
		}

		std::memcpy(&it->second.m_Packet[it->second.m_Size], string.c_str(), 10);

		it->second.m_Size += 10;
	}
}

void SetCharPacketLength(std::string packetName, std::string string, int length) {
	std::map<std::string, PMSG_CUSTOM_SOCKET_RECV>::iterator it = gSocket.m_Sockets.find(packetName);

	if (it != gSocket.m_Sockets.end()) {
		if ((it->second.m_Size + length) >= sizeof(it->second.m_Packet)) {
			return;
		}

		std::memset(&it->second.m_Packet[it->second.m_Size], 0, length);
		std::memcpy(&it->second.m_Packet[it->second.m_Size], string.c_str(), string.size());

		it->second.m_Size += length;
	}
}

std::string GetCharPacket(std::string packetName, int m_PosNumb) {
	std::map<std::string, PMSG_CUSTOM_SOCKET_RECV>::iterator it = gSocket.m_Sockets.find(packetName);

	if (it != gSocket.m_Sockets.end()) {
		if (m_PosNumb == -1) {
			DWORD m_Pos1 = it->second.m_SizeGeting;

			std::string string;
			string.clear();

			for (int i = 0; i < 10; i++) {
				string[i] = it->second.m_Packet[it->second.m_SizeGeting + i];
			}

			it->second.m_SizeGeting += 10;
			return string.c_str();
		}
	}

	return "NotFind";
}

std::string GetCharPacketLength(std::string packetName, int m_PosNumb, int length) {
	std::map<std::string, PMSG_CUSTOM_SOCKET_RECV>::iterator it = gSocket.m_Sockets.find(packetName);

	if (it != gSocket.m_Sockets.end()) {
		if (m_PosNumb == -1) {
			static char buffer[512];
			std::memset(buffer, 0, sizeof(buffer));

			for (int i = 0; i < length; i++) {
				if ((it->second.m_SizeGeting + i) >= sizeof(it->second.m_Packet)) {
					continue;
				}

				buffer[i] = it->second.m_Packet[it->second.m_SizeGeting + i];
			}

			it->second.m_SizeGeting += length;

			return &buffer[0];
		}
	}

	return "Notfind";
}

void SendPacket(std::string packetName, int aIndex) {
	std::map<std::string, PMSG_CUSTOM_SOCKET_RECV>::iterator it = gSocket.m_Sockets.find(packetName);

	if (it != gSocket.m_Sockets.end()) {
		static PMSG_CUSTOM_SOCKET_SEND pMsg;

		std::memset(&pMsg, 0, sizeof(pMsg));

		int size = sizeof(PSWMSG_HEAD) + sizeof(pMsg.m_PacketName) + it->second.m_Size;

		pMsg.header.set(0x8F, it->second.header.subh, size);

		std::memcpy(pMsg.m_PacketName, it->second.m_PacketName, sizeof(pMsg.m_PacketName));

		std::memcpy(pMsg.m_Packet, it->second.m_Packet, sizeof(pMsg.m_Packet));

		DataSend(aIndex, (BYTE*)&pMsg, size);
	}
}

void ClearPacket(std::string packetName) {
	std::map<std::string, PMSG_CUSTOM_SOCKET_RECV>::iterator it = gSocket.m_Sockets.find(packetName);

	if (it != gSocket.m_Sockets.end()) {
		gSocket.m_Sockets.erase(it);
	}
}

using namespace luaaa;

void CSocket::SetGlobalFunctions(lua_State * Lua) {
	LuaModule(Lua).fun(("CreatePacket"), CreatePacket);

	LuaModule(Lua).fun(("SetDwordPacket"), SetDwordPacket);

	LuaModule(Lua).fun(("GetDwordPacket"), GetDwordPacket);

	LuaModule(Lua).fun(("SetWordPacket"), SetWordPacket);

	LuaModule(Lua).fun(("GetWordPacket"), GetWordPacket);

	LuaModule(Lua).fun(("SetBytePacket"), SetBytePacket);

	LuaModule(Lua).fun(("GetBytePacket"), GetBytePacket);

	LuaModule(Lua).fun(("SetCharPacket"), SetCharPacket);

	LuaModule(Lua).fun(("SetCharPacketLength"), SetCharPacketLength);

	LuaModule(Lua).fun(("GetCharPacket"), GetCharPacket);

	LuaModule(Lua).fun(("GetCharPacketLength"), GetCharPacketLength);

	LuaModule(Lua).fun(("SendPacket"), SendPacket);

	LuaModule(Lua).fun(("ClearPacket"), ClearPacket);
}

void CSocket::ClearPackets()
{
	this->m_Sockets.clear();
}
