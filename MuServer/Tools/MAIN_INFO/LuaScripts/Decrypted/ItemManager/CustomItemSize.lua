OpenFolder("Definitions")

CUSTOM_ITEM_SIZE = 
{
	{ItemIndex = GET_ITEM(14, 511), SizeInventory = 0.0015 },
}

function StartLoadItemSize()
	for i = 1, #CUSTOM_ITEM_SIZE do
		SetItemSize(CUSTOM_ITEM_SIZE[i].ItemIndex, CUSTOM_ITEM_SIZE[i].SizeInventory)
	end
end