#include "StdAfx.h"
#include "HelperView.h"
#include "HelperSystem.h"
#include "Console.h"
#include "ZzzObject.h"
#include "ZzzLodTerrain.h"
#include "ZzzAI.h"
#include "MapManager.h"
#include "ZzzBMD.h"
#include "ZzzEffect.h"
#include "CDirection.h"
#include "ZzzCharacter.h"
#include "GOBoid.h"

enum eActionType
{
	ACTION_STAND = 0,
	ACTION_WALK = 0,
	ACTION_RUN = 0,
	ACTION_ATTACK1 = 3,
	ACTION_ATTACK2 = 4,
	ACTION_STUCK = 5,
	ACTION_DEAD = 6,
};

static bool IsAnimationFrameRange(OBJECT* ObjectStruct, float BeginFrame, float EndFrame)
{
	if (ObjectStruct == nullptr)
	{
		return false;
	}

	if (ObjectStruct->AnimationFrame >= ObjectStruct->PriorAnimationFrame)
	{
		return (ObjectStruct->PriorAnimationFrame <= EndFrame && ObjectStruct->AnimationFrame >= BeginFrame);
	}

	return (ObjectStruct->AnimationFrame >= BeginFrame && ObjectStruct->AnimationFrame <= EndFrame);
}

HELPER_VIEWPtr HELPER_VIEW::Make()
{
	HELPER_VIEWPtr HelperProcess(new HELPER_VIEW);
	HelperProcess->Init();
	return HelperProcess;
}

bool HELPER_VIEW::Create(int itemType, int modelType, vec3_t Position, CHARACTER* Owner, int SubType)
{

	if (this->m_Obj == nullptr)
	{
		return 0;
	}

	if (this->m_Obj->Live)
	{
		return 0;
	}

	if (gMapManager.InChaosCastle() == true)
	{
		return false;
	}

	m_startTick = GetTickCount();

	m_pOwner = Owner;
	m_itemType = itemType + MODEL_ITEM;

	this->m_Obj->Type = modelType;
	this->m_Obj->Live = true;
	this->m_Obj->Visible = false;
	this->m_Obj->LightEnable = true;
	this->m_Obj->ContrastEnable = false;
	this->m_Obj->AlphaEnable = false;
	this->m_Obj->EnableBoneMatrix = false;
	this->m_Obj->Owner = &m_pOwner->Object;
	this->m_Obj->SubType = SubType;
	this->m_Obj->HiddenMesh = -1;
	this->m_Obj->BlendMesh = -1;
	this->m_Obj->BlendMeshLight = 1.f;
	this->m_Obj->Scale = 0.7f;
	this->m_Obj->LifeTime = 30;
	this->m_Obj->Alpha = 0.f;
	this->m_Obj->AlphaTarget = 1.f;

	VectorCopy(Position, this->m_Obj->Position);
	VectorCopy(this->m_Obj->Owner->Angle, this->m_Obj->Angle);
	Vector(3.f, 3.f, 3.f, this->m_Obj->Light);

	this->m_Obj->PriorAnimationFrame = 0.f;
	this->m_Obj->AnimationFrame = 0.f;
	this->m_Obj->Velocity = 0.5f;

	return 1;
}

void HELPER_VIEW::Render()
{
	if (m_Obj == nullptr || m_Obj->Owner == nullptr)
	{
		return;
	}

	if (!m_Obj->Live)
	{
		return;
	}

	int Effect = 0;

	if(g_isCharacterBuff(m_Obj->Owner, eBuff_Cloaking))
	{
		Effect = 10;
	}

	m_Obj->Visible = TestFrustrum2D(m_Obj->Position[0] * 0.010f, m_Obj->Position[1] * 0.01f, -20.0f);

	if (!m_Obj->Visible)
	{
		return;
	}

	RenderObject(m_Obj, 0, 0, Effect);

	if (m_Type > 0)
	{
		WORD safe = (TerrainWall[TERRAIN_INDEX_REPEAT((int)(m_Obj->Owner->Position[0] / TERRAIN_SCALE), (int)(m_Obj->Owner->Position[1] / TERRAIN_SCALE))] & TW_SAFEZONE);

		if (safe == 1)
		{
			return;
		}
	}

	BMD* b = &Models[m_Obj->Type];

	if (b) 
	{
		gHelperSystem.m_Lua.Generic_Call("MoveEffects", "iii>", b, (DWORD)(m_Obj), m_Obj->Type);
		return;
	}
}

void HELPER_VIEW::Update()
{
	if (m_Obj == nullptr)
	{
		return;
	}

	if (!m_Obj->Live || m_Obj->Owner == nullptr)
	{
		m_Obj->Live = 0;
		return;
	}

	if (!m_Obj->Owner->Live || m_Obj->Owner->Kind != KIND_PLAYER)
	{
		m_Obj->Live = 0;
		return;
	}

	Alpha(m_Obj);

	BMD* b = &Models[m_Obj->Type];

	if (b)
	{
		UpdateMove(m_Obj);

		UpdateModel(m_Obj);
	}
}

void HELPER_VIEW::UpdateModel(OBJECT* model)
{
	BMD* b = &Models[m_Obj->Type];

	b->CurrentAction = model->CurrentAction;
	b->PlayAnimation(&model->AnimationFrame, &model->PriorAnimationFrame, &model->PriorAction, model->Velocity, model->Position, model->Angle);
}

void HELPER_VIEW::WorkMoviment(OBJECT* ObjectStruct, float MaxPos, int type, int movement, int state)
{
	BMD* b = &Models[ObjectStruct->Type];

	vec3_t p, Position;
	Vector(0.f, 0.f, 0.f, p);

	vec3_t Color;
	Vector(0.f, 0.f, 0.f, Color);

	float OwnerPosition0;
	float OwnerPosition1;
	float Position0;
	float Position1;
	float PetPosition;
	float Angle;
	float SetDirection;

	if (type == 0)
	{
		if (movement == 0) // Uma fada estranha
		{
			MaxPos = 150.0f;

			OwnerPosition0 = ObjectStruct->Owner->Position[0];
			OwnerPosition1 = ObjectStruct->Owner->Position[1];
			Position0 = ObjectStruct->Position[0];
			Position1 = ObjectStruct->Position[1];

			PetPosition = (OwnerPosition1 - Position1) * (OwnerPosition1 - Position1) + (OwnerPosition0 - Position0) * (OwnerPosition0 - Position0);

			if (PetPosition >= (MaxPos * MaxPos))
			{
				Angle = CreateAngle(Position0, Position1, OwnerPosition0, OwnerPosition1);
				ObjectStruct->Angle[2] = TurnAngle2(ObjectStruct->Angle[2], Angle, 20.0);
			}

			AngleMatrix(ObjectStruct->Angle, ObjectStruct->Matrix);
			VectorRotate(ObjectStruct->Direction, ObjectStruct->Matrix, Position);

			VectorAddScaled(ObjectStruct->Position, Position, ObjectStruct->Position, FPS_ANIMATION_FACTOR);

			ObjectStruct->Position[2] += (float)(rand() % 16 - 8) * FPS_ANIMATION_FACTOR;

			if (rand_fps_check(32))
			{
				if (PetPosition < MaxPos * MaxPos)
				{
					SetDirection = -(float)(rand() % 64 + 16) * 0.10f;
					ObjectStruct->Angle[2] = (float)(rand() % 360);
				}
				else
				{
					SetDirection = -(float)(rand() % 64 + 128) * 0.10f;
				}

				ObjectStruct->Direction[0] = 0.0f;
				ObjectStruct->Direction[1] = SetDirection;
				ObjectStruct->Direction[2] = (float)(rand() % 64 - 32) * 0.1f;
			}

			if (ObjectStruct->Owner->Position[2] + 150.0 > ObjectStruct->Position[2])
			{
				ObjectStruct->Direction[2] = ObjectStruct->Direction[2] + 1.5f;
			}
			if (ObjectStruct->Owner->Position[2] + 200.0 < ObjectStruct->Position[2])
			{
				ObjectStruct->Direction[2] = ObjectStruct->Direction[2] - 1.5f;
			}

			ObjectStruct->CurrentAction = 0;
		}
		else if (movement == 2) // Andando
		{
			vec3_t OwnerPos;
			float Angle;

			if (SceneFlag == 4)
			{
				ObjectStruct->Position[1] = ObjectStruct->Owner->Position[1] + 180.f;
			}

			ObjectStruct->Position[2] = ObjectStruct->Owner->Scale * 230.0f + ObjectStruct->Owner->Position[2];

			Vector(ObjectStruct->Owner->Position[0] - ObjectStruct->Position[0], ObjectStruct->Owner->Position[1] - ObjectStruct->Position[1], ObjectStruct->Owner->Position[2] - ObjectStruct->Position[2], OwnerPos);

			float PetMap = OwnerPos[0] * OwnerPos[0] + OwnerPos[1] * OwnerPos[1];

			bool Walking = 0;

			if (PetMap < MaxPos * MaxPos * MaxPos * MaxPos)
			{
				if (PetMap >= MaxPos * MaxPos)
				{
					Angle = CreateAngle(ObjectStruct->Position[0], ObjectStruct->Position[1], ObjectStruct->Owner->Position[0], ObjectStruct->Owner->Position[1]);
					ObjectStruct->Angle[2] = TurnAngle2(ObjectStruct->Angle[2], Angle, 10.0);

					Walking = 1;
					ObjectStruct->CurrentAction = 2; //Setar correr
				}
			}
			else
			{
				ObjectStruct->Position[0] = ObjectStruct->Owner->Position[0];
				ObjectStruct->Position[1] = ObjectStruct->Owner->Position[1];
				ObjectStruct->Position[2] = ObjectStruct->Owner->Position[2];
				ObjectStruct->Position[2] = ObjectStruct->Owner->Position[2] + 200.0f;
				ObjectStruct->Angle[0] = ObjectStruct->Owner->Angle[0];
				ObjectStruct->Angle[1] = ObjectStruct->Owner->Angle[1];
				ObjectStruct->Angle[2] = ObjectStruct->Owner->Angle[2];
			}

			vec3_t VecPos;

			AngleMatrix(ObjectStruct->Angle, ObjectStruct->Matrix);
			VectorRotate(ObjectStruct->Direction, ObjectStruct->Matrix, VecPos);

			VectorAddScaled(ObjectStruct->Position, VecPos, ObjectStruct->Position, FPS_ANIMATION_FACTOR);

			float Velocity = 0.0;

			if (PetMap > MaxPos * MaxPos)
			{
				if (PetMap >= (MaxPos * 2) * (MaxPos * 2))
				{
					Velocity = log(PetMap) * 1.30f;
				}
				else if (PetMap >= (MaxPos + (MaxPos / 2)) * (MaxPos + (MaxPos / 2)))
				{
					Velocity = log(PetMap) * 1.20f;
				}
				else
				{
					Velocity = log(PetMap) * 1.05f;
				}
			}

			ObjectStruct->Direction[0] = 0.0f;
			ObjectStruct->Direction[1] = -Velocity;
			ObjectStruct->Direction[2] = 0.0f;

			if (!Walking)
			{
				ObjectStruct->CurrentAction = 0;
			}

			if (SceneFlag == 4)
			{
				ObjectStruct->CurrentAction = 0; //Setar correr
			}
		}
		else // Voando
		{
			bool AutoMove = 0;
			static float LostPostion = 1000.0f;

			vec3_t OwnerPos;

			Vector(ObjectStruct->Owner->Position[0], ObjectStruct->Owner->Position[1], ObjectStruct->Owner->Position[2], OwnerPos);

			vec3_t PetPos;

			Vector(ObjectStruct->Position[0], ObjectStruct->Position[1], ObjectStruct->Position[2], PetPos);

			vec3_t d;

			Vector(OwnerPos[0] - PetPos[0], OwnerPos[1] - PetPos[1], OwnerPos[2] - PetPos[2], d);

			float PetMap = d[0] * d[0] + d[1] * d[1];

			float dir1 = ((OwnerPos[0] > PetPos[0]) ? OwnerPos[0] - PetPos[0] : PetPos[0] - OwnerPos[0]);
			float dir2 = ((OwnerPos[1] > PetPos[1]) ? OwnerPos[1] - PetPos[1] : PetPos[1] - OwnerPos[1]);

			float angle = atan2(dir1, dir2) * 180.0f / 3.14159265359f;

			vec3_t VecPos, Random;

			if (PetMap < MaxPos * MaxPos * MaxPos * MaxPos)
			{
				if (PetMap >= MaxPos * MaxPos)
				{
					if (ObjectStruct->Position[0] < ObjectStruct->Owner->Position[0] - 1.0f && ObjectStruct->Position[1] < ObjectStruct->Owner->Position[1] - 1.0f)
					{
						ObjectStruct->Angle[2] = 180.0;//v->
						ObjectStruct->Angle[2] -= angle;//v->
					}
					else if (ObjectStruct->Position[0] < ObjectStruct->Owner->Position[0] - 1.0f && ObjectStruct->Position[1] > ObjectStruct->Owner->Position[1] - 1.0f)
					{
						ObjectStruct->Angle[2] = 0.0;//v->
						ObjectStruct->Angle[2] += angle;//v->
					}
					else if (ObjectStruct->Position[0] > ObjectStruct->Owner->Position[0] - 1.0f && ObjectStruct->Position[1] > ObjectStruct->Owner->Position[1] - 1.0f)
					{
						ObjectStruct->Angle[2] = 360.0;//v->
						ObjectStruct->Angle[2] -= angle;//v->
					}
					else if (ObjectStruct->Position[0] > ObjectStruct->Owner->Position[0] - 1.0f && ObjectStruct->Position[1] < ObjectStruct->Owner->Position[1] - 1.0f)
					{
						ObjectStruct->Angle[2] = 180.0;//v->
						ObjectStruct->Angle[2] += angle;//v->
					}

					ObjectStruct->CurrentAction = ACTION_RUN;
				}
				else
				{
					if (AutoMove)
					{
						float t = (float)(timeGetTime() % 4001) * 0.003140000160783529f;

						Random[0] = cos(t) * 50.0f + ObjectStruct->Owner->Position[0];

						Random[1] = sin(t) * 50.0f + ObjectStruct->Owner->Position[1];

						Random[2] = ObjectStruct->Owner->Position[2];

						float a = CreateAngle(ObjectStruct->Position[0], ObjectStruct->Position[1], Random[0], Random[1]);

						ObjectStruct->Angle[2] = TurnAngle2(ObjectStruct->Angle[2], a, 8.0f);
					}
					else
					{
						ObjectStruct->CurrentAction = ACTION_STAND;
					}
				}
			}
			else
			{
				ObjectStruct->Position[0] = ObjectStruct->Owner->Position[0];
				ObjectStruct->Position[1] = ObjectStruct->Owner->Position[1];
				ObjectStruct->Position[2] = ObjectStruct->Owner->Position[2];
				ObjectStruct->Angle[0] = ObjectStruct->Owner->Angle[0];
				ObjectStruct->Angle[1] = ObjectStruct->Owner->Angle[1];
				ObjectStruct->Angle[2] = ObjectStruct->Owner->Angle[2];
			}

			AngleMatrix(ObjectStruct->Angle, ObjectStruct->Matrix);

			VectorRotate(ObjectStruct->Direction, ObjectStruct->Matrix, VecPos);

			VectorAddScaled(ObjectStruct->Position, VecPos, ObjectStruct->Position, FPS_ANIMATION_FACTOR);

			float MoveSpeed = 0.0;

			if (PetMap > MaxPos * MaxPos)
			{
				if (PetMap >= (MaxPos * 2) * (MaxPos * 2))
				{
					MoveSpeed = log(PetMap) * 1.30f;
				}
				else if (PetMap >= (MaxPos + (MaxPos / 2)) * (MaxPos + (MaxPos / 2)))
				{
					MoveSpeed = log(PetMap) * 1.20f;
				}
				else
				{
					MoveSpeed = log(PetMap) * 1.05f;
				}
			}
			else
			{
				if (AutoMove)
				{
					MoveSpeed = log(PetMap) * 1.0f;
				}
			}

			ObjectStruct->Direction[0] = 0.0f;
			ObjectStruct->Direction[1] = -MoveSpeed;
			ObjectStruct->Direction[2] = 0.0f;

			if (PetMap >= LostPostion * LostPostion)
			{
				if (ObjectStruct->Position[0] < ObjectStruct->Owner->Position[0])
				{
					ObjectStruct->Position[0] = ObjectStruct->Position[0] + d[0] + (float)(rand() % 360);
				}

				if (ObjectStruct->Position[0] > ObjectStruct->Owner->Position[0])
				{
					ObjectStruct->Position[0] = ObjectStruct->Position[0] + d[0] - (float)(rand() % 360);
				}

				if (ObjectStruct->Position[1] < ObjectStruct->Owner->Position[1])
				{
					ObjectStruct->Position[1] = ObjectStruct->Position[1] + d[1] + (float)(rand() % 360);
				}

				if (ObjectStruct->Position[1] > ObjectStruct->Owner->Position[1])
				{
					ObjectStruct->Position[1] = ObjectStruct->Position[1] + d[1] - (float)(rand() % 360);
				}
			}

			if (SceneFlag == 4)
			{
				ObjectStruct->Position[2] = ObjectStruct->Owner->Position[2];
			}

			if (AutoMove)
			{
				ObjectStruct->CurrentAction = ACTION_RUN;
			}
		}
	}
	else if (type == 2 || type == 4) //Uniria // Dinorant
	{
		if ((TerrainWall[TERRAIN_INDEX_REPEAT((int)floor(ObjectStruct->Owner->Position[0] / 100.0f), (int)floor(ObjectStruct->Owner->Position[1] / 100.0f))] & 1) == 1 && !state)
		{
			if (this->Miniature == 1)
			{
				this->WorkdMovimentMiniature(ObjectStruct, MaxPos);
			}
			else
			{
				ObjectStruct->Alpha = 0.0f;
			}
			return;
		}

		ObjectStruct->Scale = this->PetOriginalSize;

		if (ObjectStruct->Owner->Teleport == 1 || ObjectStruct->Owner->Teleport == 2) 
		{
			ObjectStruct->Alpha = ObjectStruct->Alpha - (0.1f * FPS_ANIMATION_FACTOR);

			if (ObjectStruct->Alpha < 0.0f) 
			{
				ObjectStruct->Alpha = 0.0f;
			}
		}
		else 
		{
			ObjectStruct->Alpha = 1.0f;
		}

		ObjectStruct->Position[0] = ObjectStruct->Owner->Position[0];
		ObjectStruct->Position[1] = ObjectStruct->Owner->Position[1];
		ObjectStruct->Position[2] = ObjectStruct->Owner->Position[2];

		if (gHelperSystem.CheckHelperType(ObjectStruct->Type, 4) != 0)
		{
			if (gMapManager.WorldActive == 8 || gMapManager.WorldActive == 10 || g_Direction.m_CKanturu.IsMayaScene())
			{
				ObjectStruct->Position[2] = ObjectStruct->Position[2] - 10.0f;
			}
			else if (gMapManager.WorldActive != -1)
			{
				ObjectStruct->Position[2] = ObjectStruct->Position[2] - 30.0f;
			}
		}

		ObjectStruct->Angle[0] = ObjectStruct->Owner->Angle[0];
		ObjectStruct->Angle[1] = ObjectStruct->Owner->Angle[1];
		ObjectStruct->Angle[2] = ObjectStruct->Owner->Angle[2];

		if (ObjectStruct->Owner->CurrentAction >= 15 && ObjectStruct->Owner->CurrentAction <= 37 || ObjectStruct->Owner->CurrentAction == 74 || ObjectStruct->Owner->CurrentAction == 75 || ObjectStruct->Owner->CurrentAction == 259 || ObjectStruct->Owner->CurrentAction == 260)
		{
			if (gHelperSystem.CheckHelperType(ObjectStruct->Type, 4) != 0 && (gMapManager.WorldActive == 8 || gMapManager.WorldActive == 10 || g_Direction.m_CKanturu.IsMayaScene()))
			{
				SetAction(ObjectStruct, 3);
			}
			else
			{
				SetAction(ObjectStruct, 2);
			}
			if (rand_fps_check(2) && gMapManager.WorldActive != 10 && !g_Direction.m_CKanturu.IsMayaScene())
			{
				if (ObjectStruct->Owner)
				{
					if (!g_isCharacterBuff(ObjectStruct->Owner, eBuff_Cloaking))
					{
						Vector((float)(rand() % 64 - 32) + ObjectStruct->Position[0], (float)(rand() % 64 - 32) + ObjectStruct->Position[1], (float)(rand() % 32 - 16) + ObjectStruct->Position[2], Position);

						Vector(1.0f, 1.0f, 1.0f, Color);

						if (gMapManager.WorldActive == 2)
						{
							CreateParticle(BITMAP_SMOKE, Position, ObjectStruct->Angle, Color);
						}
						else
						{
							CreateParticle(BITMAP_SMOKE + 1, Position, ObjectStruct->Angle, Color);
						}
					}
				}
			}
		}
		else if (ObjectStruct->Owner->CurrentAction == 68 || ObjectStruct->Owner->CurrentAction == 69)
		{
			if (gMapManager.WorldActive == 8 || gMapManager.WorldActive == 10 || g_Direction.m_CKanturu.IsMayaScene())
			{
				SetAction(ObjectStruct, 7);
			}
			else
			{
				SetAction(ObjectStruct, 6);
			}
		}
		else if (ObjectStruct->Owner->CurrentAction >= 38 && ObjectStruct->Owner->CurrentAction <= 59 || IsAliceRideAction_UniDino(ObjectStruct->Owner->CurrentAction) == true)
		{
			if (gHelperSystem.CheckHelperType(ObjectStruct->Type, 4) != 0)
			{
				if (gMapManager.WorldActive == 8 || gMapManager.WorldActive == 10 || g_Direction.m_CKanturu.IsMayaScene())
				{
					SetAction(ObjectStruct, 5);
				}
				else
				{
					SetAction(ObjectStruct, 4);
				}
			}
			else
			{
				SetAction(ObjectStruct, 3);
			}
		}
		else
		{
			if (gHelperSystem.CheckHelperType(ObjectStruct->Type, 4) != 0)
			{
				if (gMapManager.WorldActive == 8 || gMapManager.WorldActive == 10 || g_Direction.m_CKanturu.IsMayaScene())
				{
					SetAction(ObjectStruct, 1);
				}
				else
				{
					SetAction(ObjectStruct, 0);
				}
			}
			else
			{
				SetAction(ObjectStruct, 0);
			}
		}

		ObjectStruct->Velocity = 0.34f;
		ObjectStruct->Live = ObjectStruct->Owner->Live;
	}
	else if (type == 8) //Fenrir
	{
		if ((TerrainWall[TERRAIN_INDEX_REPEAT((int)floor(ObjectStruct->Owner->Position[0] / 100.0f), (int)floor(ObjectStruct->Owner->Position[1] / 100.0f))] & 1) == 1 && !state)
		{
			if (this->Miniature == 1)
			{
				this->WorkdMovimentMiniature(ObjectStruct, MaxPos);
			}
			else
			{
				ObjectStruct->Alpha = 0.0f;
			}
			return;
		}

		if (this->Miniature == 1 && m_pOwner != nullptr && m_pOwner->MiniatureMode)
		{
			ObjectStruct->Alpha = 1.0f;
			this->WorkdMovimentMiniature(ObjectStruct, MaxPos);
			ObjectStruct->Live = ObjectStruct->Owner->Live;
			return;
		}

		ObjectStruct->Scale = this->PetOriginalSize;

		if (ObjectStruct->Owner->Teleport == 1 || ObjectStruct->Owner->Teleport == 2)
		{
			ObjectStruct->Alpha = ObjectStruct->Alpha - (0.1f * FPS_ANIMATION_FACTOR);

			if (ObjectStruct->Alpha < 0.0f)
			{
				ObjectStruct->Alpha = 0.0f;
			}
		}
		else
		{
			ObjectStruct->Alpha = 1.0f;
		}

		ObjectStruct->HeadAngle[0] = ObjectStruct->Owner->HeadAngle[0];
		ObjectStruct->HeadAngle[1] = ObjectStruct->Owner->HeadAngle[1];
		ObjectStruct->HeadAngle[2] = ObjectStruct->Owner->HeadAngle[2];
		ObjectStruct->Position[0] = ObjectStruct->Owner->Position[0];
		ObjectStruct->Position[1] = ObjectStruct->Owner->Position[1];
		ObjectStruct->Position[2] = ObjectStruct->Owner->Position[2];
		ObjectStruct->Angle[0] = ObjectStruct->Owner->Angle[0];
		ObjectStruct->Angle[1] = ObjectStruct->Owner->Angle[1];
		ObjectStruct->Angle[2] = ObjectStruct->Owner->Angle[2];

		if (ObjectStruct->Owner->CurrentAction >= 90 && ObjectStruct->Owner->CurrentAction <= 101 || IsAliceRideAction_Fenrir(ObjectStruct->Owner->CurrentAction) == true || ObjectStruct->Owner->CurrentAction == 282)
		{
			SetAction(ObjectStruct, 3);
			ObjectStruct->Velocity = 0.40f;
		}
		else if (ObjectStruct->Owner->CurrentAction >= 102 && ObjectStruct->Owner->CurrentAction <= 105)
		{
			SetAction(ObjectStruct, 4);
			ObjectStruct->Velocity = 0.40f;
		}
		else if (ObjectStruct->Owner->CurrentAction >= 106 && ObjectStruct->Owner->CurrentAction <= 109)
		{
			SetAction(ObjectStruct, 5);
			ObjectStruct->Velocity = 0.40f;
		}
		else if (ObjectStruct->Owner->CurrentAction >= 122 && ObjectStruct->Owner->CurrentAction <= 125 || ObjectStruct->Owner->CurrentAction == 231)
		{
			SetAction(ObjectStruct, 0);
			ObjectStruct->Velocity = 0.40f;
		}
		else
		{
			if (ObjectStruct->Owner->CurrentAction >= 126 && ObjectStruct->Owner->CurrentAction <= 129)
			{
				SetAction(ObjectStruct, 1);
				ObjectStruct->Velocity = 1.0f;
			}
			else if (ObjectStruct->Owner->CurrentAction >= 110 && ObjectStruct->Owner->CurrentAction <= 121)
			{
				SetAction(ObjectStruct, 2);
				ObjectStruct->Velocity = 0.60f;
			}

			if (gMapManager.WorldActive != 10)
			{
				if (rand_fps_check(3) && !gMapManager.InHellas())
				{
					if (ObjectStruct->Owner)
					{
						if (!g_isCharacterBuff(ObjectStruct->Owner, eBuff_Cloaking))
						{
							Vector((float)(rand() % 64 - 32) + ObjectStruct->Position[0], (float)(rand() % 64 - 32) + ObjectStruct->Position[1], (float)(rand() % 32 - 16) + ObjectStruct->Position[2], Position);
							Vector(1.0f, 1.0f, 1.0f, Color);

							if (gMapManager.WorldActive == 2)
							{
								CreateParticle(BITMAP_SMOKE, Position, ObjectStruct->Angle, Color);
							}
							else if (gMapManager.WorldActive != 10 && !g_Direction.m_CKanturu.IsMayaScene())
							{
								CreateParticle(BITMAP_SMOKE + 1, Position, ObjectStruct->Angle, Color);
							}
						}
					}
				}
			}
			else
			{
				int Effect = 0;

				Vector(1.0f, 1.0f, 1.0f, Color);
				Vector(120.0f, 0.0f, 32.0f, p);

				if (IsAnimationFrameRange(ObjectStruct, 1.0f, 1.2f))
				{
					b->TransformPosition(BoneTransform[22], p, Position);	// R Hand

					Effect = 1;

					Position[0] = Position[0] + 40.0f;
				}

				if (IsAnimationFrameRange(ObjectStruct, 4.8f, 5.0f))
				{
					b->TransformPosition(BoneTransform[44], p, Position);	// R Foot

					Effect = 1;
					Position[0] = Position[0] + 40.0f;
					Position[2] = Position[2] + 700.0f;
				}

				if (Effect && rand_fps_check(1))
				{
					p[0] = Position[0];
					p[1] = Position[1];
					p[2] = Position[1];

					CreateEffect(BITMAP_SHOCK_WAVE, p, ObjectStruct->Angle, Color, 1);

					for (int j = 0; j < ((rand() % 5) + 5); ++j)
					{
						p[0] = (float)(rand() % 50 - 25) + Position[0];
						p[1] = (float)(rand() % 50 - 25) + Position[1];
						p[2] = (float)(rand() % 16 - 8) + Position[2] - 10.0f;

						CreateParticle(BITMAP_SMOKE, p, ObjectStruct->Angle, Color);
					}
				}
			}
		}
	}
	else if (type == 16) //Horse
	{
		if ((TerrainWall[TERRAIN_INDEX_REPEAT((BYTE)floor(ObjectStruct->Owner->Position[0] / 100.0f), (BYTE)floor(ObjectStruct->Owner->Position[1] / 100.0f))] & 1) == 1 && !state)
		{
			if (this->Miniature == 1)
			{
				this->WorkdMovimentMiniature(ObjectStruct, MaxPos);
			}
			else
			{
				ObjectStruct->Alpha = 0.0f;
			}
			return;
		}

		ObjectStruct->Scale = this->PetOriginalSize;

		b->BoneHead = 7;

		VectorCopy(ObjectStruct->Owner->HeadAngle, ObjectStruct->HeadAngle);
		VectorCopy(ObjectStruct->Owner->Position, ObjectStruct->Position);

		VectorCopy(ObjectStruct->Owner->Angle, ObjectStruct->Angle);
		if (ObjectStruct->Owner->CurrentAction == PLAYER_ATTACK_DARKHORSE)
		{
			SetAction(ObjectStruct, 3);
			ObjectStruct->Velocity = 0.34f;
		}
		else if (ObjectStruct->Owner->CurrentAction == PLAYER_RUN_RIDE_HORSE)
		{
			Vector(1.f, 1.f, 1.f, Color);

			SetAction(ObjectStruct, 1);
			if (gMapManager.WorldActive == WD_10HEAVEN || g_Direction.m_CKanturu.IsMayaScene())
			{
				bool   bWave = false;
				vec3_t p = { 120.f, 0.f, rand() % 64 - 32.f };

				if (IsAnimationFrameRange(ObjectStruct, 1.0f, 1.2f))
				{
					b->TransformPosition(BoneTransform[19], p, Position);
					bWave = true;
				}
				else if (IsAnimationFrameRange(ObjectStruct, 1.1f, 1.4f))
				{
					b->TransformPosition(BoneTransform[25], p, Position);
					bWave = true;
				}
				else if (IsAnimationFrameRange(ObjectStruct, 1.3f, 1.6f))
				{
					b->TransformPosition(BoneTransform[32], p, Position);
					bWave = true;
				}
				else if (IsAnimationFrameRange(ObjectStruct, 1.5f, 1.8f))
				{
					b->TransformPosition(BoneTransform[38], p, Position);
					bWave = true;
				}

				if (bWave)
				{
					Vector(Position[0] + (float)(rand() % 16 - 8),
						Position[1] + (float)(rand() % 16 - 8),
						Position[2], p);
					CreateEffect(BITMAP_SHOCK_WAVE, p, ObjectStruct->Angle, Color, 1);

					for (int i = 0; i < rand() % 5 + 5; ++i)
					{
						Vector(Position[0] + (float)(rand() % 50 - 25),
							Position[1] + (float)(rand() % 50 - 25),
							Position[2] + (float)(rand() % 16 - 8) - 10, p);
						CreateParticle(BITMAP_SMOKE, p, ObjectStruct->Angle, Color);
					}
				}
			}
			else if (rand_fps_check(2) && !gMapManager.InHellas())
			{
				if (ObjectStruct->Owner && !g_isCharacterBuff(ObjectStruct->Owner, eBuff_Cloaking))
				{
					Vector(ObjectStruct->Position[0] + (float)(rand() % 64 - 32),
						ObjectStruct->Position[1] + (float)(rand() % 64 - 32),
						ObjectStruct->Position[2] + (float)(rand() % 32 - 16), Position);

					if (gMapManager.WorldActive == WD_2DEVIAS)
						CreateParticle(BITMAP_SMOKE, Position, ObjectStruct->Angle, Color);
					else if (gMapManager.WorldActive != WD_10HEAVEN)
						if (!g_Direction.m_CKanturu.IsMayaScene())
							CreateParticle(BITMAP_SMOKE + 1, Position, ObjectStruct->Angle, Color);
				}
			}

			ObjectStruct->Velocity = 0.34f;
		}
		else if (ObjectStruct->Owner->CurrentAction >= PLAYER_ATTACK_RIDE_STRIKE && ObjectStruct->Owner->CurrentAction <= PLAYER_ATTACK_RIDE_ATTACK_MAGIC)
		{
			SetAction(ObjectStruct, 2);
			ObjectStruct->Velocity = 0.34f;
		}
		else if (ObjectStruct->Owner->CurrentAction == PLAYER_IDLE1_DARKHORSE)
		{
			SetAction(ObjectStruct, 5);
			ObjectStruct->Velocity = 1.0f;
		}
		else if (ObjectStruct->Owner->CurrentAction == PLAYER_IDLE2_DARKHORSE)
		{
			SetAction(ObjectStruct, 6);
			ObjectStruct->Velocity = 1.0f;
		}
		else
		{
			SetAction(ObjectStruct, 0);
			ObjectStruct->WeaponLevel = 0;
			ObjectStruct->Velocity = 0.3f;
		}

		if (ObjectStruct->CurrentAction != 1)
		{
			if (ObjectStruct->Owner && !g_isCharacterBuff(ObjectStruct->Owner, eBuff_Cloaking))
			{
				if (rand_fps_check(3))
				{
					vec3_t p = { 50.f, -4.f, 0.f };
					b->TransformPosition(BoneTransform[27], p, Position);

					if (gMapManager.WorldActive == WD_7ATLANSE || gMapManager.WorldActive == WD_67DOPPLEGANGER3)
					{
						CreateParticle(BITMAP_BUBBLE, Position, ObjectStruct->Angle, ObjectStruct->Light);
					}
					else
					{
						CreateParticle(BITMAP_SMOKE, Position, ObjectStruct->Angle, ObjectStruct->Light);
					}
				}
			}
		}

		if (ObjectStruct->CurrentAction == 3)
		{
			vec3_t Pos1, Pos2, p, p2;

			Vector(60.f, 0.f, 0.f, Pos1);
			Vector(0.f, 0.f, 0.f, Pos2);
			Vector(1.f, 1.f, 1.f, Color);
			b->TransformPosition(BoneTransform[9], Pos1, p);
			b->TransformPosition(BoneTransform[9], Pos2, p2);
			CreateBlur(Hero, p, p2, Color, 0);
		}

		if (ObjectStruct->CurrentAction == 3)
		{
			RenderDarkHorseSkill(ObjectStruct, b);
		}

		if (ObjectStruct->Owner->ExtState == 1)
		{
			vec3_t p;
			vec3_t Angle = { 0.f, 0.f, 0.f };
			float  Matrix[3][4];

			Vector(0.f, 50, 0.f, p);
			for (int i = 0; i < 4; ++i)
			{
				Angle[2] += 90.f;
				AngleMatrix(Angle, Matrix);
				VectorRotate(p, Matrix, Position);
				VectorAdd(Position, ObjectStruct->Position, Position);

				for (int j = 0; j < 3; ++j)
				{
					CreateJoint(BITMAP_FLARE, Position, Position, Angle, 0, ObjectStruct, 40, 2);
				}
			}
		}
		else if (ObjectStruct->Owner->ExtState == 2)
		{
		}
		ObjectStruct->Owner->ExtState = 0;

		ObjectStruct->Live = ObjectStruct->Owner->Live;
	}
}

void HELPER_VIEW::WorkdMovimentMiniature(OBJECT* ObjectStruct, float MaxPos)
{
	vec3_t OwnerPos;
	float Angle;

	ObjectStruct->Position[2] = ObjectStruct->Owner->Position[2];

	Vector(ObjectStruct->Owner->Position[0] - ObjectStruct->Position[0], ObjectStruct->Owner->Position[1] - ObjectStruct->Position[1], ObjectStruct->Owner->Position[2] - ObjectStruct->Position[2], OwnerPos);

	float PetMap = OwnerPos[0] * OwnerPos[0] + OwnerPos[1] * OwnerPos[1];

	bool Walking = 0;

	if (PetMap < MaxPos * MaxPos * MaxPos)
	{
		if (PetMap >= (MaxPos * MaxPos) + 20)
		{
			Angle = CreateAngle(ObjectStruct->Position[0], ObjectStruct->Position[1], ObjectStruct->Owner->Position[0], ObjectStruct->Owner->Position[1]);
			ObjectStruct->Angle[2] = TurnAngle2(ObjectStruct->Angle[2], Angle + 25, 10.0);

			Walking = 1;

			if (m_Type == 16)
			{
				ObjectStruct->CurrentAction = 1;
			}
			else
			{
				ObjectStruct->CurrentAction = 2; //Setar correr
			}
		}
	}
	else
	{
		ObjectStruct->Position[0] = ObjectStruct->Owner->Position[0];
		ObjectStruct->Position[1] = ObjectStruct->Owner->Position[1];
		ObjectStruct->Position[2] = ObjectStruct->Owner->Position[2];
		ObjectStruct->Angle[0] = ObjectStruct->Owner->Angle[0];
		ObjectStruct->Angle[1] = ObjectStruct->Owner->Angle[1];
		ObjectStruct->Angle[2] = ObjectStruct->Owner->Angle[2];
	}

	vec3_t VecPos;

	AngleMatrix(ObjectStruct->Angle, ObjectStruct->Matrix);
	VectorRotate(ObjectStruct->Direction, ObjectStruct->Matrix, VecPos);

	if (OwnerPos[0] == 0.0f && OwnerPos[1] == 0.0f && OwnerPos[2] == 0.0f)
	{
		VecPos[0] = 40.f;
		VecPos[1] = 120.f;
	}

	VectorAddScaled(ObjectStruct->Position, VecPos, ObjectStruct->Position, FPS_ANIMATION_FACTOR);

	float Velocity = 0.0;

	if (PetMap > MaxPos * MaxPos)
	{
		if (PetMap >= (MaxPos * 2) * (MaxPos * 2))
		{
			Velocity = log(PetMap) * 1.30f;
		}
		else if (PetMap >= (MaxPos + (MaxPos / 2)) * (MaxPos + (MaxPos / 2)))
		{
			Velocity = log(PetMap) * 1.20f;
		}
		else
		{
			Velocity = log(PetMap) * 1.05f;
		}
	}

	ObjectStruct->Direction[0] = 0.0f;
	ObjectStruct->Direction[1] = -Velocity;
	ObjectStruct->Direction[2] = 0.0f;

	if (!Walking)
	{
		ObjectStruct->CurrentAction = 0;
	}

	ObjectStruct->Scale = this->SizeMiniature;
	ObjectStruct->Velocity = this->VelocityMiniature;
}

void HELPER_VIEW::UpdateMove(OBJECT* model)
{
	WorkMoviment(m_Obj, 180.f, m_Type, m_Movement, 0);

	if (m_HeightFloor > 0.0f && m_Type == 0)
	{
		m_Obj->Position[2] = m_Obj->Owner->Position[2] + m_HeightFloor;
	}

	if (SceneFlag == 4)
	{
		if (m_Type == 0)
		{
			m_Obj->Position[2] = m_Obj->Owner->Position[2] + m_HeightFloor;
		}
	}
}
