	WarDevias = {}
	WarDevias.Info = {}
	WarDevias.Participants = {}
	WarDevias.Ranking = {
	}

	local guildAlertedPlayers = {}

	function WarDevias.Command(aIndex)
	   local player = User.new(aIndex)
	   
	   if player:getAuthority() ~= 32 then return end
	 
	   if not WarDevias.Info._Status and not WarDevias.Info._Wait then
		   SendMessageGlobal("[WarDevias]", 0)
		   SendMessageGlobal("Abertura concluída com sucesso", 0)
		   SendMessageGlobal("Vá até o Castelo de WarDevias e registre sua Guild.", 0)
		   WarDevias.Info = {}
		   WarDevias.Ranking = {}
		   WarDevias.Participants = {}
		   WarDevias.Info._NumberGuilds = 0
		   WarDevias.Info._Wait = true
		   WarDevias.CheckMonsters()
		   local timeRemaining = (60 * WarDevias_Config.TimerStart + 1)
		   Timer.TimeOut(timeRemaining, function()
			   SendMessageGlobal("[WarDevias]", 0)
			   SendMessageGlobal("O registro das Guilds foi encerrado.", 0)
			   WarDevias.Info._Wait = false
			   WarDevias.Info._Status = true
			   WarDevias.StartEvent()
		   end)
		   
		   Timer.TimeOut(timeRemaining - 60, function()
			   SendMessageGlobal("[WarDevias]", 0)
			   SendMessageGlobal("O Registro será encerrado em 1 minuto", 0)
		   end)

		   local registrationTimer
		   registrationTimer = Timer.Interval(60, function()
			   if WarDevias.Info._Status then
				   Timer.Cancel(registrationTimer)
			   elseif timeRemaining <= 60 then
				   Timer.Cancel(registrationTimer)
			   else
				   timeRemaining = timeRemaining - 60 
				   local minutesRemaining = math.floor(timeRemaining / 60)
				   if timeRemaining >= 120 then
					   SendMessageGlobal("[WarDevias]", 0)
					   SendMessageGlobal(string.format("O Registro será encerrado em %d minuto(s)", minutesRemaining), 0)
				   end
			   end
		   end)
	  else
		   SendMessage("[WarDevias] O evento já está aberto!", aIndex, 1)
	   end
	end

	function WarDevias.Cron()
		if not WarDevias.Info._Status and not WarDevias.Info._Wait then
		   SendMessageGlobal("[WarDevias]", 0)
		   SendMessageGlobal("Abertura concluída com sucesso", 0)
		   SendMessageGlobal("Vá até o Castelo de WarDevias e registre sua Guild.", 0)
		   WarDevias.Info = {}
		   WarDevias.Ranking = {}
		   WarDevias.Participants = {}
		   WarDevias.Info._NumberGuilds = 0
		   WarDevias.Info._Wait = true
		   WarDevias.CheckMonsters()
		   local timeRemaining = (60 * WarDevias_Config.TimerStart + 1)
		   Timer.TimeOut(timeRemaining, function()
			   SendMessageGlobal("[WarDevias]", 0)
			   SendMessageGlobal("O registro das Guilds foi encerrado.", 0)
			   WarDevias.Info._Wait = false
			   WarDevias.Info._Status = true
			   WarDevias.StartEvent()
		   end)
		   
		   Timer.TimeOut(timeRemaining - 60, function()
			   SendMessageGlobal("[WarDevias]", 0)
			   SendMessageGlobal("O Registro será encerrado em 1 minuto", 0)
		   end)

		   local registrationTimer
		   registrationTimer = Timer.Interval(60, function()
			   if WarDevias.Info._Status then
				   Timer.Cancel(registrationTimer)
			   elseif timeRemaining <= 60 then
				   Timer.Cancel(registrationTimer)
			   else
				   timeRemaining = timeRemaining - 60 
				   local minutesRemaining = math.floor(timeRemaining / 60)
				   if timeRemaining >= 120 then
					   SendMessageGlobal("[WarDevias]", 0)
					   SendMessageGlobal(string.format("O Registro será encerrado em %d minuto(s)", minutesRemaining), 0)
				   end
			   end
		   end)
	   else
		   LogAddC(2, "[WarDevias - Cron] Já existe um evento em andamento.")
	   end
	end

	function WarDevias.StartEvent()
		if WarDevias.Info._Status then
			if WarDevias.Info._NumberGuilds >= WarDevias_Config.NumberGuilds then
				WarDevias.Info._GuildChampionName = "No Champion"
				local Query = string.format("UPDATE Guild SET WarDevias = '0'")
				ret = db:exec(Query)

				if ret == 0 then
					LogAddC(2, string.format("[WarDevias] Não foi possível executar a query: %s", Query))
					SendMessageGlobal("[WarDevias]", 0)
					SendMessageGlobal("Evento cancelado, contate a administração.", 0)
					db:clear()
					return
				end

				db:clear()
				
				WarDevias.Ranking = {
					{ Name = "Nenhuma", Points = 0 },
					{ Name = "Nenhuma", Points = 0 },
					{ Name = "Nenhuma", Points = 0 }
				}
				
				WarDevias.Info._TimeOut = Timer.TimeOut((60*WarDevias_Config.TimerFinish), function()
					WarDevias.FinishEvent()
				end)
				
				for _, aIndex in ipairs(ConnectedPlayers) do
					local player = User.new(aIndex)
					local x = player:getX()
					local y = player:getY()
					if player:getMapNumber() == WarDevias_Config.MapEventNumber and x >= WarDevias_Config.MapEventRangeX_1 and y >= WarDevias_Config.MapEventRangeY_1
					and x <= WarDevias_Config.MapEventRangeX_2 and y <= WarDevias_Config.MapEventRangeY_2 and player:getAuthority() ~= 32 then
						WarDevias.RemovePlayer(aIndex)
					end
				end
				
			   local TimerAnnounce = (60*WarDevias_Config.TimerAnnounce)
			   local timeRemaining = (60*WarDevias_Config.TimerFinish)
			   local announceTimerId = Timer.Interval(TimerAnnounce, function()
				   if WarDevias.Info._TimeOut ~= nil then
					   timeRemaining = timeRemaining - TimerAnnounce
					   local minutesRemaining = math.floor(timeRemaining / 60)
					   if minutesRemaining >= 1 then
						   SendMessageGlobal(string.format("[WarDevias] Tempo restante: %d minuto(s).", minutesRemaining), 1)
					   else
						   Timer.Cancel(announceTimerId)
						   announceTimerId = nil
					   end
					   WarDevias.AnnounceRanking()
				   end
			   end)

				local thirtySecondsTimerId
				local thirtySeconds = 31
				Timer.TimeOut((60*WarDevias_Config.TimerFinish - 30), function()
					thirtySecondsTimerId = Timer.Interval(1, function()

						if thirtySeconds > 1 and WarDevias.Info._TimeOut ~= nil then
							SendMessageGlobal(string.format("[WarDevias] O evento terminará em %d segundo(s)", thirtySeconds-1), 1)
							thirtySeconds = thirtySeconds - 1
						else
							SendMessageGlobal(string.format("[WarDevias] Evento finalizado."), 1)
							Timer.Cancel(thirtySecondsTimerId)
							thirtySecondsTimerId = nil
						end
					end)
				end)

				local pointsInterval = WarDevias_Config.PointsInterval
				WarDevias.Info._PointsTimer = Timer.Interval(pointsInterval, WarDevias.AddPointsToGuild)
				WarDevias.Info._CountTimer = Timer.Interval(1, WarDevias.Count)
				WarDevias.CheckMonsters()
				SendMessageGlobal("", 0)
				SendMessageGlobal("[WarDevias]", 0)
				SendMessageGlobal("Evento iniciado com sucesso", 0)
				SendMessageGlobal("Iniciem os combates para dominar o Castelo.", 0)

			else
				WarDevias.FinishEvent()
			end
		end
	end

	function WarDevias.RegisterGuilds(aIndex)
		if WarDevias.Info._Wait then
			local player = User.new(aIndex)
			local guildName = player:getGuildName()
			
			if player:getAuthority() == 32 then
			   player = nil
			   return
			end
			
			if guildName == "" then return end
		
			local guildMaster = DataBase.GetString("Guild", "G_Master", "G_Name", guildName)
			
			local isGuildRegistered = false
			for _, info in pairs(WarDevias.Participants) do
				if info.Name == guildName then
					isGuildRegistered = true
					break
				end
			end
			
			local registrationAllowed = WarDevias.Info._NumberGuilds < WarDevias_Config.NumberMaxGuilds
			local registrationMessage = ""
			local registrationSuccess = false
			
			if not isGuildRegistered and registrationAllowed and guildMaster == player:getName() then
				local guildInfo = {
					Name = guildName,
					Number = player:getGuildNumber(),
					Union = DataBase.GetValue("Guild", "G_Union", "G_Name", guildName),
					Points = 0
				}
				WarDevias.Info._NumberGuilds = WarDevias.Info._NumberGuilds + 1
				WarDevias.Participants[WarDevias.Info._NumberGuilds] = guildInfo
				registrationMessage = string.format("[WarDevias] Guild %s registrada com sucesso!", guildName)
				registrationSuccess = true
			elseif isGuildRegistered and guildMaster == player:getName() then
				registrationMessage = string.format("[WarDevias] Sua guild já foi registrada para o evento.")
			elseif not registrationAllowed and guildMaster == player:getName() then
				registrationMessage = string.format("[WarDevias] Limite máximo de guilds atingido.")
			end
			
			if guildMaster == player:getName() and registrationMessage ~= "" then
				if not guildAlertedPlayers[guildMaster] then
					SendMessage(registrationMessage, aIndex, 1)
					guildAlertedPlayers[guildMaster] = true
					Timer.TimeOut(5, function()
						guildAlertedPlayers[guildMaster] = nil
					end)
				end
			end
			
			if WarDevias.Info._NumberGuilds == WarDevias_Config.NumberMaxGuilds then
				WarDevias.StartEvent()
				if WarDevias.Info._TimeOut ~= nil then
					Timer.Cancel(WarDevias.Info._TimeOut)
					WarDevias.Info._TimeOut = nil
				end
			end
			
			player = nil
		end
	end

	function WarDevias.RemovePlayer(aIndex)
		p = User.new(aIndex)
		if p:getSkin() == 55 then
			p:setSkin(-1)
			p = nil
		end

		TeleportMapRange(aIndex, WarDevias_Config.MapEventNumber, WarDevias_Config.TeleportX, WarDevias_Config.TeleportY, 5)
	end

	function WarDevias.CheckMonsters()
		if WarDevias.Info._Wait then
			for i = OBJECT_START_MONSTER, MAX_OBJECT_MONSTER do
				local monster = User.new(i)
				if monster:getMapNumber() == WarDevias_Config.MapEventNumber and monster:getX() >= WarDevias_Config.MapEventRangeX_1 - 50 and monster:getY() >= WarDevias_Config.MapEventRangeY_1 - 50 and monster:getX() <= WarDevias_Config.MapEventRangeX_2 + 50 and monster:getY() <= WarDevias_Config.MapEventRangeY_2 + 50 then
					gObjDel(i)
				end
				monster = nil
			end
		end

		if WarDevias.Info._Status then
			CreateMonster = AddMonster(WarDevias_Config.MapEventNumber)
		
			if CreateMonster == -1 then
				LogAdd(string.format("Erro ao criar o NPC WarDevias"))
				WarDevias.FinishEvent()
				return
			end

			local monster = User.new(CreateMonster)
			
			SetMapMonster(CreateMonster, WarDevias_Config.MapEventNumber, WarDevias_Config.MonsterCordX, WarDevias_Config.MonsterCordY)
			monster:setDir(2)
			SetMonster(CreateMonster, WarDevias_Config.MonsterSwitch)
			monster:setType(3)
			monster = nil
		end
	end

	function WarDevias.NpcTalk(Monster, aIndex)
		if WarDevias.Info._Status then		
			local m = User.new(Monster)
			local p = User.new(aIndex)

			if m:getMapNumber() == WarDevias_Config.MapEventNumber
			then
				if WarDevias_Config.MonsterCordX == m:getX() and WarDevias_Config.MonsterCordY == m:getY() then
					if WarDevias.Info._SwitchBlock or p:getAuthority() == 32 then
						p = nil
						m = nil
						return 0
					end
					
					if not WarDevias.Info._PlayerSwitch then
						if WarDevias.Info._GuildDominant ~= p:getGuildName() then
							WarDevias.Info._GuildSwitch = p:getGuildName()
							WarDevias.Info._PlayerSwitch = aIndex
							p:setSkin(55)
							PkLevelSend(aIndex, 3)
							ViewportCreate(aIndex)
							Teleport(aIndex, p:getMapNumber(), p:getX(), p:getY())
							WarDevias.Info._ThroneTimer = 0
							ChatTargetSend(Monster, string.format("Você capturou a Switch!!"), aIndex)
							SendMessageGlobal(string.format("Guild %s capturou a Switch!!", p:getGuildName()), 0)
							p = nil
							m = nil
						else
							SendMessage("Você já capturou a Switch! Proteja-a!", aIndex, 1)
						end
						return 0
					end
				end
			end
		end
	end

	function WarDevias.CloseCastle()
		if WarDevias.Info._Status then
			if WarDevias.Info._PlayerSwitch == nil or WarDevias.Info._GuildSwitch == nil then
				return
			end
			
			local guildChampionUnion = DataBase.GetValue("Guild", "G_Union", "G_Name", WarDevias.Info._GuildSwitch)
			
			for _, aIndex in ipairs(ConnectedPlayers) do
				local player = User.new(aIndex)
				local x = player:getX()
				local y = player:getY()

				if player:getMapNumber() == WarDevias_Config.MapEventNumber and x >= WarDevias_Config.MapEventRangeX_1 and y >= WarDevias_Config.MapEventRangeY_1
				and x <= WarDevias_Config.MapEventRangeX_2 and y <= WarDevias_Config.MapEventRangeY_2 and player:getAuthority() ~= 32 then
					local playerGuildName = player:getGuildName()
					
					if playerGuildName ~= "" then
						local playerGuildUnion = DataBase.GetValue("Guild", "G_Union", "G_Name", playerGuildName)
						if playerGuildName == WarDevias.Info._GuildSwitch or playerGuildUnion > 0 and playerGuildUnion == guildChampionUnion then
							TeleportMapRange(aIndex, WarDevias_Config.MapEventNumber, 16, 41, 2)
						else
							WarDevias.RemovePlayer(aIndex)
						end
					else
						WarDevias.RemovePlayer(aIndex)
					end
				end
				player = nil
			end
			
			SendMessageGlobal(string.format("Guild %s dominou o CASTELO", WarDevias.Info._GuildSwitch), 0)
			WarDevias.Info._GuildDominant = WarDevias.Info._GuildSwitch
			local p = User.new(WarDevias.Info._PlayerSwitch)
			p:setSkin(-1)
			ViewportCreate(WarDevias.Info._PlayerSwitch)
			WarDevias.Info._PlayerSwitch = false
			WarDevias.Info._GuildSwitch = false
			WarDevias.Info._CountCastle = false
			p = nil
			
			WarDevias.Info._SwitchBlock = true    
			Timer.TimeOut(WarDevias_Config.TimerNextRound, function()
				WarDevias.Info._SwitchBlock = false
				WarDevias.Info._PlayerSwitch = false
				WarDevias.Info._GuildSwitch = false
				WarDevias.Info._CountCastle = false
				WarDevias.Info._ThroneTimer = 0
				WarDevias.Info._LastUpdateTime = os.time()
				SendMessageGlobal("[WarDevias]", 0)
				SendMessageGlobal("A Switch foi liberada", 0)
				SendMessageGlobal("Domine e conquiste o Castelo.", 0)
			end)
		end
	end

	function WarDevias.Count()
		if WarDevias.Info._Status and WarDevias.Info._GuildSwitch and WarDevias.Info._CountCastle and not WarDevias.Info._SwitchBlock then
			if WarDevias.Info._ThroneTimer == WarDevias_Config.TimerThrone then
				WarDevias.Info._ThroneTimer = WarDevias.Info._ThroneTimer + 1
				WarDevias.CloseCastle()
			elseif WarDevias.Info._ThroneTimer < WarDevias_Config.TimerThrone then
				SendMessageGlobal(string.format("[WarDevias]"), 0)
				SendMessageGlobal(string.format("Guild %s está dominando o Castelo de WarDevias (%d|%d)", WarDevias.Info._GuildSwitch, WarDevias.Info._ThroneTimer, WarDevias_Config.TimerThrone), 0)
				WarDevias.Info._ThroneTimer = WarDevias.Info._ThroneTimer + 1
			end
		end
	end

	function WarDevias.PlayerAttack(aIndex, TargetIndex)
		if WarDevias.Info._Status then
			local p = User.new(aIndex)
			local p2 = User.new(TargetIndex)
			local guild1 = p:getGuildName()
			local guild2 = p2:getGuildName()
			local x = p:getX()
			local y = p:getY()

			if p:getMapNumber() == WarDevias_Config.MapEventNumber and x >= WarDevias_Config.MapEventRangeX_1 and y >= WarDevias_Config.MapEventRangeY_1
			and x <= WarDevias_Config.MapEventRangeX_2 and y <= WarDevias_Config.MapEventRangeY_2 then
				if p:getAuthority() == 32 or p2:getAuthority() == 32 then
				   return 1
				end
				
				if not WarDevias_Config.AllowDamageFriend then
					if guild1 ~= "" and guild2 ~= "" then
						local G_Union = DataBase.GetValue("Guild", "G_Union", "G_Name", guild1)
						local G_Union2 = DataBase.GetValue("Guild", "G_Union", "G_Name", guild2)
						if guild1 == guild2 or G_Union > 0 and G_Union == G_Union2 then
							return 1
						end
					end
				end
			end
			
			p = nil
			p2 = nil
		end
	end

	function WarDevias.PlayerDie(aIndex, TargetIndex)
		if not WarDevias.Info._Status then
			return
		end
		
		local p = User.new(aIndex)
		local p2 = User.new(TargetIndex)
		local nome1 = p:getName()
		local nome2 = p2:getName()
		local guild1 = p:getGuildName()
		local guild2 = p2:getGuildName()
		local x = p:getX()
		local y = p:getY() 

		if p:getMapNumber() == WarDevias_Config.MapEventNumber and x >= WarDevias_Config.MapEventRangeX_1 and y >= WarDevias_Config.MapEventRangeY_1
		and x <= WarDevias_Config.MapEventRangeX_2 and y <= WarDevias_Config.MapEventRangeY_2 then
			SendMessageGlobal(string.format("[WarDevias] - (%s [G]: %s) matou (%s [G]: %s)", nome1, guild1, nome2, guild2), 1)  
			SendMessage(string.format("[WarDevias] Você foi morto por %s [G]: %s", nome1, guild1), TargetIndex, 1)
		end
		
		if TargetIndex == WarDevias.Info._PlayerSwitch then
			p2:setSkin(-1)
			WarDevias.Info._PlayerSwitch = false
			WarDevias.Info._GuildSwitch = false
			WarDevias.Info._CountCastle = false
			WarDevias.Info._ThroneTimer = 0
		end

		p = nil
		p2 = nil
	end

	function WarDevias.AnnounceRanking()
		local hasValidGuild = false
		for _, guild in ipairs(WarDevias.Ranking) do
			if guild.Name ~= "Nenhuma" then
				hasValidGuild = true
				break
			end
		end
		
		if not hasValidGuild then
			return
		end
		
		SendMessageGlobal(string.format("[WarDevias Ranking]"), 0)
		for i, guild in ipairs(WarDevias.Ranking) do
			SendMessageGlobal(string.format("%dº Lugar - [G]: %s (%d)", i, guild.Name, guild.Points), 0)
		end
	end

	function WarDevias.UpdateRanking(guildName, points)
		for i, guild in ipairs(WarDevias.Ranking) do
			if guild.Name == guildName then
				if points > guild.Points then
					WarDevias.Ranking[i].Points = points
				end
				return
			end
		end
		
		table.insert(WarDevias.Ranking, { Name = guildName, Points = points })

		table.sort(WarDevias.Ranking, function(a, b) return a.Points > b.Points end)

		while #WarDevias.Ranking > 3 do
			table.remove(WarDevias.Ranking)
		end
	end

	function WarDevias.AddPointsToGuild()
		if not WarDevias.Info._Status or WarDevias.Info._GuildDominant == nil then return end
		
		local guildPoints = 0
		local guildName = ""
		for _, info in pairs(WarDevias.Participants) do
			if info.Name == WarDevias.Info._GuildDominant then
				info.Points = info.Points + WarDevias_Config.PointsValue
				guildPoints = info.Points
				guildName = info.Name
				break
			end
		end
	  
		WarDevias.UpdateRanking(guildName, guildPoints)
		SendMessageGlobal(string.format("[WarDevias]"), 0)
		SendMessageGlobal(string.format("A Guild %s está com %d pontos", guildName, guildPoints), 0)
	end

	function WarDevias.FinishEvent()
		MonsterReload()

		if WarDevias.Info._NumberGuilds < WarDevias_Config.NumberGuilds then
			SendMessageGlobal("[WarDevias]", 0)
			SendMessageGlobal("A quantidade de guildas não foi suficiente para o início do evento.", 0)
			WarDevias.Info = {}
			WarDevias.Ranking = {}
			WarDevias.Participants = {}

			if WarDevias.Info._TimeOut ~= nil then
				Timer.Cancel(WarDevias.Info._TimeOut)
				WarDevias.Info._TimeOut = nil
			end

			if WarDevias.Info._PointsTimer ~= nil then
				Timer.Cancel(WarDevias.Info._PointsTimer)
				WarDevias.Info._PointsTimer = nil
			end

			if WarDevias.Info._CountTimer ~= nil then
				Timer.Cancel(WarDevias.Info._CountTimer)
				WarDevias.Info._CountTimer = nil
			end
			
			return WarDevias.RefreshInfo()
		end

		local winnerGuilds = {}
		local winningPoints = 0
		
		for _, guildInfo in pairs(WarDevias.Participants) do
			if guildInfo.Points > winningPoints then
				winnerGuilds = {guildInfo}
				winningPoints = guildInfo.Points
			elseif guildInfo.Points == winningPoints then
				table.insert(winnerGuilds, guildInfo)
			end
		end

		local function updateGuildWarDevias(guildName)
			local Query = string.format("UPDATE Guild SET WarDevias = '1' WHERE G_Name = '%s'", guildName)
			local ret = db:exec(Query)
		
			if ret == 0 then
				LogAddC(2, string.format("[WarDevias] Não foi possível executar a query: %s", Query))
				db:clear()
			else
				db:clear()
			end
		end
		
		local function TeleportWinners(ChampionGuild)
			if winnerGuilds then
				for _, aIndex in ipairs(ConnectedPlayers) do
					local player = User.new(aIndex)
					local guildName = player:getGuildName()
					if guildName ~= "" and player:getMapNumber() == WarDevias_Config.MapEventNumber then
						local G_Union = DataBase.GetValue("Guild", "G_Union", "G_Name", guildName)
						local ChampionUnion = DataBase.GetValue("Guild", "G_Union", "G_Name", ChampionGuild)
						local MemberValue = DataBase.GetValue("GuildMember", "G_Status", "Name", player:getName())
						if (guildName == ChampionGuild or G_Union > 0 and G_Union == ChampionUnion) then
							if MemberValue == 128 then
								DataBase.SetAddValue(WarDevias_Config.Table, WarDevias_Config.Column, WarDevias_Config.GuildMasterReward, WarDevias_Config.Where, player:getAccountID())
								SendMessage(string.format("[WarDevias] - Parabéns, você recebeu %d %s(s) ", WarDevias_Config.GuildMasterReward, WarDevias_Config.CoinName),aIndex, 1)
							elseif MemberValue == 64 then
								DataBase.SetAddValue(WarDevias_Config.Table, WarDevias_Config.Column, WarDevias_Config.AssistenteReward, WarDevias_Config.Where, player:getAccountID())
								SendMessage(string.format("[WarDevias] - Parabéns, você recebeu %d %s(s) ", WarDevias_Config.AssistenteReward, WarDevias_Config.CoinName),aIndex, 1)
							elseif MemberValue == 32 then
								DataBase.SetAddValue(WarDevias_Config.Table, WarDevias_Config.Column, WarDevias_Config.MestreReward, WarDevias_Config.Where, player:getAccountID())
								SendMessage(string.format("[WarDevias] - Parabéns, você recebeu %d %s(s) ", WarDevias_Config.MestreReward, WarDevias_Config.CoinName),aIndex, 1)
							elseif MemberValue == 0 then
								DataBase.SetAddValue(WarDevias_Config.Table, WarDevias_Config.Column, WarDevias_Config.MembroReward, WarDevias_Config.Where, player:getAccountID())
								SendMessage(string.format("[WarDevias] - Parabéns, você recebeu %d %s(s) ", WarDevias_Config.MembroReward, WarDevias_Config.CoinName), aIndex, 1)
							end
							TeleportMapRange(aIndex, WarDevias_Config.MapEventNumber, WarDevias_Config.TeleportWinsCordX, WarDevias_Config.TeleportWinsCordY, 4)
							timerId = Timer.Repeater(1, 5, function()
								FireWorks(-1, WarDevias_Config.MapEventNumber, WarDevias_Config.TeleportWinsCordX + 2, WarDevias_Config.TeleportWinsCordY)
							end)
						end
					end
					player = nil
				end
			end
		end
		
		if #winnerGuilds > 1 then
			local winnerGuild = winnerGuilds[math.random(#winnerGuilds)]
			local winnerMessage = string.format("A Guild %s tornou-se dona do Castelo!!", winnerGuild.Name)
			
			SendMessageGlobal("[WarDevias]", 0)
			SendMessageGlobal("Houve um empate entre as guildas com a maior pontuação.", 0)
			SendMessageGlobal("Um sorteio será realizado para definir o vencedor do evento.", 0)
			
			local timerId
			local function sendWinnerMessage()
				SendMessageGlobal("[WarDevias]", 0)
				SendMessageGlobal(winnerMessage, 0)
				Timer.Cancel(timerId)  
				updateGuildWarDevias(winnerGuild.Name)
				TeleportWinners(winnerGuild.Name)
				WarDevias.RefreshInfo()
			end
			timerId = Timer.TimeOut(5, sendWinnerMessage)
		elseif #winnerGuilds == 1 then
			local winnerGuild = winnerGuilds[1]
			SendMessageGlobal("[WarDevias]", 0)
			SendMessageGlobal(string.format("A Guild %s tornou-se dona do Castelo!!", winnerGuild.Name), 0)
			updateGuildWarDevias(winnerGuild.Name)
			TeleportWinners(winnerGuild.Name)
		else
			SendMessageGlobal("[WarDevias]", 0)
			SendMessageGlobal(string.format("O evento terminou sem vencedores."), 0)
		end

		local function sendFireworks(aIndex, mapNumber, x, y)
			FireWorks(aIndex, mapNumber, x, y)
		end
		
		WarDevias.Info = {}
		WarDevias.Ranking = {}
		WarDevias.Participants = {}
		
		if WarDevias.Info._TimeOut ~= nil then
			Timer.Cancel(WarDevias.Info._TimeOut)
			WarDevias.Info._TimeOut = nil
		end

		if WarDevias.Info._PointsTimer ~= nil then
			Timer.Cancel(WarDevias.Info._PointsTimer)
			WarDevias.Info._PointsTimer = nil
		end

		if WarDevias.Info._CountTimer ~= nil then
			Timer.Cancel(WarDevias.Info._CountTimer)
			WarDevias.Info._CountTimer = nil
		end
		
		return WarDevias.RefreshInfo()
	end

	local switchMonsterIndex = nil
	function WarDevias.Check(aIndex)
		local player = User.new(aIndex)
		local x = player:getX()
		local y = player:getY()
		local guildName = player:getGuildName()
		
		if player:getInterfaceUse() == 1 and player:getInterfaceType() == 12 then
			player:setInterfaceUse(0)
			player:setInterfaceType(0)
		end  
		
		if player:getAuthority() == 32 then
		   player = nil
		   return
		end
		
		if WarDevias.Info._Wait and guildName ~= "" then
			WarDevias.RegisterGuilds(aIndex)
		end	

		if not WarDevias.Info._Status and WarDevias.Info._GuildChampionName == "No Champion" or WarDevias.Info._Wait then
			WarDevias.RemovePlayer(aIndex)
			SendMessage(string.format(""), aIndex, 0)
			SendMessage(string.format("O Castelo não está disponível para acesso no momento"), aIndex, 0)
			player = nil
		elseif not WarDevias.Info._Status and WarDevias.Info._GuildChampionName ~= nil and not WarDevias.Info._Wait then
			if guildName == "" then
				WarDevias.RemovePlayer(aIndex)
				SendMessage(string.format("Castelo pertence a [G]: %s", WarDevias.Info._GuildChampionName), aIndex, 0)
				player = nil
				return 
			end
			
			local G_Union = DataBase.GetValue("Guild", "G_Union", "G_Name", guildName)
			local ChampionUnion = DataBase.GetValue("Guild", "G_Union", "G_Name", WarDevias.Info._GuildChampionName)
			if (guildName ~= WarDevias.Info._GuildChampionName and G_Union == 0 or G_Union ~= ChampionUnion) then
				WarDevias.RemovePlayer(aIndex)
				SendMessage(string.format("Castelo pertence a [G]: %s", WarDevias.Info._GuildChampionName), aIndex, 0)
			end

		elseif WarDevias.Info._Status then
		
			if guildName == "" then
				WarDevias.RemovePlayer(aIndex)
				SendMessage(string.format("[WarDevias]"), aIndex, 0)
				SendMessage(string.format("Você precisa estar em uma para participar do evento."), aIndex, 0)
				player = nil
				return
			end
			
			local playerAccess = false
			local guildMaster = DataBase.GetString("Guild", "G_Master", "G_Name", guildName)
			local G_Union = DataBase.GetValue("Guild", "G_Union", "G_Name", guildName)
			
			for _, participant in ipairs(WarDevias.Participants) do
				if G_Union and participant.Union and (G_Union > 0 and G_Union == participant.Union) then
					playerAccess = true
					break
				elseif player:getGuildNumber() and participant.Number and (player:getGuildNumber() == participant.Number) then
					playerAccess = true
					break
				end
			end

			if not playerAccess then
				WarDevias.RemovePlayer(aIndex)
				SendMessage(string.format("[WarDevias]"), aIndex, 0)
				SendMessage(string.format("Você não tem permissão para participar do evento."), aIndex, 0)
			end
			
			if x == WarDevias_Config.MapCordXThrone and y == WarDevias_Config.MapCordYThrone then
				if guildMaster == player:getName() then
					if WarDevias.Info._GuildSwitch ~= guildName then
						SendMessage(string.format("[WarDevias]"), aIndex, 0)
						SendMessage(string.format("Sua Guild não tem o controle da Switch"), aIndex, 0)
						SendMessage(string.format(""), aIndex, 0)
					else
						WarDevias.Info._CountCastle = true
					end
				else
					SendMessage(string.format("[WarDevias]"), aIndex, 0)
					SendMessage(string.format("Apenas o Game Master de sua guild pode dominar o trono"), aIndex, 0)
					SendMessage(string.format(""), aIndex, 0)
				end
			else
				if guildMaster == player:getName() and WarDevias.Info._GuildSwitch == guildName then
					WarDevias.Info._CountCastle = false
					WarDevias.Info._ThroneTimer = 0
				end
			end
			
			if not WarDevias.Info._PlayerSwitch then
				if switchMonsterIndex == nil then
					for i = OBJECT_START_MONSTER, MAX_OBJECT_MONSTER do
						local monster = User.new(i)
						if monster:getConnected() == 3 and monster:getX() == WarDevias_Config.MonsterCordX and monster:getY() == WarDevias_Config.MonsterCordY then
							switchMonsterIndex = i
							break
						end
						monster = nil
					end
				elseif switchMonsterIndex ~= nil then
					local monster = User.new(switchMonsterIndex)
					if WarDevias.Info._SwitchBlock then
						ChatTargetSend(switchMonsterIndex, "A Switch está bloqueada, aguarde a liberação!", aIndex)
					else
						ChatTargetSend(switchMonsterIndex, "Switch está liberada e ainda não foi capturada!", aIndex)
					end
					monster = nil
				end
			else
				local p = User.new(WarDevias.Info._PlayerSwitch)
				if switchMonsterIndex == nil then
					for i = OBJECT_START_MONSTER, MAX_OBJECT_MONSTER do
						local monster = User.new(i)
						if monster:getConnected() == 3 and monster:getX() == WarDevias_Config.MonsterCordX and monster:getY() == WarDevias_Config.MonsterCordY then
							switchMonsterIndex = i
							break
						end
						monster = nil
					end
				end
				
				if switchMonsterIndex ~= nil then
						ChatTargetSend(switchMonsterIndex, string.format("Switch capturada pelo jogador %s [G]: %s", p:getName(), p:getGuildName()), aIndex)
				end
			
				if p:getMapNumber() == WarDevias_Config.MapEventNumber and (p:getX() < WarDevias_Config.PlayerRangeSwitchX_1 or p:getX() > WarDevias_Config.PlayerRangeSwitchX_2 
				or p:getY() < WarDevias_Config.PlayerRangeSwitchY_1 or p:getY() > WarDevias_Config.PlayerRangeSwitchY_2) then
					p:setSkin(-1)
					ViewportCreate(WarDevias.Info._PlayerSwitch)
					WarDevias.Info._PlayerSwitch = false
					WarDevias.Info._GuildSwitch = false
					WarDevias.Info._CountCastle = false
					WarDevias.Info._ThroneTimer = 0
				end
			end
		end
		player = nil
	end

	function WarDevias.UserMap()
		for _, aIndex in ipairs(ConnectedPlayers) do
			local player = User.new(aIndex)
			if player:getConnected() == 3 and player:getMapNumber() == WarDevias_Config.MapEventNumber then
				local x = player:getX()
				local y = player:getY()
				if x >= WarDevias_Config.MapEventRangeX_1 and y >= WarDevias_Config.MapEventRangeY_1
				and x <= WarDevias_Config.MapEventRangeX_2 and y <= WarDevias_Config.MapEventRangeY_2 and player:getAuthority() ~= 32 then
					WarDevias.Check(aIndex)
				end
			end
			player = nil
		end
	end

	function WarDevias.RefreshInfo()
		local query = "SELECT TOP 1 WarDevias, G_Name, G_Union FROM Guild ORDER BY WarDevias DESC"
		local ret = db:exec(query)

		if ret == 0 then
			WarDevias.Info._GuildChampionName = "No Champion"
			LogAddC(2, string.format("Falha ao executar a consulta: %s", query))
			db:clear()
			return
		end

		local nRet = db:fetch()
		if nRet == SQL_NO_DATA then
			WarDevias.Info._GuildChampionName = "No Champion"
			LogAddC(2, string.format("Nenhum dado retornado pela consulta: %s", query))
			db:clear()
			return
		end

		local warDeviasNumber = db:getInt("WarDevias")
		local guildChampionName = db:getStr("G_Name")
		db:clear()
		
		if warDeviasNumber <= 0 then
			WarDevias.Info._GuildChampionName = "No Champion"
			return
		end

		WarDevias.Info._GuildChampionName = guildChampionName
	end

	function WarDevias.Init()
		if WarDevias_Config.Enable then
			Timer.TimeOut(5, WarDevias.RefreshInfo)
			Timer.Interval(1, WarDevias.UserMap)
			GameServerFunctions.NpcTalk(WarDevias.NpcTalk)
			GameServerFunctions.PlayerAttack(WarDevias.PlayerAttack)
			GameServerFunctions.PlayerDie(WarDevias.PlayerDie)
			Commands.Register(WarDevias_Config.Command, WarDevias.Command)
			
			for index, cron in ipairs(WarDevias_Config.Cron) do
				cron.index = index
				if cron.dayofweek ~= -1 then
					Schedule.SetDayOfWeek(cron.dayofweek, cron.hour, cron.minute, WarDevias.Cron, index)
				elseif cron.day ~= -1 then
					Schedule.SetDayAndHourAndMinute(cron.day, cron.hour, cron.minute, WarDevias.Cron, index)
				else
					Schedule.SetHourAndMinute(cron.hour, cron.minute, WarDevias.Cron, index)
				end
			end
		end	
	end


	WarDevias.Init()

	return WarDevias