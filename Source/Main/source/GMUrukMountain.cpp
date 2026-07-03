#include "stdafx.h"
#include "CustomWorldEffectCompat.h"
#include "ZzzLodTerrain.h"
#include "GMUrukMountain.h"

CGMUrukMountainPtr CGMUrukMountain::Make()
{
	CGMUrukMountainPtr pkfield(new CGMUrukMountain);
	return pkfield;
}

CGMUrukMountain::CGMUrukMountain()
{
}

CGMUrukMountain::~CGMUrukMountain()
{
}

bool CGMUrukMountain::LoadMapData()
{
	return false;
}

bool CGMUrukMountain::MoveObject(OBJECT* pObject)
{
	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return false;

	int Index = (pObject->Type);
	float fLumi;
	vec3_t Light;
	switch (Index)
	{
	case 16:
		pObject->Timer -= timefac(0.015);
		break;
	case 27:
		fLumi = (float)(rand() % 4 + 3) * 0.1;
		Light[0] = fLumi;
		Light[1] = fLumi * 0.6f;
		Light[2] = fLumi * 0.20f;
		AddTerrainLight(pObject->Position[0], pObject->Position[1], Light, 3, PrimaryTerrainLight);
		break;
	case 44:
		pObject->Timer -= timefac(0.0035);
		break;
	case 72:
		pObject->Timer += timefac(0.007);
		break;
	case 73:
		pObject->Timer -= timefac(0.02);
		break;
	default:
		if (Index > 83 && Index <= 86)
			pObject->Timer -= timefac(0.017);
		break;
	}
	return true;
}

bool CGMUrukMountain::CreateObject(OBJECT* pObject)
{
	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return false;

	int Index = (pObject->Type);

	pObject->m_iAnimation = 0;

	if (Index > 56)
	{
		switch (Index)
		{
		case 87:
		case 88:
		case 89:
		case 90:
		case 91:
		case 92:
		case 93:
		case 94:
		case 99:
		case 100:
		case 105:
		case 106:
		case 108:
		case 109:
			goto LABEL_14;
		case 96:
		case 97:
			goto LABEL_13;
		default:
			return true;
		}
		return true;
	}
	if (Index < 54 && Index != 20)
	{
		if (Index == 27)
		{
		LABEL_14:
			pObject->HiddenMesh = -2;
			return 1;
		}
		if (Index != 29 && (Index <= 32 || Index > 35 && Index != 39))
			return true;
	}
LABEL_13:
	pObject->CollisionRange = -400.0;
	return true;
}

bool CGMUrukMountain::RenderObjectVisual(OBJECT* pObject, BMD* pModel)
{
	return false;
}

bool CGMUrukMountain::RenderObjectMesh(OBJECT* pObject, BMD* pModel, bool ExtraMon)
{
	if (RANGE_OBJET_MAP(pObject->Type) == 0)
		return false;

	int Index = (pObject->Type);
	float fLumi;

	if (Index > 71)
	{
		if (Index == 72)
		{
			pObject->HiddenMesh = 4;
			pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, 0.0, 0.0, -1);
			pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, 0.0, 0.0, -1);
			pModel->StreamMesh = 2;
			pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(3, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, 0.0, 0.0, -1);
			pModel->StreamMesh = -1;
			pObject->HiddenMesh = -1;
		}
		else if (Index == 73)
		{
			pObject->HiddenMesh = 1;
			pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, 0.0, 0.0, -1);
			pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, 0.0, 0.0, -1);
			pModel->StreamMesh = 2;
			pModel->RenderMesh(2, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, ((-(int)WorldTime % 2000) * 0.0005), -1);
			pModel->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, ((-(int)WorldTime % 2000) * 0.0005), -1);
			pModel->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, ((-(int)WorldTime % 2000) * 0.0005), -1);
			pModel->RenderMesh(2, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, ((-(int)WorldTime % 2000) * 0.0005), -1);
			pModel->StreamMesh = -1;
			pObject->HiddenMesh = -1;
		}
		else
		{
			if (Index <= 83)
				return 0;
			if (Index > 86)
			{
				return 0;
			}
			pModel->RenderBody(RENDER_TEXTURE,pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1, -1);
		}
	}
	else
	{
		switch (Index)
		{
		case 71:
			fLumi = (sinf((WorldTime * 0.005)) + 1.0) * 0.2 + 0.6;
			if (fLumi > 1.0)
				fLumi = 1.0;
			pObject->HiddenMesh = 4;
			pModel->RenderBody(RENDER_TEXTURE,pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, pObject->HiddenMesh, -1);
			pObject->HiddenMesh = 0;
			pModel->RenderBody(RENDER_TEXTURE,pObject->Alpha, 4, fLumi, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, pObject->HiddenMesh, -1);
			break;
		case 16:
			pModel->StreamMesh = 0;
			pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->StreamMesh = 1;
			pObject->BlendMeshTexCoordV -= (0.005);
			pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pObject->BlendMeshTexCoordV += (0.005);
			pModel->StreamMesh = -1;
			break;
		case 44:
			pObject->HiddenMesh = 2;
			pModel->RenderMesh(0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, 0.0, 0.0, -1);
			pModel->StreamMesh = 1;
			pModel->RenderMesh(1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pModel->RenderMesh(1, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
			pObject->HiddenMesh = -1;
			pModel->StreamMesh = -1;
			break;
		case 49:
			fLumi = (sinf((WorldTime * 0.001)) + 1.0) * 0.6 + 0.2;
			if (fLumi > 1.0)
				fLumi = 1.0;
			pObject->HiddenMesh = 1;
			pModel->RenderBody(RENDER_TEXTURE,pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, pObject->HiddenMesh, -1);
			pObject->HiddenMesh = 0;
			pModel->RenderBody(RENDER_TEXTURE,pObject->Alpha, 0, fLumi, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, pObject->HiddenMesh, -1);
			break;
		case 57:
			fLumi = (sinf((WorldTime * 0.001)) + 1.0) * 0.2 + 0.2;
			if (fLumi > 1.0)
				fLumi = 1.0;
			pModel->RenderBody(RENDER_TEXTURE,pObject->Alpha, 0, fLumi, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, pObject->HiddenMesh, -1);
			break;
		case 58:
			fLumi = (sinf((WorldTime * 0.001)) + 1.0) * 0.2 + 0.2;
			if (fLumi > 1.0)
				fLumi = 1.0;
			pModel->RenderBody(RENDER_TEXTURE,pObject->Alpha, 0, fLumi, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, pObject->HiddenMesh, -1);
			break;
		case 70:
			fLumi = (sinf((WorldTime * 0.005)) + 1.0) * 0.2 + 0.6;
			if (fLumi > 1.0)
				fLumi = 1.0;
			pObject->HiddenMesh = 4;
			pModel->RenderBody(RENDER_TEXTURE,pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, pObject->HiddenMesh, -1);
			pObject->HiddenMesh = 0;
			pModel->RenderBody(RENDER_TEXTURE,pObject->Alpha, 4, fLumi, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, pObject->HiddenMesh, -1);
			break;
		default:
			return 0;
		}
	}

	return true;
}

void CGMUrukMountain::RenderAfterObjectMesh(OBJECT* pObject, BMD* pModel, bool ExtraMon)
{
}
