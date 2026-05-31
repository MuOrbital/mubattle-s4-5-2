--[[
here you can upload images, I recommend using from the number 40000
--]]

function LoadImages()
	LoadImage("Custom\\Guardiao\\GB_Chest.tga", 		40000)
	LoadImage("Custom\\Guardiao\\GB_Ponto.tga", 		40001)
	LoadImage("Custom\\WarTeam\\PlacarWT.tga", 		40003)
	LoadImage("Custom\\WarTeam\\BgWT.tga", 			40004)
	LoadImage("Custom\\WarTeam\\ButonWT.tga", 		40005)
	LoadImage("Custom\\Central\\registro.tga", 		40006)
	LoadImage("Custom\\Central\\botao.tga", 		40007)
	LoadImage("Custom\\Central\\login.tga", 		40008)
	LoadImage("Custom\\Central\\configuracoes.tga", 	40009)
	LoadImage("Custom\\Central\\checkbox.tga", 		40010)
	LoadImage("Custom\\DailyReward.tga", 			40011)
end

function UnloadImages()
	UnloadImage(40000)
	UnloadImage(40001)
	UnloadImage(40003)
	UnloadImage(40004)
	UnloadImage(40005)
	UnloadImage(40006)
	UnloadImage(40007)
	UnloadImage(40008)
	UnloadImage(40009)
	UnloadImage(40010)
	UnloadImage(40011)
end