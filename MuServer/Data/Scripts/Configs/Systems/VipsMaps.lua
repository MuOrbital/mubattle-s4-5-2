--Creado por Striker
-- VIPS_MAPS

Vip_Map_System = {}


local VIP_MAPS_SWITCH = 0
--Gate de onde se mandara al jugador que no tenga el VIP requerido
local GATE_TARGET = 17
--cada cuantos segundos se revisara a los usuarios
local CHECK_USERS_TIME = 60
--Mapas donde se eliminara a los players
local VIPS_MAPS = {}
--VIPS_MAPS[MapNumber]
VIPS_MAPS[85] = {Vip = 1}
VIPS_MAPS[86] = {Vip = 1}
VIPS_MAPS[87] = {Vip = 1}
local timer_Check

function Vip_Map_System.Init()
    if VIP_MAPS_SWITCH == 0 then
        return
    end
   GameServerFunctions.CharacterMove(Vip_Map_System.CharacterMove)
   timer_Check = Timer.Interval(CHECK_USERS_TIME, Vip_Map_System.Kick)
end



function Vip_Map_System.CharacterMove(aIndex, map, x, y)
    if VIP_MAPS_SWITCH == 0 then
        return
    end
	
    local player = User.new(aIndex)
    local vip = player:getVip()

    if VIPS_MAPS[map] then
        if vip < VIPS_MAPS[map].Vip then
            SendMessage(string.format("[System] Você não pode permanecer neste mapa sem VIP ", VIPS_MAPS[map].Vip), player:getIndex(), 1)
            MoveGate(player:getIndex(), GATE_TARGET)
            return 1
        end
    end
end

function Vip_Map_System.Kick()
    for i = OBJECT_START_USER, MAX_OBJECT do
        local player = User.new(i)
        
        if player:getConnected() == 0 or player:getConnected() == 1 or player:getConnected() == 2 or player:getConnected() == 3 then
            local vip = player:getVip()
            local mapNumber = player:getMapNumber()
            
            if VIPS_MAPS[mapNumber] and vip < VIPS_MAPS[mapNumber].Vip then
                MoveGate(player:getIndex(), GATE_TARGET)
                SendMessage(string.format("[System] Você não pode permanecer neste mapa sem VIP ", VIPS_MAPS[mapNumber].Vip), player:getIndex(), 1)
            end
        end
    end
end

Vip_Map_System.Init()

return Vip_Map_System
