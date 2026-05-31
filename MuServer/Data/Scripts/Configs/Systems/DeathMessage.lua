-- SISTEMA DE ENVIO DE NOTIFICAÇÃO DE KILL DE PLAYERS E DE MONSTROS
DeathMessage = {}
local NO_DEATH_MESSAGE_MAPS = {}
--Desativar Mensagem de Kills nos mapas Abaixo
-- Todos os outros mapas que não estiver listados abaixo irão exibir
-- mensagem informando que o Jogador X matou o Jogador Y

--Mapas do Chaos Castle
NO_DEATH_MESSAGE_MAPS[18] = false
NO_DEATH_MESSAGE_MAPS[19] = false
NO_DEATH_MESSAGE_MAPS[20] = false
NO_DEATH_MESSAGE_MAPS[21] = false
NO_DEATH_MESSAGE_MAPS[22] = false
NO_DEATH_MESSAGE_MAPS[23] = false
NO_DEATH_MESSAGE_MAPS[53] = false

function DeathMessage.OnPlayerDie(aIndex, killerIndex)
    if killerIndex <= 0 then
        return
    end

    local dead = User.new(aIndex)
    local killer = User.new(killerIndex)

    if not dead or not killer then
        return
    end

    local mapNumber = dead:getMapNumber()

    if NO_DEATH_MESSAGE_MAPS[mapNumber] == false then
        return
    end

    SendMessageGlobal(string.format(" %s matou %s ", dead:getName(), killer:getName()), 4)
end

function DeathMessage.Init()
    GameServerFunctions.PlayerDie(DeathMessage.OnPlayerDie)
end

DeathMessage.Init()
return DeathMessage