OpenFolder("Definitions")

CUSTOM_EFFECT_SETS_SWITCH = 0

CUSTOM_SET_EFFECTS = {}

--[[
this config only detect boots so you need put boots of the set you want put effect

EffectType, 0 = sprite, 1 = particle, 2 = skill
]]--
--AZUL
for uniqueseet = 510, 511 do
CUSTOM_SET_EFFECTS[GET_ITEM_MODEL(11, uniqueseet)] = { 
	{ BonePlayer = 20, ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 0, EffectID = 32002, EffectLv = 0, Size =  1.8, R = 0.0, G = 0.5, B = 0.6, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 27, ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 0, EffectID = 32002, EffectLv = 0, Size =  1.3, R = 0.0, G = 0.5, B = 0.6, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 36, ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 0, EffectID = 32002, EffectLv = 0, Size =  1.3, R = 0.0, G = 0.5, B = 0.6, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 18,  ItemMinLevel = 0, ItemMaxLevel = 15, EffectType = 0, EffectID = 32002, EffectLv = 0, Size=   1.3, R = 0.0, G = 0.5, B = 0.6, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 4,  ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 0, EffectID = 32002, EffectLv = 0, Size =  1.3, R = 0.0, G = 0.5, B = 0.6, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 11, ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 0, EffectID = 32002, EffectLv = 0, Size =  1.3, R = 0.0, G = 0.5, B = 0.6, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 28, ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 0, EffectID = 32002, EffectLv = 0, Size =  1.3, R = 0.0, G = 0.5, B = 0.6, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 5, ItemMinLevel =   0, ItemMaxLevel = 15, EffectType = 0, EffectID = 32002, EffectLv = 0, Size =  1.3, R = 0.0, G = 0.5, B = 0.6, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 12,ItemMinLevel =   0, ItemMaxLevel = 15, EffectType = 0, EffectID = 32002, EffectLv = 0, Size =  1.3, R = 0.0, G = 0.5, B = 0.6, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 37,ItemMinLevel =   0, ItemMaxLevel = 15, EffectType = 0, EffectID = 32002, EffectLv = 0, Size =  1.3, R = 0.0, G = 0.5, B = 0.6, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },

	{ BonePlayer = 20, ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 1, EffectID = 32290, EffectLv = 12, Size =  0.4, R = 0.1, G = 0.2, B = 0.2, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 27, ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 1, EffectID = 32290, EffectLv = 12, Size =  0.4, R = 0.1, G = 0.2, B = 0.2, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 36, ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 1, EffectID = 32290, EffectLv = 12, Size =  0.4, R = 0.1, G = 0.2, B = 0.2, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 18, ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 1, EffectID = 32290, EffectLv = 12, Size =  0.4, R = 0.1, G = 0.2, B = 0.2, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 4,  ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 1, EffectID = 32290, EffectLv = 12, Size =  0.4, R = 0.1, G = 0.2, B = 0.2, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 11, ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 1, EffectID = 32290, EffectLv = 12, Size =  0.4, R = 0.1, G = 0.2, B = 0.2, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 28, ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 1, EffectID = 32290, EffectLv = 12, Size =  0.4, R = 0.1, G = 0.2, B = 0.2, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 5,  ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 1, EffectID = 32290, EffectLv = 12, Size =  0.4, R = 0.1, G = 0.2, B = 0.2, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 12, ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 1, EffectID = 32290, EffectLv = 12, Size =  0.4, R = 0.1, G = 0.2, B = 0.2, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 37, ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 1, EffectID = 32290, EffectLv = 12, Size =  0.4, R = 0.1, G = 0.2, B = 0.2, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },

	{ BonePlayer = 20, ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 1, EffectID = 32387, EffectLv = 5, Size =  0.1, R = 0.1, G = 0.1, B = 0.1, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 27, ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 1, EffectID = 32387, EffectLv = 5, Size =  0.1, R = 0.1, G = 0.1, B = 0.1, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 36, ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 1, EffectID = 32387, EffectLv = 5, Size =  0.1, R = 0.1, G = 0.1, B = 0.1, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 18, ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 1, EffectID = 32387, EffectLv = 5, Size =  0.1, R = 0.1, G = 0.1, B = 0.1, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 4,  ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 1, EffectID = 32387, EffectLv = 5, Size =  0.1, R = 0.1, G = 0.1, B = 0.1, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 11, ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 1, EffectID = 32387, EffectLv = 5, Size =  0.1, R = 0.1, G = 0.1, B = 0.1, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 28, ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 1, EffectID = 32387, EffectLv = 5, Size =  0.1, R = 0.1, G = 0.1, B = 0.1, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 5,  ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 1, EffectID = 32387, EffectLv = 5, Size =  0.1, R = 0.1, G = 0.1, B = 0.1, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 12, ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 1, EffectID = 32387, EffectLv = 5, Size =  0.1, R = 0.1, G = 0.1, B = 0.1, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
	{ BonePlayer = 37, ItemMinLevel =  0, ItemMaxLevel = 15, EffectType = 1, EffectID = 32387, EffectLv = 5, Size =  0.1, R = 0.1, G = 0.1, B = 0.1, PosX = -1, PosY = -1,  PosZ = -1, RandTime = 100 },
}
end

function CreateEffectSetPlayer(BMDStruct, ObjectStruct, ItemIndex, SetLevel)
	if CUSTOM_EFFECT_SETS_SWITCH == 0
	then
		return
	end
	
	local setInfo = CUSTOM_SET_EFFECTS[ItemIndex]
	
	if setInfo ~= nil
	then
		math.randomseed(os.time())
		
		for key in pairs(setInfo) do
			local effectInfo = setInfo[key]
			
			if (SetLevel >= effectInfo.ItemMinLevel and SetLevel <= effectInfo.ItemMaxLevel)
			then
				local RandValue = 100
				
				if effectInfo.RandTime ~= 100
				then
					randValue = math.random(1, 100)
				end
				
				if effectInfo.RandTime >= randValue
				then
					if effectInfo.EffectType == 0
					then
						CreateSprite(BMDStruct, ObjectStruct, effectInfo.BonePlayer, effectInfo.EffectID, effectInfo.Size, effectInfo.R, effectInfo.G, effectInfo.B, effectInfo.PosX, effectInfo.PosY, effectInfo.PosZ)
					elseif effectInfo.EffectType == 1
					then
						CreateParticle(BMDStruct, ObjectStruct, effectInfo.BonePlayer, effectInfo.EffectID, effectInfo.EffectLv, effectInfo.Size, effectInfo.R, effectInfo.G, effectInfo.B, effectInfo.PosX, effectInfo.PosY, effectInfo.PosZ)
					elseif effectInfo.EffectType == 2
					then
						CreateSkill(BMDStruct, ObjectStruct, effectInfo.BonePlayer, effectInfo.EffectID, effectInfo.EffectLv, effectInfo.Size, effectInfo.R, effectInfo.G, effectInfo.B, effectInfo.PosX, effectInfo.PosY, effectInfo.PosZ)
					end
				end
			end
		end
	end
end