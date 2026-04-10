#pragma once

#include "wsclientinline.h"
#include "Packets.h"

struct PMSG_ANTI_CLIENT_KEY_SEND
{
	PSBMSG_HEAD header;
	char m_MacAddress[40];
	char m_Language[4];
};

class cProtocol
{
public:
	void GetMac();
	void ClientSendMac();
	void DataSend(BYTE* lpMsg, int size);
public:
	char mac_addr[40];
};

extern cProtocol gProtocol;