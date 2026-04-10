#include "stdafx.h"
#include "NProtocol.h"
#include "Preview.h"
#include "Console.h"

void GCCustomPreviewCharListRecv(PMSG_CUSTOM_PREVIEW_CHAR_LIST_RECV* lpMsg)
{
	gCustomPreview.ClearCustomPreviewCharList();

	for (int n = 0; n < lpMsg->count; n++)
	{
		PMSG_CUSTOM_PREVIEW_CHAR_LIST* lpInfo = (PMSG_CUSTOM_PREVIEW_CHAR_LIST*)(((BYTE*)lpMsg) + sizeof(PMSG_CUSTOM_PREVIEW_CHAR_LIST_RECV) + (sizeof(PMSG_CUSTOM_PREVIEW_CHAR_LIST) * n));

		gCustomPreview.InsertCustomPreviewCharList(n, lpInfo->Name, lpInfo->PetIndex, lpInfo->WingIndex);

		Console.Write(1, "TESTE 1 - %d", lpInfo->WingIndex);
	}
}

void GCCustomPreviewCharRecv(PMSG_CUSTOM_PREVIEW_CHARSET_RECV* lpMsg)
{
	if (lpMsg->Viewport == 1)
	{
		gCustomPreview.ClearCustomPreviewList(1);

		for (int n = 0; n < lpMsg->count; n++)
		{
			PMSG_CUSTOM_PREVIEW_CHARSET_LIST* lpInfo = (PMSG_CUSTOM_PREVIEW_CHARSET_LIST*)(((BYTE*)lpMsg) + sizeof(PMSG_CUSTOM_PREVIEW_CHARSET_RECV) + (sizeof(PMSG_CUSTOM_PREVIEW_CHARSET_LIST) * n));

			gCustomPreview.InsertCustomPreviewList(n + 1, lpInfo->Name, lpInfo->PetIndex, lpInfo->WingIndex, lpInfo->index, lpInfo->SecondPetIndex, lpInfo->ElementSlot);


			Console.Write(1, "TESTE 2 - %d", lpInfo->WingIndex);
		}
	}
	else
	{
		gCustomPreview.ClearCustomPreviewList(0);

		for (int n = 0; n < lpMsg->count; n++)
		{
			PMSG_CUSTOM_PREVIEW_CHARSET_LIST* lpInfo = (PMSG_CUSTOM_PREVIEW_CHARSET_LIST*)(((BYTE*)lpMsg) + sizeof(PMSG_CUSTOM_PREVIEW_CHARSET_RECV) + (sizeof(PMSG_CUSTOM_PREVIEW_CHARSET_LIST) * n));

			gCustomPreview.InsertCustomPreviewList(n, lpInfo->Name, lpInfo->PetIndex, lpInfo->WingIndex, lpInfo->index, lpInfo->SecondPetIndex, lpInfo->ElementSlot);


			Console.Write(1, "TESTE 3 - %d", lpInfo->WingIndex);
		}
	}
}