#pragma once

#include "Lua.h"

class itemObject 
{
public:
	itemObject() : itemObj(nullptr) 
	{

	};

	itemObject(DWORD objClass) 
	{
		this->itemObj = (ITEM*)objClass;
	};

	~itemObject() {

	}

	int getLevel() const 
	{
		if (this->itemObj == nullptr) 
		{
			return 0;
		}

		return this->itemObj->Level * 8;
	}

	int getOption1() const 
	{
		if (this->itemObj == nullptr) 
		{
			return 0;
		}

		return this->itemObj->Option1;
	}

	int getExc() const 
	{
		if (this->itemObj == nullptr) 
		{
			return 0;
		}

		return this->itemObj->ExtOption;
	}

private:
	ITEM* itemObj;
};

void RegisterLuaItemObject(lua_State* lua);