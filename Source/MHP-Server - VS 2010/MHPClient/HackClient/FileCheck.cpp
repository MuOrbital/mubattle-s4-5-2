#include "StdAfx.h"
#include "FileCheck.h"
#include "HackServerProtocol.h"
#include "ListManager.h"
#include "CCRC32.h"
#include "Util.h"
#include "Log.h"

CFileCheck gFileCheck;

CFileCheck::CFileCheck()
{
	this->m_count = 0;
}

CFileCheck::~CFileCheck()
{
}

void CFileCheck::Scan()
{
	//gLog.Output(LOG_DEBUG, "[SCAN] Start scan (count: %d)",this->m_count);	
	if((this->m_count++) < 12)
	{
		CCRC32 CRC32;

		for(std::vector<CSUM_LIST_INFO>::iterator it=gListManager.gChecksumListInfo.begin();it != gListManager.gChecksumListInfo.end();it++)
		{
			//gLog.Output(LOG_DEBUG, "[SCAN] File: %s",it->path);
			if(FileExists(it->path) == 1)
			{
				DWORD FileHash;

				if(CRC32.FileCRC(it->path,&FileHash,1024) == 0)
				{
					CHClientDisconnectSend(CLIENT_DISCONNECT_FILE_DETECTION,it->path,0);
					return;
				}
				//gLog.Output(LOG_DEBUG, "[SCAN] Path = [%s] CRC = [%x|%x]",it->path,it->checksum,FileHash);
				if(it->checksum != FileHash)
				{
					CHClientDisconnectSend(CLIENT_DISCONNECT_FILE_DETECTION,it->path,0);
					return;
				}
			}
			else
			{
				CHClientDisconnectSend(CLIENT_DISCONNECT_FILE_DETECTION,it->path,0);
				return;
			}
		}
	}
}
