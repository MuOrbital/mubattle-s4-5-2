#pragma once

#include "Lua.h"
#include <map>
#include <string>
#include <vector>
#include <algorithm>

#ifndef MAX_ITEM_INDEX
#define MAX_ITEM_INDEX 512
#endif

struct ITEM_OPTION_INFO
{
    int   ItemType;    // section*MAX_ITEM_INDEX + index
    int   MinLevel;    // para regras por limiar (>=)
    int   ExactLevel;  // para regra exata (==); use -1 quando não for exata

    std::string Line1;
    std::string Line2;

    int   Color1;
    int   Color2;
    bool  Bold1;
    bool  Bold2;
};

class CItemNewOption
{
public:
    CItemNewOption();
    virtual ~CItemNewOption();

    void Init();
    void RestartLua();

    // Consulta para RenderItemInfo (sem ITEM no header):
    bool GetLinesFor(int itemType, int itemLevel,
        std::string& out1, std::string& out2,
        int& col1, int& col2, bool& b1, bool& b2) const;

    // === Lua bindings ===
    // Regras por limiar (mínimo): LoadNewOption(type, minLevel, line1, line2, color1, color2, bold1, bold2)
    static int LuaLoadNewOption(lua_State* L);
    // Regras por nível exato: NewOptionItem(type, level, line1, line2, color1, color2, bold1, bold2)
    static int LuaNewOptionItem(lua_State* L);
    // Helper para montar o type:
    static int LuaGetItemModel(lua_State* L);

private:
    static int MakeType(int section, int index);

private:
    // Para cada type, guardamos:
    // - regras exatas por nível
    // - regras por limiar (ordenadas por MinLevel)
    struct PerTypeRules {
        std::map<int, ITEM_OPTION_INFO> exactByLevel; // level -> info
        std::vector<ITEM_OPTION_INFO>   thresholds;   // list com MinLevel, ordenada asc
    };

    std::map<int, PerTypeRules> m_rules; // type -> rules
    Lua m_Lua;
};

extern CItemNewOption gItemNewOption;
