#include "stdafx.h"
#include "NewOptions.h"
#include "LuaStack.hpp"
using namespace luaaa;

CItemNewOption gItemNewOption;

CItemNewOption::CItemNewOption()
    : m_Lua()
{
    m_rules.clear();
}

CItemNewOption::~CItemNewOption()
{
    m_rules.clear();
}

int CItemNewOption::MakeType(int section, int index)
{
    return section * MAX_ITEM_INDEX + index;
}

void CItemNewOption::Init()
{
    LuaModule(this->m_Lua.GetState())
        .fun("LoadNewOption", &CItemNewOption::LuaLoadNewOption)
        .fun("NewOptionItem", &CItemNewOption::LuaNewOptionItem)
        .fun("GET_ITEM_MODEL", &CItemNewOption::LuaGetItemModel);

    m_Lua.DoFile("Data//Lua//ItemManager//NewOptions.lua");
    m_Lua.Generic_Call("StartLoadNewOptions", "");
}

void CItemNewOption::RestartLua()
{
    m_rules.clear();

    LuaModule(this->m_Lua.GetState())
        .fun("LoadNewOption", &CItemNewOption::LuaLoadNewOption)
        .fun("NewOptionItem", &CItemNewOption::LuaNewOptionItem)
        .fun("GET_ITEM_MODEL", &CItemNewOption::LuaGetItemModel);

    m_Lua.DoFile("Data//Lua//ItemManager//NewOptions.lua");
    m_Lua.Generic_Call("StartLoadNewOptions", "");
}

int CItemNewOption::LuaLoadNewOption(lua_State* L)
{
    const int  type = (int)lua_tointeger(L, 1);
    const int  minLevel = (int)lua_tointeger(L, 2);
    const char* line1 = lua_tostring(L, 3);
    const char* line2 = lua_tostring(L, 4);
    const int  color1 = (int)lua_tointeger(L, 5);
    const int  color2 = (int)lua_tointeger(L, 6);
    const bool bold1 = lua_toboolean(L, 7) ? true : false;
    const bool bold2 = lua_toboolean(L, 8) ? true : false;

    ITEM_OPTION_INFO info{};
    info.ItemType = type;
    info.MinLevel = minLevel;
    info.ExactLevel = -1; 
    info.Line1 = line1 ? line1 : "";
    info.Line2 = line2 ? line2 : "";
    info.Color1 = color1;
    info.Color2 = color2;
    info.Bold1 = bold1;
    info.Bold2 = bold2;

    auto& rules = gItemNewOption.m_rules[type];
    rules.thresholds.push_back(info);
    std::sort(rules.thresholds.begin(), rules.thresholds.end(),
        [](const ITEM_OPTION_INFO& a, const ITEM_OPTION_INFO& b) {
            return a.MinLevel < b.MinLevel;
        });

    return 0;
}

int CItemNewOption::LuaNewOptionItem(lua_State* L)
{
    const int  type = (int)lua_tointeger(L, 1);
    const int  level = (int)lua_tointeger(L, 2);
    const char* line1 = lua_tostring(L, 3);
    const char* line2 = lua_tostring(L, 4);
    const int  color1 = (int)lua_tointeger(L, 5);
    const int  color2 = (int)lua_tointeger(L, 6);
    const bool bold1 = lua_toboolean(L, 7) ? true : false;
    const bool bold2 = lua_toboolean(L, 8) ? true : false;

    ITEM_OPTION_INFO info{};
    info.ItemType = type;
    info.MinLevel = 0;   
    info.ExactLevel = level; 
    info.Line1 = line1 ? line1 : "";
    info.Line2 = line2 ? line2 : "";
    info.Color1 = color1;
    info.Color2 = color2;
    info.Bold1 = bold1;
    info.Bold2 = bold2;

    auto& rules = gItemNewOption.m_rules[type];
    rules.exactByLevel[level] = std::move(info);
    return 0;
}

int CItemNewOption::LuaGetItemModel(lua_State* L)
{
    const int section = (int)lua_tointeger(L, 1);
    const int index = (int)lua_tointeger(L, 2);
    lua_pushinteger(L, MakeType(section, index));
    return 1;
}

bool CItemNewOption::GetLinesFor(int itemType, int itemLevel,
    std::string& out1, std::string& out2,
    int& col1, int& col2, bool& b1, bool& b2) const
{
    auto it = m_rules.find(itemType);
    if (it == m_rules.end())
        return false;

    const PerTypeRules& rules = it->second;

    auto ex = rules.exactByLevel.find(itemLevel);
    if (ex != rules.exactByLevel.end())
    {
        const ITEM_OPTION_INFO& opt = ex->second;
        out1 = opt.Line1; out2 = opt.Line2;
        col1 = opt.Color1; col2 = opt.Color2;
        b1 = opt.Bold1; b2 = opt.Bold2;
        return true;
    }

    const ITEM_OPTION_INFO* best = nullptr;
    for (const auto& opt : rules.thresholds)
    {
        if (opt.MinLevel <= itemLevel)
            best = &opt;
        else
            break;
    }

    if (!best)
        return false;

    out1 = best->Line1; out2 = best->Line2;
    col1 = best->Color1; col2 = best->Color2;
    b1 = best->Bold1; b2 = best->Bold2;
    return true;
}
