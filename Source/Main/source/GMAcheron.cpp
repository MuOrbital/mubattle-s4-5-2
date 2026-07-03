#include "stdafx.h"
#include "CustomWorldEffectCompat.h"
#include "GMAcheron.h"
#include "ZzzEffect.h"

CGMAcheronPtr CGMAcheron::Make()
{
	CGMAcheronPtr pkfield(new CGMAcheron);
	return pkfield;
}

CGMAcheron::CGMAcheron()
{
}

CGMAcheron::~CGMAcheron()
{
}

bool CGMAcheron::LoadMapData()
{
	char FileName[100];

	sprintf(FileName, "Object%d\\cuttree01A_R.jpg", World + 1);
	LoadBitmap(FileName, BITMAP_map_texture01, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);

	sprintf(FileName, "Object%d\\cuttree01A_base.jpg", World + 1);
	LoadBitmap(FileName, BITMAP_map_texture02, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);

	sprintf(FileName, "Object%d\\jung03_R.jpg", World + 1);
	LoadBitmap(FileName, BITMAP_map_texture03, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);

	sprintf(FileName, "Object%d\\jung03_base.jpg", World + 1);
	LoadBitmap(FileName, BITMAP_map_texture04, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);

	return true;
}

bool CGMAcheron::MoveObject(OBJECT* pObject)
{
	if (IsCurrentMap(World) == false)
		return false;

	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return false;

	int Index = pObject->Type;

	switch (Index)
	{
	case 26:
	case 25:
		pObject->BlendMeshTexCoordV -= timefac(0.03000000);
		return true;
	case 23:
	case 24:
		pObject->BlendMeshTexCoordV -= timefac(0.02000000);
		return true;
	case 22:
		pObject->BlendMeshTexCoordV -= timefac(0.01000000);
		return true;
	case 78:
		pObject->BlendMeshTexCoordU += timefac(0.01000000);
		return true;
	}

	return true;
}

bool CGMAcheron::CreateObject(OBJECT* pObject)
{
	if (IsCurrentMap(World) == false)
		return false;

	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return false;

	int IndexObject = (pObject->Type);

	switch (IndexObject)
	{
		//case 38:
	case 54:
	case 55:
	case 56:
	case 57:
	case 69:
	case 80:
		pObject->HiddenMesh = -2;
		return true;
	}
	return false;
}

bool CGMAcheron::RenderObjectVisual(OBJECT* pObject, BMD* pModel)
{
	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return false;

	float Scale;
	vec3_t Light, Position, vPos;
	int IndexObject = (pObject->Type);

	switch (IndexObject)
	{
	case 31:
		Vector(0.0, 0.0, 0.0, vPos);
		Vector(0.0, 0.0, 0.0, Position);

		Scale = 0.20000000;
		Light[0] = 0.8000000;
		Light[1] = 0.9400000;
		Light[2] = 0.9400000;
		pModel->TransformPosition(BoneTransform[72], vPos, Position, false);
		CreateSprite(BITMAP_SPARK + 1, Position, Scale, Light, pObject, 0, 0);

		Scale = 1.2000000;
		Light[0] = 0.7800000;
		Light[1] = 0.4400000;
		Light[2] = 0.7500000;
		pModel->TransformPosition(BoneTransform[84], vPos, Position, false);
		CreateSprite(BITMAP_SPARK + 1, Position, Scale, Light, pObject, 0, 0);

		Scale = 0.8000000;
		Light[0] = 0.5800000;
		Light[1] = 0.0700000;
		Light[2] = 0.5600000;
		pModel->TransformPosition(BoneTransform[96], vPos, Position, false);
		CreateSprite(BITMAP_SPARK + 1, Position, Scale, Light, pObject, 0, 0);
		return true;
	case 38:

		if (!(rand() % 6))
		{
			Vector(0.02000000, 0.02000000, 0.05000000, Light);
			CreateParticleSync(BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 20, pObject->Scale, pObject);
		}
		return true;
	case 54:
		if (!(rand() % 2))
		{
			Light[0] = 1.0;
			Light[1] = 1.0;
			Light[2] = 1.0;
			Scale = pObject->Scale;
			CreateParticleSync(BITMAP_WATERFALL_1, pObject->Position, pObject->Angle, Light, 2, Scale, NULL);
		}
		return true;
	case 55:
		Light[0] = 1.0;
		Light[1] = 1.0;
		Light[2] = 1.0;
		Scale = pObject->Scale * 2.000000;
		CreateParticleSync(BITMAP_WATERFALL_3, pObject->Position, pObject->Angle, Light, 14, Scale, NULL);
		return true;
	case 56:
		if (!(rand() % 3))
		{
			Light[0] = 1.0;
			Light[1] = 1.0;
			Light[2] = 1.0;
			Scale = pObject->Scale;
			CreateParticleSync(BITMAP_WATERFALL_2, pObject->Position, pObject->Angle, Light, 2, Scale, NULL);
		}
		return true;
	case 69:
		if (!(rand() % 3))
		{
			Scale = 0.2f;
			Light[0] = 0.4f;
			Light[1] = 0.6f;
			Light[2] = 0.7f;
			CreateParticleSync(BITMAP_TWINTAIL_WATER, pObject->Position, pObject->Angle, Light, 1, Scale, NULL);
		}
		return true;
	case 80:
		if (!(rand() % 8))
		{
			Scale = pObject->Scale;
			Vector(1.0, 1.0, 1.0, Light);
			CreateParticleSync(BITMAP_SMOKE, pObject->Position, pObject->Angle, Light, 69, Scale, NULL);
		}
		return true;
	case 9:
		Scale = 0.7f;
		Vector(0.0, 0.0, 0.0, vPos);
		Vector(0.0, 0.0, 0.0, Position);

		pModel->TransformPosition(BoneTransform[72], vPos, Position, false);
		Light[0] = 0.9372549;
		Light[1] = 0.2196078;
		Light[2] = 1.500000;
		CreateSprite(BITMAP_SPARK + 1, Position, Scale, Light, pObject, 0, 0);

		pModel->TransformPosition(BoneTransform[132], vPos, Position, false);
		Light[0] = 0.9372549;
		Light[1] = 0.2784314;
		Light[2] = 0.3019608;
		CreateSprite(BITMAP_SPARK + 1, Position, Scale, Light, pObject, 0, 0);

		//pModel->TransformPosition(BoneTransform[204], vPos, Position, false);
		//Light[0] = 1.0;
		//Light[1] = 0.1600000;
		//Light[2] = 0.1607843;
		//CreateSprite(BITMAP_SPARK+1, Position, Scale, Light, pObject, 0, 0);
		return true;
	case 89:
		for (int i = 18; i < 26; ++i)
		{
			if (i == 23 || i == 22 || i == 21 || i == 20 || i == 18)
				Scale = 1.800000;
			else
				Scale = 0.7f;
			Light[0] = 0.1600000;
			Light[1] = 0.1607843;
			Light[2] = 0.6f;

			vec3_t vPos;
			Vector(0.0, 0.0, 0.0, vPos);
			Vector(0.0, 0.0, 0.0, Position);

			pModel->TransformPosition(BoneTransform[i], vPos, Position, false);
			CreateSprite(BITMAP_SPARK + 1, Position, Scale, Light, pObject, 0, 0);
			CreateSprite(BITMAP_SPARK + 1, Position, Scale, Light, pObject, 0, 0);
		}
		return true;
	}
	return false;
}

bool CGMAcheron::RenderObjectMesh(OBJECT* pObject, BMD* pModel, bool ExtraMon)
{
	if (IsCurrentMap(World) == false)
		return false;

	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return false;

	float RenderAlpha = 1.0;
	int IndexObject = (pObject->Type);

	if (IndexObject == 101 || IndexObject == 102)
	{
		pObject->m_bRenderAfterCharacter = true;
		return true;
	}
	if (IndexObject <= 65)
	{
		if (IndexObject != 65)
		{
			switch (IndexObject)
			{
			case 7:
			case 8:
			case 17:
			case 22:
			case 23:
			case 24:
			case 25:
			case 26:
			case 28:
			case 30:
			case 31:
			case 32:
			case 33:
			case 36:
			case 47:
			case 61:
			case 62:
				//-- articulos para effectos
				goto LABEL_47;
			case 38:
			case 54:
			case 55:
			case 56:
			case 57:
			case 69:
			case 80:
				return true;
			case 9:
			case 14:
			case 27:
			case 51:
			case 59:
				goto LABEL_43;
			case 15:
			case 60:
				pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pObject->m_bRenderAfterCharacter = true;
				return true;
			case 16:
			case 52:
				pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pObject->m_bRenderAfterCharacter = true;
				return true;
			case 48:
				pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_map_texture02);

				pObject->Light[0] = WorldTime * 0.9400000;
				pObject->Light[1] = 0.9372549;
				pObject->Light[2] = 0.8396000;
				RenderAlpha = (float)(pObject->Light[0] * 1.000000) + 0.6000000;

				pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, -2, RenderAlpha, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_map_texture01);
				pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, -2, RenderAlpha, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_map_texture01);
				pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				return true;
			default:
				return false;
			}
		}
		pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pObject->m_bRenderAfterCharacter = true;
		return true;
	}
	if (IndexObject > 91)
	{
		if (IndexObject > 109)
		{
			if (IndexObject != 110)
			{
				return false;
			}
			pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderBody(RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, pObject->HiddenMesh, -1);
			pObject->m_bRenderAfterCharacter = true;
			return true;
		}
		if (IndexObject < 107 && IndexObject != 92)
		{
			if (IndexObject == 93)
				goto LABEL_47;
			if (IndexObject != 100)
			{
				if (IndexObject <= 100 || IndexObject > 102)
				{
					return false;
				}
			}
		}
	LABEL_43:
		pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pObject->m_bRenderAfterCharacter = true;
		return true;
	}
	if (IndexObject >= 90 || IndexObject == 69 || IndexObject == 80)
		goto LABEL_47;
	if (IndexObject > 79)
	{
		if (IndexObject != 87)
		{
			if (IndexObject == 88)
			{
			LABEL_40:
				pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pObject->m_bRenderAfterCharacter = true;
				return true;
			}
			if (IndexObject != 89)
				return false;
		}
		goto LABEL_43;
	}
	if (IndexObject >= 77)
		goto LABEL_47;
	if (IndexObject == 66)
	{
		pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pObject->m_bRenderAfterCharacter = true;
	}
	else
	{
		if (IndexObject != 68)
		{
			if (IndexObject <= 70)
				return false;
			if (IndexObject > 72)
			{
				if (IndexObject != 76)
					return false;
				goto LABEL_40;
			}
		LABEL_47:
			pObject->m_bRenderAfterCharacter = true;
			return true;
		}
		pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_map_texture04);
		pModel->StreamMesh = 0;
		pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, -2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, ((float)((int)WorldTime % 4000) * 0.0002500000), BITMAP_map_texture03);
		pModel->StreamMesh = -1;
		pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
	}

	return true;
}

void CGMAcheron::RenderAfterObjectMesh(OBJECT* pObject, BMD* pModel, bool ExtraMon)
{
	if (IsCurrentMap(World) == false)
		return;

	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return;

	float RenderAlpha = 1.0;
	int IndexObject = (pObject->Type);

	switch (IndexObject)
	{
	case 7:
	case 8:
	case 0x16:
	case 0x17:
	case 0x18:
	case 0x19:
	case 0x1A:
	case 0x1E:
	case 0x1F:
	case 0x20:
	case 0x21:
	case 0x24:
	case 0x2F:
	case 0x3D:
	case 0x3E:
	case 0x47:
	case 0x48:
	case 0x4E:
	case 0x5A:
	case 0x5B:
	case 0x5D:
		pModel->RenderBody(RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, pObject->HiddenMesh, -1);
		break;
	case 0x65:
	case 0x66:
		pModel->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 0, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		break;
	case 9:
		pModel->StreamMesh = 2;
		pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, -2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, ((float)((int)WorldTime % 10000) * 0.0003200000), -1);
		pModel->StreamMesh = -1;
		pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, -2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		break;
	case 0xE:
	case 0x33:
	case 0x57:
	case 0x59:
	case 0x64:
		pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, -2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		break;
	case 0xF:
		pModel->StreamMesh = 0;
		pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, -2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, ((float)((int)WorldTime % 10000) * 0.0001000000), -1);
		pModel->StreamMesh = -1;
		break;
	case 0x10:
	case 0x34:
		pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, -2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		break;
	case 0x11:
		pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, -2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->BeginRender(pObject->Alpha);
		pModel->RenderMesh(0, RENDER_CHROME7 | RENDER_DARK, 0.35f, 0, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_CHROME);
		pModel->EndRender();
		break;
	case 0x1B:
		pObject->Light[0] = WorldTime * 0.001000000;
		RenderAlpha = (float)(pObject->Light[0] * 0.001000000);
		pModel->StreamMesh = 2;
		pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, -2, pObject->BlendMeshLight, (RenderAlpha * 0.1800000), ((float)((int)WorldTime % 10000) * 0.0001000000), -1);
		pModel->StreamMesh = 2;
		pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, -2, ((float)(RenderAlpha * 0.80f) + 1.000000), pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->StreamMesh = -1;
		break;
	case 0x1C:
		pObject->Light[0] = WorldTime * 0.9400000;
		pObject->Light[1] = 0.9372549;
		pObject->Light[2] = 0.8396000;
		RenderAlpha = (float)(pObject->Light[0] * 0.3f) + 0.5f;
		pModel->RenderBody(RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, RenderAlpha, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, pObject->HiddenMesh, -1);
		break;
	case 0x3B:
		pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, -2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, -2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		break;
	case 0x3C:
	case 0x4D:
	case 0x58:
		pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, -2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		break;
	case 0x41:
		pModel->RenderMesh(2, 576, pObject->Alpha, -2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->RenderMesh(0, RENDER_CHROME | RENDER_BRIGHT, pObject->Alpha, -2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->StreamMesh = 1;
		pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, -2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, ((float)((int)WorldTime % 10000) * 0.001000000), -1);
		pModel->StreamMesh = -1;
		break;
	case 0x42:
		pModel->BeginRender(pObject->Alpha);
		pModel->RenderMesh(1, RENDER_CHROME7 | RENDER_DARK, pObject->Alpha, -2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_CHROME);
		pModel->EndRender();
		break;
	case 0x4C:
		pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, -2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->StreamMesh = 3;
		pModel->RenderMesh(3, RENDER_TEXTURE, pObject->Alpha, -2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, ((float)((int)WorldTime % 10000) * 0.0003000000), -1);
		pModel->StreamMesh = -1;
		break;
	case 0x4F:
		pModel->BeginRender(pObject->Alpha);
		pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, -2, 1.0, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->RenderMesh(0, RENDER_CHROME7 | RENDER_DARK, 0.7f, 0, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_CHROME);
		pModel->EndRender();
		break;
	case 0x5C:
		pObject->Light[0] = WorldTime * 0.001000000;
		pObject->Light[1] = WorldTime * 0.001000000;
		pObject->Light[2] = 0.8396000;
		RenderAlpha = (float)(pObject->Light[0] * 0.1800000);

		pModel->StreamMesh = 2;
		pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, -2, pObject->BlendMeshLight, RenderAlpha, ((float)((int)WorldTime % 10000) * 0.0001000000), -1);
		pModel->StreamMesh = 2;
		pObject->Light[0] = WorldTime * 0.001000000;
		pObject->Light[1] = 0.9372549;
		pObject->Light[2] = 0.8396000;

		RenderAlpha = (float)(pObject->Light[0] * 0.80f) + 0.6000000;
		pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, -2, RenderAlpha, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->StreamMesh = -1;
		break;
	case 0x6B:
	case 0x6C:
	case 0x6D:
		pObject->Light[0] = WorldTime * 0.9400000;
		pObject->Light[1] = 0.9372549;
		pObject->Light[2] = 0.8396000;
		pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, -2, (pObject->Light[2] * 0.99f), pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		break;
	case 0x6E:
		pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, -2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->BeginRender(pObject->Alpha);
		pModel->RenderMesh(1, RENDER_CHROME7 | RENDER_DARK, pObject->Alpha, -2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_CHROME);
		pModel->EndRender();
		break;
	default:
		return;
	}
}
