#include "stdafx.h"
#include "ProcessListInfo.h"
#include "Util.h"
#include "Connection.h"
#include "CCRC32.h"

CProcessListInfo gProcessListInfo;

wchar_t *GetWC(const char *c)
{
    const size_t cSize = strlen(c)+1;
    wchar_t* wc = new wchar_t[cSize];
    mbstowcs (wc, c, cSize);

    return wc;
}

void CProcessListInfo::RecvGetProcessDump(PMSG_GET_PROCESS_DUMP_RECV* lpMsg)
{
	int index = -1;
	try
	{
		PROCESS_INFO_DATA* info = &this->m_ProcessListData.at(lpMsg->index);

		if(info->PID == lpMsg->PID && strcmp(info->path,lpMsg->path) == 0)
			index = lpMsg->index;
	}
	catch (std::out_of_range)
	{
		index = -1;
	}

	if(index == -1)
		return;

	if (lpMsg->type == RequestType::request_dump)
		this->GetDump(GetWC(this->m_ProcessListData.at(index).path));
	else if (lpMsg->type == RequestType::request_window)
		this->GetProcessWindowClass(lpMsg->PID);
	else if (lpMsg->type == RequestType::request_file)
		this->SendFile(this->m_ProcessListData.at(index).path);

	
	/*if (lpMsg->type == 1)
		this->GetProcessWindowClass(lpMsg->PID);
	else
		GetDump(GetWC(this->m_ProcessListData.at(index).path));*/
	
	//ErrorMessageBox("dump path: %s\nlist path: %s",lpMsg->path,this->m_ProcessListData[lpMsg->index].path);
}

void CProcessListInfo::GetProcessWindowClass(int pid)
{
	std::vector<HWND> hWnds;

	GetAllWindowsFromProcessID(pid, hWnds);

	PMSG_WINDOW_CLASS_SEND pMsg;

	pMsg.header.set(0x17, sizeof(pMsg));

	pMsg.count = hWnds.size();

	for (int i=0;i<hWnds.size();i++)
	{
		if (i >= 20)
			break;

		char window_name[MAX_PATH];
		char window_class[MAX_PATH];

		GetWindowTextA(hWnds[i], window_name, MAX_PATH);
		GetClassNameA(hWnds[i], window_class, MAX_PATH);

		memcpy(pMsg.window_name[i], window_name, sizeof(pMsg.window_name[i]));
		memcpy(pMsg.window_class[i], window_class, sizeof(pMsg.window_class[i]));
	}

	gConnection.DataSend((BYTE*)&pMsg, sizeof(pMsg));
}


void CProcessListInfo::RecvProcessListReq()
{
	this->m_ProcessListData.clear();
	//std::vector<PROCESS_INFO_DATA> mData;

	//this->GetProcessList();
	if (!this->GetProcessList())
	{
		return;
	}

	BYTE send[8192];

	PMSG_PROCESS_INFO_DATA_SEND pMsg;

	pMsg.header.set(0x12,0);

	pMsg.count = 0;

	pMsg.isFirst = 1;

	int size = sizeof(pMsg);

	PROCESS_INFO_DATA info;
	const int ITEMS_PER_SEND = 25;

	int counter = 0;

	for (int i=0; i < m_ProcessListData.size(); i++)
	{
		info.PID = m_ProcessListData[i].PID;
		memcpy(info.name,m_ProcessListData[i].name,sizeof(info.name));
		memcpy(info.path,m_ProcessListData[i].path,sizeof(info.path));

		memcpy(&send[size],&info,sizeof(info));

		size += sizeof(info);

		pMsg.count++;

		counter++;

		if (counter >= ITEMS_PER_SEND || i == m_ProcessListData.size() - 1)
		{
			pMsg.header.size[0] = SET_NUMBERHB(size);

			pMsg.header.size[1] = SET_NUMBERLB(size);

			memcpy(send,&pMsg,sizeof(pMsg));

			while(true)
			{
				if (gConnection.CheckSendSize() > 0)
				{
					Sleep(1);
				}
				else
				{
					break;
				}
			}


			gConnection.DataSend(send,size);

			counter = 0;

			pMsg.count = 0;

			pMsg.isFirst = 0;

			size = sizeof(pMsg);

			memset(send,0,sizeof(send));
		}
	}

	
}

bool CProcessListInfo::GetProcessList()
{
	DWORD ProcessIds[1024],BytesReturned=0,ProcessCount=0;

	if(EnumProcesses(ProcessIds,sizeof(ProcessIds),&BytesReturned) == 0)
	{
		return false;
	}

	//std::wofstream out("outtext.txt");

	for(int n=0,ProcessCount=(BytesReturned/sizeof(DWORD));n < ProcessCount;n++)
	{
		if(ProcessIds[n] != 0)
		{
			wchar_t szProcessName[MAX_PATH], sTemp[MAX_PATH];

			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ProcessIds[n]);

			if (hProcess != NULL)
			{
				if(GetProcessImageFileNameW(hProcess,szProcessName,MAX_PATH) != 0)
				{
					//out << szProcessName << "\n";
					char ProcessName[64];
					wsprintf(ProcessName,"%S",ConvertModuleFileName(szProcessName));
					if(ConvertProcessImageFileNameW(szProcessName,sTemp,MAX_PATH) != 0)
					{
						PROCESS_INFO_DATA info;

						info.PID = ProcessIds[n];
						memcpy(info.name,ProcessName,sizeof(info.name));

						sprintf(info.path,"%ws",sTemp);

						//fout << "PID: " << ProcessIds[n] << " Name: " << ProcessName << " Path: " << sTemp << std::endl;

						m_ProcessListData.emplace_back(info);
					}
				}
			}
			
			CloseHandle(hProcess);
		}
	}
	//out.close();
	return true;
}

bool CProcessListInfo::FindEntryPointInfo(wchar_t* filename,DWORD* address,BYTE* dump) // OK
{
	HANDLE file = CreateFileW(filename,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);

	if(file == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	BY_HANDLE_FILE_INFORMATION bhfi;

	if(GetFileInformationByHandle(file,&bhfi) == 0)
	{
		CloseHandle(file);
		return 0;
	}

	HANDLE mapping = CreateFileMapping(file,0,PAGE_READONLY,bhfi.nFileSizeHigh,bhfi.nFileSizeLow,0);

	if(mapping == 0)
	{
		CloseHandle(file);
		return 0;
	}

	BYTE* lpBaseAddress = (BYTE*)MapViewOfFile(mapping,FILE_MAP_READ,0,0,bhfi.nFileSizeLow);

	if(lpBaseAddress == 0)
	{
		CloseHandle(mapping);
		CloseHandle(file);
		return 0;
	}

	IMAGE_DOS_HEADER* lpDosHeader = (IMAGE_DOS_HEADER*)lpBaseAddress;

	if(lpDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		UnmapViewOfFile((LPCVOID)lpBaseAddress);
		CloseHandle(mapping);
		CloseHandle(file);
		return 0;
	}

	IMAGE_NT_HEADERS* lpNtHeader = (IMAGE_NT_HEADERS*)(lpBaseAddress+lpDosHeader->e_lfanew);

	if(lpNtHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		UnmapViewOfFile((LPCVOID)lpBaseAddress);
		CloseHandle(mapping);
		CloseHandle(file);
		return 0;
	}

	if(lpNtHeader->FileHeader.Machine != IMAGE_FILE_MACHINE_I386 && lpNtHeader->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64)
	{
		UnmapViewOfFile((LPCVOID)lpBaseAddress);
		CloseHandle(mapping);
		CloseHandle(file);
		return 0;
	}

	IMAGE_SECTION_HEADER* lpSectionHeader = ImageRvaToSection(lpNtHeader,(PVOID)lpNtHeader->OptionalHeader.ImageBase,lpNtHeader->OptionalHeader.AddressOfEntryPoint);

	if(lpSectionHeader == 0)
	{
		UnmapViewOfFile((LPCVOID)lpBaseAddress);
		CloseHandle(mapping);
		CloseHandle(file);
		return 0;
	}

	if(SetFilePointer(file,((lpNtHeader->OptionalHeader.AddressOfEntryPoint-lpSectionHeader->VirtualAddress)+lpSectionHeader->PointerToRawData),0,FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		UnmapViewOfFile((LPCVOID)lpBaseAddress);
		CloseHandle(mapping);
		CloseHandle(file);
		return 0;
	}

	DWORD NumberOfBytesRead = 0;

	if(ReadFile(file,dump,32,&NumberOfBytesRead,0) == 0)
	{
		UnmapViewOfFile((LPCVOID)lpBaseAddress);
		CloseHandle(mapping);
		CloseHandle(file);
		return 0;
	}

	(*address) = lpNtHeader->OptionalHeader.ImageBase+lpNtHeader->OptionalHeader.AddressOfEntryPoint;

	UnmapViewOfFile((LPCVOID)lpBaseAddress);

	CloseHandle(mapping);

	CloseHandle(file);

	return 1;
}

void copy_to_clipboard(char* text)
{
	EmptyClipboard(); //очищаем буфер
	HGLOBAL hgBuffer = GlobalAlloc(GMEM_DDESHARE, 512);//выделяем память
	char* chBuffer = (char*)GlobalLock(hgBuffer); //блокируем память
	
	strcpy(chBuffer, LPCSTR(text));
	GlobalUnlock(hgBuffer);//разблокируем память
	SetClipboardData(CF_TEXT, hgBuffer);//помещаем текст в буфер обмена
	CloseClipboard(); //закрываем буфер обмена
}

void CProcessListInfo::GetDump(wchar_t* filename)
{
	DWORD type = 0;
	DWORD address;
	BYTE dump[32];

	if(FindEntryPointInfo(filename,&address,dump) == 0)
	{
		type = 1;

		CCRC32 CRCChecker;

		address = CRCChecker.FileCRC(filename);

		memset(dump,0,sizeof(dump));
	}

	char temp[256];

	wcscpy_s(filename,256,ConvertFileName(filename));

	filename[31] = ((wcslen(filename)>31)?0:filename[31]);

	wsprintf(temp,"%d	\"0x%.8X\"	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	%.3d	\"%S\"\r\n",
	(DWORD)type, (DWORD)address, (BYTE)dump[0], (BYTE)dump[1], (BYTE)dump[2], (BYTE)dump[3], (BYTE)dump[4], (BYTE)dump[5], (BYTE)dump[6], (BYTE)dump[7], (BYTE)dump[8], (BYTE)dump[9], (BYTE)dump[10],
	(BYTE) dump[11], (BYTE)dump[12], (BYTE)dump[13], (BYTE)dump[14], (BYTE)dump[15], (BYTE)dump[16], (BYTE)dump[17], (BYTE)dump[18], (BYTE)dump[19], (BYTE)dump[20],
	(BYTE) dump[21], (BYTE)dump[22], (BYTE)dump[23], (BYTE)dump[24], (BYTE)dump[25], (BYTE)dump[26], (BYTE)dump[27], (BYTE)dump[28], (BYTE)dump[29], (BYTE)dump[30],
	(BYTE) dump[31], filename);


	PMSG_GET_PROCESS_DUMP_SEND pMsg;

	pMsg.header.set(0x13,sizeof(pMsg));

	memcpy(pMsg.dump,temp,sizeof(pMsg.dump));

	gConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg));
	
	//WriteFile(temp,TYPE_DUMP);

	//if(OpenClipboard(nullptr))//открываем буфер обмена
	//{
	//	
	//	char text[512];

	//	copy_to_clipboard(temp);
	//	MessageBox(nullptr,"Registration data successfully copied to clipboard!","INFO",MB_OK | MB_ICONINFORMATION);
	//}
}

void CProcessListInfo::SendFile(char* path)
{
	//FILE* fSource = NULL;
	//unsigned char* sBuf = NULL;
	//int iBytesRead = 0;

	//if (fopen_s(&fSource, path, "rb") != NULL)
	//{
	//	return; //Failed to open file for read access.
	//}

	std::ifstream myFile(path, std::ios::in | std::ios::binary);

	if (!myFile.is_open())
		return;
	
	myFile.seekg(0, myFile.end);
	int length = myFile.tellg();
	myFile.seekg(0, myFile.beg);

	char* fileData = new char[length];

	myFile.read(fileData, length);

	BYTE send[500020];

	PMSG_FILE_SEND pMsg;

	pMsg.header.set(0x18, 0);

	pMsg.fullSize = length;

	pMsg.isFirst = 1;

	int size = sizeof(pMsg);

	const int MAX_SEND_SIZE = 500000;

	for (int i = 0; i < length; i+= MAX_SEND_SIZE)
	{
		int toRead = MAX_SEND_SIZE;
		
		if(i + MAX_SEND_SIZE > length)
			toRead = length % MAX_SEND_SIZE;

		//ErrorMessageBox("toRead: %d", toRead);
		
		memcpy(&send[size], &fileData[i], toRead);

		size += toRead;

		pMsg.count = toRead;

		//pMsg.header.size[0] = SET_NUMBERHB(size);

		//pMsg.header.size[1] = SET_NUMBERLB(size);

		//pMsg.header.size = size;

		memcpy(pMsg.header.size, &size, sizeof(DWORD));

		memcpy(send, &pMsg, sizeof(pMsg));

		while (true)
		{
			if (gConnection.CheckSendSize() > 0)
			{
				Sleep(1);
			}
			else
			{
				break;
			}
		}


		gConnection.DataSend(send, size);

		pMsg.isFirst = 0;

		size = sizeof(pMsg);

		memset(send, 0, sizeof(send));
	}
}
