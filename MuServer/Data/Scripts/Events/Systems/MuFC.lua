local MuFC = {}
local config = MUFC_CONFIG
local participants = {}
local eventOpen = false
local eventRunning = false
local currentPhase = 0
local currentFighters = {}
local joinTimer = nil
local countdownTimer = nil
local fightTimer = nil
local currentEventIndex = nil
local pvpOn = false
local inventoryMonitorTimer = nil
local phaseParticipants = {}
local currentRound = 0
local defeatedOpponents = {}
local initMessage = nil

function MuFC.GetClassName(classId)
    local classes = {
        [0] = "Grand Master",
        [1] = "Blade Knight",
        [2] = "Hight Elf",
        [3] = "Duel Master",
        [4] = "Lord Emperor",
        [5] = "Dimension Master",
        [6] = "Fish Master"
    }
    return classes[classId] or "Todas as Classes"
end

function MuFC.GetEventData(eventIndex)
    if not config.AUTO_START then
        return nil
    end
    for _, event in ipairs(config.AUTO_START) do
        if event.INDEX == eventIndex then
            return event
        end
    end
    return nil
end

function MuFC.GiveReward(aIndex)
    local config = MUFC_CONFIG
    local eventData = MuFC.GetEventData(currentEventIndex) or {NAME = "MuFC"}
    
    if currentEventIndex and config.REWARDS_BY_INDEX and config.REWARDS_BY_INDEX[currentEventIndex] then
        local rewards = config.REWARDS_BY_INDEX[currentEventIndex]
        local player = User.new(aIndex)
        
        local randomReward = rewards[math.random(1, #rewards)]
        
        for i = 1, randomReward.Quantity do
            ItemSerialCreate(aIndex, 236, 0, 0, GET_ITEM(randomReward.Section, randomReward.ID), 0, 0, 0, 0, 0, 0)
        end
        
        local itemName = randomReward.ItemName or "Item de Recompensa"
        SendMessageGlobal(string.format(config.MESSAGES.REWARD_MESSAGE, eventData.NAME, player:getName(), itemName), 0)
        return
    end
    
    if not config.REWARD_WINNER then return end
    
    local randomItem = config.REWARD_WINNER[math.random(1, #config.REWARD_WINNER)]
    ItemCreate(aIndex, randomItem.Type, randomItem.Index, randomItem.Level, randomItem.Dur, randomItem.Opt, randomItem.Exc, randomItem.Anc)
    
    local player = User.new(aIndex)
    SendMessageGlobal(string.format(config.MESSAGES.REWARD_MESSAGE, eventData.NAME, player:getName(), "Item de Recompensa"), 0)
end

function MuFC.Init()
    if config.MUFC_SWITCH == 0 then
        return
    end
    
    Commands.Register(config.COMMAND_OPEN, MuFC.OpenCommand)
    Commands.Register(config.COMMAND_JOIN, MuFC.JoinCommand)
    Commands.Register(config.COMMAND_CANCEL, MuFC.CancelCommand)
    
    for _, event in ipairs(config.AUTO_START) do
        if event.ANNOUNCE_TIME and event.ANNOUNCE_TIME > 0 then
            local announceStartTime = (event.HOUR * 60 + event.MINUTE) - event.ANNOUNCE_TIME
            if announceStartTime < 0 then
                announceStartTime = announceStartTime + 1440
            end
            local announceHour = math.floor(announceStartTime / 60)
            local announceMinute = announceStartTime % 60
            if event.DAY_OF_WEEK ~= -1 then
                Schedule.SetDayOfWeek(event.DAY_OF_WEEK, announceHour, announceMinute, function() 
                    MuFC.StartAnnouncements(event.INDEX) 
                end)
            elseif event.DAY_OF_MONTH ~= -1 then
                Schedule.SetDayAndHourAndMinute(event.DAY_OF_MONTH, announceHour, announceMinute, function() 
                    MuFC.StartAnnouncements(event.INDEX) 
                end)
            else
                Schedule.SetHourAndMinute(announceHour, announceMinute, function() 
                    MuFC.StartAnnouncements(event.INDEX) 
                end)
            end
        end
        if event.DAY_OF_WEEK ~= -1 then
            Schedule.SetDayOfWeek(event.DAY_OF_WEEK, event.HOUR, event.MINUTE, function() 
                MuFC.StartEvent(event.INDEX) 
            end)
        elseif event.DAY_OF_MONTH ~= -1 then
            Schedule.SetDayAndHourAndMinute(event.DAY_OF_MONTH, event.HOUR, event.MINUTE, function() 
                MuFC.StartEvent(event.INDEX) 
            end)
        else
            Schedule.SetHourAndMinute(event.HOUR, event.MINUTE, function() 
                MuFC.StartEvent(event.INDEX) 
            end)
        end
    end
    
    GameServerFunctions.EnterCharacter(MuFC.OnPlayerEnterGame)
    GameServerFunctions.PlayerDie(MuFC.OnPlayerDie)
    GameServerFunctions.PlayerLogout(MuFC.OnPlayerLogout)
    GameServerFunctions.PlayerAttack(MuFC.PlayerAttack)
    GameServerFunctions.PlayerCanEquipItem(MuFC.PlayerCanEquipItem)
end

function MuFC.StartAnnouncements(eventIndex)
    if config.MUFC_SWITCH == 0 then
        return
    end
    
    local eventData = MuFC.GetEventData(eventIndex)
    if not eventData or not eventData.ANNOUNCE_TIME or eventData.ANNOUNCE_TIME <= 0 then
        return
    end

    local remainingMinutes = eventData.ANNOUNCE_TIME
    
    local function DoAnnouncement()
        if remainingMinutes > 0 then
            SendMessageGlobal(string.format(config.MESSAGES.COUNTDOWN_ANNOUNCE, 
                eventData.NAME, remainingMinutes), 0)
            
            remainingMinutes = remainingMinutes - 1
            Timer.TimeOut(60, DoAnnouncement)
        end
    end
    
    DoAnnouncement()
end

function MuFC.PlayerAttack(aIndex, bIndex)
    if config.MUFC_SWITCH == 0 then
        return 0
    end
    
    if eventRunning and currentFighters[aIndex] and currentFighters[bIndex] then
        if not pvpOn then
            SendMessage("[Event] Aguarde a contagem para atacar!", aIndex, 1)
            return 1
        end
        return 0
    end
    return 0
end

function MuFC.PlayerCanEquipItem(aIndex, SourceSlot, TargetSlot)
    if config.MUFC_SWITCH == 0 then
        return
    end
    
    if participants[aIndex] and User.new(aIndex):getClass() == 1 and config.BK_DISABLED_SHIELD == 1 then
        local pInv = Inventory.new(aIndex)
        if pInv:isItem(SourceSlot) ~= 0 and TargetSlot == 1 then
            SendMessage("[Event] Blade Knights não podem equipar escudos durante o evento!", aIndex, 1)
            return 1
        end
    end
    return 0
end

function MuFC.OpenCommand(aIndex, args)
    if config.MUFC_SWITCH == 0 then
        SendMessage(config.MESSAGES.EVENT_DISABLED, aIndex, 1)
        return
    end
    
    local player = User.new(aIndex)
    if player:getAuthority() ~= 32 and CheckGameMasterLevel(player:getAccountID(), player:getName(), EVENT_PVP_GAME_MASTER_LEVEL) == 0 then
        SendMessage("[Event] Você não tem permissão para abrir o evento", aIndex, 1)
        return
    end

    if not args or args == "" then
        local msg = "[Event] Eventos disponíveis:"
        for _, event in ipairs(config.AUTO_START) do
            msg = msg .. string.format("\n%d - %s (Classe: %s)", 
                  event.INDEX, event.NAME, MuFC.GetClassName(event.CLASS))
        end
        SendMessage(msg, aIndex, 1)
        SendMessage("Uso: "..config.COMMAND_OPEN.." [índice]", aIndex, 1)
        return
    end

    local eventIndex
    if type(args) == "table" then
        eventIndex = tonumber(args[1])
    elseif type(args) == "string" then
        eventIndex = tonumber(args:match("%d+"))
    else
        SendMessage("[Event] Índice inválido. Use um número (ex.: 1, 2, 3).", aIndex, 1)
        return
    end

    if not eventIndex then
        SendMessage("[Event] Índice inválido. Use um número (ex.: 1, 2, 3).", aIndex, 1)
        return
    end

    local eventData = MuFC.GetEventData(eventIndex)
    if not eventData then
        SendMessage(string.format("[Event] Evento com índice %d não encontrado.", eventIndex), aIndex, 1)
        return
    end

    currentEventIndex = eventIndex
    participants = {}
    phaseParticipants = {}
    eventOpen = true
    eventRunning = false
    currentPhase = 0
    currentFighters = {}
    pvpOn = false
    currentRound = 0
    defeatedOpponents = {}

    SendMessageGlobal(string.format(config.MESSAGES.OPEN_ANNOUNCE1, eventData.NAME), 0)
    SendMessageGlobal(string.format(config.MESSAGES.OPEN_ANNOUNCE, config.COMMAND_JOIN, config.JOIN_TIME), 0)
    SendMessage(string.format(config.MESSAGES.EVENT_STARTED, eventIndex, eventData.NAME), aIndex, 1)

    local remaintime = config.JOIN_TIME
    initMessage = Timer.Interval(config.REMAIN_TIME_MESSAGE, function()
	remaintime = remaintime - config.REMAIN_TIME_MESSAGE
	if remaintime > 0 then
		SendMessageGlobal(string.format(config.MESSAGES.OPEN_ANNOUNCE1, eventData.NAME), 0)
		SendMessageGlobal(string.format(config.MESSAGES.OPEN_ANNOUNCE, config.COMMAND_JOIN, remaintime), 0)
	end
    end)

    if inventoryMonitorTimer then Timer.Cancel(inventoryMonitorTimer) end
    inventoryMonitorTimer = Timer.Interval(2, MuFC.MonitorInventory)

    if joinTimer then Timer.Cancel(joinTimer) end
    joinTimer = Timer.TimeOut(config.JOIN_TIME, function()
        eventOpen = false
        SendMessageGlobal(string.format(config.MESSAGES.COMMAND_CLOSED, eventData.NAME), 0)
        MuFC.BeginEvent()
    end)
end

function MuFC.JoinCommand(aIndex, args)
    if config.MUFC_SWITCH == 0 then
        SendMessage(config.MESSAGES.EVENT_DISABLED, aIndex, 1)
        return
    end
    
    if not eventOpen then
        local eventData = MuFC.GetEventData(currentEventIndex) or {NAME = "MuFC"}
        SendMessage(string.format(config.MESSAGES.COMMAND_CLOSED, eventData.NAME), aIndex, 1)
        return
    end
    
    local player = User.new(aIndex)
    local eventData = MuFC.GetEventData(currentEventIndex)
    
    if not eventData then
        SendMessage(config.MESSAGES.EVENT_NOT_FOUND, aIndex, 1)
        return
    end
    
    local playerLevel = player:getLevel()
    if playerLevel < config.MINIMUM_LEVEL then
        SendMessage(string.format(config.MESSAGES.LEVEL_TOO_LOW, eventData.NAME, config.MINIMUM_LEVEL), aIndex, 1)
        return
    end
    
    if eventData.CLASS ~= -1 and player:getClass() ~= eventData.CLASS then
        local className = MuFC.GetClassName(eventData.CLASS)
        SendMessage(string.format(config.MESSAGES.CLASS_RESTRICTED, eventData.NAME, className), aIndex, 1)
        return
    end
    
    if participants[aIndex] then
        SendMessage("[Event] Você já está no evento", aIndex, 1)
        return
    end
    
    local pInv = Inventory.new(aIndex)
    
    if config.BLOCK_ITEM_VISUAL == 1 then
        for slot = 240, 247 do
            if pInv:isItem(slot) ~= 0 then
                SendMessage("[Event] Não é permitido participar usando itens visuais!", aIndex, 1)
                return
            end
        end
    end
    
    if player:getClass() == 1 and config.BK_DISABLED_SHIELD == 1 and pInv:getIndex(1) >= GET_ITEM(6,0) and pInv:getIndex(1) <= GET_ITEM(7,0) then
        if pInv:isItem(1) ~= 0 then
            SendMessage("[Event] Classe BK não pode entrar no evento com shield!", aIndex, 1)
            return
        end
    end
    
    participants[aIndex] = {
        index = aIndex,
        name = player:getName(),
        class = player:getClass(),
        phase = 1
    }
    
    defeatedOpponents[aIndex] = 0
    
    player:setPKLevel(6)
    
    if config.REMOVE_BUFFS == 1 then
	for buff = 1, 205 do
		if buff ~= 28 then
		RemoveEffect(aIndex, buff)
		end
	end
	CalCharacter(aIndex)
	RefreshCharacter(aIndex)
    end

    Teleport(aIndex, config.WAITING_ROOM.MAP, config.WAITING_ROOM.X, config.WAITING_ROOM.Y)
    SendMessage("[Event] Você entrou no evento. Aguarde o início.", aIndex, 1)
    SendMessageGlobal(string.format(config.MESSAGES.JOIN_PLAYER, eventData.NAME, player:getName()), 0)
end

function MuFC.CancelCommand(aIndex)
    if config.MUFC_SWITCH == 0 then
        SendMessage(config.MESSAGES.EVENT_DISABLED, aIndex, 1)
        return
    end
    
    local player = User.new(aIndex)
    if player:getAuthority() ~= 32 and CheckGameMasterLevel(player:getAccountID(), player:getName(), EVENT_PVP_GAME_MASTER_LEVEL) == 0 then
        SendMessage("[Event] Você não tem permissão para cancelar o evento", aIndex, 1)
        return
    end

    if not eventOpen and not eventRunning then
        SendMessage(config.MESSAGES.NO_EVENT_TO_CANCEL, aIndex, 1)
        return
    end

    local eventData = MuFC.GetEventData(currentEventIndex) or {NAME = "MuFC"}
    SendMessageGlobal(string.format(config.MESSAGES.EVENT_CANCELED, eventData.NAME), 0)
    
    for index, _ in pairs(participants) do
        local PkPlayer = User.new(index)
	PkPlayer:setPKLevel(3)
	Teleport(index, 0, 125, 125)
    end
    
    MuFC.ResetEvent()
end

function MuFC.StartEvent(eventIndex)
    if config.MUFC_SWITCH == 0 then
        return
    end
    
    if eventRunning then return end
    
    local eventData = MuFC.GetEventData(eventIndex)
    if not eventData then
        return
    end
    
    currentEventIndex = eventIndex
    participants = {}
    phaseParticipants = {}
    eventOpen = true
    eventRunning = false
    currentPhase = 0
    currentFighters = {}
    pvpOn = false
    currentRound = 0
    defeatedOpponents = {}
       
    SendMessageGlobal(string.format(config.MESSAGES.OPEN_ANNOUNCE1, eventData.NAME), 0)
SendMessageGlobal(string.format(config.MESSAGES.OPEN_ANNOUNCE, config.COMMAND_JOIN, config.JOIN_TIME), 0)

local remaintime = config.JOIN_TIME
initMessage = Timer.Interval(config.REMAIN_TIME_MESSAGE, function()
    remaintime = remaintime - config.REMAIN_TIME_MESSAGE
    if remaintime > 0 then
        SendMessageGlobal(string.format(config.MESSAGES.OPEN_ANNOUNCE1, eventData.NAME), 0)
        SendMessageGlobal(string.format(config.MESSAGES.OPEN_ANNOUNCE, config.COMMAND_JOIN, remaintime), 0)
    end
end)

if inventoryMonitorTimer then Timer.Cancel(inventoryMonitorTimer) end
inventoryMonitorTimer = Timer.Interval(2, MuFC.MonitorInventory)

if joinTimer then Timer.Cancel(joinTimer) end
joinTimer = Timer.TimeOut(config.JOIN_TIME, function()
    eventOpen = false
    SendMessageGlobal(string.format(config.MESSAGES.COMMAND_CLOSED, eventData.NAME), 0)
    MuFC.BeginEvent()
end)
end

function MuFC.MonitorInventory()
    if config.MUFC_SWITCH == 0 then
        return
    end
    
    if not (eventOpen or eventRunning) then
        return
    end

    local eventData = MuFC.GetEventData(currentEventIndex) or {NAME = "MuFC"}

    for aIndex, participant in pairs(participants) do
        local player = User.new(aIndex)
        local pInv = Inventory.new(aIndex)
        local disqualified = false
        local reason = ""

        if config.BLOCK_ITEM_VISUAL == 1 then
            for slot = 240, 247 do
                if pInv:isItem(slot) ~= 0 then
                    disqualified = true
                    reason = string.format("equipar itens visuais no evento")
                    SendMessage("[Event] Você foi desclassificado por equipar itens visuais na sala de espera ou durante o evento!", aIndex, 1)
                    break
                end
            end
        end

        if not disqualified and player:getClass() == 1 and config.BK_DISABLED_SHIELD == 1 and pInv:getIndex(1) >= GET_ITEM(6,0) and pInv:getIndex(1) <= GET_ITEM(7,0) then
            if pInv:isItem(1) ~= 0 then
                disqualified = true
                reason = "equipar um Shield no evento"
                SendMessage("[Event] Você foi desclassificado por equipar um escudo na sala de espera ou durante o evento!", aIndex, 1)
            end
        end
                -- Checagem de saída do mapa do evento (mapa 6)
        if not disqualified then
            local map = player:getMapNumber()
            if map ~= 6 then
                disqualified = true
                reason = "sair do mapa do evento"
                SendMessage("[Event] Você foi desclassificado por sair do mapa do evento!", aIndex, 1)
            end
        end 
	 
        if disqualified then
            Teleport(aIndex, 0, 125, 125)
            SendMessageGlobal(string.format("[Event] %s foi desclassificado por %s!", player:getName(), reason), 0)
            Timer.TimeOut(5, function()
                CloseChar(aIndex, 1)
            end)

            participants[aIndex] = nil
            defeatedOpponents[aIndex] = nil

            if currentFighters[aIndex] then
                local remainingFighter = nil
                local remainingFighterData = nil

                for index in pairs(currentFighters) do
                    if index ~= aIndex then
                        remainingFighter = index
                        remainingFighterData = currentFighters[index]
                        break
                    end
                end

                currentFighters[aIndex] = nil

                if eventRunning and remainingFighter and remainingFighterData then
                    Teleport(remainingFighter, config.WAITING_ROOM.MAP, config.WAITING_ROOM.X, config.WAITING_ROOM.Y)
                    currentFighters = {}

                    phaseParticipants[remainingFighter] = participants[remainingFighter]
                    participants[remainingFighter] = nil
                    defeatedOpponents[remainingFighter] = (defeatedOpponents[remainingFighter] or 0) + 1

                    local count = 0
                    for _ in pairs(phaseParticipants) do count = count + 1 end

                    if count < 2 then
                        local winner = phaseParticipants[remainingFighter]
                        if winner then
                            SendMessageGlobal(string.format(config.MESSAGES.FINAL_WINNER, 
                                eventData.NAME, winner.name, eventData.NAME, defeatedOpponents[remainingFighter] or 0), 0)
                            MuFC.GiveReward(winner.index)
                            Teleport(winner.index, 0, 125, 125)
                        end
                        MuFC.ResetEvent()
                    else
                        Timer.TimeOut(2, function()
                            MuFC.StartNextPhase()
                        end)
                    end
                elseif eventRunning then
                    currentFighters = {}
                    Timer.TimeOut(2, function()
                        MuFC.StartNextFight()
                    end)
                end
            end
        end
    end
end

function MuFC.BeginEvent()
    if config.MUFC_SWITCH == 0 then
        return
    end
    
    Timer.Cancel(initMessage)
    
    local count = 0
    for _ in pairs(participants) do count = count + 1 end
    
    local eventData = MuFC.GetEventData(currentEventIndex) or {NAME = "MuFC"}
    
    if count < config.MIN_USERS then
        for index, _ in pairs(participants) do
	    local PkPlayer = User.new(index)
	    PkPlayer:setPKLevel(3)
            Teleport(index, 0, 125, 125)
        end
        SendMessageGlobal(string.format(config.MESSAGES.NOT_ENOUGH_PLAYERS, eventData.NAME), 0)
        MuFC.ResetEvent()
        return
    end
    
    eventRunning = true
    currentPhase = 1
    SendMessageGlobal(string.format(config.MESSAGES.START_ANNOUNCE, eventData.NAME), 0)
    
    if inventoryMonitorTimer then Timer.Cancel(inventoryMonitorTimer) end
    inventoryMonitorTimer = Timer.Interval(2, MuFC.MonitorInventory)
    
    MuFC.StartNextFight()
end

function MuFC.StartNextPhase()
    currentPhase = currentPhase + 1
    local eventData = MuFC.GetEventData(currentEventIndex) or {NAME = "MuFC"}
    
    participants = {}
    for index, participant in pairs(phaseParticipants) do
        participants[index] = participant
        participant.phase = currentPhase
    end
    phaseParticipants = {}
    
    SendMessageGlobal(string.format("[Event] Iniciando Fase %d do %s!", currentPhase, eventData.NAME), 0)
    MuFC.StartNextFight()
end

function MuFC.StartNextFight()
    if config.MUFC_SWITCH == 0 then
        return
    end
    
    local count = 0
    for _ in pairs(participants) do count = count + 1 end
    
    local eventData = MuFC.GetEventData(currentEventIndex) or {NAME = "MuFC"}
    
    if count < 2 then
        if count == 1 then
            for index, participant in pairs(participants) do
                phaseParticipants[index] = participant
                participants[index] = nil
            end
            
            local nextPhaseCount = 0
            for _ in pairs(phaseParticipants) do nextPhaseCount = nextPhaseCount + 1 end
            
            if nextPhaseCount >= 2 then
                MuFC.StartNextPhase()
            else
                local winner = nil
                for _, participant in pairs(phaseParticipants) do
                    winner = participant
                    break
                end
                
                if winner then
                    SendMessageGlobal(string.format(config.MESSAGES.FINAL_WINNER, 
                        eventData.NAME, winner.name, eventData.NAME, defeatedOpponents[winner.index] or 0), 0)
                    MuFC.GiveReward(winner.index)
                    Teleport(winner.index, 0, 125, 125)
                end
                MuFC.ResetEvent()
            end
        else
            MuFC.EndEvent()
        end
        return
    end
    
    local available = {}
    for _, p in pairs(participants) do
        table.insert(available, p)
    end
    
    local p1Index = math.random(1, #available)
    local p1 = available[p1Index]
    table.remove(available, p1Index)
    
    local p2Index = math.random(1, #available)
    local p2 = available[p2Index]
    
    currentFighters = {
        [p1.index] = {index = p1.index, name = p1.name, class = p1.class, phase = p1.phase, roundWins = 0},
        [p2.index] = {index = p2.index, name = p2.name, class = p2.class, phase = p2.phase, roundWins = 0}
    }
    
    currentRound = 0
    
    SendMessageGlobal(string.format(config.MESSAGES.MATCH_ANNOUNCE, 
        eventData.NAME, p1.name, p2.name), 0)
    
    Timer.TimeOut(2, function()
        MuFC.StartNextRound()
    end)
end

function MuFC.StartNextRound()
    if config.MUFC_SWITCH == 0 then
        return
    end
    
    currentRound = currentRound + 1
    local eventData = MuFC.GetEventData(currentEventIndex) or {NAME = "MuFC"}
    
    pvpOn = false
    
    for index in pairs(currentFighters) do
        Teleport(index, config.BATTLE_AREA.MAP, config.BATTLE_AREA.X, config.BATTLE_AREA.Y)
        local player = User.new(index)
        player:setPKLevel(6)
    end
    
    MuFC.StartCountdown()
end

function MuFC.StartCountdown()
    if config.MUFC_SWITCH == 0 then
        return
    end
    
    local eventData = MuFC.GetEventData(currentEventIndex) or {NAME = "MuFC"}
    
    if countdownTimer then
        Timer.Cancel(countdownTimer)
        countdownTimer = nil
    end
    
    for index in pairs(currentFighters) do
        SendMessage(string.format(config.MESSAGES.BUFF_MESSAGE, eventData.NAME), index, 0)
    end
    
    pvpOn = false
    Timer.TimeOut(5, function()
        for index in pairs(currentFighters) do
            SendMessage(string.format(config.MESSAGES.PREPARE_MESSAGE, eventData.NAME), index, 0)
        end
        
        local count = 5
        countdownTimer = Timer.Interval(1, function()
            if count > 0 then
                for index in pairs(currentFighters) do
                    if currentFighters[index] then
                        SendMessage(string.format(config.MESSAGES.COUNTDOWN, eventData.NAME, count), index, 0)
                    end
                end
                count = count - 1
            else
                Timer.Cancel(countdownTimer)
                countdownTimer = nil
                if next(currentFighters) then
                    MuFC.StartFight()
                end
            end
        end)
    end)
end

function MuFC.StartFight()
    if config.MUFC_SWITCH == 0 then
        return
    end
    
    local eventData = MuFC.GetEventData(currentEventIndex) or {NAME = "MuFC"}
    
    if not next(currentFighters) then
        return
    end
    
    for index in pairs(currentFighters) do
        SendMessage(string.format("[ %s ] - Round %d: Ataquem-se! GOOOOOOOOOOOOOOO!", eventData.NAME, currentRound), index, 0)
    end
    
    pvpOn = true
    
    if fightTimer then
        Timer.Cancel(fightTimer)
        fightTimer = nil
    end
end

function MuFC.EndFight(winnerIndex, loserIndex)
    if config.MUFC_SWITCH == 0 then
        return
    end
    
    if fightTimer then Timer.Cancel(fightTimer) end
    pvpOn = false
    
    local eventData = MuFC.GetEventData(currentEventIndex) or {NAME = "MuFC"}
    local winner = currentFighters[loserIndex]
    local loser = currentFighters[winnerIndex]
    
    if not (winner and loser) then
        currentFighters = {}
        Timer.TimeOut(5, function()
            MuFC.StartNextFight()
        end)
        return
    end
    
    winner.roundWins = winner.roundWins + 1
    
    local fighter1, fighter2
    for index, fighter in pairs(currentFighters) do
        if fighter1 then
            fighter2 = fighter
        else
            fighter1 = fighter
        end
    end
    
    if fighter1 and fighter2 then
        SendMessageGlobal(string.format(config.MESSAGES.SCORE_MESSAGE, eventData.NAME, fighter1.name, fighter1.roundWins, fighter2.roundWins, fighter2.name), 0)
    end
    
    if currentRound >= config.ROUNDS then
        local matchWinner = winner.roundWins > loser.roundWins and winner or loser
        local matchLoser = winner.roundWins > loser.roundWins and loser or winner
        
        SendMessageGlobal(string.format(config.MESSAGES.MATCH_WINNER, 
            eventData.NAME, matchWinner.name), 0)
        
        defeatedOpponents[matchWinner.index] = (defeatedOpponents[matchWinner.index] or 0) + 1
        
        phaseParticipants[matchWinner.index] = participants[matchWinner.index]
        participants[matchWinner.index] = nil
        
        Teleport(matchWinner.index, config.WAITING_ROOM.MAP, config.WAITING_ROOM.X, config.WAITING_ROOM.Y)
        
        participants[matchLoser.index] = nil
        defeatedOpponents[matchLoser.index] = nil
        Teleport(matchLoser.index, 0, 125, 125)
        
        currentFighters = {}
        
        local remainingInPhase = 0
        for _ in pairs(participants) do remainingInPhase = remainingInPhase + 1 end
        
        if remainingInPhase >= 2 then
            Timer.TimeOut(5, function()
                MuFC.StartNextFight()
            end)
        else
            for index, participant in pairs(participants) do
                phaseParticipants[index] = participant
                participants[index] = nil
                SendMessageGlobal(string.format("[Event] %s avança automaticamente para a próxima fase!", participant.name), 0)
                Teleport(index, config.WAITING_ROOM.MAP, config.WAITING_ROOM.X, config.WAITING_ROOM.Y)
            end
            
            local nextPhaseCount = 0
            for _ in pairs(phaseParticipants) do nextPhaseCount = nextPhaseCount + 1 end
            
            if nextPhaseCount >= 2 then
                Timer.TimeOut(5, function()
                    MuFC.StartNextPhase()
                end)
            else
                local winner = nil
                for _, participant in pairs(phaseParticipants) do
                    winner = participant
                    break
                end
                
                if winner then
                    SendMessageGlobal(string.format(config.MESSAGES.FINAL_WINNER, 
                        eventData.NAME, winner.name, eventData.NAME, defeatedOpponents[winner.index] or 0), 0)
                    MuFC.GiveReward(winner.index)
                    Teleport(winner.index, 0, 125, 125)
                end
                MuFC.ResetEvent()
            end
        end
    else
        Timer.TimeOut(5, function()
            MuFC.StartNextRound()
        end)
    end
end

function MuFC.EndEvent()
    if config.MUFC_SWITCH == 0 then
        return
    end
    
    local winner = nil
    local eventData = MuFC.GetEventData(currentEventIndex) or {NAME = "MuFC"}
    
    for _, participant in pairs(phaseParticipants) do
        winner = participant
        break
    end
    
    if not winner then
        for _, participant in pairs(participants) do
            winner = participant
            break
        end
    end
    
    if winner then
        SendMessageGlobal(string.format(config.MESSAGES.FINAL_WINNER, 
            eventData.NAME, winner.name, eventData.NAME, defeatedOpponents[winner.index] or 0), 0)
        MuFC.GiveReward(winner.index)
        Teleport(winner.index, 0, 125, 125)
    end
    
    MuFC.ResetEvent()
end

function MuFC.ResetEvent()
    if config.MUFC_SWITCH == 0 then
        return
    end
    
    eventOpen = false
    eventRunning = false
    currentPhase = 0
    currentFighters = {}
    participants = {}
    phaseParticipants = {}
    currentEventIndex = nil
    pvpOn = false
    currentRound = 0
    defeatedOpponents = {}
    
    if joinTimer then Timer.Cancel(joinTimer) end
    if countdownTimer then Timer.Cancel(countdownTimer) end
    if fightTimer then Timer.Cancel(fightTimer) end
    if inventoryMonitorTimer then Timer.Cancel(inventoryMonitorTimer) end
end

function MuFC.OnPlayerDie(aIndex, TargetIndex)
    if config.MUFC_SWITCH == 0 then
        return
    end
    
    if not eventRunning then return end
    
    local dead = currentFighters[aIndex]
    if not dead then return end
    
    local loserIndex = aIndex
    local winnerIndex = TargetIndex
    
    if winnerIndex <= 0 or not currentFighters[winnerIndex] then
        for index, fighter in pairs(currentFighters) do
            if index ~= aIndex then
                winnerIndex = index
                break
            end
        end
    end
    
    if not winnerIndex then
        return
    end
    
    MuFC.EndFight(winnerIndex, loserIndex)
end

function MuFC.OnPlayerLogout(aIndex)
    if config.MUFC_SWITCH == 0 then
        return
    end
    local pL = User.new(aIndex)
    pL:setPKLevel(3)
    
    if participants[aIndex] then
        local eventData = MuFC.GetEventData(currentEventIndex) or {NAME = "MuFC"}
        SendMessageGlobal(string.format("[Event] %s foi desclassificado por sair do jogo.", participants[aIndex].name), 0)
        participants[aIndex] = nil
        defeatedOpponents[aIndex] = nil
    end
    
    if currentFighters[aIndex] then
        local eventData = MuFC.GetEventData(currentEventIndex) or {NAME = "MuFC"}
        local remainingFighter = nil
        local remainingFighterData = nil
        
        if countdownTimer then
            Timer.Cancel(countdownTimer)
            countdownTimer = nil
        end
        
        for index in pairs(currentFighters) do
            if index ~= aIndex then
                remainingFighter = index
                remainingFighterData = currentFighters[index]
                break
            end
        end
        
        currentFighters[aIndex] = nil
        
        if eventRunning and remainingFighter and remainingFighterData then
            local winnerIndex = remainingFighter
            local loserIndex = aIndex
            
            remainingFighterData.roundWins = 2
            
            SendMessageGlobal(string.format("[Event] %s venceu a partida pois %s deslogou.", 
                remainingFighterData.name, User.new(aIndex):getName()), 0)
            
            defeatedOpponents[winnerIndex] = (defeatedOpponents[winnerIndex] or 0) + 1
            
            phaseParticipants[winnerIndex] = participants[winnerIndex]
            participants[winnerIndex] = nil
            Teleport(winnerIndex, config.WAITING_ROOM.MAP, config.WAITING_ROOM.X, config.WAITING_ROOM.Y)
            
            currentFighters = {}
            
            local remainingInPhase = 0
            for _ in pairs(participants) do remainingInPhase = remainingInPhase + 1 end
            
            if remainingInPhase >= 2 then
                Timer.TimeOut(2, function()
                    MuFC.StartNextFight()
                end)
            else
                for index, participant in pairs(participants) do
                    phaseParticipants[index] = participant
                    participants[index] = nil
                    SendMessageGlobal(string.format("[Event] %s avança automaticamente para a próxima fase!", participant.name), 0)
                    Teleport(index, config.WAITING_ROOM.MAP, config.WAITING_ROOM.X, config.WAITING_ROOM.Y)
                end
                
                local nextPhaseCount = 0
                for _ in pairs(phaseParticipants) do nextPhaseCount = nextPhaseCount + 1 end
                
                if nextPhaseCount >= 2 then
                    Timer.TimeOut(2, function()
                        MuFC.StartNextPhase()
                    end)
                else
                    local winner = nil
                    for _, participant in pairs(phaseParticipants) do
                        winner = participant
                        break
                    end
                    
                    if winner then
                        SendMessageGlobal(string.format(config.MESSAGES.FINAL_WINNER, 
                            eventData.NAME, winner.name, eventData.NAME, defeatedOpponents[winner.index] or 0), 0)
                        MuFC.GiveReward(winner.index)
                        Teleport(winner.index, 0, 125, 125)
                    end
                    MuFC.ResetEvent()
                end
            end
        elseif eventRunning then
            currentFighters = {}
            MuFC.StartNextFight()
        end
    end
end

function MuFC.OnPlayerEnterGame(aIndex)
    if not eventRunning then
        return
    end
    
    local player = User.new(aIndex)
    local map = player:getMapNumber()
    local x = player:getX()
    local y = player:getY()
    
    local duelMap = config.BATTLE_AREA.MAP
    local duelX = config.BATTLE_AREA.X
    local duelY = config.BATTLE_AREA.Y
    
    if map == duelMap and math.abs(x - duelX) <= 10 and math.abs(y - duelY) <= 10 then
	player:setPKLevel(3)
        Teleport(aIndex, 0, 125, 125)
        SendMessage("[Event] Movido para fora do evento!", aIndex, 1)
    end
end

MuFC.Init()

return MuFC