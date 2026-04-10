OpenFolder("Definitions")

CUSTOM_ITEM_POSITION = 
{
	{ItemIndex = GET_ITEM(4, 511), PosX = -0.08, PosY = 0.13, AngleX = 360.99, AngleY = 265.0, AngleZ = 182.0, },  --Bow Exemple
}

function StartLoadItemPosition()
	for i = 1, #CUSTOM_ITEM_POSITION do
		SetItemPosition(CUSTOM_ITEM_POSITION[i].ItemIndex, CUSTOM_ITEM_POSITION[i].PosX, CUSTOM_ITEM_POSITION[i].PosY, CUSTOM_ITEM_POSITION[i].AngleX, CUSTOM_ITEM_POSITION[i].AngleY, CUSTOM_ITEM_POSITION[i].AngleZ)
	end
end