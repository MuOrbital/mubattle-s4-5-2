--[[
Game Master Level
]]

GAME_MASTER_LEVEL = {}

GAME_MASTER_LEVEL['admin'] = { characterName = 'Admin', level = 2 }

function CheckGameMasterLevel(account, name, level)
    local gamemaster = GAME_MASTER_LEVEL[account]

    if gamemaster ~= nil
    then
        if name == gamemaster.characterName
        then
            if gamemaster.level >= level
            then
                return 1
            end
        end
    end

    return 0
end