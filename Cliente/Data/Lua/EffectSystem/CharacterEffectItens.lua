OpenFolder("Definitions")
CUSTOM_EFFECT_INFOS = {}

--CUSTOM_EFFECT_INFOS[GET_ITEM_MODEL(0, 108)] = {
--    { Type = 0, EffectID = 32002, EffectLv = 0, Bone = 2, Size = 1.3, ColorR = 0.0, ColorG = 0.7, ColorB = 1.0, Black = 0, RandTime = 0 },
--    { Type = 0, EffectID = 32002, EffectLv = 0, Bone = 3, Size = 1.2, ColorR = 0.0, ColorG = 0.7, ColorB = 1.0, Black = 0, RandTime = 0 },
--    { Type = 0, EffectID = 32002, EffectLv = 0, Bone = 4, Size = 1.1, ColorR = 0.0, ColorG = 0.7, ColorB = 1.0, Black = 0, RandTime = 0 },
--    { Type = 0, EffectID = 32002, EffectLv = 0, Bone = 5, Size = 1.0, ColorR = 0.0, ColorG = 0.7, ColorB = 1.0, Black = 0, RandTime = 0 },
--}

function CharacterItensEffect(ObjectStruct, BMDStruct, ItemIndex)
	--Effects in tuple
	effectInfo = CUSTOM_EFFECT_INFOS[ItemIndex]
	
	if effectInfo ~= nil
	then
		math.randomseed(os.time())
		for i = 1, #effectInfo do
			local effect = effectInfo[i]
			local randValue = 100
			
			if effect.RandTime ~= 100
			then
				randValue = math.random(1, 100)
			end
			
			if effect.RandTime >= randValue
			then
				if effect.Type == 0
				then
					CreateSprite(BMDStruct, effect.EffectID, effect.Bone, effect.Size, effect.ColorR, effect.ColorG, effect.ColorB, ObjectStruct)
				else
					CreateParticle(BMDStruct, effect.EffectID, effect.EffectLv, effect.Bone, effect.Size, effect.ColorR, effect.ColorG, effect.ColorB, ObjectStruct)
				end
			end
		end
		return
	end
end
