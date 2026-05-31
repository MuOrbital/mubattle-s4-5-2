--[[
    CORREÇÃO DE BUG DE DROP EM BOSSES (EventItemBag)
    
    Este script corrige o problema comum de drop falhando em bosses que usam EventItemBag.
    Ele intercepta a morte do monstro e força o drop através da função nativa DropEventItemBag(),
    garantindo que os itens venham corretamente do EventItemBagManager.txt.
    
    IMPORTANTE:
    - Os monstros configurados AQUI NÃO DEVEM estar setados no EventItemBagManager.txt com drop normal.
    - Apenas crie ou edite as SpecialValues no EventItemBagManager.txt (exemplo abaixo).
    - Isso evita duplicação de drops (um do GS normal + um do Script).
    
    Exemplo de configuração no EventItemBagManager.txt:
    //Index     ItemIndex   ItemLevel   MonsterClass   	 SpecialValue   Comment
      32        *           *           *                200            //Kundun (SpecialValue 200 para Kundun (Troque o ID do MonsterClass para * pois ele será definido pelo Script do Fix))
      33        *           *           *                201            //Erohim (SpecialValue 201 para Kundun (Troque o ID do MonsterClass para * pois ele será definido pelo Script do Fix))
      48        *           *           *                202            //Nightmare (SpecialValue 202 para Kundun (Troque o ID do MonsterClass para * pois ele será definido pelo Script do Fix))
      45        *           *           *                203            //Balgass (SpecialValue 203 para Kundun (Troque o ID do MonsterClass para * pois ele será definido pelo Script do Fix))
      67        *           *           *                204            //Selupan (SpecialValue 204 para Kundun (Troque o ID do MonsterClass para * pois ele será definido pelo Script do Fix))
      106       *           *           *                205            //Medusa (SpecialValue 205 para Kundun (Troque o ID do MonsterClass para * pois ele será definido pelo Script do Fix))
    
    Compatível com Rox Gaming, e todos Emuladores que usam Lua OgocX que possuem este problema do Drop.
--]]

MonsterDrop_Config = {
    --================================================================================
    -- CONFIGURAÇÃO 1: Ativar ou desativar o script inteiro
    --================================================================================
    Enabled = true,  -- true = script ativo | false = desativa tudo

    --================================================================================
    -- CONFIGURAÇÃO 2: Quais monstros serão corrigidos
    --================================================================================
    Monster = {
        -- Class   = ID do monstro (ex: 275 = Kundun)
        -- Local   = Índice da localização (referencia a seção Local abaixo)
        -- Drop    = Configuração do drop para esse monstro
        --           Enabled = true/false
        --           Index   = Qual bloco de drop usar (referencia a seção Drop abaixo)
        
        {Class = 275, Local = 1, Drop = {Enabled = true, Index = 1}},  --Kundum
        {Class = 295, Local = 2, Drop = {Enabled = true, Index = 2}},  --Erohim
	{Class = 349, Local = 3, Drop = {Enabled = true, Index = 3}},  --Balgass
        {Class = 361, Local = 4, Drop = {Enabled = true, Index = 4}},  --Nightmare
        {Class = 459, Local = 5, Drop = {Enabled = true, Index = 5}},  --Selupan
        {Class = 561, Local = 6, Drop = {Enabled = true, Index = 6}},  --Medusa
    },

    --================================================================================
    -- CONFIGURAÇÃO 3: Onde o monstro deve estar para o drop funcionar
    --================================================================================
    Local = {
        -- Índice [1] usado na configuração Monster acima
        -- Mapa     = ID do mapa (-1 = qualquer mapa)
        -- Start/End = Área retangular (coordenadas X/Y de 0 a 255)       
        [1] = {Mapa = -1, Start = {X = 0, Y = 0}, End = {X = 255, Y = 255}},
        [2] = {Mapa = -1, Start = {X = 0, Y = 0}, End = {X = 255, Y = 255}},
        [3] = {Mapa = -1, Start = {X = 0, Y = 0}, End = {X = 255, Y = 255}},
        [4] = {Mapa = -1, Start = {X = 0, Y = 0}, End = {X = 255, Y = 255}},
        [5] = {Mapa = -1, Start = {X = 0, Y = 0}, End = {X = 255, Y = 255}},
        [6] = {Mapa = -1, Start = {X = 0, Y = 0}, End = {X = 255, Y = 255}},
    },

    --================================================================================
    -- CONFIGURAÇÃO 4: Quais itens serão dropados e com qual chance
    --================================================================================
    Drop = {
        -- Índice usado na configuração Monster acima
        -- Cada entrada = um possível item a dropar
        -- SpecialValue = ID configurado no EventItemBagManager.txt
        -- Rate         = Chance em % (0 a 100) de dropar esta SpecialValue
        [1] = {
            {SpecialValue = 200, Rate = 100},  -- Kundum
        },
        [2] = {
            {SpecialValue = 201, Rate = 100},  -- Erohim
        },
        [3] = {
            {SpecialValue = 202, Rate = 100},  -- Balgass
        },
        [4] = {
            {SpecialValue = 203, Rate = 100},  -- Nightmare
        },
        [5] = {
            {SpecialValue = 204, Rate = 100},  -- Selupan
        },
        [6] = {
            {SpecialValue = 205, Rate = 100},  -- Medusa
        },
    },

    Message = {
        ["Por"] = {
            [1] = "Você recebeu %d %s",
        },
        ["Eng"] = {
            [1] = "You received %d %s",
        },
        ["Spn"] = {
            [1] = "Ricibiste %d %s",
        }
    }
}

return MonsterDrop_Config