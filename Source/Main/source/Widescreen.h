#pragma once

extern float g_fScreenRate_x;
extern float g_fScreenRate_y;

#define GetWindowsX								(WindowWidth  / g_fScreenRate_x)
#define GetWindowsY								(WindowHeight / g_fScreenRate_y)
#define GetCenterX(x)							((GetWindowsX - x) / 2.f)
#define setPosMidDown(x)						((GetWindowsY - 480.f) / 2.f + x )
#define setPosMidRight(x)						((GetWindowsX - 640.f) / 2.f + x )
#define setPosCenterX(x)						((GetWindowsX - x) / 2.f)
#define GetCenterPosition(x)					(x + (GetWindowsX / 2.f))
#define SubWindowsX(x)							(GetWindowsX - x )
#define setPosRight(x)							(GetWindowsX - 640.f + x )
#define setPosDown(x)							(GetWindowsY - 480.f + x )

#define MAX_MOVEWINDOW 3

enum InterfaceType
{
	CharacterInfo,
	QuestInfo,
	Inventario,
};

class CWideScreen
{
public:
	void Init();

public:
	int JCWinWidth;
	int JCWinWidthAdd;
	int DisplayWinCDepthBox; // eax@4
	int FrameWinCDepthBox;
	
public:
	int WidescreenPosX1;
	int WidescreenPosX2;
	double fScreen_Width;

public:
	float DepthLayer[2];

public:
	float g_WideWindowWidth;
	float g_WideWindowWidthAdd;
};

extern CWideScreen GWidescreen;