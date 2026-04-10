vkmv = {}

vkmv_COMMAND = "/vkmv" -- /vkmv <type> <name>
vkmv_SWITCH = 0

function vkmv.track(aIndex, targetIndex)
	local target = User.new(targetIndex)
	Teleport(aIndex, target:getMapNumber(), target:getX(), target:getY())
end

function vkmv.trace(aIndex, targetIndex)
	local player = User.new(aIndex)
	Teleport(targetIndex, player:getMapNumber(), player:getX(), player:getY())
end

function vkmv.Command(aIndex, Args)
	local player = User.new(aIndex)

	if vkmv_SWITCH == 0 then
		SendMessage(string.format("Comando não está ativo"), aIndex, 1)
		return
	end

	if CheckGameMasterLevel(player:getAccountID(), player:getName(), SETSTATS_COMMAND_GAME_MASTER_LEVEL) == 0
	then	
		SendMessage(string.format("Você não tem permissão para usar este comando."), aIndex, 1)
		return
	end

	local type = command:getNumber(Args, 1)
	local player_target = command:getString(Args, 2, 0)

	if type ~= 1 and type ~= 2 then
		SendMessage(string.format("Comando inválido! /vkmv <type> <name>"), aIndex, 1)
		return
	end

	local targetIndex = GetIndex(player_target)

	if targetIndex == -1 then
		SendMessage(string.format("Alvo inválido ou não está online"), aIndex, 1)
		return
	end

	if type == 1 then
		vkmv.trace(aIndex, targetIndex)
		SendMessage(string.format("Player %s puxado com sucesso.", player_target), aIndex, 1)
	elseif type == 2 then
		SendMessage(string.format("Indo até o player %s.", player_target), aIndex, 1)
		vkmv.track(aIndex, targetIndex)
	end

end

Commands.Register(vkmv_COMMAND, vkmv.Command)

return vkmv
