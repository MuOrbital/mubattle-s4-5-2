#pragma once

#define MAX_MINI_MAP 100

class CMiniMap
{
	CMiniMap();

	virtual ~CMiniMap();

	SingletonInstance(CMiniMap);

	public:
	void Init();
	void MiniMapCore();
	void MiniMapLoad();
	bool MiniMapCheck(int map);
	bool MiniMapFileCheck(int map);
};

#define gMiniMap SingNull(CMiniMap)
