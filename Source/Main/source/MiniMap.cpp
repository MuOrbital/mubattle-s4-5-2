#include "stdafx.h"
#include "MiniMap.h"
#include "MapManager.h"
#include "NewUISystem.h"

bool MiniMapTable[MAX_MINI_MAP];

CMiniMap::CMiniMap() // OK
{
	memset(MiniMapTable, 0, sizeof(MiniMapTable));
}

CMiniMap::~CMiniMap() // OK
{

}


void CMiniMap::MiniMapCore() // OK
{
	((void(*)())0x005D2F16)();

	MiniMapLoad();
}

void CMiniMap::MiniMapLoad() // OK
{
	char buff[64];

	wsprintf(buff, "Custom\\MiniMaps\\World%d.jpg", (gMapManager.WorldActive + 1));

	if (MiniMapFileCheck(gMapManager.WorldActive) != 0)
	{
		MiniMapTable[gMapManager.WorldActive] = 1;

		LoadBitmap(buff, SEASON3B::CNewUISiegeWarBase::IMAGE_LIST::IMAGE_MINIMAP, GL_LINEAR);
	}
}

bool CMiniMap::MiniMapCheck(int map) // OK
{
	if (MiniMapTable[map] != 0)
	{
		return 1;
	}
	else
	{
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_SIEGEWARFARE);
		return 0;
	}
}

bool CMiniMap::MiniMapFileCheck(int map) // OK
{
	char buff[64];

	wsprintf(buff, ".\\Data\\Custom\\MiniMaps\\World%d.ozj", (map + 1));

	FILE* file;

	if (fopen_s(&file, buff, "r") != 0)
	{
		return 0;
	}
	else
	{
		fclose(file);
		return 1;
	}
}
