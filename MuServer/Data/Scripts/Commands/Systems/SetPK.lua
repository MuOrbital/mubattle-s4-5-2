if not SETPK_CONFIG or not SETPK_CONFIG.Enabled then
    return
end

SetPK = {}

local function GetLang()
    return "Por"
end

local function Msg(aIndex, id, ...)
    local msg = SETPK_MESSAGES[GetLang()][id]
    if msg then
        SendMessage(string.format(msg, ...), aIndex, 1)
    end
end

function SetPK.Command(aIndex, text)
    local p = User.new(aIndex)

    if p:getAuthority() < SETPK_CONFIG.GMLevel then
        Msg(aIndex, 1)
        return
    end

    if not text or text == "" then
        Msg(aIndex, 2)
        return
    end

    local args = {}

    for word in string.gmatch(text, "%S+") do
        table.insert(args, word)
    end

    if #args < 3 then
        Msg(aIndex, 2)
        return
    end

    local targetName = args[2]
    local pkLevel = tonumber(args[3])

    if not pkLevel or pkLevel < 0 or pkLevel > 6 then
        Msg(aIndex, 2)
        return
    end

    local targetIndex = GetIndex(targetName)

    if targetIndex == -1 then
        Msg(aIndex, 3)
        return
    end

    local target = User.new(targetIndex)

    target:setPKLevel(pkLevel)
    target:setPKCount(0)
    PkLevelSend(targetIndex, pkLevel)

    Msg(aIndex, 4, targetName, pkLevel)
    Msg(targetIndex, 5, pkLevel)
end

function SetPK.Init()
    if not SETPK_CONFIG.Enabled then return end

    Commands.Register(SETPK_CONFIG.Command, SetPK.Command)
end

SetPK.Init()
return SetPK