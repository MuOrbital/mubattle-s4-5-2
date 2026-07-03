#include "stdafx.h"
#include "CustomWorldEffectCompat.h"
#include "ZzzEffect.h"
#include "ZzzCharacter.h"
#include "ZzzLodTerrain.h"
#include "GMZoneNars.h"

CGMZoneNarsPtr CGMZoneNars::Make()
{
	CGMZoneNarsPtr pkfield(new CGMZoneNars);
	return pkfield;
}

CGMZoneNars::CGMZoneNars()
{
}

CGMZoneNars::~CGMZoneNars()
{
}

bool CGMZoneNars::MoveObject(OBJECT* pObject)
{
	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return false;

	int index = (pObject->Type);

	if (index != 2)
	{
		vec3_t Light;

		float mul = (float)(rand() % 5) * 0.04;

		switch (index)
		{
			break;
		case 4:
			Light[0] = (0.8 * mul);
			Light[1] = (0.3 * mul);
			Light[2] = (0.2 * mul);
			AddTerrainLight(pObject->Position[0], pObject->Position[1], Light, 1, PrimaryTerrainLight);
			break;
		case 77:
			pObject->BlendMeshTexCoordV = pObject->BlendMeshTexCoordV - 0.01500000;
			break;
		case 116:
			Light[0] = (0.8 * mul);
			Light[1] = (0.3 * mul);
			Light[2] = (0.2 * mul);
			AddTerrainLight(pObject->Position[0], pObject->Position[1], Light, 1, PrimaryTerrainLight);
			break;
		case 134:
			Light[0] = (0.8 * mul);
			Light[1] = (0.3 * mul);
			Light[2] = (0.2 * mul);
			AddTerrainLight(pObject->Position[0], pObject->Position[1], Light, 2, PrimaryTerrainLight);
			break;
		case 146:
			Light[0] = (0.8 * mul);
			Light[1] = (0.3 * mul);
			Light[2] = (0.2 * mul);
			AddTerrainLight(pObject->Position[0], pObject->Position[1], Light, 1, PrimaryTerrainLight);
			break;
		case 142:
			pObject->m_bRenderAfterCharacter = true;
			break;
		}
		return true;
	}
	return true;
}

bool CGMZoneNars::CreateObject(OBJECT* pObject)
{
	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return false;

	int index = (pObject->Type);

	if (index <= 32)
	{
		if (index == 32)
		{
			pObject->HiddenMesh = -2;
		}
		else
		{
			switch (index)
			{
			case 0:
				pObject->Distance = -2800.000;
				pObject->Scale = 17.00000;
				pObject->Angle[2] = 340.0000;
				pObject->CollisionRange = -30000.00;
				return true;
			case 6:
				pObject->CollisionRange = -200.0000;
				pObject->m_bRenderAfterCharacter = true;
				return true;
			case 8:
			case 9:
				pObject->CollisionRange = -800.0000;
				return true;
			case 10:
			case 14:
			case 15:
			case 25:
			case 26:
			case 29:
			case 30:
				pObject->HiddenMesh = -2;
				return true;
			case 11:
			case 12:
				pObject->CollisionRange = -200.0000;
				return true;
			case 16:
				pObject->m_bRenderAfterCharacter = true;
				return true;
			case 17:
				pObject->CollisionRange = -400.0000;
				pObject->m_bRenderAfterCharacter = true;
				goto LABEL_34;
			case 19:
			LABEL_34:
				pObject->CollisionRange = -400.0000;
				break;
			case 21:
			case 22:
			case 23:
			case 24:
				pObject->m_bRenderAfterCharacter = true;
				break;
			case 27:
				pObject->m_bRenderAfterCharacter = true;
				break;
			case 28:
				pObject->CollisionRange = -200.0000;
				break;
			default:
				return true;
			}
		}
		return true;
	}
	if (index > 85)
	{
		if (index != 87)
		{
			switch (index)
			{
			case 134:
				/*v14 = *(DWORD*)(a6 + 88);
				 v7 = sub_93B1DB(a4);
				 v16 = sub_E33A6B(v7, a2, a3, a4, a5, v14);
				 v8 = GetVector((char*)(a6 + 304), 0);
				 *(DWORD*)GetVector(&v16[7].m128i_i8[8], 0) = *(DWORD*)v8;
				 v9 = GetVector((char*)(a6 + 304), 1);
				 *(DWORD*)GetVector(&v16[7].m128i_i8[8], 1) = *(DWORD*)v9;
				 v10 = GetVector((char*)(a6 + 304), 2);
				 *(DWORD*)GetVector(&v16[7].m128i_i8[8], 2) = *(DWORD*)v10;
				 v16[7].m128i_i32[1] = *(DWORD*)(a6 + 144);
				 v13 = (float*)sub_DF3CE5((void*)(a6 + 328));
				 sIndex = sub_DF3CE5((void*)(a6 + 316));
			 BMD:Animations(v16->m128i_i16, (int)flt_6C931B0, *(float*)(a6 + 192), *(float*)(a6 + 196), *(WORD*)(a6 + 44), sIndex, v13, 0, 1);
			 sIndex[0] = 0;
			 sIndex[1] = 0;
			 sIndex[2] = 0;
			 v12 = (float*)sub_DF3CE5((void*)(a6 + 244));
			 sub_1261D54(v16, &flt_6C931B0[48], (float*)sIndex, v12, 0);*/
				break;
			case 142:
				pObject->CollisionRange = -400.0000;
				pObject->m_bRenderAfterCharacter = true;
				break;
			case 146:
				pObject->HiddenMesh = -2;
				break;
			case 150:
				pObject->CollisionRange = -400.0000;
				pObject->m_bRenderAfterCharacter = true;
				break;
			case 154:
				pObject->CollisionRange = -400.0000;
				break;
			}
			return true;
		}
		goto LABEL_45;
	}
	if (index == 85)
	{
	LABEL_45:
		pObject->m_iAnimation = 300 * (rand() % 10);
		return true;
	}
	if (index > 60)
	{
		if (index != 64)
		{
			if (index == 66)
			{
				pObject->HiddenMesh = -2;
			}
			else if (index == 79)
			{
				pObject->HiddenMesh = -2;
			}
			return true;
		}
	}
	else if (index != 60)
	{
		if (index >= 34)
		{
			if (index <= 38)
			{
				pObject->CollisionRange = -400.0000;
				//pObject->m_bRenderAfterCharacter = true;
			}
			else if (index == 39 || index == 59)
			{
				pObject->HiddenMesh = -2;
			}
		}
		return true;
	}
	pObject->CollisionRange = -400.0000;

	return true;
}

bool CGMZoneNars::RenderObjectVisual(OBJECT* pObject, BMD* pModel)
{
	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return false;

	int index = (pObject->Type);

	float Matrix[3][4];

	vec3_t Angle, Position, p;

	switch (index)
	{
	case 25:
		if (!(rand() % 5))
		{
			Vector(0.0, (float)(rand() % 100), 0.0, p);
			Vector(0.0, 0.0, (float)(rand() % 100), Angle);

			AngleMatrix(Angle, Matrix);
			VectorRotate(p, Matrix, Position);
			VectorAdd(Position, pObject->Position, Position);
			CreateParticleSync(BITMAP_FLARE_BLUE, Position, pObject->Angle, pObject->Light, 1, 0.2f, NULL);
		}
		break;
	case 26:
		if (!(rand() % 5))
		{
			/*v117[0] = 0.0;
			v117[1] = (float)(rand() % 100);
			v117[2] = 0.0;

			v129[0] = 0.0;
			v129[1] = 0.0;
			v130 = (float)(rand() % 100);

			sub_DF6683(a5, v129, (int)v92);
			sub_DF740A(v117, v92, &v164);
			v22 = GetVector((char*)(a9 + 304), 0);
			v164 = v164 + *(float*)v22;
			v23 = GetVector((char*)(a9 + 304), 1);
			v165 = v165 + *(float*)v23;
			v24 = GetVector((char*)(a9 + 304), 2);
			v166 = v166 + *(float*)v24;
			*(GLfloat*)GetVector((char*)(a9 + 208), 0) = alpha;
			*(GLfloat*)GetVector((char*)(a9 + 208), 1) = 0.5f;
			*(GLfloat*)GetVector((char*)(a9 + 208), 2) = qword_1835DC4;
			v78 = 0.2f;
			v71 = (float*)sub_DF3CE5((void*)(a9 + 208));
			v25 = (float*)sub_DF3CE5((void*)(a9 + 316));
			CreateParticle(32655, &v164, v25, v71, 2, v78);
			v166 = v166 + *(float*)&dword_1837068;
			v130 = *(float*)&dword_18374AC;*/
		}
		break;
	case 29:
		if (!(rand() % 6))
		{
			vec3_t Light;
			Vector(0.01000000, 0.02000000, 0.05000000, Light);
			CreateParticleSync(BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 20, pObject->Scale, pObject);
		}
		break;
	case 30:
		if (!(rand() % 3))
		{
			vec3_t Light;
			Vector(0.9, 0.2, 0.2, Light);
			CreateParticleSync(BITMAP_SMOKE, pObject->Position, pObject->Angle, Light, 69, pObject->Scale, pObject);
		}
		break;
	}
	return false;
}

bool CGMZoneNars::RenderObjectMesh(OBJECT* pObject, BMD* pModel, bool ExtraMon)
{
	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return false;

	int index = (pObject->Type);

	switch (index)
	{
	case 10:
	case 14:
	case 15:
	case 25:
	case 26:
	case 29:
	case 30:
	case 32:
	case 39:
	case 59:
	case 66:
	case 146:
		return true;
	}

	if (index > 24)
	{
		if (index < 34)
			return false;
		if (index <= 38)
		{
			pObject->m_bRenderAfterCharacter = true;
			pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			return true;
		}
		if (index != 77)
		{
			return false;
		}
	LABEL_15:
		pModel->StreamMesh = 0;
		pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->StreamMesh = -1;
		return true;
	}
	if (index >= 23)
	{
		pModel->RenderBody(RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, 2, -1);
		return true;
	}
	switch (index)
	{
	case 0:
		goto LABEL_15;
	case 6:
		pObject->m_bRenderAfterCharacter = true;
		pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		break;
	case 17:
		pObject->m_bRenderAfterCharacter = true;
		pModel->RenderBody(RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1, -1);
		break;
	default:
		return false;
	}
	return true;
}

void CGMZoneNars::RenderAfterObjectMesh(OBJECT* pObject, BMD* pModel, bool ExtraMon)
{
	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return;
	int index = (pObject->Type);

	if (index > 24)
	{
		if (index == 27)
			goto LABEL_23;
		if (index <= 33)
			return;
		if (index <= 38)
		{
			pModel->BeginRender(pObject->Alpha);
			pModel->StreamMesh = 4;
			pModel->RenderMesh(4, RENDER_TEXTURE, pObject->Alpha, 4, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, ((float)(-(int)WorldTime % 2000) * 0.0005000000), -1);
			pModel->StreamMesh = -1;
			pModel->RenderMesh(3, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(3, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(3, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(3, RENDER_CHROME7 | RENDER_DARK, 1.000000, 3, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_CHROME);
			pModel->RenderMesh(3, RENDER_CHROME7 | RENDER_DARK, 1.000000, 3, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_CHROME);
			pModel->EndRender();
			return;
		}
		if (index == 142)
		{
			float iluminaty = (float)(WorldTime * 0.003000000);
			float RenderAlpha = ((float)((float)(iluminaty + 1.000000) * 0.1500000) + 0.7000000);
			pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, 0, RenderAlpha, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			return;
		}
		if (index == 150)
			goto LABEL_23;
	}
	else
	{
		if (index >= 23)
		{
			float iluminaty = (float)(WorldTime * 0.005000000);
			float RenderAlpha = ((float)((float)(iluminaty + 1.000000) * 0.2000000) + 0.6000000);
			pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, 2, RenderAlpha, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			return;
		}
		if (index == 0)
		{
			pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			return;
		}
		if (index == 6 || index > 15 && (index <= 17 || index > 20))
		{
		LABEL_23:
			pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, 0, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			return;
		}
	}
}

void CGMZoneNars::RenderFrontSideVisual()
{
	if ((TerrainWall[TERRAIN_INDEX_REPEAT(Hero->PositionX, Hero->PositionY)] & 0x1000) != 0)
	{
		EnableAlphaTest(true);

		glColor3f(0.40000001, 0.40000001, 0.44999999);

		float WindX = ((int)WorldTime % 100000) * 0.0005;

		RenderBitmapUV(BITMAP_CHROME + 3, 0.0, 0.0, pos_right(640), pos_botton(480.0), WindX, 0.0, 3.0, 2.0);

		EnableAlphaBlend();

		float WindX2 = ((int)WorldTime % 100000) * 0.0002;

		RenderBitmapUV(BITMAP_CHROME + 2, 0.0, 0.0, pos_right(640), pos_botton(480.0), WindX2, 0.0, 0.3, 0.3);
	}
}
