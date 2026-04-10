#pragma once

#include "Packets.h"

#define SET_NUMBERHB(x) ((BYTE)((DWORD)(x)>>(DWORD)8))
#define SET_NUMBERLB(x) ((BYTE)((DWORD)(x)&0xFF))

//struct PSWMSG_HEAD
//{
//	void set(BYTE head, BYTE subh, WORD size) // OK
//	{
//		this->type = 0xC2;
//		this->size[0] = SET_NUMBERHB(size);
//		this->size[1] = SET_NUMBERLB(size);
//		this->head = head;
//		this->subh = subh;
//	}
//
//	void setE(BYTE head, BYTE subh, WORD size) // OK
//	{
//		this->type = 0xC4;
//		this->size[0] = SET_NUMBERHB(size);
//		this->size[1] = SET_NUMBERLB(size);
//		this->head = head;
//		this->subh = subh;
//	}
//
//	BYTE type;
//	BYTE size[2];
//	BYTE head;
//	BYTE subh;
//};

struct PMSG_CUSTOM_PREVIEW_CHAR_LIST_RECV
{
	PSWMSG_HEAD header; // C1:F3:00
	BYTE count;
};

struct PMSG_CUSTOM_PREVIEW_CHAR_LIST
{
	char Name[10];
	WORD PetIndex;
	WORD WingIndex;
};

struct PMSG_CUSTOM_PREVIEW_CHARSET_RECV
{
	PSWMSG_HEAD header; // C1:F3:00
	BYTE count;
	BYTE Viewport;
};

struct PMSG_CUSTOM_PREVIEW_CHARSET_LIST
{
	char Name[11];
	WORD PetIndex;
	WORD SecondPetIndex;
	WORD WingIndex;
	WORD index;
	WORD ElementSlot[2];
};

//**********************************************//
//*********** GameServer -> Client  ************//
//**********************************************//
struct SDHP_CHAOS_BOX_GENESIS_OPEN_CLIENT_RECV
{
	PSBMSG_HEAD header; //
	int state;
	int selectedKey;
	int selectedComb;
};

struct PMSG_CHAOS_BOX_GENESIS_LIST_RECV
{
	NEW_PSWMSG_HEAD header; //
	BYTE count;
};

struct PMSG_CHAOS_BOX_GENESIS_ITEM_INFO
{
	BYTE slot;
	BYTE ItemInfo[12];
};

struct PMSG_CHAOS_BOX_GENESIS_STATE_RECV
{
	PSBMSG_HEAD header; //
	int state;
};

//struct PMSG_ITEM_COUNTER_RECV
//{
//	NEW_PSWMSG_HEAD h;
//	BYTE count;
//	BYTE Type;
//};

//struct PMSG_ITEM_COUNTER_INFO
//{
//	WORD ItemIndex;
//	BYTE Slot;
//	BYTE Top;
//	DWORD kills;
//};

//**********************************************//
//*********** Client -> GameServer  ************//
//**********************************************//
struct SDHP_CHAOS_BOX_GENESIS_SET_INFO_SEND
{
	PSBMSG_HEAD header; //
	int state;
	int selectedKey;
	int selectedComb;
};

struct SDHP_CHAOS_BOX_GENESIS_MIX_SEND
{
	PSBMSG_HEAD header; //
	int state;
};

//struct CUSTOM_JEWEL_STACK_INFO_SEND
//{
//	PSBMSG_HEAD header;
//	BYTE slot;
//	BYTE stack;
//};

void GCCustomPreviewCharListRecv(PMSG_CUSTOM_PREVIEW_CHAR_LIST_RECV* lpMsg);
void GCCustomPreviewCharRecv(PMSG_CUSTOM_PREVIEW_CHARSET_RECV* lpMsg);