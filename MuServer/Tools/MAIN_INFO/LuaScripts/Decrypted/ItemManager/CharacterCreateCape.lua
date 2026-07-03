OpenFolder("Definitions")

CAPE_SYSTEM_INFO = {}
--CAPE_SYSTEM_INFO[GET_ITEM_MODEL(12, 156)] = {
--	stackMax = 4, imageCape = 'Item//CkMDcMakerHorus.tga', imageStrip = '', indexCape = 60001, indexStrip = -1
--}


CAPE_SYSTEM_SHINE = {}
--CAPE_SYSTEM_SHINE[60011] = {
--	shineImage = 'Item//Render1120DcMakers.jpg', shineIndex = 60013, ColorR = 2.5, ColorG = 0.01, ColorB = 0.01
--}


CAPE_SYSTEM_RENDER_INFO = {}
--CAPE_SYSTEM_RENDER_INFO[GET_ITEM_MODEL(12, 156)] = {
--	{ Bone = 19, posX = 0.0, posY = 25.0, posZ = 5.0, width = 160.0, height = 170.0, imageIndex = 60001, renderType = 5125 }, 
--}

-----

CAPE_SYSTEM_RENDER_MODEL = {}
--CAPE_SYSTEM_RENDER_MODEL[GET_ITEM_MODEL(12, 156)] = 
--{
--	{ renderType = 1, layer = 0, effectLayer = 2, lightR = 1.0, lightG = 1.0, lightB = 1.0, color3fv = 1, renderCharacter = 1 },
--	-- Inventario
--	{ renderType = 1, layer = 2, effectLayer = 0, lightR = 1.0, lightG = 1.0, lightB = 1.0, color3fv = 0, renderCharacter = 0 },
--	{ renderType = 2, layer = 2, effectLayer = 0, lightR = 1.0, lightG = 1.0, lightB = 1.0, color3fv = 0, renderCharacter = 0 },
--}

-----

CAPE_SYSTEM_SET_BONE = {}
--CAPE_SYSTEM_SET_BONE[GET_ITEM_MODEL(12, 156)] = { Bone = 19 }


CAPE_SYSTEM_SET_POSITION = {}
--CAPE_SYSTEM_SET_POSITION[GET_ITEM_MODEL(12, 156)] = 
--{
--	posX = 0.0, posY = 90.0, posZ = 0.0, matrixX = -52.0, matrixY = -8.0, matrixZ = 0.0
--}

-----


--[[
LoadImage(
name jpg ou tga,
imageValue -> recomendo entre 37000 a 38000
]]--

--[[
CapeStack.new(
stacks
) -> 3 significa que voc� vai criar uma capa com 3 imagens, a capa e as faixas por exemplo...

Cape:Create(
Position of Stack, 
Object Struct, 
BoneCharacter, 
posX, 
posY, 
posZ, 
Unknow, 
Unknow, 
Width, 
Height, 
Img Front, 
Img Back, 
Unknow
) -> cria a imagem e anima��o

Cape:Collision(
Position of Stack, 
posX,
posY,
posZ,
Unknow,
BoneCharacter
) -> faz a anima��o movimentar melhor

Cape:SetCapePlayer(
objectStruct
) -> necessario colocar no final de tudo
--]]

--[[
load the tga ou jpg for the cape
--]]

function LoadImageCape()
	for i in pairs(CAPE_SYSTEM_INFO) do
		local capeInfo = CAPE_SYSTEM_INFO[i]
		
		if string.len(capeInfo.imageCape) > 0
		then
			LoadImage(capeInfo.imageCape, capeInfo.indexCape)
		end
		
		if string.len(capeInfo.imageStrip) > 0
		then
			LoadImage(capeInfo.imageStrip, capeInfo.indexStrip)
		end
	end

	for i in pairs(CAPE_SYSTEM_SHINE) do
		local shineInfo = CAPE_SYSTEM_SHINE[i]

		LoadImage(shineInfo.shineImage, shineInfo.shineIndex)
	end
end

--[[
Seta a bone onde vai ser tirado a base de calculo para "gola" da capa
Se a capa n�o tem "gola" retorne 1520
--]]

function CharacterCapeBone(ItemIndex)
	local getCapeBone = CAPE_SYSTEM_SET_BONE[ItemIndex]

	if getCapeBone ~= nil
	then
		return getCapeBone.Bone
	end
	
	return 0
end

--[[
faz o efeito da movimenta��o
--]]

function CreateCapePlayer(StructPsychs, objectStruct, ItemIndex, Class)
	local getCapeInfo = CAPE_SYSTEM_INFO[ItemIndex]
	
	if getCapeInfo ~= nil
	then
		local StackValue = getCapeInfo.stackMax

		local startStack = 0
		
		if Class == CLASS_DL
		then
			StackValue = StackValue + 2
			startStack = 2
		end
		
		local Cape = CapeStack.new(StructPsychs)
		
		local renderCapeInfo = CAPE_SYSTEM_RENDER_INFO[ItemIndex]
		
		if renderCapeInfo ~= nil
		then

			local count = 1

			for i = startStack, StackValue do

				local capeInfo = renderCapeInfo[count]

				if capeInfo == nil
				then
					goto continue 
				end
			
				Cape:Create(i, objectStruct, capeInfo.Bone, capeInfo.posX, capeInfo.posY, capeInfo.posZ, 10, 10, capeInfo.width, capeInfo.height, capeInfo.imageIndex, capeInfo.imageIndex, capeInfo.renderType)
				
				if i == startStack
				then
					Cape:Collision(i, -10.0, -10.0, -10.0, 25.0, 17)
					Cape:Collision(i, 10.0, -10.0, -10.0, 25.0, 17)
					Cape:Collision(i, -10.0, -10.0, 20.0, 27.0, 17)
					Cape:Collision(i, 10.0, -10.0, 20.0, 27.0, 17)
				else
					Cape:Collision(i, 0.0, -15.0, -20.0, 30.0, 2)
					Cape:Collision(i, 0.0, 0.0, 0.0, 35.0, 17)
				end

				count = count + 1

				::continue::
			end
		end
		
		Cape = nil
		collectgarbage()
		return
	end
end

--[[
cria a gola no personagem, aqui � necess�rio fazer se n�o a capa n�o some alguns "peda�os"
--]]

function RenderCapeModel(BMDStruct, ObjectStruct, ItemIndex, isCharacter)

	local getRenderModel = CAPE_SYSTEM_RENDER_MODEL[ItemIndex]

	if getRenderModel ~= nil
	then
		local Bmd = BMD.new(BMDStruct)
		local Obj = Object.new(ObjectStruct)

		local R = Bmd:GetLight(0)
		local G = Bmd:GetLight(1)
		local B = Bmd:GetLight(2)

		for i = 1, #getRenderModel do
			local render = getRenderModel[i]

			if render.renderCharacter == 1
			then
				if isCharacter ~= 1
				then
					goto continue
				end
			else
				if isCharacter == 1
				then
					goto continue
				end
			end

			Bmd:SetLight(render.lightR, render.lightG, render.lightB)

			if render.color3fv == 1
			then
				Bmd:glColor3fv()
			end

			if render.renderType == 1
			then
				Bmd:RenderMesh(render.layer, render.effectLayer, Obj:Alpha(), Obj:Mesh(), Obj:Light(), Obj:TexCoordU(), Obj:TexCoordV(), -1)
			elseif render.renderType == 2
			then
				Bmd:RenderBody(render.layer, Obj:Alpha(), Obj:Mesh(), Obj:Light(), Obj:TexCoordU(), Obj:TexCoordV(), Obj:Hidden(), -1)
			end

			::continue::
		end

		Bmd = nil
		Obj = nil
		collectgarbage()
	end
end
	

--[[
Seta a posi��o do modelo no personagem

return
posX,
posY,
posZ,
matrixX,
matrixY,
matrixZ
--]]

function CapeModelPosition(ItemIndex)
	local getPosition = CAPE_SYSTEM_SET_POSITION[ItemIndex]

	if getPosition ~= nil
	then
		return getPosition.posX, getPosition.posY, getPosition.posZ, getPosition.matrixX, getPosition.matrixY, getPosition.matrixZ
	end
end

--[[
Render shine cape
]]
function CapeRenderShine(ImageIndex)

	local getShine = CAPE_SYSTEM_SHINE[ImageIndex]

	if getShine ~= nil	then
		return 1, getShine.shineIndex, getShine.ColorR, getShine.ColorG, getShine.ColorB
	end
	
	return 0,0,0,0,0

end