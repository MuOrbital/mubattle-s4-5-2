#include "stdafx.h"
#include "CustomWorldEffectCompat.h"
#include "ZzzAI.h"
#include "ZzzEffect.h"
#include "ZzzCharacter.h"
#include "ZzzLodTerrain.h"
#include "GMDeventer.h"

CGMDeventerPtr CGMDeventer::Make()
{
	CGMDeventerPtr pkfield(new CGMDeventer);
	return pkfield;
}

CGMDeventer::CGMDeventer()
{
}

CGMDeventer::~CGMDeventer()
{
}

bool CGMDeventer::LoadMapData()
{
	char FileName[100];

	sprintf(FileName, "Object%d\\solight02_R.jpg", World + 1);
	LoadBitmap(FileName, BITMAP_map_texture01, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);
	return true;
}

bool CGMDeventer::MoveObject(OBJECT* pObject)
{
	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return false;

	int index = (pObject->Type);

	if (index == 9 || index == 11)
	{
		Action_t* v3 = &Models[pObject->Type].Actions[pObject->CurrentAction];

		if (v3 != nullptr && pObject->AnimationFrame >= (v3->NumAnimationKeys - v3->PlaySpeed))
		{
			if (pObject->m_iAnimation > 0)
			{
				SetAction(pObject, 0, true);

				if(rand_fps_check(1))
					--pObject->m_iAnimation;
			}
			else
			{
				pObject->m_iAnimation = rand() % 3 + 4;
				SetAction(pObject, 1, true);
			}
		}
	}
	else if (index == 50)
	{
		vec3_t Light;

		float fLumi = abs(sinf((WorldTime * 0.001))) * 0.4 + 0.3;

		Vector(fLumi * 0.2, fLumi * 0.3, fLumi * 0.0, Light);

		AddTerrainLight(pObject->Position[0], pObject->Position[1], Light, 2, PrimaryTerrainLight);
	}
	return true;
}

bool CGMDeventer::CreateObject(OBJECT* pObject)
{
	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return false;

	int index = (pObject->Type);

	if (index == 9)
	{
		BMD* b = &Models[pObject->Type];
		if (b)
		{
			VectorCopy(pObject->Position, b->BodyOrigin);
			b->Animation(BoneTransform, pObject->AnimationFrame, pObject->PriorAnimationFrame, pObject->PriorAction, pObject->Angle, pObject->HeadAngle, 0, 0);
			b->TransformPosition(BoneTransform[23], pObject->Position, pObject->EyeLeft, true);
			b->TransformPosition(BoneTransform[22], pObject->Position, pObject->EyeRight, true);

			CreateJoint(BITMAP_JOINT_ENERGY, pObject->Position, pObject->Position, pObject->Angle, 60, pObject, 40.0, -1, 0, 0, -1, 0, -1);
			CreateJoint(BITMAP_JOINT_ENERGY, pObject->Position, pObject->Position, pObject->Angle, 61, pObject, 40.0, -1, 0, 0, -1, 0, -1);
		}
	}
	else if (index > 55 && index <= 67)
	{
		pObject->HiddenMesh = -2;
	}
	return true;
}

bool CGMDeventer::RenderObjectVisual(OBJECT* pObject, BMD* pModel)
{
	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return false;

	int index = (pObject->Type);

	vec3_t Position, Light;
	float iluminaty, Scale;

	Scale = 0.0;
	iluminaty = 0.0;
	Vector(0.0, 0.0, 0.0, Light);
	Vector(0.0, 0.0, 0.0, Position);

	if (index > 59)
	{
		switch (index)
		{
		case 60:
			if (rand() % 3 == 0)
			{
				iluminaty = (rand() % 10) * 0.004 + 0.01;
				Vector((iluminaty * 0.2109375), (iluminaty * 0.6914062), (iluminaty * 0.5859375), Light);
				CreateParticleSync(BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 21, pObject->Scale, pObject);
				Vector((iluminaty * 0.8632812), (iluminaty * 0.4726562), (iluminaty * 0.7851562), Light);
				CreateParticleSync(BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 21, pObject->Scale, pObject);
			}
			break;
		case 61:
			if (rand() % 3 == 0)
			{
				iluminaty = (rand() % 10) * 0.002 + 0.01;
				Vector((iluminaty * 1.2), 0.0, 0.0, Light);
				CreateParticleSync(BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 21, pObject->Scale, pObject);
			}
			break;
		case 62:
			/*if (rand() % 35 == 5)
			{
				iluminaty = (rand() % 10) * 0.05 + 0.03;
				Vector((iluminaty * 1.0), 0.0, 0.0, Light);
				CreateParticleSync(31734, pObject->Position, pObject->Angle, Light, 0, pObject->Scale, pObject);
			}*/
			break;
		case 63:
			Scale = pObject->Scale;
			Vector(0.05, 0.03, 0.5, Light);
			switch (rand() % 3)
			{
			case 0:
				CreateParticleSync(BITMAP_FIRE_HIK1_MONO, pObject->Position, pObject->Angle, Light, 2, Scale);
				Vector(0.39, 0.65, 0.39, Light);
				CreateParticleSync(BITMAP_FIRE_HIK1_MONO, pObject->Position, pObject->Angle, Light, 2, Scale * 0.5);
				break;
			case 1:
				CreateParticleSync(BITMAP_FIRE_HIK2_MONO, pObject->Position, pObject->Angle, Light, 6, Scale);
				Vector(0.39, 0.65, 0.39, Light);
				CreateParticleSync(BITMAP_FIRE_HIK2_MONO, pObject->Position, pObject->Angle, Light, 6, Scale * 0.5);
				break;
			case 2:
				CreateParticleSync(BITMAP_FIRE_HIK3_MONO, pObject->Position, pObject->Angle, Light, 2, Scale);
				Vector(0.39, 0.65, 0.39, Light);
				CreateParticleSync(BITMAP_FIRE_HIK3_MONO, pObject->Position, pObject->Angle, Light, 2, Scale * 0.5);
				break;
			}
			//CreateParticleSync(BITMAP_FIRE_HIK3_MONO, pObject->Position, pObject->Angle, Light, 2, pObject->Scale, 0);
			//Vector(0.39, 0.65, 0.39, Light);
			//Scale = pObject->Scale * 0.5;
			//Vector(0.39, 0.65, 0.39, Light);
			//CreateParticleSync(BITMAP_FIRE_HIK3_MONO, pObject->Position, pObject->Angle, Light, 2, pObject->Scale, 0);
			break;
		case 64:
			if (!(rand() % 10))
			{
				Vector(0.5, 0.9, 0.5, Light);
				Scale = pObject->Scale * 0.5;
				CreateParticleSync(BITMAP_SMOKE, pObject->Position, pObject->Angle, Light, 83, Scale, 0);
			}
			break;
		case 65:
			break;
		case 66:
			if (rand() % 3 == 0)
			{
				iluminaty = (rand() % 10) * 0.002 + 0.01;
				Vector((iluminaty * 0.12), 0.0, (iluminaty * 0.6), Light);
				CreateParticleSync(BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 21, pObject->Scale, pObject);
			}
			break;
		case 67:
			/*if (!(rand() % 2))
			{
				Vector(0.4, 0.4, 0.4, Light);
				Scale = pObject->Scale * 0.6;
				CreateParticleSync((rand() % 3) + 32392, pObject->Position, pObject->Angle, Light, 8, Scale, pObject);
			}*/
			break;
		}
		return true;
	}
	if (index > 31)
	{
		if (index == 56)
		{
			if (!(rand() % 10))
			{
				Position[0] = pObject->Position[0];
				Position[1] = pObject->Position[1];
				Position[2] = pObject->Position[2] + 50.0;

				iluminaty = (rand() % 4 + 3) * 0.1;
				Vector(iluminaty, (iluminaty * 0.6), (iluminaty * 0.2), Light);
				CreateParticleSync(BITMAP_SMOKE, Position, pObject->Angle, Light, 33, pObject->Scale, 0);
			}
		}
		else if (index == 57)
		{
			if (rand() % 6 == 0)
			{
				Vector(0.02, 0.03, 0.02, Light);
				CreateParticleSync(BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 9, pObject->Scale, pObject);
			}
		}
		else if (index == 58 && rand() % 3 == 0)
		{
			iluminaty = (rand() % 10) * 0.004 + 0.01;
			Light[0] = iluminaty * 0.2109375;
			Light[1] = iluminaty * 0.69140625;
			Light[2] = iluminaty * 0.5859375;
			CreateParticleSync(BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 21, pObject->Scale, pObject);
		}
		else if (index == 59)
		{
			if (rand() % 3 == 0)
			{
				iluminaty = (rand() % 10) * 0.004 + 0.02;
				Light[0] = iluminaty * 0.29296875;
				Light[1] = iluminaty * 0.05859375;
				Light[2] = 0.0;
				CreateParticleSync(BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 21, pObject->Scale, pObject);
			}
		}
		return true;
	}

	return false;

}

bool CGMDeventer::RenderObjectMesh(OBJECT* pObject, BMD* pModel, bool ExtraMon)
{
	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return false;

	int index = (pObject->Type);
	float iluminaty, RenderAlpha, BlendMeshTexCoordV;

	switch (index)
	{
	case 11:
		BlendMeshTexCoordV = ((float)-((int)*(float*)&WorldTime % 10000) * 0.0001000000);
		pModel->RenderBody(RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, 2, -1);
		pModel->StreamMesh = 2;
		pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, -1, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, BlendMeshTexCoordV, -1);
		pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, -2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, BlendMeshTexCoordV, -1);
		pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, -2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, BlendMeshTexCoordV, -1);
		pModel->StreamMesh = -1;
		break;
	case 37:
		pModel->StreamMesh = 0;
		BlendMeshTexCoordV = ((float)((int)*(float*)&WorldTime % 25000) * 0.000040);
		pModel->RenderBody(RENDER_TEXTURE, pObject->Alpha, -1, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, BlendMeshTexCoordV, pObject->HiddenMesh, -1);
		pModel->StreamMesh = -1;
		break;
	case 50:
		iluminaty = (float)(WorldTime * 0.8396000);
		RenderAlpha = (float)(iluminaty * 0.400000) + 0.300000;
		pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, -2, RenderAlpha, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_map_texture01);
		break;
	default:
		if (index <= 55)
			return false;
		if (index > 67)
			return false;
		break;
	}
	return true;
}

void CGMDeventer::RenderAfterObjectMesh(OBJECT* pObject, BMD* pModel, bool ExtraMon)
{
}
