LockStore = {}
function LockStore.IsBlocked(aIndex)
    local player = User.new(aIndex)
    local account = player:getAccountID()
    player = nil
    local query = "SELECT ISNULL(LockStore,0) FROM MEMB_INFO WHERE memb___id = '"..account.."'"
    local ret = db:exec(query)
    if ret == 0 then
        db:clear()
        return 0
    end
    db:fetch()
    local value = tonumber(db:getResult(0)) or 0
    db:clear()
    return value
end
AccountLock = {}
local function GetLang(aIndex)
    return "Por"
end
local function Msg(aIndex, id)
    local lang = GetLang(aIndex)
    SendMessage(string.format("[LockSystem] %s", AccountLock_Config.Messages[lang][id]), aIndex, 1)
end
local function Clean(str)
    if str == nil then return "" end
    str = tostring(str)
    str = str:gsub("%z", "")
    str = str:gsub("[%c]", "")
    str = str:gsub("^%s*(.-)%s*$", "%1")
    return str
end
function AccountLock.HasPassword(aIndex)
    local player = User.new(aIndex)
    local account = player:getAccountID()
    player = nil
    local query = "SELECT LockPwd FROM MEMB_INFO WHERE memb___id = '" .. account .. "'"
    local ret = db:exec(query)
    if ret == 0 then db:clear() return false end
    db:fetch()
    local pwd = Clean(db:getResult(0))
    db:clear()
    if pwd == "" or pwd == "NULL" or pwd == "null" or pwd == "0" then
        return false
    end
    return true
end
local function IsLocked(aIndex, lockType)
    local player = User.new(aIndex)
    local account = player:getAccountID()
    player = nil
    local query = string.format("SELECT LockPwd, ISNULL(%s, 0) FROM MEMB_INFO WHERE memb___id = '%s'", lockType, account)
    local ret = db:exec(query)
    if ret == 0 then db:clear() return false end
    db:fetch()
    local pwd = Clean(db:getResult(0))
    local lockValue = tonumber(db:getResult(1)) or 0
    db:clear()
    if pwd == "" then return false end
    return lockValue == 1
end
function AccountLock.RegisterPassword(aIndex, password)
    local player = User.new(aIndex)
    local account = player:getAccountID()
    player = nil
    password = Clean(password)
    local query = string.format("UPDATE MEMB_INFO SET LockPwd = '%s' WHERE memb___id = '%s'", password, account)
    db:exec(query)
    db:clear()
    Msg(aIndex, 1)
    AccountLock.OpenInterface(aIndex)
end
function AccountLock.CheckPassword(aIndex, password)
    local player = User.new(aIndex)
    local account = player:getAccountID()
    player = nil
    local query = "SELECT LockPwd FROM MEMB_INFO WHERE memb___id = '" .. account .. "'"
    local ret = db:exec(query)
    if ret == 0 then db:clear() return false end
    db:fetch()
    local savedPwd = Clean(db:getResult(0))
    db:clear()
    password = Clean(password)
    if savedPwd == "" then return false end
    return savedPwd == password
end
function AccountLock.SaveLocks(aIndex, trade, ware, move, pvp, store, itemlock, drop, lockpick, party, command)
    local player = User.new(aIndex)
    local account = player:getAccountID()
    player = nil
    local query = string.format(
        "UPDATE MEMB_INFO SET LockTrade=%d, LockWare=%d, LockMove=%d, LockPvP=%d, LockStore=%d, LockItem=%d, LockDrop=%d, LockPick=%d, LockParty=%d, LockCommand=%d WHERE memb___id = '%s'",
        trade or 0, ware or 0, move or 0, pvp or 0, store or 0, itemlock or 0, drop or 0, lockpick or 0, party or 0, command or 0, account
    )
    db:exec(query)
    db:clear()
end
function AccountLock.LoadAndSendSettings(aIndex)
    local player = User.new(aIndex)
    local account = player:getAccountID()
    player = nil
    local query = "SELECT ISNULL(LockTrade,0), ISNULL(LockWare,0), ISNULL(LockMove,0), ISNULL(LockPvP,0), ISNULL(LockStore,0), ISNULL(LockItem,0), ISNULL(LockDrop,0), ISNULL(LockPick,0), ISNULL(LockParty,0), ISNULL(LockCommand,0) FROM MEMB_INFO WHERE memb___id = '" .. account .. "'"
    local ret = db:exec(query)
    if ret == 0 then
        db:clear()
        return
    end
    db:fetch()
    local trade = tonumber(db:getResult(0)) or 0
    local ware = tonumber(db:getResult(1)) or 0
    local move = tonumber(db:getResult(2)) or 0
    local pvp = tonumber(db:getResult(3)) or 0
    local store = tonumber(db:getResult(4)) or 0
    local item = tonumber(db:getResult(5)) or 0
    local drop = tonumber(db:getResult(6)) or 0
    local pick = tonumber(db:getResult(7)) or 0
    local party = tonumber(db:getResult(8)) or 0
    local command = tonumber(db:getResult(9)) or 0
    db:clear()
    CreatePacket("AC_LOCK_LOAD_SETTINGS", AccountLock_Config.PacketLoad)
    SetBytePacket("AC_LOCK_LOAD_SETTINGS", trade)
    SetBytePacket("AC_LOCK_LOAD_SETTINGS", ware)
    SetBytePacket("AC_LOCK_LOAD_SETTINGS", move)
    SetBytePacket("AC_LOCK_LOAD_SETTINGS", pvp)
    SetBytePacket("AC_LOCK_LOAD_SETTINGS", store)
    SetBytePacket("AC_LOCK_LOAD_SETTINGS", item)
    SetBytePacket("AC_LOCK_LOAD_SETTINGS", drop)
    SetBytePacket("AC_LOCK_LOAD_SETTINGS", pick)
    SetBytePacket("AC_LOCK_LOAD_SETTINGS", party)
    SetBytePacket("AC_LOCK_LOAD_SETTINGS", command)
    SendPacket("AC_LOCK_LOAD_SETTINGS", aIndex)
    ClearPacket("AC_LOCK_LOAD_SETTINGS")
end
function AccountLock.OpenInterface(aIndex)
    local hasPwd = AccountLock.HasPassword(aIndex)
    CreatePacket("AC_LOCK_OPEN", AccountLock_Config.PacketOpen)
    SetBytePacket("AC_LOCK_OPEN", hasPwd and 1 or 0)
    SendPacket("AC_LOCK_OPEN", aIndex)
    ClearPacket("AC_LOCK_OPEN")
    Msg(aIndex, 10)
end
function AccountLock.Protocol(aIndex, Packet, PacketName)

    -- 🔥 IGNORA PACOTES QUE NÃO SÃO DO ACCOUNT LOCK
    if Packet ~= AccountLock_Config.PacketOpen
    and Packet ~= AccountLock_Config.PacketRegister
    and Packet ~= AccountLock_Config.PacketLogin
    and Packet ~= AccountLock_Config.PacketSave
    and Packet ~= AccountLock_Config.PacketLoad then
        return false
    end

    if PacketName == "AC_REGISTER_PWD" then
        local pwd = Clean(GetCharPacket("AC_REGISTER_PWD", -1))
        local pwd2 = Clean(GetCharPacket("AC_REGISTER_PWD", -1))
        if pwd ~= pwd2 then Msg(aIndex, 3) return true end
        if #pwd > 8 or #pwd == 0 then Msg(aIndex, 4) return true end
        AccountLock.RegisterPassword(aIndex, pwd)
        return true
    end

    if PacketName == "AC_LOGIN_PWD" then
        local pwd = Clean(GetCharPacket("AC_LOGIN_PWD", -1))
        if AccountLock.CheckPassword(aIndex, pwd) then
            AccountLock.LoadAndSendSettings(aIndex)
        else
            Msg(aIndex, 2)
        end
        return true
    end

    if PacketName == "AC_SAVE_LOCKS" then
        local trade = GetBytePacket("AC_SAVE_LOCKS", -1)
        local ware = GetBytePacket("AC_SAVE_LOCKS", -1)
        local move = GetBytePacket("AC_SAVE_LOCKS", -1)
        local pvp = GetBytePacket("AC_SAVE_LOCKS", -1)
        local store = GetBytePacket("AC_SAVE_LOCKS", -1)
        local itemlock = GetBytePacket("AC_SAVE_LOCKS", -1)
        local drop = GetBytePacket("AC_SAVE_LOCKS", -1)
        local lockpick = GetBytePacket("AC_SAVE_LOCKS", -1)
        local party = GetBytePacket("AC_SAVE_LOCKS", -1)
        local command = GetBytePacket("AC_SAVE_LOCKS", -1)

        AccountLock.SaveLocks(aIndex, trade, ware, move, pvp, store, itemlock, drop, lockpick, party, command)
        return true
    end

    return false
end
function AccountLock.PlayerDropItem(aIndex, x, y, slot)
    if IsLocked(aIndex, "LockDrop") then
        Msg(aIndex, 12)
        return 1
    end
    return 0
end
function AccountLock.PlayerPickItem(aIndex, index)
    if IsLocked(aIndex, "LockPick") then
        Msg(aIndex, 13)
        return 1
    end
    return 0
end
function AccountLock.PlayerMoveItem(aIndex, SourceSlot, TargetSlot, Type)
    if IsLocked(aIndex, "LockItem") then
        Msg(aIndex, 11)
        return 1
    end
    return 0
end
function AccountLock.PlayerOpenPersonalShop(aIndex)
    if LockStore.IsBlocked(aIndex) == 1 then
        SendMessage(string.format("[Sistema] Loja pessoal bloqueada."), aIndex, 1)
        return 1
    end
    return 0
end
function AccountLock.PlayerAttack(aIndex, TargetIndex)
    if IsLocked(aIndex, "LockPvP") then
        Msg(aIndex, 8)
        return 1
    end
    return 0
end
function AccountLock.PlayerCheckDamage(aIndex, TargetIndex, Damage, ShieldDamage, Effect)
    if IsLocked(aIndex, "LockPvP") then
        return 0
    end
    return Damage
end
function AccountLock.PlayerSendTrade(aIndex, TargetIndex)
    if IsLocked(aIndex, "LockTrade") then
        Msg(aIndex, 5)
        return 1
    end
    if IsLocked(TargetIndex, "LockTrade") then
        Msg(aIndex, 5)
        return 1
    end
    return 0
end
function AccountLock.PlayerTradeOk(aIndex, TargetIndex)
    if IsLocked(aIndex, "LockTrade") then
        Msg(aIndex, 5)
        return 1
    end
    if IsLocked(TargetIndex, "LockTrade") then
        Msg(aIndex, 5)
        return 1
    end
    return 0
end
function AccountLock.PlayerSendTradeX(aIndex, TargetIndex)
    if IsLocked(aIndex, "LockTrade") then
        Msg(aIndex, 5)
        return 1
    end
    if IsLocked(TargetIndex, "LockTrade") then
        Msg(aIndex, 5)
        return 1
    end
    return 0
end
function AccountLock.PlayerTradeXOk(aIndex, TargetIndex)
    if IsLocked(aIndex, "LockTrade") then
        Msg(aIndex, 5)
        return 1
    end
    if IsLocked(TargetIndex, "LockTrade") then
        Msg(aIndex, 5)
        return 1
    end
    return 0
end
function AccountLock.NpcTalk(Npc, Player)
    if IsLocked(Player, "LockWare") then Msg(Player, 6) return 1 end
    return 0
end

function AccountLock.BlockCommands(aIndex, command, text)
    if IsLocked(aIndex, "LockCommand") and command ~= AccountLock_Config.Command then
        Msg(aIndex, 15)
        return 1
    end
    if command == "/abrirbau" and IsLocked(aIndex, "LockWare") then
        Msg(aIndex, 6)
        return 1
    end
    if (command == "/store" or command == "/loja") and IsLocked(aIndex, "LockStore") then
        Msg(aIndex, 9)
        return 1
    end
    if (command == "/limparinv" or command == "/make" or command == "/makeset") and (IsLocked(aIndex, "LockItem") or IsLocked(aIndex, "LockDrop")) then
        Msg(aIndex, 11)
        return 1
    end
    return 0
end

--function AccountLock.PlayerMove(aIndex, Map, ax, ay, sx, sy)
--    if IsLocked(aIndex, "LockMove") then Msg(aIndex, 7) return 1 end
--    return 0
--end

function AccountLock.CharacterMove(aIndex, map, x, y)
    if IsLocked(aIndex, "LockMove") then Msg(aIndex, 7) return 1 end
    return 0
end
function AccountLock.Command(aIndex, Arguments)
    AccountLock.OpenInterface(aIndex)
    return 1
end
function AccountLock.BlockSendParty(aIndex, target)
    if IsLocked(aIndex, "LockParty") then
        Msg(aIndex, 14)
        return 1
    end
    return 0
end
function AccountLock.BlockRecParty(aIndex, sender)
    if IsLocked(aIndex, "LockParty") then
        Msg(aIndex, 14)
        return 1
    end
    return 0
end
function AccountLock.Init()
    GameServerFunctions.GameServerProtocol(AccountLock.Protocol)
    GameServerFunctions.NpcTalk(AccountLock.NpcTalk)
    GameServerFunctions.PlayerMove(AccountLock.PlayerMove)
    GameServerFunctions.CharacterMove(AccountLock.CharacterMove)
    GameServerFunctions.PlayerAttack(AccountLock.PlayerAttack)
    GameServerFunctions.PlayerCheckDamage(AccountLock.PlayerCheckDamage)
    GameServerFunctions.PlayerOpenPersonalShop(AccountLock.PlayerOpenPersonalShop)
    GameServerFunctions.PlayerSendTrade(AccountLock.PlayerSendTrade)
    GameServerFunctions.PlayerTradeOk(AccountLock.PlayerTradeOk)
    GameServerFunctions.PlayerSendTradeX(AccountLock.PlayerSendTradeX)
    GameServerFunctions.PlayerTradeXOk(AccountLock.PlayerTradeXOk)
    GameServerFunctions.PlayerMoveItem(AccountLock.PlayerMoveItem)
    GameServerFunctions.PlayerDropItem(AccountLock.PlayerDropItem)
    GameServerFunctions.PlayerPickItem(AccountLock.PlayerPickItem)
    GameServerFunctions.BlockSendParty(AccountLock.BlockSendParty)
    GameServerFunctions.BlockRecParty(AccountLock.BlockRecParty)
    Commands.CallCommand(AccountLock.BlockCommands)
    Commands.Register(AccountLock_Config.Command, AccountLock.Command)
end
AccountLock.Init()
return AccountLock