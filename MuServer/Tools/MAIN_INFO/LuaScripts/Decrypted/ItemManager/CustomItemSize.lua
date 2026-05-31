OpenFolder("Definitions")

CUSTOM_ITEM_SIZE = 
{
	--Box: GreenChaos, RedChaos e PurpleChaos
	{ItemIndex = GET_ITEM(14, 55), SizeInventory = 0.0015 },
	{ItemIndex = GET_ITEM(14, 56), SizeInventory = 0.0015 },
	{ItemIndex = GET_ITEM(14, 57), SizeInventory = 0.0015 },
}

function StartLoadItemSize()
	for i = 1, #CUSTOM_ITEM_SIZE do
		SetItemSize(CUSTOM_ITEM_SIZE[i].ItemIndex, CUSTOM_ITEM_SIZE[i].SizeInventory)
	end
end