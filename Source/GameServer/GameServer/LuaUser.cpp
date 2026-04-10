#include "StdAfx.h"
#include "LuaUser.h"
#include "LuaStack.hpp"
#include "Util.h"
#include "User.h"
#include "Guild.h"
#include "PlayerCache.h"

class UserClass {
public:
	UserClass() : aIndex(-1) {

	};

	UserClass(int aIndex) {
		if (OBJECT_RANGE(aIndex)) {
			this->aIndex = aIndex;
		}
		else {
			this->aIndex = 0;
		}
	};

	~UserClass() {

	};

	int luaGetIndex() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Index;
		}
		else {
			return 0;
		}
	}

	std::string luaGetAccountID() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Account;
		}
		else {
			return "not found";
		}
	};

	std::string luaGetName() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Name;
		}
		else {
			return "not found";
		}
	};

	void luaSetName(const std::string& name) const {
		if (OBJECT_RANGE(this->aIndex)) {
			std::memcpy(&gObj[aIndex].Name, name.c_str(), sizeof(gObj[aIndex].Name));
		}
	};

	int luaGetConnected() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Connected;
		}
		else {
			return 0;
		}
	};

	void luaSetConnected(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Connected = value;
		}
	};

	int luaGetLive() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Live;
		}
		else {
			return 0;
		}
	};

	void luaSetLive(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Live = value;
		}
	};

	int luaGetType() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Type;
		}
		else {
			return 0;
		}
	};

	void luaSetType(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Type = value;
		}
	};

	int luaGetClass() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Class;
		}
		else {
			return 0;
		}
	};

	void luaSetClass(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Class = value;
		}
	};

	int luaGetDbClass() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].DBClass;
		}
		else {
			return 0;
		}
	};

	void luaSetDbClass(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].DBClass = value;
		}
	};

	int luaGetChangeUP() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].ChangeUp;
		}
		else {
			return 0;
		}
	};

	void luaSetChangeUP(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].ChangeUp = value;
		}
	};

	int luaGetLevel() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Level;
		}
		else {
			return 0;
		}
	};

	void luaSetUserTable(int tipo, int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			switch (tipo) {
			case 1:
				gObj[this->aIndex].DefensePvP = value;
				break;
			case 2:
				gObj[this->aIndex].Defense = value;
				break;
			case 3:
				gObj[this->aIndex].DefenseSuccessRate = value;
				break;
			case 4:
				gObj[this->aIndex].DefenseSuccessRatePvP = value;
				break;
			case 5:
				gObj[this->aIndex].AddLife = value;
				break;
			case 6:
				gObj[this->aIndex].AddMana = value;
				break;
			case 7:
				gObj[this->aIndex].PhysiDamageMinLeft = value;
				break;
			case 8:
				gObj[this->aIndex].PhysiDamageMaxLeft = value;
				break;
			case 9:
				gObj[this->aIndex].PhysiDamageMinRight = value;
				break;
			case 10:
				gObj[this->aIndex].PhysiDamageMaxRight = value;
				break;
			case 11:
				gObj[this->aIndex].MagicDamageMin = value;
				break;
			case 12:
				gObj[this->aIndex].MagicDamageMax = value;
				break;
			case 13:
				gObj[this->aIndex].PhysiSpeed = value;
				break;
			case 14:
				gObj[this->aIndex].MagicSpeed = value;
				break;
			case 15:
				gObj[this->aIndex].EffectOption.AddExcellentDamageRate = value;
				break;
			case 16:
				gObj[this->aIndex].ExcellentDamageRate = value;
				break;
			case 17:
				gObj[this->aIndex].ResistExcellentDamageRate = value;
				break;
			case 18:
				gObj[this->aIndex].EffectOption.AddExcellentDamage = value;
				break;
			case 19:
				gObj[this->aIndex].FullDamageReflectRate = value;
				break;
			case 20:
				gObj[this->aIndex].DamageReflect = value;
				break;
			case 21:
				gObj[this->aIndex].DamagePvP = value;
				break;
			case 22:
				gObj[this->aIndex].EffectOption.AddDamageReduction = value;
				break;
			case 23:
				gObj[this->aIndex].ShieldDamageReduction = value;
				break;
			case 24:
				gObj[this->aIndex].TripleDamageRate = value;
				break;
			case 25:
				gObj[this->aIndex].DoubleDamageRate = value;
				break;
			case 26:
				gObj[this->aIndex].ResistDoubleDamageRate = value;
				break;
			case 27:
				gObj[this->aIndex].EffectOption.AddCriticalDamageRate = value;
				break;
			case 28:
				gObj[this->aIndex].CriticalDamageRate = value;
				break;
			case 29:
				gObj[this->aIndex].ResistCriticalDamageRate = value;
				break;
			case 30:
				gObj[this->aIndex].EffectOption.AddCriticalDamage = value;
				break;
			case 31:
				gObj[this->aIndex].AttackSuccessRatePvP = value;
				break;
			case 32:
				gObj[this->aIndex].IgnoreShieldGaugeRate = value;
				break;
			case 33:
				gObj[this->aIndex].ResistIgnoreShieldGaugeRate = value;
				break;
			}

		}
	};

	int luaGetUserTable(int tipo) {
		if (!OBJECT_RANGE(this->aIndex)) {
			return 0;
		}

		switch (tipo) {
		case 1:
			return gObj[this->aIndex].DefensePvP;
		case 2:
			return gObj[this->aIndex].Defense;
		case 3:
			return gObj[this->aIndex].DefenseSuccessRate;
		case 4:
			return gObj[this->aIndex].DefenseSuccessRatePvP;
		case 5:
			return gObj[this->aIndex].AddLife;
		case 6:
			return gObj[this->aIndex].AddMana;
		case 7:
			return gObj[this->aIndex].PhysiDamageMinLeft;
		case 8:
			return gObj[this->aIndex].PhysiDamageMaxLeft;
		case 9:
			return gObj[this->aIndex].PhysiDamageMinRight;
		case 10:
			return gObj[this->aIndex].PhysiDamageMaxRight;
		case 11:
			return gObj[this->aIndex].MagicDamageMin;
		case 12:
			return gObj[this->aIndex].MagicDamageMax;
		case 13:
			return gObj[this->aIndex].PhysiSpeed;
		case 14:
			return gObj[this->aIndex].MagicSpeed;
		case 15:
			return gObj[this->aIndex].EffectOption.AddExcellentDamageRate;
		case 16:
			return gObj[this->aIndex].ExcellentDamageRate;
		case 17:
			return gObj[this->aIndex].ResistExcellentDamageRate;
		case 18:
			return gObj[this->aIndex].EffectOption.AddExcellentDamage;
		case 19:
			return gObj[this->aIndex].FullDamageReflectRate;
		case 20:
			return gObj[this->aIndex].DamageReflect;
		case 21:
			return gObj[this->aIndex].DamagePvP;
		case 22:
			return gObj[this->aIndex].EffectOption.AddDamageReduction;
		case 23:
			return gObj[this->aIndex].ShieldDamageReduction;
		case 24:
			return gObj[this->aIndex].TripleDamageRate;
		case 25:
			return gObj[this->aIndex].DoubleDamageRate;
		case 26:
			return gObj[this->aIndex].ResistDoubleDamageRate;
		case 27:
			return gObj[this->aIndex].EffectOption.AddCriticalDamageRate;
		case 28:
			return gObj[this->aIndex].CriticalDamageRate;
		case 29:
			return gObj[this->aIndex].ResistCriticalDamageRate;
		case 30:
			return gObj[this->aIndex].EffectOption.AddCriticalDamage;
		case 31:
			return gObj[this->aIndex].AttackSuccessRatePvP;
		case 32:
			return gObj[this->aIndex].IgnoreShieldGaugeRate;
		case 33:
			return gObj[this->aIndex].ResistIgnoreShieldGaugeRate;

		default:
			return 0;
		}
	};

	void setCacheInt(std::string key, int value) {
		if (OBJECT_RANGE(this->aIndex)) {
			gPlayerCache[this->aIndex].m_cacheInt[key] = value;
		}
	};

	int getCacheInt(std::string key) {
		if (OBJECT_RANGE(this->aIndex)) {
			return gPlayerCache[this->aIndex].m_cacheInt[key];
		}

		return -1;
	};

	void luaSetLevel(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Level = value;
		}
	};

	int luaGetLevelUpPoint() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].LevelUpPoint;
		}
		else {
			return 0;
		}
	};

	void luaSetLevelUpPoint(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].LevelUpPoint = value;
		}
	};

	int luaGetMoney() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Money;
		}
		else {
			return 0;
		}
	};

	void luaSetMoney(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Money = value;
		}
	};

	int luaGetStrength() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Strength;
		}
		else {
			return 0;
		}
	};

	void luaSetStrength(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Strength = value;
		}
	};

	int luaGetDexterity() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Dexterity;
		}
		else {
			return 0;
		}
	};

	void luaSetDexterity(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Dexterity = value;
		}
	};

	int luaGetVitality() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Vitality;
		}
		else {
			return 0;
		}
	};

	void luaSetVitality(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Vitality = value;
		}
	};

	int luaGetEnergy() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Energy;
		}
		else {
			return 0;
		}
	};

	void luaSetEnergy(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Energy = value;
		}
	};

	int luaGetLeaderShip() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Leadership;
		}
		else {
			return 0;
		}
	};

	void luaSetLeaderShip(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Leadership = value;
		}
	};

	int luaGetPKCount() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].PKCount;
		}
		else {
			return 0;
		}
	};

	void luaSetPKCount(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].PKCount = value;
		}
	};

	int luaGetPKLevel() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].PKLevel;
		}
		else {
			return 0;
		}
	};

	void luaSetPKLevel(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].PKLevel = value;
		}
	};

	int luaGetPKTime() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].PKTime;
		}
		else {
			return 0;
		}
	};

	void luaSetPKTime(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].PKTime = value;
		}
	};

	int luaGetX() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].X;
		}
		else {
			return 0;
		}
	};

	void luaSetX(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].X = value;
		}
	};

	int luaGetY() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Y;
		}
		else {
			return 0;
		}
	};

	void luaSetY(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Y = value;
		}
	};

	int luaGetTX() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].TX;
		}
		else {
			return 0;
		}
	};

	void luaSetTX(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].TX = value;
		}
	};

	int luaGetTY() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].TY;
		}
		else {
			return 0;
		}
	};

	void luaSetTY(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].TY = value;
		}
	};

	int luaGetMTX() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].MTX;
		}
		else {
			return 0;
		}
	};

	void luaSetMTX(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].MTX = value;
		}
	};

	int luaGetMTY() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].MTY;
		}
		else {
			return 0;
		}
	};

	void luaSetMTY(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].MTY = value;
		}
	};

	int luaGetMapNumber() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Map;
		}
		else {
			return 0;
		}
	};

	void luaSetMapNumber(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Map = value;
		}
	};

	int luaGetAuthority() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Authority;
		}
		else {
			return 0;
		}
	};

	void luaSetAuthority(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Authority = value;
		}
	};

	int luaGetAuthorityCode() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].AuthorityCode;
		}
		else {
			return 0;
		}
	};

	void luaSetAuthorityCode(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].AuthorityCode = value;
		}
	};

	int luaGetSkin() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Change;
		}
		else {
			return 0;
		}
	};

	void luaSetSkin(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Change = value;
		}
	};

	int luaGetExp() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Experience;
		}
		else {
			return 0;
		}
	};

	void luaSetExp(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Experience = value;
		}
	};

	int luaGetTargetState(int slot) const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].VpPlayer[slot].state;
		}
		else {
			return 0;
		}
	};

	void luaSetTargetState(int slot, int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].VpPlayer[slot].state = value;
		}
	};

	int luaGetTargetNumber(int slot) const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].VpPlayer[slot].index;
		}
		else {
			return 0;
		}
	};

	void luaSetTargetNumber(int slot, int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].VpPlayer[slot].index = value;
		}
	};

	int luaGetTargetType(int slot) const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].VpPlayer[slot].type;
		}
		else {
			return 0;
		}
	};

	void luaSetTargetType(int slot, int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].VpPlayer[slot].type = value;
		}
	};

	int luaGetInterfaceUse() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Interface.use;
		}
		else {
			return 0;
		}
	};

	void luaSetInterfaceUse(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Interface.use = value;
		}
	};

	int luaGetInterfaceState() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Interface.state;
		}
		else {
			return 0;
		}
	};

	void luaSetInterfaceState(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Interface.state = value;
		}
	};

	int luaGetInterfaceType() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Interface.type;
		}
		else {
			return 0;
		}
	};

	void luaSetInterfaceType(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Interface.type = value;
		}
	};

	int luaGetDefense() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Defense;
		}
		else {
			return 0;
		}
	};

	void luaSetDefense(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Defense = value;
		}
	};

	int luaGetDefensePvP() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].DefensePvP;
		}
		else {
			return 0;
		}
	};

	void luaSetDefensePvP(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].DefensePvP = value;
		}
	};

	int luaGetSuccessBlock() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].DefenseSuccessRate;
		}
		else {
			return 0;
		}
	};

	void luaSetSuccessBlock(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].DefenseSuccessRate = value;
		}
	};

	int luaGetSuccessBlockPvP() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].DefenseSuccessRatePvP;
		}
		else {
			return 0;
		}
	};

	void luaSetSuccessBlockPvP(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].DefenseSuccessRatePvP = value;
		}
	};

	int luaGetState() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].State;
		}
		else {
			return 0;
		}
	};

	void luaSetState(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].State = value;
		}
	};

	int luaGetDieRegen() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].DieRegen;
		}
		else {
			return 0;
		}
	};

	void luaSetDieRegen(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].DieRegen = value;
		}
	};

	int luaGetTeleport() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Teleport;
		}
		else {
			return 0;
		}
	};

	void luaSetTeleport(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Teleport = value;
		}
	};

	int luaGetTargetShopNumber() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].TargetShopNumber;
		}
		else {
			return 0;
		}
	};

	void luaSetTargetShopNumber(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].TargetShopNumber = value;
		}
	};

	int luaGetReqWarehouseOpen() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].ReqWarehouseOpen;
		}
		else {
			return 0;
		}
	};

	void luaSetReqWarehouseOpen(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].ReqWarehouseOpen = value;
		}
	};

	int luaGetWarehouseCount() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].WarehouseCount;
		}
		else {
			return 0;
		}
	};

	void luaSetWarehouseCount(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].WarehouseCount = value;
		}
	};

	int luaGetDir() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Dir;
		}
		else {
			return 0;
		}
	};

	void luaSetDir(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Dir = value;
		}
	};

	int luaGetNextExp() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].NextExperience;
		}
		else {
			return 0;
		}
	};

	void luaSetNextExp(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].NextExperience = value;
		}
	};

	int luaGetLife() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return (int)gObj[this->aIndex].Life;
		}
		else {
			return 0;
		}
	};

	void luaSetLife(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Life = (float)value;
		}
	};

	int luaGetMana() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return (int)gObj[this->aIndex].Mana;
		}
		else {
			return 0;
		}
	};

	void luaSetMana(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Mana = (float)value;
		}
	};

	int luaGetMaxLife() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return (int)gObj[this->aIndex].MaxLife;
		}
		else {
			return 0;
		}
	};

	void luaSetMaxLife(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].MaxLife = (float)value;
		}
	};

	int luaGetMaxMana() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return (int)gObj[this->aIndex].MaxMana;
		}
		else {
			return 0;
		}
	};

	void luaSetMaxMana(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].MaxMana = (float)value;
		}
	};

	int luaGetBP() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return (int)gObj[this->aIndex].BP;
		}
		else {
			return 0;
		}
	};

	void luaSetBP(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].BP = value;
		}
	};

	int luaGetMaxBP() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return (int)gObj[this->aIndex].MaxBP;
		}
		else {
			return 0;
		}
	};

	void luaSetMaxBP(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].MaxBP = value;
		}
	};

	int luaGetShield() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return (int)gObj[this->aIndex].Shield;
		}
		else {
			return 0;
		}
	};

	void luaSetShield(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Shield = value;
		}
	};

	int luaGetMaxShield() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return (int)gObj[this->aIndex].MaxShield;
		}
		else {
			return 0;
		}
	};

	void luaSetMaxShield(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].MaxShield = value;
		}
	};

	int luaGetVitalityToLife() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return (int)gObj[this->aIndex].VitalityToLife;
		}
		else {
			return 0;
		}
	};

	void luaSetVitalityToLife(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].VitalityToLife = (float)value;
		}
	};

	int luaGetEnergyToMana() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return (int)gObj[this->aIndex].EnergyToMana;
		}
		else {
			return 0;
		}
	};

	void luaSetEnergyToMana(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].EnergyToMana = (float)value;
		}
	};

	int luaGetOption() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Option;
		}
		else {
			return 0;
		}
	};

	void luaSetOption(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Option = value;
		}
	};

	int luaGetAddLife() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].AddLife;
		}
		else {
			return 0;
		}
	};

	void luaSetAddLife(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].AddLife = value;
		}
	};

	int luaGetAddMana() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].AddMana;
		}
		else {
			return 0;
		}
	};

	void luaSetAddMana(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].AddMana = value;
		}
	};

	int luaGetAddBP() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].AddBP;
		}
		else {
			return 0;
		}
	};

	void luaSetAddBP(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].AddBP = value;
		}
	};

	int luaGetAddShield() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].AddShield;
		}
		else {
			return 0;
		}
	};

	void luaSetAddShield(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].AddShield = value;
		}
	};

	int luaGetAttackSpeed() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].PhysiSpeed;
		}
		else {
			return 0;
		}
	};

	void luaSetAttackSpeed(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].PhysiSpeed = value;
		}
	};

	int luaGetMagicSpeed() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].MagicSpeed;
		}
		else {
			return 0;
		}
	};

	void luaSetMagicSpeed(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].MagicSpeed = value;
		}
	};

	int luaGetMagicDamageMin() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].MagicDamageMin;
		}
		else {
			return 0;
		}
	};

	void luaSetMagicDamageMin(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].MagicDamageMin = value;
		}
	};

	int luaGetMagicDamageMax() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].MagicDamageMax;
		}
		else {
			return 0;
		}
	};

	void luaSetMagicDamageMax(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].MagicDamageMax = value;
		}
	};

	int luaGetAttackDamageMinLeft() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].PhysiDamageMinLeft;
		}
		else {
			return 0;
		}
	};

	void luaSetAttackDamageMinLeft(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].PhysiDamageMinLeft = value;
		}
	};

	int luaGetAttackDamageMaxLeft() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].PhysiDamageMaxLeft;
		}
		else {
			return 0;
		}
	};

	void luaSetAttackDamageMaxLeft(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].PhysiDamageMaxLeft = value;
		}
	};

	int luaGetAttackDamageMinRight() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].PhysiDamageMinRight;
		}
		else {
			return 0;
		}
	};

	void luaSetAttackDamageMinRight(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].PhysiDamageMinRight = value;
		}
	};

	int luaGetAttackDamageMaxRight() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].PhysiDamageMaxRight;
		}
		else {
			return 0;
		}
	};

	void luaSetAttackDamageMaxRight(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].PhysiDamageMaxRight = value;
		}
	};

	int luaGetRegenTime() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].RegenTime;
		}
		else {
			return 0;
		}
	};

	void luaSetRegenTime(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].RegenTime = value;
		}
	};

	int luaGetGetRegen() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].RegenOk;
		}
		else {
			return 0;
		}
	};

	void luaSetGetRegen(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].RegenOk = value;
		}
	};

	int luaGetKillerType() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].KillerType;
		}
		else {
			return 0;
		}
	};

	void luaSetKillerType(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].KillerType = value;
		}
	};

	int luaGetConnectCheckTime() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].ConnectTickCount;
		}
		else {
			return 0;
		}
	};

	void luaSetConnectCheckTime(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].ConnectTickCount = value;
		}
	};

	int luaGetCheckSumTime() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].CheckSumTime;
		}
		else {
			return 0;
		}
	};

	void luaSetCheckSumTime(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].CheckSumTime = value;
		}
	};

	int luaGetPathCount() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].PathCount;
		}
		else {
			return 0;
		}
	};

	void luaSetPathCount(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].PathCount = value;
		}
	};

	int luaGetRegenMapNumber() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].RegenMapNumber;
		}
		else {
			return 0;
		}
	};

	void luaSetRegenMapNumber(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].RegenMapNumber = value;
		}
	};

	int luaGetRegenMapX() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].RegenMapX;
		}
		else {
			return 0;
		}
	};

	void luaSetRegenMapX(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].RegenMapX = value;
		}
	};

	int luaGetRegenMapY() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].RegenMapY;
		}
		else {
			return 0;
		}
	};

	void luaSetRegenMapY(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].RegenMapY = value;
		}
	};

	int luaGetPathCur() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].PathCur;
		}
		else {
			return 0;
		}
	};

	void luaSetPathCur(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].PathCur = value;
		}
	};

	int luaGetPathX(int path) const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].PathX[path];
		}
		else {
			return 0;
		}
	};

	void luaSetPathX(int path, int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].PathX[path] = value;
		}
	};

	int luaGetPathY(int path) const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].PathY[path];
		}
		else {
			return 0;
		}
	};

	void luaSetPathY(int path, int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].PathY[path] = value;
		}
	};

	int luaGetPathDir(int path) const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].PathDir[path];
		}
		else {
			return 0;
		}
	};

	void luaSetPathDir(int path, int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].PathDir[path] = value;
		}
	};

	int luaGetTradeTarget() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].TargetNumber;
		}
		else {
			return 0;
		}
	};

	void luaSetTradeTarget(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].TargetNumber = value;
		}
	};

	int luaGetVip() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].AccountLevel;
		}
		else {
			return 0;
		}
	};

	void luaSetVip(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].AccountLevel = value;
		}
	};

	int luaGetReset() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Reset;
		}
		else {
			return 0;
		}
	};

	void luaSetReset(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].Reset = value;
		}
	};

	int luaGetMasterReset() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].MasterReset;
		}
		else {
			return 0;
		}
	};

	void luaSetMasterReset(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].MasterReset = value;
		}
	};

	int luaGetLoginMessageSend() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].LoginMessageSend;
		}
		else {
			return 0;
		}
	};

	void luaSetLoginMessageSend(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].LoginMessageSend = value;
		}
	};

	int luaGetLoginMessageCount() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].LoginMessageCount;
		}
		else {
			return 0;
		}
	};

	void luaSetLoginMessageCount(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].LoginMessageCount = value;
		}
	};

	int luaGetSendQuestInfo() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].SendQuestInfo;
		}
		else {
			return 0;
		}
	};

	void luaSetSendQuestInfo(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].SendQuestInfo = value;
		}
	};

	int luaGetExpRate() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].ExperienceRate;
		}
		else {
			return 0;
		}
	};

	void luaSetExpRate(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].ExperienceRate = value;
		}
	};

	int luaGetShopOpen() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].PShopOpen;
		}
		else {
			return 0;
		}
	};

	void luaSetShopOpen(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].PShopOpen = value;
		}
	};

#if(GAMESERVER_UPDATE>=401)
	int getMasterLevel() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].MasterLevel;
		}
		else {
			return 0;
		}
	};

	void setMasterLevel(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].MasterLevel = value;
		}
	};

	int getMasterPoint() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].MasterPoint;
		}
		else {
			return 0;
		}
	};

	void setMasterPoint(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].MasterPoint = value;
		}
	};

	long long getMasterExperience() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].MasterExperience;
		}
		else {
			return 0;
		}
	};

	void setMasterExperience(long long value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].MasterExperience = value;
		}
	};
#endif

	std::string getLanguage() {
		if (OBJECT_RANGE(this->aIndex)) 
		{
			return "Eng";
		}

		return "Por";
	}

#ifdef CHAOS_MACHINE_GENESIS

	int getChaosBoxGenesisState() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].ChaosBoxGenesisState;
		}
		else {
			return 0;
		}
	};

	void setChaosBoxGenesisState(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].ChaosBoxGenesisState = value;
		}
	};

	int getChaosBoxGenesisSelectedKey() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].ChaosBoxGenesisSelectedKey;
		}
		else {
			return 0;
		}
	};

	void setChaosBoxGenesisSelectedKey(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].ChaosBoxGenesisSelectedKey = value;
		}
	};

	int getChaosBoxGenesisSelectedComb() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].ChaosBoxGenesisSelectedComb;
		}
		else {
			return 0;
		}
	};

	void setChaosBoxGenesisSelectedComb(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].ChaosBoxGenesisSelectedComb = value;
		}
	};
	#endif

#if(GAMESERVER_UPDATE==603)
	int getExtInventory() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].ExtInventory;
		}
		else {
			return 0;
		}
	};

	void setExtInventory(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].ExtInventory = value;
		}
	};

	int getExtWarehouse() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].ExtWarehouse;
		}
		else {
			return 0;
		}
	};

	void setExtWarehouse(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].ExtWarehouse = value;
		}
	};
#endif

	int getChatLimitTime() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].ChatLimitTime;
		}
		else {
			return 0;
		}
	};

	void setChatLimitTime(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].ChatLimitTime = value;
		}
	};

	int getGuildNumber() const {
		if (!OBJECT_RANGE(this->aIndex)) {
			return -1;
		}

		return gObj[this->aIndex].GuildNumber;
	};

	std::string getGuildName() const {
		if (!OBJECT_RANGE(this->aIndex)) {
			return "Not find";
		}

		return gObj[this->aIndex].GuildName;
	};

	int getGuildWarDeclareState() const {
		if (OBJECT_USER_RANGE(this->aIndex) == 0) {
			return 0;
		}

		if (gObj[this->aIndex].Guild == nullptr) {
			return 0;
		}

		return gObj[this->aIndex].Guild->WarDeclareState;
	};

	int getGuildWarState() const {
		if (OBJECT_USER_RANGE(this->aIndex) == 0) {
			return 0;
		}

		if (gObj[this->aIndex].Guild == nullptr) {
			return 0;
		}

		return gObj[this->aIndex].Guild->WarState;
	};

	int getGuildWarType() const {
		if (OBJECT_USER_RANGE(this->aIndex) == 0) {
			return 0;
		}

		if (gObj[this->aIndex].Guild == nullptr) {
			return 0;
		}

		return gObj[this->aIndex].Guild->WarType;
	};

	int getGuildTotalScore() const {
		if (OBJECT_USER_RANGE(this->aIndex) == 0) {
			return 0;
		}

		if (gObj[this->aIndex].Guild == nullptr) {
			return 0;
		}

		return gObj[this->aIndex].Guild->TotalScore;
	};

	int getPartyNumber() const {
		if (OBJECT_USER_RANGE(this->aIndex) == 0) {
			return 0;
		}

		return gObj[this->aIndex].PartyNumber;
	};

	void setPartyNumber(int value) const {
		if (OBJECT_USER_RANGE(this->aIndex) == 0) {
			return;
		}

		gObj[this->aIndex].PartyNumber = value;
	};

	std::string getPShopText() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].PShopText;
		}
		else {
			return "not found";
		}
	};

	void setPShopText(std::string string) const {
		if (OBJECT_USER_RANGE(this->aIndex) == 0) {
			return;
		}

		std::memcpy(gObj[this->aIndex].PShopText, string.c_str(), sizeof(gObj[this->aIndex].PShopText));
	};

	int getSummonIndex() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].SummonIndex;
		}
		else {
			return 0;
		}
	};

	void setSummonIndex(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].SummonIndex = value;
		}
	};

	int getAutoResetEnable() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].AutoResetEnable;
		}
		else {
			return 0;
		}
	};

	void setAutoResetEnable(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].AutoResetEnable = value;
		}
	};

	int getAutoResetStats(int key) const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].AutoResetStats[key];
		}
		else {
			return 0;
		}
	};

	void setAutoResetStats(int key, int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].AutoResetStats[key] = value;
		}
	};

	int getAutoAddPointCount() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].AutoAddPointCount;
		}
		else {
			return 0;
		}
	};

	void setAutoAddPointCount(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].AutoAddPointCount = value;
		}
	};

	int getAutoAddPointStats(int key) const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].AutoAddPointStats[key];
		}
		else {
			return 0;
		}
	};

	void setAutoAddPointStats(int key, int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].AutoAddPointStats[key] = value;
		}
	};




	//void setCacheInt(std::string key, int value) {
	//	if (OBJECT_RANGE(this->aIndex)) {
	//		gPlayerCache[this->aIndex].m_cacheInt[key] = value;
	//	}
	//};

	void clearCacheInt(std::string key) {
		if (OBJECT_RANGE(this->aIndex)) {
			gPlayerCache[this->aIndex].m_cacheInt.erase(key);
		}
	};

	std::string getCacheStr(std::string key) {
		if (OBJECT_RANGE(this->aIndex)) {
			return gPlayerCache[this->aIndex].m_cacheStr[key];
		}

		return "nil";
	};

	void setCacheStr(std::string key, std::string value) {
		if (OBJECT_RANGE(this->aIndex)) {
			gPlayerCache[this->aIndex].m_cacheStr[key] = value;
		}
	};

	void clearCacheStr(std::string key) {
		if (OBJECT_RANGE(this->aIndex)) {
			gPlayerCache[this->aIndex].m_cacheStr.erase(key);
		}
	};

	int getCashShopTransaction(int key) const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].CashShopTransaction[key];
		}
		else {
			return 0;
		}
	};

	void setCashShopTransaction(int key, int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].CashShopTransaction[key] = value;
		}
	};

	void luaSetMonitoring(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			switch (value) {
			case 1:
				gObj[this->aIndex].Monitoring = 1;
				break;
			default:
				gObj[this->aIndex].Monitoring = 0;
				break;
			}
		}
	};

	int luaGetMonitoring() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Monitoring;
		}
		else {
			return 0;
		}
	};

	void luaSetMonitored(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			switch (value) {
			case 1:
				gObj[this->aIndex].Monitored = 1;
				break;
			default:
				gObj[this->aIndex].Monitored = 0;
				break;
			}
		}
	};

	int luaGetMonitored() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].Monitored;
		}
		else {
			return 0;
		}
	};

#if (GAMESERVER_UPDATE>=600)
	int luaGetPatent() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].m_Patent;
		}
		else {
			return 0;
		}
	};

	void luaSetPatent(int value) const 
	{
		if (OBJECT_RANGE(this->aIndex)) 
		{
			gObj[this->aIndex].m_Patent = value;
		}
	};

	int luaGetPatentType() const {
		if (OBJECT_RANGE(this->aIndex)) 
		{
			return gObj[this->aIndex].m_PatentType;
		}
		else {
			return 0;
		}
	};

	void luaSetPatentType(int value) const 
	{
		if (OBJECT_RANGE(this->aIndex)) 
		{
			gObj[this->aIndex].m_PatentType = value;
		}
	};

	int getInMuHelperOffline() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].m_OfflineMode;
		}
		else {
			return 0;
		}
	};

	void setInMuHelperOffline(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].m_OfflineMode = value;
		}
	};

	int getInOffAttack() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].AttackCustom;
		}
		else {
			return 0;
		}
	};

	void setInOffAttack(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].AttackCustom = value;
		}
	};

	int getInOffStore() const {
		if (OBJECT_RANGE(this->aIndex)) {
			return gObj[this->aIndex].PShopCustomOffline;
		}
		else {
			return 0;
		}
	};

	void setInOffStore(int value) const {
		if (OBJECT_RANGE(this->aIndex)) {
			gObj[this->aIndex].PShopCustomOffline = value;
		}
	};
#endif

private:
	int aIndex;
};

void RegisterUserClass(lua_State* lua) {
	luaaa::LuaClass<UserClass> luaUser(lua, "User");
	luaUser.ctor<int>();
	luaUser.fun("getIndex", &UserClass::luaGetIndex);
	luaUser.fun("getAccountID", &UserClass::luaGetAccountID);
	luaUser.fun("getName", &UserClass::luaGetName);
	luaUser.fun("setName", &UserClass::luaSetName);
	luaUser.fun("getConnected", &UserClass::luaGetConnected);
	luaUser.fun("setConnected", &UserClass::luaSetConnected);
	luaUser.fun("getLive", &UserClass::luaGetLive);
	luaUser.fun("setLive", &UserClass::luaSetLive);
	luaUser.fun("getType", &UserClass::luaGetType);
	luaUser.fun("setType", &UserClass::luaSetType);
	luaUser.fun("getClass", &UserClass::luaGetClass);
	luaUser.fun("setClass", &UserClass::luaSetClass);
	luaUser.fun("getDbClass", &UserClass::luaGetDbClass);
	luaUser.fun("setDbClass", &UserClass::luaSetDbClass);
	luaUser.fun("getChangeUP", &UserClass::luaGetChangeUP);
	luaUser.fun("setChangeUP", &UserClass::luaSetChangeUP);
	luaUser.fun("getLevel", &UserClass::luaGetLevel);
	luaUser.fun("setLevel", &UserClass::luaSetLevel);
	luaUser.fun("getLevelUpPoint", &UserClass::luaGetLevelUpPoint);
	luaUser.fun("setLevelUpPoint", &UserClass::luaSetLevelUpPoint);
	luaUser.fun("getMoney", &UserClass::luaGetMoney);
	luaUser.fun("setMoney", &UserClass::luaSetMoney);
	luaUser.fun("getStrength", &UserClass::luaGetStrength);
	luaUser.fun("setStrength", &UserClass::luaSetStrength);
	luaUser.fun("getDexterity", &UserClass::luaGetDexterity);
	luaUser.fun("setDexterity", &UserClass::luaSetDexterity);
	luaUser.fun("getVitality", &UserClass::luaGetVitality);
	luaUser.fun("setVitality", &UserClass::luaSetVitality);
	luaUser.fun("getEnergy", &UserClass::luaGetEnergy);
	luaUser.fun("setEnergy", &UserClass::luaSetEnergy);
	luaUser.fun("getLeaderShip", &UserClass::luaGetLeaderShip);
	luaUser.fun("setLeaderShip", &UserClass::luaSetLeaderShip);
	luaUser.fun("getPKCount", &UserClass::luaGetPKCount);
	luaUser.fun("setPKCount", &UserClass::luaSetPKCount);
	luaUser.fun("getPKLevel", &UserClass::luaGetPKLevel);
	luaUser.fun("setPKLevel", &UserClass::luaSetPKLevel);
	luaUser.fun("getPKTime", &UserClass::luaGetPKTime);
	luaUser.fun("setPKTime", &UserClass::luaSetPKTime);
	luaUser.fun("getX", &UserClass::luaGetX);
	luaUser.fun("setX", &UserClass::luaSetX);
	luaUser.fun("getY", &UserClass::luaGetY);
	luaUser.fun("setY", &UserClass::luaSetY);
	luaUser.fun("getTX", &UserClass::luaGetTX);
	luaUser.fun("setTX", &UserClass::luaSetTX);
	luaUser.fun("getTY", &UserClass::luaGetTY);
	luaUser.fun("setTY", &UserClass::luaSetTY);
	luaUser.fun("getMTX", &UserClass::luaGetMTX);
	luaUser.fun("setMTX", &UserClass::luaSetMTX);
	luaUser.fun("getMTY", &UserClass::luaGetMTY);
	luaUser.fun("setMTY", &UserClass::luaSetMTY);
	luaUser.fun("getMapNumber", &UserClass::luaGetMapNumber);
	luaUser.fun("setMapNumber", &UserClass::luaSetMapNumber);
	luaUser.fun("getAuthority", &UserClass::luaGetAuthority);
	luaUser.fun("setAuthority", &UserClass::luaSetAuthority);
	luaUser.fun("getAuthorityCode", &UserClass::luaGetAuthorityCode);
	luaUser.fun("setAuthorityCode", &UserClass::luaSetAuthorityCode);
	luaUser.fun("getSkin", &UserClass::luaGetSkin);
	luaUser.fun("setSkin", &UserClass::luaSetSkin);
	luaUser.fun("getExp", &UserClass::luaGetExp);
	luaUser.fun("setExp", &UserClass::luaSetExp);
	luaUser.fun("getTargetState", &UserClass::luaGetTargetState);
	luaUser.fun("setTargetState", &UserClass::luaSetTargetState);
	luaUser.fun("getTargetNumber", &UserClass::luaGetTargetNumber);
	luaUser.fun("setTargetNumber", &UserClass::luaSetTargetNumber);
	luaUser.fun("getTargetType", &UserClass::luaGetTargetType);
	luaUser.fun("setTargetType", &UserClass::luaSetTargetType);
	luaUser.fun("getInterfaceUse", &UserClass::luaGetInterfaceUse);
	luaUser.fun("setInterfaceUse", &UserClass::luaSetInterfaceUse);
	luaUser.fun("getInterfaceState", &UserClass::luaGetInterfaceState);
	luaUser.fun("setInterfaceState", &UserClass::luaSetInterfaceState);
	luaUser.fun("getInterfaceType", &UserClass::luaGetInterfaceType);
	luaUser.fun("setInterfaceType", &UserClass::luaSetInterfaceType);
	luaUser.fun("getDefense", &UserClass::luaGetDefense);
	luaUser.fun("setDefense", &UserClass::luaSetDefense);
	luaUser.fun("getDefensePvP", &UserClass::luaGetDefensePvP);
	luaUser.fun("setDefensePvP", &UserClass::luaSetDefensePvP);
	luaUser.fun("getSuccessBlock", &UserClass::luaGetSuccessBlock);
	luaUser.fun("setSuccessBlock", &UserClass::luaSetSuccessBlock);
	luaUser.fun("getSuccessBlockPvP", &UserClass::luaGetSuccessBlockPvP);
	luaUser.fun("setSuccessBlockPvP", &UserClass::luaSetSuccessBlockPvP);
	luaUser.fun("getState", &UserClass::luaGetState);
	luaUser.fun("setState", &UserClass::luaSetState);
	luaUser.fun("getDieRegen", &UserClass::luaGetDieRegen);
	luaUser.fun("setDieRegen", &UserClass::luaSetDieRegen);
	luaUser.fun("getTeleport", &UserClass::luaGetTeleport);
	luaUser.fun("setTeleport", &UserClass::luaSetTeleport);
	luaUser.fun("getTargetShopNumber", &UserClass::luaGetTargetShopNumber);
	luaUser.fun("setTargetShopNumber", &UserClass::luaSetTargetShopNumber);
	luaUser.fun("getReqWarehouseOpen", &UserClass::luaGetReqWarehouseOpen);
	luaUser.fun("setReqWarehouseOpen", &UserClass::luaSetReqWarehouseOpen);
	luaUser.fun("getWarehouseCount", &UserClass::luaGetWarehouseCount);
	luaUser.fun("setWarehouseCount", &UserClass::luaSetWarehouseCount);
	luaUser.fun("getDir", &UserClass::luaGetDir);
	luaUser.fun("setDir", &UserClass::luaSetDir);
	luaUser.fun("getNextExp", &UserClass::luaGetNextExp);
	luaUser.fun("setNextExp", &UserClass::luaSetNextExp);
	luaUser.fun("getLife", &UserClass::luaGetLife);
	luaUser.fun("setLife", &UserClass::luaSetLife);
	luaUser.fun("getMana", &UserClass::luaGetMana);
	luaUser.fun("setMana", &UserClass::luaSetMana);
	luaUser.fun("getMaxLife", &UserClass::luaGetMaxLife);
	luaUser.fun("setMaxLife", &UserClass::luaSetMaxLife);
	luaUser.fun("getMaxMana", &UserClass::luaGetMaxMana);
	luaUser.fun("setMaxMana", &UserClass::luaSetMaxMana);
	luaUser.fun("getBP", &UserClass::luaGetBP);
	luaUser.fun("setBP", &UserClass::luaSetBP);
	luaUser.fun("getMaxBP", &UserClass::luaGetMaxBP);
	luaUser.fun("setMaxBP", &UserClass::luaSetMaxBP);
	luaUser.fun("getShield", &UserClass::luaGetShield);
	luaUser.fun("setShield", &UserClass::luaSetShield);
	luaUser.fun("getMaxShield", &UserClass::luaGetMaxShield);
	luaUser.fun("setMaxShield", &UserClass::luaSetMaxShield);
	luaUser.fun("getVitalityToLife", &UserClass::luaGetVitalityToLife);
	luaUser.fun("setVitalityToLife", &UserClass::luaSetVitalityToLife);
	luaUser.fun("getEnergyToMana", &UserClass::luaGetEnergyToMana);
	luaUser.fun("setEnergyToMana", &UserClass::luaSetEnergyToMana);
	luaUser.fun("getOption", &UserClass::luaGetOption);
	luaUser.fun("setOption", &UserClass::luaSetOption);
	luaUser.fun("getAddLife", &UserClass::luaGetAddLife);
	luaUser.fun("setAddLife", &UserClass::luaSetAddLife);
	luaUser.fun("getAddMana", &UserClass::luaGetAddMana);
	luaUser.fun("setAddMana", &UserClass::luaSetAddMana);
	luaUser.fun("getAddBP", &UserClass::luaGetAddBP);
	luaUser.fun("setAddBP", &UserClass::luaSetAddBP);
	luaUser.fun("getAddShield", &UserClass::luaGetAddShield);
	luaUser.fun("setAddShield", &UserClass::luaSetAddShield);
	luaUser.fun("getAttackSpeed", &UserClass::luaGetAttackSpeed);
	luaUser.fun("setAttackSpeed", &UserClass::luaSetAttackSpeed);
	luaUser.fun("getMagicSpeed", &UserClass::luaGetMagicSpeed);
	luaUser.fun("setMagicSpeed", &UserClass::luaSetMagicSpeed);
	luaUser.fun("getMagicDamageMin", &UserClass::luaGetMagicDamageMin);
	luaUser.fun("setMagicDamageMin", &UserClass::luaSetMagicDamageMin);
	luaUser.fun("getMagicDamageMax", &UserClass::luaGetMagicDamageMax);
	luaUser.fun("setMagicDamageMax", &UserClass::luaSetMagicDamageMax);
	luaUser.fun("getAttackDamageMinLeft", &UserClass::luaGetAttackDamageMinLeft);
	luaUser.fun("setAttackDamageMinLeft", &UserClass::luaSetAttackDamageMinLeft);
	luaUser.fun("getAttackDamageMaxLeft", &UserClass::luaGetAttackDamageMaxLeft);
	luaUser.fun("setAttackDamageMaxLeft", &UserClass::luaSetAttackDamageMaxLeft);
	luaUser.fun("getAttackDamageMinRight", &UserClass::luaGetAttackDamageMinRight);
	luaUser.fun("setAttackDamageMinRight", &UserClass::luaSetAttackDamageMinRight);
	luaUser.fun("getAttackDamageMaxRight", &UserClass::luaGetAttackDamageMaxRight);
	luaUser.fun("setAttackDamageMaxRight", &UserClass::luaSetAttackDamageMaxRight);
	luaUser.fun("getRegenTime", &UserClass::luaGetRegenTime);
	luaUser.fun("setRegenTime", &UserClass::luaSetRegenTime);
	luaUser.fun("getGetRegen", &UserClass::luaGetGetRegen);
	luaUser.fun("setGetRegen", &UserClass::luaSetGetRegen);
	luaUser.fun("getKillerType", &UserClass::luaGetKillerType);
	luaUser.fun("setKillerType", &UserClass::luaSetKillerType);
	luaUser.fun("getConnectCheckTime", &UserClass::luaGetConnectCheckTime);
	luaUser.fun("setConnectCheckTime", &UserClass::luaSetConnectCheckTime);
	luaUser.fun("getCheckSumTime", &UserClass::luaGetCheckSumTime);
	luaUser.fun("setCheckSumTime", &UserClass::luaSetCheckSumTime);
	luaUser.fun("getPathCount", &UserClass::luaGetPathCount);
	luaUser.fun("setPathCount", &UserClass::luaSetPathCount);
	luaUser.fun("getRegenMapNumber", &UserClass::luaGetRegenMapNumber);
	luaUser.fun("setRegenMapNumber", &UserClass::luaSetRegenMapNumber);
	luaUser.fun("getRegenMapX", &UserClass::luaGetRegenMapX);
	luaUser.fun("setRegenMapX", &UserClass::luaSetRegenMapX);
	luaUser.fun("getRegenMapY", &UserClass::luaGetRegenMapY);
	luaUser.fun("setRegenMapY", &UserClass::luaSetRegenMapY);
	luaUser.fun("getPathCur", &UserClass::luaGetPathCur);
	luaUser.fun("setPathCur", &UserClass::luaSetPathCur);
	luaUser.fun("getPathX", &UserClass::luaGetPathX);
	luaUser.fun("setPathX", &UserClass::luaSetPathX);
	luaUser.fun("getPathY", &UserClass::luaGetPathY);
	luaUser.fun("setPathY", &UserClass::luaSetPathY);
	luaUser.fun("getPathDir", &UserClass::luaGetPathDir);
	luaUser.fun("setPathDir", &UserClass::luaSetPathDir);
	luaUser.fun("getTradeTarget", &UserClass::luaGetTradeTarget);
	luaUser.fun("setTradeTarget", &UserClass::luaSetTradeTarget);
	luaUser.fun("getVip", &UserClass::luaGetVip);
	luaUser.fun("setVip", &UserClass::luaSetVip);
	luaUser.fun("getReset", &UserClass::luaGetReset);
	luaUser.fun("setReset", &UserClass::luaSetReset);
	luaUser.fun("getMasterReset", &UserClass::luaGetMasterReset);
	luaUser.fun("setMasterReset", &UserClass::luaSetMasterReset);
	luaUser.fun("getLoginMessageSend", &UserClass::luaGetLoginMessageSend);
	luaUser.fun("setLoginMessageSend", &UserClass::luaSetLoginMessageSend);
	luaUser.fun("getLoginMessageCount", &UserClass::luaGetLoginMessageCount);
	luaUser.fun("setLoginMessageCount", &UserClass::luaSetLoginMessageCount);
	luaUser.fun("getSendQuestInfo", &UserClass::luaGetSendQuestInfo);
	luaUser.fun("setSendQuestInfo", &UserClass::luaSetSendQuestInfo);

	luaUser.fun("getExpRate", &UserClass::luaGetExpRate);
	luaUser.fun("setExpRate", &UserClass::luaSetExpRate);

	#if(GAMESERVER_UPDATE>=401)
	luaUser.fun("getMasterLevel", &UserClass::getMasterLevel);
	luaUser.fun("setMasterLevel", &UserClass::setMasterLevel);
	luaUser.fun("getMasterPoint", &UserClass::getMasterPoint);
	luaUser.fun("setMasterPoint", &UserClass::setMasterPoint);
	luaUser.fun("getMasterExperience", &UserClass::getMasterExperience);
	luaUser.fun("setMasterExperience", &UserClass::setMasterExperience);
	#endif

	luaUser.fun("getLanguage", &UserClass::getLanguage);
#ifdef CHAOS_MACHINE_GENESIS
	luaUser.fun("getChaosBoxGenesisState", &UserClass::getChaosBoxGenesisState);
	luaUser.fun("setChaosBoxGenesisState", &UserClass::setChaosBoxGenesisState);
	luaUser.fun("getChaosBoxGenesisSelectedKey", &UserClass::getChaosBoxGenesisSelectedKey);
	luaUser.fun("setChaosBoxGenesisSelectedKey", &UserClass::setChaosBoxGenesisSelectedKey);
	luaUser.fun("getChaosBoxGenesisSelectedComb", &UserClass::getChaosBoxGenesisSelectedComb);
	luaUser.fun("setChaosBoxGenesisSelectedComb", &UserClass::setChaosBoxGenesisSelectedComb);
	#endif

	#if(GAMESERVER_UPDATE==603)
	luaUser.fun("getExtInventory", &UserClass::getExtInventory);
	luaUser.fun("setExtInventory", &UserClass::setExtInventory);
	luaUser.fun("getExtWarehouse", &UserClass::getExtWarehouse);
	luaUser.fun("setExtWarehouse", &UserClass::setExtWarehouse);
	#endif

	luaUser.fun("getChatLimitTime", &UserClass::getChatLimitTime);
	luaUser.fun("setChatLimitTime", &UserClass::setChatLimitTime);

	//Guild
	luaUser.fun("getGuildNumber", &UserClass::getGuildNumber);
	luaUser.fun("getGuildName", &UserClass::getGuildName);
	luaUser.fun("getGuildWarDeclareState", &UserClass::getGuildWarDeclareState);
	luaUser.fun("getGuildWarState", &UserClass::getGuildWarState);
	luaUser.fun("getGuildWarType", &UserClass::getGuildWarType);
	luaUser.fun("getGuildTotalScore", &UserClass::getGuildTotalScore);

	//Party
	luaUser.fun("getPartyNumber", &UserClass::getPartyNumber);
	luaUser.fun("setPartyNumber", &UserClass::setPartyNumber);

	//PersonalShop
	luaUser.fun("getShopOpen", &UserClass::luaGetShopOpen);
	luaUser.fun("setShopOpen", &UserClass::luaSetShopOpen);
	luaUser.fun("getPShopText", &UserClass::getPShopText);
	luaUser.fun("setPShopText", &UserClass::setPShopText);


	luaUser.fun("getSummonIndex", &UserClass::getSummonIndex);
	luaUser.fun("setSummonIndex", &UserClass::setSummonIndex);

	//Auto Reset and auto add point
	luaUser.fun("getAutoResetEnable", &UserClass::getAutoResetEnable);
	luaUser.fun("setAutoResetEnable", &UserClass::setAutoResetEnable);

	luaUser.fun("getAutoResetStats", &UserClass::getAutoResetStats);
	luaUser.fun("setAutoResetStats", &UserClass::setAutoResetStats);

	luaUser.fun("getAutoAddPointCount", &UserClass::getAutoAddPointCount);
	luaUser.fun("setAutoAddPointCount", &UserClass::setAutoAddPointCount);

	luaUser.fun("getAutoAddPointStats", &UserClass::getAutoAddPointStats);
	luaUser.fun("setAutoAddPointStats", &UserClass::setAutoAddPointStats);

	luaUser.fun("getCashShopTransaction", &UserClass::getCashShopTransaction);
	luaUser.fun("setCashShopTransaction", &UserClass::setCashShopTransaction);

	//cache player
	luaUser.fun("getCacheInt", &UserClass::getCacheInt);
	luaUser.fun("setCacheInt", &UserClass::setCacheInt);
	luaUser.fun("clearCacheInt", &UserClass::clearCacheInt);

	luaUser.fun("getCacheStr", &UserClass::getCacheStr);
	luaUser.fun("setCacheStr", &UserClass::setCacheStr);
	luaUser.fun("clearCacheStr", &UserClass::clearCacheStr);

	// Pvp Control
	luaUser.fun("getUserTable", &UserClass::luaGetUserTable);
	luaUser.fun("setUserTable", &UserClass::luaSetUserTable);

#if (GAMESERVER_UPDATE>=600)
	luaUser.fun("getPatent", &UserClass::luaGetPatent);
	luaUser.fun("setPatent", &UserClass::luaSetPatent);
	luaUser.fun("getPatentType", &UserClass::luaGetPatentType);
	luaUser.fun("setPatentType", &UserClass::luaSetPatentType);
	luaUser.fun("getInMuHelperOffline", &UserClass::getInMuHelperOffline);
	luaUser.fun("setInMuHelperOffline", &UserClass::setInMuHelperOffline);

	luaUser.fun("getInOffAttack", &UserClass::getInOffAttack);
	luaUser.fun("setInOffAttack", &UserClass::setInOffAttack);

	luaUser.fun("getInOffStore", &UserClass::getInOffStore);
	luaUser.fun("setInOffStore", &UserClass::setInOffStore);
#endif
}