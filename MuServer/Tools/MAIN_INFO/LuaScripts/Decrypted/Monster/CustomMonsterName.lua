CUSTOM_MONSTER_NAME_INFO = {
    { Class = 600, Map = 0, CoordX = 130, CoordY = 130, Name = "NpcName - Activate" },
}


function StartLoadCustomMonsterName()
    for i = 1, #CUSTOM_MONSTER_NAME_INFO do
        monsterInfo = CUSTOM_MONSTER_NAME_INFO[i]

        SetCustomMonsterName(monsterInfo.Class, monsterInfo.Map, monsterInfo.CoordX, monsterInfo.CoordY, monsterInfo.Name)
    end
end

StartLoadCustomMonsterName()