#include "stdafx.h"
#include "HackCheck.h"
#include "Protect.h"

BYTE EncDecKey1;
BYTE EncDecKey2;

void DecryptData(BYTE* lpMsg, int size) // OK
{
	for (int n = 0; n < size; n++)
	{
		lpMsg[n] = (lpMsg[n] ^ EncDecKey1) - EncDecKey2;
	}
}

void EncryptData(BYTE* lpMsg, int size) // OK
{
	for (int n = 0; n < size; n++)
	{
		lpMsg[n] = (lpMsg[n] + EncDecKey2) ^ EncDecKey1;
	}
}

bool CheckSocketPort(SOCKET s) // OK
{
	SOCKADDR_IN addr;
	int addr_len = sizeof(addr);

	if (getpeername(s, (SOCKADDR*)&addr, &addr_len) == SOCKET_ERROR)
	{
		return 0;
	}

	if (PORT_RANGE(ntohs(addr.sin_port)) == 0)
	{
		return 0;
	}

	return 1;
}

int WINAPI MyRecv(SOCKET s, char* buf, int len, int flags) // OK
{
	int result = recv(s, (char*)buf, len, flags);

	if (result == SOCKET_ERROR || result == 0)
	{
		return result;
	}

	if (CheckSocketPort(s) != 0)
	{
		DecryptData((BYTE*)buf, result);
	}

	return result;
}

int WINAPI MySend(SOCKET s, char* buf, int len, int flags) // OK
{
	if (CheckSocketPort(s) != 0)
	{
		EncryptData((BYTE*)buf, len);
	}

	return send(s, buf, len, flags);
}

void InitHackCheck() // OK
{
	WORD EncDecKey = 0;

	for (int n = 0; n < sizeof(gProtect->m_MainInfo.CustomerName); n++)
	{
		EncDecKey += (BYTE)(gProtect->m_MainInfo.CustomerName[n] ^ gProtect->m_MainInfo.ClientSerial[(n % sizeof(gProtect->m_MainInfo.ClientSerial))]);
	}

	EncDecKey1 = (BYTE)0xF6;
	EncDecKey2 = (BYTE)0xA9;

	EncDecKey1 += LOBYTE(EncDecKey);
	EncDecKey2 += HIBYTE(EncDecKey);
}
