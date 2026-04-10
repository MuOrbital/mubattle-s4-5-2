#include "StdAfx.h"
#include "LuaSocket.h"
#include "wsclientinline.h"
#include "NewUISystem.h"
#include "LuaStack.hpp"
#include "Packets.h"
#include "PacketManager.h"
#include "Interfaces.h"

using namespace luaaa;

CSocket gSocket;

CSocket::CSocket() {
	m_Sockets.clear();
}

CSocket::~CSocket() {
	m_Sockets.clear();
}

void CSocket::SetGlobalFunctions(lua_State * lua) {
	lua_register(lua, ("CreatePacket"), this->CreatePacket);
	lua_register(lua, ("SetDwordPacket"), this->SetDwordPacket);
	lua_register(lua, ("GetDwordPacket"), this->GetDwordPacket);
	lua_register(lua, ("SetWordPacket"), this->SetWordPacket);
	lua_register(lua, ("GetWordPacket"), this->GetWordPacket);
	lua_register(lua, ("SetBytePacket"), this->SetBytePacket);
	lua_register(lua, ("GetBytePacket"), this->GetBytePacket);
	lua_register(lua, ("SetCharPacket"), this->SetCharPacket);
	lua_register(lua, ("GetCharPacket"), this->GetCharPacket);
	lua_register(lua, ("SetCharPacketLength"), this->SetCharPacketLength);
	lua_register(lua, ("GetCharPacketLength"), this->GetCharPacketLength);
	lua_register(lua, ("SendPacket"), this->SendPacket);
	lua_register(lua, ("ClearPacket"), this->GetClearPacket);
}

void CSocket::RecvCustomPacket(BYTE head, PMSG_CUSTOM_SOCKET_SEND * lpMsg, int size) {
	
	std::map<std::string, PMSG_CUSTOM_SOCKET_SEND>::iterator it = m_Sockets.find(std::string(lpMsg->m_PacketName));

	if (it == gSocket.m_Sockets.end()) {
		PMSG_CUSTOM_SOCKET_SEND info;

		std::memset(info.m_Packet, 0, sizeof(info.m_Packet));

		std::memcpy(info.m_PacketName, lpMsg->m_PacketName, sizeof(info.m_PacketName));

		std::memcpy(info.m_Packet, lpMsg->m_Packet, sizeof(info.m_Packet));

		info.m_Size = size;

		info.m_SizeGeting = 0;

		gSocket.m_Sockets.insert(std::pair<std::string, PMSG_CUSTOM_SOCKET_SEND>(std::string(lpMsg->m_PacketName), info));
	}
	else {
		std::memset(it->second.m_Packet, 0, sizeof(it->second.m_Packet));

		std::memcpy(it->second.m_PacketName, lpMsg->m_PacketName, sizeof(it->second.m_PacketName));

		std::memcpy(it->second.m_Packet, lpMsg->m_Packet, sizeof(it->second.m_Packet));

		it->second.m_Size = size;

		it->second.m_SizeGeting = 0;
	}

	gInterface.m_Lua.Generic_Call("ClientProtocol", "is>", head, lpMsg->m_PacketName);
}

BOOL CSocket::CreatePacket(lua_State * lua) {
	PMSG_CUSTOM_SOCKET_SEND pMsg;

	pMsg.header.set(0xFA, (BYTE)lua_tonumber(lua, 2), 0);

	lua_pop(lua, 1);

	pMsg.m_Size = 0;

	pMsg.m_SizeGeting = 0;

	std::memcpy(pMsg.m_PacketName, (char*)lua_tostring(lua, 1), sizeof(pMsg.m_PacketName));

	memset(pMsg.m_Packet, 0x0, sizeof(pMsg.m_Packet));

	std::map<std::string, PMSG_CUSTOM_SOCKET_SEND>::iterator it = gSocket.m_Sockets.find(std::string((char*)lua_tostring(lua, 1)));

	if (it == gSocket.m_Sockets.end()) {
		gSocket.m_Sockets.insert(std::pair<std::string, PMSG_CUSTOM_SOCKET_SEND>(std::string((char*)lua_tostring(lua, 1)), pMsg));
		return true;
	}
	return false;
};

BOOL CSocket::SetDwordPacket(lua_State * lua) {
	std::map<std::string, PMSG_CUSTOM_SOCKET_SEND>::iterator it = gSocket.m_Sockets.find(std::string((char*)lua_tostring(lua, 1)));

	if (it != gSocket.m_Sockets.end()) {
		DWORD m_Number = (DWORD)lua_tointeger(lua, 2);

		lua_pop(lua, 1);

		it->second.m_Packet[it->second.m_Size++] = SET_NUMBERHB(SET_NUMBERHW(m_Number));
		it->second.m_Packet[it->second.m_Size++] = SET_NUMBERLB(SET_NUMBERHW(m_Number));
		it->second.m_Packet[it->second.m_Size++] = SET_NUMBERHB(SET_NUMBERLW(m_Number));
		it->second.m_Packet[it->second.m_Size++] = SET_NUMBERLB(SET_NUMBERLW(m_Number));
		return true;
	}

	return false;
};

BOOL CSocket::GetDwordPacket(lua_State * lua) {
	std::map<std::string, PMSG_CUSTOM_SOCKET_SEND>::iterator it = gSocket.m_Sockets.find(std::string((char*)lua_tostring(lua, 1)));

	if (it != gSocket.m_Sockets.end()) {
		DWORD PosNumber = (DWORD)lua_tointeger(lua, 2);

		lua_pop(lua, 1);

		if (PosNumber == -1) {
			DWORD m_Pos1 = it->second.m_SizeGeting++;
			DWORD m_Pos2 = it->second.m_SizeGeting++;
			DWORD m_Pos3 = it->second.m_SizeGeting++;
			DWORD m_Pos4 = it->second.m_SizeGeting++;

			DWORD GetDwordSizing = (it->second.m_Packet[m_Pos1] << 24) + (it->second.m_Packet[m_Pos2] << 16) + (it->second.m_Packet[m_Pos3] << 8) + (it->second.m_Packet[m_Pos4]);

			lua_pushnumber(lua, (DWORD)GetDwordSizing);
		}
		else {
			DWORD GetDword = (it->second.m_Packet[PosNumber] << 24) + (it->second.m_Packet[PosNumber + 1] << 16) + (it->second.m_Packet[PosNumber + 2] << 8) + (it->second.m_Packet[PosNumber + 3]);

			lua_pushnumber(lua, (DWORD)GetDword);
		}
		return true;
	}
	return false;
};

BOOL CSocket::SetWordPacket(lua_State * lua) {
	std::map<std::string, PMSG_CUSTOM_SOCKET_SEND>::iterator it = gSocket.m_Sockets.find(std::string((char*)lua_tostring(lua, 1)));

	if (it != gSocket.m_Sockets.end()) {
		WORD m_Number = (WORD)lua_tointeger(lua, 2);

		lua_pop(lua, 1);

		it->second.m_Packet[it->second.m_Size++] = SET_NUMBERHB(m_Number);
		it->second.m_Packet[it->second.m_Size++] = SET_NUMBERLB(m_Number);
		return true;
	}

	return false;
};

BOOL CSocket::GetWordPacket(lua_State * lua) {
	std::map<std::string, PMSG_CUSTOM_SOCKET_SEND>::iterator it = gSocket.m_Sockets.find(std::string((char*)lua_tostring(lua, 1)));

	if (it != gSocket.m_Sockets.end()) {
		DWORD PosNumber = (DWORD)lua_tointeger(lua, 2);

		lua_pop(lua, 1);

		if (PosNumber == -1) {
			DWORD m_Pos1 = it->second.m_SizeGeting++;
			DWORD m_Pos2 = it->second.m_SizeGeting++;

			WORD GetWordSizing = ((int)(it->second.m_Packet[m_Pos1]) << 8) + it->second.m_Packet[m_Pos2];

			lua_pushnumber(lua, (WORD)GetWordSizing);
		}
		else {
			WORD GetWord = ((int)(it->second.m_Packet[PosNumber]) << 8) + it->second.m_Packet[PosNumber + 1];

			lua_pushnumber(lua, (WORD)GetWord);
		}
		return true;
	}

	return false;
};

BOOL CSocket::SetBytePacket(lua_State * lua) {
	std::map<std::string, PMSG_CUSTOM_SOCKET_SEND>::iterator it = gSocket.m_Sockets.find(std::string((char*)lua_tostring(lua, 1)));

	if (it != gSocket.m_Sockets.end()) {
		BYTE m_Number = (BYTE)lua_tointeger(lua, 2);

		lua_pop(lua, 1);

		it->second.m_Packet[it->second.m_Size++] = m_Number;
		return true;
	}

	return false;
};

BOOL CSocket::GetBytePacket(lua_State * lua) {
	std::map<std::string, PMSG_CUSTOM_SOCKET_SEND>::iterator it = gSocket.m_Sockets.find(std::string((char*)lua_tostring(lua, 1)));

	if (it != gSocket.m_Sockets.end()) {
		DWORD PosNumber = (DWORD)lua_tointeger(lua, 2);

		lua_pop(lua, 1);

		if (PosNumber == -1) {
			DWORD m_Pos1 = it->second.m_SizeGeting++;

			BYTE GetByteSizing = it->second.m_Packet[m_Pos1];

			lua_pushnumber(lua, (BYTE)GetByteSizing);
		}
		else {
			BYTE GetByte = it->second.m_Packet[PosNumber];

			lua_pushnumber(lua, (BYTE)GetByte);
		}
		return true;
	}
	return false;
};

BOOL CSocket::SetCharPacket(lua_State * lua) {
	std::map<std::string, PMSG_CUSTOM_SOCKET_SEND>::iterator it = gSocket.m_Sockets.find(std::string((char*)lua_tostring(lua, 1)));

	if (it != gSocket.m_Sockets.end()) {
		std::memcpy(&it->second.m_Packet[it->second.m_Size], (char*)lua_tostring(lua, 2), 10);

		it->second.m_Size += 10;
		return true;
	}

	return false;
};

BOOL CSocket::GetCharPacket(lua_State * lua) {
	std::map<std::string, PMSG_CUSTOM_SOCKET_SEND>::iterator it = gSocket.m_Sockets.find(std::string((char*)lua_tostring(lua, 1)));

	if (it != gSocket.m_Sockets.end()) {
		DWORD PosNumber = (DWORD)lua_tointeger(lua, 2);

		if (PosNumber == -1) {
			DWORD m_Pos1 = it->second.m_SizeGeting;

			char string[11];

			for (int i = 0; i < 10; i++) {
				string[i] = it->second.m_Packet[it->second.m_SizeGeting + i];
			}

			it->second.m_SizeGeting += 10;

			lua_pushstring(lua, (char*)string);
		}
		else {
			BYTE GetByte = it->second.m_Packet[PosNumber];

			lua_pushnumber(lua, (BYTE)GetByte);
		}
		return true;
	}

	return false;
}

BOOL CSocket::SetCharPacketLength(lua_State * lua)
{
	std::map<std::string, PMSG_CUSTOM_SOCKET_SEND>::iterator it = gSocket.m_Sockets.find(std::string((char*)lua_tostring(lua, 1)));

	if (it != gSocket.m_Sockets.end()) {
		DWORD length = (DWORD)lua_tointeger(lua, 3);

		std::memset(&it->second.m_Packet[it->second.m_Size], 0, length);
		std::memcpy(&it->second.m_Packet[it->second.m_Size], (char*)lua_tostring(lua, 2), length);

		it->second.m_Size += length;
		return true;
	}

	return false;
}

BOOL CSocket::GetCharPacketLength(lua_State * lua)
{
	std::map<std::string, PMSG_CUSTOM_SOCKET_SEND>::iterator it = gSocket.m_Sockets.find(std::string((char*)lua_tostring(lua, 1)));

	if (it != gSocket.m_Sockets.end()) {
		DWORD PosNumber = (DWORD)lua_tointeger(lua, 2);

		if (PosNumber == -1) {
			DWORD length = (DWORD)lua_tointeger(lua, 3);

			char string[512];

			for (DWORD i = 0; i < length; i++) {
				string[i] = it->second.m_Packet[it->second.m_SizeGeting + i];
			}

			it->second.m_SizeGeting += length;

			lua_pushlstring(lua, string, length);
		}
		else {
			BYTE GetByte = it->second.m_Packet[PosNumber];

			lua_pushnumber(lua, (BYTE)GetByte);
		}

		return true;
	}

	return false;
};

BOOL CSocket::SendPacket(lua_State * lua) {
	std::map<std::string, PMSG_CUSTOM_SOCKET_SEND>::iterator it = gSocket.m_Sockets.find(std::string((char*)lua_tostring(lua, 1)));

	if (it != gSocket.m_Sockets.end()) {
		static PMSG_CUSTOM_SOCKET_SEND pMsg;

		std::memset(&pMsg, 0, sizeof(pMsg));

		int size = sizeof(NEW_PSWMSG_HEAD) + sizeof(pMsg.m_PacketName) + it->second.m_Size;

		pMsg.header.set(0x8F, it->second.header.subh, (size));

		std::memcpy(pMsg.m_PacketName, it->second.m_PacketName, sizeof(pMsg.m_PacketName));

		std::memcpy(pMsg.m_Packet, it->second.m_Packet, sizeof(pMsg.m_Packet));

		gSocket.DataSend((BYTE*)&pMsg, size);
		return true;
	}

	return false;
}

BOOL CSocket::GetClearPacket(lua_State * lua) {
	std::map<std::string, PMSG_CUSTOM_SOCKET_SEND>::iterator it = gSocket.m_Sockets.find(std::string((char*)lua_tostring(lua, 1)));

	if (it != gSocket.m_Sockets.end()) {
		gSocket.m_Sockets.erase((char*)lua_tostring(lua, 1));
		return true;
	}
	return false;
};


void CSocket::DataSend(BYTE* lpMsg, int size)
{
	BYTE EncBuff[2048];

	if (gPacketManager.AddData(lpMsg, size) != 0 && gPacketManager.ExtractPacket(EncBuff) != 0)
	{
		BYTE send[2048];

		memcpy(send, EncBuff, size);

		if (EncBuff[0] == 0xC3 || EncBuff[0] == 0xC4)
		{
			if (EncBuff[0] == 0xC3)
			{
				BYTE save = EncBuff[1];

				EncBuff[1] = g_byPacketSerialSend++;

				size = gPacketManager.Encrypt(&send[2], &EncBuff[1], (size - 1)) + 2;

				EncBuff[1] = save;

				send[0] = 0xC3;
				send[1] = LOBYTE(size);
			}
			else
			{
				BYTE save = EncBuff[2];

				EncBuff[2] = g_byPacketSerialSend++;

				size = gPacketManager.Encrypt(&send[3], &EncBuff[2], (size - 2)) + 3;

				EncBuff[2] = save;

				send[0] = 0xC4;
				send[1] = HIBYTE(size);
				send[2] = LOBYTE(size);
			}
		}

		g_pSocketClient->sSend((char*)send, size);
	}
}