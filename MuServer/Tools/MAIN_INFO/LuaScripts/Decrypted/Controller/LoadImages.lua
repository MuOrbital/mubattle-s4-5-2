--[[
here you can upload images, I recommend using from the number 40000
--]]

function LoadImages()
	LoadImage("Custom\\Guardiao\\GB_Chest.tga", 		40000)
	LoadImage("Custom\\Guardiao\\GB_Ponto.tga", 		40001)
	LoadImage("Custom\\Guardiao\\GB_Ponto.tga", 		40002)
end

function UnloadImages()
	UnloadImage(40000)
	UnloadImage(40001)
	UnloadImage(40002)
end