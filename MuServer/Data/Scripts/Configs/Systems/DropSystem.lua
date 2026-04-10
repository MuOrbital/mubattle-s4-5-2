DropSystem = {}

--Configuração sistema de drop
CONFIG_DROP_SWITCH = 0

--[[
	DROP_MOEDA_TABLE = Tabela onde  é inserido a moedaColumn
	DROP_MOEDA_WHERE = nome da coluna para comparar o Login
	DROP_MOEDA_CONFIG = Configurações da moeda, começar em 1 e seguir sequencia
		nomeMoeda = O nome que será apresentado pro player
		moedaColumn = O nome da coluna da moeda no SQL
]]--
DROP_MOEDA_TABLE = "CashShopData" --Não Mexer
DROP_MOEDA_WHERE = "AccountID" --Não Mexer
DROP_MOEDA_CONFIG = {}
DROP_MOEDA_CONFIG[1] = {nomeMoeda = "WcoinC", moedaColumn = "WCoinC"}
DROP_MOEDA_CONFIG[2] = {nomeMoeda = "GoblinPoint", moedaColumn = "GoblinPoint"}

--Index do drop
--Nome da Box (aparece no global)
--Seção da Box no item.txt
--Index da Box no item.txt
--Level da Box (-1 qualquer level)
--ItemInventory (1 para dropar no inventario, 0 para dropar no chao)
--Porcentagem - 1 para usar porcentagem, 0 para ser aleatorio.
--Type (1 Dropa item, 2 Portal)

--14 55, 14 56, 14 213, 14 57, 12 32,

CONFIG_DROP_SYSTEM = { 
{_Index = 1, _NomeBox = "Box Wing", _BoxSection = 12, _BoxIndex = 32, _Level = 0, ItemInventory = 0, _Porcentagem = 1, _type = 1},
}

-- Index (deixe o mesmo do CONFIG_DROP_SYSTEM)
-- Nome do item
-- Seção do item
-- Index do item
-- SetCompleto - 1 dropa o set completo (colocar somente o helm), 0 dropa somente o item configurado.(lembre de deixar o KitAneis e KitSword 0)
-- KitSword - 1 para dropar um shield (configurar somente a sword/mace)(lembre de deixar o SetCompleto e KitAneis 0)
-- Index do shield caso KitSword esteja 1
-- KitAneis - 1 dropa o kit anel completo (colocar somente o pendant)(lembre de deixar o SetCompleto e KitSword 0)
-- Index do Ring caso KitAneis = 1
-- Level do item
-- Durabilidade do item
-- Sorte
-- Skill
-- Options
-- Options excelentes
-- quantidade de itens a dropar (Caso coloque 0 será considerado Drop Troll, e avisara o jogador que ele foi trolado.)
-- Porcentagem do item - Minimo 0.01 máximo 100 (sempre dividir os 100% entre os itens)
-- AnuncioGlobal - 1 Anuncia no global que dropou o item, 0 não anuncia

CONFIG_DROP_SYSTEM_ITEM = { 
----Box Wing Full
{_Index = 1, _NomeItem = "Wing of Spirits",              _DropMoeda = false,  _MoedaIndex = 0, _MoedaQtd = 0, _Section = 12, 	_IndexItem = 3, 		_SetCompleto = 0,	_KitSword = 0,	_IndexShield = 0,	_KitAneis = 0, _IndexRing = 0, _Level = 13,	_Durabilidade = 255,	_Luck = 1, _Skill = 1, _Opt = 7, _Exc_1 = 1, _Exc_2 = 1, _Exc_3 = 1, _Exc_4 = 1, _Exc_5 = 1, _Exc_6 = 1, _Count = 1, _Porcentagem = 13.33, _AnuncioGlobal = 0},
{_Index = 1, _NomeItem = "Wing of Soul",                 _DropMoeda = false,  _MoedaIndex = 0, _MoedaQtd = 0, _Section = 12, 	_IndexItem = 4, 		_SetCompleto = 0,	_KitSword = 0,	_IndexShield = 0,	_KitAneis = 0, _IndexRing = 0, _Level = 13,	_Durabilidade = 255,	_Luck = 1, _Skill = 1, _Opt = 7, _Exc_1 = 1, _Exc_2 = 1, _Exc_3 = 1, _Exc_4 = 1, _Exc_5 = 1, _Exc_6 = 1, _Count = 1, _Porcentagem = 13.33, _AnuncioGlobal = 0},
{_Index = 1, _NomeItem = "Wing of Dragon",               _DropMoeda = false,  _MoedaIndex = 0, _MoedaQtd = 0, _Section = 12, 	_IndexItem = 5, 		_SetCompleto = 0,	_KitSword = 0,	_IndexShield = 0,	_KitAneis = 0, _IndexRing = 0, _Level = 13,	_Durabilidade = 255,	_Luck = 1, _Skill = 1, _Opt = 7, _Exc_1 = 1, _Exc_2 = 1, _Exc_3 = 1, _Exc_4 = 1, _Exc_5 = 1, _Exc_6 = 1, _Count = 1, _Porcentagem = 13.33, _AnuncioGlobal = 0},
{_Index = 1, _NomeItem = "Wing of Darkness",             _DropMoeda = false,  _MoedaIndex = 0, _MoedaQtd = 0, _Section = 12, 	_IndexItem = 6, 		_SetCompleto = 0,	_KitSword = 0,	_IndexShield = 0,	_KitAneis = 0, _IndexRing = 0, _Level = 13,	_Durabilidade = 255,	_Luck = 1, _Skill = 1, _Opt = 7, _Exc_1 = 1, _Exc_2 = 1, _Exc_3 = 1, _Exc_4 = 1, _Exc_5 = 1, _Exc_6 = 1, _Count = 1, _Porcentagem = 13.33, _AnuncioGlobal = 0},
{_Index = 1, _NomeItem = "Cape of the Lord",             _DropMoeda = false,  _MoedaIndex = 0, _MoedaQtd = 0, _Section = 13, 	_IndexItem = 30, 		_SetCompleto = 0,	_KitSword = 0,	_IndexShield = 0,	_KitAneis = 0, _IndexRing = 0, _Level = 13,	_Durabilidade = 255,	_Luck = 1, _Skill = 1, _Opt = 7, _Exc_1 = 1, _Exc_2 = 1, _Exc_3 = 1, _Exc_4 = 1, _Exc_5 = 1, _Exc_6 = 1, _Count = 1, _Porcentagem = 13.33, _AnuncioGlobal = 0},
{_Index = 1, _NomeItem = "Wing of Despair",              _DropMoeda = false,  _MoedaIndex = 0, _MoedaQtd = 0, _Section = 12, 	_IndexItem = 42, 		_SetCompleto = 0,	_KitSword = 0,	_IndexShield = 0,	_KitAneis = 0, _IndexRing = 0, _Level = 13,	_Durabilidade = 255,	_Luck = 1, _Skill = 1, _Opt = 7, _Exc_1 = 1, _Exc_2 = 1, _Exc_3 = 1, _Exc_4 = 1, _Exc_5 = 1, _Exc_6 = 1, _Count = 1, _Porcentagem = 13.33, _AnuncioGlobal = 0},
{_Index = 1, _NomeItem = "Wing of Storm",		 _DropMoeda = false,  _MoedaIndex = 0, _MoedaQtd = 0, _Section = 12, 	_IndexItem = 36, 		_SetCompleto = 0,	_KitSword = 0,	_IndexShield = 0,	_KitAneis = 0, _IndexRing = 0, _Level = 13,	_Durabilidade = 255,	_Luck = 1, _Skill = 1, _Opt = 7, _Exc_1 = 1, _Exc_2 = 1, _Exc_3 = 1, _Exc_4 = 1, _Exc_5 = 1, _Exc_6 = 1, _Count = 1, _Porcentagem = 5.0, _AnuncioGlobal = 0},
{_Index = 1, _NomeItem = "Wing of Eternal",	         _DropMoeda = false,  _MoedaIndex = 0, _MoedaQtd = 0, _Section = 12, 	_IndexItem = 37, 		_SetCompleto = 0,	_KitSword = 0,	_IndexShield = 0,	_KitAneis = 0, _IndexRing = 0, _Level = 13,	_Durabilidade = 255,	_Luck = 1, _Skill = 1, _Opt = 7, _Exc_1 = 1, _Exc_2 = 1, _Exc_3 = 1, _Exc_4 = 1, _Exc_5 = 1, _Exc_6 = 1, _Count = 1, _Porcentagem = 5.0, _AnuncioGlobal = 0},
{_Index = 1, _NomeItem = "Wing of Illusion",	         _DropMoeda = false,  _MoedaIndex = 0, _MoedaQtd = 0, _Section = 12, 	_IndexItem = 38, 		_SetCompleto = 0,	_KitSword = 0,	_IndexShield = 0,	_KitAneis = 0, _IndexRing = 0, _Level = 13,	_Durabilidade = 255,	_Luck = 1, _Skill = 1, _Opt = 7, _Exc_1 = 1, _Exc_2 = 1, _Exc_3 = 1, _Exc_4 = 1, _Exc_5 = 1, _Exc_6 = 1, _Count = 1, _Porcentagem = 5.0, _AnuncioGlobal = 0},
{_Index = 1, _NomeItem = "Wing of Ruin",	         _DropMoeda = false,  _MoedaIndex = 0, _MoedaQtd = 0, _Section = 12, 	_IndexItem = 39, 		_SetCompleto = 0,	_KitSword = 0,	_IndexShield = 0,	_KitAneis = 0, _IndexRing = 0, _Level = 13,	_Durabilidade = 255,	_Luck = 1, _Skill = 1, _Opt = 7, _Exc_1 = 1, _Exc_2 = 1, _Exc_3 = 1, _Exc_4 = 1, _Exc_5 = 1, _Exc_6 = 1, _Count = 1, _Porcentagem = 5.0, _AnuncioGlobal = 0},
{_Index = 1, _NomeItem = "Wing of Emperor",	         _DropMoeda = false,  _MoedaIndex = 0, _MoedaQtd = 0, _Section = 12, 	_IndexItem = 40, 		_SetCompleto = 0,	_KitSword = 0,	_IndexShield = 0,	_KitAneis = 0, _IndexRing = 0, _Level = 13,	_Durabilidade = 255,	_Luck = 1, _Skill = 1, _Opt = 7, _Exc_1 = 1, _Exc_2 = 1, _Exc_3 = 1, _Exc_4 = 1, _Exc_5 = 1, _Exc_6 = 1, _Count = 1, _Porcentagem = 5.0, _AnuncioGlobal = 0},
{_Index = 1, _NomeItem = "Wing of Dimension",		 _DropMoeda = false,  _MoedaIndex = 0, _MoedaQtd = 0, _Section = 12, 	_IndexItem = 43, 		_SetCompleto = 0,	_KitSword = 0,	_IndexShield = 0,	_KitAneis = 0, _IndexRing = 0, _Level = 13,	_Durabilidade = 255,	_Luck = 1, _Skill = 1, _Opt = 7, _Exc_1 = 1, _Exc_2 = 1, _Exc_3 = 1, _Exc_4 = 1, _Exc_5 = 1, _Exc_6 = 1, _Count = 1, _Porcentagem = 5.0, _AnuncioGlobal = 0},
{_Index = 1, _NomeItem = "Wing of War",		 	 _DropMoeda = false,  _MoedaIndex = 0, _MoedaQtd = 0, _Section = 12, 	_IndexItem = 150, 		_SetCompleto = 0,	_KitSword = 0,	_IndexShield = 0,	_KitAneis = 0, _IndexRing = 0, _Level = 13,	_Durabilidade = 255,	_Luck = 1, _Skill = 1, _Opt = 7, _Exc_1 = 1, _Exc_2 = 1, _Exc_3 = 1, _Exc_4 = 1, _Exc_5 = 1, _Exc_6 = 1, _Count = 1, _Porcentagem = 3.0, _AnuncioGlobal = 1},
}

-- Index (deixe o mesmo do CONFIG_DROP_SYSTEM) (Não use 0, apenas 1 move por index)
--Numeor do mapa
--Coordenada X
--Coordenada Y
CONFIG_DROP_SYSTEM_PORTAL = {
--{_Index = 3, _Map = 0, _CoordX = 125, _CoordY = 125},
}



--Não mexer nisso
local DROP_ITEM_RAND = {}

function DropSystem.InsertItem(item)
	if CONFIG_DROP_SWITCH == 0
	then
		return
	end

	InsertKey(DROP_ITEM_RAND, item)
end

function DropSystem.GetItemRandom(index, porcentagem)
	if CONFIG_DROP_SWITCH == 0
	then
		return
	end

	DROP_ITEM_RAND = {}
	
	if porcentagem == 1
	then
		for i in ipairs(CONFIG_DROP_SYSTEM_ITEM) do
			if CONFIG_DROP_SYSTEM_ITEM[i]._Index == index
			then
				qtd = (CONFIG_DROP_SYSTEM_ITEM[i]._Porcentagem/100)*10000
				for x = 1, qtd do
					DropSystem.InsertItem(CONFIG_DROP_SYSTEM_ITEM[i])
				end		
			end		
		end
	else
		for i in ipairs(CONFIG_DROP_SYSTEM_ITEM) do
			if CONFIG_DROP_SYSTEM_ITEM[i]._Index == index
			then
				DropSystem.InsertItem(CONFIG_DROP_SYSTEM_ITEM[i])
			end
		end
	end
	
	if #DROP_ITEM_RAND > 0
	then
		return DROP_ITEM_RAND[GetRand(1, #DROP_ITEM_RAND)]
	end
	
	return -1
end

function DropSystem.PlayerDropItem(aIndex, x, y, position)
	if CONFIG_DROP_SWITCH == 0
	then
		return
	end
	local p = User.new(aIndex)
	local pInv = Inventory.new(aIndex)
	local item_index = pInv:getIndex(position)
	
	for i in ipairs(CONFIG_DROP_SYSTEM) do
		if item_index == GET_ITEM(CONFIG_DROP_SYSTEM[i]._BoxSection, CONFIG_DROP_SYSTEM[i]._BoxIndex)
		then	
			if CONFIG_DROP_SYSTEM[i]._type == 1
			then		
				if (CONFIG_DROP_SYSTEM[i]._Level == -1 or pInv:getLevel(position) == CONFIG_DROP_SYSTEM[i]._Level)
				then
					local NomeBox = CONFIG_DROP_SYSTEM[i]._NomeBox
					local NomePlayer = p:getName()
					local Item = DropSystem.GetItemRandom(CONFIG_DROP_SYSTEM[i]._Index, CONFIG_DROP_SYSTEM[i]._Porcentagem)
					
					if Item == -1
					then
						SendMessage(string.format("[Sistema] Houve um problema ao dropar o item, contate o administrador."), aIndex, 1)
						return 1
					end
					
					if CONFIG_DROP_SYSTEM[i].ItemInventory == 1
					then
						for i = 13, MAX_INVENTORY_RANGE do
							if pInv:isItem(i) ~= 0
							then
								SendMessage(string.format("Seu inventário precisa estar vázio,[%s] no primeiro quadrado!", NomeBox), aIndex, 1)
								return 1
							end
						end
					end
		
					if  Item._DropMoeda then 
						local moeda = DROP_MOEDA_CONFIG[Item._MoedaIndex]
						DataBase.SetAddValue(DROP_MOEDA_TABLE, moeda.moedaColumn, Item._MoedaQtd, DROP_MOEDA_WHERE,  p:getAccountID())
						SendMessage(string.format("Parabéns voce recebeu %d de %s", Item._MoedaQtd, moeda.nomeMoeda), aIndex, 1)
						
						InventoryDeleteItem(aIndex, position)
						SendInventoryDeleteItem(aIndex, position)
						
						return 1
					end
					
					if Item._Count == 0
					then
						if Item._AnuncioGlobal == 1
						then
							SendMessageGlobal(string.format("%s foi trolado ao dropar uma %s ;)", NomePlayer, NomeBox), 0)
							SendMessage(string.format("Voce foi trolado ao dropar a %s", NomeBox), aIndex, 1)			
						else
							SendMessage(string.format("Voce foi trolado ao dropar a %s ;)", NomeBox), aIndex, 1)
						end
						InventoryDeleteItem(aIndex, position)
						SendInventoryDeleteItem(aIndex, position)
						return 1
					end
					
					local exc = 0
					
					if Item._Exc_1 ~= 0
					then
						exc = exc + 1
					end
					if Item._Exc_2 ~= 0
					then
						exc = exc + 2
					end
					if Item._Exc_3 ~= 0
					then
						exc = exc + 4
					end
					if Item._Exc_4 ~= 0
					then
						exc = exc + 8
					end
					if Item._Exc_5 ~= 0
					then
						exc = exc + 16
					end
					if Item._Exc_6 ~= 0
					then
						exc = exc + 32
					end
					
					for forcount = 1, Item._Count do
						if CONFIG_DROP_SYSTEM[i].ItemInventory == 1 then
							ItemSerialCreate(aIndex, 236, 0, 0, GET_ITEM(Item._Section, Item._IndexItem), Item._Level, Item._Durabilidade, Item._Luck, Item._Skill, Item._Opt, exc)
							
							if Item._SetCompleto == 1 then
								ItemSerialCreate(aIndex, 236, 0, 0, GET_ITEM(8, Item._IndexItem), Item._Level, Item._Durabilidade, Item._Luck, Item._Skill, Item._Opt, exc)
								ItemSerialCreate(aIndex, 236, 0, 0, GET_ITEM(9, Item._IndexItem), Item._Level, Item._Durabilidade, Item._Luck, Item._Skill, Item._Opt, exc)
								ItemSerialCreate(aIndex, 236, 0, 0, GET_ITEM(10, Item._IndexItem), Item._Level, Item._Durabilidade, Item._Luck, Item._Skill, Item._Opt, exc)
								ItemSerialCreate(aIndex, 236, 0, 0, GET_ITEM(11, Item._IndexItem), Item._Level, Item._Durabilidade, Item._Luck, Item._Skill, Item._Opt, exc)
							end
							
							if Item._KitAneis == 1 then
								ItemSerialCreate(aIndex, 236, 0, 0, GET_ITEM(Item._Section, Item._IndexRing), Item._Level, Item._Durabilidade, Item._Luck, Item._Skill, Item._Opt, exc)
								ItemSerialCreate(aIndex, 236, 0, 0, GET_ITEM(Item._Section, Item._IndexRing), Item._Level, Item._Durabilidade, Item._Luck, Item._Skill, Item._Opt, exc)
							end
							
							if Item._KitSword == 1 then
								ItemSerialCreate(aIndex, 236, 0, 0, GET_ITEM(6, Item._IndexShield), Item._Level, Item._Durabilidade, Item._Luck, Item._Skill, Item._Opt, exc)
							end
							else
							local map = p:getMapNumber()
							local playerX = p:getX()
							local playerY = p:getY()
							ItemSerialCreate(aIndex, map, playerX, playerY, GET_ITEM(Item._Section, Item._IndexItem), Item._Level, Item._Durabilidade, Item._Luck, Item._Skill, Item._Opt, exc)
							
							if Item._SetCompleto == 1 then
								ItemSerialCreate(aIndex, map, playerX, playerY, GET_ITEM(8, Item._IndexItem), Item._Level, Item._Durabilidade, Item._Luck, Item._Skill, Item._Opt, exc)
								ItemSerialCreate(aIndex, map, playerX, playerY, GET_ITEM(9, Item._IndexItem), Item._Level, Item._Durabilidade, Item._Luck, Item._Skill, Item._Opt, exc)
								ItemSerialCreate(aIndex, map, playerX, playerY, GET_ITEM(10, Item._IndexItem), Item._Level, Item._Durabilidade, Item._Luck, Item._Skill, Item._Opt, exc)
								ItemSerialCreate(aIndex, map, playerX, playerY, GET_ITEM(11, Item._IndexItem), Item._Level, Item._Durabilidade, Item._Luck, Item._Skill, Item._Opt, exc)
							end
							
							if Item._KitAneis == 1 then
								ItemSerialCreate(aIndex, map, playerX, playerY, GET_ITEM(Item._Section, Item._IndexRing), Item._Level, Item._Durabilidade, Item._Luck, Item._Skill, Item._Opt, exc)
								ItemSerialCreate(aIndex, map, playerX, playerY, GET_ITEM(Item._Section, Item._IndexRing), Item._Level, Item._Durabilidade, Item._Luck, Item._Skill, Item._Opt, exc)
							end
							
							if Item._KitSword == 1 then
								ItemSerialCreate(aIndex, map, playerX, playerY, GET_ITEM(6, Item._IndexShield), Item._Level, Item._Durabilidade, Item._Luck, Item._Skill, Item._Opt, exc)
							end
							end					
					end	
					
					if Item._AnuncioGlobal == 1
					then
						SendMessageGlobal(string.format("Opa!!!!  [%s] O JOGADOR [%s] Teve sucesso em Dropar %d [%s]", NomeBox, NomePlayer, Item._Count, Item._NomeItem), 0)
					end	
					
					InventoryDeleteItem(aIndex, position)
					SendInventoryDeleteItem(aIndex, position)
					
					return 1				
				end
			elseif CONFIG_DROP_SYSTEM[i]._type == 2
			then
				for t in ipairs(CONFIG_DROP_SYSTEM_PORTAL) do
					if CONFIG_DROP_SYSTEM[i]._Index == CONFIG_DROP_SYSTEM_PORTAL[t]._Index
					then
						Teleport(aIndex, CONFIG_DROP_SYSTEM_PORTAL[t]._Map, CONFIG_DROP_SYSTEM_PORTAL[t]._CoordX, CONFIG_DROP_SYSTEM_PORTAL[t]._CoordY)
						break
					end
				end						
				InventoryDeleteItem(aIndex, position)
				SendInventoryDeleteItem(aIndex, position)
				return 1
			end
		end
	end	
	return
end

GameServerFunctions.PlayerDropItem(DropSystem.PlayerDropItem)

return DropSystem