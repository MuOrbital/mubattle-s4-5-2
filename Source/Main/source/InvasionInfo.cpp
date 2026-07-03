#include "stdafx.h"
#include "InvasionInfo.h"
#include "NewUICommon.h"
#include "NewUIQuestProgress.h"
#include "NewUIWindowMenu.h"
#include "ZzzInfomation.h"

CInvasionInfo gInvasionInfo;

using namespace SEASON3B;

extern float g_fScreenRate_x;
extern float g_fScreenRate_y;

static const int INVASION_MAX_COUNT = 30;
static const float INVASION_FRAME_WIDTH = 160.0f;
static const float INVASION_FRAME_HEIGHT = 22.0f;
static const float INVASION_TABLE_GAP = 4.0f;
static const float INVASION_ARROW_WIDTH = 17.0f;
static const float INVASION_ARROW_HEIGHT = 18.0f;
static const float INVASION_ARROW_LEFT_X = 3.0f;
static const float INVASION_ARROW_RIGHT_MARGIN = 20.0f;

static float InvasionClampFloat(float value, float minimum, float maximum)
{
	if (value < minimum)
	{
		return minimum;
	}

	if (value > maximum)
	{
		return maximum;
	}

	return value;
}

static void InvasionRenderArrow(GLuint image, float x, float y, bool enabled)
{
	SEASON3B::RenderImage(image, x, y, INVASION_ARROW_WIDTH, INVASION_ARROW_HEIGHT, 0.0f, (enabled != false) ? 0.0f : INVASION_ARROW_HEIGHT);
}

CInvasionInfo::CInvasionInfo()
{
	this->m_CurrentIndex = -1;
	this->m_OpenTable = false;
	this->m_LastTick = GetTickCount();
	this->m_ClickTick = 0;
	this->m_CustomPosition = false;
	this->m_Dragging = false;
	this->m_DragMoved = false;
	this->m_FrameX = 0.0f;
	this->m_FrameY = 0.0f;
	this->m_DragStartX = 0.0f;
	this->m_DragStartY = 0.0f;
	this->m_DragLastX = 0.0f;
	this->m_DragLastY = 0.0f;
	this->m_MouseX = 0.0f;
	this->m_MouseY = 0.0f;
}

void CInvasionInfo::GCInvasionInfoRecv(BYTE* lpMsg, int size)
{
	if (lpMsg == 0 || size < (int)sizeof(PMSG_INVASION_INFO_RECV))
	{
		return;
	}

	PMSG_INVASION_INFO_RECV* lpRecv = (PMSG_INVASION_INFO_RECV*)lpMsg;

	if (lpRecv->Index < 0 || lpRecv->Index >= INVASION_MAX_COUNT)
	{
		return;
	}

	if (lpRecv->RemainTime <= 0 || lpRecv->count <= 0)
	{
		this->m_InvasionInfo.erase(lpRecv->Index);

		if (this->m_CurrentIndex == lpRecv->Index)
		{
			this->SelectFirstActive();
		}

		return;
	}

	int offset = sizeof(PMSG_INVASION_INFO_RECV);
	int maxCount = (size - offset) / sizeof(INVASION_CLIENT_MONSTER_INFO);
	int count = lpRecv->count;

	if (count > maxCount)
	{
		count = maxCount;
	}

	INVASION_CLIENT_INFO info;
	info.Index = lpRecv->Index;
	info.TimeRemaining = (DWORD)lpRecv->RemainTime;
	memset(info.Name, 0, sizeof(info.Name));
	memcpy(info.Name, lpRecv->Name, sizeof(info.Name) - 1);
	info.MonsterInfo.clear();

	for (int n = 0; n < count; n++)
	{
		INVASION_CLIENT_MONSTER_INFO* lpMonster = (INVASION_CLIENT_MONSTER_INFO*)(lpMsg + offset);
		offset += sizeof(INVASION_CLIENT_MONSTER_INFO);

		if (lpMonster->MonsterCount <= 0)
		{
			continue;
		}

		INVASION_CLIENT_MONSTER_INFO monster;
		monster.MonsterIndex = lpMonster->MonsterIndex;
		monster.MonsterCount = lpMonster->MonsterCount;
		monster.Monster_Kill = lpMonster->Monster_Kill;

		if (monster.Monster_Kill < 0)
		{
			monster.Monster_Kill = 0;
		}

		if (monster.Monster_Kill > monster.MonsterCount)
		{
			monster.Monster_Kill = monster.MonsterCount;
		}

		info.MonsterInfo[monster.MonsterIndex] = monster;
	}

	if (info.MonsterInfo.empty() != 0)
	{
		this->m_InvasionInfo.erase(info.Index);
		return;
	}

	this->m_InvasionInfo[info.Index] = info;

	if (this->m_CurrentIndex == -1 || this->m_InvasionInfo.find(this->m_CurrentIndex) == this->m_InvasionInfo.end())
	{
		this->SelectFirstActive();
	}
}

void CInvasionInfo::GCInvasionKillRecv(PMSG_INVASION_KILL_RECV* lpMsg)
{
	if (lpMsg == 0)
	{
		return;
	}

	std::map<int, INVASION_CLIENT_INFO>::iterator invasion = this->m_InvasionInfo.find(lpMsg->Index);

	if (invasion == this->m_InvasionInfo.end())
	{
		return;
	}

	INVASION_CLIENT_MONSTER_INFO monster;
	monster.MonsterIndex = lpMsg->MonsterIndex;
	monster.Monster_Kill = lpMsg->Monster_Kill;
	monster.MonsterCount = lpMsg->MonsterCount;

	if (monster.MonsterCount <= 0)
	{
		invasion->second.MonsterInfo.erase(monster.MonsterIndex);
		return;
	}

	if (monster.Monster_Kill < 0)
	{
		monster.Monster_Kill = 0;
	}

	if (monster.Monster_Kill > monster.MonsterCount)
	{
		monster.Monster_Kill = monster.MonsterCount;
	}

	invasion->second.MonsterInfo[monster.MonsterIndex] = monster;

	bool finished = true;

	for (std::map<int, INVASION_CLIENT_MONSTER_INFO>::iterator it = invasion->second.MonsterInfo.begin(); it != invasion->second.MonsterInfo.end(); it++)
	{
		if (it->second.MonsterCount > 0 && it->second.Monster_Kill < it->second.MonsterCount)
		{
			finished = false;
			break;
		}
	}

	if (finished != false)
	{
		int removedIndex = invasion->first;
		this->m_InvasionInfo.erase(invasion);

		if (this->m_CurrentIndex == removedIndex)
		{
			this->SelectFirstActive();
		}
	}
}

void CInvasionInfo::Update()
{
	DWORD currentTick = GetTickCount();

	if ((currentTick - this->m_LastTick) < 1000)
	{
		return;
	}

	DWORD elapsedSeconds = (currentTick - this->m_LastTick) / 1000;
	this->m_LastTick += (elapsedSeconds * 1000);

	for (std::map<int, INVASION_CLIENT_INFO>::iterator it = this->m_InvasionInfo.begin(); it != this->m_InvasionInfo.end();)
	{
		if (it->second.TimeRemaining > elapsedSeconds)
		{
			it->second.TimeRemaining -= elapsedSeconds;
			it++;
		}
		else
		{
			int removedIndex = it->first;
			std::map<int, INVASION_CLIENT_INFO>::iterator remove = it;
			it++;
			this->m_InvasionInfo.erase(remove);

			if (this->m_CurrentIndex == removedIndex)
			{
				this->SelectFirstActive();
			}
		}
	}
}

void CInvasionInfo::Draw()
{
	if (SceneFlag != MAIN_SCENE)
	{
		return;
	}

	INVASION_CLIENT_INFO* lpInfo = this->GetCurrentInvasion();

	if (lpInfo == 0)
	{
		return;
	}

	float frameX;
	float frameY;
	float frameW;
	float frameH;
	this->GetFrame(frameX, frameY, frameW, frameH);

	float arrowY = frameY + ((frameH - INVASION_ARROW_HEIGHT) / 2.0f);

	EnableAlphaBlend3();
	RenderColor(frameX, frameY, frameW, frameH, 0.80f, 1);
	EndRenderColor();
	EnableAlphaTest();

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetBgColor(0, 0, 0, 0);
	g_pRenderText->SetTextColor(255, 220, 120, 255);

	InvasionRenderArrow(CNewUIQuestProgress::IMAGE_QP_BTN_L, frameX + INVASION_ARROW_LEFT_X, arrowY, true);
	InvasionRenderArrow(CNewUIQuestProgress::IMAGE_QP_BTN_R, frameX + frameW - INVASION_ARROW_RIGHT_MARGIN, arrowY, true);
	g_pRenderText->RenderText((int)(frameX + 24.0f), (int)(frameY + 5.0f), lpInfo->Name, (int)(frameW - 48.0f), 0, RT3_SORT_CENTER);

	if (this->m_OpenTable == false)
	{
		return;
	}

	float tableW = INVASION_FRAME_WIDTH;
	float tableX = frameX + ((frameW - tableW) / 2.0f);
	float tableY = frameY + frameH + INVASION_TABLE_GAP;
	float tableH = 30.0f + (float)(lpInfo->MonsterInfo.size() * 12);

	EnableAlphaBlend3();
	RenderColor(tableX, tableY, tableW, tableH, 0.80f, 1);
	EndRenderColor();
	EnableAlphaTest();

	DWORD totalSeconds = lpInfo->TimeRemaining;
	DWORD hours = totalSeconds / 3600;
	DWORD minutes = (totalSeconds / 60) % 60;
	DWORD seconds = totalSeconds % 60;

	char text[128];
	sprintf(text, "Time: %02d:%02d:%02d", hours, minutes, seconds);
	g_pRenderText->SetTextColor(255, 220, 120, 255);
	g_pRenderText->RenderText((int)(tableX + 4.0f), (int)(tableY + 8.0f), text, (int)(tableW - 8.0f), 0, RT3_SORT_CENTER);

	int line = 0;

	for (std::map<int, INVASION_CLIENT_MONSTER_INFO>::iterator it = lpInfo->MonsterInfo.begin(); it != lpInfo->MonsterInfo.end(); it++)
	{
		char fallbackName[32];
		char* monsterName = getMonsterName(it->second.MonsterIndex);

		if (monsterName == 0 || strlen(monsterName) == 0)
		{
			strcpy(fallbackName, "Monster");
			monsterName = fallbackName;
		}

		g_pRenderText->SetFont(g_hFont);
		g_pRenderText->SetTextColor(235, 235, 235, 255);
		g_pRenderText->RenderText((int)(tableX + 6.0f), (int)(tableY + 24.0f + line), monsterName, 108, 0, RT3_SORT_LEFT);

		sprintf(text, "%d/%d", it->second.Monster_Kill, it->second.MonsterCount);
		g_pRenderText->SetTextColor(80, 255, 160, 255);
		g_pRenderText->RenderText((int)(tableX + tableW - 46.0f), (int)(tableY + 24.0f + line), text, 40, 0, RT3_SORT_RIGHT);

		line += 12;
	}
}

bool CInvasionInfo::Mouse(MOUSEHOOKSTRUCTEX* Mouse, WPARAM wParam)
{
	if (SceneFlag != MAIN_SCENE)
	{
		return false;
	}

	POINT cursor = Mouse->pt;
	ScreenToClient(g_hWnd, &cursor);
	this->m_MouseX = (float)cursor.x / ((g_fScreenRate_x > 0.0f) ? g_fScreenRate_x : 1.0f);
	this->m_MouseY = (float)cursor.y / ((g_fScreenRate_y > 0.0f) ? g_fScreenRate_y : 1.0f);

	INVASION_CLIENT_INFO* lpInfo = this->GetCurrentInvasion();

	if (lpInfo == 0)
	{
		this->m_Dragging = false;
		return false;
	}

	float frameX;
	float frameY;
	float frameW;
	float frameH;
	this->GetFrame(frameX, frameY, frameW, frameH);

	float tableW = INVASION_FRAME_WIDTH;
	float tableX = frameX + ((frameW - tableW) / 2.0f);
	float tableY = frameY + frameH + INVASION_TABLE_GAP;
	float tableH = 30.0f + (float)(lpInfo->MonsterInfo.size() * 12);
	float arrowY = frameY + ((frameH - INVASION_ARROW_HEIGHT) / 2.0f);
	int activeCount = this->GetActiveCount();

	bool overFrame = this->IsMouseIn(frameX, frameY, frameW, frameH);
	bool overTable = (this->m_OpenTable != false && this->IsMouseIn(tableX, tableY, tableW, tableH));
	bool overPrevious = this->IsMouseIn(frameX + INVASION_ARROW_LEFT_X, arrowY, INVASION_ARROW_WIDTH, INVASION_ARROW_HEIGHT);
	bool overNext = this->IsMouseIn(frameX + frameW - INVASION_ARROW_RIGHT_MARGIN, arrowY, INVASION_ARROW_WIDTH, INVASION_ARROW_HEIGHT);

	if (wParam == WM_MOUSEMOVE && this->m_Dragging != false)
	{
		float sideOffset = ((this->m_OpenTable != false && tableW > frameW) ? ((tableW - frameW) / 2.0f) : 0.0f);
		float minX = sideOffset;
		float logicalWidth = ((g_fScreenRate_x > 0.0f) ? ((float)WindowWidth / g_fScreenRate_x) : 640.0f);
		float logicalHeight = ((g_fScreenRate_y > 0.0f) ? ((float)WindowHeight / g_fScreenRate_y) : 480.0f);
		float maxX = logicalWidth - frameW - sideOffset;
		float visibleHeight = frameH + ((this->m_OpenTable != false) ? (INVASION_TABLE_GAP + tableH) : 0.0f);
		float maxY = logicalHeight - visibleHeight;
		float deltaX = this->m_MouseX - this->m_DragLastX;
		float deltaY = this->m_MouseY - this->m_DragLastY;

		if (maxX < minX)
		{
			maxX = minX;
		}

		if (maxY < 0.0f)
		{
			maxY = 0.0f;
		}

		if (fabs(deltaX) > (logicalWidth / 2.0f))
		{
			deltaX = 0.0f;
		}

		if (fabs(deltaY) > (logicalHeight / 2.0f))
		{
			deltaY = 0.0f;
		}

		this->m_FrameX += deltaX;
		this->m_FrameY += deltaY;
		this->m_DragLastX = this->m_MouseX;
		this->m_DragLastY = this->m_MouseY;

		if (this->m_FrameX < minX)
		{
			this->m_FrameX = minX;
		}
		else if (this->m_FrameX > maxX)
		{
			this->m_FrameX = maxX;
		}

		if (this->m_FrameY < 0.0f)
		{
			this->m_FrameY = 0.0f;
		}
		else if (this->m_FrameY > maxY)
		{
			this->m_FrameY = maxY;
		}

		if (fabs(this->m_MouseX - this->m_DragStartX) > 2.0f || fabs(this->m_MouseY - this->m_DragStartY) > 2.0f)
		{
			this->m_DragMoved = true;
		}

		return true;
	}

	if (wParam != WM_LBUTTONDOWN && wParam != WM_LBUTTONUP)
	{
		return false;
	}

	if (wParam == WM_LBUTTONDOWN && overFrame != false && overPrevious == false && overNext == false)
	{
		this->m_CustomPosition = true;
		this->m_Dragging = true;
		this->m_DragMoved = false;
		this->m_FrameX = frameX;
		this->m_FrameY = frameY;
		this->m_DragStartX = this->m_MouseX;
		this->m_DragStartY = this->m_MouseY;
		this->m_DragLastX = this->m_MouseX;
		this->m_DragLastY = this->m_MouseY;
		return true;
	}

	if (wParam == WM_LBUTTONDOWN)
	{
		return (overFrame != false || overTable != false);
	}

	bool wasDragging = this->m_Dragging;
	bool wasMoved = this->m_DragMoved;
	this->m_Dragging = false;

	if (overFrame == false && overTable == false && wasDragging == false)
	{
		return false;
	}

	if (overPrevious != false)
	{
		if (activeCount > 1)
		{
			this->PreviousInvasion();
		}

		return true;
	}

	if (overNext != false)
	{
		if (activeCount > 1)
		{
			this->NextInvasion();
		}

		return true;
	}

	if (wasDragging != false && wasMoved == false)
	{
		this->m_OpenTable = !this->m_OpenTable;

		if (this->m_OpenTable != false && this->m_CustomPosition != false)
		{
			float logicalWidth = ((g_fScreenRate_x > 0.0f) ? ((float)WindowWidth / g_fScreenRate_x) : 640.0f);
			float logicalHeight = ((g_fScreenRate_y > 0.0f) ? ((float)WindowHeight / g_fScreenRate_y) : 480.0f);
			float sideOffset = (tableW - frameW) / 2.0f;
			float maxX = logicalWidth - frameW - sideOffset;
			float maxY = logicalHeight - frameH - INVASION_TABLE_GAP - tableH;

			this->m_FrameX = InvasionClampFloat(this->m_FrameX, sideOffset, maxX);
			this->m_FrameY = InvasionClampFloat(this->m_FrameY, 0.0f, ((maxY > 0.0f) ? maxY : 0.0f));
		}

		return true;
	}

	return (wasDragging != false || overTable != false);
}

bool CInvasionInfo::CheckMouse()
{
	if (SceneFlag != MAIN_SCENE)
	{
		return false;
	}

	INVASION_CLIENT_INFO* lpInfo = this->GetCurrentInvasion();

	if (lpInfo == 0)
	{
		return false;
	}

	if (this->m_Dragging != false)
	{
		return true;
	}

	float frameX;
	float frameY;
	float frameW;
	float frameH;
	this->GetFrame(frameX, frameY, frameW, frameH);

	if (this->IsMouseIn(frameX, frameY, frameW, frameH) != false)
	{
		return true;
	}

	if (this->m_OpenTable != false)
	{
		float tableW = INVASION_FRAME_WIDTH;
		float tableX = frameX + ((frameW - tableW) / 2.0f);
		float tableY = frameY + frameH + INVASION_TABLE_GAP;
		float tableH = 30.0f + (float)(lpInfo->MonsterInfo.size() * 12);

		if (this->IsMouseIn(tableX, tableY, tableW, tableH) != false)
		{
			return true;
		}
	}

	return false;
}

void CInvasionInfo::UpdateMouse()
{
	if (this->CheckMouse() == false)
	{
		return;
	}

	MouseOnWindow = true;
}

INVASION_CLIENT_INFO* CInvasionInfo::GetCurrentInvasion()
{
	std::map<int, INVASION_CLIENT_INFO>::iterator it = this->m_InvasionInfo.find(this->m_CurrentIndex);

	if (it == this->m_InvasionInfo.end() || it->second.TimeRemaining == 0 || it->second.MonsterInfo.empty() != 0)
	{
		this->SelectFirstActive();
		it = this->m_InvasionInfo.find(this->m_CurrentIndex);
	}

	if (it == this->m_InvasionInfo.end())
	{
		return 0;
	}

	return &it->second;
}

int CInvasionInfo::GetActiveCount()
{
	int count = 0;

	for (std::map<int, INVASION_CLIENT_INFO>::iterator it = this->m_InvasionInfo.begin(); it != this->m_InvasionInfo.end(); it++)
	{
		if (it->second.TimeRemaining > 0 && it->second.MonsterInfo.empty() == 0)
		{
			count++;
		}
	}

	return count;
}

void CInvasionInfo::SelectFirstActive()
{
	this->m_CurrentIndex = -1;

	for (std::map<int, INVASION_CLIENT_INFO>::iterator it = this->m_InvasionInfo.begin(); it != this->m_InvasionInfo.end(); it++)
	{
		if (it->second.TimeRemaining > 0 && it->second.MonsterInfo.empty() == 0)
		{
			this->m_CurrentIndex = it->first;
			return;
		}
	}
}

void CInvasionInfo::NextInvasion()
{
	if (this->m_InvasionInfo.empty() != 0)
	{
		return;
	}

	bool selectNext = false;

	for (std::map<int, INVASION_CLIENT_INFO>::iterator it = this->m_InvasionInfo.begin(); it != this->m_InvasionInfo.end(); it++)
	{
		if (it->second.TimeRemaining == 0 || it->second.MonsterInfo.empty() != 0)
		{
			continue;
		}

		if (selectNext != false)
		{
			this->m_CurrentIndex = it->first;
			return;
		}

		if (it->first == this->m_CurrentIndex)
		{
			selectNext = true;
		}
	}

	this->SelectFirstActive();
}

void CInvasionInfo::PreviousInvasion()
{
	if (this->m_InvasionInfo.empty() != 0)
	{
		return;
	}

	int previousIndex = -1;
	int lastIndex = -1;
	bool foundCurrent = false;

	for (std::map<int, INVASION_CLIENT_INFO>::iterator it = this->m_InvasionInfo.begin(); it != this->m_InvasionInfo.end(); it++)
	{
		if (it->second.TimeRemaining == 0 || it->second.MonsterInfo.empty() != 0)
		{
			continue;
		}

		lastIndex = it->first;

		if (it->first == this->m_CurrentIndex)
		{
			foundCurrent = true;
			break;
		}

		previousIndex = it->first;
	}

	if (foundCurrent == false)
	{
		this->SelectFirstActive();
		return;
	}

	if (previousIndex != -1)
	{
		this->m_CurrentIndex = previousIndex;
		return;
	}

	for (std::map<int, INVASION_CLIENT_INFO>::iterator it = this->m_InvasionInfo.begin(); it != this->m_InvasionInfo.end(); it++)
	{
		if (it->second.TimeRemaining > 0 && it->second.MonsterInfo.empty() == 0)
		{
			lastIndex = it->first;
		}
	}

	if (lastIndex != -1)
	{
		this->m_CurrentIndex = lastIndex;
	}
}

bool CInvasionInfo::IsMouseIn(float x, float y, float width, float height)
{
	return (this->m_MouseX >= x && this->m_MouseX <= (x + width) && this->m_MouseY >= y && this->m_MouseY <= (y + height));
}

void CInvasionInfo::GetFrame(float& x, float& y, float& width, float& height)
{
	width = INVASION_FRAME_WIDTH;
	height = INVASION_FRAME_HEIGHT;

	if (this->m_CustomPosition != false)
	{
		float logicalWidth = ((g_fScreenRate_x > 0.0f) ? ((float)WindowWidth / g_fScreenRate_x) : 640.0f);
		float logicalHeight = ((g_fScreenRate_y > 0.0f) ? ((float)WindowHeight / g_fScreenRate_y) : 480.0f);
		float maxX = logicalWidth - width;
		float maxY = logicalHeight - height;

		if (maxX < 0.0f)
		{
			maxX = 0.0f;
		}

		if (maxY < 0.0f)
		{
			maxY = 0.0f;
		}

		this->m_FrameX = InvasionClampFloat(this->m_FrameX, 0.0f, maxX);
		this->m_FrameY = InvasionClampFloat(this->m_FrameY, 0.0f, maxY);
		x = this->m_FrameX;
		y = this->m_FrameY;
	}
	else
	{
		float logicalWidth = ((g_fScreenRate_x > 0.0f) ? ((float)WindowWidth / g_fScreenRate_x) : 640.0f);
		x = (logicalWidth / 2.0f) - (width / 2.0f);
		y = 20.0f;
	}
}
