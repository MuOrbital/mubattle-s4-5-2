#pragma once

class cMoveWindow
{
public:
	cMoveWindow();
	~cMoveWindow();
	void Create(int x, int y, int width, int height);
	bool UpdatePosition(int x, int y, int WindowID = -1);
	//
	POINT m_PosTemp;
	POINT m_PosOffset;
	//
	bool IsMove;
	//
	int Width;
	int Height;
};