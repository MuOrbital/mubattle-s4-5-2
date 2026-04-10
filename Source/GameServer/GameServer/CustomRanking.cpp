#include "stdafx.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "Util.h"
#include "Message.h"
#include "User.h"
#include "Path.h"
#include "ServerInfo.h"
#include "MemScript.h"
#include "CustomRanking.h"
#include "ObjectManager.h"
#include "Notice.h"

CCustomRanking gCustomRanking;

void CCustomRanking::Load(char* path)
{

	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);
		return;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	for (int n = 0; n < MAX_RANK; n++)
	{
		this->r_Data[n];
	}

	this->m_count = 0;

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while (true)
			{
				if (section == 0)
				{
					if (strcmp("end", lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					int index = lpMemScript->GetNumber();

					strcpy_s(this->r_Data[index].Name, lpMemScript->GetAsString());

					strcpy_s(this->r_Data[index].Col1, lpMemScript->GetAsString());

					strcpy_s(this->r_Data[index].Col2, lpMemScript->GetAsString());

					this->m_count++;
				}
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CCustomRanking::GCReqRankingCount(int Index, PMSG_CUSTOM_RANKING_COUNT_RECV* lpMsg)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 6)
	//if (gServerInfo.m_CustomRankingSwitch == 0)
	//{
	//	return;
	//}

	PMSG_CUSTOM_RANKING_COUNT_SEND pMsg;

	pMsg.header.set(0xF3, 0xE7, sizeof(pMsg));

	pMsg.count = (this->m_count > MAX_RANK) ? MAX_RANK : this->m_count;

	DataSend(Index, (LPBYTE)&pMsg, pMsg.header.size);
#endif
}

void CCustomRanking::GCReqRanking(int Index, PMSG_CUSTOM_RANKING_RECV* lpMsg)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 6)
	if (gObjIsConnected(Index) != false)
	{
		//if (gServerInfo.m_CustomRankingSwitch == 0)
		//{
		//	return;
		//}

		if (this->m_count <= 0)
		{
			return;
		}

		if (lpMsg->type < 0 || lpMsg->type >= this->m_count)
		{
			return;
		}

		if (this->m_count == 0)
		{
			return;
		}

		SDHP_CUSTOM_RANKING_SEND pMsg;

		pMsg.header.set(0xF4, sizeof(pMsg));

		pMsg.index = Index;

		pMsg.type = lpMsg->type;

		gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
	}
#endif
}

void CCustomRanking::GDCustomRankingRecv(BYTE* ReceiveBuffer)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 6)
	SDHP_CUSTOM_RANKING_RECV* Data = (SDHP_CUSTOM_RANKING_RECV*)ReceiveBuffer;

	BYTE send[4096];

	PMSG_CUSTOM_RANKING_SEND pMsg;

	pMsg.header.set(0xF3, 0xE6, 0);

	pMsg.RankIndex = Data->type;

	memcpy(pMsg.rankname, this->r_Data[Data->type].Name, sizeof(pMsg.rankname));

	memcpy(pMsg.col1, this->r_Data[Data->type].Col1, sizeof(pMsg.col1));

	memcpy(pMsg.col2, this->r_Data[Data->type].Col2, sizeof(pMsg.col2));

	int size = sizeof(pMsg);

	int offset = sizeof(SDHP_CUSTOM_RANKING_RECV);

	pMsg.count = 0;

	for (int n = 0; n < Data->count; n++)
	{
		CUSTOM_RANKING_DATA* Data2 = (CUSTOM_RANKING_DATA*)(ReceiveBuffer + offset);

		CUSTOM_RANKING_DATA info;

		memcpy(info.szName, Data2->szName, sizeof(info.szName));

		info.Score = Data2->Score;

		info.Vip = Data2->Vip;

		info.Class = (BYTE)gDefaultClassInfo.GetCharacterDefaultStat(Data2->Class, 0);

		memcpy(&send[size], &info, sizeof(CUSTOM_RANKING_DATA));

		size += sizeof(CUSTOM_RANKING_DATA);

		offset += (sizeof(CUSTOM_RANKING_DATA));

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(Data->index, send, size);
#endif
}