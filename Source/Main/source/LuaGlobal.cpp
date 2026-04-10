#include "stdafx.h"
#include "LuaGlobal.h"
#include "LuaStack.hpp"
#include "CSItemOption.h"
#include "ZzzInterface.h"
#include "ZzzInventory.h"
#include "NewUISystem.h"
#include "MapManager.h"

using namespace luaaa;

LuaGlobal::LuaGlobal() {

}

LuaGlobal::~LuaGlobal() {

}

std::string GetLanguage() {
	if (_strcmpi(g_aszMLSelection, "Por") == 0) 
	{
		return "Por";
	}
	else if (_strcmpi(g_aszMLSelection, "Eng") == 0) 
	{
		return "Eng";
	}
	else if (_strcmpi(g_aszMLSelection, "Spn") == 0) 
	{
		return "Spn";
	}
	else 
	{
		return "Eng";
	}
}

void setLang(char * lang) {
	static DWORD m_CallFunc = 0x00409B10;
	static DWORD m_CallFunc2 = 0x00500E80;

	_asm {
		MOV EAX, lang;
		PUSH EAX;
		MOV ECX, 0x00E8C6EC;
		CALL[m_CallFunc];
		CALL[m_CallFunc2];
	}

}

void SetLanguage(char* value) 
{
	setLang(value);

	std::memcpy(g_aszMLSelection, value, size_t(strlen(g_aszMLSelection)));

	//gMenuWindow.ChangeLanguage(g_aszMLSelection);

	//gAntiHack.ClientSendMac();
}

std::string GetHardwareIdLua()
{
	char HardwareID[36] = { 0 };

	if (GetLocalComputerHardwareId(HardwareID, sizeof(HardwareID)))
	{
		return std::string(HardwareID);
	}

	return "UNKNOWN_HWID";
}


int GetWindowWidth() {
	return WindowWidth;
}

int GetWindowHeight() {
	return WindowHeight;
}

int GetFontValue() {
	return FontHeight;
}

void SetFontValue(int value) {
	//gMenuWindow.ChangeFontSize(value);
}

int GetResolution() {
	return m_Resolution;
}

void SetResolution(int value) {
	m_Resolution = value;

	//gMenuWindow.ChangeResolution(value);
}

int GetVolume() {
	return 0;//return ((int(__thiscall*)(LPVOID))0x849E80)(((LPVOID(__thiscall*)(LPVOID))0x861620)(pWindowThis()));
}

void SetVolume(int value) {
	//((void(__thiscall*)(LPVOID, int))0x849E60)(((LPVOID(__thiscall*)(LPVOID))0x861620)(pWindowThis()), value);
}

void SetHealthBar(int value) {
	//gMenuWindow.m_HealthBarDisabled = value;
}

void SetGlowSwitch(int value) {
	//((void(__thiscall*)(LPVOID, int))0x849EA0)(((LPVOID(__thiscall*)(LPVOID))0x861620)(pWindowThis()), value);
}

void SetWingSwitch(int value) {
	//gMenuWindow.m_WingDisabled = value;
}

char* GetNameByIndex(int index) 
{
	auto itemAtt = &ItemAttribute[index];

	if (itemAtt != nullptr) 
	{
		return itemAtt->Name;
	}

	return "not find";
}

void StartGames() {
	//((void(*)(int))0x4D6A60)(0);

	//lpViewObj lpObj = &*(ObjectPreview*)((DWORD(__thiscall*)(void*, DWORD))0x0096A4C0)(((void* (*)())0x00402BC0)(), ((int(*)(int))0x57D9A0)(*(DWORD*)0xE60B4C));
	//
	//*(WORD *)(*(DWORD*)0x8128AC8 + 14) = lpObj->Level;
	//*(BYTE *)(*(DWORD*)0x8128AC8 + 11) = lpObj->Class;
	//*(BYTE *)(*(DWORD*)0x8128AC8 + 12) = lpObj->Skin;
	//strcpy((char*)(*(DWORD*)0x8128AC8), lpObj->Name);
	//((void(*)())0x631610)();
	//*(BYTE*)0xE8CB4D = 0;
	//*(DWORD*)0xE609E8 = 3;
}

std::string GetCompleteNameByIndex(int index, int level, int exc) {
	static char buffer[120];
	std::memset(buffer, 0, sizeof(buffer));

	auto itemAtt = &ItemAttribute[index];

	int ItemIndex = index + 1171;

	if (ItemIndex == 8354) {
		return itemAtt->Name;
	}
	else {
		if (ItemIndex != 8356 && ItemIndex != 8357 && ItemIndex != 8358) {
			if (ItemIndex != 8350 || level != 7) {
				if (ItemIndex == 8351) {
					if (level)
					{
						if (level == 1) {
							return GlobalText[101];
						}
						else if (level == 2) {
							return GlobalText[104];
						}
					}
					else {
						return GlobalText[100];
					}
				}
				else
				{
					if (ItemIndex == 7842)
					{
						switch (level)
						{
						case 0:
							sprintf_s(buffer, "%s %s", GlobalText[168], itemAtt->Name);
							return &buffer[0];
						case 1:
							sprintf_s(buffer, "%s %s", GlobalText[169], itemAtt->Name);
							return &buffer[0];
						case 2:
							sprintf_s(buffer, "%s %s", GlobalText[167], itemAtt->Name);
							return &buffer[0];
						case 3:
							sprintf_s(buffer, "%s %s", GlobalText[166], itemAtt->Name);
							return &buffer[0];
						case 4:
							sprintf_s(buffer, "%s %s", GlobalText[1900], itemAtt->Name);
							return &buffer[0];
						default:
							return itemAtt->Name;
						}
					}
					else
					{
						switch (ItemIndex)
						{
						case 0x1EB2:
							if (level)
							{
								if (level == 1)
								{
									sprintf_s(buffer, "%s of %s", GlobalText[1214], itemAtt->Name);
									return &buffer[0];
								}
							}
							else
							{
								sprintf_s(buffer, "%s of %s", GlobalText[1187], itemAtt->Name);
								return &buffer[0];
							}
							break;
						case 0x2535:
							return GlobalText[1235];
						case 0x2529:
							return GlobalText[105];
						case 0x252A:
							if (level == 14)
							{
								return GlobalText[1650];
							}
							else
							{
								if (level == 15) {
									return GlobalText[1651];
								}

								return GlobalText[106];
							}
							break;
						case 0x252B:
							if (level == 13)
							{
								return GlobalText[117];
							}
							else
							{
								return GlobalText[107];
							}
							break;
						case 0x252C:
							return GlobalText[108];
						case 0x252D:
							return GlobalText[109];
						case 0x252E:
							return GlobalText[110];
						case 0x252F:
							sprintf_s(buffer, "%s +%d", GlobalText[115], level - 7);
							return &buffer[0];
						default:
							if (ItemIndex != 8371 || level != 1)
							{
								if (ItemIndex != 8372 || level != 1)
								{
									if (ItemIndex != 8373 || level != 1)
									{
										switch (ItemIndex)
										{
										case 0x253A:
											return GlobalText[2012];
										case 0x253B:
											return GlobalText[2013];
										case 0x253C:
											return GlobalText[2014];
										case 0x2530:
											if (level == 2) {
												return GlobalText[1098];
											}

											return GlobalText[810];
										case 0x2531:
											return GlobalText[906];
										case 0x2532:
											return GlobalText[907];
										default:
											switch (ItemIndex)
											{
											case 0x2533:
												if (level == 2)
												{
													return GlobalText[928];
												}
												else
												{
													if (level == 3) {
														return GlobalText[929];
													}
			
													return GlobalText[922];
												}
												break;
											case 0x2534:
												return GlobalText[925];
											default:
												if (ItemIndex != 8360 || level != 3)
												{
													if (ItemIndex == 8346)
													{
														if (level)
														{
															if (level == 1)
															{
																return GlobalText[1414];
															}
														}
														else
														{
															return GlobalText[1413];
														}
													}
													else if (ItemIndex == 7834)
													{
														if (level)
														{
															if (level == 1)
															{
																return GlobalText[1461];
															}
														}
														else
														{
															return GlobalText[1460];
														}
													}
													else
													{
														switch (ItemIndex)
														{
														case 0x1E9E:
															if (level)
															{
																if (level == 1)
																{
																	return GlobalText[1462];
																}
															}
															else
															{
																return GlobalText[1416];
															}
															break;
														case 0x2537:
															return GlobalText[1462];
														default:
															break;
														}
													}
												}
												else
												{
													return GlobalText[1290];
												}
												break;
											}
											break;
										}
									}
									else
									{
										return GlobalText[2014];
									}
								}
								else
								{
									return GlobalText[2013];
								}
							}
							else
							{
								return GlobalText[2012];
							}
							break;
						}
					}
				}
			}
			else
			{
				return GlobalText[111];
			}
		}
		else if ((exc >> 3) & 0xF)
		{
			sprintf_s(buffer, "%s +%d", itemAtt->Name, (exc >> 3) & 0xF);
			return &buffer[0];
		}
		else
		{
			return itemAtt->Name;
		}
	}

	return itemAtt->Name;;
}

int GetWidthByIndex(int index) 
{
	auto itemAtt = &ItemAttribute[index];

	if (itemAtt != nullptr) {
		return itemAtt->Width;
	}

	return -1;
}

int GetHeightByIndex(int index) 
{
	auto itemAtt = &ItemAttribute[index];

	if (itemAtt != nullptr) {
		return itemAtt->Height;
	}

	return -1;
}

int GetSlotByIndex(int index) 
{
	auto itemAtt = &ItemAttribute[index];

	if (itemAtt != nullptr) {
		return itemAtt->m_byItemSlot;
	}

	return -1;
}

int GetInventoryMouseSlot() {
	return g_pMyInventory->GetPointedItemIndex();
}

int GetInventoryMouseItemSlot()
{	
	//DWORD ItemSlot = m_pNewInventoryCtrl->FindItemptIndex(MouseX, MouseY);
	//
	//if (ItemSlot != -1) {
	//	return (ItemSlot + 12);
	//}

	return -1;
}

int GetInventoryMouseItemIndex() 
{
	DWORD index = g_pMyInventory->GetPointedItemIndex();
	
	//if (index != -1) {
	//	auto item = pInventoryCtrlFindItem(pInventoryGetMyInventoryCtrl(pInventoryThis(pWindowThis()), 0), pCursorX, pCursorY);
	//
	//	if (item) {
	//		return item->Type;
	//	}
	//}

	return -1;
}

int GetInventoryMouseItemLevel() {
	//DWORD index = pInventoryGetPointedItemIndex(pInventoryThis(pWindowThis()));
	//
	//if (index != -1) {
	//	auto item = pInventoryCtrlFindItem(pInventoryGetMyInventoryCtrl(pInventoryThis(pWindowThis()), 0), pCursorX, pCursorY);
	//
	//	if (item) {
	//		return (item->Level >> 3 & 0xF);
	//	}
	//}

	return -1;
}

int GetInventoryMouseItemExc() {
	//DWORD index = pInventoryGetPointedItemIndex(pInventoryThis(pWindowThis()));
	//
	//if (index != -1) {
	//	auto item = pInventoryCtrlFindItem(pInventoryGetMyInventoryCtrl(pInventoryThis(pWindowThis()), 0), pCursorX, pCursorY);
	//
	//	if (item) {
	//		return item->ExtOption;
	//	}
	//}

	return -1;
}

void SendMessageClient(std::string msg) 
{
	g_pChatListBox->AddText("", msg.c_str(), SEASON3B::TYPE_SYSTEM_MESSAGE);
}

char* GetGlobalText(int index) {
	
	char* name = (char*)GlobalText[index];

	return name;
}

std::string GetMapName(int map) 
{
	return gMapManager.GetMapName(map);
}

int GetCountParty() 
{
	return PartyNumber;
}

std::string GetMonsterName(int Class) 
{
	for (int i = 0; i < MAX_MONSTER; ++i)
	{
		if (MonsterScript[i].Type == Class)
		{
			return MonsterScript[i].Name;
		}
	}

	return "Not Find";
}

float getFPS()
{
	return (float)FPS_AVG;
}

void LuaGlobal::RegisterGlobal(lua_State * lua)
{
	//main funcs
	LuaModule(lua).fun("GetLanguage", &GetLanguage);
	LuaModule(lua).fun("SetLanguage", &SetLanguage);
	LuaModule(lua).fun("GetWindowWidth", &GetWindowWidth);
	LuaModule(lua).fun("GetWindowHeight", &GetWindowHeight);
	LuaModule(lua).fun("GetFontValue", &GetFontValue);
	LuaModule(lua).fun("SetFontValue", &SetFontValue);
	LuaModule(lua).fun("GetResolution", &GetResolution);
	LuaModule(lua).fun("SetResolution", &SetResolution);
	LuaModule(lua).fun("GetVolume", &GetVolume);
	LuaModule(lua).fun("SetVolume", &SetVolume);
	LuaModule(lua).fun("SetHealthBarSwitch", &SetHealthBar);
	LuaModule(lua).fun("SetGlowSwitch", &SetGlowSwitch);
	LuaModule(lua).fun("SetWingSwitch", &SetWingSwitch);

	//Item funcs
	LuaModule(lua).fun("GetNameByIndex", &GetNameByIndex);
	LuaModule(lua).fun("GetCompleteNameByIndex", &GetCompleteNameByIndex);
	LuaModule(lua).fun("GetWidthByIndex", &GetWidthByIndex);
	LuaModule(lua).fun("GetHeightByIndex", &GetHeightByIndex);
	LuaModule(lua).fun("GetSlotByIndex", &GetSlotByIndex);

	//Inventory funcs
	LuaModule(lua).fun("GetInventoryMouseSlot", &GetInventoryMouseSlot);
	LuaModule(lua).fun("GetInventoryMouseItemSlot", &GetInventoryMouseItemSlot);
	LuaModule(lua).fun("GetInventoryMouseItemIndex", &GetInventoryMouseItemIndex);
	LuaModule(lua).fun("GetInventoryMouseItemLevel", &GetInventoryMouseItemLevel);
	LuaModule(lua).fun("GetInventoryMouseItemExc", &GetInventoryMouseItemExc);

	LuaModule(lua).fun("SendMessageClient", &SendMessageClient);

	LuaModule(lua).fun("GetGlobalText", &GetGlobalText);

	LuaModule(lua).fun("GetMapName", &GetMapName);

	LuaModule(lua).fun("GetCountParty", &GetCountParty);

	LuaModule(lua).fun("GetMonsterName", &GetMonsterName);

	LuaModule(lua).fun("getFPS", &getFPS);

	LuaModule(lua).fun("GetHardwareId", &GetHardwareIdLua);
}
