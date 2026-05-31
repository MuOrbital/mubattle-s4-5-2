MonsterDrop = {}
function MonsterDrop.Init()
    if MonsterDrop_Config.Enabled then
        GameServerFunctions.MonsterDie(MonsterDrop.GiveItem)
    end
end
function MonsterDrop.GiveItem(Player, Monster)
    local m = User.new(Monster)
    local p = User.new(Player)
    local lang = p:getLanguage()
    local mClass = m:getClass()
    local mMap = m:getMapNumber()
    local mX = m:getX()
    local mY = m:getY()
    local config = -1
    for i, monster in ipairs(MonsterDrop_Config.Monster) do
        if monster.Class == mClass or monster.Class == -1 then
            if monster.Local ~= -1 then
                if MonsterDrop_Config.Local[monster.Local] == nil then
                    LogAdd(string.format("Monster Drop local config error on index %d", i))
                    goto continue_specific
                end
            
                local loc = MonsterDrop_Config.Local[monster.Local]
                if (loc.Mapa == mMap or loc.Mapa == -1)
                and mX >= loc.Start.X and mX <= loc.End.X
                and mY >= loc.Start.Y and mY <= loc.End.Y then
                    config = MonsterDrop_Config.Monster[i]
                    goto found
                end
            end
        end
        ::continue_specific::
    end
    for i, monster in ipairs(MonsterDrop_Config.Monster) do
        if (monster.Class == mClass or monster.Class == -1)
        and monster.Local == -1 then
            config = MonsterDrop_Config.Monster[i]
            goto found
        end
    end
    ::found::
    if config == -1 then return 0 end
    local drop = 0
    if config.Drop.Enabled then
        drop = 1
    end
    local returnType = 0
    if drop == 1 then
        if MonsterDrop_Config.Drop[config.Drop.Index] == nil then
            returnType = 0
            goto continua
        end
    
        local dropList = MonsterDrop_Config.Drop[config.Drop.Index]
    
        for i, item in ipairs(dropList) do
            if type(item) == "table" and item.SpecialValue ~= nil and item.SpecialValue > 0 then
                if Utils.GetSuccessByRate(item.Rate) then
                    DropEventItemBag(Player, item.SpecialValue)
                end
            end
        end
        
        returnType = 1
    end
    ::continua::
    return returnType
end
function MonsterDrop.GetItem(Drop, tipo)
    local t = {}
    for i, dropItem in ipairs(Drop) do
        if type(dropItem.Rate) == "number" then
            for x = 1, (dropItem.Rate/100)*10000 do
                table.insert(t, dropItem)
            end
        end
    end
    if #t == 0 then
        if tipo == 0 then
            t = {SpecialValue = -1}
            return t
        else
            return -1
        end
    end
    return t[Utils.GetRandom(1, #t, math.random(1, 100))]
end
MonsterDrop.Init()
return MonsterDrop