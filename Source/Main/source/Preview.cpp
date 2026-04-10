#include "stdafx.h"
#include "Preview.h"
#include "Console.h"

CCustomPreview gCustomPreview;

CCustomPreview::CCustomPreview()
{
	ClearCustomPreviewCharList();

	ClearCustomPreviewList(0);
}

CCustomPreview::~CCustomPreview()
{

}

void CCustomPreview::ClearCustomPreviewCharList()
{
	for (int i = 0; i < MAX_CUSTOM_PREVIEW_CHAR; i++)
	{
		m_CustomPreviewCharList[i].Reset();
	}
}

void CCustomPreview::ClearCustomPreviewList(int slot)
{
	for (int i = slot; i < MAX_CUSTOM_PREVIEW; i++)
	{
		m_CustomPreviewList[i].Reset();
	}
}

void CCustomPreview::InsertCustomPreviewCharList(int slot, char* name, int pet, int wing)
{
	m_CustomPreviewCharList[slot].PetIndex = pet;
	m_CustomPreviewCharList[slot].WingIndex = wing;
	m_CustomPreviewCharList[slot].slot = slot;

	std::memcpy(m_CustomPreviewCharList[slot].name, name, sizeof(m_CustomPreviewCharList[slot].name));
}

CUSTOM_PET_VIEW_CHAR_LIST* CCustomPreview::GetCustomPreviewCharList(char* name)
{
	for (int i = 0; i < MAX_CUSTOM_PREVIEW_CHAR; i++)
	{
		if (strncmp(m_CustomPreviewCharList[i].name, name, sizeof(m_CustomPreviewCharList[i].name)) == 0)
		{
			return &m_CustomPreviewCharList[i];
		}
	}

	return 0;
}

void CCustomPreview::InsertCustomPreviewList(int slot, char* name, int pet, int wing, WORD index, WORD secondpet, WORD Element[2])
{
	for (int i = 0; i < MAX_CUSTOM_PREVIEW; i++)
	{
		if (m_CustomPreviewList[i].index != 0xFFFF)
		{
			continue;
		}

		m_CustomPreviewList[i].PetIndex = pet;
		m_CustomPreviewList[i].SecondPetIndex = secondpet;
		m_CustomPreviewList[i].WingIndex = wing;
		m_CustomPreviewList[i].slot = i;
		m_CustomPreviewList[i].index = index;
		m_CustomPreviewList[i].Element[0] = Element[0];
		m_CustomPreviewList[i].Element[1] = Element[1];

		std::memcpy(m_CustomPreviewList[i].name, name, sizeof(m_CustomPreviewList[i].name));
		return;
	}
}

CUSTOM_PET_VIEW_LIST* CCustomPreview::GetCustomPreviewList(int index)
{
	for (int i = 0; i < MAX_CUSTOM_PREVIEW; i++)
	{
		if (m_CustomPreviewList[i].index == 0xFFFF)
		{
			continue;
		}

		if (m_CustomPreviewList[i].index == index)
		{
			return &m_CustomPreviewList[i];
		}
	}

	return 0;
}

void CCustomPreview::DeleteCustomPreview(int index)
{
	for (int i = 0; i < MAX_CUSTOM_PREVIEW; i++)
	{
		if (m_CustomPreviewList[i].index == 0xFFFF)
		{
			continue;
		}

		if (m_CustomPreviewList[i].index == index)
		{
			m_CustomPreviewList[i].Reset();
		}
	}
}
