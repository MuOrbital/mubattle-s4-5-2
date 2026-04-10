TransferCoin = { }
TransferCoin.PendingTransactions = { }

function TransferCoin.GetCoinConfig(coinName)
    local coins = {}
    if TRANSFER_COIN_COUNT >= 1 then
        coins[TRANSFER_COIN_NAME1] = {table = TRANSFER_COIN_TABLE1, accountColumn = TRANSFER_COIN_COLUMN_ACCOUNT1, coinColumn = TRANSFER_COIN_COLUMN_COIN1}
    end
    if TRANSFER_COIN_COUNT >= 2 then
        coins[TRANSFER_COIN_NAME2] = {table = TRANSFER_COIN_TABLE2, accountColumn = TRANSFER_COIN_COLUMN_ACCOUNT2, coinColumn = TRANSFER_COIN_COLUMN_COIN2}
    end
    if TRANSFER_COIN_COUNT >= 3 then
        coins[TRANSFER_COIN_NAME3] = {table = TRANSFER_COIN_TABLE3, accountColumn = TRANSFER_COIN_COLUMN_ACCOUNT3, coinColumn = TRANSFER_COIN_COLUMN_COIN3}
    end
    if TRANSFER_COIN_COUNT >= 4 then
        coins[TRANSFER_COIN_NAME4] = {table = TRANSFER_COIN_TABLE4, accountColumn = TRANSFER_COIN_COLUMN_ACCOUNT4, coinColumn = TRANSFER_COIN_COLUMN_COIN4}
    end
    return coins[coinName]
end

function TransferCoin.CheckRecipientData(targetAccount, coinConfig)
    local result = DataBase.GetValue(coinConfig.table, coinConfig.coinColumn, coinConfig.accountColumn, targetAccount)
    return result ~= nil
end

function TransferCoin.CheckSenderBalance(aIndex, coinConfig, amount)
    local player = User.new(aIndex)
    local account = player:getAccountID()
    local balance = DataBase.GetValue(coinConfig.table, coinConfig.coinColumn, coinConfig.accountColumn, account)
    return balance >= amount
end

function TransferCoin.CheckDiscountBalance(aIndex)
    if TRANSFER_COIN_DISCOUNT == 0 then return true end
    local player = User.new(aIndex)
    local account = player:getAccountID()
    local balance = DataBase.GetValue(TRANSFER_COIN_DISCOUNT_TABLE, TRANSFER_COIN_DISCOUNT_COLUMN_COIN, TRANSFER_COIN_DISCOUNT_COLUMN, account)
    return balance >= TRANSFER_COIN_DISCOUNT_VALUE
end

function TransferCoin.TransferCommand(aIndex, Arguments)
    local player = User.new(aIndex)
    local Language = player:getLanguage()
    
    if TRANSFER_COIN_SWITCH == 0 then return end
    
    -- Verifica o nível VIP do jogador
    if player:getVip() < TRANSFER_COIN_COMMAND_VIP then
        SendMessage(TRANSFER_COIN_MESSAGES[Language][18], aIndex, 1)
        return
    end
    
    if TransferCoin.PendingTransactions[aIndex] ~= nil then
        SendMessage(TRANSFER_COIN_MESSAGES[Language][17], aIndex, 1)
        return
    end
    
    local amount = command:getNumber(Arguments, 1)
    local coinName = command:getString(Arguments, 2, 0)
    local targetName = command:getString(Arguments, 3, 0)
    
    if #coinName <= 0 or amount <= 0 or #targetName <= 0 then
        SendMessage(string.format(TRANSFER_COIN_MESSAGES[Language][1], amount, coinName, targetName), aIndex, 1)
        return
    end
    
    if amount < TRANSFER_COIN_COUNT_MIN then
        SendMessage(string.format(TRANSFER_COIN_MESSAGES[Language][11], TRANSFER_COIN_COUNT_MIN), aIndex, 1)
        return
    end
    
    local coinConfig = TransferCoin.GetCoinConfig(coinName)
    if coinConfig == nil then
        SendMessage(TRANSFER_COIN_MESSAGES[Language][12], aIndex, 1)
        return
    end
    
    local targetIndex = GetIndex(targetName)
    if targetIndex == -1 then
        SendMessage(string.format(TRANSFER_COIN_MESSAGES[Language][4], targetName), aIndex, 1)
        return
    end
    
    local targetPlayer = User.new(targetIndex)
    local targetAccount = targetPlayer:getAccountID()
    if not TransferCoin.CheckRecipientData(targetAccount, coinConfig) then
        SendMessage(TRANSFER_COIN_MESSAGES[Language][5], aIndex, 1)
        return
    end
    
    if not TransferCoin.CheckSenderBalance(aIndex, coinConfig, amount) then
        SendMessage(TRANSFER_COIN_MESSAGES[Language][13], aIndex, 1)
        return
    end
    
    if TRANSFER_COIN_DISCOUNT == 1 and not TransferCoin.CheckDiscountBalance(aIndex) then
        SendMessage(TRANSFER_COIN_MESSAGES[Language][14], aIndex, 1)
        return
    end
    
    TransferCoin.PendingTransactions[aIndex] = {
        coinName = coinName,
        amount = amount,
        targetIndex = targetIndex,
        targetName = targetName,
        coinConfig = coinConfig
    }
    
    if TRANSFER_COIN_DISCOUNT == 1 then
        SendMessage(string.format(TRANSFER_COIN_MESSAGES[Language][2], amount, coinName, targetName, TRANSFER_COIN_DISCOUNT_VALUE, TRANSFER_COIN_DISCOUNT_COIN), aIndex, 1)
    else
        SendMessage(string.format(TRANSFER_COIN_MESSAGES[Language][1], amount, coinName, targetName), aIndex, 1)
    end
    SendMessage(string.format(TRANSFER_COIN_MESSAGES[Language][3], TRANSFER_COIN_COMMAND_CONFIRM, TRANSFER_COIN_COMMAND_CANCEL), aIndex, 1)
end

function TransferCoin.ConfirmCommand(aIndex, Arguments)
    local player = User.new(aIndex)
    local Language = player:getLanguage()
    
    local transaction = TransferCoin.PendingTransactions[aIndex]
    if transaction == nil then return end
    
    local targetPlayer = User.new(transaction.targetIndex)
    local targetAccount = targetPlayer:getAccountID()
    local senderAccount = player:getAccountID()
    
    if targetPlayer:getConnected() < 3 then
        SendMessage(string.format(TRANSFER_COIN_MESSAGES[Language][4], transaction.targetName), aIndex, 1)
        TransferCoin.PendingTransactions[aIndex] = nil
        return
    end
    
    if not TransferCoin.CheckSenderBalance(aIndex, transaction.coinConfig, transaction.amount) then
        SendMessage(TRANSFER_COIN_MESSAGES[Language][13], aIndex, 1)
        TransferCoin.PendingTransactions[aIndex] = nil
        return
    end
    
    if TRANSFER_COIN_DISCOUNT == 1 and not TransferCoin.CheckDiscountBalance(aIndex) then
        SendMessage(TRANSFER_COIN_MESSAGES[Language][14], aIndex, 1)
        TransferCoin.PendingTransactions[aIndex] = nil
        return
    end
    
    DataBase.SetAddValue(transaction.coinConfig.table, transaction.coinConfig.coinColumn, transaction.amount, transaction.coinConfig.accountColumn, targetAccount)
    DataBase.SetAddValue(transaction.coinConfig.table, transaction.coinConfig.coinColumn, -transaction.amount, transaction.coinConfig.accountColumn, senderAccount)
    
    if TRANSFER_COIN_DISCOUNT == 1 then
        DataBase.SetAddValue(TRANSFER_COIN_DISCOUNT_TABLE, TRANSFER_COIN_DISCOUNT_COLUMN_COIN, -TRANSFER_COIN_DISCOUNT_VALUE, TRANSFER_COIN_DISCOUNT_COLUMN, senderAccount)
        SendMessage(string.format(TRANSFER_COIN_MESSAGES[Language][8], TRANSFER_COIN_DISCOUNT_COIN, TRANSFER_COIN_DISCOUNT_VALUE), aIndex, 1)
        LogAdd(string.format("[TranferCoin] Descontado - [%d] | [%s] de [%s] por Tranferencia pra [%s]", transaction.amount, transaction.coinName, player:getName(), transaction.targetName), 0)
    end
    
    SendMessage(string.format(TRANSFER_COIN_MESSAGES[Language][6], transaction.targetName, transaction.amount, transaction.coinName), aIndex, 1)
    SendMessage(string.format(TRANSFER_COIN_MESSAGES[targetPlayer:getLanguage()][9], transaction.amount, transaction.coinName, player:getName()), transaction.targetIndex, 1)
    LogAdd(string.format("[TranferCoin] Player - [%s] | [%d] - [%s] para [%s]", player:getName(), transaction.amount, transaction.coinName, transaction.targetName), 0)
    
    SendMessage(TRANSFER_COIN_MESSAGES[Language][10], aIndex, 1)
    SendMessage(TRANSFER_COIN_MESSAGES[targetPlayer:getLanguage()][10], transaction.targetIndex, 1)
    CloseChar(aIndex, 1)
    CloseChar(transaction.targetIndex, 1)
    
    TransferCoin.PendingTransactions[aIndex] = nil
end

function TransferCoin.CancelCommand(aIndex, Arguments)
    local player = User.new(aIndex)
    local Language = player:getLanguage()
    
    if TransferCoin.PendingTransactions[aIndex] ~= nil then
        SendMessage(TRANSFER_COIN_MESSAGES[Language][15], aIndex, 1)
        TransferCoin.PendingTransactions[aIndex] = nil
    end
end

function TransferCoin.PlayerLogout(aIndex)
    if TransferCoin.PendingTransactions[aIndex] ~= nil then
        local player = User.new(aIndex)
        local Language = player:getLanguage()
        SendMessage(TRANSFER_COIN_MESSAGES[Language][16], aIndex, 1)
        TransferCoin.PendingTransactions[aIndex] = nil
    end
end

function TransferCoin.Init()
    if TRANSFER_COIN_SWITCH == 0 then return end
    
    Commands.Register(TRANSFER_COIN_COMMAND, TransferCoin.TransferCommand)
    Commands.Register(TRANSFER_COIN_COMMAND_CONFIRM, TransferCoin.ConfirmCommand)
    Commands.Register(TRANSFER_COIN_COMMAND_CANCEL, TransferCoin.CancelCommand)
    GameServerFunctions.PlayerLogout(TransferCoin.PlayerLogout)
end

TransferCoin.Init()

return TransferCoin