#pragma once

class CItemBank
{
	CItemBank();

	virtual ~CItemBank();

	SingletonInstance(CItemBank);

public:
	void Render(int x, int y);
	void RenderItem(float sx, float sy, float w, float h, int Type, int Level = 0, int Option1 = 0, int ExtOption = 0);
};

#define gItemBank SingNull(CItemBank)
