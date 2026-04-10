#include "stdafx.h"
#include "ItemBank.h"
#include "ItemManager.h"
#include "NewUISystem.h"

CItemBank::CItemBank()
{

}

CItemBank::~CItemBank()
{

}

void CItemBank::Render(int x, int y)
{
	RenderItem(x + 2, y + 35, 50, 50, gItemManager->GetItem(14, 13));

	RenderItem(x + 2, y + 50, 50, 50, gItemManager->GetItem(14, 14));

	g_pRenderText->SetFont(g_hFont);

	g_pRenderText->SetBgColor(0, 0, 0, 0);

	g_pRenderText->SetTextColor(255, 255, 255, 255);

	g_pRenderText->RenderText(x + 35, y + 70, "x150", 190 * 0.3f, 0, RT3_SORT_LEFT);

	g_pRenderText->RenderText(x + 35, y + 85, "x0", 190 * 0.3f, 0, RT3_SORT_LEFT);
}

void CItemBank::RenderItem(float sx, float sy, float w, float h, int Type, int Level, int Option1, int ExtOption)
{
	EndBitmap();

	glMatrixMode(GL_PROJECTION);

	glPushMatrix();

	glLoadIdentity();

	glViewport2(0, 0, WindowWidth, WindowHeight);

	gluPerspective2(1.f, (float)(WindowWidth) / (float)(WindowHeight), RENDER_ITEMVIEW_NEAR, RENDER_ITEMVIEW_FAR);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glLoadIdentity();

	GetOpenGLMatrix(CameraMatrix);

	EnableDepthTest();

	EnableDepthMask();

	RenderItem3D(sx, sy, w, h, Type, Level * 8, Option1, ExtOption, false);

	UpdateMousePositionn();

	glMatrixMode(GL_MODELVIEW);

	glPopMatrix();

	glMatrixMode(GL_PROJECTION);

	glPopMatrix();

	BeginBitmap();
}