if not ABRIREVENTO_CONFIG or not ABRIREVENTO_CONFIG.Enabled then
    return
end
ABRIREVENTO = {
    Active = false,
    GMIndex = nil,
    EndTime = 0,
    TimerId = nil,
    CountdownId = nil,
    Participants = {},
    LastMessageTime = 0,
    LastRemaining = 0,
    EventMap = 0,
    EventX = 0,
    EventY = 0,
}
local function GetLanguage()
    return "Por"
end
local function GlobalMsg(msgId, ...)
    local lang = GetLanguage()
    local msg = ABRIREVENTO_MESSAGES[lang][msgId]
    if msg then
        SendMessageGlobal(string.format(msg, ...), 0)
    end
end
local function PrivateMsg(aIndex, msgId, ...)
    local lang = GetLanguage()
    local msg = ABRIREVENTO_MESSAGES[lang][msgId]
    if msg then
        SendMessage(string.format(msg, ...), aIndex, 1)
    end
end
local function CloseEvent(manual)
    ABRIREVENTO.Active = false
    ABRIREVENTO.GMIndex = nil
    ABRIREVENTO.EndTime = 0
    ABRIREVENTO.LastMessageTime = 0
    ABRIREVENTO.LastRemaining = 0
   
    if ABRIREVENTO.TimerId then
        Timer.Cancel(ABRIREVENTO.TimerId)
        ABRIREVENTO.TimerId = nil
    end
   
    if ABRIREVENTO.CountdownId then
        Timer.Cancel(ABRIREVENTO.CountdownId)
        ABRIREVENTO.CountdownId = nil
    end
   
    ABRIREVENTO.Participants = {}
   
    if manual then
        GlobalMsg(9)
    else
        GlobalMsg(4)
    end
end
local function FinalCountdown()
    if not ABRIREVENTO.Active then return end
   
    local remaining = ABRIREVENTO.EndTime - os.time()
   
    if remaining <= 0 then
        GlobalMsg(10)
        CloseEvent(false)
        return
    end
    if remaining <= 10 and remaining ~= ABRIREVENTO.LastRemaining then
        GlobalMsg(8, remaining)
        ABRIREVENTO.LastRemaining = remaining
    end
    if ABRIREVENTO.CountdownId then
        Timer.Cancel(ABRIREVENTO.CountdownId)
        ABRIREVENTO.CountdownId = nil
    end
    if remaining > 0 then
        ABRIREVENTO.CountdownId = Timer.TimeOut(1, FinalCountdown)
    end
end
local function GM_OpenEvent(aIndex)
    local p = User.new(aIndex)
   
    if p:getAuthority() < ABRIREVENTO_CONFIG.GMMinLevel then
        PrivateMsg(aIndex, 1)
        return
    end
   
    if ABRIREVENTO.Active then
        local remaining = ABRIREVENTO.EndTime - os.time()
        PrivateMsg(aIndex, 2, remaining)
        return
    end
   
    local durationSec = ABRIREVENTO_CONFIG.DurationSeconds
   
    ABRIREVENTO.Active = true
    ABRIREVENTO.GMIndex = aIndex
    ABRIREVENTO.EndTime = os.time() + durationSec
    ABRIREVENTO.Participants = {}
    ABRIREVENTO.LastMessageTime = 0
    ABRIREVENTO.LastRemaining = 0
    ABRIREVENTO.EventMap = p:getMapNumber()
    ABRIREVENTO.EventX = p:getX()
    ABRIREVENTO.EventY = p:getY()
   
    GlobalMsg(3, p:getName(), durationSec)
   
    ABRIREVENTO.TimerId = Timer.TimeOut(durationSec, function()
        CloseEvent(false)
    end)
    if ABRIREVENTO.CountdownId then
        Timer.Cancel(ABRIREVENTO.CountdownId)
        ABRIREVENTO.CountdownId = nil
    end
   
    if ABRIREVENTO_CONFIG.FinalCountdown then
        FinalCountdown()
    end
end
local function Player_Join(aIndex)
    if not ABRIREVENTO.Active then
        PrivateMsg(aIndex, 7)
        return
    end
   
    local p = User.new(aIndex)
    local playerName = p:getName()
   
    if ABRIREVENTO.Participants[playerName] then
        PrivateMsg(aIndex, 6)
        return
    end
   
    if ABRIREVENTO.GMIndex then
        local map = ABRIREVENTO.EventMap
        local x = ABRIREVENTO.EventX
        local y = ABRIREVENTO.EventY
           
        if p:getPKLevel() > 3 then
            p:setPKLevel(3)
            p:setPKCount(0)
            PkLevelSend(aIndex, 3)
        end
           
        Teleport(aIndex, map, x, y)
        PrivateMsg(aIndex, 5)
           
        GlobalMsg(11, playerName)
           
        ABRIREVENTO.Participants[playerName] = true
    end
end
Commands.Register(ABRIREVENTO_CONFIG.GMCommand, function(aIndex)
    GM_OpenEvent(aIndex)
end)
Commands.Register(ABRIREVENTO_CONFIG.PlayerCommand, function(aIndex)
    Player_Join(aIndex)
end)