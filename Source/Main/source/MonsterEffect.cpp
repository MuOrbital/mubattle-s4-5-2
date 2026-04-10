#include "stdafx.h"
#include "MonsterEffect.h"
#include "LuaStack.hpp"
#include "Console.h"
#include <ZzzEffect.h>

using namespace luaaa;

CMonsterEffect gMonsterEffect;

CMonsterEffect::CMonsterEffect() : m_Lua()
{
	this->m_MonsterEffect.clear();
}

CMonsterEffect::~CMonsterEffect()
{
	this->m_MonsterEffect.clear();
}

void SetMonsterEffect(int MonsterID, int Type, int EffectID, int EffectLv, int Bone, float Size, float R, float G, float B, float Black, int Rand)
{
	int MonsterModel = MonsterID + 574;

	CUSTOM_MONSTER_EFFECT monsterInfo;

	monsterInfo.MonsterID = MonsterModel;

	CUSTOM_MONSTER_EFFECT_INFO glowInfo;

	glowInfo.Type = Type;

	glowInfo.EffectID = EffectID;

	glowInfo.EffectLv = EffectLv;

	glowInfo.Bone = Bone;

	glowInfo.Size = Size;

	glowInfo.R = R;

	glowInfo.G = G;

	glowInfo.B = B;

	glowInfo.Black = Black;

	glowInfo.Rand = Rand;

	auto it = gMonsterEffect.m_MonsterEffect.find(MonsterModel);

	if (it == gMonsterEffect.m_MonsterEffect.end())
	{
		monsterInfo.m_Effects.push_back(glowInfo);

		gMonsterEffect.m_MonsterEffect.insert(std::make_pair(MonsterModel, monsterInfo));
	}
	else
	{
		it->second.m_Effects.push_back(glowInfo);
	}
}

void CMonsterEffect::CreateEffectMonster(OBJECT* ObjectStruct, BMD* pModel)
{
	if (ObjectStruct == 0 || pModel == 0)
	{
		return;
	}

	auto monsterEffect = gMonsterEffect.getMonsterEffect(ObjectStruct->Type);

	if (monsterEffect != nullptr)
	{
		vec3_t p, Position, Light;

		for (const auto& effect : monsterEffect->m_Effects)
		{
			vec34_t* BoneTransform = &ObjectStruct->BoneTransform[effect.Bone];

			if (BoneTransform)
			{
				if (effect.Rand != 100)
				{
					if ((rand() % 100) > effect.Rand)
					{
						continue;
					}
				}

				Vector(0.0f, 0.0f, 0.0f, p);

				Vector(effect.R, effect.G, effect.B, Light);

				pModel->TransformPosition(BoneTransform[0], p, Position, 1);

				if (effect.Type == 1)
				{
					CreateParticleFpsChecked(effect.EffectID, Position, ObjectStruct->Angle, Light, effect.EffectLv, effect.Size, ObjectStruct);
				}
				else
				{
					CreateSprite(effect.EffectID, Position, effect.Size, Light, ObjectStruct, effect.Black, 0);
				}
			}
		}
	}
}


void CMonsterEffect::Init()
{
	LuaModule(this->m_Lua.GetState()).fun("SetMonsterEffect", SetMonsterEffect);

	this->m_Lua.DoFile("Data//Lua//Monster//CustomMonsterEffect.lua");
}

CUSTOM_MONSTER_EFFECT* CMonsterEffect::getMonsterEffect(int Type)
{
	auto it = this->m_MonsterEffect.find(Type);

	if (it == this->m_MonsterEffect.end())
	{
		return nullptr;
	}

	return &it->second;
}
