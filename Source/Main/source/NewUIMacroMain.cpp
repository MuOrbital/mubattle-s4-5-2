#include "stdafx.h"
#include "NewUISystem.h"
#include "DSPlaySound.h"
#include "ZzzInterface.h"
#include "CAIController.h"
#include "NewUIMacroMain.h"
#include "CharacterManager.h"

SEASON3B::CNewUIMacroMain::CNewUIMacroMain()
{
	m_stSkillListPos[0].x = 0;
	m_stSkillListPos[1].x = 0;
	m_stSkillListPos[0].y = 0;
	m_stSkillListPos[1].y = 0;

	m_pNewUIMng = NULL;
	m_iCurrentClass = 0;
	m_Pos.x = 0;
	m_Pos.y = 0;
	m_RangeHunting = 1;
	m_RangePickUp = 1;
	m_iNumCurOpenTab = 0;
	m_SkillATTK[0] = -1;
	m_SkillATTK[1] = -1;
	m_SkillATTK[2] = -1;
	m_SkillBuff[0] = -1;
	m_SkillBuff[1] = -1;
	m_SkillBuff[2] = -1;
	IsUsedSkill[0] = 0;
	IsUsedSkill[1] = 0;
	IsUsedSkill[2] = 0;
	IsUsedSkill[3] = 0;
	IsUsedSkill[4] = 0;
	IsUsedSkill[5] = 0;
	tooltipvisible = 0;
	m_iRenderSkillInfoType = -1;
	m_iRenderSkillInfoPosX = 0;
	m_iRenderSkillInfoPosY = 0;
	m_iRenderListType = 0;
	m_temp_skills = -1;
}

SEASON3B::CNewUIMacroMain::~CNewUIMacroMain()
{
	Release();
}


bool SEASON3B::CNewUIMacroMain::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	bool Success = false;

	if (pNewUIMng)
	{
		m_pNewUIMng = pNewUIMng;

		m_pNewUIMng->AddUIObj(INTERFACE_MACRO_OFICIAL, this);

		this->LoadImages();

		m_ListBox.Create(x + 18, y + 234, 154, 78, 5, MAX_PICK_EXTRA);

		this->SetPos(x, y);

		this->InitButtons();

		this->InitCheckBoxs();

		this->InitInputBoxs();

		this->InitTexts();

		this->InitImages();

		this->InitOptionButtonGroup();

		Show(false);

		Success = true;
	}

	return Success;
}

void SEASON3B::CNewUIMacroMain::Release()
{
	UnloadImages();

	ClearMap(m_checkBox);
	ClearMap(m_InputBox);
	ClearMap(m_Buttons);
	ClearMap(m_RadioOption);
	ClearMap(m_stMacroUIText);
	ClearMap(m_stMacroUIImage);

	for (int i = 0; i < MAX_CLASS_PLAYER; i++)
	{
		m_checkBoxGroup[i].clear();
		m_RadioOptionGroup[i].clear();
		m_InputBoxGroup[i].clear();
		m_buttonGroup[i].clear();
		m_MacroUITextGroup[i].clear();
		m_MacroUIImageGroup[i].clear();
	}

	for (size_t i = 0; i < 2; i++)
	{
		std::list<stSkillList*>::iterator li = m_stSkillList[i].begin();
		for (; li != m_stSkillList[i].end(); li++)
		{
			SAFE_DELETE((*li));
		}
		m_stSkillList[i].clear();
	}

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

bool SEASON3B::CNewUIMacroMain::UpdateKeyEvent()
{
	bool Success = true;
	if (IsVisible())
	{
		if (IsPress(VK_ESCAPE) == true)
		{
			g_pNewUISystem->Hide(INTERFACE_MACRO_OFICIAL);
			PlayBuffer(SOUND_CLICK01);
			Success = false;
		}
	}
	return Success;
}

bool SEASON3B::CNewUIMacroMain::UpdateMouseEvent()
{
	int iNumCurOpenTab = m_iTab.UpdateMouseEvent();
	if (iNumCurOpenTab != -1 && iNumCurOpenTab != m_iNumCurOpenTab)
	{
		m_iNumCurOpenTab = iNumCurOpenTab;
		this->CloseSkillList();
		// MuBattle's party window has no movable-state wrapper; closing the skill
		// list is sufficient and avoids forcing the existing party UI off-screen.
	}
	if (this->UpdateButtons() == true)
	{
		this->CloseSkillList();
	}
	else
	{
		if (m_iNumCurOpenTab)
		{
			if (m_iNumCurOpenTab == 1)
			{
				for (auto it = m_InputBoxGroup[m_iCurrentClass].begin(); it != m_InputBoxGroup[m_iCurrentClass].end(); ++it)
				{
					if (it->first == 3)
					{
						CUITextInputBox* temp_input = it->second;
						if (temp_input->HaveFocus())
						{
							if (GetRelatedWnd() != temp_input->GetHandle())
							{
								SetRelatedWnd(temp_input->GetHandle());
							}
						}
						if (IsPress(VK_LBUTTON))
						{
							int x = temp_input->GetPosition_x();
							int y = temp_input->GetPosition_y();
							int width = temp_input->GetWidth();
							int height = temp_input->GetHeight();
							if (!CheckMouseIn(x, y, width, height))
								SetFocus(g_hWnd);
						}
						temp_input->DoAction();
						if (GetRelatedWnd() != g_hWnd)
							SetRelatedWnd(g_hWnd);
					}
				}
				m_ListBox.UpdateMouseEvent();
			}
		}
		else
		{
			for (auto it = m_InputBoxGroup[m_iCurrentClass].begin(); it != m_InputBoxGroup[m_iCurrentClass].end(); ++it)
			{
				if (it->first >= 0 && it->first <= 2)
				{
					CUITextInputBox* temp_input = it->second;
					if (temp_input->HaveFocus())
					{
						if (GetRelatedWnd() != temp_input->GetHandle())
						{
							SetRelatedWnd(temp_input->GetHandle());
						}
					}
					if (IsPress(VK_LBUTTON))
					{
						int x = temp_input->GetPosition_x();
						int y = temp_input->GetPosition_y();
						int width = temp_input->GetWidth();
						int height = temp_input->GetHeight();
						if (!CheckMouseIn(x, y, width, height))
							SetFocus(g_hWnd);
					}
					temp_input->DoAction();
					if (GetRelatedWnd() != g_hWnd)
						SetRelatedWnd(g_hWnd);
				}
			}
			if (ImageMapUpdateMouseEvent(0) == true)
			{
				if (IsPress(VK_LBUTTON))
				{
					m_iRenderListType = 1;
					m_temp_skills = m_SkillATTK[0];
					CloseSkillListExceptOne(&IsUsedSkill[0]);
				}
				else if (IsPress(VK_RBUTTON))
				{
					m_SkillATTK[0] = -1;
				}
			}
			else if (ImageMapUpdateMouseEvent(1) == true)
			{
				if (IsPress(VK_LBUTTON))
				{
					m_iRenderListType = 1;
					m_temp_skills = m_SkillATTK[1];
					CloseSkillListExceptOne(&IsUsedSkill[1]);
				}
				else if (IsPress(VK_RBUTTON))
				{
					m_SkillATTK[1] = -1;
				}
			}
			else if (ImageMapUpdateMouseEvent(2) == true)
			{
				if (IsPress(VK_LBUTTON))
				{
					m_iRenderListType = 1;
					m_temp_skills = m_SkillATTK[2];
					CloseSkillListExceptOne(&IsUsedSkill[2]);
				}
				else if (IsPress(VK_RBUTTON))
				{
					m_SkillATTK[2] = -1;
				}
			}
			else if (ImageMapUpdateMouseEvent(3) == true)
			{
				if (IsPress(VK_LBUTTON))
				{
					m_iRenderListType = 2;
					m_temp_skills = m_SkillBuff[0];
					CloseSkillListExceptOne(&IsUsedSkill[3]);
				}
				else if (IsPress(VK_RBUTTON))
				{
					m_SkillBuff[0] = -1;
				}
			}
			else if (ImageMapUpdateMouseEvent(4) == true)
			{
				if (IsPress(VK_LBUTTON))
				{
					m_iRenderListType = 2;
					m_temp_skills = m_SkillBuff[1];
					CloseSkillListExceptOne(&IsUsedSkill[4]);
				}
				else if (IsPress(VK_RBUTTON))
				{
					m_SkillBuff[1] = -1;
				}
			}
			else if (ImageMapUpdateMouseEvent(5) == true)
			{
				if (IsPress(VK_LBUTTON))
				{
					m_iRenderListType = 2;
					m_temp_skills = m_SkillBuff[2];
					CloseSkillListExceptOne(&IsUsedSkill[5]);
				}
				else if (IsPress(VK_RBUTTON))
				{
					m_SkillBuff[2] = -1;
				}
			}
			UpdateMouseEventSelectedSkill();
		}

		if (IsUsedSkillList())
		{
			int skill_index = -1;
			if (UpdateMouseEventSkillList(&skill_index) == true)
			{
				if (skill_index != -1)
					RegisterSkillIndex(skill_index);
				return false;
			}
		}
	}

	return !CheckMouseIn(m_Pos.x, m_Pos.y, 190, 429);
}

void SEASON3B::CNewUIMacroMain::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;

#if MAIN_UPDATE > 603
	m_iTab.ChangeRadioButtonInfo(true, x + 18.f, y + 30.f, 56, 22);
	m_ListBox.SetPos(x + 22, y + 218);
#else
	m_iTab.ChangeRadioButtonInfo(true, x + 10.f, y + 48.f, 56, 22);
	m_ListBox.SetPos(x + 18, y + 238);
#endif

}

void SEASON3B::CNewUIMacroMain::UI2DEffectCallback(LPVOID pClass, DWORD dwParamA, DWORD dwParamB)
{
	if (pClass)
	{
		((CNewUIMacroMain*)pClass)->RenderSkillInfo();
	}
}

bool SEASON3B::CNewUIMacroMain::RenderSkillInfo()
{
	if (tooltipvisible == true && m_iRenderSkillInfoType != -1)
		::RenderSkillInfo(m_iRenderSkillInfoPosX + 15, m_iRenderSkillInfoPosY - 10, m_iRenderSkillInfoType);

	return false;
}

bool SEASON3B::CNewUIMacroMain::UpdateButtons()
{
	if (IsRelease(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 169, m_Pos.y + 7, 13, 12))
	{
		g_pNewUISystem->Hide(INTERFACE_MACRO_OFICIAL);

		if (g_pNewUISystem->IsVisible(INTERFACE_MACRO_OFICIAL_SUB) == true)
		{
			g_pNewUISystem->Hide(INTERFACE_MACRO_OFICIAL_SUB);
		}
		return true;
	}

	if (m_iNumCurOpenTab)
	{
		if (m_iNumCurOpenTab == 1)
		{
			for (auto it = m_checkBoxGroup[m_iCurrentClass].begin(); it != m_checkBoxGroup[m_iCurrentClass].end(); ++it)
			{
				if (it->first >= 13 && it->first <= 20)
				{
					CCheckBox* temp_check = it->second;
					if (temp_check->UpdateMouseEvent())
					{
						if (it->first == 14)
						{
							this->SetCheckBoxValue(15, false);
						}
						else if (it->first == 15)
						{
							this->SetCheckBoxValue(14, false);
						}
					}
				}
			}
			for (auto it = m_buttonGroup[m_iCurrentClass].begin(); it != m_buttonGroup[m_iCurrentClass].end(); ++it)
			{
				CNewUIButton* temp_button = it->second;

				if (it->first == 15 && temp_button->UpdateMouseEvent())
				{
					g_pNewUISystem->Hide(INTERFACE_MACRO_OFICIAL);
					if (g_pNewUISystem->IsVisible(INTERFACE_MACRO_OFICIAL_SUB) == true)
					{
						g_pNewUISystem->Hide(INTERFACE_MACRO_OFICIAL_SUB);
					}
					return true;
				}
				if (it->first == 9 && temp_button->UpdateMouseEvent())
				{
					if (m_RangePickUp < 8)
						++m_RangePickUp;
					return true;
				}
				if (it->first == 10 && temp_button->UpdateMouseEvent())
				{
					if (m_RangePickUp > 1)
						--m_RangePickUp;
					return true;
				}
				if (it->first == 11 && temp_button->UpdateMouseEvent())
				{
					char pszText[254];
					memset(pszText, 0, sizeof(pszText));
					this->GetInputBoxText(3, pszText);

					if (!strcmp(pszText, ""))
					{
						CreateOkMessageBox(GlobalText[3567], -1);
					}
					else
					{
						this->SetInputBoxText(3, "");
						if (!m_ListBox.AddText(pszText))
						{
							CreateOkMessageBox(GlobalText[3568], -1);
						}
					}
					return true;
				}
				if (it->first == 12 && temp_button->UpdateMouseEvent())
				{
					m_ListBox.DelSelectedListBoxText();
					return true;
				}
				if (it->first == 13 && temp_button->UpdateMouseEvent())
				{
					this->SaveConfig();
					g_pMacroSubWindow->SaveConfig();

					g_pNewUISystem->Hide(INTERFACE_MACRO_OFICIAL);
					if (g_pNewUISystem->IsVisible(INTERFACE_MACRO_OFICIAL_SUB) == true)
					{
						g_pNewUISystem->Hide(INTERFACE_MACRO_OFICIAL_SUB);
					}
					return true;
				}
				if (it->first == 14 && temp_button->UpdateMouseEvent())
				{
					this->ResetConfig();
					g_pMacroSubWindow->ResetConfig();
					return true;
				}
			}
		}
		else if (m_iNumCurOpenTab == 2)
		{
			for (auto it = m_checkBoxGroup[m_iCurrentClass].begin(); it != m_checkBoxGroup[m_iCurrentClass].end(); ++it)
			{
				CCheckBox* temp_check = it->second;
				if (it->first >= 21 && it->first <= 23 && temp_check->UpdateMouseEvent())
				{
					return true;
				}
			}

			for (auto it = m_buttonGroup[m_iCurrentClass].begin(); it != m_buttonGroup[m_iCurrentClass].end(); ++it)
			{
				CNewUIButton* temp_button = it->second;

				if (it->first == 15 && temp_button->UpdateMouseEvent())
				{
					g_pNewUISystem->Hide(INTERFACE_MACRO_OFICIAL);
					if (g_pNewUISystem->IsVisible(INTERFACE_MACRO_OFICIAL_SUB) == true)
					{
						g_pNewUISystem->Hide(INTERFACE_MACRO_OFICIAL_SUB);
					}
					return true;
				}
				if (it->first == 13 && temp_button->UpdateMouseEvent())
				{
					this->SaveConfig();
					g_pMacroSubWindow->SaveConfig();

					g_pNewUISystem->Hide(INTERFACE_MACRO_OFICIAL);
					if (g_pNewUISystem->IsVisible(INTERFACE_MACRO_OFICIAL_SUB) == true)
					{
						g_pNewUISystem->Hide(INTERFACE_MACRO_OFICIAL_SUB);
					}
					return true;
				}
				if (it->first == 14 && temp_button->UpdateMouseEvent())
				{
					this->ResetConfig();
					g_pMacroSubWindow->ResetConfig();
					return true;
				}
			}
		}
	}
	else
	{
		for (auto it = m_RadioOptionGroup[m_iCurrentClass].begin(); it != m_RadioOptionGroup[m_iCurrentClass].end(); ++it)
		{
			COptionButtonGroup* option = it->second;

			if (option->UpdateMouseEvent())
			{
				return true;
			}
		}

		for (auto it = m_checkBoxGroup[m_iCurrentClass].begin(); it != m_checkBoxGroup[m_iCurrentClass].end(); ++it)
		{
			CCheckBox* temp_check = it->second;
			if (it->first >= 0 && it->first <= 12 && temp_check->UpdateMouseEvent())
			{
				if (it->first == 7 && temp_check->IsSelected() == true)
				{
					if (m_SkillATTK[0] == -1 || m_SkillATTK[1] == -1 || m_SkillATTK[2] == -1)
					{
						CreateOkMessageBox(GlobalText[3565], -1);
						return true;
					}
					if (this->GetCheckBoxValue(3) == true)
					{
						this->SetCheckBoxValue(3, false);
					}
					if (this->GetCheckBoxValue(4) == true)
					{
						this->SetCheckBoxValue(4, false);
					}
					if (this->GetCheckBoxValue(5) == true)
					{
						this->SetCheckBoxValue(5, false);
					}
					if (this->GetCheckBoxValue(6) == true)
					{
						this->SetCheckBoxValue(6, false);
					}
				}
				if (it->first == 3)
				{
					if (this->GetCheckBoxValue(7) == true)
					{
						this->SetCheckBoxValue(7, false);
						CreateOkMessageBox(GlobalText[3566], -1);
						return true;
					}
					if (this->GetCheckBoxValue(4) == true)
					{
						this->SetCheckBoxValue(4, false);
					}
				}
				else if (it->first == 4)
				{
					if (this->GetCheckBoxValue(7) == true)
					{
						this->SetCheckBoxValue(7, false);
						CreateOkMessageBox(GlobalText[3566], -1);
						return true;
					}
					if (this->GetCheckBoxValue(3) == true)
					{
						this->SetCheckBoxValue(3, false);
					}
				}
				if (it->first == 5)
				{
					if (this->GetCheckBoxValue(7) == true)
					{
						this->SetCheckBoxValue(7, false);
						CreateOkMessageBox(GlobalText[3566], -1);
						return true;
					}
					if (this->GetCheckBoxValue(6) == true)
					{
						this->SetCheckBoxValue(6, false);
					}
				}
				else if (it->first == 6)
				{
					if (this->GetCheckBoxValue(7) == true)
					{
						this->SetCheckBoxValue(7, false);
						CreateOkMessageBox(GlobalText[3566], -1);
						return true;
					}
					if (this->GetCheckBoxValue(5) == true)
					{
						this->SetCheckBoxValue(5, false);
					}
				}
				return true;
			}
		}
		for (auto it = m_buttonGroup[m_iCurrentClass].begin(); it != m_buttonGroup[m_iCurrentClass].end(); ++it)
		{
			CNewUIButton* temp_button = it->second;

			if (it->first == 15 && temp_button->UpdateMouseEvent())
			{
				g_pNewUISystem->Hide(INTERFACE_MACRO_OFICIAL);
				if (g_pNewUISystem->IsVisible(INTERFACE_MACRO_OFICIAL_SUB) == true)
				{
					g_pNewUISystem->Hide(INTERFACE_MACRO_OFICIAL_SUB);
				}
				return true;
			}
			if (it->first == 0 && temp_button->UpdateMouseEvent())
			{
				if (m_RangeHunting < 8)
					++m_RangeHunting;
				return true;
			}
			if (it->first == 1 && temp_button->UpdateMouseEvent())
			{
				if (m_RangeHunting > 1)
					--m_RangeHunting;
				return true;
			}
			if (it->first == 2 && temp_button->UpdateMouseEvent())
			{
				if (g_pMacroSubWindow->GetWindowsType() == 1)
				{
					g_pNewUISystem->Toggle(INTERFACE_MACRO_OFICIAL_SUB);
				}
				else
				{
					g_pNewUISystem->Show(INTERFACE_MACRO_OFICIAL_SUB);
				}
				g_pMacroSubWindow->OpenningProcess(1);
				return true;
			}
			if (it->first == 3 && temp_button->UpdateMouseEvent())
			{
				if (g_pMacroSubWindow->GetWindowsType() == 2)
				{
					g_pNewUISystem->Toggle(INTERFACE_MACRO_OFICIAL_SUB);
				}
				else
				{
					g_pNewUISystem->Show(INTERFACE_MACRO_OFICIAL_SUB);
				}
				g_pMacroSubWindow->OpenningProcess(2);
				return true;
			}
			if (it->first == 4 && temp_button->UpdateMouseEvent())
			{
				if (g_pMacroSubWindow->GetWindowsType() == 3)
				{
					g_pNewUISystem->Toggle(INTERFACE_MACRO_OFICIAL_SUB);
				}
				else
				{
					g_pNewUISystem->Show(INTERFACE_MACRO_OFICIAL_SUB);
				}
				g_pMacroSubWindow->OpenningProcess(3);
				return true;
			}
			if (it->first == 5 && temp_button->UpdateMouseEvent())
			{
				if (g_pMacroSubWindow->GetWindowsType() == 4)
				{
					g_pNewUISystem->Toggle(INTERFACE_MACRO_OFICIAL_SUB);
				}
				else
				{
					g_pNewUISystem->Show(INTERFACE_MACRO_OFICIAL_SUB);
				}
				g_pMacroSubWindow->OpenningProcess(4);
				return true;
			}
			if (it->first == 6 && temp_button->UpdateMouseEvent())
			{
				if (g_pMacroSubWindow->GetWindowsType() == 5)
				{
					g_pNewUISystem->Toggle(INTERFACE_MACRO_OFICIAL_SUB);
				}
				else
				{
					g_pNewUISystem->Show(INTERFACE_MACRO_OFICIAL_SUB);
				}
				g_pMacroSubWindow->OpenningProcess(5);
				return true;
			}
			if (it->first == 7 && temp_button->UpdateMouseEvent())
			{
				if (g_pMacroSubWindow->GetWindowsType() == 7)
				{
					g_pNewUISystem->Toggle(INTERFACE_MACRO_OFICIAL_SUB);
				}
				else
				{
					g_pNewUISystem->Show(INTERFACE_MACRO_OFICIAL_SUB);
				}
				g_pMacroSubWindow->OpenningProcess(7);
				return true;
			}
			if (it->first == 8 && temp_button->UpdateMouseEvent())
			{
				if (g_pMacroSubWindow->GetWindowsType() == 6)
				{
					g_pNewUISystem->Toggle(INTERFACE_MACRO_OFICIAL_SUB);
				}
				else
				{
					g_pNewUISystem->Show(INTERFACE_MACRO_OFICIAL_SUB);
				}
				g_pMacroSubWindow->OpenningProcess(6);
				return true;
			}
			if (it->first == 13 && temp_button->UpdateMouseEvent())
			{
				this->SaveConfig();
				g_pMacroSubWindow->SaveConfig();

				g_pNewUISystem->Hide(INTERFACE_MACRO_OFICIAL);
				if (g_pNewUISystem->IsVisible(INTERFACE_MACRO_OFICIAL_SUB) == true)
				{
					g_pNewUISystem->Hide(INTERFACE_MACRO_OFICIAL_SUB);
				}
				return true;
			}
			if (it->first == 14 && temp_button->UpdateMouseEvent())
			{
				this->ResetConfig();
				g_pMacroSubWindow->ResetConfig();
				return true;
			}
		}
	}
	return false;
}

bool SEASON3B::CNewUIMacroMain::Update()
{
	return true;
}

bool SEASON3B::CNewUIMacroMain::Render()
{
	EnableAlphaTest(true);
	glColor4f(1.0, 1.0, 1.0, 1.0);

	this->RenderFrame();

	if (m_iNumCurOpenTab)
	{
		if (m_iNumCurOpenTab == 1)
		{
			for (auto it = m_MacroUIImageGroup[m_iCurrentClass].begin(); it != m_MacroUIImageGroup[m_iCurrentClass].end(); ++it)
			{
				stMacroUIImage* temp_image = it->second;
				if (it->first == 9 || it->first == (m_RangePickUp + 17))
				{
					temp_image->Render();
				}
			}
			for (auto it = m_MacroUITextGroup[m_iCurrentClass].begin(); it != m_MacroUITextGroup[m_iCurrentClass].end(); ++it)
			{
				stMacroUIText* temp_text = it->second;
				if (it->first >= 11 && it->first <= 12)
				{
					temp_text->Render();
				}
			}
			for (auto it = m_checkBoxGroup[m_iCurrentClass].begin(); it != m_checkBoxGroup[m_iCurrentClass].end(); ++it)
			{
				CCheckBox* temp_check = it->second;
				if (it->first >= checkbox_RepairItem && it->first <= checkbox_Pick_AddExtra)
				{
					temp_check->Render();
				}
			}
			for (auto it = m_InputBoxGroup[m_iCurrentClass].begin(); it != m_InputBoxGroup[m_iCurrentClass].end(); ++it)
			{
				if (it->first == 3)
				{
					CUITextInputBox* temp_input = it->second;
					temp_input->Render();

				}
			}
			for (auto it = m_buttonGroup[m_iCurrentClass].begin(); it != m_buttonGroup[m_iCurrentClass].end(); ++it)
			{
				CNewUIButton* temp_button = it->second;
				if (it->first >= button_PickUp_Max && it->first <= button_Exit)
				{
					temp_button->Render();
				}
			}

			m_ListBox.Render();
		}
		else if (m_iNumCurOpenTab == 2)
		{
			for (auto it = m_checkBoxGroup[m_iCurrentClass].begin(); it != m_checkBoxGroup[m_iCurrentClass].end(); ++it)
			{
				CCheckBox* temp_check = it->second;
				if (it->first >= checkbox_PartyAutoRequestFriend && it->first <= checkbox_PartyAutoRequestGuild)
				{
					temp_check->Render();
				}
			}
			for (auto it = m_buttonGroup[m_iCurrentClass].begin(); it != m_buttonGroup[m_iCurrentClass].end(); ++it)
			{
				CNewUIButton* temp_button = it->second;
				if (it->first >= button_Save && it->first <= button_Exit)
				{
					temp_button->Render();
				}
			}
		}
	}
	else
	{
		POINT temp_position;

		for (auto it = m_MacroUIImageGroup[m_iCurrentClass].begin(); it != m_MacroUIImageGroup[m_iCurrentClass].end(); ++it)
		{
			stMacroUIImage* temp_image = it->second;
#if MAIN_UPDATE > 603
			if ((it->first >= 6 && it->first <= 8) || it->first == (m_RangeHunting + 9))
#else
			if ((it->first >= 0 && it->first <= 8) || it->first == (m_RangeHunting + 9))
#endif
			{
				temp_image->Render();
			}
		}
		for (auto it = m_MacroUITextGroup[m_iCurrentClass].begin(); it != m_MacroUITextGroup[m_iCurrentClass].end(); ++it)
		{
			stMacroUIText* temp_text = it->second;
			if (it->first >= 0 && it->first <= 10)
			{
				temp_text->Render();
			}
		}
		for (auto it = m_checkBoxGroup[m_iCurrentClass].begin(); it != m_checkBoxGroup[m_iCurrentClass].end(); ++it)
		{
			CCheckBox* temp_check = it->second;
			if (it->first >= checkbox_AutoPotion && it->first <= checkbox_DrainLife)
			{
				temp_check->Render();
			}
		}
		for (auto it = m_InputBoxGroup[m_iCurrentClass].begin(); it != m_InputBoxGroup[m_iCurrentClass].end(); ++it)
		{
			CUITextInputBox* temp_input = it->second;
			if (it->first >= 0 && it->first <= 2)
			{
				temp_input->Render();
			}
		}
		for (auto it = m_buttonGroup[m_iCurrentClass].begin(); it != m_buttonGroup[m_iCurrentClass].end(); ++it)
		{
			CNewUIButton* temp_button = it->second;
			if ((it->first >= 0 && it->first <= 8) || (it->first >= 13 && it->first <= 15))
			{
				temp_button->Render();
			}
		}
		for (auto it = m_RadioOptionGroup[m_iCurrentClass].begin(); it != m_RadioOptionGroup[m_iCurrentClass].end(); ++it)
		{
			COptionButtonGroup* option = it->second;

			option->Render();
		}

		if (this->IsUsedSkillList())
		{
			this->RenderSkillList();
		}

		for (int k = 0; k < 3; k++)
		{
#if MAIN_UPDATE > 603
			this->GetImagePositionInImageMap(&temp_position, k);
			SEASON3B::RenderImageF(BITMAP_skill_box, temp_position.x + 2.f, temp_position.y + 1.f, 28.f, 38.f, 0.0, 0.0, 104.f, 158.f);
			if (m_SkillATTK[k] >= 0)
			{
				g_pSkillList->RenderSkillIcon(m_SkillATTK[k], temp_position.x + 6, temp_position.y + 6, 20.0, 28.0);
			}

			this->GetImagePositionInImageMap(&temp_position, k + 3);
			SEASON3B::RenderImageF(BITMAP_skill_box, temp_position.x + 2.f, temp_position.y + 1.f, 28.f, 38.f, 0.0, 0.0, 104.f, 158.f);

			if (m_SkillBuff[k] >= 0)
			{
				g_pSkillList->RenderSkillIcon(m_SkillBuff[k], temp_position.x + 6, temp_position.y + 6, 20.0, 28.0);
			}
#else
			if (m_SkillATTK[k] >= 0)
			{
				this->GetImagePositionInImageMap(&temp_position, k);
				g_pSkillList->RenderSkillIcon(m_SkillATTK[k], temp_position.x + 6, temp_position.y + 6, 20.0, 28.0);
			}

			if (m_SkillBuff[k] >= 0)
			{
				this->GetImagePositionInImageMap(&temp_position, k + 3);
				g_pSkillList->RenderSkillIcon(m_SkillBuff[k], temp_position.x + 6, temp_position.y + 6, 20.0, 28.0);
			}
#endif
		}

		/*if (m_SkillATTK[1] >= 0)
		{
			this->GetImagePositionInImageMap(&temp_position, 1);
			g_pSkillList->RenderSkillIcon(m_SkillATTK[1], temp_position.x + 6, temp_position.y + 6, 20.0, 28.0);
		}
		if (m_SkillATTK[2] >= 0)
		{
			this->GetImagePositionInImageMap(&temp_position, 2);
			g_pSkillList->RenderSkillIcon(m_SkillATTK[2], temp_position.x + 6, temp_position.y + 6, 20.0, 28.0);
		}
		if (m_SkillBuff[0] >= 0)
		{
			this->GetImagePositionInImageMap(&temp_position, 3);
			g_pSkillList->RenderSkillIcon(m_SkillBuff[0], temp_position.x + 6, temp_position.y + 6, 20.0, 28.0);
		}
		if (m_SkillBuff[1] >= 0)
		{
			this->GetImagePositionInImageMap(&temp_position, 4);
			g_pSkillList->RenderSkillIcon(m_SkillBuff[1], temp_position.x + 6, temp_position.y + 6, 20.0, 28.0);
		}
		if (m_SkillBuff[2] >= 0)
		{
			this->GetImagePositionInImageMap(&temp_position, 5);
			g_pSkillList->RenderSkillIcon(m_SkillBuff[2], temp_position.x + 6, temp_position.y + 6, 20.0, 28.0);
		}*/
	}

	if (tooltipvisible == true && g_pNewUI3DRenderMng)
	{
		g_pNewUI3DRenderMng->RenderUI2DEffect(INVENTORY_CAMERA_Z_ORDER, UI2DEffectCallback, this, 0, 0);
	}
	DisableAlphaBlend();

	return true;
}

float SEASON3B::CNewUIMacroMain::GetLayerDepth()
{
	return 3.6f;
}

void SEASON3B::CNewUIMacroMain::EventOrderWindows(double WindowsX, double WindowsY)
{
	float RenderFrameX = (GetWindowsX);

	RenderFrameX -= (WindowsX - m_Pos.x);

	for (MAPE_CHECKBOX::iterator& pair = m_checkBox.begin(); pair != m_checkBox.end(); pair++)
	{
		CCheckBox* element = pair->second;
		int x = RenderFrameX + (element->GetPos().x - m_Pos.x);
		element->SetPos(x, element->GetPos().y);
	}

	for (MAPE_BUTTON::iterator& pair = m_Buttons.begin(); pair != m_Buttons.end(); pair++)
	{
		CNewUIButton* element = pair->second;
		int x = RenderFrameX + (element->GetPos().x - m_Pos.x);
		element->SetPos(x, element->GetPos().y);
	}

	for (MAPE_TXTINPUT::iterator& pair = m_InputBox.begin(); pair != m_InputBox.end(); pair++)
	{
		CUITextInputBox* element = pair->second;
		int x = RenderFrameX + (element->GetPosition_x() - m_Pos.x);
		element->SetPosition(x, element->GetPosition_y());
	}

	for (MAPE_MACROTXT::iterator& pair = m_stMacroUIText.begin(); pair != m_stMacroUIText.end(); pair++)
	{
		stMacroUIText* element = pair->second;
		int x = RenderFrameX + (element->GetPos().x - m_Pos.x);
		element->SetPos(x, element->GetPos().y);
	}

	for (MAPE_MACROIMG::iterator& pair = m_stMacroUIImage.begin(); pair != m_stMacroUIImage.end(); pair++)
	{
		stMacroUIImage* element = pair->second;
		int x = RenderFrameX + (element->GetPos().x - m_Pos.x);
		element->SetPos(x, element->GetPos().y);
	}

	this->SetPos((int)RenderFrameX, m_Pos.y);
	this->SetPosOptionButtonGroup(RenderFrameX + 63, m_Pos.y + 233);
}

void SEASON3B::CNewUIMacroMain::RenderFrame()
{
	float RenderFrameX = m_Pos.x;
	float RenderFrameY = m_Pos.y;

	SEASON3B::window_nomal(RenderFrameX, RenderFrameY, 3);

	g_pRenderText->SetBgColor(0);
	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(0xffffffff);
	g_pRenderText->RenderText(RenderFrameX + WIN_WINDOW_SIZEX / 2.f, RenderFrameY + 15, GlobalText[3536], 0, 0, RT3_WRITE_CENTER, NULL);

	m_iTab.Render();

	g_pRenderText->SetFont(g_hFont);

#if MAIN_UPDATE > 603
	int addx = ((WIN_WINDOW_SIZEX - 168) / 2) - 12;

	RenderTable(RenderFrameX + addx + 12, RenderFrameY + 332, 170, 46);

	g_pRenderText->RenderText(RenderFrameX + addx + 20, RenderFrameY + 337, GlobalText[3537], 0, 0, RT3_SORT_LEFT, 0);
	g_pRenderText->SetTextColor(-16730881);
	g_pRenderText->RenderText(RenderFrameX + addx + 20, RenderFrameY + 355, GlobalText[3538], 0, 0, RT3_SORT_LEFT, 0);

	g_pRenderText->SetTextColor(0xffffffff);

	if (m_iNumCurOpenTab == 0)
	{
		this->RenderTable(RenderFrameX + addx + 12, RenderFrameY + 54, 68, 42);
		this->RenderTable(RenderFrameX + addx + 82, RenderFrameY + 54, 100, 42);

		this->RenderTable(RenderFrameX + addx + 12, RenderFrameY + 98, 170, 38);
		this->RenderTable(RenderFrameX + addx + 12, RenderFrameY + 138, 170, 112);
		this->RenderTable(RenderFrameX + addx + 12, RenderFrameY + 252, 170, 78);
	}
	else if (m_iNumCurOpenTab == 1)
	{
		this->RenderTable(RenderFrameX + addx + 12, RenderFrameY + 54, 68, 42);
		this->RenderTable(RenderFrameX + addx + 82, RenderFrameY + 54, 100, 42);

		this->RenderTable(RenderFrameX + addx + 12, RenderFrameY + 98, 170, 24);
		this->RenderTable(RenderFrameX + addx + 12, RenderFrameY + 124, 170, 206);
	}
	else
	{
		this->RenderTable(RenderFrameX + addx + 12, RenderFrameY + 54, 170, 42);
		this->RenderTable(RenderFrameX + addx + 12, RenderFrameY + 98, 170, 232);
	}
#else
	RenderTable(RenderFrameX + 12, RenderFrameY + 340, 166, 46);

	g_pRenderText->RenderText(RenderFrameX + 20, RenderFrameY + 347, GlobalText[3537], 0, 0, RT3_SORT_CENTER, 0);
	g_pRenderText->SetTextColor(-16730881);
	g_pRenderText->RenderText(RenderFrameX + 20, RenderFrameY + 365, GlobalText[3538], 0, 0, RT3_SORT_CENTER, 0);

	g_pRenderText->SetTextColor(255, 255, 255, 255);

	if (m_iNumCurOpenTab == 0)
	{
		this->RenderTable(RenderFrameX + 12, RenderFrameY + 73, 68, 50);
		this->RenderTable(RenderFrameX + 75, RenderFrameY + 73, 102, 50);
		this->RenderTable(RenderFrameX + 12, RenderFrameY + 120, 165, 39);
		this->RenderTable(RenderFrameX + 12, RenderFrameY + 156, 165, 120);
		this->RenderTable(RenderFrameX + 12, RenderFrameY + 273, 165, 69);
	}
	else if (m_iNumCurOpenTab == 1)
	{
		this->RenderTable(RenderFrameX + 12, RenderFrameY + 73, 68, 50);
		this->RenderTable(RenderFrameX + 75, RenderFrameY + 73, 102, 50);
		this->RenderTable(RenderFrameX + 12, RenderFrameY + 120, 165, 30);
		this->RenderTable(RenderFrameX + 12, RenderFrameY + 147, 165, 195);
	}
	else
	{
		this->RenderTable(RenderFrameX + 12, RenderFrameY + 73, 165, 50);
		this->RenderTable(RenderFrameX + 12, RenderFrameY + 120, 165, 222);
	}
#endif
}

void SEASON3B::CNewUIMacroMain::InitButtons()
{
	std::list<unicode::t_string> ltext;
	ltext.push_back(GlobalText[3500]);
	ltext.push_back(GlobalText[3501]);
	ltext.push_back(GlobalText[3590]);

	m_iTab.CreateRadioGroup(3, BITMAP_GUILDINFO_BEGIN);
	m_iTab.ChangeRadioText(ltext);

	m_iTab.ChangeFrame(m_iNumCurOpenTab);

	float RenderFrameX = m_Pos.x;
	float RenderFrameY = m_Pos.y;

#if MAIN_UPDATE > 603
	m_iTab.ChangeRadioButtonInfo(true, RenderFrameX + 18.f, RenderFrameY + 30.f, 56, 22);

	int addx = ((WIN_WINDOW_SIZEX - 168) / 2) - 12;
	RenderFrameX += addx;
	RenderFrameY -= 20.f;

	this->AddButtonMap(IMAGE_CHAINFO_BTN_STAT, RenderFrameX + 56, RenderFrameY + 78, 16, 15, false, false, false, false, "", "", button_Hunting_Max);
	this->AddButtonMap(Image_RangeMinus, RenderFrameX + 56, RenderFrameY + 97, 16, 15, false, false, false, false, "", "", button_Hunting_Min);
	this->AddButtonMap(IMAGE_MINIMAPALPBTN, RenderFrameX + 132, RenderFrameY + 191, 38, 24, true, false, true, true, GlobalText[3502], "", 2);
	this->AddButtonMap(IMAGE_MINIMAPALPBTN, RenderFrameX + 132, RenderFrameY + 243, 38, 24, true, false, true, true, GlobalText[3502], "", 3);
	this->AddButtonMap(IMAGE_MINIMAPALPBTN, RenderFrameX + 132, RenderFrameY + 84, 38, 24, true, false, true, true, GlobalText[3502], "", 4);
	this->AddButtonMap(IMAGE_MINIMAPALPBTN, RenderFrameX + 132, RenderFrameY + 76, 38, 24, true, false, true, true, GlobalText[3502], "", 5);
	this->AddButtonMap(IMAGE_MINIMAPALPBTN, RenderFrameX + 132, RenderFrameY + 84, 38, 24, true, false, true, true, GlobalText[3502], "", 6);
	this->AddButtonMap(IMAGE_MINIMAPALPBTN, RenderFrameX + 17, RenderFrameY + 234, 38, 24, true, false, true, true, GlobalText[3502], "", 8);
	this->AddButtonMap(IMAGE_MINIMAPALPBTN, RenderFrameX + 17, RenderFrameY + 234, 38, 24, true, false, true, true, GlobalText[3502], "", 7);
	this->AddButtonMap(IMAGE_CHAINFO_BTN_STAT, RenderFrameX + 56, RenderFrameY + 78, 16, 15, false, false, false, false, "", "", button_PickUp_Max);
	this->AddButtonMap(Image_RangeMinus, RenderFrameX + 56, RenderFrameY + 97, 16, 15, false, false, false, false, "", "", button_PickUp_Min);
	this->AddButtonMap(IMAGE_MINIMAPALPBTN, RenderFrameX + 132, RenderFrameY + 214, 38, 24, true, false, true, true, GlobalText[3505], "", button_AddText);
	this->AddButtonMap(IMAGE_MINIMAPALPBTN, RenderFrameX + 132, RenderFrameY + 316, 38, 24, true, false, true, true, GlobalText[3506], "", button_DelText);

	RenderFrameX = m_Pos.x;
	RenderFrameY = m_Pos.y;
	this->AddButtonMap(IMAGE_MACRO_MAIN_EXIT_BTN, RenderFrameX + 22, RenderFrameY + 384, 20, 20, false, false, false, false, "", GlobalText[229], button_Exit);  //-- Exit
	this->AddButtonMap(IMAGE_IGS_VIEWDETAIL_BTN, RenderFrameX + 120, RenderFrameY + 382, 52, 26, true, false, true, true, GlobalText[3503], "", button_Save); //-- Save
	this->AddButtonMap(IMAGE_IGS_VIEWDETAIL_BTN, RenderFrameX + 65, RenderFrameY + 382, 52, 26, true, false, true, true, GlobalText[3504], "", button_Initialize);  //-- Inicialize

#else
	m_iTab.ChangeRadioButtonInfo(true, RenderFrameX + 10.f, RenderFrameY + 48.f, 56, 22);

	this->AddButtonMap(IMAGE_CHAINFO_BTN_STAT, RenderFrameX + 56, RenderFrameY + 78, 16, 15, false, false, false, false, "", "", button_Hunting_Max);
	this->AddButtonMap(Image_RangeMinus, RenderFrameX + 56, RenderFrameY + 97, 16, 15, false, false, false, false, "", "", button_Hunting_Min);
	this->AddButtonMap(IMAGE_MINIMAPALPBTN, RenderFrameX + 132, RenderFrameY + 191, 38, 24, true, false, true, true, GlobalText[3502], "", 2);
	this->AddButtonMap(IMAGE_MINIMAPALPBTN, RenderFrameX + 132, RenderFrameY + 243, 38, 24, true, false, true, true, GlobalText[3502], "", 3);
	this->AddButtonMap(IMAGE_IGS_VIEWDETAIL_BTN, RenderFrameX + 120, RenderFrameY + 388, 52, 26, true, false, true, true, GlobalText[3503], "", button_Save); //-- Save
	this->AddButtonMap(IMAGE_IGS_VIEWDETAIL_BTN, RenderFrameX + 65, RenderFrameY + 388, 52, 26, true, false, true, true, GlobalText[3504], "", button_Initialize);  //-- Inicialize
	this->AddButtonMap(IMAGE_MACRO_MAIN_EXIT_BTN, RenderFrameX + 20, RenderFrameY + 388, 36, 29, false, false, false, false, "", GlobalText[229], button_Exit);  //-- Exit
	this->AddButtonMap(IMAGE_MINIMAPALPBTN, RenderFrameX + 132, RenderFrameY + 84, 38, 24, true, false, true, true, GlobalText[3502], "", 4);
	this->AddButtonMap(IMAGE_MINIMAPALPBTN, RenderFrameX + 132, RenderFrameY + 76, 38, 24, true, false, true, true, GlobalText[3502], "", 5);
	this->AddButtonMap(IMAGE_MINIMAPALPBTN, RenderFrameX + 132, RenderFrameY + 84, 38, 24, true, false, true, true, GlobalText[3502], "", 6);
	this->AddButtonMap(IMAGE_MINIMAPALPBTN, RenderFrameX + 17, RenderFrameY + 234, 38, 24, true, false, true, true, GlobalText[3502], "", 8);
	this->AddButtonMap(IMAGE_MINIMAPALPBTN, RenderFrameX + 17, RenderFrameY + 234, 38, 24, true, false, true, true, GlobalText[3502], "", 7);
	this->AddButtonMap(IMAGE_CHAINFO_BTN_STAT, RenderFrameX + 56, RenderFrameY + 78, 16, 15, false, false, false, false, "", "", button_PickUp_Max);
	this->AddButtonMap(Image_RangeMinus, RenderFrameX + 56, RenderFrameY + 97, 16, 15, false, false, false, false, "", "", button_PickUp_Min);
	this->AddButtonMap(IMAGE_MINIMAPALPBTN, RenderFrameX + 132, RenderFrameY + 214, 38, 24, true, false, true, true, GlobalText[3505], "", button_AddText);
	this->AddButtonMap(IMAGE_MINIMAPALPBTN, RenderFrameX + 132, RenderFrameY + 316, 38, 24, true, false, true, true, GlobalText[3506], "", button_DelText);
#endif

	this->AddButtonMapByClass(-1, button_Hunting_Max);
	this->AddButtonMapByClass(-1, button_Hunting_Min);
	this->AddButtonMapByClass(-1, 2);
	this->AddButtonMapByClass(-1, button_PickUp_Max);
	this->AddButtonMapByClass(-1, button_PickUp_Min);
	this->AddButtonMapByClass(-1, button_AddText);
	this->AddButtonMapByClass(-1, button_DelText);
	this->AddButtonMapByClass(-1, button_Save);
	this->AddButtonMapByClass(-1, button_Initialize);
	this->AddButtonMapByClass(-1, button_Exit);

	this->AddButtonMapByClass(-1, 13);
	this->AddButtonMapByClass(-1, 14);
	this->AddButtonMapByClass(-1, 15);
	this->AddButtonMapByClass(Dark_Knight, 3);
	this->AddButtonMapByClass(Dark_Knight, 6);
	this->AddButtonMapByClass(Dark_Wizard, 3);
	this->AddButtonMapByClass(Dark_Wizard, 6);
	this->AddButtonMapByClass(Dark_Wizard, 7);
	this->AddButtonMapByClass(Magic_Gladiator, 3);
	this->AddButtonMapByClass(Magic_Gladiator, 6);
	this->AddButtonMapByClass(Dark_Lord, 6);
	this->AddButtonMapByClass(Rage_Fighter, 3);
	this->AddButtonMapByClass(Rage_Fighter, 6);
	this->AddButtonMapByClass(Fairy_Elf, 3);
	this->AddButtonMapByClass(Fairy_Elf, 4);
	this->AddButtonMapByClass(Fairy_Elf, 8);
	this->AddButtonMapByClass(Summoner, 3);
	this->AddButtonMapByClass(Summoner, 5);
}

void SEASON3B::CNewUIMacroMain::InitCheckBoxs()
{
	float RenderFrameX = m_Pos.x;
	float RenderFrameY = m_Pos.y;

#if MAIN_UPDATE > 603
	int addx = ((WIN_WINDOW_SIZEX - 168) / 2) - 12;
	RenderFrameX += addx;
	RenderFrameY -= 20.f;

	this->AddCheckBoxMap(BITMAP_check_box, RenderFrameX + addx + 79, RenderFrameY + 80, 15, 15, GlobalText[3507], checkbox_AutoPotion);
	this->AddCheckBoxMap(BITMAP_check_box, RenderFrameX + addx + 79, RenderFrameY + 97, 15, 15, GlobalText[3516], checkbox_AutoHeal);
	this->AddCheckBoxMap(BITMAP_check_box, RenderFrameX + addx + 79, RenderFrameY + 97, 15, 15, GlobalText[3517], checkbox_DrainLife);

	this->AddCheckBoxMap(BITMAP_check_box, RenderFrameX + 17, RenderFrameY + 122, 15, 15, GlobalText[3508], checkbox_LongDistance);
	this->AddCheckBoxMap(BITMAP_check_box, RenderFrameX + 17, RenderFrameY + 139, 15, 15, GlobalText[3509], checkbox_ReturnOriginal);

	this->AddCheckBoxMap(BITMAP_check_box, RenderFrameX + 98, RenderFrameY + 174, 15, 15, GlobalText[3510], checkbox_DelayActiveSkill2);
	this->AddCheckBoxMap(BITMAP_check_box, RenderFrameX + 98, RenderFrameY + 191, 15, 15, GlobalText[3511], checkbox_ConAtiveSkill2);
	this->AddCheckBoxMap(BITMAP_check_box, RenderFrameX + 98, RenderFrameY + 226, 15, 15, GlobalText[3510], checkbox_DelayActiveSkill3);
	this->AddCheckBoxMap(BITMAP_check_box, RenderFrameX + 98, RenderFrameY + 243, 15, 15, GlobalText[3511], checkbox_ConAtiveSkill3);
	this->AddCheckBoxMap(BITMAP_check_box, RenderFrameX + 17, RenderFrameY + 226, 15, 15, GlobalText[3512], checkbox_ComboATK);
	this->AddCheckBoxMap(BITMAP_check_box, RenderFrameX + 60, RenderFrameY + 218, 15, 15, GlobalText[3514], checkbox_Use_DarkSpirit);
	this->AddCheckBoxMap(BITMAP_check_box, RenderFrameX + 17, RenderFrameY + 218, 15, 15, GlobalText[3515], checkbox_PartyBuffDuration);

	this->AddCheckBoxMap(BITMAP_check_box, RenderFrameX + 25, RenderFrameY + 276, 15, 15, GlobalText[3513], checkbox_BuffDuration);


	this->AddCheckBoxMap(BITMAP_check_box, RenderFrameX + addx + 79, RenderFrameY + 80, 15, 15, GlobalText[3518], checkbox_RepairItem);

	this->AddCheckBoxMap(BITMAP_check_box, RenderFrameX + 17, RenderFrameY + 122, 15, 15, GlobalText[3519], checkbox_Pick_NearItems);
	this->AddCheckBoxMap(BITMAP_check_box, RenderFrameX + 17, RenderFrameY + 152, 15, 15, GlobalText[3520], checkbox_Pick_SelectItems);

	this->AddCheckBoxMap(BITMAP_check_box, RenderFrameX + 22, RenderFrameY + 170, 15, 15, GlobalText[3521], checkbox_Pick_JewelGem);
	this->AddCheckBoxMap(BITMAP_check_box, RenderFrameX + 85, RenderFrameY + 170, 15, 15, GlobalText[3522], checkbox_Pick_SetItem);
	this->AddCheckBoxMap(BITMAP_check_box, RenderFrameX + 22, RenderFrameY + 187, 15, 15, GlobalText[3523], checkbox_Pick_Zen);
	this->AddCheckBoxMap(BITMAP_check_box, RenderFrameX + 85, RenderFrameY + 187, 15, 15, GlobalText[3524], checkbox_Pick_Excellent);
	this->AddCheckBoxMap(BITMAP_check_box, RenderFrameX + 22, RenderFrameY + 204, 15, 15, GlobalText[3525], checkbox_Pick_AddExtra);

	this->AddCheckBoxMap(BITMAP_check_box, RenderFrameX + 17, RenderFrameY + 80, 15, 15, GlobalText[3591], checkbox_PartyAutoRequestFriend);
	this->AddCheckBoxMap(BITMAP_check_box, RenderFrameX + 17, RenderFrameY + 97, 15, 15, GlobalText[3592], checkbox_UseElitePotion);

	this->AddCheckBoxMap(BITMAP_check_box, RenderFrameX + 17, RenderFrameY + 129, 15, 15, GlobalText[3594], checkbox_PartyAutoRequestGuild);
#else
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 79, RenderFrameY + 80, 15, 15, GlobalText[3507], checkbox_AutoPotion);
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 18, RenderFrameY + 122, 15, 15, GlobalText[3508], checkbox_LongDistance);
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 18, RenderFrameY + 137, 15, 15, GlobalText[3509], checkbox_ReturnOriginal);
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 94, RenderFrameY + 174, 15, 15, GlobalText[3510], checkbox_DelayActiveSkill2);
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 94, RenderFrameY + 191, 15, 15, GlobalText[3511], checkbox_ConAtiveSkill2);
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 94, RenderFrameY + 226, 15, 15, GlobalText[3510], checkbox_DelayActiveSkill3);
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 94, RenderFrameY + 243, 15, 15, GlobalText[3511], checkbox_ConAtiveSkill3);
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 18, RenderFrameY + 226, 15, 15, GlobalText[3512], checkbox_ComboATK);
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 25, RenderFrameY + 276, 15, 15, GlobalText[3513], checkbox_BuffDuration);
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 60, RenderFrameY + 218, 15, 15, GlobalText[3514], checkbox_Use_DarkSpirit);
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 15, RenderFrameY + 218, 15, 15, GlobalText[3515], checkbox_PartyBuffDuration);
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 79, RenderFrameY + 97, 15, 15, GlobalText[3516], checkbox_AutoHeal);
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 79, RenderFrameY + 97, 15, 15, GlobalText[3517], checkbox_DrainLife);
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 79, RenderFrameY + 80, 15, 15, GlobalText[3518], checkbox_RepairItem);
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 17, RenderFrameY + 125, 15, 15, GlobalText[3519], checkbox_Pick_NearItems);
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 17, RenderFrameY + 152, 15, 15, GlobalText[3520], checkbox_Pick_SelectItems);
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 22, RenderFrameY + 170, 15, 15, GlobalText[3521], checkbox_Pick_JewelGem);
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 85, RenderFrameY + 170, 15, 15, GlobalText[3522], checkbox_Pick_SetItem);
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 22, RenderFrameY + 187, 15, 15, GlobalText[3523], checkbox_Pick_Zen);
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 85, RenderFrameY + 187, 15, 15, GlobalText[3524], checkbox_Pick_Excellent);
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 22, RenderFrameY + 204, 15, 15, GlobalText[3525], checkbox_Pick_AddExtra);
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 18, RenderFrameY + 80, 15, 15, GlobalText[3591], checkbox_PartyAutoRequestFriend);
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 18, RenderFrameY + 97, 15, 15, GlobalText[3592], checkbox_UseElitePotion);
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 18, RenderFrameY + 125, 15, 15, GlobalText[3594], checkbox_PartyAutoRequestGuild);

#if MAIN_UPDATE < 603
	this->AddCheckBoxMap(IMAGE_MACRO_MAIN_CHECK, RenderFrameX + 18, RenderFrameY + 114, 15, 15, "follow party", checkbox_follow_party_member);
#endif // MAIN_UPDATE < 603
#endif

	this->AddCheckBoxMapByClass(-1, checkbox_AutoPotion);
	this->AddCheckBoxMapByClass(-1, checkbox_LongDistance);
	this->AddCheckBoxMapByClass(-1, checkbox_ReturnOriginal);
	this->AddCheckBoxMapByClass(-1, checkbox_DelayActiveSkill2);
	this->AddCheckBoxMapByClass(-1, checkbox_ConAtiveSkill2);
	this->AddCheckBoxMapByClass(-1, checkbox_BuffDuration);
	this->AddCheckBoxMapByClass(-1, checkbox_RepairItem);
	this->AddCheckBoxMapByClass(-1, checkbox_Pick_NearItems);
	this->AddCheckBoxMapByClass(-1, checkbox_Pick_SelectItems);
	this->AddCheckBoxMapByClass(-1, checkbox_Pick_JewelGem);
	this->AddCheckBoxMapByClass(-1, checkbox_Pick_SetItem);
	this->AddCheckBoxMapByClass(-1, checkbox_Pick_Zen);
	this->AddCheckBoxMapByClass(-1, checkbox_Pick_Excellent);
	this->AddCheckBoxMapByClass(-1, checkbox_Pick_AddExtra);
	this->AddCheckBoxMapByClass(-1, checkbox_PartyAutoRequestFriend);
	this->AddCheckBoxMapByClass(-1, checkbox_UseElitePotion);
	this->AddCheckBoxMapByClass(-1, checkbox_PartyAutoRequestGuild);

#if MAIN_UPDATE < 603
	this->AddCheckBoxMapByClass(-1, checkbox_follow_party_member);
#endif // MAIN_UPDATE < 603

	this->AddCheckBoxMapByClass(Dark_Knight, checkbox_DelayActiveSkill3);
	this->AddCheckBoxMapByClass(Dark_Knight, checkbox_ConAtiveSkill3);
	this->AddCheckBoxMapByClass(Dark_Knight, checkbox_ComboATK);
	this->AddCheckBoxMapByClass(Dark_Wizard, checkbox_DelayActiveSkill3);
	this->AddCheckBoxMapByClass(Dark_Wizard, checkbox_ConAtiveSkill3);
	this->AddCheckBoxMapByClass(Dark_Wizard, checkbox_PartyBuffDuration);
	this->AddCheckBoxMapByClass(Magic_Gladiator, checkbox_DelayActiveSkill3);
	this->AddCheckBoxMapByClass(Magic_Gladiator, checkbox_ConAtiveSkill3);
	this->AddCheckBoxMapByClass(Dark_Lord, checkbox_Use_DarkSpirit);
	this->AddCheckBoxMapByClass(Fairy_Elf, checkbox_AutoHeal);
	this->AddCheckBoxMapByClass(Fairy_Elf, checkbox_DelayActiveSkill3);
	this->AddCheckBoxMapByClass(Fairy_Elf, checkbox_ConAtiveSkill3);
	this->AddCheckBoxMapByClass(Fairy_Elf, checkbox_PartyBuffDuration);
	this->AddCheckBoxMapByClass(Summoner, checkbox_DelayActiveSkill3);
	this->AddCheckBoxMapByClass(Summoner, checkbox_ConAtiveSkill3);
	this->AddCheckBoxMapByClass(Summoner, checkbox_DrainLife);
	this->AddCheckBoxMapByClass(Rage_Fighter, checkbox_DelayActiveSkill3);
	this->AddCheckBoxMapByClass(Rage_Fighter, checkbox_ConAtiveSkill3);
}

void SEASON3B::CNewUIMacroMain::InitInputBoxs()
{
	float RenderFrameX = m_Pos.x;
	float RenderFrameY = m_Pos.y;

#if MAIN_UPDATE > 603
	int addx = ((WIN_WINDOW_SIZEX - 168) / 2) - 12;
	RenderFrameX += addx;
	RenderFrameY -= 23.f;
#endif

	this->AddTextInputMap(RenderFrameX + 142, RenderFrameY + 141, 30, 14, 3, 1, "", 0);
	this->AddTextInputMap(RenderFrameX + 142, RenderFrameY + 178, 30, 14, 3, 1, "", 1);
	this->AddTextInputMap(RenderFrameX + 142, RenderFrameY + 230, 30, 14, 3, 1, "", 2);
	this->AddTextInputMap(RenderFrameX + 40, RenderFrameY + 219, 100, 14, 16, 0, "", 3);

	this->AddTextInputMapByClass(-1, 0);
	this->AddTextInputMapByClass(-1, 1);
	this->AddTextInputMapByClass(-1, 3);
	this->AddTextInputMapByClass(Dark_Knight, 2);
	this->AddTextInputMapByClass(Dark_Wizard, 2);
	this->AddTextInputMapByClass(Fairy_Elf, 2);
	this->AddTextInputMapByClass(Magic_Gladiator, 2);
	this->AddTextInputMapByClass(Summoner, 2);
	this->AddTextInputMapByClass(Rage_Fighter, 2);
}

void SEASON3B::CNewUIMacroMain::InitImages()
{
	float RenderFrameX = m_Pos.x;
	float RenderFrameY = m_Pos.y;

#if MAIN_UPDATE > 603
	int addx = ((WIN_WINDOW_SIZEX - 168) / 2) - 12;
	RenderFrameX += addx;
	RenderFrameY -= 20.f;
#endif

	this->AddImageMap(IMAGE_MVP_NUMBER, RenderFrameX + 29, RenderFrameY + 92, 15.0, 19.0, 10);
	this->AddImageMap(IMAGE_MVP_NUMBER + 1, RenderFrameX + 29, RenderFrameY + 92, 15.0, 19.0, 11);
	this->AddImageMap(IMAGE_MVP_NUMBER + 2, RenderFrameX + 29, RenderFrameY + 92, 15.0, 19.0, 12);
	this->AddImageMap(IMAGE_MVP_NUMBER + 3, RenderFrameX + 29, RenderFrameY + 92, 15.0, 19.0, 13);
	this->AddImageMap(IMAGE_MVP_NUMBER + 4, RenderFrameX + 29, RenderFrameY + 92, 15.0, 19.0, 14);
	this->AddImageMap(IMAGE_MVP_NUMBER + 5, RenderFrameX + 29, RenderFrameY + 92, 15.0, 19.0, 15);
	this->AddImageMap(IMAGE_MVP_NUMBER + 6, RenderFrameX + 29, RenderFrameY + 92, 15.0, 19.0, 16);
	this->AddImageMap(IMAGE_MVP_NUMBER + 7, RenderFrameX + 29, RenderFrameY + 92, 15.0, 19.0, 17);

	this->AddImageMap(IMAGE_SKILLBOX, RenderFrameX + 17, RenderFrameY + 171, 32.0, 38.0, 0);
	this->AddImageMap(IMAGE_SKILLBOX, RenderFrameX + 61, RenderFrameY + 171, 32.0, 38.0, 1);
	this->AddImageMap(IMAGE_SKILLBOX, RenderFrameX + 61, RenderFrameY + 222, 32.0, 38.0, 2);
	this->AddImageMap(IMAGE_SKILLBOX, RenderFrameX + 21, RenderFrameY + 293, 32.0, 38.0, 3);
	this->AddImageMap(IMAGE_SKILLBOX, RenderFrameX + 55, RenderFrameY + 293, 32.0, 38.0, 4);
	this->AddImageMap(IMAGE_SKILLBOX, RenderFrameX + 89, RenderFrameY + 293, 32.0, 38.0, 5);

	this->AddImageMap(IMAGE_MVP_NUMBER, RenderFrameX + 29, RenderFrameY + 92, 15.0, 19.0, 18);
	this->AddImageMap(IMAGE_MVP_NUMBER + 1, RenderFrameX + 29, RenderFrameY + 92, 15.0, 19.0, 19);
	this->AddImageMap(IMAGE_MVP_NUMBER + 2, RenderFrameX + 29, RenderFrameY + 92, 15.0, 19.0, 20);
	this->AddImageMap(IMAGE_MVP_NUMBER + 3, RenderFrameX + 29, RenderFrameY + 92, 15.0, 19.0, 21);
	this->AddImageMap(IMAGE_MVP_NUMBER + 4, RenderFrameX + 29, RenderFrameY + 92, 15.0, 19.0, 22);
	this->AddImageMap(IMAGE_MVP_NUMBER + 5, RenderFrameX + 29, RenderFrameY + 92, 15.0, 19.0, 23);
	this->AddImageMap(IMAGE_MVP_NUMBER + 6, RenderFrameX + 29, RenderFrameY + 92, 15.0, 19.0, 24);
	this->AddImageMap(IMAGE_MVP_NUMBER + 7, RenderFrameX + 29, RenderFrameY + 92, 15.0, 19.0, 25);

	this->AddImageMap(Image_InputNumber, RenderFrameX + 135, RenderFrameY + 137, 28.0, 15.0, 6);
	this->AddImageMap(Image_InputNumber, RenderFrameX + 135, RenderFrameY + 174, 28.0, 15.0, 7);
	this->AddImageMap(Image_InputNumber, RenderFrameX + 135, RenderFrameY + 226, 28.0, 15.0, 8);
	this->AddImageMap(Image_InputString, RenderFrameX + 34, RenderFrameY + 216, 93.0, 15.0, 9);

	this->AddImageMapByClass(-1, 10);
	this->AddImageMapByClass(-1, 11);
	this->AddImageMapByClass(-1, 12);
	this->AddImageMapByClass(-1, 13);
	this->AddImageMapByClass(-1, 14);
	this->AddImageMapByClass(-1, 15);
	this->AddImageMapByClass(-1, 16);
	this->AddImageMapByClass(-1, 17);
	this->AddImageMapByClass(-1, 0);
	this->AddImageMapByClass(-1, 1);
	this->AddImageMapByClass(-1, 3);
	this->AddImageMapByClass(-1, 4);
	this->AddImageMapByClass(-1, 5);
	this->AddImageMapByClass(-1, 18);
	this->AddImageMapByClass(-1, 19);
	this->AddImageMapByClass(-1, 20);
	this->AddImageMapByClass(-1, 21);
	this->AddImageMapByClass(-1, 22);
	this->AddImageMapByClass(-1, 23);
	this->AddImageMapByClass(-1, 24);
	this->AddImageMapByClass(-1, 25);
	this->AddImageMapByClass(-1, 6);
	this->AddImageMapByClass(-1, 7);
	this->AddImageMapByClass(-1, 9);
	//--
	this->AddImageMapByClass(Dark_Knight, 2);
	this->AddImageMapByClass(Dark_Knight, 8);
	this->AddImageMapByClass(Dark_Wizard, 2);
	this->AddImageMapByClass(Dark_Wizard, 8);
	this->AddImageMapByClass(Fairy_Elf, 2);
	this->AddImageMapByClass(Fairy_Elf, 8);
	this->AddImageMapByClass(Magic_Gladiator, 2);
	this->AddImageMapByClass(Magic_Gladiator, 8);
	this->AddImageMapByClass(Summoner, 2);
	this->AddImageMapByClass(Summoner, 8);
	this->AddImageMapByClass(Rage_Fighter, 2);
	this->AddImageMapByClass(Rage_Fighter, 8);
}

void SEASON3B::CNewUIMacroMain::InitTexts()
{
	float RenderFrameX = m_Pos.x;
	float RenderFrameY = m_Pos.y;

#if MAIN_UPDATE > 603
	int addx = ((WIN_WINDOW_SIZEX - 168) / 2) - 12;
	RenderFrameX += addx;
	RenderFrameY -= 20.f;
#endif

	this->AddTextMap(RenderFrameX + 18, RenderFrameY + 78, GlobalText[3526], 1);
	this->AddTextMap(RenderFrameX + 18, RenderFrameY + 83, "________", 2);
	this->AddTextMap(RenderFrameX + 113, RenderFrameY + 141, GlobalText[3527], 3);
	this->AddTextMap(RenderFrameX + 162, RenderFrameY + 141, GlobalText[3528], 4);
	this->AddTextMap(RenderFrameX + 18, RenderFrameY + 160, GlobalText[3529], 5);
	this->AddTextMap(RenderFrameX + 59, RenderFrameY + 160, GlobalText[3530], 7);
	this->AddTextMap(RenderFrameX + 162, RenderFrameY + 178, GlobalText[3528], 8);
	this->AddTextMap(RenderFrameX + 59, RenderFrameY + 212, GlobalText[3531], 9);
	this->AddTextMap(RenderFrameX + 162, RenderFrameY + 230, GlobalText[3528], 10);

	this->AddTextMap(RenderFrameX + 18, RenderFrameY + 78, GlobalText[3532], 11);
	this->AddTextMap(RenderFrameX + 18, RenderFrameY + 83, "________", 12);

	this->AddTextMapByClass(-1, 1);
	this->AddTextMapByClass(-1, 2);
	this->AddTextMapByClass(-1, 3);
	this->AddTextMapByClass(-1, 4);
	this->AddTextMapByClass(-1, 5);
	this->AddTextMapByClass(-1, 7);
	this->AddTextMapByClass(-1, 8);
	this->AddTextMapByClass(-1, 11);
	this->AddTextMapByClass(-1, 12);
	this->AddTextMapByClass(Dark_Knight, 9);
	this->AddTextMapByClass(Dark_Knight, 10);
	this->AddTextMapByClass(Dark_Wizard, 9);
	this->AddTextMapByClass(Dark_Wizard, 10);
	this->AddTextMapByClass(Fairy_Elf, 9);
	this->AddTextMapByClass(Fairy_Elf, 10);
	this->AddTextMapByClass(Magic_Gladiator, 9);
	this->AddTextMapByClass(Magic_Gladiator, 10);
	this->AddTextMapByClass(Summoner, 9);
	this->AddTextMapByClass(Summoner, 10);
	this->AddTextMapByClass(Rage_Fighter, 9);
	this->AddTextMapByClass(Rage_Fighter, 10);
}

void SEASON3B::CNewUIMacroMain::InitOptionButtonGroup()
{
	float RenderFrameX = m_Pos.x;
	float RenderFrameY = m_Pos.y;

#if MAIN_UPDATE > 603
	int addx = ((WIN_WINDOW_SIZEX - 168) / 2) - 12;
	RenderFrameX += addx;
	RenderFrameY -= 20.f;
#endif

	COptionButtonGroup* optionDark = new COptionButtonGroup;

#if MAIN_UPDATE > 603
	optionDark->Create(BITMAP_check_box, RenderFrameX + 63, RenderFrameY + 233, 15, 15, 3);
#else
	optionDark->Create(Image_OptionButton, RenderFrameX + 63, RenderFrameY + 233, 15, 15, 3);
#endif
	optionDark->SetOptionText(0, 52, 0, GlobalText[3533]);
	optionDark->SetOptionText(1, 0, 0, GlobalText[3534]);
	optionDark->SetOptionText(2, 0, 17, GlobalText[3535]);

	AddOptionButtonMapInGroup(optionDark, 0);
	AddOptionButtonGroupMapByClass(Dark_Lord, 0);
}

void SEASON3B::CNewUIMacroMain::RenderTable(float x, float y, float width, float height)
{

#if MAIN_UPDATE > 603
	EnableAlphaTest();
	RenderColor(x, y, width, (height), 0.70000001, 1);
	EndRenderColor();
#else
	EnableAlphaTest();
	RenderColor((x + 3), (y + 2), (width - 7), (height - 7), 0.40000001, 1);
	EndRenderColor();

	RenderImage(IMAGE_MACRO_MAIN_TABLE_TOP_LEFT, x, y, 14.0, 14.0);

	RenderImage(IMAGE_MACRO_MAIN_TABLE_TOP_RIGHT, (x + width - 14), y, 14.0, 14.0);

	RenderImage(IMAGE_MACRO_MAIN_TABLE_BOTTOM_LEFT, x, (y + height - 14), 14.0, 14.0);

	RenderImage(IMAGE_MACRO_MAIN_TABLE_BOTTOM_RIGHT, (x + width - 14), (y + height - 14), 14.0, 14.0);

	RenderImage(IMAGE_MACRO_MAIN_TABLE_TOP_PIXEL, (x + 6), y, (width - 12), 14.0);

	RenderImage(IMAGE_MACRO_MAIN_TABLE_RIGHT_PIXEL, (x + width - 14), (y + 6), 14.0, (height - 14));

	RenderImage(IMAGE_MACRO_MAIN_TABLE_BOTTOM_PIXEL, (x + 6), (y + height - 14), (width - 12), 14.0);

	RenderImage(IMAGE_MACRO_MAIN_TABLE_LEFT_PIXEL, x, (y + 6), 14.0, (height - 14));
#endif
}

void SEASON3B::CNewUIMacroMain::LoadImages()
{
	LoadBitmap("Interface\\MacroUI\\MacroUI_RangeMinus.tga", Image_RangeMinus, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);
	LoadBitmap("Interface\\MacroUI\\MacroUI_OptionButton.tga", Image_OptionButton, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);
	LoadBitmap("Interface\\MacroUI\\MacroUI_InputNumber.tga", Image_InputNumber, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);
	LoadBitmap("Interface\\MacroUI\\MacroUI_InputString.tga", Image_InputString, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);

#if MAIN_UPDATE > 603
	LoadBitmap("Interface\\HUD_TEST\\texture_check_box.dds", BITMAP_check_box, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);
	LoadBitmap("Interface\\HUD_TEST\\texture_skill_box.dds", BITMAP_skill_box, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);
	LoadBitmap("Interface\\HUD_TEST\\newui_bt_min_max.dds", BITMAP_bt_min_max, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);
	LoadBitmap("Interface\\HUD_TEST\\ingame_bt01.dds", BITMAP_ingame_bt01, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);
	LoadBitmap("Interface\\HUD_TEST\\newui_bt_clearness_illusion.dds", BITMAP_bt_clearness_illusion, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);
#endif
}

void SEASON3B::CNewUIMacroMain::UnloadImages()
{
	DeleteBitmap(Image_RangeMinus);
	DeleteBitmap(Image_OptionButton);
	DeleteBitmap(Image_InputNumber);
	DeleteBitmap(Image_InputString);
}

void SEASON3B::CNewUIMacroMain::OpenningProcess()
{
	m_iCurrentClass = gCharacterManager.GetBaseClass(Hero->Class);

	m_iTab.ChangeFrame(0);

	m_iNumCurOpenTab = 0;

	this->LoadConfig();

	this->LoadSkillList(1);

	this->LoadSkillList(2);
}

void SEASON3B::CNewUIMacroMain::ClosingProcess()
{
	this->CloseSkillList();

	m_stSkillList[0].clear();

	m_stSkillList[1].clear();
}

void SEASON3B::CNewUIMacroMain::LoadSkillList(int sType)
{
	int width = 32;
	int height = 38;
	int positionX = m_Pos.x;
	int positionY = 200;
	float OriginY = (double)200;
	int Magic_Icon = 0;
	int index = 0;
	int currentX = 0;
	int currentY = 0;

	stSkillList* info;

	for (int i = 0; i < MAX_MAGIC; ++i)
	{
		Magic_Icon = CharacterAttribute->Skill[i];

		if (Magic_Icon && (Magic_Icon < 67 || Magic_Icon > 72))
		{
			int SkillType = SkillAttribute[Magic_Icon].SkillUseType;

			if (SkillType != 1 && SkillType != 3 && IsCanBCSkill(Magic_Icon))
			{

				if (sType == 1)
				{
					if (SkillAttribute[Magic_Icon].TypeSkill && SkillAttribute[Magic_Icon].TypeSkill != 2)
						continue;
				}
				else if (sType == 2 && SkillAttribute[Magic_Icon].TypeSkill != 1)
				{
					continue;
				}

				int rand1 = index % 8;
				int rand2 = index / 8;

				if (!(index % 8))
					positionX -= width;

				if (rand2)
				{
					positionY = (signed int)((double)(height * (rand1 - 4)) + OriginY);
				}
				else
				{
					int v4 = index / 2;
					if (index % 2)
						positionY = (signed int)(OriginY - (float)(height * (v4 + 1)));
					else
						positionY = (signed int)((float)(height * v4) + OriginY);
				}
				++index;

				info = new stSkillList;
				info->iIndex = i;
				info->iX = positionX;
				info->iY = positionY;

				if (currentX || currentY)
				{
					if (currentX > info->iX)
						currentX = info->iX;
					if (currentY > info->iY)
						currentY = info->iY;
				}
				else
				{
					currentX = info->iX;
					currentY = info->iY;
				}
				if (sType == 1)
				{
					m_stSkillList[0].push_back(info);
					m_stSkillListPos[0].x = currentX;
					m_stSkillListPos[0].y = currentY;
				}
				else if (sType == 2)
				{
					m_stSkillList[1].push_back(info);
					m_stSkillListPos[1].x = currentX;
					m_stSkillListPos[1].y = currentY;
				}
			}
		}
	}
}

void SEASON3B::CNewUIMacroMain::SetPosButtons(int x, int y, BYTE Number)
{
	MAPE_BUTTON::iterator it = m_buttonGroup[m_iCurrentClass].find(Number);

	if (it != m_buttonGroup[m_iCurrentClass].end())
	{
		it->second->SetPos(x, y);
	}
}

void SEASON3B::CNewUIMacroMain::SetPosCheckBoxs(int x, int y, BYTE Number)
{
	MAPE_CHECKBOX::iterator it = m_checkBoxGroup[m_iCurrentClass].find(Number);

	if (it != m_checkBoxGroup[m_iCurrentClass].end())
	{
		it->second->SetPos(x, y);
	}
}

void SEASON3B::CNewUIMacroMain::SetPosInputBoxs(int x, int y, BYTE Number)
{
	MAPE_TXTINPUT::iterator it = m_InputBoxGroup[m_iCurrentClass].find(Number);

	if (it != m_InputBoxGroup[m_iCurrentClass].end())
	{
		it->second->SetPosition(x, y);
	}
}

void SEASON3B::CNewUIMacroMain::SetPosImages(int x, int y, BYTE Number)
{
	MAPE_MACROIMG::iterator it = m_MacroUIImageGroup[m_iCurrentClass].find(Number);

	if (it != m_MacroUIImageGroup[m_iCurrentClass].end())
	{
		it->second->SetPos(x, y);
	}
}

void SEASON3B::CNewUIMacroMain::SetPosTexts(int x, int y, BYTE Number)
{
	MAPE_MACROTXT::iterator it = m_MacroUITextGroup[m_iCurrentClass].find(Number);

	if (it != m_MacroUITextGroup[m_iCurrentClass].end())
	{
		it->second->SetPos(x, y);
	}
}

void SEASON3B::CNewUIMacroMain::SetPosOptionButtonGroup(int x, int y)
{
	MAPE_BTNGROUP::iterator it = m_RadioOptionGroup[m_iCurrentClass].find(0);

	if (it != m_RadioOptionGroup[m_iCurrentClass].end())
	{
		COptionButtonGroup* optionDark = it->second;

		optionDark->SetPos(x, y);
		optionDark->SetOptionText(0, 52, 0, GlobalText[3533]);
		optionDark->SetOptionText(1, 0, 0, GlobalText[3534]);
		optionDark->SetOptionText(2, 0, 17, GlobalText[3535]);
	}
}

int SEASON3B::CNewUIMacroMain::GetSkillIndex(int sType)
{
	int SkillIndex = 0;

	switch (sType)
	{
	case 0:
		SkillIndex = m_SkillATTK[0];
		break;
	case 1:
		SkillIndex = m_SkillATTK[1];
		break;
	case 2:
		SkillIndex = m_SkillATTK[2];
		break;
	case 3:
		SkillIndex = m_SkillBuff[0];
		break;
	case 4:
		SkillIndex = m_SkillBuff[1];
		break;
	case 5:
		SkillIndex = m_SkillBuff[2];
		break;
	}
	return SkillIndex;
}

bool SEASON3B::CNewUIMacroMain::GetCheckBoxValue(int Number)
{
	bool success = false;
	MAPE_CHECKBOX::iterator it = m_checkBoxGroup[m_iCurrentClass].find(Number);

	if (it != m_checkBoxGroup[m_iCurrentClass].end())
	{
		success = it->second->IsSelected();
	}
	return success;
}

void SEASON3B::CNewUIMacroMain::SetCheckBoxValue(int Number, bool iVal)
{
	MAPE_CHECKBOX::iterator it = m_checkBoxGroup[m_iCurrentClass].find(Number);

	if (it != m_checkBoxGroup[m_iCurrentClass].end())
	{
		it->second->SetSelected(iVal);
	}
}

int SEASON3B::CNewUIMacroMain::GetOptionButtonValue(int Number)
{
	int value = 0;
	MAPE_BTNGROUP::iterator it = m_RadioOptionGroup[m_iCurrentClass].find(Number);

	if (it != m_RadioOptionGroup[m_iCurrentClass].end())
	{
		value = it->second->GetIndex();
	}
	return value;
}

void SEASON3B::CNewUIMacroMain::SetOptionButtonValue(int Number, int iVal)
{
	MAPE_BTNGROUP::iterator it = m_RadioOptionGroup[m_iCurrentClass].find(Number);

	if (it != m_RadioOptionGroup[m_iCurrentClass].end())
	{
		it->second->SetIndex(iVal);
	}
}

int SEASON3B::CNewUIMacroMain::GetInputBoxNumber(int Number)
{
	char pszText[255];
	memset(pszText, 0, sizeof(pszText));
	pszText[254] = 0;

	MAPE_TXTINPUT::iterator it = m_InputBoxGroup[m_iCurrentClass].find(Number);

	if (it != m_InputBoxGroup[m_iCurrentClass].end())
	{
		CUITextInputBox* temp_inputbox = it->second;

		temp_inputbox->GetText(pszText);
	}

	return atoi(pszText);
}

void SEASON3B::CNewUIMacroMain::GetInputBoxText(int Number, char* pszText)
{
	MAPE_TXTINPUT::iterator it = m_InputBoxGroup[m_iCurrentClass].find(Number);

	if (it != m_InputBoxGroup[m_iCurrentClass].end())
	{
		CUITextInputBox* temp_inputbox = it->second;

		temp_inputbox->GetText(pszText);
	}
}

void SEASON3B::CNewUIMacroMain::SetInputBoxNumber(int Number, int iVal)
{
	char pszText[255];
	memset(pszText, 0, sizeof(pszText));
	itoa(iVal, pszText, 0xAu);

	MAPE_TXTINPUT::iterator it = m_InputBoxGroup[m_iCurrentClass].find(Number);

	if (it != m_InputBoxGroup[m_iCurrentClass].end())
	{
		CUITextInputBox* temp_inputbox = it->second;

		temp_inputbox->SetText(pszText);
	}
}

void SEASON3B::CNewUIMacroMain::SetInputBoxText(int Number, const char* pszText)
{
	MAPE_TXTINPUT::iterator it = m_InputBoxGroup[m_iCurrentClass].find(Number);

	if (it != m_InputBoxGroup[m_iCurrentClass].end())
	{
		CUITextInputBox* temp_inputbox = it->second;

		temp_inputbox->SetText(pszText);
	}
}

void SEASON3B::CNewUIMacroMain::AddOptionButtonMapInGroup(COptionButtonGroup* option, BYTE Number)
{
	m_RadioOption.insert(MAPE_BTNGROUP::value_type(Number, option));
}

void SEASON3B::CNewUIMacroMain::AddCheckBoxMap(int imgindex, int x, int y, int width, int height, const char* text, BYTE Number)
{
	CCheckBox* temp_check = new CCheckBox;
	temp_check->RegisterImgIndex(imgindex);
	temp_check->ChangeCheckBoxText(text);
#if MAIN_UPDATE > 603
	temp_check->ChangeCheckBoxInfo(x, y, width, height, 45.f);
#else
	temp_check->ChangeCheckBoxInfo(x, y, width, height);
#endif

	m_checkBox.insert(MAPE_CHECKBOX::value_type(Number, temp_check));
}

void SEASON3B::CNewUIMacroMain::AddTextInputMap(int x, int y, int width, int height, int iMaxLength, int iOption, const char* text, BYTE Number)
{
	CUITextInputBox* temp_inputbox = new CUITextInputBox;

	temp_inputbox->Init(g_hWnd, width, height, iMaxLength, FALSE);
	temp_inputbox->SetPosition(x, y);
	temp_inputbox->SetText(text);
	temp_inputbox->SetTextColor(255, 0, 0, 0);
	temp_inputbox->SetBackColor(0, 0, 0, 0);
	temp_inputbox->SetFont(g_hFont);
	temp_inputbox->SetOption(iOption);
	temp_inputbox->SetState(UISTATE_NORMAL);

	m_InputBox.insert(MAPE_TXTINPUT::value_type(Number, temp_inputbox));
}

void SEASON3B::CNewUIMacroMain::AddButtonMap(int imgindex, int x, int y, int sx, int sy, bool overflg, bool isimgwidth, bool bClickEffect, bool MoveTxt, const char* btname, const char* tooltiptext, BYTE Number)
{
	CNewUIButton* temp_button = new CNewUIButton;

#if MAIN_UPDATE > 603
	if (imgindex == IMAGE_CHAINFO_BTN_STAT)
	{
		sx = 16; sy = 16;
		temp_button->ChangeButtonImgState(true, BITMAP_bt_min_max, overflg, isimgwidth, bClickEffect, 45.f, 0.f, 45.f, 46.f);
	}
	else if (imgindex == Image_RangeMinus)
	{
		sx = 16; sy = 16;
		temp_button->ChangeButtonImgState(true, BITMAP_bt_min_max, overflg, isimgwidth, bClickEffect, 0.f, 0.f, 45.f, 46.f);
	}
	else if (imgindex == IMAGE_IGS_VIEWDETAIL_BTN)
	{
		sy -= 2;
		temp_button->ChangeButtonImgState(true, BITMAP_ingame_bt01, overflg, isimgwidth, bClickEffect, 0.f, 0.f, 208, 77.f);
	}
	else if (imgindex == IMAGE_MINIMAPALPBTN)
	{
		sy -= 4;
		temp_button->ChangeButtonImgState(true, BITMAP_bt_clearness_illusion, overflg, isimgwidth, bClickEffect, 0.f, 0.f, 148, 68.f);
	}
	else if (imgindex == IMAGE_MACRO_MAIN_EXIT_BTN)
	{
		sx = 20; sy = 20;
		temp_button->ChangeButtonImgState(true, BITMAP_btn_exit_00, overflg, isimgwidth, bClickEffect, 0.f, 0.f, 69.f, 72.f);
	}
	else
		temp_button->ChangeButtonImgState(true, imgindex, overflg, isimgwidth, bClickEffect);
#else
	temp_button->ChangeButtonImgState(true, imgindex, overflg, isimgwidth, bClickEffect);
#endif
	temp_button->ChangeButtonInfo(x, y, sx, sy);
	temp_button->ChangeText(btname);
	temp_button->ChangeToolTipText(tooltiptext);

	if (MoveTxt)
		temp_button->MoveTextPos(0, -1);

	m_Buttons.insert(MAPE_BUTTON::value_type(Number, temp_button));
}

void SEASON3B::CNewUIMacroMain::AddTextMap(int x, int y, const char* text, BYTE Number)
{
	stMacroUIText* temp_text = new stMacroUIText;
	temp_text->Register(x, y, text);

	m_stMacroUIText.insert(MAPE_MACROTXT::value_type(Number, temp_text));
}

void SEASON3B::CNewUIMacroMain::AddImageMap(int imgindex, float x, float y, float width, float height, int Number)
{
	stMacroUIImage* temp_image = new stMacroUIImage;
	temp_image->Register(imgindex, x, y, width, height);

	m_stMacroUIImage.insert(MAPE_MACROIMG::value_type(Number, temp_image));
}

void SEASON3B::CNewUIMacroMain::AddOptionButtonGroupMapByClass(int sClass, BYTE Number)
{
	MAPE_BTNGROUP::iterator it = m_RadioOption.find(Number);

	if (it != m_RadioOption.end())
	{
		COptionButtonGroup* temp_check = it->second;

		if (sClass >= 0 && sClass < MAX_CLASS_PLAYER)
		{
			m_RadioOptionGroup[sClass].insert(MAPE_BTNGROUP::value_type(Number, temp_check));
		}
		else if (sClass == -1)
		{
			for (int i = 0; i < MAX_CLASS_PLAYER; i++)
			{
				m_RadioOptionGroup[i].insert(MAPE_BTNGROUP::value_type(Number, temp_check));
			}
		}
	}
}

void SEASON3B::CNewUIMacroMain::AddCheckBoxMapByClass(int sClass, BYTE Number)
{
	MAPE_CHECKBOX::iterator it = m_checkBox.find(Number);

	if (it != m_checkBox.end())
	{
		CCheckBox* temp_check = it->second;

		if (sClass >= 0 && sClass < MAX_CLASS_PLAYER)
		{
			m_checkBoxGroup[sClass].insert(MAPE_CHECKBOX::value_type(Number, temp_check));
		}
		else if (sClass == -1)
		{
			for (int i = 0; i < MAX_CLASS_PLAYER; i++)
			{
				m_checkBoxGroup[i].insert(MAPE_CHECKBOX::value_type(Number, temp_check));
			}
		}
	}
}

void SEASON3B::CNewUIMacroMain::AddTextInputMapByClass(int sClass, BYTE Number)
{
	MAPE_TXTINPUT::iterator it = m_InputBox.find(Number);

	if (it != m_InputBox.end())
	{
		CUITextInputBox* temp_inputbox = it->second;

		if (sClass >= 0 && sClass < MAX_CLASS_PLAYER)
		{
			m_InputBoxGroup[sClass].insert(MAPE_TXTINPUT::value_type(Number, temp_inputbox));
		}
		else if (sClass == -1)
		{
			for (int i = 0; i < MAX_CLASS_PLAYER; i++)
			{
				m_InputBoxGroup[i].insert(MAPE_TXTINPUT::value_type(Number, temp_inputbox));
			}
		}
	}
}

void SEASON3B::CNewUIMacroMain::AddButtonMapByClass(int sClass, BYTE Number)
{
	MAPE_BUTTON::iterator it = m_Buttons.find(Number);

	if (it != m_Buttons.end())
	{
		CNewUIButton* temp_button = it->second;

		if (sClass >= 0 && sClass < MAX_CLASS_PLAYER)
		{
			m_buttonGroup[sClass].insert(MAPE_BUTTON::value_type(Number, temp_button));
		}
		else if (sClass == -1)
		{
			for (int i = 0; i < MAX_CLASS_PLAYER; i++)
			{
				m_buttonGroup[i].insert(MAPE_BUTTON::value_type(Number, temp_button));
			}
		}
	}
}

void SEASON3B::CNewUIMacroMain::AddTextMapByClass(int sClass, BYTE Number)
{
	MAPE_MACROTXT::iterator it = m_stMacroUIText.find(Number);

	if (it != m_stMacroUIText.end())
	{
		stMacroUIText* temp_text = it->second;

		if (sClass >= 0 && sClass < MAX_CLASS_PLAYER)
		{
			m_MacroUITextGroup[sClass].insert(MAPE_MACROTXT::value_type(Number, temp_text));
		}
		else if (sClass == -1)
		{
			for (int i = 0; i < MAX_CLASS_PLAYER; i++)
			{
				m_MacroUITextGroup[i].insert(MAPE_MACROTXT::value_type(Number, temp_text));
			}
		}
	}
}

void SEASON3B::CNewUIMacroMain::AddImageMapByClass(int sClass, BYTE Number)
{
	MAPE_MACROIMG::iterator it = m_stMacroUIImage.find(Number);

	if (it != m_stMacroUIImage.end())
	{
		stMacroUIImage* temp_image = it->second;

		if (sClass >= 0 && sClass < MAX_CLASS_PLAYER)
		{
			m_MacroUIImageGroup[sClass].insert(MAPE_MACROIMG::value_type(Number, temp_image));
		}
		else if (sClass == -1)
		{
			for (int i = 0; i < MAX_CLASS_PLAYER; i++)
			{
				m_MacroUIImageGroup[i].insert(MAPE_MACROIMG::value_type(Number, temp_image));
			}
		}
	}
}

bool SEASON3B::CNewUIMacroMain::ImageMapUpdateMouseEvent(BYTE Number)
{
	MAPE_MACROIMG::iterator it = m_MacroUIImageGroup[m_iCurrentClass].find(Number);

	stMacroUIImage* image = NULL;

	if (it != m_MacroUIImageGroup[m_iCurrentClass].end())
	{
		image = it->second;
		return CheckMouseIn(image->m_Pos.x, image->m_Pos.y, image->output_width, image->output_height);
	}
	return false;
}

bool SEASON3B::CNewUIMacroMain::UpdateMouseEventSkillList(int* skillIndex)
{
	std::list<stSkillList*>* SkillList = nullptr;

	if (m_iRenderListType == 1)
	{
		SkillList = &m_stSkillList[0];
	}
	else if (m_iRenderListType == 2)
	{
		SkillList = &m_stSkillList[1];
	}

	if (SkillList)
	{
		for (auto it = SkillList->begin(); it != SkillList->end(); ++it)
		{
			stSkillList* info = *it;

			if (CheckMouseIn(info->iX, info->iY, 32, 38))
			{
				tooltipvisible = true;
				m_iRenderSkillInfoType = info->iIndex;
				m_iRenderSkillInfoPosX = info->iX;
				m_iRenderSkillInfoPosY = info->iY;

				if (IsRelease(VK_LBUTTON))
				{
					*skillIndex = info->iIndex;
				}
				return true;
			}
			tooltipvisible = false;
		}
	}
	return false;
}

bool SEASON3B::CNewUIMacroMain::UpdateMouseEventSelectedSkill()
{
	for (auto it = m_MacroUIImageGroup[m_iCurrentClass].begin(); it != m_MacroUIImageGroup[m_iCurrentClass].end(); ++it)
	{
		stMacroUIImage* image = it->second;

		if (it->first >= 0 && it->first <= 5)
		{
			if (CheckMouseIn(image->m_Pos.x, image->m_Pos.y, image->output_width, image->output_height))
			{
				tooltipvisible = true;
				m_iRenderSkillInfoType = GetSkillIndex(it->first);
				m_iRenderSkillInfoPosX = image->m_Pos.x;
				m_iRenderSkillInfoPosY = image->m_Pos.y;
				return true;
			}
		}
		tooltipvisible = false;
	}
	return false;
}

void SEASON3B::CNewUIMacroMain::RenderSkillList()
{
	std::list<stSkillList*>* SkillList = nullptr;

	if (m_iRenderListType == 1)
	{
		SkillList = &m_stSkillList[0];
	}
	else if (m_iRenderListType == 2)
	{
		SkillList = &m_stSkillList[1];
	}

	if (SkillList)
	{
		for (auto it = SkillList->begin(); it != SkillList->end(); ++it)
		{
			stSkillList* info = *it;

			if (info->iIndex == m_temp_skills)
			{
				RenderImage(IMAGE_SKILLBOX + 1, info->iX, info->iY, 32.0, 38.0);
			}
			else
			{
				RenderImage(IMAGE_SKILLBOX, info->iX, info->iY, 32.0, 38.0);
			}
			g_pSkillList->RenderSkillIcon(info->iIndex, info->iX + 6, info->iY + 6, 20.0, 28.0);
		}
	}
}

void SEASON3B::CNewUIMacroMain::CloseSkillList()
{
	IsUsedSkill[0] = false;
	IsUsedSkill[1] = false;
	IsUsedSkill[2] = false;
	IsUsedSkill[3] = false;
	IsUsedSkill[4] = false;
	IsUsedSkill[5] = false;
}

void SEASON3B::CNewUIMacroMain::CloseSkillListExceptOne(bool* success)
{
	if (!*success)
		CloseSkillList();
	*success = (*success == 0);
}

bool SEASON3B::CNewUIMacroMain::IsUsedSkillList()
{
	bool success = false;

	if (IsUsedSkill[0] == true
		|| IsUsedSkill[1] == true
		|| IsUsedSkill[2] == true
		|| IsUsedSkill[3] == true
		|| IsUsedSkill[4] == true
		|| IsUsedSkill[5] == true)
	{
		success = true;
	}

	return success;
}

void SEASON3B::CNewUIMacroMain::RegisterSkillIndex(int skillIndex)
{
	int* current_skill = NULL;
	int current_index = -1;

	if (IsUsedSkill[0] == true)
	{
		current_index = m_SkillATTK[0];
		current_skill = &m_SkillATTK[0];
	}
	else if (IsUsedSkill[1] == true)
	{
		current_index = m_SkillATTK[1];
		current_skill = &m_SkillATTK[1];
	}
	else if (IsUsedSkill[2] == true)
	{
		current_index = m_SkillATTK[2];
		current_skill = &m_SkillATTK[2];
	}
	else if (IsUsedSkill[3] == true)
	{
		current_index = m_SkillBuff[0];
		current_skill = &m_SkillBuff[0];
	}
	else if (IsUsedSkill[4] == true)
	{
		current_index = m_SkillBuff[1];
		current_skill = &m_SkillBuff[1];
	}
	else if (IsUsedSkill[5] == true)
	{
		current_index = m_SkillBuff[2];
		current_skill = &m_SkillBuff[2];
	}
	if (skillIndex != m_SkillATTK[0]
		&& skillIndex != m_SkillATTK[1]
		&& skillIndex != m_SkillATTK[2]
		&& skillIndex != m_SkillBuff[0]
		&& skillIndex != m_SkillBuff[1]
		&& skillIndex != m_SkillBuff[2]
		|| skillIndex == current_index)
	{
		if (current_skill)
			*current_skill = skillIndex;
	}
	else
	{
		CreateOkMessageBox(GlobalText[3569], -1);
	}
	this->CloseSkillList();
}

void SEASON3B::CNewUIMacroMain::GetImagePositionInImageMap(POINT* pos, BYTE Number)
{
	MAPE_MACROIMG::iterator it = m_MacroUIImageGroup[m_iCurrentClass].find(Number);

	stMacroUIImage* temp_image = NULL;

	if (it != m_MacroUIImageGroup[m_iCurrentClass].end())
	{
		temp_image = it->second;
		(*pos).x = temp_image->m_Pos.x;
		(*pos).y = temp_image->m_Pos.y;
	}
}

void SEASON3B::CNewUIMacroMain::LoadConfig()
{
	m_RangeHunting = gmAIController->AttackRange;
	m_RangePickUp = gmAIController->m_PickDistance;

	m_SkillATTK[0] = gmAIController->GetSkillIndex(gmAIController->SkillIndex);
	m_SkillATTK[1] = gmAIController->GetSkillIndex(gmAIController->SkillGroup[0].Index);
	m_SkillATTK[2] = gmAIController->GetSkillIndex(gmAIController->SkillGroup[1].Index);
	m_SkillBuff[0] = gmAIController->GetSkillIndex(gmAIController->m_SkillBuff[0]);
	m_SkillBuff[1] = gmAIController->GetSkillIndex(gmAIController->m_SkillBuff[1]);
	m_SkillBuff[2] = gmAIController->GetSkillIndex(gmAIController->m_SkillBuff[2]);

	this->SetCheckBoxValue(0, gmAIController->healPotion.Shutdown);
	this->SetCheckBoxValue(1, gmAIController->m_DistanceLong);
	this->SetCheckBoxValue(2, gmAIController->DistanceReturn);
	this->SetCheckBoxValue(3, gmAIController->SkillGroup[0].ShutdownDelay);
	this->SetCheckBoxValue(4, gmAIController->SkillGroup[0].Counter);
	this->SetCheckBoxValue(5, gmAIController->SkillGroup[1].ShutdownDelay);
	this->SetCheckBoxValue(6, gmAIController->SkillGroup[1].Counter);
	this->SetCheckBoxValue(7, gmAIController->ShutdownCombo);
	this->SetCheckBoxValue(8, gmAIController->ShutdownBuff);
	this->SetCheckBoxValue(9, gmAIController->ShutdownPetCmd);
	this->SetCheckBoxValue(10, gmAIController->ShutdownParty);
	this->SetCheckBoxValue(11, gmAIController->healskillLife.Shutdown);
	this->SetCheckBoxValue(12, gmAIController->healskilldrain.Shutdown);
	this->SetCheckBoxValue(13, gmAIController->ShutdownRepair);
	this->SetCheckBoxValue(14, gmAIController->Picker_work.Near);
	this->SetCheckBoxValue(15, gmAIController->Picker_work.Select);
	this->SetCheckBoxValue(16, gmAIController->Picker_work.Jewel);
	this->SetCheckBoxValue(17, gmAIController->Picker_work.Ancient);
	this->SetCheckBoxValue(18, gmAIController->Picker_work.Zen);
	this->SetCheckBoxValue(19, gmAIController->Picker_work.Excellent);
	this->SetCheckBoxValue(20, gmAIController->Picker_work.Extra);
	this->SetCheckBoxValue(21, gmAIController->m_AutoFriend);
	this->SetCheckBoxValue(23, gmAIController->m_AutoGuildMember);
	this->SetCheckBoxValue(22, gmAIController->m_EliteManaPotion);

	this->SetOptionButtonValue(0, gmAIController->PetCmdType);

	this->SetInputBoxNumber(0, gmAIController->m_DistanceOnSec);
	this->SetInputBoxNumber(1, gmAIController->SkillGroup[0].TimeDelay);
	this->SetInputBoxNumber(2, gmAIController->SkillGroup[1].TimeDelay);

	m_ListBox.ClearText();
	for (size_t i = 0; i < MAX_PICK_EXTRA; i++)
	{
		if (strlen(gmAIController->Picker_work.Name[i]))
		{
			m_ListBox.AddText(gmAIController->Picker_work.Name[i]);
		}
	}
}

void SEASON3B::CNewUIMacroMain::SaveConfig()
{
	gmAIController->AttackRange = m_RangeHunting;
	gmAIController->m_PickDistance = m_RangePickUp;

	if (m_SkillATTK[0] == -1)
		gmAIController->SkillIndex = 0;
	else
		gmAIController->SkillIndex = CharacterAttribute->Skill[m_SkillATTK[0]];

	if (m_SkillATTK[1] == -1)
		gmAIController->SkillGroup[0].Index = 0;
	else
		gmAIController->SkillGroup[0].Index = CharacterAttribute->Skill[m_SkillATTK[1]];

	if (m_SkillATTK[2] == -1)
		gmAIController->SkillGroup[1].Index = 0;
	else
		gmAIController->SkillGroup[1].Index = CharacterAttribute->Skill[m_SkillATTK[2]];

	if (m_SkillBuff[0] == -1)
		gmAIController->m_SkillBuff[0] = 0;
	else
		gmAIController->m_SkillBuff[0] = CharacterAttribute->Skill[m_SkillBuff[0]];

	if (m_SkillBuff[1] == -1)
		gmAIController->m_SkillBuff[1] = 0;
	else
		gmAIController->m_SkillBuff[1] = CharacterAttribute->Skill[m_SkillBuff[1]];

	if (m_SkillBuff[2] == -1)
		gmAIController->m_SkillBuff[2] = 0;
	else
		gmAIController->m_SkillBuff[2] = CharacterAttribute->Skill[m_SkillBuff[2]];

	gmAIController->healPotion.Shutdown = this->GetCheckBoxValue(0);
	gmAIController->m_DistanceLong = this->GetCheckBoxValue(1);
	gmAIController->DistanceReturn = this->GetCheckBoxValue(2);

	gmAIController->SkillGroup[0].ShutdownDelay = this->GetCheckBoxValue(3);
	gmAIController->SkillGroup[0].Counter = this->GetCheckBoxValue(4);
	gmAIController->SkillGroup[1].ShutdownDelay = this->GetCheckBoxValue(5);
	gmAIController->SkillGroup[1].Counter = this->GetCheckBoxValue(6);

	gmAIController->ShutdownCombo = this->GetCheckBoxValue(7);
	gmAIController->ShutdownBuff = this->GetCheckBoxValue(8);
	gmAIController->ShutdownPetCmd = this->GetCheckBoxValue(9);
	gmAIController->ShutdownParty = this->GetCheckBoxValue(10);

	gmAIController->healskillLife.Shutdown = this->GetCheckBoxValue(11);
	gmAIController->healskilldrain.Shutdown = this->GetCheckBoxValue(12);
	gmAIController->ShutdownRepair = this->GetCheckBoxValue(13);
	gmAIController->Picker_work.Near = this->GetCheckBoxValue(14);
	gmAIController->Picker_work.Select = this->GetCheckBoxValue(15);
	gmAIController->Picker_work.Jewel = this->GetCheckBoxValue(16);
	gmAIController->Picker_work.Ancient = this->GetCheckBoxValue(17);
	gmAIController->Picker_work.Zen = this->GetCheckBoxValue(18);
	gmAIController->Picker_work.Excellent = this->GetCheckBoxValue(19);
	gmAIController->Picker_work.Extra = this->GetCheckBoxValue(20);

	gmAIController->m_AutoFriend = this->GetCheckBoxValue(21);
	gmAIController->m_AutoGuildMember = this->GetCheckBoxValue(23);
	gmAIController->m_EliteManaPotion = this->GetCheckBoxValue(22);

	gmAIController->PetCmdType = this->GetOptionButtonValue(0);

	gmAIController->m_DistanceOnSec = this->GetInputBoxNumber(0);
	gmAIController->SkillGroup[0].TimeDelay = this->GetInputBoxNumber(1);
	gmAIController->SkillGroup[1].TimeDelay = this->GetInputBoxNumber(2);

	memset(gmAIController->Picker_work.Name, 0, sizeof(gmAIController->Picker_work.Name));

	for (size_t i = 0; i < MAX_PICK_EXTRA; i++)
	{
		const char* pszText = m_ListBox.GetText(i);

		if (pszText && strlen(pszText))
		{
			strcpy_s(gmAIController->Picker_work.Name[i], pszText);
		}
	}
	gmAIController->SaveConfig();
}

void SEASON3B::CNewUIMacroMain::ResetConfig()
{
	m_SkillATTK[0] = -1;
	m_SkillATTK[1] = -1;
	m_SkillATTK[2] = -1;
	m_SkillBuff[0] = -1;
	m_SkillBuff[1] = -1;
	m_SkillBuff[2] = -1;

	m_RangePickUp = 8;
	m_RangeHunting = 6;

	this->SetCheckBoxValue(0, 1);
	this->SetCheckBoxValue(1, 0);
	this->SetCheckBoxValue(2, 1);
	this->SetCheckBoxValue(3, 0);
	this->SetCheckBoxValue(4, 0);
	this->SetCheckBoxValue(5, 0);
	this->SetCheckBoxValue(6, 0);
	this->SetCheckBoxValue(7, 0);
	this->SetCheckBoxValue(8, 0);
	this->SetCheckBoxValue(9, 0);
	this->SetCheckBoxValue(10, 0);
	this->SetCheckBoxValue(11, 0);
	this->SetCheckBoxValue(12, 0);
	this->SetCheckBoxValue(13, 0);
	this->SetCheckBoxValue(14, 1);
	this->SetCheckBoxValue(15, 0);
	this->SetCheckBoxValue(16, 0);
	this->SetCheckBoxValue(17, 0);
	this->SetCheckBoxValue(18, 0);
	this->SetCheckBoxValue(19, 0);
	this->SetCheckBoxValue(20, 0);
	this->SetCheckBoxValue(21, 0);
	this->SetCheckBoxValue(23, 0);
	this->SetCheckBoxValue(22, 0);
	this->SetOptionButtonValue(0, 0);
	this->SetInputBoxNumber(0, 10);
	this->SetInputBoxNumber(1, 0);
	this->SetInputBoxNumber(2, 0);
	m_ListBox.ClearText();
}

int SEASON3B::CNewUIMacroMain::GetSkillListOffset()
{
	// The MuBattle party mini-window is fixed and does not expose its rectangle.
	// The Helper list already renders beside the configuration window safely.
	return 0;
}

/*
void SEASON3B::CNewUIMacroMain::ClearMap(MAPE_CHECKBOX& map)
{
	for (MAPE_CHECKBOX::iterator& pair = map.begin(); pair != map.end(); pair++)
	{
		SAFE_DELETE(pair->second);
	}
	map.clear();
}

void SEASON3B::CNewUIMacroMain::ClearMap(MAPE_BTNGROUP& map)
{
	for (MAPE_BTNGROUP::iterator& pair = map.begin(); pair != map.end(); pair++)
	{
		SAFE_DELETE(pair->second); // Libera la memoria apuntada
	}
	map.clear(); // Limpia el mapa
}

void SEASON3B::CNewUIMacroMain::ClearMap(MAPE_TXTINPUT& map)
{
	for (MAPE_TXTINPUT::iterator& pair = map.begin(); pair != map.end(); pair++)
	{
		SAFE_DELETE(pair->second); // Libera la memoria apuntada
	}
	map.clear(); // Limpia el mapa
}

void SEASON3B::CNewUIMacroMain::ClearMap(MAPE_BUTTON& map)
{
	for (MAPE_BUTTON::iterator& pair = map.begin(); pair != map.end(); pair++)
	{
		SAFE_DELETE(pair->second); // Libera la memoria apuntada
	}
	map.clear(); // Limpia el mapa
}

void SEASON3B::CNewUIMacroMain::ClearMap(MAPE_MACROTXT& map)
{
	for (MAPE_MACROTXT::iterator& pair = map.begin(); pair != map.end(); pair++)
	{
		SAFE_DELETE(pair->second); // Libera la memoria apuntada
	}
	map.clear(); // Limpia el mapa
}

void SEASON3B::CNewUIMacroMain::ClearMap(MAPE_MACROIMG& map)
{
	for (MAPE_MACROIMG::iterator& pair = map.begin(); pair != map.end(); pair++)
	{
		SAFE_DELETE(pair->second); // Libera la memoria apuntada
	}
	map.clear(); // Limpia el mapa
}*/
