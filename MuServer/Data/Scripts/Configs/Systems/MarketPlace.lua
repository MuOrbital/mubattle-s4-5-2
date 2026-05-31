MarketPlace = {}
MarketPlace_PendingPrice = {}

function MarketPlace.Init()
    if not MarketPlace_Enabled then return end
    GameServerFunctions.GameServerProtocol(MarketPlace.Protocol)
    GameServerFunctions.EnterCharacter(MarketPlace.EnterCharacter)
    GameServerFunctions.PlayerLogout(MarketPlace.PlayerLogout)
    --GameServerFunctions.ChatProc(MarketPlace.ChatProc)
end

function MarketPlace.EnterCharacter(aIndex)
    --local player    = User.new(aIndex)
    --local accountID = player:getAccountID()
    --MarketPlace.SendCategories(aIndex)
    --MarketPlace.SendBalance(aIndex, accountID)
end

function MarketPlace.PlayerLogout(aIndex)
    MarketPlace_PendingPrice[aIndex] = nil
end

function MarketPlace.CoinCount()
    local n = 0
    for _ in pairs(MarketPlace_Coins) do n = n + 1 end
    return n
end

function MarketPlace.IsJewelIndex(coinIndex)
    return coinIndex > MarketPlace.CoinCount()
end

function MarketPlace.GetJewelCfg(coinIndex)
    local jewelIdx = coinIndex - MarketPlace.CoinCount()
    return MarketPlace_Jewels[jewelIdx], jewelIdx
end

function MarketPlace.GetDisplayName(coinIndex)
    if MarketPlace.IsJewelIndex(coinIndex) then
        local cfg = MarketPlace.GetJewelCfg(coinIndex)
        return cfg and cfg.Display or "?"
    else
        local coin = MarketPlace_Coins[coinIndex]
        return coin and coin.Display or "?"
    end
end

function MarketPlace.GetIndexByAccount(accountID)
    for i = 0, 3000 do
        if gObjIsConnectedGP and gObjIsConnectedGP(i) ~= 0 then
            local p = User.new(i)
            if p and p:getAccountID() == accountID then
                return i
            end
        end
    end
    return -1
end

function MarketPlace.DBGetCoinBalance(accountID, coinKey)
    local cfg = nil
    for _, c in pairs(MarketPlace_Coins) do
        if c.Key == coinKey then cfg = c; break end
    end
    if not cfg then return 0 end
    local sql = string.format("SELECT %s FROM %s WHERE AccountID = '%s'", cfg.Column, cfg.Table, accountID)
    local ret = db:exec(sql)
    if ret == 0 then db:clear(); return 0 end
    if db:fetch() == 100 then db:clear(); return 0 end
    local val = tonumber(db:getStr(cfg.Column)) or 0
    db:clear()
    return val
end

function MarketPlace.DBDebitCoin(accountID, coinKey, amount)
    local cfg = nil
    for _, c in pairs(MarketPlace_Coins) do
        if c.Key == coinKey then cfg = c; break end
    end
    if not cfg then return false end
    local sql = string.format(
        "UPDATE %s SET %s = %s - %d WHERE AccountID = '%s' AND %s >= %d",
        cfg.Table, cfg.Column, cfg.Column, amount, accountID, cfg.Column, amount)
    local ret = db:exec(sql)
    db:clear()
    
    local idx = MarketPlace.GetIndexByAccount(accountID)
    if idx and idx ~= -1 then
        local p = User.new(idx)
        local cur = p:getCacheInt(cfg.Key) or 0
        p:setCacheInt(cfg.Key, cur - amount)
    end
    
    return (ret ~= 0)
end

function MarketPlace.DBCreditCoin(accountID, coinKey, amount)
    local cfg = nil
    for _, c in pairs(MarketPlace_Coins) do
        if c.Key == coinKey then cfg = c; break end
    end
    if not cfg then return false end
    local sql = string.format(
        "UPDATE %s SET %s = %s + %d WHERE AccountID = '%s'",
        cfg.Table, cfg.Column, cfg.Column, amount, accountID)
    local ret = db:exec(sql)
    db:clear()
    
    local idx = MarketPlace.GetIndexByAccount(accountID)
    if idx and idx ~= -1 then
        local p = User.new(idx)
        local cur = p:getCacheInt(cfg.Key) or 0
        p:setCacheInt(cfg.Key, cur + amount)
    end
    
    return (ret ~= 0)
end

function MarketPlace.DBGetJewelBalance(accountID, jewelCfg)
    if not jewelCfg or not jewelCfg.Column or not MarketPlace_JewelBank_Table or not MarketPlace_JewelBank_Where then
        return 0
    end
    local sql = string.format(
        "SELECT %s FROM %s WHERE %s = '%s'",
        jewelCfg.Column, MarketPlace_JewelBank_Table, MarketPlace_JewelBank_Where, accountID)
    local ret = db:exec(sql)
    if ret == 0 then db:clear(); return 0 end
    if db:fetch() == 100 then db:clear(); return 0 end
    local val = tonumber(db:getStr(jewelCfg.Column)) or 0
    db:clear()
    return val
end

function MarketPlace.DBDebitJewel(aIndex, accountID, jewelCfg, amount)
    local sql = string.format(
        "UPDATE %s SET %s = %s - %d WHERE %s = '%s' AND %s >= %d",
        MarketPlace_JewelBank_Table,
        jewelCfg.Column, jewelCfg.Column, amount,
        MarketPlace_JewelBank_Where, accountID,
        jewelCfg.Column, amount)
    local ret = db:exec(sql)
    db:clear()

    if aIndex and aIndex ~= -1 then
        local p = User.new(aIndex)
        local cur = p:getCacheInt(jewelCfg.CacheName) or 0
        p:setCacheInt(jewelCfg.CacheName, cur - amount)
        MarketPlace.SendSingleJewelBalance(aIndex, jewelCfg, cur - amount)
    end

    return (ret ~= 0)
end

function MarketPlace.DBCreditJewel(aIndex, accountID, jewelCfg, amount)
    local sql = string.format(
        "UPDATE %s SET %s = %s + %d WHERE %s = '%s'",
        MarketPlace_JewelBank_Table,
        jewelCfg.Column, jewelCfg.Column, amount,
        MarketPlace_JewelBank_Where, accountID)
    local ret = db:exec(sql)
    db:clear()

    if aIndex and aIndex ~= -1 then
        local p = User.new(aIndex)
        local cur = p:getCacheInt(jewelCfg.CacheName) or 0
        p:setCacheInt(jewelCfg.CacheName, cur + amount)
        MarketPlace.SendSingleJewelBalance(aIndex, jewelCfg, cur + amount)
    end

    return (ret ~= 0)
end

function MarketPlace.SendSingleJewelBalance(aIndex, jewelCfg, newBalance)
    local player = User.new(aIndex)
    local name = player:getName()
    
    local jewelIndex = nil
    for i, jewel in pairs(MarketPlace_Jewels) do
        if jewel.Column == jewelCfg.Column then
            jewelIndex = i
            break
        end
    end
    
    if jewelIndex then
        CreatePacket("MP_JewelBalance_" .. name, MarketPlace_Packet)
        SetBytePacket("MP_JewelBalance_" .. name, jewelIndex)
        SetDwordPacket("MP_JewelBalance_" .. name, newBalance)
        SendPacket("MP_JewelBalance_" .. name, aIndex)
        ClearPacket("MP_JewelBalance_" .. name)
    end
end

function MarketPlace.GetBalance(accountID, coinIndex)
    if MarketPlace.IsJewelIndex(coinIndex) then
        local cfg = MarketPlace.GetJewelCfg(coinIndex)
        if not cfg then return 0 end
        return MarketPlace.DBGetJewelBalance(accountID, cfg)
    else
        local coin = MarketPlace_Coins[coinIndex]
        if not coin then return 0 end
        return MarketPlace.DBGetCoinBalance(accountID, coin.Key)
    end
end

function MarketPlace.Debit(aIndex, accountID, coinIndex, amount)
    if MarketPlace.IsJewelIndex(coinIndex) then
        local cfg = MarketPlace.GetJewelCfg(coinIndex)
        if not cfg then return false end
        return MarketPlace.DBDebitJewel(aIndex, accountID, cfg, amount)
    else
        local coin = MarketPlace_Coins[coinIndex]
        if not coin then return false end
        return MarketPlace.DBDebitCoin(accountID, coin.Key, amount)
    end
end

function MarketPlace.Credit(aIndex, accountID, coinIndex, amount)
    if MarketPlace.IsJewelIndex(coinIndex) then
        local cfg = MarketPlace.GetJewelCfg(coinIndex)
        if not cfg then return false end
        return MarketPlace.DBCreditJewel(aIndex, accountID, cfg, amount)
    else
        local coin = MarketPlace_Coins[coinIndex]
        if not coin then return false end
        return MarketPlace.DBCreditCoin(accountID, coin.Key, amount)
    end
end

-- ============================================================
-- Retorna a quantidade de itens ATIVOS que uma conta possui
-- anunciados no Marketplace. A verificação usa o AccountID,
-- ou seja, vale para todos os personagens da mesma conta.
-- ============================================================
function MarketPlace.DBGetActiveListingCountByAccount(accountID)
    local sql = string.format(
        "SELECT COUNT(*) AS Total FROM MarketPlace_Listings WHERE SellerAccount = '%s' AND Status = %d",
        accountID, MarketPlace_Status.ACTIVE)
    local ret = db:exec(sql)
    if ret == 0 then db:clear(); return 0 end
    if db:fetch() == 100 then db:clear(); return 0 end
    local count = tonumber(db:getStr("Total")) or 0
    db:clear()
    return count
end

function MarketPlace.DBInsertListing(accountID, name, item, price, coinIndex)
    local section      = item.section
    local index2       = item.index2
    local itemIndexFull = item.itemIndexFull or (section * 512 + index2)

    local sql = string.format([[
        INSERT INTO MarketPlace_Listings
        (SellerAccount, SellerName, ItemSection, ItemIndex2, ItemIndexFull, ItemName,
         ItemLevel, ItemDur, ItemSkill, ItemLuck, ItemOpt3, ItemExc, ItemAncient,
         ItemJoH, ItemOp380, ItemSockBonus, ItemSock1, ItemSock2, ItemSock3, ItemSock4, ItemSock5,
         ItemSerial, CoinIndex, Price, Status, PostedAt)
        VALUES ('%s', '%s', %d, %d, %d, '%s',
                %d, %d, %d, %d, %d, %d, %d,
                %d, %d, %d, %d, %d, %d, %d, %d,
                %d, %d, %d, %d, GETDATE())
    ]],
        accountID, name,
        section, index2, itemIndexFull, item.name,
        item.level, item.dur, item.skill, item.luck, item.opt, item.exc, item.ancient,
        item.joh or 0, item.op380 or 0, item.sockbonus or 0,
        item.sock1 or 0, item.sock2 or 0, item.sock3 or 0, item.sock4 or 0, item.sock5 or 0,
        item.serial, coinIndex, price, MarketPlace_Status.ACTIVE)

    db:exec(sql)
    db:clear()
end

function MarketPlace.DBUpdateListingSold(id, buyerAccount)
    local sql = string.format([[
        UPDATE MarketPlace_Listings
        SET Status = %d, BuyerAccount = '%s', SoldAt = GETDATE()
        WHERE ID = %d
    ]], MarketPlace_Status.SOLD, buyerAccount, id)
    db:exec(sql)
    db:clear()
end

function MarketPlace.DBUpdateListingCancelled(id)
    local sql = string.format(
        "UPDATE MarketPlace_Listings SET Status = %d WHERE ID = %d",
        MarketPlace_Status.CANCELLED, id)
    db:exec(sql)
    db:clear()
end

function MarketPlace.DBGetListingsByCategory(category)
    local listings = {}
    local sql = string.format([[
        SELECT ID, SellerName, ItemName, ItemLevel, ItemLuck, ItemOpt3, ItemExc,
               Price, CoinIndex, ItemSection, ItemIndex2, ItemIndexFull,
               ItemDur, ItemSkill, ItemAncient, ItemJoH, ItemOp380,
               ItemSockBonus, ItemSock1, ItemSock2, ItemSock3, ItemSock4, ItemSock5, ItemSerial
        FROM MarketPlace_Listings
        WHERE ItemSection = %d AND Status = %d
        ORDER BY PostedAt DESC
    ]], category, MarketPlace_Status.ACTIVE)

    local ret = db:exec(sql)
    if ret == 0 then db:clear(); return listings end

    while db:fetch() ~= 100 do
        local listing = {
            ID            = tonumber(db:getStr("ID")),
            SellerName    = db:getStr("SellerName"),
            ItemName      = db:getStr("ItemName"),
            ItemLevel     = tonumber(db:getStr("ItemLevel")),
            ItemLuck      = tonumber(db:getStr("ItemLuck")),
            ItemOpt3      = tonumber(db:getStr("ItemOpt3")),
            ItemExc       = tonumber(db:getStr("ItemExc")),
            Price         = tonumber(db:getStr("Price")),
            CoinIndex     = tonumber(db:getStr("CoinIndex")),
            ItemSection   = tonumber(db:getStr("ItemSection")),
            ItemIndex2    = tonumber(db:getStr("ItemIndex2")),
            ItemIndexFull = tonumber(db:getStr("ItemIndexFull")),
            ItemDur       = tonumber(db:getStr("ItemDur")) or 255,
            ItemSkill     = tonumber(db:getStr("ItemSkill")),
            ItemAncient   = tonumber(db:getStr("ItemAncient")),
            ItemJoH       = tonumber(db:getStr("ItemJoH")),
            ItemOp380     = tonumber(db:getStr("ItemOp380")),
            ItemSockBonus = tonumber(db:getStr("ItemSockBonus")),
            ItemSock1     = tonumber(db:getStr("ItemSock1")),
            ItemSock2     = tonumber(db:getStr("ItemSock2")),
            ItemSock3     = tonumber(db:getStr("ItemSock3")),
            ItemSock4     = tonumber(db:getStr("ItemSock4")),
            ItemSock5     = tonumber(db:getStr("ItemSock5")),
            ItemSerial    = tonumber(db:getStr("ItemSerial")),
        }
        table.insert(listings, listing)
    end
    db:clear()
    return listings
end

function MarketPlace.DBGetListingById(id)
    local sql = string.format([[
        SELECT ID, SellerAccount, SellerName, ItemName, ItemLevel, ItemLuck, ItemOpt3, ItemExc,
               Price, CoinIndex, ItemSection, ItemIndex2, ItemIndexFull,
               ItemDur, ItemSkill, ItemAncient, ItemJoH, ItemOp380,
               ItemSockBonus, ItemSock1, ItemSock2, ItemSock3, ItemSock4, ItemSock5, ItemSerial
        FROM MarketPlace_Listings
        WHERE ID = %d AND Status = %d
    ]], id, MarketPlace_Status.ACTIVE)

    local ret = db:exec(sql)
    if ret == 0 then db:clear(); return nil end
    if db:fetch() == 100 then db:clear(); return nil end

    local listing = {
        ID            = tonumber(db:getStr("ID")),
        SellerAccount = db:getStr("SellerAccount"),
        SellerName    = db:getStr("SellerName"),
        ItemName      = db:getStr("ItemName"),
        ItemLevel     = tonumber(db:getStr("ItemLevel")),
        ItemLuck      = tonumber(db:getStr("ItemLuck")),
        ItemOpt3      = tonumber(db:getStr("ItemOpt3")),
        ItemExc       = tonumber(db:getStr("ItemExc")),
        Price         = tonumber(db:getStr("Price")),
        CoinIndex     = tonumber(db:getStr("CoinIndex")),
        ItemSection   = tonumber(db:getStr("ItemSection")),
        ItemIndex2    = tonumber(db:getStr("ItemIndex2")),
        ItemIndexFull = tonumber(db:getStr("ItemIndexFull")),
        ItemDur       = tonumber(db:getStr("ItemDur")) or 255,
        ItemSkill     = tonumber(db:getStr("ItemSkill")),
        ItemAncient   = tonumber(db:getStr("ItemAncient")),
        ItemJoH       = tonumber(db:getStr("ItemJoH")),
        ItemOp380     = tonumber(db:getStr("ItemOp380")),
        ItemSockBonus = tonumber(db:getStr("ItemSockBonus")),
        ItemSock1     = tonumber(db:getStr("ItemSock1")),
        ItemSock2     = tonumber(db:getStr("ItemSock2")),
        ItemSock3     = tonumber(db:getStr("ItemSock3")),
        ItemSock4     = tonumber(db:getStr("ItemSock4")),
        ItemSock5     = tonumber(db:getStr("ItemSock5")),
        ItemSerial    = tonumber(db:getStr("ItemSerial")),
    }
    db:clear()
    return listing
end

function MarketPlace.GetItemFromInventory(aIndex, slot)
    local inv = Inventory.new(aIndex)
    if inv:isItem(slot) == 0 then return nil end
    
    local item = {
        section = inv:getSection(slot),
        index2 = inv:getIndex2(slot),
        itemIndexFull = inv:getIndex(slot),
        name = inv:getName(slot),
        level = inv:getLevel(slot),
        dur = inv:getDurability(slot),
        skill = inv:getItemTable(slot, 2),
        luck = inv:getItemTable(slot, 3),
        opt = inv:getItemTable(slot, 4),
        exc = inv:getItemTable(slot, 1),
        ancient = inv:getItemTable(slot, 5),
        joh = inv:getItemTable(slot, 6),
        op380 = inv:getItemTable(slot, 7),
        sockbonus = inv:getItemTable(slot, 8),
        sock1 = inv:getItemTable(slot, 9),
        sock2 = inv:getItemTable(slot, 10),
        sock3 = inv:getItemTable(slot, 11),
        sock4 = inv:getItemTable(slot, 12),
        sock5 = inv:getItemTable(slot, 13),
        serial = inv:getSerial(slot),
    }
    return item
end

function MarketPlace.CreateItemInInventory(aIndex, listing)
    local itemCode = GET_ITEM(listing.ItemSection, listing.ItemIndex2)

    CreateItemInventory(
        aIndex, itemCode, listing.ItemLevel, listing.ItemDur,
        listing.ItemSkill, listing.ItemLuck, listing.ItemOpt3, listing.ItemExc,
        listing.ItemAncient, listing.ItemJoH, listing.ItemSockBonus,
        listing.ItemSock1 or 0, listing.ItemSock2 or 0,
        listing.ItemSock3 or 0, listing.ItemSock4 or 0, listing.ItemSock5 or 0,
        listing.ItemOp380 or 0, 0)

    ItemListSend(aIndex)
end

function MarketPlace.DeleteItemBySerial(aIndex, serial)
    local inv = Inventory.new(aIndex)
    for slot = 0, 75 do
        if inv:isItem(slot) == 1 then
            if inv:getSerial(slot) == serial then
                InventoryDeleteItem(aIndex, slot)
                SendInventoryDeleteItem(aIndex, slot)
            end
        end
    end
end

function MarketPlace.SendCategories(aIndex)
    local player = User.new(aIndex)
    local name   = player:getName()

    CreatePacket("MP_Categories_" .. name, MarketPlace_Packet)

    local catCount = 0
    for _ in pairs(MarketPlace_Categories) do catCount = catCount + 1 end
    SetBytePacket("MP_Categories_" .. name, catCount)

    for i = 0, 13 do
        if MarketPlace_Categories[i] then
            local catName = MarketPlace_Categories[i].Name
            SetBytePacket("MP_Categories_" .. name, #catName)
            for c = 1, #catName do
                SetBytePacket("MP_Categories_" .. name, string.byte(catName, c))
            end
        end
    end

    local coinCount = MarketPlace.CoinCount()
    SetBytePacket("MP_Categories_" .. name, coinCount)
    for i = 1, coinCount do
        local coin     = MarketPlace_Coins[i]
        local enabled  = (coin and coin.Enable == 1)
        local coinName = (coin and coin.Display) or ""
        SetBytePacket("MP_Categories_" .. name, #coinName)
        for c = 1, #coinName do
            SetBytePacket("MP_Categories_" .. name, string.byte(coinName, c))
        end
        SetBytePacket("MP_Categories_" .. name, enabled and 1 or 0)
    end

    local jewelCount = #MarketPlace_Jewels
    SetBytePacket("MP_Categories_" .. name, jewelCount)
    for i = 1, jewelCount do
        local jewel     = MarketPlace_Jewels[i]
        local enabled   = (jewel and jewel.Enable == 1)
        local jewelName = (jewel and jewel.Display) or ""
        SetBytePacket("MP_Categories_" .. name, #jewelName)
        for c = 1, #jewelName do
            SetBytePacket("MP_Categories_" .. name, string.byte(jewelName, c))
        end
        SetBytePacket("MP_Categories_" .. name, enabled and 1 or 0)
    end

    SendPacket("MP_Categories_" .. name, aIndex)
    ClearPacket("MP_Categories_" .. name)
end

function MarketPlace.SendBalance(aIndex, accountID)
    local player = User.new(aIndex)
    local name   = player:getName()

    CreatePacket("MP_Balance_" .. name, MarketPlace_Packet)

    local coinCount = MarketPlace.CoinCount()
    for i = 1, coinCount do
        local coin    = MarketPlace_Coins[i]
        local balance = 0
        if coin and coin.Key then
            balance = MarketPlace.DBGetCoinBalance(accountID, coin.Key)
        end
        SetDwordPacket("MP_Balance_" .. name, balance)
    end

    for i = 1, #MarketPlace_Jewels do
        local jewel   = MarketPlace_Jewels[i]
        local balance = 0
        if jewel and jewel.Column and MarketPlace_JewelBank_Table and MarketPlace_JewelBank_Where then
            balance = MarketPlace.DBGetJewelBalance(accountID, jewel)
        end
        SetDwordPacket("MP_Balance_" .. name, balance)
    end

    SendPacket("MP_Balance_" .. name, aIndex)
    ClearPacket("MP_Balance_" .. name)
end

function MarketPlace.SendListings(aIndex, category)
    local player    = User.new(aIndex)
    local name      = player:getName()
    local listings  = MarketPlace.DBGetListingsByCategory(category)

    CreatePacket("MP_Listings_" .. name, MarketPlace_Packet)
    SetBytePacket("MP_Listings_" .. name, category)
    SetWordPacket("MP_Listings_" .. name, #listings)

    for _, listing in ipairs(listings) do
        SetDwordPacket("MP_Listings_" .. name, listing.ID)
        SetCharPacketLength("MP_Listings_" .. name, listing.SellerName, 10)
        SetCharPacketLength("MP_Listings_" .. name, listing.ItemName, 50)
        SetBytePacket("MP_Listings_" .. name, listing.ItemLevel  or 0)
        SetBytePacket("MP_Listings_" .. name, listing.ItemLuck   or 0)
        SetBytePacket("MP_Listings_" .. name, listing.ItemOpt3   or 0)
        SetWordPacket ("MP_Listings_" .. name, listing.ItemExc   or 0)
        SetDwordPacket("MP_Listings_" .. name, listing.Price     or 0)
        SetBytePacket ("MP_Listings_" .. name, listing.CoinIndex or 1)
        SetBytePacket ("MP_Listings_" .. name, listing.ItemSection or 0)
        SetWordPacket ("MP_Listings_" .. name, listing.ItemIndex2  or 0)
        SetDwordPacket("MP_Listings_" .. name, listing.ItemIndexFull or 0)
        SetBytePacket ("MP_Listings_" .. name, listing.ItemSkill   or 0)
        SetBytePacket ("MP_Listings_" .. name, listing.ItemAncient or 0)
        SetBytePacket ("MP_Listings_" .. name, listing.ItemJoH    or 0)
        SetBytePacket ("MP_Listings_" .. name, listing.ItemOp380  or 0)
        SetBytePacket ("MP_Listings_" .. name, listing.ItemSock1  or 0)
        SetBytePacket ("MP_Listings_" .. name, listing.ItemSock2  or 0)
        SetBytePacket ("MP_Listings_" .. name, listing.ItemSock3  or 0)
        SetBytePacket ("MP_Listings_" .. name, listing.ItemSock4  or 0)
        SetBytePacket ("MP_Listings_" .. name, listing.ItemSock5  or 0)
        SetBytePacket ("MP_Listings_" .. name, listing.ItemDur    or 255)
        local isOwner = (listing.SellerName == name)
        SetBytePacket("MP_Listings_" .. name, isOwner and 1 or 0)
    end

    SendPacket("MP_Listings_" .. name, aIndex)
    ClearPacket("MP_Listings_" .. name)
end

function MarketPlace.PostItem(aIndex, serial, price, coinIndex)
    if not MarketPlace_Enabled then
        SendMessage(MarketPlace_Messages["Por"][2], aIndex, 1)
        return false
    end

    local player    = User.new(aIndex)
    local accountID = player:getAccountID()
    local name      = player:getName()
    local inv       = Inventory.new(aIndex)
    local vip       = player:getVip()

    if vip < MarketPlace_VIP.MinAccountLevel then
        SendMessage(string.format("Somente VIPs podem postar Itens no Marketplace! "), aIndex, 1)
        return false
    end

    if MarketPlace.IsJewelIndex(coinIndex) then
        local cfg = MarketPlace.GetJewelCfg(coinIndex)
        if not cfg or cfg.Enable ~= 1 then
            SendMessage(MarketPlace_Messages["Por"][3], aIndex, 1)
            return false
        end
    else
        local coin = MarketPlace_Coins[coinIndex]
        if not coin or coin.Enable ~= 1 then
            SendMessage(MarketPlace_Messages["Por"][3], aIndex, 1)
            return false
        end
    end

    local foundSlot = -1
    for slot = 0, 75 do
        if inv:isItem(slot) == 1 and inv:getSerial(slot) == serial then
            foundSlot = slot
            break
        end
    end

    if foundSlot == -1 then
        SendMessage(MarketPlace_Messages["Por"][3], aIndex, 1)
        return false
    end

    local item = {
        section      = inv:getSection(foundSlot),
        index2       = inv:getIndex2(foundSlot),
        itemIndexFull = inv:getIndex(foundSlot),
        name         = inv:getName(foundSlot),
        level        = inv:getLevel(foundSlot),
        dur          = inv:getDurability(foundSlot),
        skill        = inv:getItemTable(foundSlot, 2),
        luck         = inv:getItemTable(foundSlot, 3),
        opt          = inv:getItemTable(foundSlot, 4),
        exc          = inv:getItemTable(foundSlot, 1),
        ancient      = inv:getItemTable(foundSlot, 5),
        joh          = inv:getItemTable(foundSlot, 6),
        op380        = inv:getItemTable(foundSlot, 7),
        sockbonus    = inv:getItemTable(foundSlot, 8),
        sock1        = inv:getItemTable(foundSlot, 9),
        sock2        = inv:getItemTable(foundSlot, 10),
        sock3        = inv:getItemTable(foundSlot, 11),
        sock4        = inv:getItemTable(foundSlot, 12),
        sock5        = inv:getItemTable(foundSlot, 13),
        serial       = serial,
    }

    if not MarketPlace_Categories[item.section] then
        SendMessage(MarketPlace_Messages["Por"][3], aIndex, 1)
        return false
    end

    if price < 1 then
        SendMessage(MarketPlace_Messages["Por"][16], aIndex, 1)
        return false
    end

    -- ============================================================
    -- Verificação de limite de anúncios ativos por conta.
    -- Usa o AccountID para cobrir todos os personagens da conta.
    -- Jogadores que já ultrapassaram o limite antes da config
    -- não são removidos; apenas novos anúncios são bloqueados.
    -- ============================================================
    if MarketPlace_MaxListings and MarketPlace_MaxListings > 0 then
        local activeCount = MarketPlace.DBGetActiveListingCountByAccount(accountID)
        if activeCount >= MarketPlace_MaxListings then
            SendMessage(string.format(MarketPlace_Messages["Por"][18], activeCount, MarketPlace_MaxListings), aIndex, 1)
            return false
        end
    end

    for slot = 0, 75 do
        if inv:isItem(slot) == 1 and inv:getSerial(slot) == serial then
            InventoryDeleteItem(aIndex, slot)
            SendInventoryDeleteItem(aIndex, slot)
            break
        end
    end

    MarketPlace.DBInsertListing(accountID, name, item, price, coinIndex)

    local display = MarketPlace.GetDisplayName(coinIndex)
    SendMessage(string.format(MarketPlace_Messages["Por"][1], price, display), aIndex, 1)
    SendMessageGlobal(string.format("[MarketPlace] [%s] publicou [%s] por [%d %s] para venda!", name, item.name, price, display), 1)
    LogAddC(3, string.format("[MarketPlace] [%s] publicou [%s] por [%d %s] para venda!", name, item.name, price, display))
    return true
end

function MarketPlace.BuyItem(aIndex, listingID)
    if not MarketPlace_Enabled then
        SendMessage(MarketPlace_Messages["Por"][2], aIndex, 1)
        return false
    end

    local player    = User.new(aIndex)
    local accountID = player:getAccountID()
    local name      = player:getName()

    local listing = MarketPlace.DBGetListingById(listingID)
    if not listing then
        SendMessage(MarketPlace_Messages["Por"][10], aIndex, 1)
        return false
    end

    if listing.SellerName == name then
        SendMessage(MarketPlace_Messages["Por"][9], aIndex, 1)
        return false
    end

    local display = MarketPlace.GetDisplayName(listing.CoinIndex)
    local balance = MarketPlace.GetBalance(accountID, listing.CoinIndex)

    if balance < listing.Price then
        SendMessage(MarketPlace_Messages["Por"][6], aIndex, 1)
        return false
    end

    local itemCode = GET_ITEM(listing.ItemSection, listing.ItemIndex2)
    if InventoryCheckSpaceByItem(aIndex, itemCode) == 0 then
        SendMessage(MarketPlace_Messages["Por"][11], aIndex, 1)
        return false
    end

    if not MarketPlace.Debit(aIndex, accountID, listing.CoinIndex, listing.Price) then
        SendMessage(MarketPlace_Messages["Por"][6], aIndex, 1)
        return false
    end

    local sellerIndex = GetIndex(listing.SellerName)
    if not MarketPlace.Credit(sellerIndex, listing.SellerAccount, listing.CoinIndex, listing.Price) then
        LogAddC(2, "[MarketPlace] Erro ao creditar para " .. listing.SellerName)
    end

    MarketPlace.DBUpdateListingSold(listingID, accountID)
    MarketPlace.CreateItemInInventory(aIndex, listing)

    SendMessage(MarketPlace_Messages["Por"][7], aIndex, 1)

    if sellerIndex ~= -1 then
        SendMessage(string.format(MarketPlace_Messages["Por"][14], listing.Price, display), sellerIndex, 1)
    end

    SendMessageGlobal(string.format(MarketPlace_Messages["Por"][13], name, listing.ItemName, listing.SellerName, listing.Price, display), 1)
    LogAddC(3, string.format("[MarketPlace] %s comprou %s de %s por %d %s", name, listing.ItemName, listing.SellerName, listing.Price, display))

    MarketPlace.SendBalance(aIndex, accountID)
    MarketPlace.SendListings(aIndex, listing.ItemSection)

    return true
end

function MarketPlace.CancelListing(aIndex, listingID)
    if not MarketPlace_Enabled then
        SendMessage(aIndex, 1, MarketPlace_Messages["Por"][2])
        return false
    end

    local player = User.new(aIndex)
    local name   = player:getName()

    local listing = MarketPlace.DBGetListingById(listingID)
    if not listing then
        SendMessage(aIndex, 1, MarketPlace_Messages["Por"][10])
        return false
    end

    if listing.SellerName ~= name then
        SendMessage(aIndex, 1, MarketPlace_Messages["Por"][9])
        return false
    end

    local itemCode = GET_ITEM(listing.ItemSection, listing.ItemIndex2)
    if InventoryCheckSpaceByItem(aIndex, itemCode) == 0 then
        SendMessage(aIndex, 1, MarketPlace_Messages["Por"][11])
        return false
    end

    MarketPlace.DBUpdateListingCancelled(listingID)
    MarketPlace.CreateItemInInventory(aIndex, listing)

    SendMessage(aIndex, 4, string.format(MarketPlace_Messages["Por"][15], listing.ItemName))
    LogAddC(3, string.format("[MarketPlace] %s cancelou a publicação de %s", name, listing.ItemName))

    MarketPlace.SendListings(aIndex, listing.ItemSection)

    return true
end

function MarketPlace.RequestPrice(aIndex, slot)
    local player = User.new(aIndex)
    local name   = player:getName()
    local inv    = Inventory.new(aIndex)

    local targetSlot = -1

    local clickedSerial = inv:getSerial(slot)

    if clickedSerial and clickedSerial ~= 0 then
        for s = 0, 75 do
            if inv:isItem(s) == 1 and inv:getSerial(s) == clickedSerial then
                targetSlot = s
                break
            end
        end
    end

    if targetSlot == -1 or inv:isItem(targetSlot) == 0 then
        SendMessage(aIndex, 1, MarketPlace_Messages["Por"][3])
        return
    end

    local serial = inv:getSerial(targetSlot)
    MarketPlace_PendingPrice[aIndex] = serial

    CreatePacket("MP_RequestPrice_" .. name, MarketPlace_Packet)
    SetDwordPacket("MP_RequestPrice_" .. name, serial)
    SendPacket("MP_RequestPrice_" .. name, aIndex)
    ClearPacket("MP_RequestPrice_" .. name)
end

function MarketPlace.Protocol(aIndex, Packet, PacketName)
    if Packet ~= MarketPlace_Packet then return false end

    local player    = User.new(aIndex)
    local name      = player:getName()
    local accountID = player:getAccountID()
    
    if PacketName == "MP_Open_" .. name then
        ClearPacket(PacketName)
        MarketPlace.SendCategories(aIndex)
        MarketPlace.SendBalance(aIndex, accountID)
        MarketPlace.SendListings(aIndex, 0)
        return true
    end

    if PacketName == "MP_Category_" .. name then
        local category = GetBytePacket(PacketName, -1)
        ClearPacket(PacketName)
        MarketPlace.SendListings(aIndex, category)
        return true
    end

    if PacketName == "MP_Buy_" .. name then
        local listingID = GetDwordPacket(PacketName, -1)
        ClearPacket(PacketName)
        MarketPlace.BuyItem(aIndex, listingID)
        return true
    end

    if PacketName == "MP_Cancel_" .. name then
        local listingID = GetDwordPacket(PacketName, -1)
        ClearPacket(PacketName)
        MarketPlace.CancelListing(aIndex, listingID)
        return true
    end

    if PacketName == "MP_Deposit_" .. name then
        local slot = GetBytePacket(PacketName, -1)
        ClearPacket(PacketName)
        MarketPlace.RequestPrice(aIndex, slot)
        return true
    end

    if PacketName == "MP_Price_" .. name then
        local price     = GetDwordPacket(PacketName, -1)
        local coinIndex = GetBytePacket(PacketName, -1)
        local serial    = GetDwordPacket(PacketName, -1)
        ClearPacket(PacketName)

        if serial then
            MarketPlace.PostItem(aIndex, serial, price, coinIndex)
            MarketPlace.SendBalance(aIndex, accountID)
        else
            LogAddC(2, "[MarketPlace] Erro: Serial invalido no MP_Price_")
        end
        return true
    end

    return false
end

--function MarketPlace.ChatProc(index, text)
--    if text == "/mp_reload" then
--        local player = User.new(index)
--        if player:getAuthority() >= 32 then
--            MarketPlace.SendCategories(index)
--            MarketPlace.SendBalance(index, player:getAccountID())
--            SendMessage(index, 4, "[MarketPlace] Configurações recarregadas!")
--        end
--        return 1
--    end
--    return 0
--end

MarketPlace.Init()
return MarketPlace
