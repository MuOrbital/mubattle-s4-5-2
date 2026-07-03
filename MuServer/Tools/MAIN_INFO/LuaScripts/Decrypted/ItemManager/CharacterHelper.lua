OpenFolder("Definitions")

local HELPER_CONFIG = {
	{ ItemIndex = GET_ITEM_MODEL(13, 130), Type = 0, Movement = 1, Height = 5.0,   Size = 1.0, SizeSelectChar = 1.0, Model = "DragonKnight",  ObjectModel = "DragonKnight", 	   Miniature = 1, SizeMiniature = 0.40, VelocityMiniature = 0.50 },
	{ ItemIndex = GET_ITEM_MODEL(13, 134), Type = 3, Movement = 0, Height = 5.0,   Size = 0.9, SizeSelectChar = 1.0, Model = "fenrilv2",	  ObjectModel = "fenrilv2", 		   Miniature = 1, SizeMiniature = 0.40, VelocityMiniature = 0.50 },
	{ ItemIndex = GET_ITEM_MODEL(13, 135), Type = 4, Movement = 0, Height = 5.0,   Size = 1.0, SizeSelectChar = 1.2, Model = "horsev2", 	  ObjectModel = "Horse_ghost_skin3_skill", Miniature = 1, SizeMiniature = 0.40, VelocityMiniature = 0.50 },
}

local HELPER_CONFIG_GLOW = {}

HELPER_CONFIG_GLOW[GET_ITEM_MODEL(13, 130)] = {
	{ RenderType = 1, Mesh = 2, ModelGlow = 65602, LightR = 1.0, LightG = 1.0, LightB = 1.0 },
}

HELPER_CONFIG_GLOW[GET_ITEM_MODEL(13, 134)] = {
	{ RenderType = 1, Mesh = 2, ModelGlow = 65602, LightR = 1.0, LightG = 1.0, LightB = 1.0 },
}

HELPER_CONFIG_GLOW[GET_ITEM_MODEL(13, 135)] = {
	{ RenderType = 1, Mesh = 2, ModelGlow = 65602, LightR = 1.0, LightG = 1.0, LightB = 1.0 },
}

local HELPER_CONFIG_EFFECT = {}

HELPER_CONFIG_EFFECT[GET_ITEM_MODEL(13, 135)] = {
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 17  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 18  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 23  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 28  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 29  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 33  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 34  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 63  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 96  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 97  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 98  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 99  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 103 , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 31  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 64  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 65  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 66  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 90  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 42  , Size = 1.0, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 52  , Size = 1.0, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 74  , Size = 1.0, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 85  , Size = 1.0, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 43  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 44  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 45  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 53  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 54  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 58  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 75  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 76  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 77  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 86  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 87  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 88  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 78  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 0, EffectIndex = 32378, EffectLevel = 0,  Skeleton = 89  , Size = 0.8, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 1, EffectIndex = 32392, EffectLevel = 0,  Skeleton = 42  , Size = 0.62, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 1, EffectIndex = 32392, EffectLevel = 0,  Skeleton = 52  , Size = 0.62, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 1, EffectIndex = 32392, EffectLevel = 0,  Skeleton = 74  , Size = 0.62, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 1, EffectIndex = 32392, EffectLevel = 0,  Skeleton = 85  , Size = 0.62, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 1, EffectIndex = 32392, EffectLevel = 0,  Skeleton = 30  , Size = 0.30, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
	{ EffectType = 1, EffectIndex = 32392, EffectLevel = 0,  Skeleton = 35  , Size = 0.30, LightR = 1.00, LightG = 0.3, LightB = 0.00, PosX = -1, PosY = -1, PosZ = -1, Black = 0.0 },
}

function LoadHelpers()
	for i = 1, #HELPER_CONFIG do
		local config = HELPER_CONFIG[i]
		SetHelper(config.ItemIndex, config.Type, config.Movement, config.Height, config.Size, config.SizeSelectChar, config.Miniature, config.SizeMiniature, config.VelocityMiniature, config.Model, config.ObjectModel)
	end
end

function RenderHelper(BMDStruct, ObjectStruct, Type)
	for i = 1, #HELPER_CONFIG do
		local config = HELPER_CONFIG[i]

		if config.ItemIndex == Type or CheckModelById(config.ItemIndex) == Type then
			if config.Type == 0 then
				CreateFlyEffect(BMDStruct, ObjectStruct, config.ItemIndex)
			elseif config.Type == 3 then
				CreateFenrirEffect(BMDStruct, ObjectStruct, config.ItemIndex)
			elseif config.Type == 4 then
				CreateHorseEffect(BMDStruct, ObjectStruct, config.ItemIndex)
			end

			return
		end
	end
end

function MoveEffects(BMDStruct, ObjectStruct, Type)
	for i = 1, #HELPER_CONFIG do
		local config = HELPER_CONFIG[i]

		if config.ItemIndex == Type or CheckModelById(config.ItemIndex) == Type then
			local Bmd = BMD.new(BMDStruct)
			local helperEffect = HELPER_CONFIG_EFFECT[config.ItemIndex]

			if helperEffect ~= nil then
				for n = 1, #helperEffect do
					local effect = helperEffect[n]

					Bmd:TransformPosition(effect.Skeleton, effect.PosX, effect.PosY, effect.PosZ)

					if effect.EffectType == 0 then
						Bmd:CreateSprite(effect.EffectIndex, effect.Size, effect.LightR, effect.LightG, effect.LightB, ObjectStruct)
					elseif effect.EffectType == 1 then
						Bmd:CreateParticle(effect.EffectIndex, effect.EffectLevel, effect.Size, effect.LightR, effect.LightG, effect.LightB, ObjectStruct)
					elseif effect.EffectType == 2 then
						Bmd:CreateEffect(effect.EffectIndex, effect.EffectLevel, effect.LightR, effect.LightG, effect.LightB, ObjectStruct)
					end
				end
			end

			Bmd = nil
			return
		end
	end
end

function RenderGlow(BMDStruct, ObjectStruct, Type)
	local Bmd = BMD.new(BMDStruct)
	local Obj = Object.new(ObjectStruct)
	local helperGlow = HELPER_CONFIG_GLOW[Type]
	local rendered = false

	if helperGlow ~= nil then
		for i = 1, #helperGlow do
			local glow = helperGlow[i]
			Bmd:SetLight(glow.LightR, glow.LightG, glow.LightB)
			rendered = true

			if glow.RenderType == 1 then
				Bmd:RenderBody(glow.Mesh, Obj:Alpha(), Obj:Mesh(), Obj:Light(), Obj:TexCoordU(), Obj:TexCoordV(), Obj:Hidden(), -1)
			else
				Bmd:RenderMesh(glow.Mesh, glow.ModelGlow, Obj:Alpha(), Obj:Mesh(), Obj:Light(), Obj:TexCoordU(), Obj:TexCoordV(), -1)
			end
		end
	end

	Bmd = nil
	Obj = nil

	return rendered
end

function RenderBaseHelper(BMDStruct, ObjectStruct)
	local Bmd = BMD.new(BMDStruct)
	local Obj = Object.new(ObjectStruct)

	Bmd:SetLight(1.0, 1.0, 1.0)
	Bmd:RenderBody(2, Obj:Alpha(), Obj:Mesh(), Obj:Light(), Obj:TexCoordU(), Obj:TexCoordV(), Obj:Hidden(), -1)

	Bmd = nil
	Obj = nil
end

function CreateFlyEffect(BMDStruct, ObjectStruct, Type)
	if not RenderGlow(BMDStruct, ObjectStruct, Type) then
		RenderBaseHelper(BMDStruct, ObjectStruct)
	end
end

function CreateFenrirEffect(BMDStruct, ObjectStruct, Type)
	local Bmd = BMD.new(BMDStruct)
	local Obj = Object.new(ObjectStruct)

	Bmd:BeginRender(1.0)
	Bmd:setMesh(0)
	if not RenderGlow(BMDStruct, ObjectStruct, Type) then
		Bmd:SetLight(1.0, 1.0, 1.0)
		Bmd:RenderBody(2, Obj:Alpha(), Obj:Mesh(), Obj:Light(), Obj:TexCoordU(), Obj:TexCoordV(), Obj:Hidden(), -1)
	end
	Bmd:setMesh(-1)

	if Obj:getAction() == 4 then
		Bmd:SetLight(1.0, 1.0, 1.0)
		Bmd:RenderMesh(1, 70, Obj:Alpha(), Obj:Mesh(), Obj:Light(), Obj:TexCoordU(), Obj:TexCoordV(), -1)
	end

	Bmd:EndRender()
	Bmd:RenderShadowModel()
	Bmd:CreateEffectsInFenrirUsingSkill(ObjectStruct)

	Bmd = nil
	Obj = nil
end

function CreateHorseEffect(BMDStruct, ObjectStruct, Type)
	if not RenderGlow(BMDStruct, ObjectStruct, Type) then
		RenderBaseHelper(BMDStruct, ObjectStruct)
	end
end
