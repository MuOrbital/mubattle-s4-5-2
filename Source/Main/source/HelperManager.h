#pragma once

#include <map>
#include <vector>
#include "HelperView.h"

class CHelperManager
{
public:
	CHelperManager();
	virtual ~CHelperManager();

	void Relese();
	void Init();
	static void RenderPetMain();
	static void UpdatePetMain();
	bool CreateHelper(int Index, int ModelIndex, vec3_t Position, CHARACTER* m_Owner, int SubType = 0);
	void DeleteHelper(CHARACTER* Owner, int itemType = -1, bool allDelete = false);
	void Register(BoostSmart_Ptr(HELPER_VIEW) info);
	void UnRegister(CHARACTER* Owner, int itemType, bool isUnregistAll = false);
	bool CheckExistsHelper(CHARACTER* Owner);

	void RenderHelpers();
	void UpdateHelpers();

private:
	std::vector<BoostSmart_Ptr(HELPER_VIEW)> m_HelperList;
};

extern CHelperManager gHelperManager;