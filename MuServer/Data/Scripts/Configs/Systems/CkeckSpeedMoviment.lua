local SPEED_NORMAL = 420		-- Velocidade máxima sem montaria (coords/seg)
local SPEED_MOUNT = 420			-- Velocidade máxima com montaria (coords/seg)
local SPECIAL_MAP = 58 			-- Mapa onde será aplicado o Bonus de Velocidade
local SPEED_BONUS_SPECIAL_MAP = 40 	-- Bonus adicional no mapa 58 (boss Selupan)
local SPEED_TOLERANCE = 10.0		-- Tolerância para delay de ping alto
local SPEED_ENABLE_DEBUG = 0        	-- 0 = Desligado | 1 = Ligado (mostra log de velocidade no GS)

--Adicione Pets e montarias inclusive as Customs (que aumentam velocidade de movimento)
local MOUNT_ITEMS = {
    GET_ITEM(13, 4),
    GET_ITEM(13, 5),
    GET_ITEM(13, 37),
    GET_ITEM(13, 38),
    GET_ITEM(13, 64),
    GET_ITEM(13, 65),
    GET_ITEM(13, 66),
    GET_ITEM(13, 67),
}

local PlayerMoveData = {}

local function IsMounted(aIndex)
    local inv = Inventory.new(aIndex)
    if not inv:isItem(8) then
        return false
    end
    local itemIndex = inv:getIndex(8)
    for _, mount in ipairs(MOUNT_ITEMS) do
        if itemIndex == mount then
            return true
        end
    end
    return false
end

local function GetMaxSpeed(aIndex, map)
    local baseSpeed = IsMounted(aIndex) and SPEED_MOUNT or SPEED_NORMAL
    if map == SPECIAL_MAP then
        baseSpeed = baseSpeed + SPEED_BONUS_SPECIAL_MAP
    end
    return baseSpeed
end

local function CheckPlayerMovement(aIndex, map, x, y, sx, sy)
    local player = User.new(aIndex)
    if player:getType() ~= 1 or not player:getLive() then
        return
    end

    local currentTime = os.clock()

    if not PlayerMoveData[aIndex] then
        PlayerMoveData[aIndex] = {
            lastX = x,
            lastY = y,
            lastTime = currentTime,
            lastMap = map
        }
        return
    end

    local data = PlayerMoveData[aIndex]

    if map ~= data.lastMap then
        data.lastX = x
        data.lastY = y
        data.lastTime = currentTime
        data.lastMap = map
        return
    end

    local dx = x - data.lastX
    local dy = y - data.lastY
    local distance = math.sqrt(dx * dx + dy * dy)
    local timeDiff = currentTime - data.lastTime

    if timeDiff < 0.01 then
        return
    end

    local currentSpeed = distance / timeDiff
    local maxSpeed = GetMaxSpeed(aIndex, map)

    if SPEED_ENABLE_DEBUG == 1 then
        local playerName = player:getName()
        local mounted = IsMounted(aIndex) and "Sim" or "Não"
        LogAdd(string.format("[SPEED DEBUG] %s | Mapa: %d | Coord: %d,%d | Dist: %.2f | Tempo: %.3f | Vel: %.2f | Máx: %.1f | Montado: %s",
            playerName, map, x, y, distance, timeDiff, currentSpeed, maxSpeed, mounted))
    end

    if currentSpeed > (maxSpeed + SPEED_TOLERANCE) then
        -- SendMessageGlobal(string.format("[AntiSpeed] %s desconectado por speed hack (%.2f > %.1f) Mapa: %d", player:getName(), currentSpeed, maxSpeed, map), 1)
        SendMessage(string.format("[Anti-Cheat] Você foi desconectado por suspeita de Speed Hack!"), aIndex, 1)
        CloseChar(aIndex, 1)
    end

    data.lastX = x
    data.lastY = y
    data.lastTime = currentTime
end

GameServerFunctions.PlayerMove(CheckPlayerMovement)

local function OnPlayerLogout(aIndex)
    PlayerMoveData[aIndex] = nil
end
GameServerFunctions.PlayerLogout(OnPlayerLogout)

local function OnEnterCharacter(aIndex)
    PlayerMoveData[aIndex] = nil
end
GameServerFunctions.EnterCharacter(OnEnterCharacter)