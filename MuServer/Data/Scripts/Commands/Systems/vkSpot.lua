--[[ Sistema vkspot - By VKDev - Victor Jardim 2024 ]]--

VK_SPOT = {}

function VK_SPOT.Command(aIndex, Args)
    local player = User.new(aIndex)

    if VKSPOT_COMMAND_SWITCH == 0 then
        SendMessage(string.format("O comando não está ativado."), aIndex, 1)
        return
    end

    if CheckGameMasterLevel(player:getAccountID(), player:getName(), VKSPOT_COMMAND_GAME_MASTER_LEVEL) == 0
	then
		SendMessage(string.format("Você não tem permissão para usar este comando."), aIndex, 1)
		return
	end

    local map = player:getMapNumber()
    local x = player:getX()
    local y = player:getY()

    local class = command:getNumber(Args, 1)
    local qtn = command:getNumber(Args, 2)
    
    if qtn > 0 then
        for i = 1, qtn do
            local monster = AddMonster(map)
            SetMapMonster(monster, map, math.floor(x + math.random(-2, 2) ), math.floor(y + math.random(-2, 2) ))
            SetMonster(monster, class)
        end
    else
        local monster = AddMonster(map)
        SetMapMonster(monster, map, x, y)
        SetMonster(monster, class)
    end

end

Commands.Register(VKSPOT_COMMAND, VK_SPOT.Command)

return VK_SPOT
