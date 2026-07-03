#pragma once

#include <map>
#include "Protocol.h"

struct INVASION_CLIENT_MONSTER_INFO
{
	int MonsterIndex;
	int Monster_Kill;
	int MonsterCount;
};

struct INVASION_CLIENT_INFO
{
	int Index;
	char Name[64];
	DWORD TimeRemaining;
	std::map<int, INVASION_CLIENT_MONSTER_INFO> MonsterInfo;
};

struct PMSG_INVASION_INFO_RECV
{
	PSWMSG_HEAD header;
	int Index;
	char Name[64];
	int RemainTime;
	int count;
};

struct PMSG_INVASION_KILL_RECV
{
	PSWMSG_HEAD header;
	int Index;
	int MonsterIndex;
	int Monster_Kill;
	int MonsterCount;
};

class CInvasionInfo
{
public:
	CInvasionInfo();
	void GCInvasionInfoRecv(BYTE* lpMsg, int size);
	void GCInvasionKillRecv(PMSG_INVASION_KILL_RECV* lpMsg);
	void Update();
	void Draw();
	bool Mouse(MOUSEHOOKSTRUCTEX* Mouse, WPARAM wParam);
	bool CheckMouse();
	void UpdateMouse();

private:
	INVASION_CLIENT_INFO* GetCurrentInvasion();
	int GetActiveCount();
	void SelectFirstActive();
	void NextInvasion();
	void PreviousInvasion();
	bool IsMouseIn(float x, float y, float width, float height);
	void GetFrame(float& x, float& y, float& width, float& height);

private:
	std::map<int, INVASION_CLIENT_INFO> m_InvasionInfo;
	int m_CurrentIndex;
	bool m_OpenTable;
	DWORD m_LastTick;
	DWORD m_ClickTick;
	bool m_CustomPosition;
	bool m_Dragging;
	bool m_DragMoved;
	float m_FrameX;
	float m_FrameY;
	float m_DragStartX;
	float m_DragStartY;
	float m_DragLastX;
	float m_DragLastY;
	float m_MouseX;
	float m_MouseY;
};

extern CInvasionInfo gInvasionInfo;
