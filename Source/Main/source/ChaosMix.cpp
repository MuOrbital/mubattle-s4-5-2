#include "stdafx.h"
#include "ChaosMix.h"
#include "UIControls.h"
#include "MixMgr.h"
#include "wsclientinline.h"

CChaosMix::CChaosMix()
{
	this->MixRate = 0;
	this->MixMoney = 0;
	this->MixTax = 0;
}

CChaosMix::~CChaosMix()
{

}

void CChaosMix::ChaosBoxMainProc() // OK
{
	if (this->ChaosBoxMixCheck() != 0)
	{
		SendRequestChaosMix(g_MixRecipeMgr.GetCurMixID());
	}
}

bool CChaosMix::ChaosBoxMixCheck() // OK
{
	if (g_MixRecipeMgr.GetCurMixID() > 57)
	{
		return 1;
	}

	switch (g_MixRecipeMgr.GetCurMixID())
		{
		case CHAOS_MIX_CHAOS_ITEM:
		case CHAOS_MIX_DEVIL_SQUARE:
		case CHAOS_MIX_PLUS_ITEM_LEVEL1:
		case CHAOS_MIX_PLUS_ITEM_LEVEL2:
		case CHAOS_MIX_DINORANT:
		case CHAOS_MIX_FRUIT:
		case CHAOS_MIX_WING1:
		case CHAOS_MIX_BLOOD_CASTLE:
		case CHAOS_MIX_WING2:
		case CHAOS_MIX_PET1:
		case CHAOS_MIX_PET2:
		case CHAOS_MIX_PLUS_ITEM_LEVEL3:
		case CHAOS_MIX_PLUS_ITEM_LEVEL4:
		case CHAOS_MIX_WING3:
		case CHAOS_MIX_PIECE_OF_HORN:
		case CHAOS_MIX_BROKEN_HORN:
		case CHAOS_MIX_HORN_OF_FENRIR:
		case CHAOS_MIX_HORN_OF_FENRIR_UPGRADE:
		case CHAOS_MIX_SHIELD_POTION1:
		case CHAOS_MIX_SHIELD_POTION2:
		case CHAOS_MIX_SHIELD_POTION3:
		case CHAOS_MIX_JEWEL_OF_HARMONY_ITEM_PURITY:
		case CHAOS_MIX_JEWEL_OF_HARMONY_ITEM_SMELT:
		case CHAOS_MIX_ITEM_380:
		case CHAOS_MIX_ILLUSION_TEMPLE:
		case CHAOS_MIX_FEATHER_OF_CONDOR:
		case CHAOS_MIX_WING4:
		case CHAOS_MIX_SOCKET_ITEM_CREATE_SEED:
		case CHAOS_MIX_SOCKET_ITEM_CREATE_SEED_SPHERE:
		case CHAOS_MIX_PLUS_ITEM_LEVEL5:
		case CHAOS_MIX_PLUS_ITEM_LEVEL6:
		case CHAOS_MIX_LUCKY_ITEM_REFINE:
			return 1;
	}

	return 0;
}