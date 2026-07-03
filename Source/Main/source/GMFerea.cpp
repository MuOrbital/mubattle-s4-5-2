#include "stdafx.h"
#include "CustomWorldEffectCompat.h"
#include "GMFerea.h"
#include "ZzzEffect.h"
#include "ZzzLodTerrain.h"

CGMFereaPtr CGMFerea::Make()
{
	CGMFereaPtr pkfield(new CGMFerea);
	return pkfield;
}

CGMFerea::CGMFerea()
{
}

CGMFerea::~CGMFerea()
{
}

bool CGMFerea::LoadMapData()
{
	LoadBitmap("Effect\\ghosteffect01.jpg", BITMAP_GHOST_CLOUD1, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);

	return true;
}

bool CGMFerea::MoveObject(OBJECT* pObject)
{
	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return false;

	vec3_t Light;
	int index = (pObject->Type);

	if (index > 51)
	{
		if (index != 71)
		{
			if (index == 72)
			{
				Vector(1.0, 0.2, 0.0, Light);
				AddTerrainLight(pObject->Position[0], pObject->Position[1], Light, 1, PrimaryTerrainLight);
				pObject->HiddenMesh = -2;
				return true;
			}
			if (index == 123)
			{
				Vector(0.0, 0.45, 0.45, Light);
				AddTerrainLight(pObject->Position[0], pObject->Position[1], Light, 1, PrimaryTerrainLight);
				return true;
			}
			if (index <= 127 || index > 130 && (index <= 139 || index > 142))
				return false;
		}
	}
	switch (index)
	{
	case 51:
		return true;
	case 17:
		Vector(0.8, 0.3, 0.2, Light);
		AddTerrainLight(pObject->Position[0], pObject->Position[1], Light, 1, PrimaryTerrainLight);
		return true;
	}
	if (index <= 31)
		return false;

	if (index != 43)
		return false;

	pObject->BlendMeshTexCoordV -= timefac(0.1000000);

	return true;
}

bool CGMFerea::CreateObject(OBJECT* pObject)
{
	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return false;

	int index = (pObject->Type);

	switch (index)
	{
	case 6:
	case 9:
	case 28:
	case 33:
	case 71:
	case 72:
	case 129:
	case 130:
	case 140:
	case 141:
	case 142:
		pObject->HiddenMesh = -2;
		return true;
	}

	if (index > 60)
	{
		if (index > 114)
		{
			if (index < 126 || index > 127 && index != 137 && (index <= 151 || index > 154))
				return false;
		}
		else if (index != 114 && index != 65 && index != 84 && (index <= 96 || index > 98 && index != 106))
		{
			return false;
		}
		goto LABEL_32;
	}
	if (index == 60)
		goto LABEL_32;
	if (index <= 29)
	{
		if (index != 29 && index != 10 && index != 18 && index != 22 && index != 26)
			return false;
		goto LABEL_32;
	}

	if (index == 31 || index == 34 || index == 37)
	{
	LABEL_32:
		pObject->CollisionRange = -400.0;
		return true;
	}
	if (index != 39)
		return false;

	return true;
}

bool CGMFerea::RenderObjectVisual(OBJECT* pObject, BMD* pModel)
{
	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return false;

	int index = (pObject->Type);

	float Scale;
	vec3_t Light, vRelative, Position;

	if (index > 81)
	{
		if (index > 130)
		{
			switch (index)
			{
			case 140:
				Vector(1.f, 1.f, 1.f, Light);
				if (rand() % 2 == 0)
					CreateParticleSync(BITMAP_WATERFALL_1, pObject->Position, pObject->Angle, Light, 2, pObject->Scale, NULL);
				break;
			case 141:
				Vector(1.f, 1.f, 1.f, Light);
				CreateParticleSync(BITMAP_WATERFALL_3, pObject->Position, pObject->Angle, Light, 4, pObject->Scale, NULL);
				break;
			case 142:
				Vector(1.f, 1.f, 1.f, Light);
				if (rand() % 3 == 0)
					CreateParticleSync(BITMAP_WATERFALL_2, pObject->Position, pObject->Angle, Light, 2, pObject->Scale, NULL);
				break;
			case 148:
				if (rand() % 3 == 0)
				{
					Vector(0.4000000, 0.4000000, 1.000000, Light);
					Scale = (float)(pObject->Scale * 0.4000000) * 1.800000;
					pObject->Position[2] = pObject->Position[2] + 15.00000;
					CreateParticleSync(BITMAP_TRUE_BLUE, pObject->Position, pObject->Angle, Light, 0, Scale, NULL);
				}
				break;
			default:
				return false;
			}
		}
		else
		{
			switch (index)
			{
			case 130:
				if (!(rand() % 20))
				{
					Vector(0.03000000, 0.03000000, 0.07000000, Light);
					CreateParticleSync(BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 20, pObject->Scale, pObject);
				}
				break;
			case 87:
			{
				Vector(0.0, 0.0, 0.0, vRelative);
				Vector(1.000000, 1.000000, 1.000000, Light);
				int Bone = CustomWorldFindBone(pObject, pModel, "Bone21");
				pModel->TransformPosition(BoneTransform[Bone], vRelative, Position, false);

				Scale = pObject->Scale * 0.7000000;
				CreateParticleSync(BITMAP_WATERFALL_3, Position, pObject->Angle, Light, 14, Scale, NULL);
				if (rand() % 2 == 0)
				{
					Scale = pObject->Scale * 0.4000000;
					CreateParticleSync(BITMAP_WATERFALL_2, Position, pObject->Angle, Light, 2, Scale, NULL);
				}

				Bone = CustomWorldFindBone(pObject, pModel, "Bone01");
				pModel->TransformPosition(BoneTransform[Bone], vRelative, Position, false);

				Scale = pObject->Scale;
				Position[2] += 45.00000;
				CreateParticleSync(BITMAP_WATERFALL_3, Position, pObject->Angle, Light, 14, Scale, NULL);
				if (rand() % 2 == 0)
				{
					CreateParticleSync(BITMAP_WATERFALL_2, Position, pObject->Angle, Light, 2, Scale, NULL);
				}
			}
			break;
			case 95:
			{
				Vector(0.0, 0.0, 0.0, vRelative);
				Vector(1.000000, 1.000000, 1.000000, Light);
				char* BoneName1[] = { "e01", "e02", "e04", "e03", };
				char* BoneName2[] = { "e05", "e06", "e08", "e07", };

				for (int i = 0; i < 4; i++)
				{
					Scale = pObject->Scale * 0.5000000;
					int Bone = CustomWorldFindBone(pObject, pModel, BoneName1[i]);
					pModel->TransformPosition(BoneTransform[Bone], vRelative, Position, false);
					CreateParticleSync(BITMAP_WATERFALL_3, Position, pObject->Angle, Light, 14, Scale, NULL);
					if (rand() % 2 == 0)
					{
						CreateParticleSync(BITMAP_WATERFALL_2, Position, pObject->Angle, Light, 2, Scale, NULL);
					}
				}
				for (int i = 0; i < 4; i++)
				{
					Scale = pObject->Scale * 0.5000000;
					int Bone = CustomWorldFindBone(pObject, pModel, BoneName2[i]);
					pModel->TransformPosition(BoneTransform[Bone], vRelative, Position, false);
					CreateParticleSync(BITMAP_WATERFALL_3, Position, pObject->Angle, Light, 14, Scale, NULL);
					if (rand() % 2 == 0)
					{
						CreateParticleSync(BITMAP_WATERFALL_2, Position, pObject->Angle, Light, 2, Scale, NULL);
					}
				}
			}
			break;
			case 123:
			{
				Vector(0.5f, 0.9f, 0.5f, Light);
				Vector(0.0, 0.0, 20.00000, vRelative);

				int Bone = CustomWorldFindBone(pObject, pModel, "Crowd01");
				pModel->TransformPosition(BoneTransform[Bone], vRelative, Position, false);
				Position[2] += 40.00000;

				if (rand() % 4 == 0)
				{
					CreateParticleSync(BITMAP_SMOKE, Position, pObject->Angle, Light, 60, pObject->Scale, 0);
					CreateParticleSync(BITMAP_SMOKE, Position, pObject->Angle, Light, 13, pObject->Scale, pObject);
				}
			}
			break;
			case 128:
				if (rand() % 34 == 0)
				{
					float fLumi = (rand() % 10) * 0.05f + 0.03f;
					Vector(256.f / 256.f * fLumi, 256.f / 256.f * fLumi, 256.f / 256.f * fLumi, Light);
					CreateParticleSync(BITMAP_GHOST_CLOUD1, pObject->Position, pObject->Angle, Light, 0, pObject->Scale, pObject);
				}
				break;
			}
		}
		return true;
	}
	if (index == 81)
	{
	LABEL_58:
		Vector(0.0, 0.0, 0.0, vRelative);
		Vector(0.5000000, 0.8000000, 1.000000, Light);
		if (index == 81)
		{
			Vector(1.000000, 0.5000000, 0, Light);
		}
		if (rand() % 1 == 0)
		{
			Scale = 0.2000000;
			CreateParticleSync(BITMAP_LIGHT + 3, pObject->Position, pObject->Angle, Light, 6, Scale, NULL);
		}
		char* BoneName[] = { "Point02", "Point03", "objSphere002", };

		for (int i = 0; i < 3; i++)
		{
			int Bone = CustomWorldFindBone(pObject, pModel, BoneName[i]);
			pModel->TransformPosition(BoneTransform[Bone], vRelative, Position, false);
			Scale = 1.500000;
			CreateSprite(BITMAP_LIGHT, Position, Scale, Light, NULL, 0.0, 3);
		}
		return true;
	}
	if (index > 33)
	{
		switch (index)
		{
		case 36:
		{
			Vector(0.0, 0.0, 0.0, vRelative);
			Vector(0.5000000, 0.8000000, 1.000000, Light);
			char* BoneName[] = { "e01", "e02", "e03", "e04", "e05", "e06", };
			for (int i = 0; i < 6; i++)
			{
				int Bone = CustomWorldFindBone(pObject, pModel, BoneName[i]);
				pModel->TransformPosition(BoneTransform[Bone], vRelative, Position, false);

				Scale = 2.000000;
				CreateSprite(BITMAP_SPARK + 1, Position, Scale, Light, NULL, 0.0, 3);
				Scale = 1.500000;
				CreateSprite(BITMAP_SPARK + 1, Position, Scale, Light, NULL, 0.0, 3);
			}
		}
		return true;
		case 71:
			/*sub_93E95E(v220, SLODWORD(0.3f), 0.2f, 0.2f);
			v301 = 24;
			sub_93E95E(
				v221,
				*(DWORD*)(a9 + 304),
				*(DWORD*)(a9 + 308),
				COERCE_INT(*(float*)(a9 + 312) + *(float*)&dword_18374C0));
			LOBYTE(v301) = 25;
			v275 = sub_11B5EE9(0.0, *(float*)&flt_1836170, 1u);
			if (v275 == 0.0)
			{
				HIDWORD(v148) = a9;
				LODWORD(v148) = *(DWORD*)(a9 + 144);
				v110 = (float*)sub_DF3CE5(v220);
				v87 = (float*)sub_DF3CE5((void*)(a9 + 316));
				v26 = (float*)sub_DF3CE5(v221);
				CreateParticleSync(BITMAP_SMOKE, v26, v87, v110, 69, v148);
			}
			LOBYTE(v301) = 24;
			sub_93773F();
			v301 = -1;
			sub_93773F();*/
			return true;
		case 72:
			/*sub_9F95E7(v218, SLODWORD(alpha));
			v301 = 26;
			HIDWORD(v111) = -1;
			LODWORD(v111) = *(DWORD*)(a9 + 144);
			v72 = sub_DF3CE5(v218);
			v69 = sub_DF3CE5((void*)(a9 + 316));
			v27 = sub_DF3CE5((void*)(a9 + 304));
			sub_137F914(283, v27, (int)v69, (int)v72, 10, 0, -1, 0, 0, 0, v111, 0i64);
			sub_93E95E(v219, SLODWORD(alpha), 0.2f, 0);
			LOBYTE(v301) = 27;
			HIDWORD(v77) = sub_DF3CE5(v219);
			*(float*)&v77 = 2.000000 * *(float*)(a9 + 144);
			v28 = sub_DF3CE5((void*)(a9 + 304));
			sub_153F324(BITMAP_LIGHT, v28, v77, a9, 0, 0, 4);
			LOBYTE(v301) = 26;
			sub_93773F();
			v301 = -1;
			sub_93773F();*/
			return true;
		}
		if (index != 75)
			return false;
		goto LABEL_58;
	}
	if (index == 33)
	{
		if (!(rand() % 20))
		{
			Vector(0.05000000, 0.0, 0.0, Light);
			CreateParticleSync(BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 20, pObject->Scale, NULL);
		}
	}
	else if (index != 6)
	{
		switch (index)
		{
		case 9:
			if (!(rand() % 20))
			{
				Vector(0.05000000, 0.0, 0.0, Light);
				CreateParticleSync(BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 20, pObject->Scale, NULL);
			}
			break;
		case 17:
		{
			Vector(0.0, 0.0, 0.0, Position);
			Vector(0.0, 0.0, 0.0, vRelative);
			Vector(1.000000, 1.000000, 1.000000, Light);

			int Bone = CustomWorldFindBone(pObject, pModel, "mansionA007");
			pModel->TransformPosition(BoneTransform[Bone], vRelative, Position, false);
			if (sub_11B5EE9(0.0, 4.000000, 1u) == 0.0)
			{
				CreateParticleSync(BITMAP_FIRE, Position, pObject->Angle, Light, 0, 1.0, NULL);
			}
			Position[2] += 110.0;
			if (sub_11B5EE9(0.0, 4.000000, 1u) == 0.0)
			{
				Scale = pObject->Scale;
				CreateParticleSync(BITMAP_SMOKE, Position, pObject->Angle, Light, 2, Scale, NULL);
			}
		}
		break;
		case 28:
			if (!(rand() % 20))
			{
				Vector(0, 0.03000000, 0, Light);
				CreateParticleSync(BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 20, pObject->Scale, NULL);
			}
			break;
		case 32:
			if (!(rand() % 20))
			{
				Vector(0.03000000, 0, 0.06000000, Light);
				CreateParticleSync(BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 20, pObject->Scale, NULL);
			}
			break;
		default:
			return false;
		}
	}
	return true;
}

bool CGMFerea::RenderObjectMesh(OBJECT* pObject, BMD* pModel, bool ExtraMon)
{
	if (IsCurrentMap(World) == false)
		return false;

	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return false;

	int index = (pObject->Type);

	float iluminaty, RenderAlpha, BlendMeshTexCoordV;

	switch (index)
	{
	case 43:
	case 51:
	case 75:
	case 81:
	case 87:
	case 114:
	case 153:
	case 155:
		pObject->m_bRenderAfterCharacter = true;
		break;
	case 6:
	case 9:
	case 28:
	case 33:
	case 71:
	case 72:
	case 129:
	case 130:
	case 140:
	case 141:
	case 142:
		return true;
	}
	if (index > 115)
	{
		if (index > 155)
		{
			return false;
		}
		else
		{
			switch (index)
			{
			case 155:
				goto LABEL_42;
			case 123:
				pModel->RenderBody(2, 1.000000, -1, pObject->BlendMeshLight, 0, 0, -1, -1);
				return true;
			case 138:
				iluminaty = (float)(WorldTime * 0.002000000);
				RenderAlpha = ((float)((float)(iluminaty + 1.000000) * 0.3000000) + 0.2000000);

				pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, 2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(3, RENDER_TEXTURE, pObject->Alpha, 3, RenderAlpha, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(4, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				return true;
			case 153:
			LABEL_42:
				pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				return true;
			}
			if (index != 154)
				return false;
			iluminaty = (float)(WorldTime * 0.002000000);
			RenderAlpha = ((float)((float)(iluminaty + 1.000000) * 1.650000) + 8.750000);

			pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, 1, RenderAlpha, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		}
	}
	else
	{
		if (index == 115)
		{
			iluminaty = (float)(WorldTime * 0.002000000);
			RenderAlpha = (float)((float)((float)(iluminaty + 1.000000) * 0.5000000) * 0.5000000) + 0.5000000;

			pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(1, 2, 1.000000, 1, RenderAlpha, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			return true;
		}
		if (index == 10)
		{
			pModel->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 0, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			return true;
		}
		if (index > 81)
		{
			switch (index)
			{
			case 87:
				BlendMeshTexCoordV = (float)((float)((float)((float)((int)WorldTime % 5000) * 0.0001000000) * 2.000000) * 2.000000) - 1.000000;

				pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(3, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(3, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(4, RENDER_TEXTURE, pObject->Alpha, 4, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, BlendMeshTexCoordV, -1);
				break;
			case 95:
				BlendMeshTexCoordV = (float)((float)((float)((float)((int)WorldTime % 5000) * 0.0001000000) * 2.000000) * 2.000000) - 1.000000;
				pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(3, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(3, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(4, RENDER_TEXTURE, pObject->Alpha, 4, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, BlendMeshTexCoordV, -1);
				break;
			case 114:
				break;
			default:
				return false;
			}
		}
		else
		{
			if (index == 81)
			{
			LABEL_33:
				pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(0, RENDER_CHROME | RENDER_BRIGHT, pObject->Alpha, 0, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(5, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				return true;
			}
			if (index == 39)
			{
				pModel->RenderMesh(0, RENDER_TEXTURE, 1.000000, -1, 1.000000, 0, 0, -1);
				pModel->RenderMesh(1, RENDER_TEXTURE, 1.000000, -1, 1.000000, 0, 0, -1);
				pModel->RenderMesh(2, RENDER_TEXTURE, 1.000000, -1, 1.000000, 0, 0, -1);
				pModel->RenderMesh(4, RENDER_TEXTURE, 1.000000, -1, 1.000000, 0, 0, -1);
				pModel->RenderMesh(5, RENDER_TEXTURE, 1.000000, -1, 1.000000, 0, 0, -1);
				if (pObject->CurrentAction == 1)
				{
					pObject->BlendMeshTexCoordV = pObject->BlendMeshTexCoordV + 0.1000000;
				}
				else if (pObject->CurrentAction == 2)
				{
					pObject->BlendMeshTexCoordV = pObject->BlendMeshTexCoordV - 0.1000000;
				}
				pModel->StreamMesh = 3;
				pModel->RenderMesh(3, RENDER_TEXTURE, 1.000000, -1, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				return true;
			}
			if (index != 43 && index != 51)
			{
				if (index != 75)
					return false;
				goto LABEL_33;
			}
		}
	}
	return true;
}

void CGMFerea::RenderAfterObjectMesh(OBJECT* pObject, BMD* pModel, bool ExtraMon)
{
	if (IsCurrentMap(World) == false)
		return;

	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return;

	int index = (pObject->Type);
	float iluminaty, RenderAlpha, BlendMeshTexCoordU, BlendMeshTexCoordV;

	if (index > 87)
	{
		switch (index)
		{
		case 114:
			BlendMeshTexCoordU = ((float)((float)((float)((float)((int)WorldTime % 33336) * 0.0001000000) * 0.3000000) * 2.000000) - 1.000000);
			BlendMeshTexCoordV = ((float)((float)((float)((float)((int)WorldTime % 33336) * 0.0001000000) * 0.3000000) * 2.000000) - 1.000000);
			pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, 0, pObject->BlendMeshLight, BlendMeshTexCoordU, BlendMeshTexCoordV, -1);
			break;
		case 153:
			BlendMeshTexCoordV = ((float)((float)((float)((float)((int)WorldTime % 10001) * 0.0001000000) * 1.000000) * 2.000000) - 1.000000);
			pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, 1, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, BlendMeshTexCoordV, -1);
			pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, 2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordU, -1);
			break;
		case 155:
			BlendMeshTexCoordU = (float)((float)((float)((float)((int)WorldTime % 16668) * 0.0001000000) * 0.6000000) * 2.000000) - 1.000000;
			pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, 1, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, 2, pObject->BlendMeshLight, BlendMeshTexCoordU, BlendMeshTexCoordU, -1);
			break;
		}
	}
	else
	{
		switch (index)
		{
		case 87:
			BlendMeshTexCoordU = ((float)((float)((float)((float)((int)WorldTime % 6667) * 0.0001000000) * 1.500000) * 2.000000) - 1.000000);
			BlendMeshTexCoordV = ((float)((float)((float)((float)((int)WorldTime % 6667) * 0.0001000000) * 1.500000) * 2.000000) - 1.000000);

			pModel->RenderMesh(5, RENDER_TEXTURE, pObject->Alpha, 5, pObject->BlendMeshLight, BlendMeshTexCoordU, BlendMeshTexCoordV, -1);
			break;
		case 43:
			pModel->RenderBody(RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1, -1);
			break;
		case 51:
			pModel->RenderBody(RENDER_TEXTURE, pObject->Alpha, -1, 1.000000, 0, 0, -1, -1);
			break;
		case 75:
		case 81:
			iluminaty = (float)(WorldTime * 0.007000000);
			RenderAlpha = (float)(iluminaty + 2.000000) * 0.5000000;

			BlendMeshTexCoordV = ((float)((float)((float)((float)((int)WorldTime % 33336) * 0.0001000000) * 0.3000000) * 2.000000) - 1.000000);

			iluminaty = 1.000000;
			if (index == 81)
				iluminaty = 0.7f;

			pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, 2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(3, RENDER_TEXTURE, pObject->Alpha, 3, iluminaty, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(4, RENDER_TEXTURE, pObject->Alpha, 4, RenderAlpha, pObject->BlendMeshTexCoordU, BlendMeshTexCoordV, -1);
			pModel->RenderMesh(6, RENDER_TEXTURE, pObject->Alpha, 6, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			break;
		}
	}
}
