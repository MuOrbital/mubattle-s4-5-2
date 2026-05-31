local CONFIG = {
    ENABLED = true,
    HEADCODE_HWID = 0xF1,
    SUBCODE_REQUEST = 0x01,
    SUBCODE_RESPONSE = 0x02,
    COMMAND_TIMEOUT = 5,
    BLOCK_COMMAND = "/block",
    ADMIN_ACCOUNTS = { "admin" },
    ADMIN_CHARACTERS = { "Admin" },
    MSG_TIMEOUT = "DETECTADO AÇÕES SUSPEITAS NO CLIENTE",
    MSG_HWID_BLOCKED = "Este computador está bloqueado permanentemente.",
    MSG_COMMAND_SUCCESS = "Jogador %s bloqueado por HWID com sucesso.",
    MSG_COMMAND_USAGE = "Uso: %s <nome do personagem>",
    MSG_NO_PERMISSION = "Você não tem permissão para usar este comando.",
    MSG_PLAYER_NOT_FOUND = "Jogador não encontrado ou não está online.",
    MSG_WAITING_HWID = "Solicitando HWID do jogador... aguarde.",
    MSG_HWID_TIMEOUT = "Falha ao obter HWID do jogador (timeout). Jogador desconectado por suspeita."
}
local PendingRequests = {}
local PlayerHWID = {}

local function IsPlayerAdmin(aIndex)
    if not gObjIsConnectedGP(aIndex) then return false end
    local player = User.new(aIndex)
    local account = string.upper(player:getAccountID() or "")
    local charName = string.upper(player:getName() or "")
    for _, acc in ipairs(CONFIG.ADMIN_ACCOUNTS) do
        if string.upper(acc) == account then return true end
    end
    for _, name in ipairs(CONFIG.ADMIN_CHARACTERS) do
        if string.upper(name) == charName then return true end
    end
    return false
end
local function KickPlayer(aIndex, reason)
    if not gObjIsConnectedGP(aIndex) then return end
    SendMessage(reason or "Você foi desconectado.", aIndex, 0)
    Timer.TimeOut(1, function()
        if gObjIsConnectedGP(aIndex) then
            CloseChar(aIndex, 0)
        end
    end)
end
local function NormalizeHWID(hwid)
    if not hwid or hwid == "" then return "" end
    return string.upper(string.gsub(hwid, "[-%s]", ""))
end
local function IsHWIDBlocked(hwid)
    if hwid == "" then return false end
    local clean = NormalizeHWID(hwid)
    local query = string.format("SELECT TOP 1 1 FROM AccountHWID WHERE HWID = '%s'", clean)
    local res = db:exec(query)
    if res ~= 0 then
        if db:fetch() ~= 100 then
            db:clear()
            return true
        end
    end
    db:clear()
    return false
end

local function KickAllWithSameHWID(blockedHwid, excludeIndex)
    if not blockedHwid or blockedHwid == "" then return end
    local clean = NormalizeHWID(blockedHwid)
    for i = 9000, 12000 do
        if gObjIsConnectedGP(i) and (not excludeIndex or i ~= excludeIndex) then
            local playerHwid = PlayerHWID[i]
            if playerHwid and NormalizeHWID(playerHwid) == clean then
                local player = User.new(i)
                local charName = player:getName() or "UNKNOWN"
                local account = player:getAccountID() or "UNKNOWN"
                
                KickPlayer(i, CONFIG.MSG_HWID_BLOCKED)
                LogAdd(string.format("[HWID Block] Desconectado por compartilhar HWID bloqueado (char: %s, conta: %s, aIndex: %d)", charName, account, i))
            end
        end
    end
end

local function SendHWIDRequest(targetIndex)
    local pktName = "ForceHWIDReq_" .. targetIndex
    CreatePacket(pktName, CONFIG.HEADCODE_HWID)
    SetBytePacket(pktName, CONFIG.SUBCODE_REQUEST)
    SendPacket(pktName, targetIndex)
    ClearPacket(pktName)
end
local function BlockWithHWID(targetIndex, adminIndex)
    local hwid = PlayerHWID[targetIndex] or ""
    if hwid == "" then return false end
    local targetPlayer = User.new(targetIndex)
    local account = targetPlayer:getAccountID() or "UNKNOWN"
    local charName = targetPlayer:getName() or "UNKNOWN"
    local cleanHwid = NormalizeHWID(hwid)
    local adminName = User.new(adminIndex):getName() or "Admin"
    local insertQuery = string.format([[
        IF NOT EXISTS (SELECT 1 FROM AccountHWID WHERE HWID = '%s')
            INSERT INTO AccountHWID (memb___id, HWID, BloqueadoPor)
            VALUES ('%s', '%s', '%s')
    ]], cleanHwid, account, cleanHwid, adminName)
    db:exec(insertQuery)
    db:clear()
    
    KickAllWithSameHWID(hwid, targetIndex)
    
    KickPlayer(targetIndex, CONFIG.MSG_HWID_BLOCKED)
    SendMessage(string.format(CONFIG.MSG_COMMAND_SUCCESS, charName), adminIndex, 0)
    LogAdd(string.format("[HWID Block] HWID %s bloqueado (conta: %s, char: %s, por: %s)", cleanHwid, account, charName, adminName))
    PlayerHWID[targetIndex] = nil
    if PendingRequests[targetIndex] then
        Timer.Cancel(PendingRequests[targetIndex].timerID)
        PendingRequests[targetIndex] = nil
    end
    return true
end

local function FindPlayerIndexByName(name)
    local search = string.upper(name or "")
    if search == "" then return -1 end
    for i = 9000, 12000 do
        if gObjIsConnectedGP(i) then
            local p = User.new(i)
            if string.upper(p:getName() or "") == search then
                return i
            end
        end
    end
    return -1
end

local function CommandBlockHWID(aIndex, args)
    if not CONFIG.ENABLED or not IsPlayerAdmin(aIndex) then
        SendMessage(CONFIG.MSG_NO_PERMISSION, aIndex, 1)
        return
    end
    local targetName = command:getString(args, 1, "")
    if targetName == "" then
        SendMessage(string.format(CONFIG.MSG_COMMAND_USAGE, CONFIG.BLOCK_COMMAND), aIndex, 1)
        return
    end
    local targetIndex = FindPlayerIndexByName(targetName)
    if targetIndex == -1 or not gObjIsConnectedGP(targetIndex) then
        SendMessage(CONFIG.MSG_PLAYER_NOT_FOUND, aIndex, 1)
        return
    end
    if PlayerHWID[targetIndex] and PlayerHWID[targetIndex] ~= "" then
        BlockWithHWID(targetIndex, aIndex)
        return
    end
    SendMessage(CONFIG.MSG_WAITING_HWID, aIndex, 1)
    SendHWIDRequest(targetIndex)
    local timerID = Timer.TimeOut(CONFIG.COMMAND_TIMEOUT, function()
        PendingRequests[targetIndex] = nil
        SendMessage(CONFIG.MSG_HWID_TIMEOUT, aIndex, 1)
        KickPlayer(targetIndex, CONFIG.MSG_TIMEOUT)
        LogAdd(string.format("[HWID Block] Timeout ao forçar HWID do jogador %s (aIndex %d)", targetName, targetIndex))
    end)
    PendingRequests[targetIndex] = { timerID = timerID, adminIndex = aIndex }
end
Commands.Register(CONFIG.BLOCK_COMMAND, CommandBlockHWID)

function OnPlayerEnter(aIndex)
    if not CONFIG.ENABLED or not gObjIsConnectedGP(aIndex) then return end
    PlayerHWID[aIndex] = nil
    SendHWIDRequest(aIndex)
    local timerID = Timer.TimeOut(CONFIG.COMMAND_TIMEOUT, function()
        if gObjIsConnectedGP(aIndex) then
            KickPlayer(aIndex, CONFIG.MSG_TIMEOUT)
        end
        PendingRequests[aIndex] = nil
    end)
    PendingRequests[aIndex] = { timerID = timerID }
end
function OnHWIDProtocol(aIndex, headcode, packetName)
    if not CONFIG.ENABLED or headcode ~= CONFIG.HEADCODE_HWID then return false end
    local subcode = GetBytePacket(packetName, -1)
    if subcode ~= CONFIG.SUBCODE_RESPONSE then return false end
    local rawHwid = GetCharPacketLength(packetName, -1, 40)
    ClearPacket(packetName)
    local hwid = NormalizeHWID(rawHwid)
    PlayerHWID[aIndex] = hwid

    local formattedHwid = ""
    if #hwid >= 32 then
        formattedHwid = hwid:sub(1,8) .. "-" .. hwid:sub(9,16) .. "-" .. hwid:sub(17,24) .. "-" .. hwid:sub(25,32)
    else
        formattedHwid = hwid
    end

    LogAdd(string.format("[Reg HWID] Login: %s | Personagem: %s | HWID: %s", 
        User.new(aIndex):getAccountID() or "?", 
        User.new(aIndex):getName() or "?", 
        formattedHwid))

    if PendingRequests[aIndex] then
        Timer.Cancel(PendingRequests[aIndex].timerID)
        local adminIndex = PendingRequests[aIndex].adminIndex
        PendingRequests[aIndex] = nil
        if adminIndex and gObjIsConnectedGP(adminIndex) then
            BlockWithHWID(aIndex, adminIndex)
            return true
        end
    end
    if not gObjIsConnectedGP(aIndex) then return true end
    if IsHWIDBlocked(hwid) then
        KickAllWithSameHWID(hwid, aIndex)
        
        KickPlayer(aIndex, CONFIG.MSG_HWID_BLOCKED)
        LogAdd(string.format("[HWID Block] HWID bloqueado ao entrar (conta: %s, HWID: %s)", User.new(aIndex):getAccountID() or "?", hwid))
    end
    return true
end

local function OnPlayerLogout(aIndex)
    if PendingRequests[aIndex] then
        Timer.Cancel(PendingRequests[aIndex].timerID)
        PendingRequests[aIndex] = nil
    end
    PlayerHWID[aIndex] = nil
end

GameServerFunctions.PlayerLogout(OnPlayerLogout)

GameServerFunctions.EnterCharacter(OnPlayerEnter)
GameServerFunctions.GameServerProtocol(OnHWIDProtocol)