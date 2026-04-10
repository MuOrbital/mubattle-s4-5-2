DeathMessage = {}

function DeathMessage.OnPlayerDie(aIndex, killerIndex)
    if killerIndex <= 0 then
        return
    end

    local dead = User.new(aIndex)
    local killer = User.new(killerIndex)

    if not dead or not killer then
        return
    end

    SendMessageGlobal(string.format(" [ %s ] matou [ %s ] ", dead:getName(), killer:getName()), 4)
end

function DeathMessage.Init()
    GameServerFunctions.PlayerDie(DeathMessage.OnPlayerDie)
end

DeathMessage.Init()

return DeathMessage