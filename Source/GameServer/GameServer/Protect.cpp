// Protect.cpp: implementation of the CProtect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Protect.h"
#include "ServerInfo.h"
#include "ThemidaSDK.h"

CProtect gProtect;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProtect::CProtect() // OK
{
#if(PROTECT_STATE==1)

	CLEAR_START

		VM_START

		srand((DWORD)time(0));

	this->m_socket = INVALID_SOCKET;

	memset(this->m_ComputerHardwareId, 0, sizeof(this->m_ComputerHardwareId));

	this->m_EncKey = rand() % 256;

	memset(&this->m_AuthInfo, 0, sizeof(this->m_AuthInfo));

	VM_END

		CLEAR_END

#endif
}

CProtect::~CProtect()
{

}

bool CProtect::GetComputerHardwareId()
{
#if(PROTECT_STATE==1)

	CLEAR_START

		VM_START

		DWORD VolumeSerialNumber;

	if (GetVolumeInformation(this->GetEncryptedString(gProtectString1, sizeof(gProtectString1)), 0, 0, &VolumeSerialNumber, 0, 0, 0, 0) == 0)
	{
		return 0;
	}

	UUID uuid;

	UuidCreateSequential(&uuid);

	SYSTEM_INFO SystemInfo;

	GetSystemInfo(&SystemInfo);

	DWORD ComputerHardwareId1 = VolumeSerialNumber ^ 0x12B586FE;

	DWORD ComputerHardwareId2 = *(DWORD*)(&uuid.Data4[2]) ^ 0x5D78A569;

	DWORD ComputerHardwareId3 = ((*(WORD*)(&uuid.Data4[6]) & 0xFFFF) | (SystemInfo.wProcessorArchitecture << 16)) ^ 0xF45BC123;

	DWORD ComputerHardwareId4 = ((SystemInfo.wProcessorLevel & 0xFFFF) | (SystemInfo.wProcessorRevision << 16)) ^ 0xB542D8E1;

	wsprintf(this->m_ComputerHardwareId, this->GetEncryptedString(gProtectString2, sizeof(gProtectString2)), ComputerHardwareId1, ComputerHardwareId2, ComputerHardwareId3, ComputerHardwareId4);

	VM_END

		CLEAR_END

		return 1;

#else

	return 0;

#endif
}

bool CProtect::ConnectToAuthServer(char* IpAddress, WORD port) // OK
{
#if(PROTECT_STATE==1)

	CLEAR_START

		VM_START

		if (this->m_socket != INVALID_SOCKET)
		{
			return 0;
		}

	this->m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (this->m_socket == INVALID_SOCKET)
	{
		return 0;
	}

	SOCKADDR_IN target;

	target.sin_family = AF_INET;
	target.sin_port = htons(port);
	target.sin_addr.s_addr = inet_addr(IpAddress);

	if (target.sin_addr.s_addr == INADDR_NONE)
	{
		HOSTENT* host = gethostbyname(IpAddress);

		if (host != 0)
		{
			memcpy(&target.sin_addr.s_addr, *host->h_addr_list, host->h_length);
		}
	}

	if (connect(this->m_socket, (SOCKADDR*)&target, sizeof(target)) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return 0;
		}
	}

	VM_END

		CLEAR_END

		return 1;

#else

	return 0;

#endif
}

bool CProtect::AuthServerDataSend(eAuthServerType ServerType)
{
#if(PROTECT_STATE==1)

	CLEAR_START

		VM_START

		this->m_EncKey = rand() % 256;

	SDHP_AUTH_SERVER_DATA_SEND pMsg;

	pMsg.type = 0xC1;

	pMsg.size = sizeof(pMsg);

	pMsg.head = 0x00;

	pMsg.EncKey = this->m_EncKey;

	pMsg.ServerType = ServerType;

	memcpy(pMsg.CustomerName, gServerInfo.m_CustomerName, sizeof(pMsg.CustomerName));

	memcpy(pMsg.CustomerHardwareId, gServerInfo.m_CustomerHardwareId, sizeof(pMsg.CustomerHardwareId));

	for (int n = 4; n < sizeof(SDHP_AUTH_SERVER_DATA_SEND); n++)
	{
		((BYTE*)&pMsg)[n] ^= pMsg.EncKey + 0x1A;
		((BYTE*)&pMsg)[n] -= 0x69;
	}

	if (send(this->m_socket, (char*)&pMsg, pMsg.size, 0) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return 0;
		}
	}

	VM_END

		CLEAR_END

		return 1;

#else

	return 0;

#endif
}

void CProtect::StartAuth(eAuthServerType ServerType)
{
#if(PROTECT_STATE==1)
	CLEAR_START
		VM_START

		static DWORD CheckProtectionValue;
	CHECK_PROTECTION(CheckProtectionValue, 0x10000000)
		if (CheckProtectionValue != 0x10000000)
		{
			memset((void*)&gProtectTable[0], 0, sizeof(gProtectTable));
			memset((void*)&gProtectInsertStart[0], 0, sizeof(gProtectInsertStart));
			memset((void*)&gProtectInsertFinal[0], 0, sizeof(gProtectInsertFinal));
			memset((void*)&gProtectRemoveStart[0], 0, sizeof(gProtectRemoveStart));
			memset((void*)&gProtectRemoveFinal[0], 0, sizeof(gProtectRemoveFinal));
		}

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		this->ErrorMessageBox(0);
		return;
	}

	if (this->ConnectToAuthServer("192.168.0.1", 4673) == 0)
	{
		closesocket(this->m_socket);
		this->m_socket = INVALID_SOCKET;
		this->ErrorMessageBox(1);
		return;
	}

	if (this->AuthServerDataSend(ServerType) == 0)
	{
		closesocket(this->m_socket);
		this->m_socket = INVALID_SOCKET;
		this->ErrorMessageBox(2);
		return;
	}

	if (recv(this->m_socket, (char*)&this->m_AuthInfo, sizeof(this->m_AuthInfo), 0) != sizeof(SDHP_AUTH_SERVER_DATA_RECV))
	{
		closesocket(this->m_socket);
		this->m_socket = INVALID_SOCKET;
		this->ErrorMessageBox(3);
		return;
	}

	if (this->m_AuthInfo.EncKey != this->m_EncKey)
	{
		closesocket(this->m_socket);
		this->m_socket = INVALID_SOCKET;
		this->ErrorMessageBox(4);
		return;
	}

	for (int n = 4; n < sizeof(SDHP_AUTH_SERVER_DATA_RECV); n++)
	{
		((BYTE*)&this->m_AuthInfo)[n] += 0xDA;
		((BYTE*)&this->m_AuthInfo)[n] ^= this->m_AuthInfo.EncKey + 0x25;
	}

	if (this->m_AuthInfo.Ativo != AUTH_SERVER_STATUS_SUCCESS)
	{
		closesocket(this->m_socket);
		this->m_socket = INVALID_SOCKET;
		this->ErrorMessageBox(5);
		return;
	}

	if (strcmp(this->m_AuthInfo.CustomerName, gServerInfo.m_CustomerName) != 0)
	{
		closesocket(this->m_socket);
		this->m_socket = INVALID_SOCKET;
		this->ErrorMessageBox(6);
		return;
	}

	if (this->GetComputerHardwareId() == 0)
	{
		closesocket(this->m_socket);
		this->m_socket = INVALID_SOCKET;
		this->ErrorMessageBox(7);
		return;
	}

	if (strcmp(this->m_AuthInfo.CustomerHardwareId, this->m_ComputerHardwareId) != 0)
	{
		closesocket(this->m_socket);
		this->m_socket = INVALID_SOCKET;
		this->ErrorMessageBox(8);
		return;
	}

	if (this->m_AuthInfo.Update < UPDATE_GAMING)
	{
		closesocket(this->m_socket);
		this->m_socket = INVALID_SOCKET;
		this->ErrorMessageBox(9);
		return;
	}

	this->m_EncKeyNew = this->m_AuthInfo.Mensal;

	if (this->m_EncKeyNew == 2)
	{
		memcpy(this->m_AccountExpireDate, this->m_AuthInfo.AccountExpireDate, sizeof(this->m_AccountExpireDate));
	}
	else
	{
		memcpy(this->m_AccountExpireDate, "Licença Vitalícia", sizeof(this->m_AccountExpireDate));
	}

	char mensagem_sucesso[512] = { 0 };

	if (this->m_EncKeyNew == 2)
	{
		wsprintf(mensagem_sucesso,
			"Acesso autorizado com sucesso!\n\n"
			"Você pode utilizar esta source até:\n"
			"%s",
			this->m_AccountExpireDate);
	}
	else
	{
		wsprintf(mensagem_sucesso,
			"Acesso autorizado com sucesso!\n\n"
			"Licença Vitalícia - sem data de expiração");
	}

	MessageBox(0, mensagem_sucesso, "Autenticação Aprovada", MB_ICONINFORMATION | MB_OK);

	this->ReleaseBlock(0);

	closesocket(this->m_socket);
	this->m_socket = INVALID_SOCKET;

	memset(this->m_ComputerHardwareId, 0, sizeof(this->m_ComputerHardwareId));
	memset(this->m_AuthInfo.CustomerName, 0, sizeof(this->m_AuthInfo.CustomerName));
	memset(this->m_AuthInfo.CustomerHardwareId, 0, sizeof(this->m_AuthInfo.CustomerHardwareId));

	VM_END
		CLEAR_END
#endif
}

void CProtect::StartAuthProc(eAuthServerType ServerType)
{
#if(PROTECT_STATE==1)
	CLEAR_START
		VM_START

		static DWORD CheckProtectionValue;
	CHECK_PROTECTION(CheckProtectionValue, 0x10000000);
	if (CheckProtectionValue != 0x10000000)
	{
		memset((void*)&gProtectTable[0], 0, sizeof(gProtectTable));
		memset((void*)&gProtectInsertStart[0], 0, sizeof(gProtectInsertStart));
		memset((void*)&gProtectInsertFinal[0], 0, sizeof(gProtectInsertFinal));
		memset((void*)&gProtectRemoveStart[0], 0, sizeof(gProtectRemoveStart));
		memset((void*)&gProtectRemoveFinal[0], 0, sizeof(gProtectRemoveFinal));
	}

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		this->ErrorMessageBox(0);
		return;
	}

	if (this->ConnectToAuthServer("192.168.0.1", 4673) == 0)
	{
		closesocket(this->m_socket);
		this->m_socket = INVALID_SOCKET;
		this->ErrorMessageBox(1);
		return;
	}

	if (this->AuthServerDataSend(ServerType) == 0)
	{
		closesocket(this->m_socket);
		this->m_socket = INVALID_SOCKET;
		this->ErrorMessageBox(2);
		return;
	}

	if (recv(this->m_socket, (char*)&this->m_AuthInfo, sizeof(this->m_AuthInfo), 0) != sizeof(SDHP_AUTH_SERVER_DATA_RECV))
	{
		closesocket(this->m_socket);
		this->m_socket = INVALID_SOCKET;
		this->ErrorMessageBox(3);
		return;
	}

	if (this->m_AuthInfo.EncKey != this->m_EncKey)
	{
		closesocket(this->m_socket);
		this->m_socket = INVALID_SOCKET;
		this->ErrorMessageBox(4);
		return;
	}

	for (int n = 4; n < sizeof(SDHP_AUTH_SERVER_DATA_RECV); n++)
	{
		((BYTE*)&this->m_AuthInfo)[n] += 0xDA;
		((BYTE*)&this->m_AuthInfo)[n] ^= this->m_AuthInfo.EncKey + 0x25;
	}

	if (this->m_AuthInfo.Ativo != AUTH_SERVER_STATUS_SUCCESS)
	{
		closesocket(this->m_socket);
		this->m_socket = INVALID_SOCKET;
		this->ErrorMessageBox(5);
		return;
	}

	// === ORDEM CORRIGIDA ===
	if (this->GetComputerHardwareId() == 0)
	{
		closesocket(this->m_socket);
		this->m_socket = INVALID_SOCKET;
		this->ErrorMessageBox(7);
		return;
	}

	if (strcmp(this->m_AuthInfo.CustomerHardwareId, this->m_ComputerHardwareId) != 0)
	{
		closesocket(this->m_socket);
		this->m_socket = INVALID_SOCKET;
		this->ErrorMessageBox(8);
		return;
	}

	if (strcmp(this->m_AuthInfo.CustomerName, gServerInfo.m_CustomerName) != 0)
	{
		closesocket(this->m_socket);
		this->m_socket = INVALID_SOCKET;
		this->ErrorMessageBox(6);
		return;
	}

	if (this->m_AuthInfo.Update != 2)
	{
		if (this->m_AuthInfo.Mensal < UPDATE_GAMING)
		{
			closesocket(this->m_socket);
			this->m_socket = INVALID_SOCKET;
			this->ErrorMessageBox(10);
			return;
		}
	}

	this->m_EncKeyNew = this->m_AuthInfo.Mensal;
	if (this->m_EncKeyNew == 2)
	{
		memcpy(this->m_AccountExpireDate, this->m_AuthInfo.AccountExpireDate, sizeof(this->m_AccountExpireDate));
	}

	this->ReleaseBlock(0);
	closesocket(this->m_socket);
	this->m_socket = INVALID_SOCKET;
	memset(this->m_ComputerHardwareId, 0, sizeof(this->m_ComputerHardwareId));
	memset(this->m_AuthInfo.CustomerName, 0, sizeof(this->m_AuthInfo.CustomerName));
	memset(this->m_AuthInfo.CustomerHardwareId, 0, sizeof(this->m_AuthInfo.CustomerHardwareId));

	VM_END
		CLEAR_END
#endif
}

inline void CProtect::ErrorMessageBox(int Erro)
{
#if(PROTECT_STATE==1)
	char buff[512];
	memset(this->m_ComputerHardwareId, 0, sizeof(this->m_ComputerHardwareId));
	memset(&this->m_AuthInfo, 0, sizeof(this->m_AuthInfo));

	const char* titulo = "Erro de Autenticação";
	const char* mensagem = NULL;

	switch (Erro)
	{
	case 0:
		mensagem = "Falha ao inicializar.\n"
			"Verifique sua conexão com a internet.";
		titulo = "Erro de Inicialização";
		break;

	case 1:
		mensagem = "Não foi possível conectar ao servidor de autenticação.\n"
			"Possíveis causas:\n"
			"• IP ou porta incorretos\n"
			"• Servidor offline\n"
			"• Firewall ou antivírus bloqueando";
		titulo = "Falha de Conexão";
		break;

	case 2:
		mensagem = "Falha ao enviar os dados de autenticação.\n"
			"A conexão foi feita, mas o envio falhou.";
		titulo = "Erro ao Enviar Dados";
		break;

	case 3:
		mensagem = "Falha ao receber resposta do servidor.\n"
			"Conexão interrompida ou resposta corrompida.";
		titulo = "Erro ao Receber Resposta";
		break;

	case 4:
		mensagem = "Chave de criptografia inválida na resposta.\n"
			"Possível incompatibilidade ou ataque.";
		titulo = "Erro de Criptografia";
		break;

	case 5:
		mensagem = "Autenticação rejeitada.\n"
			"Status retornado: Customername ou HardwareID incorretos.";
		titulo = "Erro de: Customername ou HardwareID";
		break;

	case 6:
		mensagem = "Customername não corresponde ao registrado.\n"
			"Verifique o CustomerName no GameServerInfo - Common.dat.";
		titulo = "Customername Inválido";
		break;

	case 7:
		mensagem = "Não foi possível obter o Hardware ID do computador.\n"
			"Problema ao acessar informações do disco.";
		titulo = "Erro ao Gerar Hardware ID";
		break;

	case 8:
		mensagem = "Hardware ID não autorizado.\n"
			"Este computador não está registrado na licença.";
		titulo = "Hardware Não Autorizado";
		break;

	case 9:
		mensagem = "Pacote/Plano não compatível com esta versão.\n"
			"Seu plano não permite este pacote/atualização.";
		titulo = "Plano Insuficiente";
		break;

	case 10:
		mensagem = "Plano insuficiente para esta versão do GameServer.\n"
			"Necessário plano premium ou superior.";
		titulo = "Plano Não Permitido";
		break;

	default:
		wsprintf(buff, "Erro desconhecido (%d).\nEntre em contato com o suporte.", Erro);
		mensagem = buff;
		titulo = "Erro Desconhecido";
		break;
	}

	MessageBox(0, mensagem, titulo, MB_ICONSTOP | MB_OK);
	this->SafeExitProcess();
#endif
}
inline void CProtect::SafeExitProcess()
{
#if(PROTECT_STATE==1)
    while (true)
    {
        TerminateProcess(GetCurrentProcess(), 0);
        CRASH_APPLICATION_MACRO
    }
#endif
}
void CProtect::ProtectBlock(DWORD size)
{
#if(PROTECT_STATE==1)

	CLEAR_START

		VM_START

		static DWORD CheckProtectionValue;

	CHECK_PROTECTION(CheckProtectionValue, 0x10000000)

		if (CheckProtectionValue != 0x10000000)
		{
			memset((void*)&gProtectTable[0], 0, sizeof(gProtectTable));
			memset((void*)&gProtectInsertStart[0], 0, sizeof(gProtectInsertStart));
			memset((void*)&gProtectInsertFinal[0], 0, sizeof(gProtectInsertFinal));
			memset((void*)&gProtectRemoveStart[0], 0, sizeof(gProtectRemoveStart));
			memset((void*)&gProtectRemoveFinal[0], 0, sizeof(gProtectRemoveFinal));
		}

	MODULEINFO ModuleInfo;

	memset(&ModuleInfo, 0, sizeof(ModuleInfo));

	GetModuleInformation(GetCurrentProcess(), GetModuleHandle(0), &ModuleInfo, sizeof(ModuleInfo));

	DWORD start_offset = (DWORD)ModuleInfo.lpBaseOfDll;

	DWORD final_offset = (DWORD)ModuleInfo.lpBaseOfDll + ModuleInfo.SizeOfImage;

	DWORD start_block, final_block;

	while (this->GetInsertStartBlock(start_offset, final_offset, &start_block) != 0 && this->GetInsertFinalBlock(start_block, final_offset, &final_block) != 0)
	{
		this->InsertMemoryMacro((BYTE*)start_block, (BYTE*)&gProtectRemoveStart[0], sizeof(gProtectRemoveStart));

		this->InsertMemoryMacro((BYTE*)final_block, (BYTE*)&gProtectRemoveFinal[0], sizeof(gProtectRemoveFinal));

		this->EncryptBlock((BYTE*)(start_block + sizeof(gProtectInsertStart)), (final_block - (start_block + sizeof(gProtectInsertStart))));

		start_offset = final_block + sizeof(gProtectInsertFinal);
	}

	VM_END

		CLEAR_END

#endif
}

void CProtect::ReleaseBlock(DWORD size)
{
#if(PROTECT_STATE==1)

	CLEAR_START

		VM_START

		static DWORD CheckProtectionValue;

	CHECK_PROTECTION(CheckProtectionValue, 0x10000000)

		if (CheckProtectionValue != 0x10000000)
		{
			memset((void*)&gProtectTable[0], 0, sizeof(gProtectTable));
			memset((void*)&gProtectInsertStart[0], 0, sizeof(gProtectInsertStart));
			memset((void*)&gProtectInsertFinal[0], 0, sizeof(gProtectInsertFinal));
			memset((void*)&gProtectRemoveStart[0], 0, sizeof(gProtectRemoveStart));
			memset((void*)&gProtectRemoveFinal[0], 0, sizeof(gProtectRemoveFinal));
		}

	MODULEINFO ModuleInfo;

	memset(&ModuleInfo, 0, sizeof(ModuleInfo));

	GetModuleInformation(GetCurrentProcess(), GetModuleHandle(0), &ModuleInfo, sizeof(ModuleInfo));

	DWORD start_offset = (DWORD)ModuleInfo.lpBaseOfDll;

	DWORD final_offset = (DWORD)ModuleInfo.lpBaseOfDll + ModuleInfo.SizeOfImage;

	DWORD start_block, final_block;

	while (this->GetRemoveStartBlock(start_offset, final_offset, &start_block) != 0 && this->GetRemoveFinalBlock(start_block, final_offset, &final_block) != 0)
	{
		this->RemoveMemoryMacro((BYTE*)start_block, (BYTE*)&gProtectInsertStart[0], sizeof(gProtectInsertStart));

		this->RemoveMemoryMacro((BYTE*)final_block, (BYTE*)&gProtectInsertFinal[0], sizeof(gProtectInsertFinal));

		this->DecryptBlock((BYTE*)(start_block + sizeof(gProtectRemoveStart)), (final_block - (start_block + sizeof(gProtectRemoveStart))));

		start_offset = final_block + sizeof(gProtectRemoveFinal);
	}

	VM_END

		CLEAR_END

#endif
}

void CProtect::DecryptBlock(BYTE* data, int size)
{
#if(PROTECT_STATE==1)

	VM_START

		DWORD OldProtect;

	VirtualProtect((void*)data, size, PAGE_EXECUTE_READWRITE, &OldProtect);

	for (int n = 0; n < size; n++)
	{
		data[n] = (data[n] ^ gProtectTable[(n % sizeof(gProtectTable))]) - (0xAF ^ (this->m_ComputerHardwareId[(n % sizeof(this->m_ComputerHardwareId))] ^ this->m_AuthInfo.CustomerHardwareId[(n % sizeof(this->m_AuthInfo.CustomerHardwareId))]));
	}

	VirtualProtect((void*)data, size, OldProtect, &OldProtect);

	VM_END

#endif
}

void CProtect::EncryptBlock(BYTE* data, int size)
{
#if(PROTECT_STATE==1)

	VM_START

		DWORD OldProtect;

	VirtualProtect((void*)data, size, PAGE_EXECUTE_READWRITE, &OldProtect);

	for (int n = 0; n < size; n++)
	{
		data[n] = (data[n] + (0xAF ^ (this->m_ComputerHardwareId[(n % sizeof(this->m_ComputerHardwareId))] ^ this->m_AuthInfo.CustomerHardwareId[(n % sizeof(this->m_AuthInfo.CustomerHardwareId))]))) ^ gProtectTable[(n % sizeof(gProtectTable))];
	}

	VirtualProtect((void*)data, size, OldProtect, &OldProtect);

	VM_END

#endif
}

inline void CProtect::InsertMemoryMacro(BYTE* data, BYTE* info, int size)
{
#if(PROTECT_STATE==1)

	DWORD OldProtect;

	VirtualProtect((void*)data, size, PAGE_EXECUTE_READWRITE, &OldProtect);

	for (int n = 0; n < size; n++)
	{
		data[n] = info[n] ^ 0x9F;
	}

	VirtualProtect((void*)data, size, OldProtect, &OldProtect);

#endif
}

inline void CProtect::RemoveMemoryMacro(BYTE* data, BYTE* info, int size)
{
#if(PROTECT_STATE==1)

	DWORD OldProtect;

	VirtualProtect((void*)data, size, PAGE_EXECUTE_READWRITE, &OldProtect);

	for (int n = 0; n < size; n++)
	{
		data[n] = info[n] ^ 0x9F;
	}

	VirtualProtect((void*)data, size, OldProtect, &OldProtect);

#endif
}

inline bool CProtect::VerifyMemoryMacro(BYTE* data, BYTE* info, int size)
{
#if(PROTECT_STATE==1)

	for (int n = 0; n < size; n++)
	{
		if (data[n] != (info[n] ^ 0x9F))
		{
			return 0;
		}
	}

	return 1;

#else

	return 0;

#endif
}

bool CProtect::GetInsertStartBlock(DWORD start_offset, DWORD final_offset, DWORD* start_block)
{
#if(PROTECT_STATE==1)

	ENCODE_START

		bool result = 0;

	MEMORY_BASIC_INFORMATION mbi;

	memset(&mbi, 0, sizeof(MEMORY_BASIC_INFORMATION));

	for (DWORD offset = start_offset; offset < final_offset; offset++)
	{
		if (offset <= (((DWORD)mbi.BaseAddress) + mbi.RegionSize) || VirtualQuery((void*)offset, &mbi, sizeof(mbi)) != 0)
		{
			if ((final_offset - offset) >= sizeof(gProtectInsertStart) && ((((DWORD)mbi.BaseAddress) + mbi.RegionSize) - offset) >= sizeof(gProtectInsertStart))
			{
				if ((mbi.Protect & PAGE_READONLY) != 0 || (mbi.Protect & PAGE_READWRITE) != 0 || (mbi.Protect & PAGE_EXECUTE_READ) != 0 || (mbi.Protect & PAGE_EXECUTE_READWRITE) != 0)
				{
					if (this->VerifyMemoryMacro((BYTE*)offset, (BYTE*)&gProtectInsertStart[0], sizeof(gProtectInsertStart)) != 0)
					{
						(*start_block) = offset;
						result = 1;
						break;
					}
				}
			}
		}
	}

	ENCODE_END

		return result;

#else

	return 0;

#endif
}

bool CProtect::GetInsertFinalBlock(DWORD start_offset, DWORD final_offset, DWORD* final_block)
{
#if(PROTECT_STATE==1)

	ENCODE_START

		bool result = 0;

	MEMORY_BASIC_INFORMATION mbi;

	memset(&mbi, 0, sizeof(MEMORY_BASIC_INFORMATION));

	for (DWORD offset = start_offset; offset < final_offset; offset++)
	{
		if (offset <= (((DWORD)mbi.BaseAddress) + mbi.RegionSize) || VirtualQuery((void*)offset, &mbi, sizeof(mbi)) != 0)
		{
			if ((final_offset - offset) >= sizeof(gProtectInsertFinal) && ((((DWORD)mbi.BaseAddress) + mbi.RegionSize) - offset) >= sizeof(gProtectInsertFinal))
			{
				if ((mbi.Protect & PAGE_READONLY) != 0 || (mbi.Protect & PAGE_READWRITE) != 0 || (mbi.Protect & PAGE_EXECUTE_READ) != 0 || (mbi.Protect & PAGE_EXECUTE_READWRITE) != 0)
				{
					if (this->VerifyMemoryMacro((BYTE*)offset, (BYTE*)&gProtectInsertFinal[0], sizeof(gProtectInsertFinal)) != 0)
					{
						(*final_block) = offset;
						result = 1;
						break;
					}
				}
			}
		}
	}

	ENCODE_END

		return result;

#else

	return 0;

#endif
}

bool CProtect::GetRemoveStartBlock(DWORD start_offset, DWORD final_offset, DWORD* start_block)
{
#if(PROTECT_STATE==1)

	ENCODE_START

		bool result = 0;

	MEMORY_BASIC_INFORMATION mbi;

	memset(&mbi, 0, sizeof(MEMORY_BASIC_INFORMATION));

	for (DWORD offset = start_offset; offset < final_offset; offset++)
	{
		if (offset <= (((DWORD)mbi.BaseAddress) + mbi.RegionSize) || VirtualQuery((void*)offset, &mbi, sizeof(mbi)) != 0)
		{
			if ((final_offset - offset) >= sizeof(gProtectRemoveStart) && ((((DWORD)mbi.BaseAddress) + mbi.RegionSize) - offset) >= sizeof(gProtectRemoveStart))
			{
				if ((mbi.Protect & PAGE_READONLY) != 0 || (mbi.Protect & PAGE_READWRITE) != 0 || (mbi.Protect & PAGE_EXECUTE_READ) != 0 || (mbi.Protect & PAGE_EXECUTE_READWRITE) != 0)
				{
					if (this->VerifyMemoryMacro((BYTE*)offset, (BYTE*)&gProtectRemoveStart[0], sizeof(gProtectRemoveStart)) != 0)
					{
						(*start_block) = offset;
						result = 1;
						break;
					}
				}
			}
		}
	}

	ENCODE_END

		return result;

#else

	return 0;

#endif
}

bool CProtect::GetRemoveFinalBlock(DWORD start_offset, DWORD final_offset, DWORD* final_block)
{
#if(PROTECT_STATE==1)

	ENCODE_START

		bool result = 0;

	MEMORY_BASIC_INFORMATION mbi;

	memset(&mbi, 0, sizeof(MEMORY_BASIC_INFORMATION));

	for (DWORD offset = start_offset; offset < final_offset; offset++)
	{
		if (offset <= (((DWORD)mbi.BaseAddress) + mbi.RegionSize) || VirtualQuery((void*)offset, &mbi, sizeof(mbi)) != 0)
		{
			if ((final_offset - offset) >= sizeof(gProtectRemoveFinal) && ((((DWORD)mbi.BaseAddress) + mbi.RegionSize) - offset) >= sizeof(gProtectRemoveFinal))
			{
				if ((mbi.Protect & PAGE_READONLY) != 0 || (mbi.Protect & PAGE_READWRITE) != 0 || (mbi.Protect & PAGE_EXECUTE_READ) != 0 || (mbi.Protect & PAGE_EXECUTE_READWRITE) != 0)
				{
					if (this->VerifyMemoryMacro((BYTE*)offset, (BYTE*)&gProtectRemoveFinal[0], sizeof(gProtectRemoveFinal)) != 0)
					{
						(*final_block) = offset;
						result = 1;
						break;
					}
				}
			}
		}
	}

	ENCODE_END

		return result;

#else

	return 0;

#endif
}

char* CProtect::GetEncryptedString(BYTE* string, int size)
{
#if(PROTECT_STATE==1)

	VM_START

		static char buff[256];

	memset(buff, 0, sizeof(buff));

	for (int n = 0; n < size; n++)
	{
		buff[n] = string[n] ^ 0xB0;
	}

	VM_END

		return buff;

#else

	return 0;

#endif
}
