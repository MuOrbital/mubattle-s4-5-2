--[[
Configs
--]]

local CUSTOM_MONSTER_CONFIG = { 
	{ MonsterID = 600, MonsterType = 1, Size = 1.50, Map = -1, PosX = -1, PosY = -1, MonsterName = "Hell Maine V.2", MonsterFolder = "Monster\\", MonsterModel = "monster106"},
}


--[[
System working don't touch
--]]
function StartLoadMonster()
	for i = 1, #CUSTOM_MONSTER_CONFIG do
		local monster = CUSTOM_MONSTER_CONFIG[i]

		SetMonsterInfo(GET_MONSTER_MODEL(monster.MonsterID), monster.MonsterType)
	end
end

MODEL_MONSTER_MAX = 574

function GET_MONSTER_MODEL(index)
	return index + MODEL_MONSTER_MAX
end

function GET_MONSTER_ID(index)
	return index - MODEL_MONSTER_MAX
end

function CreateMonster(MonsterID, PosX, PosY, Key)
	for i = 1, #CUSTOM_MONSTER_CONFIG do
		local monster = CUSTOM_MONSTER_CONFIG[i]
	
		if monster.MonsterID == MonsterID 
			and (monster.Map == -1 or getMapNumber() == monster.Map) 
			and (monster.PosX == -1 or monster.PosX == PosX)
			and (monster.PosY == -1 or monster.PosY == PosY)
		then
			LoadMonster(MonsterID, monster.MonsterType, monster.MonsterFolder, monster.MonsterModel)

			local monsterObj = CreateMonsterStruct(Key, GET_MONSTER_MODEL(monster.MonsterID), PosX, PosY, monster.MonsterType, monster.Size, monster.MonsterName)
			return 1, monsterObj
		end
	end

	return 0, 0
end

function LooadModelMonster(MonsterID)
	for i = 1, #CUSTOM_MONSTER_CONFIG do
		local monster = CUSTOM_MONSTER_CONFIG[i]
		
		if GET_MONSTER_MODEL(monster.MonsterID) == MonsterID
		then
			LoadModel(MonsterID, monster.MonsterFolder, monster.MonsterModel, -1)
			return 1
		end
	end

	return 0
end

function LooadTextureMonster(MonsterID)
	for i = 1, #CUSTOM_MONSTER_CONFIG do
		local monster = CUSTOM_MONSTER_CONFIG[i]
		
		if GET_MONSTER_MODEL(monster.MonsterID) == MonsterID
		then
			LoadTexture(MonsterID, monster.MonsterFolder, monster.MonsterModel, -1)
			return 1
		end
	end

	return 0
end

StartLoadMonster()