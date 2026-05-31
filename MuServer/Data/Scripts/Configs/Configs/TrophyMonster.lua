--[[
===========================================================================================================
Obsservação, Nunca repita ID de Monstro ou NPC que já está sendo usado.
	Caso utilize a mesma ID de Monstro e NPC 2 ou mais veses não irá funcionar corretamente
	você pode adicionar quantos quiser mas sem repetir ID's
===========================================================================================================
É Necessario adicionar um NPCCustom com a mesma Skin do monstro no Cliente
	ou seja, se for monstro custom, deverá adiciona-lo 2x no servidor, 1x como monstro e outra como NPC
	se for monstro original, procure a skin bmd dele no cliente e readicione como NPC Custom!
===========================================================================================================
Execute a query abaixo de criação da Tabela e Colunas na sua Database:

CREATE TABLE TrophyMonster (Name VARCHAR(50) NOT NULL, MonsterID INT NOT NULL, Time DATETIME NOT NULL)

===========================================================================================================
TROPHY_MONSTER_SWITCH = Ativa ou Desativa o sistema

TROPHY_MONSTER_ID:
	MonsterID = ID do Monstro
	NpcID = ID do NPC que será sumonado no mapa
	MapNumber = N° do mapa que o NPC será colocado
	MapX = Coordenada X que o Npc Aparecerá no Mapa
	MapY = Coordenada Y que o Npc Aparecerá no Mapa
	MapDir = Direção que o NPC vai estar virado
]]--

TROPHY_MONSTER_SWITCH = 0

TROPHY_MONSTER_ID = {
    {MonsterID = 707, NpcID = 708, MapNumber = 0, MapX = 130, MapY = 127, MapDir = 3};  --Court Jester  
}

TrophyMonster = {}

function TrophyMonster.Init()
    if TROPHY_MONSTER_SWITCH == 1 then
        GameServerFunctions.MonsterDie(TrophyMonster.MonsterDie)
        GameServerFunctions.NpcTalk(TrophyMonster.NpcTalk)
        GameServerFunctions.MonsterReload(TrophyMonster.MonsterReload)
    end
    return
end

function TrophyMonster.MonsterReload()
    for _, v in ipairs(TROPHY_MONSTER_ID) do
        local monsterId = v.MonsterID
        local npcId = v.NpcID
        local map = v.MapNumber
        local x = v.MapX
        local y = v.MapY
        local dir = v.MapDir

        local query = string.format("SELECT TOP 1 Name, CONVERT(varchar, Time, 103) + ' ' + CONVERT(varchar, Time, 108) AS Time FROM TrophyMonster WHERE MonsterID = %d ORDER BY Time DESC", monsterId)
        local ret = db:exec(query)
        if ret ~= 0 then
            if db:fetch() ~= 100 then
                local playerName = db:getStr("Name")
                local killTime = db:getStr("Time")
                if playerName and killTime then
                    local index = AddMonster(map)
                    if index ~= -1 then
                        local npc = User.new(index)
                        if npc then
                            SetMapMonster(index, map, x, y)
                            npc:setDir(dir)
                            SetMonster(index, npcId)
                            npc:setType(3)
                            local displayName = string.format("%s - Derrotado por [%s] no dia [%s]", playerName, playerName, killTime)
                            npc:setName(displayName)
                        end
                    end
                end
            end
            db:clear()
        end
    end
end

function TrophyMonster.MonsterDie(Player, Monster)
    if TROPHY_MONSTER_SWITCH == 0 then
        return
    end

    local monster = User.new(Monster)
    local player = User.new(Player)
    if not monster or not player then
        return
    end

    local monsterId = monster:getClass()
    local playerName = player:getName()
    if not playerName or not monsterId then
        return
    end

    for _, v in ipairs(TROPHY_MONSTER_ID) do
        if monsterId == v.MonsterID then
            local sanitizedPlayerName = playerName:gsub("'", "''")
            local query = string.format("INSERT INTO TrophyMonster (Name, MonsterID, Time) VALUES ('%s', %d, GETDATE())", sanitizedPlayerName, monsterId)
            local ret = db:exec(query)
            if ret ~= 0 then
                db:clear()
                
                local latestQuery = string.format("SELECT TOP 1 Name, CONVERT(varchar, Time, 103) + ' ' + CONVERT(varchar, Time, 108) AS Time FROM TrophyMonster WHERE MonsterID = %d ORDER BY Time DESC", monsterId)
                ret = db:exec(latestQuery)
                if ret ~= 0 then
                    if db:fetch() ~= 100 then
                        local latestPlayerName = db:getStr("Name")
                        local killTime = db:getStr("Time")
                        if latestPlayerName and killTime then
                            local npcId = v.NpcID
                            local map = v.MapNumber
                            local x = v.MapX
                            local y = v.MapY
                            local dir = v.MapDir

                            local index = AddMonster(map)
                            if index ~= -1 then
                                local npc = User.new(index)
                                if npc then
                                    SetMapMonster(index, map, x, y)
                                    npc:setDir(dir)
                                    SetMonster(index, npcId)
                                    npc:setType(3)
                                    local displayName = string.format("%s - Derrotado por [%s] no dia [%s]", latestPlayerName, latestPlayerName, killTime)
                                    npc:setName(displayName)
                                end
                            end
                        end
                    end
                    db:clear()
                end
            end
            return
        end
    end
end

function TrophyMonster.NpcTalk(Npc, Player)
    if TROPHY_MONSTER_SWITCH == 0 then
        return 0
    end

    local npc = User.new(Npc)
    local player = User.new(Player)
    if not npc or not player then
        return 0
    end

    local monsterId = npc:getClass()
    local playerIndex = player:getIndex()

    for _, v in ipairs(TROPHY_MONSTER_ID) do
        if monsterId == v.NpcID then
            local query = string.format("SELECT TOP 1 Name, CONVERT(varchar, Time, 103) + ' ' + CONVERT(varchar, Time, 108) AS Time FROM TrophyMonster WHERE MonsterID = %d ORDER BY Time DESC", v.MonsterID)
            local ret = db:exec(query)
            if ret ~= 0 then
                if db:fetch() ~= 100 then
                    local playerName = db:getStr("Name")
                    local killTime = db:getStr("Time")
                    if playerName and killTime then
                        local message = string.format("Derrotado por [%s] no dia [%s]", playerName, killTime)
                        ChatTargetSend(Npc, message, playerIndex)
                    end
                else
                    ChatTargetSend(Npc, "Nenhum registro de derrota encontrado.", playerIndex)
                end
                db:clear()
            end
            return 1
        end
    end
    return 0
end

TrophyMonster.Init()

return TrophyMonster