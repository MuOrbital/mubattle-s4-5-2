vkResetBuff = {}

vkResetBuff_SWITCH = 1
vkResetBuff_COMMAND = "/vkbankill" -- /vkbankill <name> <time>

function vkResetBuff.Command(aIndex, Args)
	if vkmv_SWITCH == 0 then
		SendMessage(string.format("Comando não está ativo"), aIndex, 1)
		return
	end

	local player = User.new(aIndex)

	if CheckGameMasterLevel(player:getAccountID(), player:getName(), SETSTATS_COMMAND_GAME_MASTER_LEVEL) == 0
	then	
		SendMessage(string.format("Você não tem permissão para usar este comando."), aIndex, 1)
		return
	end

	local player_target = command:getString(Args, 1, 0)
	local ban_time = command:getNumber(Args, 2)

	local targetIndex = GetIndex(player_target)
	local targetPlayer = User.new(targetIndex)

	if targetIndex == -1 then
		SendMessage(string.format("Alvo inválido ou não está online"), aIndex, 1)
		return
	end

	KillPlayer(targetIndex)

	SendMessage(string.format("O player %s não está mais entre nós...", player_target), aIndex, 1)
	SendMessageGlobal(string.format("O player %s não está mais entre nós...", player_target), 0)

	Timer.TimeOut(8, function() CloseChar(targetIndex, 2) end)

	if ban_time > 0 then
		local QueryBan = string.format("UPDATE MEMB_INFO SET bloc_code = 1, Bloc_Expire = GETDATE() + %d WHERE memb___id = '%s'", ban_time, targetPlayer:getAccountID())
		ret = db:exec(QueryBan)		
		if ret == 0
		then
			LogAddC(2,string.format("N�o foi poss�vel executar a query: %s", QueryBan))
			db:clear()
			return
		end		
		db:clear()
	end

end

Commands.Register(vkResetBuff_COMMAND, vkResetBuff.Command)

return vkResetBuff
