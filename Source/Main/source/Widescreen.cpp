#include "stdafx.h"
#include "Widescreen.h"

CWideScreen GWidescreen;

void CWideScreen::Init()
{
	DisplayWinCDepthBox = (float)WindowWidth / g_fScreenRate_y - 640;

	FrameWinCDepthBox = ((float)WindowWidth / g_fScreenRate_y - 640);

	JCWinWidth = (float)WindowWidth / g_fScreenRate_y;

	JCWinWidthAdd = JCWinWidth / 2 - 320;

	// Janelas
	WidescreenPosX1 = JCWinWidth - (640 - 450);		// Apenas (1) UMA janela aberta
	WidescreenPosX2 = JCWinWidth - (640 - 260);		// Apenas (2) DUAS janelas abertas

	fScreen_Width = GetWindowsX;

	g_WideWindowWidth = (float)WindowWidth / g_fScreenRate_y;
	g_WideWindowWidthAdd = this->g_WideWindowWidth - 640;

	//
	for (int i = 0; i < MAX_MOVEWINDOW; i++)
	{
		this->DepthLayer[i] = 4.2f;	
	}
}