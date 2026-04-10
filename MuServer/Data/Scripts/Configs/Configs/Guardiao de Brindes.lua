GuardiaoBrindes_Config ={
	Name = "Guardião de Brindes",
	Enabled = false,
	Packet = 0x34,
	Npc = {
		{
			class = 255,
			map = 0,
			x = 130,
			y = 127,
			dir = 3
		},
	},
	
	Position = {
		[GET_ITEM(0,22)] = {X = 330, Y = 85, W = 40, H = 70},
		[GET_ITEM(0,26)] = {X = 330, Y = 95, W = 45, H = 55},
		[GET_ITEM(0,1)] = {X = 330, Y = 95, W = 45, H = 55},
		[GET_ITEM(0,2)] = {X = 330, Y = 95, W = 45, H = 55},
		[GET_ITEM(0,3)] = {X = 330, Y = 95, W = 45, H = 55},
		[GET_ITEM(0,4)] = {X = 330, Y = 95, W = 45, H = 55},
		[GET_ITEM(0,5)] = {X = 330, Y = 95, W = 45, H = 55},
		[GET_ITEM(0,6)] = {X = 330, Y = 95, W = 45, H = 55},
		[GET_ITEM(0,7)] = {X = 330, Y = 95, W = 45, H = 55},
	}
}

return GuardiaoBrindes_Config