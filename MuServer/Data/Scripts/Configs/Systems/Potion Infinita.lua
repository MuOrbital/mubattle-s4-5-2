PotionInfinita = {}

local Players = {}

function PotionInfinita.Init()
    if PotionInfinita_Ativado then
        Timer.Interval(0, PotionInfinita.Run)
        Timer.Interval(255, PotionInfinita.CheckPlayer)
        GameServerFunctions.PlayerLogout(PotionInfinita.CharacterClose)
        GameServerFunctions.CharacterSet(PotionInfinita.CheckStatus)    
    end
end

function PotionInfinita.CheckStatus(aIndex)    
    local pInv = Inventory.new(aIndex)
    ::check::
    if Players[aIndex] == nil then    
        for i in ipairs(PotionInfinita_Itens) do
            if pInv:isItem(PotionInfinita_Itens[i].Slot) ~= 0 then
                if pInv:getIndex(PotionInfinita_Itens[i].Slot) == GET_ITEM(PotionInfinita_Itens[i].Grupo, PotionInfinita_Itens[i].Id) then
                    if not has_key(Players, aIndex) then
                        table.insert(Players, aIndex)
                    end
                    Players[aIndex] = {Slot = PotionInfinita_Itens[i].Slot, Item = pInv:getIndex(PotionInfinita_Itens[i].Slot), Ativo = true}
                end
            end
        end
    else
        if pInv:isItem(Players[aIndex].Slot) == 0 or pInv:getIndex(Players[aIndex].Slot) ~= Players[aIndex].Item then
            Players[aIndex] = nil
            goto check
        end
    end
end

function PotionInfinita.CharacterClose(aIndex)
    if Players[aIndex] ~= nil then Players[aIndex] = nil return end
end

function PotionInfinita.Run()
    for i, aIndex in ipairs(Players) do
        if Players[aIndex] ~= nil then
            local pInv = Inventory.new(aIndex)
            for slot = 12, 75 do
                if pInv:isItem(slot) ~= 0 then
                    if pInv:getIndex(slot) == PotionInfinita_Index then
                        -- Verifica a durabilidade do item
                        local durability = pInv:getDurability(slot)
                        if durability <= 215 then
                            -- Restaura a durabilidade para 255
                            pInv:setDurability(slot, 255)
                        end
                    end
                end
            end        
        end
    end
end

function PotionInfinita.CheckPlayer()
    for aIndex = 13000,13999 do
        local p = User.new(aIndex)
        if p:getConnected() == 3 then
            PotionInfinita.CheckStatus(aIndex)
        end
    end
end

PotionInfinita.Init()

return PotionInfinita