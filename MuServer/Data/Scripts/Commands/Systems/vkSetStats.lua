--[[ SetStats System - By: Victor Jardim 2025]]--

VKSETSTATUS = {}

function VKSETSTATUS.Strength(targetPlayer, value)
	local actualStatus = targetPlayer:getStrength()
	if value == 0 then targetPlayer:setStrength(actualStatus) else targetPlayer:setStrength(value) end
end

function VKSETSTATUS.Dexterity(targetPlayer, value)
	local actualStatus = targetPlayer:getDexterity()
	if value == 0 then targetPlayer:setDexterity(actualStatus) else targetPlayer:setDexterity(value) end
end

function VKSETSTATUS.Vitality(targetPlayer, value)
	local actualStatus = targetPlayer:getVitality()
	if value == 0 then targetPlayer:setVitality(actualStatus) else targetPlayer:setVitality(value) end
end

function VKSETSTATUS.Energy(targetPlayer, value)
	local actualStatus = targetPlayer:getEnergy()
	if value == 0 then targetPlayer:setEnergy(actualStatus) else targetPlayer:setEnergy(value) end
end

function VKSETSTATUS.LeaderShip(targetPlayer, value)
	local actualStatus = targetPlayer:getLeaderShip()
	if value == 0 then targetPlayer:setLeaderShip(actualStatus) else targetPlayer:setLeaderShip(value) end
end

function VKSETSTATUS.Command(aIndex, Arguments)
	local player = User.new(aIndex)
	local languagePlayer = player:getLanguage()
	
	if CheckGameMasterLevel(player:getAccountID(), player:getName(), VKSETSTATS_COMMAND_GAME_MASTER_LEVEL) == 0
	then	
		SendMessage(string.format(VKSETSTATS_COMMAND_MESSAGES[languagePlayer][1]), aIndex, 1)
		return
	end
	
	if VKSETSTATS_COMMAND_SWITCH == 0
	then
		SendMessage(string.format(VKSETSTATS_COMMAND_MESSAGES[languagePlayer][2]), aIndex, 1)
		return
	end
	
	local TargetName = command:getString(Arguments, 1, 0)
	local typeStatus = command:getNumber(Arguments, 2)
	local valueStatus = command:getNumber(Arguments, 3)
	
	if TargetName == "" then
		SendMessage(string.format(VKSETSTATS_COMMAND_MESSAGES[languagePlayer][3]), aIndex, 1)
		return
	end
	
	local TargetIndex = GetIndex(TargetName)
	
	if TargetIndex == -1 then
		SendMessage(string.format(VKSETSTATS_COMMAND_MESSAGES[languagePlayer][4], TargetName), aIndex, 1)
		return
	end
	
	local player_target = User.new(TargetIndex)
	local player_target_language = player_target:getLanguage()
	
	if valueStatus < 0
	then
		SendMessage(string.format(VKSETSTATS_COMMAND_MESSAGES[languagePlayer][5]), aIndex, 1)
		return
	end
	
	if typeStatus == 1 then
		SendMessage(string.format(VKSETSTATS_COMMAND_MESSAGES[player_target_language][6]), TargetIndex, 0)
		VKSETSTATUS.Strength(player_target, valueStatus)
		
	elseif typeStatus == 2 then
		SendMessage(string.format(VKSETSTATS_COMMAND_MESSAGES[player_target_language][7]), TargetIndex, 0)
		VKSETSTATUS.Dexterity(player_target, valueStatus)
		
	elseif typeStatus == 3 then
		SendMessage(string.format(VKSETSTATS_COMMAND_MESSAGES[player_target_language][8]), TargetIndex, 0)
		VKSETSTATUS.Vitality(player_target, valueStatus)
		
	elseif typeStatus == 4 then
		SendMessage(string.format(VKSETSTATS_COMMAND_MESSAGES[player_target_language][9]), TargetIndex, 0)
		VKSETSTATUS.Energy(player_target, valueStatus)
		
	elseif typeStatus == 5 then
		if player_target:getClass() == 4 or player_target:getClass() == 66 then
			SendMessage(string.format(VKSETSTATS_COMMAND_MESSAGES[player_target_language][10]), TargetIndex, 0)
			VKSETSTATUS.LeaderShip(player_target, valueStatus)
		else
			SendMessage(string.format(VKSETSTATS_COMMAND_MESSAGES[languagePlayer][11]), aIndex, 1)
			return
		end
	
	elseif typeStatus == 6 then
		local actualStatus = {
			CharStr = player_target:getStrength(),
			CharDex = player_target:getDexterity(),
			CharVit = player_target:getVitality(),
			CharEne = player_target:getEnergy(),
			CharCmd = player_target:getLeaderShip() }
		
		if valueStatus == 0 then player_target:setStrength(actualStatus.CharStr) else player_target:setStrength(valueStatus) end
		if valueStatus == 0 then player_target:setDexterity(actualStatus.CharDex) else player_target:setDexterity(valueStatus) end
		if valueStatus == 0 then player_target:setVitality(actualStatus.CharVit) else player_target:setVitality(valueStatus) end
		if valueStatus == 0 then player_target:setEnergy(actualStatus.CharEne) else player_target:setEnergy(valueStatus) end
		
		if player_target:getClass() == 4 or player_target:getClass() == 66 then
			if valueStatus == 0 then player_target:setLeaderShip(actualStatus.CharCmd) else player_target:setLeaderShip(valueStatus) end
		end
		
		SendMessage(string.format(VKSETSTATS_COMMAND_MESSAGES[player_target_language][12]), TargetIndex, 0)
		
	else
		SendMessage(string.format(VKSETSTATS_COMMAND_MESSAGES[languagePlayer][13]), aIndex, 1)
		return
	end
	
	SendMessage(string.format(VKSETSTATS_COMMAND_MESSAGES[languagePlayer][14], TargetName), aIndex, 1)
	CloseChar(TargetIndex, 1)
	return
end

Commands.Register(VKSETSTATS_COMMAND, VKSETSTATUS.Command)

return VKSETSTATUS
