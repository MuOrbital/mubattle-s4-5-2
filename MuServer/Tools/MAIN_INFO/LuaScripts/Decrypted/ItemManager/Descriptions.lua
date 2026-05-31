OpenFolder("Definitions")

--descriptions
CUSTOM_DESCRIPTIONS_INFO = {}

CUSTOM_DESCRIPTIONS_INFO[GET_ITEM(14, 511)] = 
{
	{ Description = ' NPC MALUCO BOX ', Level = -1, Bold = 1, Color = 5 },
	{ Description = ' Caixa exclusiva obtida no evento NPC Maluco ', Level = -1, Bold = 0, Color = 13 },
	{ Description = '━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━', Level = -1, Bold = 0, Color = 1 },
	{ Description = ' DROP PADRÃO ', Level = -1, Bold = 1, Color = 7 },
	{ Description = ' 77%% • Joias Clássicas ', Level = -1, Bold = 0, Color = 1 },
	{ Description = ' 17%% • Joias Customs ', Level = -1, Bold = 0, Color = 1 },
	{ Description = '━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━', Level = -1, Bold = 0, Color = 1 },
	{ Description = ' CHANCE DE DROP DE ITENS RUINED GREEN ', Level = -1, Bold = 1, Color = 5 },
	{ Description = ' FREE : 2%% de chance ', Level = -1, Bold = 1, Color = 2 },
	{ Description = ' VIP   : 6%% de chance ', Level = -1, Bold = 1, Color = 13 },
	{ Description = '━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━', Level = -1, Bold = 0, Color = 1 },
	{ Description = ' Jogadores VIP possuem Chances maiores de Drop ', Level = -1, Bold = 0, Color = 3 },
}

CUSTOM_COLOR_DESCRIPTION = 
{
	{ colorID = 20, R = 204, G =  24, B =  24 },
	{ colorID = 21, R =  51, G = 204, B =  24 },
	{ colorID = 24, R =  51, G = 102, B = 204 },
	{ colorID = 26, R = 255, G =   0, B =   0 },
	{ colorID = 27, R =  77, G =  77, B =  77 },

}

--back ground for text description and text 
CUSTOM_COLOR_WITH_BACKGROUND_DESCRIPTION = 
{
	{ colorID = 22, TextR = 255, TextG = 255, TextB = 255, TextA = 255, BackR = 153, BackG = 153, BackB = 153, BackA = 153 },
	{ colorID = 23, TextR = 240, TextG = 204, TextB = 255, TextA = 255, BackR = 240, BackG =  22, BackB =  25, BackA = 255 },
	{ colorID = 25, TextR = 255, TextG =   0, TextB =   0, TextA = 255, BackR =  77, BackG =  77, BackB =  77, BackA = 255 },
	{ colorID = 28, TextR =  77, TextG =  77, TextB =  77, TextA = 255, BackR = 128, BackG =   0, BackB =   0, BackA = 255 },
}

function SetNewColorDescription()
	for i, color in ipairs(CUSTOM_COLOR_DESCRIPTION) do
		local r = (color.R/255)
		local g = (color.G/255)
		local b = (color.B/255)
		AddNewColorDescription(color.colorID, r, g, b)
	end
end

function SetNewColorBackGroundDescription(ItemIndex)
	for i, color in ipairs(CUSTOM_COLOR_WITH_BACKGROUND_DESCRIPTION) do
		AddNewColorBackGroundDescription(color.colorID, color.TextR, color.TextG, color.TextB, color.TextA, color.BackR, color.BackG, color.BackB, color.BackA)
	end
end

function createDescMacro(line, text, color, bold)
	createDescription(line, text, color, bold)
	line = line + 1
	return line
end

function SetDescriptions(line, ItemIndex, ItemObject)
	local description = CUSTOM_DESCRIPTIONS_INFO[ItemIndex]
	
	if description ~= nil
	then
		local itemInfo = Item.new(ItemObject)
		
		for i = 1, #description do
			local infos = description[i]
			
			if infos.Level == -1 or infos.Level == itemInfo:getLevel()
			then
				line = createDescMacro(line, infos.Description, infos.Color, infos.Bold)
			end
		end
		
		itemInfo = nil
		
		collectgarbage()
		return line
	end

	--example if you want create
	if ItemIndex == GET_ITEM(7, 500) or ItemIndex == GET_ITEM(8, 500) or ItemIndex == GET_ITEM(9, 500) or ItemIndex == GET_ITEM(10, 500) or ItemIndex == GET_ITEM(11, 500)
	then		
		line = createDescMacro(line, "font normal", 9, 0)
		line = createDescMacro(line, "font bold", 7, 1)
		
		if (UserGetMap() == 0)
		then
			line = createDescMacro(line, "Buff no mapa lorencia", 1, 0)
		else
			line = createDescMacro(line, "Buff no mapa lorencia", 10, 0)
		end

		line = createDescMacro(line, "Testando nova cor 20", 3, 0)
		line = createDescMacro(line, "Testando nova cor 21", 6, 0)
		line = createDescMacro(line, "Testando nova cor 28", 9, 0)
	end

	return line
end