#pragma once

#include "BoneManager.h"
#include "MapManager.h"
#include "ZzzEffect.h"
#include "ZzzTexture.h"

// Texture slots reserved for map-effect resources loaded by CustomWorld.
#define BITMAP_map_texture01 32600
#define BITMAP_map_texture02 32601
#define BITMAP_map_texture03 32602
#define BITMAP_map_texture04 32603
#define BITMAP_map_texture05 32604
#define BITMAP_map_texture06 32605
#define BITMAP_map_texture07 32606
#define BITMAP_map_texture08 32607
#define BITMAP_map_texture09 32608

#define World (gMapManager.WorldActive)

inline int CreateParticleSync(int type, vec3_t position, vec3_t angle, vec3_t light, int subType = 0, float scale = 1.0f, OBJECT* owner = NULL)
{
	return CreateParticleFpsChecked(type, position, angle, light, subType, scale, owner);
}

inline float timefac(float value)
{
	return value * FPS_ANIMATION_FACTOR;
}

inline float Render22(float speed, float phase)
{
	return (sinf((WorldTime + phase) * speed * 0.001f) + 1.0f) * 0.5f;
}

inline float pos_right(float value)
{
	return value;
}

inline float pos_botton(float value)
{
	return value;
}

inline float sub_11B5EE9(float minimum, float maximum, unsigned int)
{
	return minimum + (float)(rand() % ((int)(maximum - minimum) + 1));
}

inline int CustomWorldFindBone(OBJECT* object, BMD*, const char* name)
{
	int bone = BoneManager::GetBoneNumber(object, name);
	return (bone < 0) ? 0 : bone;
}
