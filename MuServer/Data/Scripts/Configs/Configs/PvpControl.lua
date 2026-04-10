PvpControl_Config = {
    Enabled = false,

    Itens = {},
}

for cat = 0, 5 do
    for id = 0, 512 do
        PvpControl_Config.Itens[Utils.GetItem(cat, id)] = {
            Visual = 0,
            Levels = {
                [10] = { Multiply = { NormalDamage = 100 }, Add = { CriticalDamageRate = 1 } },
                [11] = { Multiply = { NormalDamage = 200 }, Add = { CriticalDamageRate = 2 } },
                [12] = { Multiply = { NormalDamage = 300 }, Add = { CriticalDamageRate = 3 } },
                [13] = { Multiply = { NormalDamage = 400 }, Add = { CriticalDamageRate = 4 } },
            }
        }
    end
end

for cat = 6, 11 do
    for id = 0, 512 do
        PvpControl_Config.Itens[Utils.GetItem(cat, id)] = {
            Visual = 0,
            Levels = {
                [10] = { Multiply = { Life = 1.000449 }, Add = { CriticalDamageRate = 1 } }, -- +100 HP
                [11] = { Multiply = { Life = 1.00067 }, Add = { CriticalDamageRate = 2 } },  -- +150 HP
                [12] = { Multiply = { Life = 1.00202 }, Add = { CriticalDamageRate = 3 } },  -- +450 HP
                [13] = { Multiply = { Life = 1.0051 }, Add = { CriticalDamageRate = 4 } },  -- +700 HP
            }
        }
    end
end


PvpControl_Config.Itens[Utils.GetItem(13, 150)] = { 
    Visual = -1,
    Add = { ExcellentDamageRate = 1, Life = 0 },
    Multiply = { ExcellentDamage = 1.8, NormalDamage = 1.5, PerfectDamage = 1.5, DamageReduction = 1.10, CriticalDamage = 1.1 },
}

return PvpControl_Config
