SetPlayer = {}


function SetPlayer.CommandSetLevelPoint(aIndex, Arguments)
    local player = User.new(aIndex)
	
	if player:getAuthority() ~= 32 and CheckGameMasterLevel(player:getAccountID(), player:getName(), SET_PLAYER_COMMAND_GAME_MASTER_LEVEL) == 0
	then
		return
	end

    local Language = player:getLanguage()

    local Name = command:getString(Arguments, 1, 0)
	local TargetIndex = GetIndex(Name)
	
	if TargetIndex == -1
	then
		SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][1], Name), aIndex, 1)
		return
	end

    local value = command:getNumber(Arguments, 2)

    if value <= 0
    then
        SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][2]), aIndex, 1)
        return
    end

	local player_target = User.new(TargetIndex)

    player_target:setLevelUpPoint(value)

    CalCharacter(TargetIndex)
	RefreshCharacter(TargetIndex)
    
    SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][3], Name), aIndex, 1)
end

function SetPlayer.CommandSetStrength(aIndex, Arguments)
    local player = User.new(aIndex)
	
	if player:getAuthority() ~= 32 and CheckGameMasterLevel(player:getAccountID(), player:getName(), SET_PLAYER_COMMAND_GAME_MASTER_LEVEL) == 0
	then
		return
	end

    local Language = player:getLanguage()

    local Name = command:getString(Arguments, 1, 0)
	local TargetIndex = GetIndex(Name)
	
	if TargetIndex == -1
	then
		SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][1], Name), aIndex, 1)
		return
	end

    local value = command:getNumber(Arguments, 2)

    if value <= 0
    then
        SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][2]), aIndex, 1)
        return
    end

	local player_target = User.new(TargetIndex)

    player_target:setStrength(value)

    CalCharacter(TargetIndex)
	RefreshCharacter(TargetIndex)
    
    SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][3], Name), aIndex, 1)
end

function SetPlayer.CommandSetAgility(aIndex, Arguments)
    local player = User.new(aIndex)
	
	if player:getAuthority() ~= 32 and CheckGameMasterLevel(player:getAccountID(), player:getName(), SET_PLAYER_COMMAND_GAME_MASTER_LEVEL) == 0
	then
		return
	end

    local Language = player:getLanguage()

    local Name = command:getString(Arguments, 1, 0)
	local TargetIndex = GetIndex(Name)
	
	if TargetIndex == -1
	then
		SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][1], Name), aIndex, 1)
		return
	end

    local value = command:getNumber(Arguments, 2)

    if value <= 0
    then
        SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][2]), aIndex, 1)
        return
    end

	local player_target = User.new(TargetIndex)

    player_target:setDexterity(value)

    CalCharacter(TargetIndex)
	RefreshCharacter(TargetIndex)
    
    SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][3], Name), aIndex, 1)
end

function SetPlayer.CommandSetVitality(aIndex, Arguments)
    local player = User.new(aIndex)
	
	if player:getAuthority() ~= 32 and CheckGameMasterLevel(player:getAccountID(), player:getName(), SET_PLAYER_COMMAND_GAME_MASTER_LEVEL) == 0
	then
		return
	end

    local Language = player:getLanguage()

    local Name = command:getString(Arguments, 1, 0)
	local TargetIndex = GetIndex(Name)
	
	if TargetIndex == -1
	then
		SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][1], Name), aIndex, 1)
		return
	end

    local value = command:getNumber(Arguments, 2)

    if value <= 0
    then
        SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][2]), aIndex, 1)
        return
    end

	local player_target = User.new(TargetIndex)

    player_target:setVitality(value)

    CalCharacter(TargetIndex)
	RefreshCharacter(TargetIndex)
    
    SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][3], Name), aIndex, 1)
end

function SetPlayer.CommandSetEnergy(aIndex, Arguments)
    local player = User.new(aIndex)
	
	if player:getAuthority() ~= 32 and CheckGameMasterLevel(player:getAccountID(), player:getName(), SET_PLAYER_COMMAND_GAME_MASTER_LEVEL) == 0
	then
		return
	end

    local Language = player:getLanguage()

    local Name = command:getString(Arguments, 1, 0)
	local TargetIndex = GetIndex(Name)
	
	if TargetIndex == -1
	then
		SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][1], Name), aIndex, 1)
		return
	end

    local value = command:getNumber(Arguments, 2)

    if value <= 0
    then
        SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][2]), aIndex, 1)
        return
    end

	local player_target = User.new(TargetIndex)

    player_target:setEnergy(value)

    CalCharacter(TargetIndex)
	RefreshCharacter(TargetIndex)
    
    SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][3], Name), aIndex, 1)
end

function SetPlayer.CommandSetCmd(aIndex, Arguments)
    local player = User.new(aIndex)
	
	if player:getAuthority() ~= 32 and CheckGameMasterLevel(player:getAccountID(), player:getName(), SET_PLAYER_COMMAND_GAME_MASTER_LEVEL) == 0
	then
		return
	end

    local Language = player:getLanguage()

    local Name = command:getString(Arguments, 1, 0)
	local TargetIndex = GetIndex(Name)
	
	if TargetIndex == -1
	then
		SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][1], Name), aIndex, 1)
		return
	end

    local value = command:getNumber(Arguments, 2)

    if value <= 0
    then
        SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][2]), aIndex, 1)
        return
    end

	local player_target = User.new(TargetIndex)

    player_target:setLeaderShip(value)

    CalCharacter(TargetIndex)
	RefreshCharacter(TargetIndex)
    
    SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][3], Name), aIndex, 1)
end

function SetPlayer.CommandSetMasterLevel(aIndex, Arguments)
    local player = User.new(aIndex)
	
	if player:getAuthority() ~= 32 and CheckGameMasterLevel(player:getAccountID(), player:getName(), SET_PLAYER_COMMAND_GAME_MASTER_LEVEL) == 0
	then
		return
	end

    local Language = player:getLanguage()

    local Name = command:getString(Arguments, 1, 0)
	local TargetIndex = GetIndex(Name)
	
	if TargetIndex == -1
	then
		SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][1], Name), aIndex, 1)
		return
	end

    local value = command:getNumber(Arguments, 2)

    if value <= 0
    then
        SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][2]), aIndex, 1)
        return
    end

	local player_target = User.new(TargetIndex)

    player_target:setMasterLevel(value)

    CalCharacter(TargetIndex)
	MasterLevelInfoSend(TargetIndex)
    
    SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][3], Name), aIndex, 1)
end

function SetPlayer.CommandSetMasterPoint(aIndex, Arguments)
    local player = User.new(aIndex)
	
	if player:getAuthority() ~= 32 and CheckGameMasterLevel(player:getAccountID(), player:getName(), SET_PLAYER_COMMAND_GAME_MASTER_LEVEL) == 0
	then
		return
	end

    local Language = player:getLanguage()

    local Name = command:getString(Arguments, 1, 0)
	local TargetIndex = GetIndex(Name)
	
	if TargetIndex == -1
	then
		SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][1], Name), aIndex, 1)
		return
	end

    local value = command:getNumber(Arguments, 2)

    if value <= 0
    then
        SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][2]), aIndex, 1)
        return
    end

	local player_target = User.new(TargetIndex)

    player_target:setMasterPoint(value)

    CalCharacter(TargetIndex)
	MasterLevelInfoSend(TargetIndex)
    
    SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][3], Name), aIndex, 1)
end

function SetPlayer.CommandSetEffect(aIndex, Arguments)
    local player = User.new(aIndex)
	
	if player:getAuthority() ~= 32 and CheckGameMasterLevel(player:getAccountID(), player:getName(), SET_PLAYER_COMMAND_GAME_MASTER_LEVEL) == 0
	then
		return
	end

    local Language = player:getLanguage()

    local Name = command:getString(Arguments, 1, 0)
	local TargetIndex = GetIndex(Name)
	
	if TargetIndex == -1
	then
		SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][1], Name), aIndex, 1)
		return
	end

    local value = command:getNumber(Arguments, 2)

    if value <= 0
    then
        SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][2]), aIndex, 1)
        return
    end

    local timer = command:getNumber(Arguments, 3)

    if timer <= 0
    then
        return
    end

    AddEffect(TargetIndex, 1, value, os.time() + (timer * 60), 0, 0, 0, 0)

    CalCharacter(TargetIndex)
	RefreshCharacter(TargetIndex)
    
    SendMessage(string.format(SET_PLAYER_COMMAND_MESSAGES[Language][3], Name), aIndex, 1)
end

function SetPlayer.Init()
    if SET_PLAYER_COMMAND_SWITCH ~= 1
    then
        return
    end

    Commands.Register(SET_PLAYER_COMMAND_LEVELPOINT_SYNTAX, SetPlayer.CommandSetLevelPoint)
    Commands.Register(SET_PLAYER_COMMAND_STRENGTH_SYNTAX, SetPlayer.CommandSetStrength)
    Commands.Register(SET_PLAYER_COMMAND_AGILITY_SYNTAX, SetPlayer.CommandSetAgility)
    Commands.Register(SET_PLAYER_COMMAND_VITALITY_SYNTAX, SetPlayer.CommandSetVitality)
    Commands.Register(SET_PLAYER_COMMAND_ENERGY_SYNTAX, SetPlayer.CommandSetEnergy)
    Commands.Register(SET_PLAYER_COMMAND_COMMAND_SYNTAX, SetPlayer.CommandSetCmd)
    Commands.Register(SET_PLAYER_COMMAND_MASTERLEVEL_SYNTAX, SetPlayer.CommandSetMasterLevel)
    Commands.Register(SET_PLAYER_COMMAND_MASTERPOINT_SYNTAX, SetPlayer.CommandSetMasterPoint)
    Commands.Register(SET_PLAYER_COMMAND_EFFECT_SYNTAX, SetPlayer.CommandSetEffect)
end

SetPlayer.Init()

return SetPlayer