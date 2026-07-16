#pragma once

// MuBattle exposes exactly six playable base classes (DW, DK, ELF, MG, DL and SUM).
#ifndef MAX_CLASS_PLAYER
#define MAX_CLASS_PLAYER 6
#endif

// Compatibility names used by the 5.2 Helper sources.  Keeping them here avoids
// changing the shared class/skill enums used by the rest of the MuBattle client.
#define Dark_Wizard CLASS_WIZARD
#define Dark_Knight CLASS_KNIGHT
#define Fairy_Elf CLASS_ELF
#define Magic_Gladiator CLASS_DARK
#define Dark_Lord CLASS_DARK_LORD
#define Summoner CLASS_SUMMONER
// Kept only as an unreachable compatibility key. MuBattle has no Rage Fighter.
#define Rage_Fighter 6
#define Skill_Heal AT_SKILL_HEALING
#define Skill_Heal_Strengthener AT_SKILL_HEAL_UP
#define Skill_Drain_Life AT_SKILL_ALICE_DRAINLIFE
#define Skill_Drain_Life_Strengthener AT_SKILL_ALICE_DRAINLIFE_UP
#define PLAYER_FENRIR_WALK_WEAPON_LEFT PLAYER_FENRIR_WALK_ONE_LEFT

#ifndef WIN_WINDOW_SIZEX
#define WIN_WINDOW_SIZEX 190
#endif
#ifndef WIN_WINDOW_SIZEY
#define WIN_WINDOW_SIZEY 429
#endif

// Kept outside the legacy ANSI enum files so the port does not renumber existing MuBattle UI IDs.
// The values are intentionally in private ranges unused by the original interface manager.
namespace SEASON3B
{
	enum MUHELPER_INTERFACE_LIST
	{
		INTERFACE_MACRO_OFICIAL = 0x7000,
		INTERFACE_MACRO_OFICIAL_SUB,
		INTERFACE_MACRO_PROGRESS,
	};
}

enum MUHELPER_TEXTURE_LIST
{
	// Dynamic/custom bitmap IDs are map-backed in this client. This private
	// range avoids every sequential value in _TextureIndex.h (up to 33000).
	BITMAP_MACRO_MAIN_BEGIN = 542100,
	BITMAP_GAUGE_FRAME = BITMAP_MACRO_MAIN_BEGIN + 4,
	BITMAP_GAUGE_FRAME_B,
	BITMAP_GAUGE_FRAME_G,
	BITMAP_GAUGE_FRAME_Y,
	BITMAP_GAUGE_FRAME_O,
	BITMAP_GAUGE_FRAME_R,
	BITMAP_AUTO_AREA01_R,
	BITMAP_AUTO_AREA02_R,
	BITMAP_MUHELPER_POSITION_A = BITMAP_MACRO_MAIN_BEGIN + 20,
	BITMAP_MUHELPER_POSITION_B,
	BITMAP_MUHELPER_POSITION_C,
	BITMAP_MUHELPER_SETUP,
	BITMAP_MUHELPER_START,
	BITMAP_MUHELPER_STOP,
	BITMAP_MACRO_MAIN_END,
};
