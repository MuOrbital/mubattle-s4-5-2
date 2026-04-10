#pragma once

#define MAX_CUSTOM_PREVIEW_CHAR 10
#define MAX_CUSTOM_PREVIEW 75

struct CUSTOM_PET_VIEW_CHAR_LIST
{
	char name[10];
	int PetIndex;
	int WingIndex;
	BYTE slot;

	void Reset()
	{
		memset(name, 0, sizeof(name));
		PetIndex = -1;
		WingIndex = -1;
		slot = 0xFF;
	};
};

struct CUSTOM_PET_VIEW_LIST
{
	char name[11];
	int PetIndex;
	int SecondPetIndex;
	int WingIndex;
	WORD Element[2];
	BYTE slot;
	WORD index;

	void Reset()
	{
		memset(name, 0, sizeof(name));
		PetIndex = -1;
		SecondPetIndex = 0;
		WingIndex = -1;
		slot = 0xFF;
		index = 0xFFFF;

		Element[0] = 0;
		Element[1] = 0;
	};
};

class CCustomPreview
{
public:
	CCustomPreview();
	virtual ~CCustomPreview();

	void ClearCustomPreviewCharList();
	void InsertCustomPreviewCharList(int slot, char* name, int pet, int wing);
	CUSTOM_PET_VIEW_CHAR_LIST* GetCustomPreviewCharList(char* name);

	void ClearCustomPreviewList(int slot);
	void InsertCustomPreviewList(int slot, char* name, int pet, int wing, WORD index, WORD secondpet, WORD Element[2]);
	CUSTOM_PET_VIEW_LIST* GetCustomPreviewList(int index);
	void DeleteCustomPreview(int index);

private:
	CUSTOM_PET_VIEW_CHAR_LIST m_CustomPreviewCharList[MAX_CUSTOM_PREVIEW_CHAR];
	CUSTOM_PET_VIEW_LIST m_CustomPreviewList[MAX_CUSTOM_PREVIEW];
};

extern CCustomPreview gCustomPreview;