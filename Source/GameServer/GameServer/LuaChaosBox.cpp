#include "StdAfx.h"
#include "LuaChaosBox.h"
#include "LuaStack.hpp"
#include "User.h"
#include "ItemStack.h"

class ChaosBoxClass {
public:
	ChaosBoxClass() {

	};

	ChaosBoxClass(int aIndex) {
		this->aIndex = aIndex;
	};

	~ChaosBoxClass() {

	};

	int getIndex(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return gObj[this->aIndex].ChaosBox[slot].m_Index;
		}
		else {
			return -1;
		}
	};

	int isItem(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return gObj[this->aIndex].ChaosBox[slot].IsItem();
		}
		else {
			return 0;
		}
	};

	int isExc(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return gObj[this->aIndex].ChaosBox[slot].IsExtItem();
		}
		else {
			return 0;
		}
	};

	int getLevel(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return gObj[this->aIndex].ChaosBox[slot].m_Level;
		}
		else {
			return 0;
		}
	};

	std::string getName(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return gObj[this->aIndex].ChaosBox[slot].GetName();
		}
		else {
			return "Not Find";
		}
	};

	int getSection(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return (gObj[this->aIndex].ChaosBox[slot].m_Index / 512);
		}
		else {
			return 0;
		}
	};

	int getIndex2(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return (gObj[this->aIndex].ChaosBox[slot].m_Index % 512);
		}
		else {
			return 0;
		}
	};

	int getHasLife(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return gObj[this->aIndex].ChaosBox[slot].IsExtLifeAdd();
		}
		else {
			return 0;
		}
	};

	int getHasMana(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return gObj[this->aIndex].ChaosBox[slot].IsExtManaAdd();
		}
		else {
			return 0;
		}
	};

	DWORD getSerial(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return gObj[this->aIndex].ChaosBox[slot].m_Serial;
		}
		else {
			return 0;
		}
	};

	int getItemTable(int slot, int type) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			if (type == 1) {
				return gObj[this->aIndex].ChaosBox[slot].m_NewOption;
			}
			else if (type == 2) {
				return gObj[this->aIndex].ChaosBox[slot].m_Option1;
			}
			else if (type == 3) {
				return gObj[this->aIndex].ChaosBox[slot].m_Option2;
			}
			else if (type == 4) {
				return gObj[this->aIndex].ChaosBox[slot].m_Option3;
			}
			else if (type == 5) {
				return gObj[this->aIndex].ChaosBox[slot].m_SetOption;
			}
			else if (type == 6) {
				return gObj[this->aIndex].ChaosBox[slot].m_JewelOfHarmonyOption;
			}
			else if (type == 7) {
				return gObj[this->aIndex].ChaosBox[slot].m_ItemOptionEx;
			}
#if (GAMESERVER_UPDATE>=401)
			else if (type == 8) {
				return gObj[this->aIndex].ChaosBox[slot].m_SocketOptionBonus;
			}
			else if (type == 9) {
				return gObj[this->aIndex].ChaosBox[slot].m_SocketOption[0];
			}
			else if (type == 10) {
				return gObj[this->aIndex].ChaosBox[slot].m_SocketOption[1];
			}
			else if (type == 11) {
				return gObj[this->aIndex].ChaosBox[slot].m_SocketOption[2];
			}
			else if (type == 12) {
				return gObj[this->aIndex].ChaosBox[slot].m_SocketOption[3];
			}
			else if (type == 13) {
				return gObj[this->aIndex].ChaosBox[slot].m_SocketOption[4];
			}
#endif
			else {
				return 0;
			}
		}
		else {
			return 0;
		}
	};

	void setItemTable(int slot, int type, int value) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			if (type == 1) {
				gObj[this->aIndex].ChaosBox[slot].m_NewOption = value;
			}
			else if (type == 2) {
				gObj[this->aIndex].ChaosBox[slot].m_Option1 = value;
			}
			else if (type == 3) {
				gObj[this->aIndex].ChaosBox[slot].m_Option2 = value;
			}
			else if (type == 4) {
				gObj[this->aIndex].ChaosBox[slot].m_Option3 = value;
			}
			else if (type == 5) {
				gObj[this->aIndex].ChaosBox[slot].m_SetOption = value;
			}
			else if (type == 6) {
				gObj[this->aIndex].ChaosBox[slot].m_JewelOfHarmonyOption = value;
			}
			else if (type == 7) {
				gObj[this->aIndex].ChaosBox[slot].m_ItemOptionEx = value;
			}
#if (GAMESERVER_UPDATE>=401)
			else if (type == 8) {
				gObj[this->aIndex].ChaosBox[slot].m_SocketOptionBonus = value;
			}
			else if (type == 9) {
				gObj[this->aIndex].ChaosBox[slot].m_SocketOption[0] = value;
			}
			else if (type == 10) {
				gObj[this->aIndex].ChaosBox[slot].m_SocketOption[1] = value;
			}
			else if (type == 11) {
				gObj[this->aIndex].ChaosBox[slot].m_SocketOption[2] = value;
			}
			else if (type == 12) {
				gObj[this->aIndex].ChaosBox[slot].m_SocketOption[3] = value;
			}
			else if (type == 13) {
				gObj[this->aIndex].ChaosBox[slot].m_SocketOption[4] = value;
			}
#endif
		}
	};

	int getDurability(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return (int)gObj[this->aIndex].ChaosBox[slot].m_Durability;
		}
		else {
			return 0;
		}
	};

	void setDurability(int slot, int value) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			gObj[this->aIndex].ChaosBox[slot].m_Durability = (float)value;
		}
	};

	void convertItem(int slot) const {
		auto lpItem = &gObj[this->aIndex].ChaosBox[slot];

		if (lpItem) {
			lpItem->Convert(lpItem->m_Index, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, lpItem->m_NewOption, lpItem->m_SetOption, lpItem->m_JewelOfHarmonyOption, lpItem->m_ItemOptionEx, lpItem->m_SocketOption, lpItem->m_SocketOptionBonus);
		}
	};

	int getIsPeriodic(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return gObj[this->aIndex].ChaosBox[slot].m_IsPeriodicItem;
		}

		return 0;
	};

	DWORD getPeriodicTime(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return gObj[this->aIndex].ChaosBox[slot].m_PeriodicItemTime;
		}

		return 0;
	};

private:
	int aIndex;
};

void RegisterChaosBoxClass(lua_State* lua)
{
	luaaa::LuaClass<ChaosBoxClass> luaChaosBox(lua, "ChaosBox");
	luaChaosBox.ctor<int>();
	luaChaosBox.fun("getIndex", &ChaosBoxClass::getIndex);
	luaChaosBox.fun("isItem", &ChaosBoxClass::isItem);
	luaChaosBox.fun("isExc", &ChaosBoxClass::isExc);
	luaChaosBox.fun("getLevel", &ChaosBoxClass::getLevel);
	luaChaosBox.fun("getName", &ChaosBoxClass::getName);
	luaChaosBox.fun("getSection", &ChaosBoxClass::getSection);
	luaChaosBox.fun("getIndex2", &ChaosBoxClass::getIndex2);
	luaChaosBox.fun("getHasLife", &ChaosBoxClass::getHasLife);
	luaChaosBox.fun("getHasMana", &ChaosBoxClass::getHasMana);
	luaChaosBox.fun("getSerial", &ChaosBoxClass::getSerial);
	luaChaosBox.fun("getItemTable", &ChaosBoxClass::getItemTable);
	luaChaosBox.fun("setItemTable", &ChaosBoxClass::setItemTable);
	luaChaosBox.fun("getDurability", &ChaosBoxClass::getDurability);
	luaChaosBox.fun("setDurability", &ChaosBoxClass::setDurability);
	luaChaosBox.fun("convertItem", &ChaosBoxClass::convertItem);
	luaChaosBox.fun("getIsPeriodic", &ChaosBoxClass::getIsPeriodic);
	luaChaosBox.fun("getPeriodicTime", &ChaosBoxClass::getPeriodicTime);
};