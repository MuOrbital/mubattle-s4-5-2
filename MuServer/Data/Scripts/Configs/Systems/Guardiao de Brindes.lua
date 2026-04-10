GuardiaoBrindes = {}
GuardiaoBrindes.Npc = {}

function GuardiaoBrindes.Init()
	if GuardiaoBrindes_Config.Enabled then
		GameServerFunctions.NpcTalk(GuardiaoBrindes.NpcTalk)
		GameServerFunctions.GameServerProtocol(GuardiaoBrindes.Protocol)
		GameServerFunctions.MonsterReload(GuardiaoBrindes.MonsterReload)
		Timer.TimeOut(2, GuardiaoBrindes.CriarNpc)
		return
	end
end

function GuardiaoBrindes.LuaFirstLoad()
	local count = 1
	for aIndex = 1, 12000 do
		local p = User.new(aIndex)
		if p:getConnected() == 3 then
			for i in ipairs(GuardiaoBrindes_Config.Npc) do
				local npc = GuardiaoBrindes_Config.Npc[i]
				if npc.class == p:getClass() then
					if npc.map == p:getMapNumber() then
						if npc.x == p:getX() and npc.y == p:getY() then
							table.insert(GuardiaoBrindes.Npc, count)
							GuardiaoBrindes.Npc[count] = aIndex
							p:setType(3)
							count = count + 1
						end
					end
				end
			end
		end
	end	

	if #GuardiaoBrindes.Npc == 0 then
		return true
	else
		return false
	end
end

function GuardiaoBrindes.MonsterReload()
	GuardiaoBrindes.Npc = {}
	local count = 1
	for i in ipairs(GuardiaoBrindes_Config.Npc) do
		local npc = GuardiaoBrindes_Config.Npc[i]
		local index = AddMonster(npc.map)		
		if index == -1 then
			goto continue
		end
		local mob = User.new(index)
		SetMapMonster(index, npc.map, npc.x, npc.y)
		mob:setDir(npc.dir)
		SetMonster(index, npc.class)
		mob:setType(3)
		table.insert(GuardiaoBrindes.Npc, count)
		GuardiaoBrindes.Npc[count] = index
		 count = count+1
		::continue::
	end
end

function GuardiaoBrindes.CriarNpc()	
	local count = 1
	if GuardiaoBrindes.LuaFirstLoad() then
		GuardiaoBrindes.Npc = {}
		for i in ipairs(GuardiaoBrindes_Config.Npc) do
			local npc = GuardiaoBrindes_Config.Npc[i]
			local index = AddMonster(npc.map)		
			if index == -1 then
				goto continue
			end
			local mob = User.new(index)
			SetMapMonster(index, npc.map, npc.x, npc.y)
			mob:setDir(npc.dir)
			SetMonster(index, npc.class)
			mob:setType(3)
			table.insert(GuardiaoBrindes.Npc, count)
			GuardiaoBrindes.Npc[count] = index
			count = count+1
			::continue::
		end
	end
end

function GuardiaoBrindes.NpcTalk(Npc, aIndex)
	local m = User.new(Npc)
	local class = m:getClass()
	for i in ipairs(GuardiaoBrindes.Npc) do
		if GuardiaoBrindes.Npc[i] == Npc then
			local p = User.new(aIndex)
			local name = p:getName()
			local packetname = string.format("1-%s-%d", name, aIndex)
			p:setInterfaceUse(1)
			CreatePacket(packetname, GuardiaoBrindes_Config.Packet)
			SendPacket(packetname, aIndex)
			ClearPacket(packetname)
			return 1
		end
	end
	
	return 0
end

function GuardiaoBrindes.Protocol(aIndex, Packet, PacketName)
	if Packet ~= GuardiaoBrindes_Config.Packet then return false end
	local p = User.new(aIndex)
	local nome = p:getName()

	if PacketName == string.format("2-%s-%d", nome, aIndex) then
		ClearPacket(PacketName)
		p:setInterfaceUse(0)
		return true
	end

	if PacketName == string.format("3-%s-%d", nome, aIndex) then
		ClearPacket(PacketName)
		GuardiaoBrindes.SendList(p)
		return true
	end

	if PacketName == string.format("5-%s-%d", nome, aIndex) then
		local len = GetBytePacket(PacketName, -1)
		local codigo = GetCharPacketLength(PacketName, -1, len)
		ClearPacket(PacketName)
		GuardiaoBrindes.SendItemList(p, codigo, 0)
		return true
	end

	if PacketName == string.format("7-%s-%d", nome, aIndex) then
		local len = GetBytePacket(PacketName, -1)
		local codigo = GetCharPacketLength(PacketName, -1, len)
		local itemid = GetDwordPacket(PacketName, -1)
		ClearPacket(PacketName)
		GuardiaoBrindes.RetirarItem(p, codigo, itemid)
		return true
	end
end

function GuardiaoBrindes.RetirarItem(p, codigo, itemid)
	local Query = string.format("SELECT Section, Id, Level, Durabilidade, Skill, Luck, Opt, Excelente, Dias FROM NPCPremio WHERE Codigo = '%s' AND ListID = %d", codigo, itemid)
	local ret = db:exec(Query)
	
	if ret == 0 then
		db:clear()
		GuardiaoBrindes.SendItemList(p, codigo, 2)
		return
	end
	
	local nFecth = db:fetch()

	if nFecth == 100 then
		db:clear()
		GuardiaoBrindes.SendItemList(p, codigo, 2)
		return
	end
	local Section = db:getInt("Section")
	local Id = db:getInt("Id")
	local Level = db:getInt("Level")
	local Durabilidade = db:getInt("Durabilidade")
	local Skill = db:getInt("Skill")
	local Luck = db:getInt("Luck")
	local Opt = db:getInt("Opt")
	local Excelente = db:getInt("Excelente")
	local Dias = db:getInt("Dias")

	db:clear()
	local itemindex = GET_ITEM(Section, Id)
	if InventoryCheckSpaceByItem(p:getIndex(), itemindex) == 0 then
		local packetname = string.format("10-%s-%d", p:getName(), p:getIndex())
		CreatePacket(packetname, GuardiaoBrindes_Config.Packet)
		SendPacket(packetname, p:getIndex())
		ClearPacket(packetname)
		return
	end

	db:exec(string.format("DELETE FROM NPCPremio WHERE Codigo = '%s' AND ListID = %d", codigo, itemid))
	db:clear()

	ItemSerialCreateComplete(p:getIndex(), 236, 0, 0, itemindex, Level, Durabilidade, Skill, Luck, Opt, p:getIndex(), Excelente, 0, Dias)

	GuardiaoBrindes.SendItemList(p, codigo, 1)
end

function GuardiaoBrindes.SendItemList(p, codigo, tipo)
	local Query = string.format("SELECT ListID, Nome, Section, Id, Level, Durabilidade, Skill, Luck, Opt, Excelente FROM NPCPremio WHERE Codigo = '%s' ORDER BY ListID ASC", codigo)
	local ret = db:exec(Query)
	
	if ret == 0 then
		db:clear()
		db:exec(string.format("DELETE FROM NPCPremio_Codigos WHERE Codigo = '%s'", codigo))
		db:clear()
		db:exec(string.format("DELETE FROM NPCPremio WHERE Codigo = '%s'", codigo))
		db:clear()
		local packetname = string.format("8-%s-%d", p:getName(), p:getIndex())
		CreatePacket(packetname, GuardiaoBrindes_Config.Packet)
		SendPacket(packetname, p:getIndex())
		ClearPacket(packetname)
		return
	end
	
	local nFecth = db:fetch()
	
	local t = {}
	while nFecth ~= 100 do
		local ListID = db:getInt("ListID")
		local Nome = db:getStr("Nome")
		local Section = db:getInt("Section")
		local Id = db:getInt("Id")
		local Level = db:getInt("Level")
		local Durabilidade = db:getInt("Durabilidade")
		local Skill = db:getInt("Skill")
		local Luck = db:getInt("Luck")
		local Opt = db:getInt("Opt")
		local Excelente = db:getInt("Excelente")
		table.insert(t, {ListID = ListID, Nome = Nome, Section = Section, Id = Id, Level = Level, Durabilidade = Durabilidade, Skill = Skill, Luck = Luck, Opt = Opt, Excelente = Excelente})
		nFecth = db:fetch()		
	end
	db:clear()
	if #t == 0 then
		db:exec(string.format("DELETE FROM NPCPremio_Codigos WHERE Codigo = '%s'", codigo))
		db:clear()
		db:exec(string.format("DELETE FROM NPCPremio WHERE Codigo = '%s'", codigo))
		db:clear()
		local packetname = string.format("8-%s-%d", p:getName(), p:getIndex())
		CreatePacket(packetname, GuardiaoBrindes_Config.Packet)
		SendPacket(packetname, p:getIndex())
		ClearPacket(packetname)
		return
	end

	local packetname = string.format("6-%s-%d", p:getName(), p:getIndex())
	CreatePacket(packetname, GuardiaoBrindes_Config.Packet)
	SetBytePacket(packetname, #t)
	for i in ipairs(t) do
		SetDwordPacket(packetname, t[i].ListID)
		local lenNome = string.len(t[i].Nome)
		SetBytePacket(packetname, lenNome)
		SetCharPacketLength(packetname, t[i].Nome, lenNome)
		local indexitem = GET_ITEM(t[i].Section, t[i].Id)
		SetWordPacket(packetname, indexitem)
		SetBytePacket(packetname, t[i].Level)
		SetBytePacket(packetname, t[i].Durabilidade)
		SetBytePacket(packetname, t[i].Skill)
		SetBytePacket(packetname, t[i].Luck)
		SetBytePacket(packetname, t[i].Opt)
		SetBytePacket(packetname, t[i].Excelente)
		local x, y, w, h = 350, 100, 0, 0
		if GuardiaoBrindes_Config.Position[indexitem] ~= nil then
			x, y, w, h = GuardiaoBrindes_Config.Position[indexitem].X, GuardiaoBrindes_Config.Position[indexitem].Y, GuardiaoBrindes_Config.Position[indexitem].W, GuardiaoBrindes_Config.Position[indexitem].H
		end		
		SetWordPacket(packetname, x)
		SetWordPacket(packetname, y)
		SetWordPacket(packetname, w)
		SetWordPacket(packetname, h)
	end
	SetBytePacket(packetname, tipo)
	SendPacket(packetname, p:getIndex())
	ClearPacket(packetname)

	return
end

function GuardiaoBrindes.SendList(p)
	local acc = p:getAccountID()
	local nome = p:getName()
	local Lang = p:getLanguage()
	local Query = string.format("SELECT Codigo, NomePremio, DATEDIFF(SECOND, '1970-01-01', DataFim) as DataFim, Validade FROM NPCPremio_Codigos WHERE AccountID = '%s' ORDER BY NomePremio ASC", acc)
	local ret = db:exec(Query)
	
	if ret == 0 then
		db:clear()
		local packetname = string.format("9-%s-%d", p:getName(), p:getIndex())
		CreatePacket(packetname, GuardiaoBrindes_Config.Packet)
		SendPacket(packetname, p:getIndex())
		ClearPacket(packetname)
		return
	end
	
	local nFecth = db:fetch()
	if nFecth == 100 then
		db:clear()
		local packetname = string.format("9-%s-%d", p:getName(), p:getIndex())
		CreatePacket(packetname, GuardiaoBrindes_Config.Packet)
		SendPacket(packetname, p:getIndex())
		ClearPacket(packetname)
		return
	end
	
	local t = {}
	while nFecth ~= 100 do	
		local Code = db:getStr("Codigo")
		local nomePremio = db:getStr("NomePremio")
		local vencimento = db:getInt("Validade")
		local data = db:getInt("DataFim")
		table.insert(t, {Codigo = Code, Name = nomePremio, TemValidade = vencimento, Data = data})
		nFecth = db:fetch()
	end
	
	db:clear()
	local packetname = string.format("4-%s-%d", nome, p:getIndex())
	CreatePacket(packetname, GuardiaoBrindes_Config.Packet)
	SetDwordPacket(packetname, #t)
	for i in ipairs(t) do
		local lenCodigo = string.len(t[i].Codigo)
		local lenNome = string.len(t[i].Name)
		SetBytePacket(packetname, lenCodigo)
		SetCharPacketLength(packetname, t[i].Codigo, lenCodigo)
		
		SetBytePacket(packetname, lenNome)
		SetCharPacketLength(packetname, t[i].Name, lenNome)
		SetBytePacket(packetname, t[i].TemValidade)
		SetDwordPacket(packetname, t[i].Data)
	end
	SendPacket(packetname, p:getIndex())
	ClearPacket(packetname)
end

function GuardiaoBrindes.RegistrarCodigo(NomePremio, Acc, Codigo, Vencimento, Dias)
	local Query = string.format("INSERT INTO NPCPremio_Codigos (AccountID, Codigo, NomePremio, DataInicio, DataFim, Validade) VALUEs ('%s', '%s', '%s', GETDATE(), (GETDATE()+%d), %d)", Acc, Codigo, NomePremio, Dias, Vencimento)
	local ret = db:exec(Query)
	if ret == 0 then
		LogAddC(2,string.format("Query Error: %s", Query))
	end
	db:clear()
end

function GuardiaoBrindes.RegistrarItens(Codigo, Itens)
	for i in ipairs(Itens) do
		local Query = string.format("INSERT INTO NPCPremio (Nome, Codigo, Section, Id, Level, Durabilidade, Skill, Luck, Opt, Excelente, Dias) VALUES ('%s', '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d)", Itens[i].Nome, Codigo, Itens[i].Section, Itens[i].Id, Itens[i].Level, Itens[i].Durabilidade, Itens[i].Skill, Itens[i].Luck, Itens[i].Opt, Itens[i].Excelente, Itens[i].Dias)
		local ret = db:exec(Query)
		if ret == 0 then
			LogAddC(2,string.format("Query Error: %s", Query))
		end
		db:clear()
	end
end



function GuardiaoBrindes.RegistrarItem(Codigo, Item)
	local Query = string.format("INSERT INTO NPCPremio (Nome, Codigo, Section, Id, Level, Durabilidade, Skill, Luck, Opt, Excelente, Dias) VALUES ('%s', '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d)", Item.Nome, Codigo, Item.Section, Item.Id, Item.Level, Item.Durabilidade, Item.Skill, Item.Luck, Item.Opt, Item.Excelente, Item.Dias)
	local ret = db:exec(Query)
	if ret == 0 then
		LogAddC(2,string.format("Query Error: %s", Query))
	end
	db:clear()
end

function GuardiaoBrindes.GerarCodigo()
	local Query = string.format("SELECT dbo.NPCPremio_GerarCodigo() as Codigo")
	local ret = db:exec(Query)
	
	if ret == 0 then
		LogAddC(2, string.format("Query Error: %s", Query))
		db:clear()
		return "-1"
	end
	
	local nFecth = db:fetch()
	if nFecth == 100 then
		LogAddC(2, string.format("Query Error: %s", Query))
		db:clear()
		return "-1"
	end
	
	local codigo = ""
	codigo = db:getStr("Codigo")
	db:clear()
	return codigo
end

GuardiaoBrindes.Init()

return GuardiaoBrindes