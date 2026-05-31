DailyReward        = {}
DailyReward_Memory = {}

local DR_PendingCollect = {}

local DR_HEADCODE_HWID    = 0xF1
local DR_SUBCODE_REQUEST  = 0x01
local DR_SUBCODE_RESPONSE = 0x02
local DR_HWID_TIMEOUT     = 6

function DailyReward.GetTodayMonthDay()
    local t = os.date("*t")
    return t.month, t.day
end

function DailyReward.GetTotalDaysInMonth(month)
    local cal = DailyReward_Calendar[month]
    if cal and cal.TotalDays then
        return cal.TotalDays
    end
    local defaults = {31,28,31,30,31,30,31,31,30,31,30,31}
    return defaults[month] or 30
end

function DailyReward.GetItemReward(month, day, aIndex)
    local cal = DailyReward_Calendar[month]
    if not cal then return nil end
    local entry = cal[day]
    if not entry then return nil end

    if entry[1] then
        local playerClass = -1
        if aIndex then
            local p = User.new(aIndex)
            playerClass = p:getClass()
        end
        for _, variant in ipairs(entry) do
            if variant.Class == playerClass then
                return variant
            end
        end
        for _, variant in ipairs(entry) do
            if variant.Class == -1 then
                return variant
            end
        end
        return nil
    end

    return entry
end

function DailyReward.GetBonusReward(month)
    local cal = DailyReward_Calendar[month]
    if not cal then return nil end
    return cal["BONUS"]
end

local function NormalizeHWID(hwid)
    if not hwid or hwid == "" then return "" end
    return string.upper(string.gsub(hwid, "[-%s]", ""))
end

local function DR_SendHWIDRequest(aIndex)
    local p    = User.new(aIndex)
    local nome = p:getName()
    local pkt  = "DR_HWIDReq_" .. nome
    CreatePacket(pkt, DR_HEADCODE_HWID)
    SetBytePacket(pkt, DR_SUBCODE_REQUEST)
    SendPacket(pkt, aIndex)
    ClearPacket(pkt)
end

function DailyReward.DBHasCollectedByHWID(hwid, month, day)
    if not hwid or hwid == "" then return false end
    local sql = string.format(
        "SELECT TOP 1 AccountID FROM DailyReward_Collected WHERE HWID='%s' AND Month=%d AND Day=%d",
        hwid, month, day
    )
    local ret = db:exec(sql)
    if ret == 0 then db:clear() return false end
    local found = (db:fetch() ~= 100)
    db:clear()
    return found
end

function DailyReward.DBHasCollected(accountID, month, day)
    local sql = string.format(
        "SELECT TOP 1 AccountID FROM DailyReward_Collected WHERE AccountID='%s' AND Month=%d AND Day=%d",
        accountID, month, day
    )
    local ret = db:exec(sql)
    if ret == 0 then db:clear() return false end
    local found = (db:fetch() ~= 100)
    db:clear()
    return found
end

function DailyReward.DBInsertCollected(accountID, hwid, month, day)
    local safeHwid = hwid or ""
    local sql = string.format(
        "INSERT INTO DailyReward_Collected (AccountID, Month, Day, HWID) VALUES ('%s', %d, %d, '%s')",
        accountID, month, day, safeHwid
    )
    db:exec(sql)
    db:clear()
end

function DailyReward.LoadMemory(aIndex)
    local p          = User.new(aIndex)
    local accountID  = p:getAccountID()
    local charName   = p:getName()
    local month, day = DailyReward.GetTodayMonthDay()
    DailyReward_Memory[aIndex] = {
        accountID = accountID,
        charName  = charName,
        month     = month,
        day       = day,
    }
    return DailyReward_Memory[aIndex]
end

function DailyReward.GetMemory(aIndex)
    return DailyReward_Memory[aIndex]
end

function DailyReward.CheckAllCollected(hwid, accountID, month)
    local bonus = DailyReward.GetBonusReward(month)
    if not bonus then return false end

    if DailyReward.DBHasCollected(accountID, month, 0) then return false end
    if hwid ~= "" and DailyReward.DBHasCollectedByHWID(hwid, month, 0) then return false end

    local totalDays = DailyReward.GetTotalDaysInMonth(month)
    for d = 1, totalDays do
        local collectedByAccount = DailyReward.DBHasCollected(accountID, month, d)
        local collectedByHWID    = hwid ~= "" and DailyReward.DBHasCollectedByHWID(hwid, month, d)
        if not collectedByAccount and not collectedByHWID then
            return false
        end
    end
    return true
end

function DailyReward.HasCollectedToday(hwid, accountID, month, day)
    if DailyReward.DBHasCollected(accountID, month, day) then
        return true
    end
    if hwid ~= "" then
        return DailyReward.DBHasCollectedByHWID(hwid, month, day)
    end
    return false
end

function DailyReward.SendDayState(aIndex, hwid)
    local mem = DailyReward.GetMemory(aIndex)
    if not mem then return end
    local p    = User.new(aIndex)
    local nome = p:getName()
    local month, day = mem.month, mem.day
    local safeHwid   = hwid or ""

    local collected  = DailyReward.HasCollectedToday(safeHwid, mem.accountID, month, day)
    local bonusReady = DailyReward.CheckAllCollected(safeHwid, mem.accountID, month) and 1 or 0

    local pkt = "DR_State_" .. nome
    CreatePacket(pkt, DailyReward_Packet)
    SetBytePacket(pkt, 0x01)
    SetBytePacket(pkt, month)
    SetBytePacket(pkt, day)
    SetBytePacket(pkt, collected and 1 or 0)
    SetBytePacket(pkt, bonusReady)
    SendPacket(pkt, aIndex)
    ClearPacket(pkt)
end

function DailyReward.SendCalendarPreview(aIndex, hwid)
    local mem = DailyReward.GetMemory(aIndex)
    if not mem then return end
    local p         = User.new(aIndex)
    local nome      = p:getName()
    local month     = mem.month
    local totalDays = DailyReward.GetTotalDaysInMonth(month)
    local safeHwid  = hwid or ""
    local accountID = mem.accountID

    local pkt = "DR_Calendar_" .. nome
    CreatePacket(pkt, DailyReward_Packet)
    SetBytePacket(pkt, 0x02)
    SetBytePacket(pkt, month)
    SetBytePacket(pkt, totalDays)

    for d = 1, totalDays do
        local reward    = DailyReward.GetItemReward(month, d, aIndex)
        local collected = DailyReward.HasCollectedToday(safeHwid, accountID, month, d)

        SetBytePacket(pkt, d)
        if reward then
            local section = math.floor(reward.Item / 512)
            local index_  = reward.Item - (section * 512)
            SetBytePacket(pkt, section)
            SetWordPacket(pkt, index_)
            SetBytePacket(pkt, reward.Level   or 0)
            SetBytePacket(pkt, reward.Dur     or 255)
            SetBytePacket(pkt, reward.Skill   or 0)
            SetBytePacket(pkt, reward.Luck    or 0)
            SetBytePacket(pkt, reward.Opt     or 0)
            SetBytePacket(pkt, reward.Exc     or 0)
            SetBytePacket(pkt, reward.Ancient or 0)
            SetBytePacket(pkt, reward.Option  or 0)
            SetBytePacket(pkt, reward.Count   or 1)
        else
            SetBytePacket(pkt, 255)
            SetWordPacket(pkt, 0)
            SetBytePacket(pkt, 0)
            SetBytePacket(pkt, 255)
            SetBytePacket(pkt, 0)
            SetBytePacket(pkt, 0)
            SetBytePacket(pkt, 0)
            SetBytePacket(pkt, 0)
            SetBytePacket(pkt, 0)
            SetBytePacket(pkt, 0)
            SetBytePacket(pkt, 1)
        end
        SetBytePacket(pkt, collected and 1 or 0)
    end

    local bonus    = DailyReward.GetBonusReward(month)
    local bonusCol = DailyReward.HasCollectedToday(safeHwid, accountID, month, 0)

    if bonus then
        local bs = math.floor(bonus.Item / 512)
        local bi = bonus.Item - (bs * 512)
        SetBytePacket(pkt, 1)
        SetBytePacket(pkt, bs)
        SetWordPacket(pkt, bi)
        SetBytePacket(pkt, bonus.Level   or 0)
        SetBytePacket(pkt, bonus.Dur     or 255)
        SetBytePacket(pkt, bonus.Skill   or 0)
        SetBytePacket(pkt, bonus.Luck    or 0)
        SetBytePacket(pkt, bonus.Opt     or 0)
        SetBytePacket(pkt, bonus.Exc     or 0)
        SetBytePacket(pkt, bonus.Ancient or 0)
        SetBytePacket(pkt, bonus.Option  or 0)
        SetBytePacket(pkt, bonus.Count   or 1)
        SetBytePacket(pkt, bonusCol and 1 or 0)
    else
        SetBytePacket(pkt, 0)
    end
    SendPacket(pkt, aIndex)
    ClearPacket(pkt)
end

function DailyReward.DeliverItem(aIndex, item, hwid, month, day, isBonus)
    local p         = User.new(aIndex)
    local accountID = p:getAccountID()
    local nome      = p:getName()

    local section  = math.floor(item.Item / 512)
    local index_   = item.Item - (section * 512)
    local itemCode = GET_ITEM(section, index_)
    local count    = item.Count or 1

    for i = 1, count do
        if InventoryCheckSpaceByItem(aIndex, itemCode) == 0 then
            local pkt = "DR_NoSpace_" .. nome
            CreatePacket(pkt, DailyReward_Packet)
            SetBytePacket(pkt, 0x05)
            SendPacket(pkt, aIndex)
            ClearPacket(pkt)
            return
        end

        CreateItemInventory(
            aIndex, itemCode,
            item.Level, item.Dur,
            item.Skill, item.Luck, item.Opt,
            item.Exc, item.Ancient, item.Option,
            0, 0, 0, 0, 0, 0, 0, 0
        )
    end

    ItemListSend(aIndex)

    if isBonus then
        DailyReward.DBInsertCollected(accountID, hwid, month, 0)
        SendMessage(aIndex, 3, string.format(DailyReward_Messages["Por"][8], item.Name))
    else
        DailyReward.DBInsertCollected(accountID, hwid, month, day)
        SendMessage(aIndex, 3, string.format(DailyReward_Messages["Por"][1], day, item.Name))
    end

    local bonusReady = 0
    if not isBonus then
        if DailyReward.CheckAllCollected(hwid, accountID, month) then
            bonusReady = 1
        end
    end

    local pkt = "DR_CollectOK_" .. nome
    CreatePacket(pkt, DailyReward_Packet)
    SetBytePacket(pkt, 0x04)
    SetBytePacket(pkt, isBonus and 1 or 0)
    SetBytePacket(pkt, bonusReady)
    SendPacket(pkt, aIndex)
    ClearPacket(pkt)
end

function DailyReward.OnHWIDResponse(aIndex, rawHwid)
    local pending = DR_PendingCollect[aIndex]
    if not pending then return end
 
    if pending.timerID then
        Timer.Cancel(pending.timerID)
    end
    DR_PendingCollect[aIndex] = nil
 
    local hwid      = NormalizeHWID(rawHwid)
    local mem       = DailyReward.GetMemory(aIndex)
    local accountID = mem and mem.accountID or User.new(aIndex):getAccountID()
    local nome      = User.new(aIndex):getName()
    local month     = pending.month
    local day       = pending.day
 
    if pending.type == "open" then
        DailyReward.SendCalendarPreview(aIndex, hwid)
        DailyReward.SendDayState(aIndex, hwid)
        return
    end
    
    local isBonus = pending.isBonus
 
    if isBonus then
        if DailyReward.DBHasCollectedByHWID(hwid, month, 0) then
            SendMessage(aIndex, 3, DailyReward_Messages["Por"][3])
            return
        end
        local bonus = DailyReward.GetBonusReward(month)
        if not bonus then return end
        DailyReward.DeliverItem(aIndex, bonus, hwid, month, 0, true)
    else
        if DailyReward.HasCollectedToday(hwid, accountID, month, day) then
            SendMessage(aIndex, 3, DailyReward_Messages["Por"][3])
            return
        end
        local item = pending.item
        if not item then
            item = DailyReward.GetItemReward(month, day, aIndex)
        end
        if not item then return end
        DailyReward.DeliverItem(aIndex, item, hwid, month, day, false)
    end
end

function DailyReward.Protocol(aIndex, Packet, PacketName)
    if Packet == DR_HEADCODE_HWID then
        if DR_PendingCollect[aIndex] then
            local subcode = GetBytePacket(PacketName, -1)
            if subcode == DR_SUBCODE_RESPONSE then
                local rawHwid = GetCharPacketLength(PacketName, -1, 36)
                ClearPacket(PacketName)
                DailyReward.OnHWIDResponse(aIndex, rawHwid)
                return true
            end
        end
        return false
    end
 
    if Packet ~= DailyReward_Packet then return false end
 
    local p         = User.new(aIndex)
    local nome      = p:getName()
    local accountID = p:getAccountID()
 
    if PacketName == "DR_RequestData_" .. nome then
        ClearPacket(PacketName)
 
        if DR_PendingCollect[aIndex] then return true end
 
        DailyReward.LoadMemory(aIndex)
 
        DailyReward.SendCalendarPreview(aIndex, "")
        DailyReward.SendDayState(aIndex, "")
 
        local month, day = DailyReward.GetTodayMonthDay()
 
        local timerID = Timer.TimeOut(DR_HWID_TIMEOUT, function()
            if DR_PendingCollect[aIndex] and DR_PendingCollect[aIndex].type == "open" then
                DR_PendingCollect[aIndex] = nil
            end
        end)
 
        DR_PendingCollect[aIndex] = {
            type    = "open",
            month   = month,
            day     = day,
            timerID = timerID,
        }
 
        DR_SendHWIDRequest(aIndex)
        return true
    end
 
    if PacketName == "DR_Collect_" .. nome then
        ClearPacket(PacketName)
 
        if DR_PendingCollect[aIndex] then
            local t = DR_PendingCollect[aIndex].type
            if t == "open" then
                if DR_PendingCollect[aIndex].timerID then
                    Timer.Cancel(DR_PendingCollect[aIndex].timerID)
                end
                DR_PendingCollect[aIndex] = nil
            else
                return true
            end
        end
 
        local month, day = DailyReward.GetTodayMonthDay()

        local allCollected = DailyReward.CheckAllCollected("", accountID, month)
        local isBonus      = allCollected
        local item         = nil
 
        if isBonus then
            item = DailyReward.GetBonusReward(month)
        else
            if DailyReward.DBHasCollected(accountID, month, day) then
                SendMessage(aIndex, 3, DailyReward_Messages["Por"][3])
                return true
            end
	    item = DailyReward.GetItemReward(month, day, aIndex)
            if not item then
                SendMessage(aIndex, 3, DailyReward_Messages["Por"][12])
                return true
            end
        end
 
        local timerID = Timer.TimeOut(DR_HWID_TIMEOUT, function()
            if DR_PendingCollect[aIndex] and DR_PendingCollect[aIndex].type == "collect" then
                DR_PendingCollect[aIndex] = nil
                SendMessage(aIndex, 3, DailyReward_Messages["Por"][12])
            end
        end)
 
        DR_PendingCollect[aIndex] = {
            type    = "collect",
            month   = month,
            day     = day,
            isBonus = isBonus,
            item    = item,
            timerID = timerID,
        }
 
        DR_SendHWIDRequest(aIndex)
        return true
    end
 
    return false
end

function DailyReward.OnEnterCharacter(aIndex)
    if not DailyReward_Enabled then return end
    local mem = DailyReward.LoadMemory(aIndex)
 
    DailyReward.SendCalendarPreview(aIndex, "")
    DailyReward.SendDayState(aIndex, "")
 
    local month, day = DailyReward.GetTodayMonthDay()
    local p          = User.new(aIndex)
    local nome       = p:getName()
    local accountID  = mem.accountID
 
    local timerID = Timer.TimeOut(DR_HWID_TIMEOUT, function()
        if DR_PendingCollect[aIndex] and DR_PendingCollect[aIndex].type == "open" then
            DR_PendingCollect[aIndex] = nil
        end
    end)
 
    DR_PendingCollect[aIndex] = {
        type    = "open",
        month   = month,
        day     = day,
        timerID = timerID,
    }
 
    DR_SendHWIDRequest(aIndex)
end

function DailyReward.OnPlayerLogout(aIndex, name, account)
    if DR_PendingCollect[aIndex] then
        if DR_PendingCollect[aIndex].timerID then
            Timer.Cancel(DR_PendingCollect[aIndex].timerID)
        end
        DR_PendingCollect[aIndex] = nil
    end
    DailyReward_Memory[aIndex] = nil
end

function DailyReward.Init()
    if not DailyReward_Enabled then return end
    GameServerFunctions.GameServerProtocol(DailyReward.Protocol)
    GameServerFunctions.EnterCharacter(DailyReward.OnEnterCharacter)
    GameServerFunctions.PlayerLogout(DailyReward.OnPlayerLogout)
end

DailyReward.Init()
return DailyReward