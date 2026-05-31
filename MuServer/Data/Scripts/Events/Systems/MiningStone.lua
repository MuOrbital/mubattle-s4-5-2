MiningStone = {}
MiningStone.NPC = {}
MiningStone.Player = {}
local Active = false
local EndTime = 0
local TimerId = nil
local TotalStones = 0
local RemainingStones = 0
math.randomseed(os.time() + os.clock() * 1000)

local function MS_Global(id, ...)
    if Message and Message.SendMessageGlobalMultLangArgs then
        Message.SendMessageGlobalMultLangArgs(MININGSTONE_MESSAGES, id, 0, ...)
    else
        local msg = MININGSTONE_MESSAGES["Por"][id]
        if msg then
            SendMessageGlobal(string.format(msg, ...), 0)
        end
    end
end

local function MS_Private(aIndex, id, ...)
    local msg = MININGSTONE_MESSAGES["Por"][id]
    if msg then
        SendMessage(string.format(msg, ...), aIndex, 1)
    end
end

local function GetActiveMaps()
    local mapConfig = MiningStone_Config.Maps or {}
    local mapKeys = {}
    for k in pairs(mapConfig) do
        table.insert(mapKeys, k)
    end
    if #mapKeys == 0 then return {} end
    if MiningStone_Config.MapType == 0 then
        return mapKeys
    else
        local chosen = mapKeys[math.random(1, #mapKeys)]
        return {chosen}
    end
end

function MiningStone.Spawn()
    MiningStone.NPC = {}
    local activeMapNumbers = GetActiveMaps()
    TotalStones = #activeMapNumbers * (MiningStone_Config.StonesPerMap or 0)
    RemainingStones = 0

    for _, map in ipairs(activeMapNumbers) do
        for i = 1, MiningStone_Config.StonesPerMap do
            local index = AddMonster(map)
            if index == -1 then goto continue end
            SetPositionMonster(index, map)
            SetMonster(index, MiningStone_Config.MonsterClass)
            local mob = User.new(index)
            mob:setType(3)
            mob:setDir(2)
            MiningStone.NPC[index] = {
                map = mob:getMapNumber(),
                x = mob:getX(),
                y = mob:getY(),
                activeMiners = {}
            }
            RemainingStones = RemainingStones + 1
            LogAdd(string.format("[MiningStone] Stone - Map: %d (%s) X: %d Y: %d",
                mob:getMapNumber(),
                MiningStone_Config.Maps[mob:getMapNumber()] or "Desconhecido",
                mob:getX(), mob:getY()), 0)
            ::continue::
        end
    end

    TotalStones = RemainingStones
end

function MiningStone.Remove()
    for i in pairs(MiningStone.NPC) do
        gObjDel(i)
    end
    MiningStone.NPC = {}
end

function MiningStone.Start()
    if Active then return end
    Active = true
    EndTime = os.time() + (MiningStone_Config.Duration * 60)
    
    MiningStone.Spawn()
    
    local detectedMaps = {}
    for _, stone in pairs(MiningStone.NPC) do
        local m = stone.map
        detectedMaps[m] = true
    end
    
    local mapList = {}
    for m in pairs(detectedMaps) do
        table.insert(mapList, m)
    end
    
    Timer.TimeOut(1, function()
        if MiningStone_Config.ShowMapInStartMessage == 1 and #mapList == 1 and MiningStone_Config.MapType == 1 then
            local mapNumber = mapList[1]
            local mapName = MiningStone_Config.Maps[mapNumber] or "Mapa Desconhecido"
            MS_Global(1, mapName)
        else
            MS_Global(13)
        end
    end)
    
    if TimerId then
        Timer.Cancel(TimerId)
        TimerId = nil
    end
    TimerId = Timer.TimeOut(MiningStone_Config.Duration * 60, function()
        MiningStone.End()
    end)
end

function MiningStone.End()
    Active = false
    if TimerId then
        Timer.Cancel(TimerId)
        TimerId = nil
    end
    for aIndex in pairs(MiningStone.Player) do
        MiningStone.CancelMining(aIndex)
    end
    MiningStone.Player = {}
    MiningStone.Remove()
    TotalStones = 0
    RemainingStones = 0
    Timer.TimeOut(1, function() MS_Global(2) end)
end

function MiningStone.StartAnnouncements(announceTime)
    local remainingMinutes = announceTime
    local function DoAnnouncement()
        if remainingMinutes > 0 then
            MS_Global(11, remainingMinutes)
            remainingMinutes = remainingMinutes - 1
            Timer.TimeOut(60, DoAnnouncement)
        else
            MiningStone.Start()
        end
    end
    DoAnnouncement()
end

function MiningStone.Init()
    if not MiningStone_Config.Enabled then return end
    GameServerFunctions.NpcTalk(MiningStone.NpcTalk)
    GameServerFunctions.GameServerProtocol(MiningStone.Protocol)
    GameServerFunctions.PlayerMove(MiningStone.OnPlayerMove)
    GameServerFunctions.CharacterMove(MiningStone.OnCharacterMove)
    GameServerFunctions.PlayerDie(MiningStone.OnPlayerDie)
    GameServerFunctions.RespawnUser(MiningStone.OnRespawnUser)
    GameServerFunctions.CharacterSet(MiningStone.OnCharacterSet)
    GameServerFunctions.PlayerLogout(MiningStone.OnPlayerLogout)
    Commands.Register(MiningStone_Config.Command, MiningStone.Command)
    if MiningStone_Config.AUTO_START then
        for _, event in ipairs(MiningStone_Config.AUTO_START) do
            local announceTime = event.ANNOUNCE_TIME or 5
            if announceTime > 0 then
                local announceStartTime = (event.HOUR * 60 + event.MINUTE) - announceTime
                if announceStartTime < 0 then announceStartTime = announceStartTime + 1440 end
                local announceHour = math.floor(announceStartTime / 60)
                local announceMinute = announceStartTime % 60
                local announceFunc = function() MiningStone.StartAnnouncements(announceTime) end
                if event.DAY_OF_WEEK ~= -1 then
                    Schedule.SetDayOfWeek(event.DAY_OF_WEEK, announceHour, announceMinute, announceFunc)
                else
                    Schedule.SetHourAndMinute(announceHour, announceMinute, announceFunc)
                end
            end
            local startFunc = function() MiningStone.Start() end
            if event.DAY_OF_WEEK ~= -1 then
                Schedule.SetDayOfWeek(event.DAY_OF_WEEK, event.HOUR, event.MINUTE, startFunc)
            else
                Schedule.SetHourAndMinute(event.HOUR, event.MINUTE, startFunc)
            end
        end
    end
end

function MiningStone.OnPlayerMove(aIndex, map, x, y, sx, sy)
    local info = MiningStone.Player[aIndex]
    if not info then return end
    local stone = MiningStone.NPC[info.npc]
    if not stone then return end
    local dist = math.sqrt((x - stone.x)^2 + (y - stone.y)^2)
    local range = MiningStone_Config.MiningRange or 6
    if dist > range then
        MiningStone.CancelMining(aIndex)
    end
end

function MiningStone.OnCharacterMove(aIndex, map, x, y)
    local info = MiningStone.Player[aIndex]
    if not info then return end
    local stone = MiningStone.NPC[info.npc]
    if not stone then return end
    local dist = math.sqrt((x - stone.x)^2 + (y - stone.y)^2)
    local range = MiningStone_Config.MiningRange or 6
    if dist > range or map ~= info.map then
        MiningStone.CancelMining(aIndex)
    end
end

function MiningStone.CancelMining(aIndex)
    local info = MiningStone.Player[aIndex]
    if not info then return end
    local stone = MiningStone.NPC[info.npc]
    if stone and stone.activeMiners then
        stone.activeMiners[aIndex] = nil
    end
    MiningStone.Player[aIndex] = nil
    MS_Private(aIndex, 9)
    local pname = string.format("MININGCANCEL-%d", aIndex)
    CreatePacket(pname, MiningStone_Config.Packet)
    SendPacket(pname, aIndex)
    ClearPacket(pname)
end

function MiningStone.NpcTalk(npc, aIndex)
    local stone = MiningStone.NPC[npc]
    if not stone then
        return 0
    end

    if not Active then
        SendMessage("O evento não está ativo no momento.", aIndex, 1)
        return 1
    end

    if MiningStone.Player[aIndex] then
        SendMessage("Você já está minerando uma pedra!", aIndex, 1)
        return 1
    end

    stone.activeMiners[aIndex] = true
    local player = User.new(aIndex)

    MiningStone.Player[aIndex] = {
        start = os.time(),
        npc = npc,
        map = player:getMapNumber(),
        x = player:getX(),
        y = player:getY(),
        name = player:getName()
    }

    local baseMsg = "[MiningStone] %s está minerando uma Stone!"
    local extra = ""
    local mapName = MiningStone_Config.Maps[player:getMapNumber()] or "Desconhecido"
    local show = MiningStone_Config.ShowLocationInMiningMessage or 0

    if show == 1 then
        extra = " (" .. mapName .. ")"
    elseif show == 2 then
        extra = " (" .. mapName .. " - X:" .. player:getX() .. " Y:*** )"
    elseif show == 3 then
        extra = " (" .. mapName .. " - X:" .. player:getX() .. " Y:" .. player:getY() .. ")"
    end

    local fullMsg = baseMsg .. extra
    SendMessageGlobal(fullMsg:format(player:getName()), 0)

    MS_Private(aIndex, 5)

    local pname = string.format("MININGSTART-%s-%d", player:getName(), aIndex)
    CreatePacket(pname, MiningStone_Config.Packet)
    SendPacket(pname, aIndex)
    ClearPacket(pname)

    return 1
end

function MiningStone.Protocol(aIndex, Packet, PacketName)
    if not string.find(PacketName, "MININGFINISH") then return false end
    local info = MiningStone.Player[aIndex]
    if not info then return false end
    if os.time() - info.start < MiningStone_Config.MiningTime then return false end
    local stone = MiningStone.NPC[info.npc]
    if not stone then return false end
    local totalRate = 0
    for _, drop in ipairs(MiningStone_Config.Drops) do
        totalRate = totalRate + (drop.Rate or 0)
    end
    if totalRate <= 0 then return false end
    local dropsCount = MiningStone_Config.DropsPerStone or 1
    for i = 1, dropsCount do
        local rand = math.random() * totalRate
        local cumulative = 0
        local selectedDrop = nil
        for _, drop in ipairs(MiningStone_Config.Drops) do
            cumulative = cumulative + (drop.Rate or 0)
            if rand <= cumulative then
                selectedDrop = drop
                break
            end
        end
        if not selectedDrop then selectedDrop = MiningStone_Config.Drops[1] end
        local ItemCode = GET_ITEM(selectedDrop.Section, selectedDrop.Index)
        ItemSerialCreateComplete(
            aIndex, stone.map, stone.x, stone.y,
            ItemCode, selectedDrop.Level or 0, 0,
            0, 0, 0, aIndex, 0, 0, 0
        )
    end
    gObjDel(info.npc)
    if stone.activeMiners then
        for otherIndex in pairs(stone.activeMiners) do
            if otherIndex ~= aIndex then
                MiningStone.CancelMining(otherIndex)
            end
        end
    end
    MiningStone.NPC[info.npc] = nil
    MiningStone.Player[aIndex] = nil
    if RemainingStones > 0 then
        RemainingStones = RemainingStones - 1
    end
    local player = User.new(aIndex)
    MS_Global(4, player:getName())
    if MiningStone_Config.ShowRemainingStonesMessage == 1 then
        MS_Global(15, RemainingStones, TotalStones)
    end
    MS_Private(aIndex, 6)
    return true
end

function MiningStone.OnCharacterSet(aIndex)
    local info = MiningStone.Player[aIndex]
    if not info then return end
    local player = User.new(aIndex)
    if not player then return end
    if player:getDieRegen() == 1 or player:getLife() <= 0 then
        MiningStone.CancelMining(aIndex)
    end
end

function MiningStone.OnPlayerDie(aIndex, TargetIndex)
    if MiningStone.Player[TargetIndex] then
        MiningStone.CancelMining(TargetIndex)
    end
end

function MiningStone.OnRespawnUser(aIndex)
    if MiningStone.Player[aIndex] then
        MiningStone.CancelMining(aIndex)
    end
end

function MiningStone.OnPlayerLogout(aIndex)
    if MiningStone.Player[aIndex] then
        MiningStone.CancelMining(aIndex)
    end
end

function MiningStone.Command(aIndex)
    local p = User.new(aIndex)
    if CheckGameMasterLevel(p:getAccountID(), p:getName(), MiningStone_Config.GMLevel) == 0 then
        SendMessage("Sem permissão.", aIndex, 1)
        return
    end
    if Active then
        if TimerId then
            Timer.Cancel(TimerId)
            TimerId = nil
        end
        MiningStone.End()
        SendMessage("Evento MiningStone finalizado manualmente.", aIndex, 1)
    else
        MiningStone.Start()
        SendMessage("Evento MiningStone iniciado manualmente.", aIndex, 1)
    end
end

GameServerFunctions.CharacterSet(MiningStone.OnCharacterSet)
GameServerFunctions.PlayerDie(MiningStone.OnPlayerDie)
GameServerFunctions.RespawnUser(MiningStone.OnRespawnUser)
GameServerFunctions.PlayerLogout(MiningStone.OnPlayerLogout)
MiningStone.Init()
return MiningStone