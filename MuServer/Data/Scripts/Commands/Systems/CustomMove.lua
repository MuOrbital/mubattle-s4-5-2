local function GetPlayerLanguage(aIndex)
    return "Por"
end

local function SendMsg(aIndex, msgId, ...)
    local lang = GetPlayerLanguage(aIndex)
    local msgs = CUSTOM_MOVE_MESSAGES[lang] or CUSTOM_MOVE_MESSAGES["Por"]
    local text = msgs[msgId] or ("[Mensagem " .. msgId .. "]")
    SendMessage(aIndex, 1, string.format(text, ...))
end

local function CanUseTeleport(player, move)
    if not CUSTOM_MOVE_CONFIG.Enabled then
        SendMsg(player:getIndex(), 5)
        return false
    end

    local level      = player:getLevel()
    local accLevel   = player:getVip() or 0
    local pkLevel    = player:getPKLevel()
    local authority  = player:getAuthority()

    if CUSTOM_MOVE_CONFIG.GMLevelOverride > 0 and authority >= CUSTOM_MOVE_CONFIG.GMLevelOverride then
        return true
    end

    if authority < move.requiredGM then
        SendMsg(player:getIndex(), 1)
        return false
    end

    local minLv = (move.minLevel == "*") and 1 or move.minLevel
    local maxLv = (move.maxLevel == "*") and 999999 or move.maxLevel

    if level < minLv or level > maxLv then
        SendMsg(player:getIndex(), 2, level, minLv, maxLv)
        return false
    end

    if accLevel < move.minAccountLevel then
        SendMsg(player:getIndex(), 3, move.minAccountLevel)
        return false
    end

    if pkLevel >= 6 and move.allowPKMove == 0 then
        SendMsg(player:getIndex(), 4)
        return false
    end

    return true
end

local function PerformTeleport(aIndex, cmd)
    local move = nil
    
    for _, entry in ipairs(CUSTOM_MOVES) do
        if entry[1] == cmd then
            move = {
                map             = entry[2],
                x               = entry[3],
                y               = entry[4],
                minLevel        = entry[5],
                maxLevel        = entry[6],
                minAccountLevel = entry[7],
                allowPKMove     = entry[8],
                requiredGM      = entry[9],
            }
            break
        end
    end

    if not move then
        SendMsg(aIndex, 8)
        return
    end

    local player = User.new(aIndex)

    if not CanUseTeleport(player, move) then
        return
    end

    Teleport(aIndex, move.map, move.x, move.y)

    if CUSTOM_MOVE_CONFIG.ShowSuccessMsg then
        SendMsg(aIndex, 6, cmd, move.x, move.y)
    end
end

if CUSTOM_MOVE_CONFIG.Enabled then
    for _, entry in ipairs(CUSTOM_MOVES) do
        local comando = entry[1]
        Commands.Register(comando, function(aIndex)
            PerformTeleport(aIndex, comando)
        end)
    end
end