OpenFolder("Definitions")

--[[
Render your itens
--]]

local anyWingGhost = 0
local WingGhostStartImg = 50000

function StartLoadImages()
	--for i = 0, 11 do
	--	LoadImage(string.format("Item\\fire_sq_%05d.jpg", i), WingGhostStartImg + i)
	--end
	
	--LoadImage("Item\\season_wing_period_render.jpg", 50020)
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

	if (ItemIndex == GET_ITEM_MODEL(11, 511))
	then
		local Bmd = BMD.new(BMDStruct)
		local Obj = Object.new(ObjectStruct)
	
		if Bmd:GetLight(0) ~= 1.0 or Bmd:GetLight(1) ~= 1.0 or Bmd:GetLight(2) ~= 1.0
		then
			--Render Character
			Bmd:glColor3fv()
			Bmd:RenderMesh(0, 2, Obj:Alpha(), Obj:Mesh(), Obj:Light(), Obj:TexCoordU(), Obj:TexCoordV(), -1)
	
			local Vertex = GetDoubleRender(-2.0, 0.0) + 1.0
			if Vertex > 1.0
			then
				Vertex = 1.0
			end
	
			Bmd:SetLight(Vertex, Vertex, Vertex)
	
			Bmd:glColor3fv()
			Bmd:RenderMesh(1, 66, Obj:Alpha(), Obj:Mesh(), Obj:Light(), Obj:TexCoordU(), Obj:TexCoordV(), -1)
	
		else
			--Render Inventory
			Bmd:glColor3fv()
			Bmd:RenderMesh(0, 2, Obj:Alpha(), Obj:Mesh(), Obj:Light(), Obj:TexCoordU(), Obj:TexCoordV(), -1)
	
			local Vertex = GetDoubleRender(-2.0, 0.0) + 1.0
			if Vertex > 1.0
			then
				Vertex = 1.0
			end
	
			Bmd:SetLight(Vertex, Vertex, Vertex)
	
			Bmd:glColor3fv()
			Bmd:RenderMesh(1, 66, Obj:Alpha(), Obj:Mesh(), Obj:Light(), Obj:TexCoordU(), Obj:TexCoordV(), -1)
	
		end
		Bmd = nil
		Obj = nil
	
		return 1
	end

	return 0
end

local ITEM_REMOVEGLOW = {}

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