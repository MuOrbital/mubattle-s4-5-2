#include "stdafx.h"
#include "LuaReload.h"
#include "NewUISystem.h"
#include "LuaLoadImage.h"
#include "Interfaces.h"
#include "Descriptions.h"
#include "CustomWing.h"
#include <ItemPosition.h>
#include <NewOptions.h>
#include <ItemSize.h>

CLuaReload gLuaReload;

void CLuaReload::RestartLua()
{
	if (gProtect->m_MainInfo.ReloadLua == 1)
	{
		gLuaLoadImage.RestartLua();
		gInterface.RestartLua();
		gDescriptions->RestartLua();
		gItemManager->RestartLua();
		gCustomItemPosition.RestartLua();
		gItemNewOption.RestartLua();
		gCustomItemSize.RestartLua();
		gCustomWing.RestartLua();
		g_pChatListBox->AddText("", "[Client Side] Lua recarregado com sucesso!", SEASON3B::TYPE_ERROR_MESSAGE);
	}
}