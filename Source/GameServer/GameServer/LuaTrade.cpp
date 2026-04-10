#include "StdAfx.h"
#include "LuaTrade.h"
#include "LuaStack.hpp"
#include "User.h"

class TradeClass {
public:
	TradeClass() {

	};

	TradeClass(int aIndex) {
		this->aIndex = aIndex;
	};

	~TradeClass() {

	};

	int getIndex(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Trade[slot].m_Index;
		}
		else {
			return -1;
		}
	};

	int isItem(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Trade[slot].IsItem();
		}
		else {
			return 0;
		}
	};

	int isExc(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Trade[slot].IsExtItem();
		}
		else {
			return 0;
		}
	};

	int getLevel(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Trade[slot].m_Level;
		}
		else {
			return 0;
		}
	};

	std::string getName(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Trade[slot].GetName();
		}
		else {
			return "Not Find";
		}
	};

	int getSection(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return (gObj[this->aIndex].Trade[slot].m_Index / 512);
		}
		else {
			return 0;
		}
	};

	int getIndex2(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return (gObj[this->aIndex].Trade[slot].m_Index % 512);
		}
		else {
			return 0;
		}
	};

	int getHasLife(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Trade[slot].IsExtLifeAdd();
		}
		else {
			return 0;
		}
	};

	int getHasMana(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Trade[slot].IsExtManaAdd();
		}
		else {
			return 0;
		}
	};

	DWORD getSerial(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Trade[slot].m_Serial;
		}
		else {
			return 0;
		}
	};

	int getItemTable(int slot, int type) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			if (type == 1) {
				return gObj[this->aIndex].Trade[slot].m_NewOption;
			}
			else if (type == 2) {
				return gObj[this->aIndex].Trade[slot].m_Option1;
			}
			else if (type == 3) {
				return gObj[this->aIndex].Trade[slot].m_Option2;
			}
			else if (type == 4) {
				return gObj[this->aIndex].Trade[slot].m_Option3;
			}
			else if (type == 5) {
				return gObj[this->aIndex].Trade[slot].m_SetOption;
			}
			else if (type == 6) {
				return gObj[this->aIndex].Trade[slot].m_JewelOfHarmonyOption;
			}
			else if (type == 7) {
				return gObj[this->aIndex].Trade[slot].m_ItemOptionEx;
			}
#if (GAMESERVER_UPDATE>=401)
			else if (type == 8) {
				return gObj[this->aIndex].Trade[slot].m_SocketOptionBonus;
			}
			else if (type == 9) {
				return gObj[this->aIndex].Trade[slot].m_SocketOption[0];
			}
			else if (type == 10) {
				return gObj[this->aIndex].Trade[slot].m_SocketOption[1];
			}
			else if (type == 11) {
				return gObj[this->aIndex].Trade[slot].m_SocketOption[2];
			}
			else if (type == 12) {
				return gObj[this->aIndex].Trade[slot].m_SocketOption[3];
			}
			else if (type == 13) {
				return gObj[this->aIndex].Trade[slot].m_SocketOption[4];
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
				gObj[this->aIndex].Trade[slot].m_NewOption = value;
			}
			else if (type == 2) {
				gObj[this->aIndex].Trade[slot].m_Option1 = value;
			}
			else if (type == 3) {
				gObj[this->aIndex].Trade[slot].m_Option2 = value;
			}
			else if (type == 4) {
				gObj[this->aIndex].Trade[slot].m_Option3 = value;
			}
			else if (type == 5) {
				gObj[this->aIndex].Trade[slot].m_SetOption = value;
			}
			else if (type == 6) {
				gObj[this->aIndex].Trade[slot].m_JewelOfHarmonyOption = value;
			}
			else if (type == 7) {
				gObj[this->aIndex].Trade[slot].m_ItemOptionEx = value;
			}
#if (GAMESERVER_UPDATE>=401)
			else if (type == 8) {
				gObj[this->aIndex].Trade[slot].m_SocketOptionBonus = value;
			}
			else if (type == 9) {
				gObj[this->aIndex].Trade[slot].m_SocketOption[0] = value;
			}
			else if (type == 10) {
				gObj[this->aIndex].Trade[slot].m_SocketOption[1] = value;
			}
			else if (type == 11) {
				gObj[this->aIndex].Trade[slot].m_SocketOption[2] = value;
			}
			else if (type == 12) {
				gObj[this->aIndex].Trade[slot].m_SocketOption[3] = value;
			}
			else if (type == 13) {
				gObj[this->aIndex].Trade[slot].m_SocketOption[4] = value;
			}
#endif
		}
	};

	int getDurability(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return (int)gObj[this->aIndex].Trade[slot].m_Durability;
		}
		else {
			return 0;
		}
	};

	void setDurability(int slot, int value) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			gObj[this->aIndex].Trade[slot].m_Durability = (float)value;
		}
	};

	void convertItem(int slot) const {
		auto lpItem = &gObj[this->aIndex].Trade[slot];

		if (lpItem) {
			lpItem->Convert(lpItem->m_Index, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, lpItem->m_NewOption, lpItem->m_SetOption, lpItem->m_JewelOfHarmonyOption, lpItem->m_ItemOptionEx, lpItem->m_SocketOption, lpItem->m_SocketOptionBonus);
		}
	};

	int getIsPeriodic(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Trade[slot].m_IsPeriodicItem;
		}

		return 0;
	};

	DWORD getPeriodicTime(int slot) const {
		if (OBJECT_USER_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Trade[slot].m_PeriodicItemTime;
		}

		return 0;
	};

private:
	int aIndex;
};

void RegisterTradeClass(lua_State* lua)
{
	luaaa::LuaClass<TradeClass> luaTrade(lua, "Trade");
	luaTrade.ctor<int>();
	luaTrade.fun("getIndex", &TradeClass::getIndex);
	luaTrade.fun("isItem", &TradeClass::isItem);
	luaTrade.fun("isExc", &TradeClass::isExc);
	luaTrade.fun("getLevel", &TradeClass::getLevel);
	luaTrade.fun("getName", &TradeClass::getName);
	luaTrade.fun("getSection", &TradeClass::getSection);
	luaTrade.fun("getIndex2", &TradeClass::getIndex2);
	luaTrade.fun("getHasLife", &TradeClass::getHasLife);
	luaTrade.fun("getHasMana", &TradeClass::getHasMana);
	luaTrade.fun("getSerial", &TradeClass::getSerial);
	luaTrade.fun("getItemTable", &TradeClass::getItemTable);
	luaTrade.fun("setItemTable", &TradeClass::setItemTable);
	luaTrade.fun("getDurability", &TradeClass::getDurability);
	luaTrade.fun("setDurability", &TradeClass::setDurability);
	luaTrade.fun("convertItem", &TradeClass::convertItem);
	luaTrade.fun("getIsPeriodic", &TradeClass::getIsPeriodic);
	luaTrade.fun("getPeriodicTime", &TradeClass::getPeriodicTime);
};