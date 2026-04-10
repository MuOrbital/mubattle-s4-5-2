#include "stdafx.h"
#include "Reconnect.h"
#include "NewUICommon.h"
#include "wsclientinline.h"
#include "PersonalShopTitleImp.h"


using namespace SEASON3B;

void UnRegisterBuff(eBuffState buff, OBJECT* o);


CReconnect::CReconnect()
{
	memset(s_Data.GameServerAddress, 0, 16);
	s_Data.GameServerPort = 0;
	memset(s_Data.ReconnectAccount, 0, 11);
	memset(s_Data.ReconnectPassword, 0, 11);
	memset(s_Data.ReconnectName, 0, 11);
	memset(s_Data.ReconnectMapServerAddress, 0, 16);

	s_Data.ReconnectMapServerPort = 0;
	s_Data.ReconnectStatus = RECONNECT_STATUS_NONE;
	s_Data.ReconnectProgress = RECONNECT_PROGRESS_NONE;
	s_Data.ReconnectCurTime = 0;
	s_Data.ReconnectMaxTime = 0;
	s_Data.ReconnectCurWait = 0;
	s_Data.ReconnectMaxWait = 0;
	s_Data.ReconnectAuthSend = 0;
	s_Data.ReconnectHelperOn = 0;
}

CReconnect::~CReconnect()
{

}

void CReconnect::ReconnectMainProc()
{
	if (s_Data.ReconnectStatus != RECONNECT_STATUS_RECONNECT)
	{
		return;
	}

	ReconnectDrawInterface();

	if ((GetTickCount() - s_Data.ReconnectMaxTime) > s_Data.ReconnectMaxWait)
	{
		ReconnectSetInfo(RECONNECT_STATUS_DISCONNECT, RECONNECT_PROGRESS_NONE, 0, 0);
		SocketClient.Close();
		return;
	}

	if ((GetTickCount() - s_Data.ReconnectCurTime) < s_Data.ReconnectCurWait)
	{
		return;
	}

	switch (s_Data.ReconnectProgress)
	{
	case RECONNECT_PROGRESS_NONE:
		ReconnecGameServerLoad();
		break;
	case RECONNECT_PROGRESS_CONNECTED:
		ReconnecGameServerAuth();
		break;
	}

	s_Data.ReconnectCurTime = GetTickCount();
}

void CReconnect::ReconnectDrawInterface()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	float StartX = (GetWindowsX / 2.f) - (160.f / 2.f);

	float StartY = 100.f;

	float progress = ((s_Data.ReconnectMaxWait == 0) ? 0 : (((GetTickCount() - s_Data.ReconnectMaxTime) * 150.f) / (float)s_Data.ReconnectMaxWait));

	progress = ((progress > 150) ? 150 : progress);

	RenderImage(CNewUIMessageBoxMng::IMAGE_MSGBOX_PROGRESS_BG, StartX, StartY, 160.f, 18.f);

	RenderImage(CNewUIMessageBoxMng::IMAGE_MSGBOX_PROGRESS_BAR, (StartX + 5.f), (StartY + 5.f), progress, 8.f);


	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetBgColor(0);

	StartX += 80.f;
	StartY += 5.f;

	switch (s_Data.ReconnectProgress)
	{
	case RECONNECT_PROGRESS_NONE:
		g_pRenderText->RenderText((int)StartX, (int)StartY, "Reconnecting...", 0, 0, RT3_WRITE_CENTER);
		break;
	case RECONNECT_PROGRESS_CONNECTED:
		g_pRenderText->RenderText((int)StartX, (int)StartY, "Logging in...", 0, 0, RT3_WRITE_CENTER);
		break;
	case RECONNECT_PROGRESS_JOINED:
		g_pRenderText->RenderText((int)StartX, (int)StartY, "Waiting character list...", 0, 0, RT3_WRITE_CENTER);
		break;
	case RECONNECT_PROGRESS_CHAR_LIST:
		g_pRenderText->RenderText((int)StartX, (int)StartY, "Waiting character info...", 0, 0, RT3_WRITE_CENTER);
		break;
	}
}

void CReconnect::ReconnectSetInfo(DWORD status, DWORD progress, DWORD CurWait, DWORD MaxWait)
{
	s_Data.ReconnectStatus = status;

	s_Data.ReconnectProgress = progress;

	s_Data.ReconnectCurTime = GetTickCount();

	s_Data.ReconnectMaxTime = GetTickCount();

	s_Data.ReconnectCurWait = CurWait;

	s_Data.ReconnectMaxWait = MaxWait;

	s_Data.ReconnectAuthSend = ((status == RECONNECT_STATUS_NONE) ? 0 : s_Data.ReconnectAuthSend);

	s_Data.ReconnectHelperOn = ((status == RECONNECT_STATUS_NONE) ? 0 : s_Data.ReconnectHelperOn);
}

void CReconnect::ReconnecGameServerLoad()
{
	if (CreateSocket(s_Data.GameServerAddress, s_Data.GameServerPort) != 0)
	{
		g_bGameServerConnected = TRUE;

		ReconnectSetInfo(RECONNECT_STATUS_RECONNECT, RECONNECT_PROGRESS_CONNECTED, 10000, 30000);
	}
}

void CReconnect::ReconnecGameServerAuth()
{
	if (((s_Data.ReconnectAuthSend == 0) ? (s_Data.ReconnectAuthSend++) : s_Data.ReconnectAuthSend) != 0)
	{
		return;
	}

	SendRequestLogIn(s_Data.ReconnectAccount, s_Data.ReconnectPassword);
}

void CReconnect::ReconnectOnCloseSocket()
{
	if (s_Data.ReconnectStatus != RECONNECT_STATUS_DISCONNECT)
	{
		ReconnectSetInfo(RECONNECT_STATUS_RECONNECT, RECONNECT_PROGRESS_NONE, 30000, 300000);  //ReconnectTime

		s_Data.ReconnectAuthSend = 0;

		ReconnectViewportDestroy();

		PartyNumber = 0;

		memcpy(s_Data.ReconnectName, CharacterAttribute->Name, sizeof(s_Data.ReconnectName));
	}
}

void CReconnect::ReconnectOnMapServerMove(char* address, WORD port)
{
	if (s_Data.ReconnectStatus != RECONNECT_STATUS_RECONNECT || s_Data.ReconnectProgress == RECONNECT_PROGRESS_CHAR_LIST)
	{
		strcpy_s(s_Data.ReconnectMapServerAddress, address);

		s_Data.ReconnectMapServerPort = port;

		ReconnectSetInfo(RECONNECT_STATUS_DISCONNECT, ((s_Data.ReconnectProgress == RECONNECT_PROGRESS_CHAR_LIST) ? s_Data.ReconnectProgress : RECONNECT_PROGRESS_NONE), 0, 0);
	}
}

void CReconnect::ReconnectOnMapServerMoveAuth(BYTE result)
{
	if (s_Data.ReconnectStatus != RECONNECT_STATUS_RECONNECT)
	{
		if (result == 1)
		{
			ReconnectSetInfo(((s_Data.ReconnectProgress == RECONNECT_PROGRESS_CHAR_LIST) ? s_Data.ReconnectStatus : RECONNECT_STATUS_NONE), ((s_Data.ReconnectProgress == RECONNECT_PROGRESS_CHAR_LIST) ? s_Data.ReconnectProgress : RECONNECT_PROGRESS_NONE), 0, 0);
		}
		else
		{
			ReconnectSetInfo(RECONNECT_STATUS_DISCONNECT, RECONNECT_PROGRESS_NONE, 0, 0);
		}
	}
}

void CReconnect::ReconnectOnConnectAccount(BYTE result) // OK
{
	if (s_Data.ReconnectProgress == RECONNECT_PROGRESS_CONNECTED)
	{
		if (s_Data.ReconnectAuthSend != 0)
		{
			if (result == 1)
			{
				SendRequestCharactersList(g_pMultiLanguage->GetLanguage());

				ReconnectSetInfo(RECONNECT_STATUS_RECONNECT, RECONNECT_PROGRESS_JOINED, 30000, 30000);
			}
			else
			{
				if (result == 3)
				{
					ReconnectSetInfo(RECONNECT_STATUS_RECONNECT, RECONNECT_PROGRESS_CONNECTED, 10000, 30000);
					s_Data.ReconnectAuthSend = 0;
				}
				else
				{
					ReconnectSetInfo(RECONNECT_STATUS_DISCONNECT, RECONNECT_PROGRESS_NONE, 0, 0);
					SocketClient.Close();
				}
			}
		}
	}
}

void CReconnect::ReconnectOnCloseClient(BYTE result)
{
	if (s_Data.ReconnectStatus != RECONNECT_STATUS_RECONNECT)
	{
		if (result == 0 || result == 2)
		{
			ReconnectSetInfo(RECONNECT_STATUS_DISCONNECT, RECONNECT_PROGRESS_NONE, 0, 0);
		}
	}
}

void CReconnect::ReconnectOnCharacterList()
{
	if (s_Data.ReconnectProgress == RECONNECT_PROGRESS_JOINED)
	{

		SendRequestJoinMapServer(s_Data.ReconnectName);

		ReconnectSetInfo(RECONNECT_STATUS_RECONNECT, RECONNECT_PROGRESS_CHAR_LIST, 30000, 30000);
	}
}

void CReconnect::ReconnectOnCharacterInfo()
{
	ReconnectSetInfo(RECONNECT_STATUS_NONE, RECONNECT_PROGRESS_NONE, 0, 0);
}

void CReconnect::ReconnectViewportDestroy()
{
	for (int i = 0; i < MAX_CHARACTERS_CLIENT; i++)
	{
		CHARACTER* c = &CharactersClient[i];

		if (c == nullptr)
		{
			continue;
		}

		if (c->Object.Live == false)
		{
			continue;
		}


		int iIndex = g_pPurchaseShopInventory->GetShopCharacterIndex();
		if (iIndex >= 0 && iIndex < MAX_CHARACTERS_CLIENT)
		{
			CHARACTER* pCha = &CharactersClient[iIndex];
			if (pCha && pCha->Key == c->Key)
			{
				g_pNewUISystem->Hide(SEASON3B::INTERFACE_PURCHASESHOP_INVENTORY);
			}
		}


		int buffSize = g_CharacterBuffSize((&c->Object));

		for (int k = 0; k < buffSize; k++)
		{
			UnRegisterBuff(g_CharacterBuff((&c->Object), k), &c->Object);
		}

		DeleteCharacter(c->Key);
		CHARACTER* pPlayer = FindCharacterTagShopTitle(c->Key);
		if (pPlayer)
		{
			RemoveShopTitle(pPlayer);
		}
	}
}

void CReconnect::ReconnectCreateConnection(char* address, WORD port)
{
	if (PORT_RANGE(port) != 0 && s_Data.GameServerAddress != address)
	{
		if (strcmp(s_Data.ReconnectMapServerAddress, address) != 0 || s_Data.ReconnectMapServerPort != port)
		{
			wsprintf(s_Data.GameServerAddress, "%s", address);

			s_Data.GameServerPort = port;

			memset(s_Data.ReconnectMapServerAddress, 0, sizeof(s_Data.ReconnectMapServerAddress));

			s_Data.ReconnectMapServerPort = 0;
		}
	}
}

bool CReconnect::CheckSocketPort(SOCKET s)
{
	SOCKADDR_IN addr;
	int addr_len = sizeof(addr);

	int socket = getpeername(s, (SOCKADDR*)&addr, &addr_len);

	g_ConsoleDebug->Write(MCD_ERROR, "SOCKET:%d", socket);


	if (getpeername(s, (SOCKADDR*)&addr, &addr_len) == SOCKET_ERROR)
	{
		return false;
	}

	if (PORT_RANGE(ntohs(addr.sin_port)) == 0)
	{
		return false;
	}

	return true;
}

CReconnect* CReconnect::GetSingleton()
{
	static CReconnect gReconnect;
	return &gReconnect;
}