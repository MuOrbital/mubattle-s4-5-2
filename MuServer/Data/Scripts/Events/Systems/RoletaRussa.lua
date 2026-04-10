--[[
    +--------------------------------------------------------------------+
    | Roleta Russa                             |
    |--------------------------------------------------------------------|
    | Criado por: Mineiro                                                |
    +--------------------------------------------------------------------+
--]]

if not RoletaRussa_Config or not RoletaRussa_Config.Enabled then
    return
end
RoletaRussa = {
    Players    = {},
    Alive      = {},
    LastKilled = -1,
    State      = 0,
    JoinEndTime= 0,
    TimerId    = nil,
}

local function RR_Global(langId, ...)
    local lang = "Por"
    local msg = RoletaRussa_Messages[lang][langId]
    if msg then
        SendMessageGlobal(string.format(msg, ...), 0)
    end
end

local function RR_Private(aIndex, langId, ...)
    local lang = "Por"
    local msg = RoletaRussa_Messages[lang][langId]
    if msg then
        SendMessage(string.format(msg, ...), aIndex, 1)
    end
end

local function TeleportToEvent(aIndex)
    if RoletaRussa_Config.EventMap and RoletaRussa_Config.EventMap >= 0 then
        Teleport(aIndex, RoletaRussa_Config.EventMap, RoletaRussa_Config.EventX, RoletaRussa_Config.EventY)
        RR_Private(aIndex, 18)
    end
end

local function IsValidPlayer(aIndex)
    local p = User.new(aIndex)
    return p and p:getConnected() == 2 and p:getLive() == 1
end

local function GetAlivePlayersList()
    local list = {}
    for _, aIndex in ipairs(RoletaRussa.Players) do
        if RoletaRussa.Alive[aIndex] then
            table.insert(list, aIndex)
        end
    end
    return list
end

local function IsParticipant(aIndex)
    for _, idx in ipairs(RoletaRussa.Players) do
        if idx == aIndex then
            return true
        end
    end
    return false
end
local function PlayerAttack(aIndex, bIndex)
    if RoletaRussa.State ~= 1 and RoletaRussa.State ~= 2 then
        return 0
    end

    if IsParticipant(aIndex) and IsParticipant(bIndex) then
        return 1
    end

    return 0
end

local function SelectNextVictim()
    local alive = GetAlivePlayersList()
    if #alive <= 1 then
        if #alive == 1 then
            RR_Global(13, User.new(alive[1]):getName())
        end
        RoletaRussa.State = 0
        RoletaRussa.Players = {}
        RoletaRussa.Alive = {}
        RoletaRussa.LastKilled = -1
        return
    end

    local candidate
    local attempts = 0
    repeat
        candidate = alive[math.random(1, #alive)]
        attempts = attempts + 1
    until candidate ~= RoletaRussa.LastKilled or attempts > 20

    RoletaRussa.LastKilled = candidate

    local p = User.new(candidate)
    local pos = 0
    for i, idx in ipairs(RoletaRussa.Players) do
        if idx == candidate then pos = i break end
    end

    RR_Global(6, pos, p:getName())

    Timer.TimeOut(2, function()
        RR_Global(7)
        Timer.TimeOut(2, function()
            RR_Global(8)
            Timer.TimeOut(2, function()
                RR_Global(9)
                Timer.TimeOut(2, function()
                    RR_Global(10)

                    local survive = math.random(1, 100) <= RoletaRussa_Config.ChanceSurvive

                    Timer.TimeOut(1, function()
                        if survive then
                            RR_Global(11, pos, p:getName())
                            Timer.TimeOut(3, SelectNextVictim)
                        else
                            RR_Global(12, pos, p:getName())
                            if RoletaRussa_Config.DeathEffectID and RoletaRussa_Config.DeathEffectID > 0 then
                                AddEffect(candidate, 0, RoletaRussa_Config.DeathEffectID, 7, 0, 0, 0, 0)
                            end
                            KillPlayer(candidate)
                            RoletaRussa.Alive[candidate] = false
                            Timer.TimeOut(4, SelectNextVictim)
                        end
                    end)
                end)
            end)
        end)
    end)
end

local function StartGame()
    local participantCount = #RoletaRussa.Players

    if participantCount < RoletaRussa_Config.MinPlayers then
        RR_Global(5, RoletaRussa_Config.MinPlayers)
        RoletaRussa.State = 0
        RoletaRussa.Players = {}
        RoletaRussa.Alive = {}
        return
    end

    RR_Global(4, participantCount)

    for _, aIndex in ipairs(RoletaRussa.Players) do
        RoletaRussa.Alive[aIndex] = true
    end

    RoletaRussa.State = 2
    RoletaRussa.LastKilled = -1

    Timer.TimeOut(4, SelectNextVictim)
end

local function OpenEvent(aIndex)
    local p = User.new(aIndex)
    if not p or p:getAuthority() < 32 then
        SendMessage("Sem permissão.", aIndex, 1)
        return
    end
    if RoletaRussa.State ~= 0 then
        SendMessage("Já existe um evento ativo.", aIndex, 1)
        return
    end
    TeleportToEvent(aIndex)

    RoletaRussa.State = 1
    RoletaRussa.Players = {}
    RoletaRussa.Alive = {}
    RoletaRussa.JoinEndTime = os.time() + RoletaRussa_Config.JoinTime

    RR_Global(14, RoletaRussa_Config.JoinTime)
    RR_Global(1)
    RR_Global(2, RoletaRussa_Config.CommandJoin)

    local remaining = RoletaRussa_Config.JoinTime
    local function countdown()
        if RoletaRussa.State ~= 1 then return end
        if remaining <= 0 then
            StartGame()
            return
        end
        if remaining == 30 or remaining == 10 or remaining <= 5 then
            RR_Global(15, remaining)
        end
        remaining = remaining - 1
        Timer.TimeOut(1, countdown)
    end
    Timer.TimeOut(1, countdown)
end

local function JoinEvent(aIndex)
    if RoletaRussa.State ~= 1 then
        RR_Private(aIndex, 17)
        return
    end
    if #RoletaRussa.Players >= RoletaRussa_Config.MaxPlayers then
        SendMessage("Evento lotado!", aIndex, 1)
        return
    end
    for _, idx in ipairs(RoletaRussa.Players) do
        if idx == aIndex then
            RR_Private(aIndex, 16)
            return
        end
    end

    local p = User.new(aIndex)

    p:setPKLevel(3)
    p:setPKCount(0)
    PkLevelSend(aIndex, 3)

    TeleportToEvent(aIndex)

    table.insert(RoletaRussa.Players, aIndex)
    RR_Global(3, #RoletaRussa.Players, p:getName(), #RoletaRussa.Players, RoletaRussa_Config.MaxPlayers)
end

Commands.Register(RoletaRussa_Config.CommandOpen, function(aIndex)
    OpenEvent(aIndex)
end)

Commands.Register(RoletaRussa_Config.CommandJoin, function(aIndex)
    JoinEvent(aIndex)
end)

GameServerFunctions.PlayerAttack(PlayerAttack)

GameServerFunctions.PlayerDie(function(aIndex, killer)
    if RoletaRussa.State == 2 then
        RoletaRussa.Alive[aIndex] = false
    end
end)

GameServerFunctions.PlayerLogout(function(aIndex)
    if RoletaRussa.State >= 1 and RoletaRussa.State <= 2 then
        for i, idx in ipairs(RoletaRussa.Players) do
            if idx == aIndex then
                table.remove(RoletaRussa.Players, i)
                break
            end
        end
        RoletaRussa.Alive[aIndex] = nil
    end
end)

return RoletaRussa