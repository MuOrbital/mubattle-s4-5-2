OpenFolder("Definitions")

--[[
Render your itens
--]]

local anyWingGhost = 0
local WingGhostStartImg = 50000

function StartLoadImages()

end

local ITEM_TRANSPARENTE = {}

function RenderModelBody(BMDStruct, ObjectStruct, ItemIndex)

    local itemTransparente = ITEM_TRANSPARENTE[ItemIndex]
	
        if itemTransparente ~= nil
        then
            local Bmd = BMD.new(BMDStruct)
    
                Bmd:glColor3fv()
                Bmd:SetLight(1.0, 1.0, 1.0)
                Bmd = nil
    
                return 1
        end

	if (ItemIndex == GET_ITEM_MODEL(0, 511))--Sword
	then
		local Bmd = BMD.new(BMDStruct)
		local Obj = Object.new(ObjectStruct)
	
		if Bmd:GetLight(0) ~= 1.0 or Bmd:GetLight(1) ~= 1.0 or Bmd:GetLight(2) ~= 1.0
		then
			Bmd:glColor3fv()

			Bmd:RenderMesh(0, 2, Obj:Alpha(), Obj:Mesh(), Obj:Light(), Obj:TexCoordU(), Obj:TexCoordV(), -1)
			Bmd:RenderMesh(1, 2, Obj:Alpha(), Obj:Mesh(), Obj:Light(), Obj:TexCoordU(), Obj:TexCoordV(), -1)
			Bmd:RenderMesh(2, 66, Obj:Alpha(), Obj:Mesh(), Obj:Light(), Obj:TexCoordU(), Obj:TexCoordV(), -1)

			Bmd:SetLight(0.5, 0.5, 0.5)
			Bmd:RenderMesh(0, 65600, Obj:Alpha(), Obj:Mesh(), Obj:Light(), Obj:TexCoordU(), Obj:TexCoordV(), -1)
			Bmd:RenderMesh(1, 65600, Obj:Alpha(), Obj:Mesh(), Obj:Light(), Obj:TexCoordU(), Obj:TexCoordV(), -1)

			local Vertex = GetDoubleRender(-2.0, 0.0) + 0.27
			if Vertex > 1.0
			then
				Vertex = 1.0
			end
			Bmd:SetLight(Vertex, Vertex, Vertex)

			Bmd:glColor3fv()

			Bmd:RenderMesh(2, 66, Obj:Alpha(), Obj:Mesh(), Obj:Light(), Obj:TexCoordU(), Obj:TexCoordV(), -1) 
		else 
			Bmd:glColor3fv()

			Bmd:RenderMesh(0, 2, Obj:Alpha(), Obj:Mesh(), Obj:Light(), Obj:TexCoordU(), Obj:TexCoordV(), -1)
			Bmd:RenderMesh(1, 2, Obj:Alpha(), Obj:Mesh(), Obj:Light(), Obj:TexCoordU(), Obj:TexCoordV(), -1)
			Bmd:RenderMesh(2, 66, Obj:Alpha(), Obj:Mesh(), Obj:Light(), Obj:TexCoordU(), Obj:TexCoordV(), -1)

			Bmd:SetLight(0.5, 0.5, 0.5)
			Bmd:RenderMesh(0, 65600, Obj:Alpha(), Obj:Mesh(), Obj:Light(), Obj:TexCoordU(), Obj:TexCoordV(), -1)
			Bmd:RenderMesh(1, 65600, Obj:Alpha(), Obj:Mesh(), Obj:Light(), Obj:TexCoordU(), Obj:TexCoordV(), -1)

			local Vertex = GetDoubleRender(-2.0, 0.0) + 0.27
			if Vertex > 1.0
			then
				Vertex = 1.0
			end
			Bmd:SetLight(Vertex, Vertex, Vertex)

			Bmd:glColor3fv()

			Bmd:RenderMesh(2, 66, Obj:Alpha(), Obj:Mesh(), Obj:Light(), Obj:TexCoordU(), Obj:TexCoordV(), -1) 
		end
		Bmd = nil
		Obj = nil
		collectgarbage()
		return 1
	end

	return 0
end

local ITEM_REMOVEGLOW = {}

	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(0, 192)] = {removeglow = true} -- Sword 
	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(2, 188)] = {removeglow = true} -- Scepter 
	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(4, 188)] = {removeglow = true} -- Bow
	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(5, 189)] = {removeglow = true} -- Staff
	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(6, 188)] = {removeglow = true} -- Shield
	--
	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(7, 196)] = {removeglow = true} -- Helm 
	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(8, 196)] = {removeglow = true} -- Armor
	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(9, 196)] = {removeglow = true} -- Pants
	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(10, 196)] = {removeglow = true} -- Gloves
	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(11, 196)] = {removeglow = true} -- Boots
	--
	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(7, 197)] = {removeglow = true} -- Helm 
	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(8, 197)] = {removeglow = true} -- Armor
	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(9, 197)] = {removeglow = true} -- Pants
	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(10, 197)] = {removeglow = true} -- Gloves
	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(11, 197)] = {removeglow = true} -- Boots
	--
	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(0, 181)] = {removeglow = true} -- Sword
	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(2, 183)] = {removeglow = true} -- Shield
	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(4, 181)] = {removeglow = true} -- Shield
	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(5, 181)] = {removeglow = true} -- Shield
	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(6, 181)] = {removeglow = true} -- Shield
	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(7, 182)] = {removeglow = true} -- Shield
	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(8, 182)] = {removeglow = true} -- Shield
	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(9, 182)] = {removeglow = true} -- Shield
	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(10, 182)] = {removeglow = true} -- Shield
	--ITEM_REMOVEGLOW[GET_ITEM_MODEL(11, 182)] = {removeglow = true} -- Shield


function RenderModelBodyColor(BMDStruct, ObjectStruct, ItemIndex)
	if (ItemIndex == GET_ITEM_MODEL(12, 500))
	then
		return 1
	end
	
	return 0
end

function RenderModelBodyColor2(BMDStruct, ObjectStruct, ItemIndex)
	
	local removeglow = ITEM_REMOVEGLOW[ItemIndex]
	
	if removeglow ~= nil
	then
	    return 1
	end
	
    local itemTransparente = ITEM_TRANSPARENTE[ItemIndex]
	
    if itemTransparente ~= nil
    then
        return 1
    end
	
	return 0
end