EventVeloz = {}
EventVeloz.NPC = nil
EventVeloz.GMIndex = nil
EventVeloz.Active = false
EventVeloz.Processing = false
EventVeloz.EndTime = 0
EventVeloz.TimerId = nil
EventVeloz.ReminderTimerId = nil

math.randomseed(os.time() + os.clock() * 1000)

local function EV_Global(id, ...)
    local msg = EVENTVELOZ_MESSAGES["Por"][id]
    if msg then
        SendMessageGlobal(string.format(msg, ...), 0)
    end
end

local function EV_Private(aIndex, id, ...)
    local msg = EVENTVELOZ_MESSAGES["Por"][id]
    if msg then
        SendMessage(aIndex, 1, string.format(msg, ...))
    end
end

local function ChooseRandomMap()
    local keys = {}
    for k in pairs(EventVeloz_Config.Maps) do
        table.insert(keys, k)
    end
    if #keys == 0 then return 0 end
    return keys[math.random(1, #keys)]
end

function EventVeloz.SpawnNPC()
    local map = ChooseRandomMap()
    local index = AddMonster(map)
    if index == -1 then
        return false
    end

    SetPositionMonster(index, map)     -- garante posição aleatória válida
    SetMonster(index, EventVeloz_Config.MonsterClass)

    local mob = User.new(index)
    mob:setType(3)
    mob:setDir(2)

    local realMap = mob:getMapNumber()
    local realX = mob:getX()
    local realY = mob:getY()

    EventVeloz.NPC = {
        index = index,
        map = realMap,
        x = realX,
        y = realY
    }

    return true
end

function EventVeloz.RemoveNPC()
    if EventVeloz.NPC and EventVeloz.NPC.index then
        gObjDel(EventVeloz.NPC.index)
    end
    EventVeloz.NPC = nil
end

local function SendReminder()
    if not EventVeloz.Active or not EventVeloz.NPC then return end

    local mapName = EventVeloz_Config.Maps[EventVeloz.NPC.map] or "Mapa desconhecido"
    local npcName = EventVeloz_Config.NpcNameForMessage

    if EventVeloz_Config.ShowMapAndCoords then
        EV_Global(1, npcName, mapName, EventVeloz.NPC.x, EventVeloz.NPC.y)
    else
        EV_Global(1, npcName, mapName, 0, 0)
    end

    EventVeloz.ReminderTimerId = Timer.TimeOut(EventVeloz_Config.ReminderInterval, SendReminder)
end

function EventVeloz.Start(aIndex)
    if EventVeloz.Active then
        EV_Private(aIndex, 5)
        return
    end

    EventVeloz.Active = true
    EventVeloz.GMIndex = aIndex
    EventVeloz.EndTime = os.time() + (EventVeloz_Config.MaxDuration * 60)

    if not EventVeloz.SpawnNPC() then
        EventVeloz.Active = false
        return
    end

    local mapName = EventVeloz_Config.Maps[EventVeloz.NPC.map] or "Mapa desconhecido"
    local npcName = EventVeloz_Config.NpcNameForMessage

    if EventVeloz_Config.ShowMapAndCoords then
        EV_Global(1, npcName, mapName, EventVeloz.NPC.x, EventVeloz.NPC.y)
    else
        EV_Global(1, npcName, mapName, 0, 0)
    end

    if EventVeloz.TimerId then Timer.Cancel(EventVeloz.TimerId) end
    EventVeloz.TimerId = Timer.TimeOut(EventVeloz_Config.MaxDuration * 60, function()
        if EventVeloz.Active then
            EV_Global(9)
            EventVeloz.End()
        end
    end)

    if EventVeloz.ReminderTimerId then Timer.Cancel(EventVeloz.ReminderTimerId) end
    EventVeloz.ReminderTimerId = Timer.TimeOut(EventVeloz_Config.ReminderInterval, SendReminder)
end

function EventVeloz.End()
    EventVeloz.Active = false

    if EventVeloz.TimerId then
        Timer.Cancel(EventVeloz.TimerId)
        EventVeloz.TimerId = nil
    end

    if EventVeloz.ReminderTimerId then
        Timer.Cancel(EventVeloz.ReminderTimerId)
        EventVeloz.ReminderTimerId = nil
    end

    EventVeloz.RemoveNPC()
    EventVeloz.GMIndex = nil
end

function EventVeloz.NpcTalk(npc, aIndex)
    if not EventVeloz.Active then
        EV_Private(aIndex, 8)
        return 0
    end

    if not EventVeloz.NPC or EventVeloz.NPC.index ~= npc then
        return 0
    end

    -- === BLOQUEIO ANTI-SPAM DE CLIQUE ===
    if EventVeloz.Processing then
        return 0  -- Já está processando o clique anterior
    end
    EventVeloz.Processing = true
    -- ===================================

    local player = User.new(aIndex)
    local winnerName = player:getName()

    EV_Global(3, winnerName)      -- Anúncio global do vencedor
    EV_Private(aIndex, 4)         -- Mensagem privada para o vencedor

    -- Cancela o lembrete
    if EventVeloz.ReminderTimerId then
        Timer.Cancel(EventVeloz.ReminderTimerId)
        EventVeloz.ReminderTimerId = nil
    end

    -- Remove o NPC imediatamente (como você pediu)
    EventVeloz.RemoveNPC()

    -- Teleporta o vencedor e finaliza o evento após 2 segundos
    Timer.TimeOut(2, function()
        if player:getLive() == 1 then
            if EventVeloz_Config.TeleportWinnerToGM and EventVeloz.GMIndex then
                local gm = User.new(EventVeloz.GMIndex)
                if gm:getConnected() > 0 and gm:getLive() == 1 then
                    Teleport(aIndex, gm:getMapNumber(), gm:getX(), gm:getY())
                else
                    Teleport(aIndex,
                        EventVeloz_Config.DefaultTeleportMap,
                        EventVeloz_Config.DefaultTeleportX,
                        EventVeloz_Config.DefaultTeleportY)
                end
            end
        end

        EventVeloz.End()
        EventVeloz.Processing = false   -- Libera para o próximo evento (se necessário)
    end)

    return 1
end

function EventVeloz.Command(aIndex)
    local p = User.new(aIndex)
    if p:getAuthority() < EventVeloz_Config.GMLevel then
        SendMessage("Sem permissão para usar este comando.", aIndex, 1)
        return
    end

    if EventVeloz.Active then
        EV_Global(11)
        SendMessage("Evento Veloz finalizado manualmente.", aIndex, 1)
        EventVeloz.End()
    else
        EventVeloz.Start(aIndex)
        SendMessage("Evento Veloz iniciado com sucesso!", aIndex, 1)
    end
end

function EventVeloz.Init()
    if not EventVeloz_Config.Enabled then return end

    GameServerFunctions.NpcTalk(EventVeloz.NpcTalk)
    Commands.Register(EventVeloz_Config.Command, EventVeloz.Command)
end

EventVeloz.Init()
return EventVeloz