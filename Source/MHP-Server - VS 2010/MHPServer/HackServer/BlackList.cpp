#include "stdafx.h"
#include "BlackList.h"
#include "MemScript.h"
#include "Util.h"

CBlackList gBlackList;

CBlackList::CBlackList()
{
}

CBlackList::~CBlackList()
{
}

void CBlackList::SetList(char* filename)
{
	CMemScript* lpScript = new CMemScript;

	if(lpScript == nullptr)
	{
		return;
	}
	
	if(lpScript->SetBuffer(filename,0) == 0)
	{
		delete lpScript;
		return;
	}
	
	gIpAddressBlackList.clear();
	gHardwareIdBlackList.clear();

	try
	{
		while(true)
		{
			if(lpScript->GetToken() == 2)
			{
				break;
			}

			int section = lpScript->GetNumber();

			while(true)
			{
				if(section == 0)
				{
					if(strcmp("end",lpScript->GetAsString()) == 0)
					{
						break;
					}

					BLACKLIST_IPADDR_INFO IpInfo;

					strcpy_s(IpInfo.IpAddress,lpScript->GetString());

					gIpAddressBlackList.push_back(IpInfo);

				}
				else if(section == 1)
				{
					if(strcmp("end",lpScript->GetAsString()) == 0)
					{
						break;
					}

					BLACKLIST_HARDID_INFO HidInfo;

					strcpy_s(HidInfo.HardwareId,lpScript->GetString());

					gHardwareIdBlackList.push_back(HidInfo);
				}
				else
				{
					break;
				}
			}
		}
	}
	catch(...)
	{
		ErrorMessageBox("%s Error De Lectura",filename);
	}

	delete lpScript;
}

bool CBlackList::CheckIpAddress(char* IpAddress)
{
	if(gIpAddressBlackList.size() <= 0)
	{
		return true;
	}

	for(DWORD i = 0; i < gIpAddressBlackList.size(); i++)
	{
		if(strcmp(gIpAddressBlackList[i].IpAddress, IpAddress) == 0)
		{
			return false;
		}
	}

	return true;
}

bool CBlackList::CheckHardwareId(char* HardwareId)
{
	if(gHardwareIdBlackList.size() <= 0)
	{
		return true;
	}

	for(DWORD i = 0; i < gHardwareIdBlackList.size(); i++)
	{
		if(strcmp(gHardwareIdBlackList[i].HardwareId, HardwareId) == 0)
		{
			return false;
		}
	}

	return true;
}

void CBlackList::WriteNewBlackList(char* path)
{
	char new_name[128];
	sprintf(new_name, "%s.bak", path);
	MoveFile(path, new_name);

	ofstream out;
	out.open(path);

	if (out.is_open())
	{
		out << "0" << endl;

		for (int i = 0; i < this->gIpAddressBlackList.size(); i++)
		{
			out << "\"" << this->gIpAddressBlackList[i].IpAddress << "\"" << endl;
		}

		out << "end" << endl << endl;

		out << "1" << endl;

		for (int i = 0; i < this->gHardwareIdBlackList.size(); i++)
		{
			out << "\"" << this->gHardwareIdBlackList[i].HardwareId << "\"" << endl;
		}

		out << "end" << endl;

	}
}