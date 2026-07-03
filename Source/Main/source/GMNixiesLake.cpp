#include "stdafx.h"
#include "CustomWorldEffectCompat.h"
#include "ZzzEffect.h"
#include "ZzzCharacter.h"
#include "ZzzLodTerrain.h"
#include "GMNixiesLake.h"

CGMNixiesLakePtr CGMNixiesLake::Make()
{
	CGMNixiesLakePtr pkfield(new CGMNixiesLake);
	return pkfield;
}

CGMNixiesLake::CGMNixiesLake()
{
}

CGMNixiesLake::~CGMNixiesLake()
{
}

bool CGMNixiesLake::LoadMapData()
{
	char FileName[100];

	sprintf_s(FileName, "Object%d\\hstreeuniversalD01_R.jpg", World + 1);
	LoadBitmap(FileName, BITMAP_map_texture02, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);

	sprintf_s(FileName, "Object%d\\hswindattributeA02_R.jpg", World + 1);
	LoadBitmap(FileName, BITMAP_map_texture03, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);

	sprintf_s(FileName, "Object%d\\hswindattributeB02_R.jpg", World + 1);
	LoadBitmap(FileName, BITMAP_map_texture04, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);

	sprintf_s(FileName, "Object%d\\hswindattributeC01_R.jpg", World + 1);
	LoadBitmap(FileName, BITMAP_map_texture05, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);

	sprintf_s(FileName, "Object%d\\hswindattributeC02_R.jpg", World + 1);
	LoadBitmap(FileName, BITMAP_map_texture06, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);

	sprintf_s(FileName, "Object%d\\hsicestatuesA04_R.jpg", World + 1);
	LoadBitmap(FileName, BITMAP_map_texture07, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);

	sprintf_s(FileName, "World%d\\cubemap_up.jpg", World + 1);
	LoadBitmap(FileName, BITMAP_map_texture08, GL_LINEAR, GL_REPEAT, true, false);

	sprintf_s(FileName, "World%d\\TileBright.jpg", World + 1);
	LoadBitmap(FileName, BITMAP_map_texture09, GL_LINEAR, GL_REPEAT, true, false);

	LoadBitmap("Effect\\Chrome08.jpg", BITMAP_map_texture01, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);
	LoadBitmap("Effect\\Map_Smoke1.jpg", BITMAP_CHROME + 2, GL_LINEAR, GL_REPEAT, true, false);
	return true;
}

bool CGMNixiesLake::MoveObject(OBJECT* pObject)
{
	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return false;

	int index = (pObject->Type);

	if (index != 77)
		return false;

	pObject->BlendMeshTexCoordV = (double)((signed int)WorldTime % 2000) * 0.0005000000237487257;

	return true;
}

bool CGMNixiesLake::CreateObject(OBJECT* pObject)
{
	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return false;

	int index = (pObject->Type);

	switch (index)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 7:
	case 8:
	case 0xA:
	case 0xB:
	case 0x17:
	case 0x21:
	case 0x22:
	case 0x33:
	case 0x34:
	case 0x46:
	case 0x47:
	case 0x4E:
	case 0x50:
	case 0x54:
	case 0x55:
	case 0x63:
	case 0x66:
	case 0x73:
	case 0x76:
	case 0x80:
	case 0x82:
	case 0x85:
	case 0x92:
	case 0x96:
	case 0x9D:
	case 0x9F:
		pObject->CollisionRange = 0.05f;
		break;
	case 0x14:
	case 0x25:
	case 0x26:
	case 0x27:
	case 0x28:
	case 0x29:
	case 0x2A:
	case 0x2C:
	case 0x2D:
	case 0x35:
	case 0x69:
	case 0x6B:
	case 0x6C:
	case 0x72:
	case 0x7E:
	case 0x8B:
	case 0x8D:
		pObject->m_bRenderAfterCharacter = true;
		break;
	case 0x15:
	case 0x71:
	case 0x75:
	case 0x77:
	case 0x78:
	case 0x79:
	case 0x7A:
	case 0x7B:
	case 0xA1:
		pObject->HiddenMesh = -2;
		break;
	case 0x6A:
		pObject->CollisionRange = 0.05f;
		pObject->m_bRenderAfterCharacter = 1;
		break;
	default:
		return true;
	}

	return true;
}

bool CGMNixiesLake::RenderObjectVisual(OBJECT* pObject, BMD* pModel)
{
	return false;
}

bool CGMNixiesLake::RenderObjectMesh(OBJECT* pObject, BMD* pModel, bool ExtraMon)
{
	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return false;

	int index = (pObject->Type);

	if (index > 53)
	{
		if (index > 126)
		{
			switch (index)
			{
			case 128:
				pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->BeginRender(pObject->Alpha);
				pModel->RenderMesh(0, RENDER_CHROME7 | RENDER_DARK, pObject->Alpha, -2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_map_texture01);
				pModel->EndRender();
				break;
			case 132:
				pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(3, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 1, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				break;
			case 135:
				pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				break;
			case 138:
				pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(4, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(3, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 3, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				break;
			case 139:
				pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(3, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				break;
			case 141:
				pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(4, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				break;
			default:
				return false;
			}
			return true;
		}
		if (index != 126)
		{
			if (index < 102)
				return false;
			if (index <= 104)
				goto LABEL_49;
			if (index == 105)
			{
				pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(2, RENDER_TEXTURE | RENDER_CHROME, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_map_texture01);
				pModel->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(3, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(3, RENDER_TEXTURE | RENDER_CHROME, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_map_texture01);
				pModel->RenderMesh(3, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 3, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(4, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(4, RENDER_TEXTURE | RENDER_CHROME, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_map_texture01);
				pModel->RenderMesh(4, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 4, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				return true;
			}
			if (index != 106 && index != 107)
			{
				if (index != 114)
					return false;
				pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				return true;
			}
		}
		pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		return true;
	}
	if (index == 53)
	{
		pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->RenderMesh(3, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->RenderMesh(3, RENDER_TEXTURE | RENDER_CHROME, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_map_texture01);
		pModel->RenderMesh(3, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 3, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		return true;
	}
	if (index > 42)
	{
		switch (index)
		{
		case '+':
			pModel->RenderMesh(3, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(0, RENDER_TEXTURE | RENDER_CHROME, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_map_texture01);
			pModel->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 0, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 1, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			return true;
		case ',':
			pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 0, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_map_texture03);
			return true;
		case '-':
			pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 1, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_map_texture03);
			return true;
		case '.':
			pModel->RenderBody(RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1, -1);
			pModel->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 0, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_map_texture05);
			pModel->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 1, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_map_texture06);
			return true;
		}
		if (index > 49)
		{
		LABEL_49:
			pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->BeginRender(pObject->Alpha);
			pModel->RenderMesh(0, RENDER_CHROME7 | RENDER_DARK, pObject->Alpha, -2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_map_texture01);
			pModel->EndRender();
			return true;
		}
		return false;
	}
	switch (index)
	{
	case 42:
		pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->RenderMesh(3, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		break;
	case 20:
		pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 0, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_map_texture02);
		break;
	case 36:
		pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->StreamMesh = 1;
		pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->StreamMesh = -1;
		break;
	case 37:
		pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		break;
	case 38:
		pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 1, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		break;
	case 40:
	{
		pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->RenderMesh(4, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->RenderMesh(1, RENDER_CHROME | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_map_texture01);

		float iluminaty = Render22(-3.000000, 0);
		float RenderAlpha = (float)(iluminaty * 0.7f) + 0.8000000;
		pModel->RenderMesh(3, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 3, RenderAlpha, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
	}
	break;
	case 41:
		pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		pModel->RenderMesh(5, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		break;
	default:
		return false;
	}
	return true;
}

void CGMNixiesLake::RenderAfterObjectMesh(OBJECT* pObject, BMD* pModel, bool ExtraMon)
{
	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return ;

	int index = (pObject->Type);

	float iluminaty, RenderAlpha;

	if (index <= 53)
	{
		if (index == 53)
		{
			pModel->RenderMesh(4, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 4, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			return;
		}
		if (index <= 40)
		{
			if (index == 40)
				pModel->RenderMesh(5, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 5, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			if (index != 20)
			{
				switch (index)
				{
				case '%':
					iluminaty = Render22(-2.000000, 0.0);
					RenderAlpha = (float)(iluminaty * 0.6000000) + 0.4000000;
					pModel->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 0, RenderAlpha, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
					break;
				case '&':
					iluminaty = Render22(-2.000000, 0.0);
					RenderAlpha = Render22(-2.000000, 180.0000);
					pModel->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 2, iluminaty, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
					pModel->RenderMesh(3, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 3, RenderAlpha, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
					break;
				case '\'':
					iluminaty = Render22(-2.000000, 0.0);
					RenderAlpha = (float)(iluminaty * 0.5000000) + 0.5000000;
					pModel->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 0, RenderAlpha, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
					break;
				}
				return;
			}
			pModel->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 1, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			return;
		}
		if (index == 41)
		{
			pModel->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 0, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 1, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(3, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 3, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(4, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 4, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			return;
		}
		if (index != 42)
		{
			if (index == 44)
			{
				pModel->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 1, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
				return;
			}
			if (index != 45)
				return;
		}
		pModel->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 0, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		return;
	}
	if (index > 114)
	{
		switch (index)
		{
		case 126:
			pModel->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 1, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			return;
		case 139:
			pModel->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(4, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 4, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			break;
		case 141:
			pModel->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 2, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(3, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 3, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			break;
		}
	}
	else
	{
		switch (index)
		{
		case 'r':
			iluminaty = Render22(-2.000000, 0.0);
			RenderAlpha = (float)(iluminaty * 0.5000000) + 0.5000000;
			pModel->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 2, RenderAlpha, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(3, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 3, RenderAlpha, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(4, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 4, RenderAlpha, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			return;
		case 'i':
			pModel->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 0, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			return;
		case 'j':
			pModel->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 1, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		case 'k':
			iluminaty = Render22(-2.000000, 0.0);
			RenderAlpha = (float)(iluminaty * 0.5000000) + 0.5000000;
			pModel->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 1, RenderAlpha, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 2, RenderAlpha, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			return;
		case 'l':
			pModel->RenderMesh(0, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 0, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			return;
		}
	}
}

void CGMNixiesLake::RenderFrontSideVisual()
{
	int v19 = TerrainWall[TERRAIN_INDEX_REPEAT(Hero->PositionX, Hero->PositionY)];
	if ((v19 & 0x800) != 0)
	{
		EnableAlphaBlend();
		glColor3f(0.40000001, 0.40000001, 0.44999999);

		float WindX = ((int)WorldTime % 100000) * 0.0001;
		RenderBitmapUV(BITMAP_CHROME + 2, 0.0, 0.0, pos_right(640), pos_botton(480.0), WindX, 0.0, 0.3, 0.3);
	}
	else if ((v19 & 0x1000) != 0)
	{
		EnableAlphaBlend();
		glColor3f(0.40000001, 0.40000001, 0.44999999);

		float WindX = ((int)WorldTime % 100000) * 0.0001;
		RenderBitmapUV(BITMAP_CHROME + 2, 0.0, 0.0, pos_right(640), pos_botton(480.0), WindX, 0.0, 0.3, 0.3);
	}
}

bool CGMNixiesLake::CreateNixiesSnow(PARTICLE* o)
{
	return false;
}
