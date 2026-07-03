#include "MHPIntegration.h"

struct MAIN_INTEGRATED_FILE_INFO
{
	char CustomerName[32];
	char IpAddress[32];
	unsigned short IpAddressPort;
	unsigned short MHPServerPort;
	int m_UseIntegratedAntiHack;
	char ClientVersion[8];
	char ClientSerial[17];
	char WindowName[32];
};

extern "C" const MAIN_INTEGRATED_FILE_INFO* MHPIntegrationGetMainInfo();

#define MHP_INTEGRATED_CLIENT 1

#define MAIN_FILE_INFO MHP_MAIN_FILE_INFO
#define CCRC32 CMHPCRC32
#define CCriticalSection CMHPCriticalSection
#define CProtect CMHPProtect
#define CListManager CMHPListManager
#define gProtect gMHPProtect
#define gListManager gMHPListManager

#define DecryptData MHPDecryptData
#define EncryptData MHPEncryptData
#define CheckSocketPort MHPCheckSocketPort
#define MyRecv MHPMyRecv
#define MySend MHPMySend
#define InitHackCheck MHPInitHackCheck
#define EncDecKey1 MHPEncDecKey1
#define EncDecKey2 MHPEncDecKey2

#define HACK_VERIFY_FILE MHPClientHackVerifyFile
#define HACK_VERIFY_FILE_MAPPING MHPClientHackVerifyFileMapping
#define FileMappingHandle MHPClientFileMappingHandle
#define lpHackVerifyFileMapping MHPClientHackVerifyFileMappingPtr

#define EntryProc MHPClientEntryProc
#define DllMain MHPClientDllMain
#define hins MHPClientInstance
#define ThreadHandles MHPClientThreadHandles
#define ScreenThread MHPClientScreenThread
#define ConnectionReconnectThread MHPClientConnectionReconnectThread
#define ConnectionStatusThread MHPClientConnectionStatusThread
#define MainThread MHPClientMainThread

bool MHPIntegratedLoadMainInfo();

#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\CCRC32.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\Connection.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\CriticalSection.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\FileCheck.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\FileMappingCheck.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\FileProtect.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\HackServerProtocol.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\LibraryCheck.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\HackCheck.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\HackClient.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\ListManager.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\DumpCheck.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\ExecutableCheck.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\Log.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\MacroCheck.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\Message.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\ProcessCheck.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\ProcessListInfo.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\ProcessManager.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\ProcessQuery.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\Protect.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\ScreenCapture.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\ScreenShot.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\SimpleModules.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\SplashScreen.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\ThreadCheck.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\Util.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\RegistryCheck.cpp"
#include "..\..\MHP-Server - VS 2010\MHPClient\HackClient\WindowCheck.cpp"

#undef EntryProc
#undef DllMain
#undef hins
#undef HACK_VERIFY_FILE
#undef HACK_VERIFY_FILE_MAPPING
#undef FileMappingHandle
#undef lpHackVerifyFileMapping
#undef MIN_HACK_VERIFY_SIZE
#undef MAX_HACK_VERIFY_SIZE

#define EntryProc MHPVerifyEntryProc
#define CloseProc MHPVerifyCloseProc
#define DllMain MHPVerifyDllMain
#define HACK_VERIFY_FILE MHPVerifyHackVerifyFile
#define HACK_VERIFY_FILE_MAPPING MHPVerifyHackVerifyFileMapping
#define FileMappingHandle MHPVerifyFileMappingHandle
#define lpHackVerifyFileMapping MHPVerifyHackVerifyFileMappingPtr
#define HookNtReadVirtualMemory MHPVerifyHookNtReadVirtualMemory
#define HookNtWriteVirtualMemory MHPVerifyHookNtWriteVirtualMemory
#define MyNtReadVirtualMemory MHPVerifyNtReadVirtualMemory
#define MyNtWriteVirtualMemory MHPVerifyNtWriteVirtualMemory

#include "..\..\MHP-Server - VS 2010\MHPVerify\HackVerify\HackVerify.cpp"

#undef EntryProc
#undef CloseProc
#undef DllMain
#undef HACK_VERIFY_FILE
#undef HACK_VERIFY_FILE_MAPPING
#undef FileMappingHandle
#undef lpHackVerifyFileMapping
#undef HookNtReadVirtualMemory
#undef HookNtWriteVirtualMemory
#undef MyNtReadVirtualMemory
#undef MyNtWriteVirtualMemory

#undef ThreadHandles
#undef ScreenThread
#undef ConnectionReconnectThread
#undef ConnectionStatusThread
#undef MainThread

#undef DecryptData
#undef EncryptData
#undef CheckSocketPort
#undef MyRecv
#undef MySend
#undef InitHackCheck
#undef EncDecKey1
#undef EncDecKey2

#undef CCRC32
#undef CCriticalSection
#undef CProtect
#undef CListManager
#undef gProtect
#undef gListManager
#undef MAIN_FILE_INFO

static void MHPIntegratedCopyString(char* destination,size_t destinationSize,const char* source)
{
	strncpy_s(destination,destinationSize,((source == 0) ? "" : source),_TRUNCATE);
}

static void MHPIntegratedSetMessage(int index,const char* text)
{
	MESSAGE_INFO info;

	memset(&info,0,sizeof(info));

	info.Index = index;

	MHPIntegratedCopyString(info.Message,sizeof(info.Message),text);

	gMessage.SetInfoEng(info);
	gMessage.SetInfoPor(info);
	gMessage.SetInfoSpn(info);
}

static void MHPIntegratedLoadDefaultMessages()
{
	static const char* const Messages[] =
	{
		"Proteção finalizada",
		"Falha ao reiniciar",
		"Conexão perdida",
		"Tempo de resposta excedido",
		"Falha ao iniciar",
		"Servidor: %s",
		"Falha ao conectar",
		"Tempo esgotado",
		"Arquivos corrompidos",
		"Versão inválida",
		"Bloqueado temporariamente",
		"Você está bloqueado",
		"Você está bloqueado",
		"Falha ao receber dados",
		"Execução suspeita",
		"Execução suspeita",
		"Arquivos alterados",
		"Execução suspeita",
		"Execução suspeita",
		"Ameaça detectada",
		"Alteração de memória",
		"Tentativa de trapaça",
		"Ameaça detectada",
		"Alteração indevida",
		"Automação de teclado",
		"Debugger detectado"
	};

	gMessage.Init();
	MHPIntegratedCopyString(gMessage.m_Language,sizeof(gMessage.m_Language),"Por");
	gMessage.GetLanguage();

	if(gMessage.m_Language[0] == 0)
	{
		MHPIntegratedCopyString(gMessage.m_Language,sizeof(gMessage.m_Language),"Por");
	}

	for(int n = 0;n < (sizeof(Messages)/sizeof(Messages[0]));n++)
	{
		MHPIntegratedSetMessage(n,Messages[n]);
	}
}

bool MHPIntegratedLoadMainInfo()
{
	const MAIN_INTEGRATED_FILE_INFO* mainInfo = MHPIntegrationGetMainInfo();

	if(mainInfo == 0)
	{
		return 0;
	}

	if(mainInfo->m_UseIntegratedAntiHack == 0)
	{
		return 0;
	}

	memset(&gMHPProtect.m_MainInfo,0,sizeof(gMHPProtect.m_MainInfo));

	MHPIntegratedCopyString(gMHPProtect.m_MainInfo.CustomerName,sizeof(gMHPProtect.m_MainInfo.CustomerName),mainInfo->CustomerName);
	MHPIntegratedCopyString(gMHPProtect.m_MainInfo.IpAddress,sizeof(gMHPProtect.m_MainInfo.IpAddress),mainInfo->IpAddress);
	MHPIntegratedCopyString(gMHPProtect.m_MainInfo.ServerName,sizeof(gMHPProtect.m_MainInfo.ServerName),mainInfo->WindowName);
	MHPIntegratedCopyString(gMHPProtect.m_MainInfo.ClientName,sizeof(gMHPProtect.m_MainInfo.ClientName),"Main.exe");

	gMHPProtect.m_MainInfo.ServerPort = ((mainInfo->MHPServerPort == 0) ? 55999 : mainInfo->MHPServerPort);
	gMHPProtect.m_MainInfo.ClientCRC32 = 0;
	gMHPProtect.m_MainInfo.PluginCRC32 = 0;
	gMHPProtect.m_MainInfo.VerifyCRC32 = 0;
	gMHPProtect.m_ClientFileCRC = 0;
	gMHPProtect.is_x64 = false;
	gMHPProtect.x64_library = 0;

	MHPIntegratedLoadDefaultMessages();

	return 1;
}

void MHPIntegrationStart(HINSTANCE hInstance)
{
	static bool initialized = false;

	const MAIN_INTEGRATED_FILE_INFO* mainInfo = MHPIntegrationGetMainInfo();

	if(mainInfo == 0 || mainInfo->m_UseIntegratedAntiHack == 0)
	{
		return;
	}

	if(initialized != false)
	{
		return;
	}

	initialized = true;

	MHPClientDllMain((HANDLE)hInstance,DLL_PROCESS_ATTACH,0);
	MHPVerifyEntryProc(hInstance);
	MHPClientEntryProc();
}
