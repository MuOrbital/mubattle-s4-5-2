#include "stdafx.h"
#include "ProcessListInfo.h"
#include "SocketManager.h"
#include "Util.h"
#include "resource.h"
#include "ClientManager.h"

CProcessListInfo gProcessListInfo;

CProcessListInfo::CProcessListInfo()
{
	CreateDirectory("Downloads",0);
	this->currentIndex = -1;
	this->totalProcess = 0;
}

void CProcessListInfo::SendProcessRequest(int aIndex)
{
	this->ClearTotalProcess();

	PMSG_PROCESS_LIST_REQ_SEND pMsg;

	pMsg.header.set(0x11,sizeof(pMsg));

	gSocketManager.DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CProcessListInfo::RecvProcessList(PMSG_PROCESS_INFO_DATA_RECV* lpMsg, int aIndex)
{
	if (lpMsg->isFirst == 1)
	{
		ListView_DeleteAllItems(this->hWndLV);
		this->m_ProcessListInfo.clear();

		SetDlgItemText(this->hDlg, IDC_STATIC_NAME, "");
		SetDlgItemText(this->hDlg, IDC_STATIC_PATH, "");
		SetDlgItemText(this->hDlg, IDC_STATIC_PID, "");
	}

	if (aIndex != this->currentIndex)
	{
		this->currentIndex = aIndex;
		this->totalProcess = 0;
	}
	
	for(int n=0;n < lpMsg->count;n++)
	{
		PROCESS_INFO_DATA* lpInfo = (PROCESS_INFO_DATA*)(((BYTE*)lpMsg)+sizeof(PMSG_PROCESS_INFO_DATA_RECV)+(sizeof(PROCESS_INFO_DATA)*n));

		this->m_ProcessListInfo.push_back(*lpInfo);

		int iLastIndex = ListView_GetItemCount(this->hWndLV);

		LVITEM lvi;
		lvi.mask = LVIF_TEXT;
		lvi.cchTextMax = 50;
		lvi.iItem = iLastIndex;
		lvi.pszText = lpInfo->name;
		lvi.iSubItem = 0;

		ListView_InsertItem(this->hWndLV, &lvi);

		char text[10];
		sprintf(text,"%d",lpInfo->PID);

		ListView_SetItemText(this->hWndLV, iLastIndex, 1, text)
		ListView_SetItemText(this->hWndLV, iLastIndex, 2, lpInfo->path)

		this->totalProcess++;

	}

	char Text[100];
	sprintf(Text,"Total process: %d",this->totalProcess);

	SetWindowText(this->hDlg, Text);
	ShowWindow(this->ScreenWnd, SW_SHOWNORMAL);
}

void CProcessListInfo::SendRequest(int index, RequestType type)
{
	if(index <= -1)
		return;

	PROCESS_INFO_DATA* info = &this->m_ProcessListInfo[index];

	this->last_index = index;

	PMSG_GET_PROCESS_DUMP_SEND pMsg;

	pMsg.header.set(0x12,sizeof(pMsg));

	pMsg.type = type;
	
	pMsg.index = index;

	pMsg.PID = info->PID;

	pMsg.header.size[0] = SET_NUMBERHB(sizeof(pMsg));
	
	pMsg.header.size[1] = SET_NUMBERLB(sizeof(pMsg));

	memcpy(pMsg.path,info->path,sizeof(pMsg.path));

	gSocketManager.DataSend(this->currentIndex,(BYTE*)&pMsg,sizeof(pMsg));
}

void CProcessListInfo::RecvProcessDumpRequest(PMSG_GET_PROCESS_DUMP_RECV* lpMsg)
{
	if(OpenClipboard(nullptr))//открываем буфер обмена
	{
		copy_to_clipboard(lpMsg->dump);
		MessageBox(nullptr,"Dump data successfully copied to clipboard!","INFO",MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(nullptr,"Can't open clipboard!","INFO",MB_OK | MB_ICONERROR);		
	}
}

void CProcessListInfo::RecvProcessClass(PMSG_WINDOW_CLASS_SEND* lpMsg)
{
	std::stringstream ss;

	for(int i=0;i<lpMsg->count;i++)
	{
		ss << "Window" << i + 1 << ":\n";
		ss << "WindowName: " << lpMsg->window_name[i] << "\n";
		ss << "WindowClass: " << lpMsg->window_class[i] << "\n\n";
	}

	if (OpenClipboard(nullptr))//открываем буфер обмена
	{
		copy_to_clipboard((char*)ss.str().c_str());
		MessageBox(nullptr, "Dump data successfully copied to clipboard!", "INFO", MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(nullptr, "Can't open clipboard!", "INFO", MB_OK | MB_ICONERROR);
	}
}

void CProcessListInfo::RecvFile(PMSG_FILE_RECV* lpMsg)
{
	std::stringstream ss;
	ss << "Downloads\\" << this->m_ProcessListInfo[this->last_index].name;
	
	if (lpMsg->isFirst == 1)
	{
		this->m_file_total_size = lpMsg->fullSize;
		this->m_file_total_loaded = 0;

		//SendMessage(GetDlgItem(this->hDlg, IDC_PROGRESS1), PBM_SETRANGE, 0, (LPARAM)MAKELONG(0, this->m_file_total_size));
		//Установим шаг
		//SendMessage(GetDlgItem(this->hDlg, IDC_PROGRESS1), PBM_SETSTEP, (WPARAM)1, 0); //шаг 1

		DeleteFile(ss.str().c_str());
	}
	
	ofstream file(ss.str().c_str(), std::ios::binary | std::ios::app);

	file.write((char*)((BYTE*)lpMsg + sizeof(PMSG_FILE_RECV)), lpMsg->count);

	file.close();

	this->m_file_total_loaded += lpMsg->count;

	int progress = (float)this->m_file_total_loaded / (float)this->m_file_total_size * 100.0f;
	
	SendMessage(GetDlgItem(this->hDlg, IDC_PROGRESS1), PBM_SETPOS, progress, 0);

	if (progress == 100)
		MsgBox("File is loaded");
}

void CProcessListInfo::SetInfoByIndex(int index)
{
	auto var = &this->m_ProcessListInfo[index];

	SetDlgItemText(this->hDlg, IDC_STATIC_NAME, var->name);
	SetDlgItemText(this->hDlg, IDC_STATIC_PATH, var->path);
	char pid[20];
	sprintf(pid, "%d", var->PID);
	SetDlgItemText(this->hDlg, IDC_STATIC_PID, pid);
}
