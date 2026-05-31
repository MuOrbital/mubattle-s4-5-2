LuckyWheel         = {}
LuckyWheel_Memory  = {}
LuckyWheel_Pending = {}
LuckyWheel_NpcList = {}

function LuckyWheel.DBGetCoinBalance(accountID, coinKey)
    local cfg = LuckyWheel_Coins[coinKey]
    if cfg == nil then return 0 end
    local sql = string.format(
        "SELECT %s FROM %s WHERE AccountID = '%s'",
        cfg.Column, cfg.Table, accountID
    )
    local ret = db:exec(sql)
    if ret == 0 then db:clear() return 0 end
    if db:fetch() == 100 then db:clear() return 0 end
    local val = tonumber(db:getStr(cfg.Column)) or 0
    db:clear()
    return val
end

function LuckyWheel.DBDebitCoin(accountID, coinKey, amount)
    local cfg = LuckyWheel_Coins[coinKey]
    if cfg == nil then return false end
    local sql = string.format(
        "UPDATE %s SET %s = %s - %d WHERE AccountID = '%s' AND %s >= %d",
        cfg.Table, cfg.Column, cfg.Column, amount, accountID, cfg.Column, amount
    )
    local ret = db:exec(sql)
    db:clear()
    return (ret ~= 0)
end

function LuckyWheel.ChargeWheel(accountID, wheelIndex)
    local costs = LuckyWheel_WheelCost[wheelIndex]
    if costs == nil or #costs == 0 then return true end
    for _, cost in ipairs(costs) do
        local balance = LuckyWheel.DBGetCoinBalance(accountID, cost.Coin)
        if balance < cost.Amount then return false end
    end
    for _, cost in ipairs(costs) do
        LuckyWheel.DBDebitCoin(accountID, cost.Coin, cost.Amount)
    end
    return true
end

function LuckyWheel.GetCostString(wheelIndex)
    local costs = LuckyWheel_WheelCost[wheelIndex]
    if costs == nil or #costs == 0 then return "Gratis" end
    local parts = {}
    for _, cost in ipairs(costs) do
        local cfg  = LuckyWheel_Coins[cost.Coin]
        local name = cfg and cfg.Display or cost.Coin
        parts[#parts + 1] = string.format("%d %s", cost.Amount, name)
    end
    local result = ""
    for i, p in ipairs(parts) do
        if i == 1 then result = p
        else result = result .. " + " .. p end
    end
    return result
end

function LuckyWheel.SendBalance(aIndex, accountID)
    local player = User.new(aIndex)
    local nome   = player:getName()
    local wc = LuckyWheel.DBGetCoinBalance(accountID, "WCoinC")
    local wp = LuckyWheel.DBGetCoinBalance(accountID, "WCoinP")
    local gp = LuckyWheel.DBGetCoinBalance(accountID, "GoblinPoint")
    local pc = LuckyWheel.DBGetCoinBalance(accountID, "PcPoint")
    CreatePacket("LW_Balance_" .. nome, LuckyWheel_Packet)
    SetDwordPacket("LW_Balance_" .. nome, wc)
    SetDwordPacket("LW_Balance_" .. nome, wp)
    SetDwordPacket("LW_Balance_" .. nome, gp)
    SetDwordPacket("LW_Balance_" .. nome, pc)
    SendPacket("LW_Balance_" .. nome, aIndex)
    ClearPacket("LW_Balance_" .. nome)
end

function LuckyWheel.SendMeta(aIndex)
    local player    = User.new(aIndex)
    local nome      = player:getName()
    local coinOrder = {"WCoinC", "WCoinP", "GoblinPoint", "PcPoint"}
    local wheelCount = 0
    for i = 1, 19 do
        if LuckyWheel_WheelNames[i] then wheelCount = wheelCount + 1 end
    end
    CreatePacket("LW_Meta_" .. nome, LuckyWheel_Packet)
    SetBytePacket("LW_Meta_" .. nome, wheelCount)
    for _, key in ipairs(coinOrder) do
        local cfg     = LuckyWheel_Coins[key]
        local display = cfg and cfg.Display or key
        SetBytePacket("LW_Meta_" .. nome, #display)
        for c = 1, #display do
            SetBytePacket("LW_Meta_" .. nome, string.byte(display, c))
        end
    end
    for i = 1, wheelCount do
        local wname = LuckyWheel_WheelNames[i] or ("R" .. i)
        SetBytePacket("LW_Meta_" .. nome, #wname)
        for c = 1, #wname do
            SetBytePacket("LW_Meta_" .. nome, string.byte(wname, c))
        end
    end
    SendPacket("LW_Meta_" .. nome, aIndex)
    ClearPacket("LW_Meta_" .. nome)
end

function LuckyWheel.SendWheelCost(aIndex)
    local player    = User.new(aIndex)
    local nome      = player:getName()
    local coinOrder = {"WCoinC", "WCoinP", "GoblinPoint", "PcPoint"}
    local coinIndexMap = {}
    for idx, key in ipairs(coinOrder) do coinIndexMap[key] = idx end
    local wheelCount = 0
    for i = 1, 19 do
        if LuckyWheel_WheelNames[i] then wheelCount = wheelCount + 1 end
    end
    for w = 1, wheelCount do
        local costs = LuckyWheel_WheelCost[w]
        CreatePacket("LW_Cost_" .. nome .. "_" .. w, LuckyWheel_Packet)
        if costs then
            SetBytePacket("LW_Cost_" .. nome .. "_" .. w, #costs)
            for _, cost in ipairs(costs) do
                local coinIdx = coinIndexMap[cost.Coin] or 0
                SetBytePacket("LW_Cost_" .. nome .. "_" .. w, coinIdx)
                SetDwordPacket("LW_Cost_" .. nome .. "_" .. w, cost.Amount)
            end
        else
            SetBytePacket("LW_Cost_" .. nome .. "_" .. w, 0)
        end
        SendPacket("LW_Cost_" .. nome .. "_" .. w, aIndex)
        ClearPacket("LW_Cost_" .. nome .. "_" .. w)
    end
end

function LuckyWheel.DBGetPending(accountID)
    local sql = string.format(
        "SELECT TOP 1 ID, WheelNumber, ItemName, ItemSection, ItemIndex, " ..
        "ItemLevel, ItemDur, ItemOpt, ItemSkill, ItemLuck, ItemExc, ItemAncient, ItemOption " ..
        "FROM LuckyWheel_Prizes WHERE AccountID = '%s' AND Status = 0 ORDER BY ID ASC",
        accountID
    )
    local ret = db:exec(sql)
    if ret == 0 then db:clear() return nil end
    if db:fetch() == 100 then db:clear() return nil end
    local row = {
        ID          = tonumber(db:getStr("ID")),
        WheelNumber = tonumber(db:getStr("WheelNumber")),
        ItemName    = db:getStr("ItemName"),
        ItemSection = tonumber(db:getStr("ItemSection")),
        ItemIndex   = tonumber(db:getStr("ItemIndex")),
        Level       = tonumber(db:getStr("ItemLevel")),
        Dur         = tonumber(db:getStr("ItemDur")) or 255,
        Opt         = tonumber(db:getStr("ItemOpt")),
        Skill       = tonumber(db:getStr("ItemSkill")),
        Luck        = tonumber(db:getStr("ItemLuck")),
        Exc         = tonumber(db:getStr("ItemExc")),
        Ancient     = tonumber(db:getStr("ItemAncient")),
        Option      = tonumber(db:getStr("ItemOption")),
    }
    db:clear()
    return row
end

function LuckyWheel.DBSavePrize(accountID, wheelNum, item)
    local section = math.floor(item.Item / 512)
    local index   = item.Item - (section * 512)
    local sql = string.format(
        "INSERT INTO LuckyWheel_Prizes " ..
        "(AccountID, WheelNumber, ItemName, ItemSection, ItemIndex, " ..
        "ItemLevel, ItemDur, ItemOpt, ItemSkill, ItemLuck, ItemExc, ItemAncient, ItemOption, Status) " ..
        "VALUES ('%s', %d, '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, 0)",
        accountID, wheelNum, item.Name, section, index,
        item.Level, item.Dur, item.Opt, item.Skill, item.Luck, item.Exc, item.Ancient, item.Option
    )
    db:exec(sql)
    db:clear()
end

function LuckyWheel.DBGetLastInserted(accountID)
    local sql = string.format(
        "SELECT TOP 1 ID, WheelNumber, ItemName, ItemSection, ItemIndex, " ..
        "ItemLevel, ItemDur, ItemOpt, ItemSkill, ItemLuck, ItemExc, ItemAncient, ItemOption " ..
        "FROM LuckyWheel_Prizes WHERE AccountID = '%s' AND Status = 0 ORDER BY ID DESC",
        accountID
    )
    local ret = db:exec(sql)
    if ret == 0 then db:clear() return nil end
    if db:fetch() == 100 then db:clear() return nil end
    local row = {
        ID          = tonumber(db:getStr("ID")),
        WheelNumber = tonumber(db:getStr("WheelNumber")),
        ItemName    = db:getStr("ItemName"),
        ItemSection = tonumber(db:getStr("ItemSection")),
        ItemIndex   = tonumber(db:getStr("ItemIndex")),
        Level       = tonumber(db:getStr("ItemLevel")),
        Dur         = tonumber(db:getStr("ItemDur")) or 255,
        Opt         = tonumber(db:getStr("ItemOpt")),
        Skill       = tonumber(db:getStr("ItemSkill")),
        Luck        = tonumber(db:getStr("ItemLuck")),
        Exc         = tonumber(db:getStr("ItemExc")),
        Ancient     = tonumber(db:getStr("ItemAncient")),
        Option      = tonumber(db:getStr("ItemOption")),
    }
    db:clear()
    return row
end

function LuckyWheel.DBClearPending(prizeID)
    local sql = string.format(
        "UPDATE LuckyWheel_Prizes SET Status = 1 WHERE ID = %d", prizeID
    )
    db:exec(sql)
    db:clear()
end

function LuckyWheel.SendPendingPacket(aIndex, row)
    local player    = User.new(aIndex)
    local nome      = player:getName()
    local wheelNum  = row.WheelNumber
    local slotFound = 1
    local wheels    = LuckyWheel_Wheels[wheelNum]
    if wheels then
        local itemCode = GET_ITEM(row.ItemSection, row.ItemIndex)
        for s = 1, 12 do
            if wheels[s] and wheels[s].Item == itemCode then
                slotFound = s
                break
            end
        end
    end
    CreatePacket("LW_Pending_" .. nome, LuckyWheel_Packet)
    SetBytePacket("LW_Pending_" .. nome, wheelNum)
    SetBytePacket("LW_Pending_" .. nome, slotFound)
    SendPacket("LW_Pending_" .. nome, aIndex)
    ClearPacket("LW_Pending_" .. nome)
end

-- =====================================================
-- NPC — SEMPRE ATIVO
-- =====================================================
function LuckyWheel.Init()
    if not LuckyWheel_Enabled then return end
    GameServerFunctions.NpcTalk(LuckyWheel.NpcTalk)
    GameServerFunctions.GameServerProtocol(LuckyWheel.Protocol)
    GameServerFunctions.MonsterReload(LuckyWheel.MonsterReload)
    Timer.TimeOut(2, LuckyWheel.CriarNpc)
end

function LuckyWheel.LuaFirstLoad()
    local count = 1
    for aIndex = 1, 12000 do
        local p = User.new(aIndex)
        if p:getConnected() == 3 then
            for i in ipairs(LuckyWheel_Config_Npc) do
                local npc = LuckyWheel_Config_Npc[i]
                if npc.class == p:getClass()
                and npc.map  == p:getMapNumber()
                and npc.x    == p:getX()
                and npc.y    == p:getY() then
                    LuckyWheel_NpcList[count] = aIndex
                    p:setType(3)
                    count = count + 1
                end
            end
        end
    end
    return (#LuckyWheel_NpcList == 0)
end

function LuckyWheel.MonsterReload()
    LuckyWheel_NpcList = {}
    local count = 1
    for i in ipairs(LuckyWheel_Config_Npc) do
        local npc   = LuckyWheel_Config_Npc[i]
        local index = AddMonster(npc.map)
        if index == -1 then goto continue end
        local mob = User.new(index)
        SetMapMonster(index, npc.map, npc.x, npc.y)
        mob:setDir(npc.dir)
        SetMonster(index, npc.class)
        mob:setType(3)
        LuckyWheel_NpcList[count] = index
        count = count + 1
        ::continue::
    end
end

function LuckyWheel.CriarNpc()
    if LuckyWheel.LuaFirstLoad() then
        LuckyWheel.MonsterReload()
    end
end

-- =====================================================
-- NPC TALK
-- =====================================================
function LuckyWheel.NpcTalk(Npc, Player)
    local isLuckyWheelNpc = false
    for i in ipairs(LuckyWheel_NpcList) do
        if LuckyWheel_NpcList[i] == Npc then
            isLuckyWheelNpc = true
            break
        end
    end
    if not isLuckyWheelNpc then return 0 end
    if not LuckyWheel_Enabled then
        SendMessage(LuckyWheel_Messages["Por"][2], Player, 1)
        return 0
    end
    local player    = User.new(Player)
    local accountID = player:getAccountID()
    local nome      = player:getName()
    LuckyWheel.SendMeta(Player)
    LuckyWheel.SendBalance(Player, accountID)
    LuckyWheel.SendWheelCost(Player)
    local dbPending = LuckyWheel.DBGetPending(accountID)
    if dbPending ~= nil then
        LuckyWheel_Memory[Player] = dbPending
        SendMessage(LuckyWheel_Messages["Por"][4], Player, 1)
        LuckyWheel.SendPendingPacket(Player, dbPending)
        return 1
    end
    CreatePacket("LW_Open_" .. nome, LuckyWheel_Packet)
    SendPacket("LW_Open_" .. nome, Player)
    ClearPacket("LW_Open_" .. nome)
    return 1
end

-- =====================================================
-- PROTOCOLO
-- =====================================================
function LuckyWheel.Protocol(aIndex, Packet, PacketName)
    if Packet ~= LuckyWheel_Packet then return false end
    local player    = User.new(aIndex)
    local nome      = player:getName()
    local accountID = player:getAccountID()

    -- ── GIRAR: verifica saldo, sorteia, NÃO cobra ─────────────────────────
    if PacketName == "LW_SpinW_" .. nome then
        local wheelIndex = GetBytePacket(PacketName, -1)
        ClearPacket(PacketName)

        local dbPending = LuckyWheel.DBGetPending(accountID)
        if dbPending ~= nil then
            LuckyWheel_Memory[aIndex] = dbPending
            SendMessage(LuckyWheel_Messages["Por"][4], aIndex, 1)
            LuckyWheel.SendPendingPacket(aIndex, dbPending)
            return true
        end

        if LuckyWheel_Pending[aIndex] ~= nil then
            SendMessage(LuckyWheel_Messages["Por"][4], aIndex, 1)
            return true
        end

        if wheelIndex < 1 or wheelIndex > 19 then wheelIndex = 1 end

        -- Verifica saldo SEM cobrar
        local costs = LuckyWheel_WheelCost[wheelIndex]
        if costs ~= nil and #costs > 0 then
            for _, cost in ipairs(costs) do
                local balance = LuckyWheel.DBGetCoinBalance(accountID, cost.Coin)
                if balance < cost.Amount then
                    SendMessage(LuckyWheel_Messages["Por"][7], aIndex, 1)
                    LuckyWheel.SendBalance(aIndex, accountID)
                    return true
                end
            end
        end

        -- Sorteia e guarda em memória temporária (sem cobrar ainda)
        local rand = math.random(1, 12)
        local item = LuckyWheel_Wheels[wheelIndex][rand]
        LuckyWheel_Pending[aIndex] = { item = item, wheelNum = wheelIndex, slot = rand }

        CreatePacket("LW_Result_" .. nome, LuckyWheel_Packet)
        SetBytePacket("LW_Result_" .. nome, rand)
        SendPacket("LW_Result_" .. nome, aIndex)
        ClearPacket("LW_Result_" .. nome)
        return true
    end

    -- ── COLETAR: cobra moeda AQUI e entrega o item ────────────────────────
    if PacketName == "LW_Get_" .. nome then
        ClearPacket(PacketName)

        local pending = LuckyWheel_Memory[aIndex]
        local fromDB  = false
        if pending == nil then
            pending = LuckyWheel.DBGetPending(accountID)
            if pending ~= nil then
                fromDB = true
                LuckyWheel_Memory[aIndex] = pending
            end
        else
            fromDB = true
        end

        local tempSpin = LuckyWheel_Pending[aIndex]
        if pending == nil and tempSpin == nil then return true end

        local itemCode, itemLevel, itemDur, itemSkill, itemLuck, itemOpt
        local itemExc, itemAncient, itemOption, itemName, itemWheelNum, prizeID

        if fromDB and pending ~= nil then
            -- Prêmio já no DB: entrega sem cobrar de novo
            itemCode     = GET_ITEM(pending.ItemSection, pending.ItemIndex)
            itemLevel    = pending.Level
            itemDur      = pending.Dur
            itemSkill    = pending.Skill
            itemLuck     = pending.Luck
            itemOpt      = pending.Opt
            itemExc      = pending.Exc
            itemAncient  = pending.Ancient
            itemOption   = pending.Option
            itemName     = pending.ItemName
            itemWheelNum = pending.WheelNumber
            prizeID      = pending.ID
        else
            -- Spin recém-feito: cobra moeda AGORA
            local charged = LuckyWheel.ChargeWheel(accountID, tempSpin.wheelNum)
            if not charged then
                -- Sem saldo: cancela e avisa; envia GetOK para resetar o cliente
                LuckyWheel_Pending[aIndex] = nil
                SendMessage(LuckyWheel_Messages["Por"][7], aIndex, 1)
                LuckyWheel.SendBalance(aIndex, accountID)
                CreatePacket("LW_GetOK_" .. nome, LuckyWheel_Packet)
                SendPacket("LW_GetOK_" .. nome, aIndex)
                ClearPacket("LW_GetOK_" .. nome)
                return true
            end

            LuckyWheel.SendBalance(aIndex, accountID)

            local it  = tempSpin.item
            LuckyWheel.DBSavePrize(accountID, tempSpin.wheelNum, it)
            local saved = LuckyWheel.DBGetLastInserted(accountID)
            local sec   = math.floor(it.Item / 512)
            local idx   = it.Item - (sec * 512)
            itemCode    = GET_ITEM(sec, idx)
            itemLevel   = it.Level
            itemDur     = it.Dur or 255
            itemSkill   = it.Skill
            itemLuck    = it.Luck
            itemOpt     = it.Opt
            itemExc     = it.Exc
            itemAncient = it.Ancient
            itemOption  = it.Option
            itemName    = it.Name
            itemWheelNum = tempSpin.wheelNum
            prizeID     = saved and saved.ID or 0
        end

        if InventoryCheckSpaceByItem(aIndex, itemCode) == 0 then
            if not fromDB and tempSpin ~= nil then
                local saved = LuckyWheel.DBGetLastInserted(accountID)
                if saved ~= nil then LuckyWheel_Memory[aIndex] = saved end
                LuckyWheel_Pending[aIndex] = nil
            end
            CreatePacket("LW_NoSpace_" .. nome, LuckyWheel_Packet)
            SendPacket("LW_NoSpace_" .. nome, aIndex)
            ClearPacket("LW_NoSpace_" .. nome)
            SendMessage(LuckyWheel_Messages["Por"][5], aIndex, 1)
            return true
        end

        if itemDur == nil then
            itemDur = (fromDB and pending ~= nil) and (pending.Dur or 255) or (tempSpin.item.Dur or 255)
        end

        CreateItemInventory(aIndex, itemCode, itemLevel, itemDur,
            itemSkill, itemLuck, itemOpt, itemExc,
            itemAncient, itemOption,
            0, 0, 0, 0, 0, 0, 0, 0)
        ItemListSend(aIndex)
        SendMessage(string.format(LuckyWheel_Messages["Por"][1], itemName), aIndex, 1)

        if prizeID and prizeID > 0 then
            LuckyWheel.DBClearPending(prizeID)
        end

        LuckyWheel_Memory[aIndex]  = nil
        LuckyWheel_Pending[aIndex] = nil

        CreatePacket("LW_GetOK_" .. nome, LuckyWheel_Packet)
        SendPacket("LW_GetOK_" .. nome, aIndex)
        ClearPacket("LW_GetOK_" .. nome)
        return true
    end

    -- ── CANCELAR: cliente fechou durante o giro ───────────────────────────
    -- Limpa o pending temporário e confirma com LW_GetOK para o cliente
    -- resetar completamente ao reabrir.
    if PacketName == "LW_Cancel_" .. nome then
        ClearPacket(PacketName)
        LuckyWheel_Pending[aIndex] = nil
        LuckyWheel_Memory[aIndex]  = nil
        CreatePacket("LW_GetOK_" .. nome, LuckyWheel_Packet)
        SendPacket("LW_GetOK_" .. nome, aIndex)
        ClearPacket("LW_GetOK_" .. nome)
        return true
    end

    return false
end

LuckyWheel.Init()
return LuckyWheel