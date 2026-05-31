MarketPlace_Enabled = true
MarketPlace_Packet  = 0x56

-- ============================================================
-- Limite máximo de itens que uma conta pode ter anunciados
-- simultaneamente no Marketplace.
-- A verificação é feita pelo login da conta (AccountID).
-- 0 = sem limite
-- ============================================================
MarketPlace_MaxListings = 10

MarketPlace_VIP = {
    MinAccountLevel = 1,
}

MarketPlace_Coins = {
    [1] = {Key = "WCoinC",     Table = "CashShopData", Column = "WCoinC",      Display = "WCoinC",    Enable = 1},
    [2] = {Key = "WCoinP",     Table = "CashShopData", Column = "WCoinP",      Display = "WCoinP",     Enable = 0},
    [3] = {Key = "GoblinPoint",Table = "CashShopData", Column = "GoblinPoint", Display = "GoblinP",    Enable = 0},
    [4] = {Key = "PcPoint",    Table = "PcPointData",  Column = "PcPoint",     Display = "PcPoints", Enable = 1},
}

MarketPlace_JewelBank_Table = "MEMB_INFO"
MarketPlace_JewelBank_Where = "memb___id"

MarketPlace_Jewels = {    
    [1]	 = {Column = "ChaosPoints",       CacheName = "ChaosPoints",		Display = "Jewel of Chaos",	Enable = 1},
    [2]	 = {Column = "BlessPoints",       CacheName = "BlessPoints",		Display = "Jewel of Bless",	Enable = 1},
    [3]	 = {Column = "SoulPoints",        CacheName = "SoulPoints",		Display = "Jewel of Soul",	Enable = 1},
    [4]	 = {Column = "LifePoints",        CacheName = "LifePoints",		Display = "Jewel of Life",	Enable = 1},
    [5]	 = {Column = "CreationPoints",    CacheName = "CreationPoints",		Display = "Jewel of Creation",  Enable = 1},
    [6]	 = {Column = "GuardianPoints",    CacheName = "GuardianPoints",		Display = "Jewel of Guardian",  Enable = 1},
    [7]	 = {Column = "GemstonePoints",    CacheName = "GemstonePoints",		Display = "Gemstone",		Enable = 1},
    [8]	 = {Column = "HarmonyPoints",     CacheName = "HarmonyPoints",		Display = "Harmony",		Enable = 1},
    [9]	 = {Column = "LRefiningPoints",   CacheName = "LRefiningPoints",	Display = "L Refining",  	Enable = 1},
    [10] = {Column = "HRefiningPoints",   CacheName = "HRefiningPoints",	Display = "H Refining", 	Enable = 1},
    [11] = {Column = "JSkillPoints",      CacheName = "JSkillPoints",		Display = "Jewel Skill",	Enable = 1},
    [12] = {Column = "JLuckyPoints",      CacheName = "JLuckyPoints",		Display = "Jewel Lucky",	Enable = 1},
    [13] = {Column = "JWingPoints",       CacheName = "JWingPoints",		Display = "Jewel Wing",	     	Enable = 1},
    [14] = {Column = "JSetPoints",        CacheName = "JSetPoints",		Display = "Jewel Set",	     	Enable = 1},
    [15] = {Column = "JWeaponPoints",     CacheName = "JWeaponPoints",		Display = "Jewel Weapon",	Enable = 1},
    [16] = {Column = "JLvLRingPoints",    CacheName = "JLvLRingPoints",		Display = "Jewel Lvl Ring",	Enable = 1},
    [17] = {Column = "JExRingPoints",     CacheName = "JExRingPoints",		Display = "Jewel Ex Ring",	Enable = 1},
    [18] = {Column = "JLifeRingPoints",   CacheName = "JLifeRingPoints",	Display = "Jewel Life Ring",	Enable = 1},
    [19] = {Column = "JRemovLifePoints",  CacheName = "JRemovLifePoints",	Display = "Jewel Rem Life",     Enable = 1},
}

MarketPlace_Categories = {
    [0]  = {Name = "SWORDS",   Index = 0,  ButtonID = 0},
    [1]  = {Name = "AXES",     Index = 1,  ButtonID = 1},
    [2]  = {Name = "SCEPTERS", Index = 2,  ButtonID = 2},
    [3]  = {Name = "SPEAR",    Index = 3,  ButtonID = 3},
    [4]  = {Name = "BOWS",     Index = 4,  ButtonID = 4},
    [5]  = {Name = "STAFFS",   Index = 5,  ButtonID = 5},
    [6]  = {Name = "SHIELDS",  Index = 6,  ButtonID = 6},
    [7]  = {Name = "HELMS",    Index = 7,  ButtonID = 7},
    [8]  = {Name = "ARMORS",   Index = 8,  ButtonID = 8},
    [9]  = {Name = "PANTS",    Index = 9,  ButtonID = 9},
    [10] = {Name = "GLOVES",   Index = 10, ButtonID = 10},
    [11] = {Name = "BOOTS",    Index = 11, ButtonID = 11},
    [12] = {Name = "WINGS",    Index = 12, ButtonID = 12},
    [13] = {Name = "PETS",     Index = 13, ButtonID = 13},
}

MarketPlace_Status = {
    ACTIVE    = 0,
    SOLD      = 1,
    CANCELLED = 2,
}

MarketPlace_Messages = {
    ["Por"] = {
        [1]  = "Item publicado com sucesso por %d %s!",
        [2]  = "Sistema desativado!",
        [3]  = "Você não pode publicar este item!",
        [4]  = "Informe o valor do item:",
        [5]  = "Valor inválido!",
        [6]  = "Saldo insuficiente para comprar este item!",
        [7]  = "Item comprado com sucesso!",
        [8]  = "Item removido do marketplace com sucesso!",
        [9]  = "Você não pode comprar seu próprio item!",
        [10] = "Item não encontrado no marketplace!",
        [11] = "Sem espaço no inventário!",
        [12] = "Você já possui um item pendente!",
        [13] = "[%s] comprou [%s] de [%s] por [%d %s] no [Marketplace]",
        [14] = "Seu item foi vendido! +%d %s",
        [15] = "Você cancelou a publicação de %s",
        [16] = "Valor mínimo é 1!",
        [17] = "Categoria: ",
        [18] = "Limite atingido! Você já possui %d/%d itens anunciados. Remova anúncio.",
    },
    ["Eng"] = {
        [1]  = "Item posted successfully for %d %s!",
        [2]  = "System disabled!",
        [3]  = "You cannot post this item!",
        [4]  = "Enter item price:",
        [5]  = "Invalid value!",
        [6]  = "Insufficient balance to buy this item!",
        [7]  = "Item purchased successfully!",
        [8]  = "Item removed from marketplace successfully!",
        [9]  = "You cannot buy your own item!",
        [10] = "Item not found in marketplace!",
        [11] = "No inventory space!",
        [12] = "You already have a pending item!",
        [13] = "[%s] comprou [%s] de [%s] por [%d %s] no [Marketplace]",
        [14] = "Your item has been sold! +%d %s",
        [15] = "You cancelled the listing of %s",
        [16] = "Minimum value is 1!",
        [17] = "Category: ",
        [18] = "Limit reached! You already have %d/%d items listed. Remove listing.",
    },
    ["Spn"] = {
        [1]  = "¡Artículo publicado por %d %s!",
        [2]  = "¡Sistema desactivado!",
        [3]  = "¡No puedes publicar este artículo!",
        [4]  = "Ingrese el precio del artículo:",
        [5]  = "¡Valor inválido!",
        [6]  = "¡Saldo insuficiente para comprar este artículo!",
        [7]  = "¡Artículo comprado exitosamente!",
        [8]  = "¡Artículo eliminado del mercado exitosamente!",
        [9]  = "¡No puedes comprar tu propio artículo!",
        [10] = "¡Artículo no encontrado en el mercado!",
        [11] = "¡Sin espacio en el inventario!",
        [12] = "¡Ya tienes un artículo pendiente!",
        [13] = "[%s] comprou [%s] de [%s] por [%d %s] no [Marketplace]",
        [14] = "¡Tu artículo ha sido vendido! +%d %s",
        [15] = "Cancelaste la publicación de %s",
        [16] = "¡El valor mínimo es 1!",
        [17] = "Categoría: ",
        [18] = "¡Límite alcanzado! Ya tienes %d/%d artículos publicados. Elimina uno.",
    },
}
