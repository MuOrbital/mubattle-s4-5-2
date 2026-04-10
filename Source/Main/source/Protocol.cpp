#include "stdafx.h"
#include <rpc.h>
#include "Protocol.h"
#include "PacketManager.h"
#include "Winmain.h"

#pragma comment(lib, "Rpcrt4.lib")

cProtocol gProtocol;

void cProtocol::GetMac()
{
	DWORD VolumeSerialNumber = 0x5045454;

	GetVolumeInformation("C:\\", 0, 0, &VolumeSerialNumber, 0, 0, 0, 0);

	UUID uuid;

	UuidCreateSequential(&uuid);

	SYSTEM_INFO SystemInfo;

	GetSystemInfo(&SystemInfo);

	DWORD ComputerHardwareId1 = VolumeSerialNumber ^ 0x12B586FE;

	DWORD ComputerHardwareId2 = *(DWORD*)(&uuid.Data4[2]) ^ 0x5D78A569;

	DWORD ComputerHardwareId3 = ((*(WORD*)(&uuid.Data4[6]) & 0xFFFF) | (SystemInfo.wProcessorArchitecture << 16)) ^ 0xF45BC123;

	DWORD ComputerHardwareId4 = ((SystemInfo.wProcessorLevel & 0xFFFF) | (SystemInfo.wProcessorRevision << 16)) ^ 0xB542D8E1;

	sprintf_s(this->mac_addr, "%08X-%08X-%08X-%08X", ComputerHardwareId1, ComputerHardwareId2, ComputerHardwareId3, ComputerHardwareId4);
}

void cProtocol::ClientSendMac()
{
	this->GetMac();

	PMSG_ANTI_CLIENT_KEY_SEND pMsg;

	pMsg.header.set(0xF3, 0xFA, sizeof(pMsg));

	strcpy_s(pMsg.m_MacAddress, this->mac_addr);

	strcpy_s(pMsg.m_Language, g_aszMLSelection);

	DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void cProtocol::DataSend(BYTE* lpMsg, int size)
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