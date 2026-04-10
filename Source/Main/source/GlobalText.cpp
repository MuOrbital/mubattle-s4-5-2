#include "stdafx.h"
#include "GlobalText.h"
#include "MemScript.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGlobalText GlobalText;

CGlobalText::CGlobalText() // OK
{
	this->m_MessageInfo.clear();

	this->Load("Data\\Local\\Text.txt");
}

CGlobalText::~CGlobalText() // OK
{

}

void CGlobalText::Load(char* path) // OK
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		lpMemScript->ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);
		return;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		lpMemScript->ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->m_MessageInfo.clear();

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if (strcmp("end", lpMemScript->GetString()) == 0)
			{
				break;
			}

			TEXT_INFO info;

			info.Index = lpMemScript->GetNumber();

			strcpy_s(info.Message, lpMemScript->GetAsString());

			this->m_MessageInfo.insert(std::pair<int, TEXT_INFO>(info.Index, info));
		}
	}
	catch (...)
	{
		lpMemScript->ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}


char* CGlobalText::GlobalText(int index) // OK
{
	std::map<int, TEXT_INFO>::iterator it = this->m_MessageInfo.find(index);

	if (it == this->m_MessageInfo.end())
	{
		wsprintf(this->m_DefaultMessage, "Could not find message %d!", index);

		return this->m_DefaultMessage;
	}
	else
	{
		return it->second.Message;
	}
}