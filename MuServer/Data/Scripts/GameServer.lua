--[[
declare global var
--]]

command = Command.new()

--[[
Initial System
]]--
OpenFolder('LuaSystem')
OpenFolder('Utils')
--Configs
OpenFolder('Configs\\Configs')
OpenFolder('Configs\\Systems')
OpenFolder('Configs')
--Events
OpenFolder('Events\\Configs')
OpenFolder('Events\\Systems')
OpenFolder('Events')
--Commands
OpenFolder('Commands\\Configs')
OpenFolder('Commands\\Systems')
OpenFolder('Commands')

function GameServer()	
	DataBase.Connect(3, "MuOnline", "sa", "SENHA")
end