TalkNpc = {}

function TalkNpc.Init()
	GameServerFunctions.NpcTalk(TalkNpc.NpcTalk)
	return
end

function TalkNpc.NpcTalk(Npc, aIndex)

	local m = User.new(Npc)

	local NpcClass = m:getClass()

	if NpcClass == 249 then

		ChatTargetSend(Npc, string.format("MuOnline 5.2 Season 4: www.seusite.com"), aIndex)
		return 1
	end
	if NpcClass == 255 then

		ChatTargetSend(Npc, string.format("MuOnline 5.2 Season 4: www.seusite.com"), aIndex)
		return 0
	end
	
	if NpcClass == 240 then --Npc Bau

		ChatTargetSend(Npc, string.format("MuOnline 5.2 Season 4: www.seusite.com"), aIndex)
		return 0
	end
	--if NpcClass == 240 then

	--	ChatTargetSend(Npc, string.format("Olá, guarde seus itens aqui!"), aIndex)
	--	return 0
	--end
	
	return 0
end

TalkNpc.Init()

return TalkNpc