#pragma once

#include <set>
#include "Lua.h"
//#include "LuaEffects.h"
#include "Console.h"
#include "ZzzBMD.h"
#include "ZzzEffect.h"
#include "CDirection.h"
#include "MapManager.h"

class BMDClass {
public:
	BMDClass() : m_Struct(nullptr){

	};

	BMDClass(DWORD Struct) {
		this->m_Struct = (BMD*)Struct;
	};

	~BMDClass() {

	};

	float GetLight(int value) const 
	{
		if (!m_Struct) 
		{
			return 0.0f;
		}

		if (value == 0) 
		{
			return m_Struct->BodyLight[0];
		}
		else if (value == 1) 
		{
			return m_Struct->BodyLight[1];
		}
		else if (value == 2) 
		{
			return m_Struct->BodyLight[2];
		}

		return *(float*)m_Struct->BodyLight;
	};

	void SetLight(float r, float g, float b) const 
	{
		if (!m_Struct) 
		{
			return;
		}

		m_Struct->BodyLight[0] = r;
		m_Struct->BodyLight[1] = g;
		m_Struct->BodyLight[2] = b;
	};

	void RenderBody(int Flag, float Alpha, int BlendMesh, float BlendMeshLight, float BlendMeshTexCoordU, float BlendMeshTexCoordV, int HiddenMesh, int Texture) const 
	{
		if (!m_Struct) 
		{
			return;
		}

		m_Struct->RenderBody(Flag, Alpha, BlendMesh, BlendMeshLight, BlendMeshTexCoordU, BlendMeshTexCoordV, HiddenMesh, Texture);
	};

	void RenderMesh(int i, int RenderFlag, float Alpha, int BlendMesh, float BlendMeshLight, float BlendMeshTexCoordU, float BlendMeshTexCoordV, int MeshTexture) const 
	{
		if (!m_Struct) 
		{
			return;
		}

		m_Struct->RenderMesh(i, RenderFlag, Alpha, BlendMesh, BlendMeshLight, BlendMeshTexCoordU, BlendMeshTexCoordV, MeshTexture);
	};

	void glColor3f() const 
	{
		if (!m_Struct) 
		{
			return;
		}

		glColor3fv(m_Struct->BodyLight);
	};

	void BeginRender(float value) const {
		//glPushMatrix();
	}

	void EndRender() const {
		//glPopMatrix();
	}

	void setMesh(int vlaue) const 
	{
		if (!m_Struct) 
		{
			return;
		}

		m_Struct->StreamMesh = vlaue;
	}

	void RenderShadowModel() const 
	{
		if (!m_Struct) 
		{
			return;
		}
				
		if (gMapManager.WorldActive != WD_10HEAVEN && gMapManager.InHellas() == FALSE)
		{
			if (!g_Direction.m_CKanturu.IsMayaScene())
			{
				EnableAlphaTest(1);
		
				if (gMapManager.WorldActive == 7)
				{
					glColor4f(0.0f, 0.0f, 0.0f, 0.2f);
				}
				else 
				{
					glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
				}
		
				m_Struct->RenderBodyShadow(-1, -1, -1, -1);
			}
		}
	}

	void TransformPosition(int Link, float PosX, float PosY, float PosZ) 
	{
		if (!m_Struct) {
			return;
		}
		
		if (Link != -1) 
		{
			vec3_t vPos;
		
			Vector(PosX, PosY, PosZ, vPos);
		
			m_Struct->TransformPosition(BoneTransform[Link], vPos, this->Position, 0);
		}
		
		this->LinkObject = Link;
	}

	void TransformPosition2(int Link, float PosX, float PosY, float PosZ) 
	{
		if (!m_Struct) 
		{
			return;
		}
		
		if (Link != -1) {
			vec3_t vPos;
		
			Vector(PosX, PosY, PosZ, vPos);
		
			m_Struct->TransformPosition(BoneTransform[Link], vPos, this->Position, 1);
		}
		
		this->LinkObject = Link;
	}

	void CreateSprites(int Bitmap, float Scale, float LightX, float LightY, float LightZ, DWORD ObjectStruct) 
	{
		if (!m_Struct || !ObjectStruct) 
		{
			return;
		}
		
		vec3_t Light;
		
		Vector(LightX, LightY, LightZ, Light);
		
		auto obj = (OBJECT*)ObjectStruct;
		
		if (obj == nullptr) 
		{
			return;
		}
		
		CreateSprite(Bitmap, this->Position, Scale, Light, obj, 0.0, 0);
	}

	void CreateParticles(int Bitmap, int SubType, float Scale, float LightR, float LightG, float LightB, DWORD ObjectStruct) 
	{
		if (!this->m_Struct || !ObjectStruct) 
		{
			return;
		}
		
		vec3_t Light;
		
		Vector(LightR, LightG, LightB, Light);
		
		auto obj = (OBJECT*)ObjectStruct;
		
		if (obj == nullptr) 
		{
			return;
		}
		
		CreateParticle(Bitmap, Position, obj->Angle, Light, SubType, Scale, obj);
	}

	void CreateEffects(int Bitmap, int SubType, float LightX, float LightY, float LightZ, DWORD ObjectStruct) 
	{
		if (!this->m_Struct || !ObjectStruct) 
		{
			return;
		}
		
		static CCriticalSection m_critical;
		
		m_critical.lock();
		
		auto obj = (OBJECT*)ObjectStruct;
		
		if (obj == nullptr) {
			m_critical.unlock();
			return;
		}
		
		vec3_t Light;
		
		Vector(LightX, LightY, LightZ, Light);
		
		CreateEffect(Bitmap, (this->LinkObject == -1 ? obj->Position : this->Position), obj->Angle, Light, SubType, obj, -1, 0, 0, 0, 0.0, -1);
		
		m_critical.unlock();
	}

	void CreateEffectsInFenrirUsingSkill(DWORD ObjectStruct) 
	{
		if (!ObjectStruct) 
		{
			return;
		}
		
		auto o = (OBJECT*)ObjectStruct;
		vec3_t vLight;
		vec3_t vPos, vPosition;
		
		Vector(0.0, 0.0, 0.0, vPos);
		Vector(1.0f, 1.0f, 1.0f, vLight);
		
		if (o == nullptr) 
		{
			return;
		}
		
		if (o->CurrentAction == 1)
		{
			if (o->AnimationFrame >= 0.0f && o->AnimationFrame <= 1.5f)
			{
				Vector(0.0f, 0.0f, 0.0f, vPos);
				m_Struct->TransformPosition(BoneTransform[22], vPos, vPosition, false);
				CreateEffect(MODEL_FENRIR_FOOT_THUNDER, vPosition, o->Angle, vLight, 0, o, -1, 0, 0, 0, 0.0, -1);
				Vector(0.0f, 0.0f, 0.0f, vPos);
				m_Struct->TransformPosition(BoneTransform[28], vPos, vPosition, false);
				CreateEffect(MODEL_FENRIR_FOOT_THUNDER, vPosition, o->Angle, vLight, 0, o, -1, 0, 0, 0, 0.0, -1);
				Vector(0.0f, 0.0f, 0.0f, vPos);
				m_Struct->TransformPosition(BoneTransform[36], vPos, vPosition, false);
				CreateEffect(MODEL_FENRIR_FOOT_THUNDER, vPosition, o->Angle, vLight, 0, o, -1, 0, 0, 0, 0.0, -1);
				Vector(0.0f, 0.0f, 0.0f, vPos);
				m_Struct->TransformPosition(BoneTransform[44], vPos, vPosition, false);
				CreateEffect(MODEL_FENRIR_FOOT_THUNDER, vPosition, o->Angle, vLight, 0, o, -1, 0, 0, 0, 0.0, -1);
			}
		}
		else if (o->CurrentAction == 2)
		{
			if (o->AnimationFrame>1.f && o->AnimationFrame <= 1.4f)
			{
				Vector(0.0f, 0.0f, 0.0f, vPos);
				m_Struct->TransformPosition(BoneTransform[22], vPos, vPosition, false);
				CreateEffect(MODEL_FENRIR_FOOT_THUNDER, vPosition, o->Angle, vLight, 0, o, -1, 0, 0, 0, 0.0, -1);
				Vector(0.0f, 0.0f, 0.0f, vPos);
				m_Struct->TransformPosition(BoneTransform[28], vPos, vPosition, false);
				CreateEffect(MODEL_FENRIR_FOOT_THUNDER, vPosition, o->Angle, vLight, 0, o, -1, 0, 0, 0, 0.0, -1);
			}
			else if (o->AnimationFrame>4.8f && o->AnimationFrame <= 5.2f)
			{
				Vector(0.0f, 0.0f, 0.0f, vPos);
				m_Struct->TransformPosition(BoneTransform[36], vPos, vPosition, false);
				CreateEffect(MODEL_FENRIR_FOOT_THUNDER, vPosition, o->Angle, vLight, 0, o, -1, 0, 0, 0, 0.0, -1);
				Vector(0.0f, 0.0f, 0.0f, vPos);
				m_Struct->TransformPosition(BoneTransform[44], vPos, vPosition, false);
				CreateEffect(MODEL_FENRIR_FOOT_THUNDER, vPosition, o->Angle, vLight, 0, o, -1, 0, 0, 0, 0.0, -1);
			}
		}
	}

public:
	BMD* m_Struct;
	vec3_t Position;
	int LinkObject;
};

class LuaBMD {
public:
	LuaBMD();
	~LuaBMD();

	void RegisterClassBMD(lua_State* lua);
};