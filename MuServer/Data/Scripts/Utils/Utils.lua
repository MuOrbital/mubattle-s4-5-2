---@diagnostic disable: trailing-space
Utils = {}

function Utils.AlterLuaState()
	return true
end
function GET_ITEM(x, y)
	if x == nil or type(x) ~= "number" or x > 15 or x < 0 then
		local tcb = debug.traceback()
		LogAddC(2, "Section invï¿½lida")
		LogAddC(2, tcb)
		return 0
	end

	if y == nil or type(y) ~= "number" or y > 512 or y < 0 then
		local tcb = debug.traceback()
		LogAddC(2, "Index invï¿½lida")
		LogAddC(2, tcb)
		return 0
	end
	local item = x * 512 + y
	return item
end


function Utils.GetPlayerInfo(player)
	local playerInfo = {[0] = player:getAccountID(), [1] = player:getName(), [2] = player:getIndex(), [3] = player:getLanguage(), [4] = player:getVip()}
	return playerInfo
end

function Utils.GetItem(x , y)
	if x == nil or type(x) ~= "number" or x > 15 or x < 0 then
		local tcb = debug.traceback()
		LogAddC(2, "Section inválida")
		LogAddC(2, tcb)
		return 0
	end

	if y == nil or type(y) ~= "number" or y > 512 or y < 0 then
		local tcb = debug.traceback()
		LogAddC(2, "Index inválida")
		LogAddC(2, tcb)
		return 0
	end
	local item = x * 512 + y
	return item
end

function Utils.IsPlayerBusy(player)
	if player:getInterfaceUse() ~= 0 or player:getState() == 32 or player:getDieRegen() ~= 0 or player:getTeleport() ~= 0 then
		return true
	end

	return false
end

function Utils.GetItemSuccess(rate)
	local t = GetRandomValue(1, 100000)

	if rate >= t then return true end

	return false
end

function Utils.InventoryGetItemTable(aIndex, Slot)
	local PlayerInventory = Inventory.new(aIndex)
	local tabela = {Nome = "", Serial = 0, ItemIndex = 0, Index = 0, Section = 0, Dur = 0, Level = 0, Skill = 0, Luck = 0, Option = 0, Exc = 0, Ancient = 0, JoH = 0, Add380 = 0, Slot = -1}

	tabela.Nome			= PlayerInventory:getName(Slot)
	tabela.Serial		= PlayerInventory:getSerial(Slot)
	tabela.ItemIndex	= PlayerInventory:getIndex(Slot)
	tabela.Index		= PlayerInventory:getIndex2(Slot)
	tabela.Section		= PlayerInventory:getSection(Slot)
	tabela.Dur			= PlayerInventory:getDurability(Slot)
	tabela.Level		= PlayerInventory:getLevel(Slot)
	tabela.Exc			= PlayerInventory:getItemTable(Slot, 1)
	tabela.Skill		= PlayerInventory:getItemTable(Slot, 2)
	tabela.Luck			= PlayerInventory:getItemTable(Slot, 3)
	tabela.Option		= PlayerInventory:getItemTable(Slot, 4)
	tabela.Ancient		= PlayerInventory:getItemTable(Slot, 5)
	tabela.JoH			= PlayerInventory:getItemTable(Slot, 6)
	tabela.Add380		= PlayerInventory:getItemTable(Slot, 7)
	tabela.SocketBonus	= PlayerInventory:getItemTable(Slot, 8)
	tabela.Socket1		= PlayerInventory:getItemTable(Slot, 9)
	tabela.Socket2		= PlayerInventory:getItemTable(Slot, 10)
	tabela.Socket3		= PlayerInventory:getItemTable(Slot, 11)
	tabela.Socket4		= PlayerInventory:getItemTable(Slot, 12)
	tabela.Socket5		= PlayerInventory:getItemTable(Slot, 13)
	tabela.Slot			= PlayerInventory:getItemTable(Slot, 14)
	PlayerInventory = nil
	return tabela
end

function Utils.GetPlayerIndex(name)
	for aIndex = 9000, 9999 do
		local player = User.new(aIndex)
		if player:getConnected() == 3 then
			if string.lower(player:getName()) == string.lower(name) then
				player = nil
				return aIndex
			end
		end
		player = nil
	end

	return -1
end

function Utils.GetSafeLocation(map, sx, sy, ex, ey)
	if sx < 0 then sx = 0 end
	if sy < 0 then sy = 0 end
	if ex > 255 then ex = 255 end
	if ey > 255 then ey = 255 end
	
    local x = Utils.GetRandom(sx, ex)
    local y = Utils.GetRandom(sy, ey)
    if GetMapAttr(map, x, y, 255) ~= 0 then 
        return Utils.GetSafeLocation(map, sx, sy, ex, ey)
    else
        return x, y
    end
end


function Utils.InventorySetItemTable(PlayerInventory, Slot, tabela)	
	PlayerInventory:setIndex(Slot, tabela.ItemIndex)
	PlayerInventory:setDurability(Slot, tabela.Dur)
	PlayerInventory:setLevel(Slot, tabela.Level)
	PlayerInventory:setItemTable(Slot, 1, tabela.Exc)
	PlayerInventory:setItemTable(Slot, 2, tabela.Skill)
	PlayerInventory:setItemTable(Slot, 3, tabela.Luck)
	PlayerInventory:setItemTable(Slot, 4, tabela.Option)
	PlayerInventory:setItemTable(Slot, 5, tabela.Ancient)
	PlayerInventory:setItemTable(Slot, 6, tabela.JoH)
	PlayerInventory:setItemTable(Slot, 7, tabela.Add380)
	PlayerInventory:setItemTable(Slot, 8, tabela.SocketBonus)
	PlayerInventory:setItemTable(Slot, 9, tabela.Socket1)
	PlayerInventory:setItemTable(Slot, 10, tabela.Socket2)
	PlayerInventory:setItemTable(Slot, 11, tabela.Socket3)
	PlayerInventory:setItemTable(Slot, 12, tabela.Socket4)
	PlayerInventory:setItemTable(Slot, 13, tabela.Socket5)
end

function Utils.UserGetHWID(playerInfo)
	return DataBase.GetString("MEMB_STAT", "HWID", "memb___id", playerInfo[0])	
end

function Utils.UserGetIP(playerInfo)
	return DataBase.GetString("MEMB_STAT", "IP", "memb___id", playerInfo[0])	
end

function Utils.GetSuccessByRate(rate)
	if rate <= 0 or rate == nil then return false end

	if rate >= 100 then return true end

	local failRate = 100-rate
	local Count = {
		{Result = true, Rate = rate},
		{Result = false, Rate = failRate},
	}

	local selection = GetRandomValue(1, 100)
	for k,v in ipairs(Count) do
	   selection = selection - v.Rate
	   if (selection <= 0) then
		  return v.Result
	   end
	end
end

function Utils.GetItem(x , y)
	if x == nil or y ==  nil then
		local tcb = debug.traceback()
		LogAddLua(tcb)
	end
	if x > 15 or x < 0 or y < 0 or y > 512 then return -1 end
	return x * 512 + y
end

function Utils.GetOption(Item)
	local level = GetRandomValue(Item.LevelMin, Item.LevelMax)
	local skill = Item.Skill
	if skill == -1 then
		skill = GetRandomValue(0, 1)
	end
	local luck = Item.Luck
	if luck == -1 then
		luck = GetRandomValue(0, 1)
	end
	local dur = GetRandomValue(Item.DurMin, Item.DurMax)
	local opt = GetRandomValue(Item.OptMin, Item.OptMax)
	local exc = Utils.GetExcelente(Item.ExcMin, Item.ExcMax)

	return {Level = level, Skill = skill, Luck = luck, Dur = dur, Option = opt, Exc = exc}
end

function Utils.IsInventoryEmpty(aIndex)
	local PlayerInventory = Inventory.new(aIndex)
	for slot = 12, 75 do
		if PlayerInventory:isItem(slot) ~= 0 then return false end
	end

	return true
end

function Utils.GetRandom(x, y, z)
	return GetRandomValue(x, y)
end

function Utils.GetItemByIndex(Index)	
	for section = 0, 15 do
		local id = Index - 512 * section
		if id >= 0 and id <= 512 then
			return section, id
		end
	end
end

function Utils.GetExcelente(Min, Max)
	if Min == 7 and Max == 7 then return 47 end

	local ExcTable = {}
	if Max == 5 then
		ExcTable = {1,2,4,8,32}
	else
		ExcTable = {1,2,4,8,16,32}
	end

	local exc = 0
	if Min > 0
	then
		for i = 1, Min do
			local rand = GetRandomValue(1, #ExcTable)
			local pick = ExcTable[rand] 
			exc = exc + pick
			table.remove(ExcTable, rand)
		end
	end

	if Max > Min
	then
		local MaxOptions = Max - Min
		for i = 1, MaxOptions do
			if Utils.GetSuccessByRate(50)
			then
				local rand = GetRandomValue(1, #ExcTable)
				local pick = ExcTable[rand] 
				exc = exc + pick
				table.remove(ExcTable, rand)
			end
		end
	end

	return exc
end

function Utils.ExcToTable(exc)
    local t = {}       
    for i = 1, 6 do
        t[i] = math.fmod(exc, 2)
        exc = math.floor((exc - t[i]) / 2)
    end
    return t
end

function Utils.TableToExc(t)
	local id = 6
	local nt = {}
	for i in ipairs(t) do
		nt[id] = t[i]
		id = id - 1
	end	
	local a = ""
	for i in ipairs(nt) do
		a = a..nt[i]
	end
	local exc = tonumber(a,2)
	return exc
end

function Utils.ShuffleTable(t)
    for i = #t, 2, -1 do
        local j = GetRandomValue(1, i)
        t[i], t[j] = t[j], t[i]
    end
end

function Distance(x, x2, y, y2)
	return math.sqrt(2^(math.floor(math.abs(x - x2))) + 2^(math.floor(math.abs(y - y2))))
end

function CountTable(table)
	local count = 0

	for i, name in ipairs(table) do
		count = count + 1
	end

	return count
end

function RemoverTable(Tab, Player)
	for i, name in ipairs(Tab) do 
		if name == Player
		then
			table.remove(Tab, i)
		end
	end
end

function RemoverTableInt(table, identif)
	for i, key in ipairs(table) do
		if key == identif
		then
			table.remove(table, i)
		end
	end
end

function has_key(tab, val)
    for index, value in ipairs (tab) do
        if value == val then
            return true
        end
    end

    return false
end

function InsertKey(tab, key)
	table.insert(tab, key)
end

function RemoveKey(tab, key)
	table.remove(tab, key)
end

function GetRand(Start, End)
	return GetRandomValue(Start, End)
end

return Utils