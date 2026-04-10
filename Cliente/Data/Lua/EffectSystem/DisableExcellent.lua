OpenFolder("Definitions")

--[[
    this disable glow excellent and ancient!
    desabilita glow excellent e anciente!
]]--
DISABLE_EXCELLENT_ITEMS = {
    { ItemIndex = GET_ITEM_MODEL(0, 511) },
}

function StartLoadDisableExcellent()

    for i = 1, #DISABLE_EXCELLENT_ITEMS do
        SetDisableExcellent(DISABLE_EXCELLENT_ITEMS[i].ItemIndex)
    end

end

StartLoadDisableExcellent()