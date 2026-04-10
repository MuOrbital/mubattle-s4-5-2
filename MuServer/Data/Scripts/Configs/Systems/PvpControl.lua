
PvpControl = {}


local function allowedByVisual(visual, slot)
    if visual == nil or visual == -1 then return true end
    if visual == 0 then return slot <= 12 end
    if visual == 1 then return slot >= 13 end
    return true
end

local function pickLevelNode(itemCfg, level)
    if itemCfg and itemCfg.Levels and itemCfg.Levels[level] then
        local node = itemCfg.Levels[level]
        if node.Visual == nil then node.Visual = itemCfg.Visual end
        return node
    end
    return itemCfg
end

local function applyAdd(player, add)
    if not add then return end
    if add.Defense                     ~= nil then player:setUserTable(2,  player:getUserTable(2)  + add.Defense) end
    if add.MissRate                    ~= nil then player:setUserTable(4,  player:getUserTable(4)  + add.MissRate) end
    if add.Life                        ~= nil then player:setUserTable(5,  player:getUserTable(5)  + add.Life) end
    if add.Mana                        ~= nil then player:setUserTable(6,  player:getUserTable(6)  + add.Mana) end
    if add.PhysicSpeed                 ~= nil then player:setUserTable(13, player:getUserTable(13) + add.PhysicSpeed) end
    if add.MagicSpeed                  ~= nil then player:setUserTable(14, player:getUserTable(14) + add.MagicSpeed) end
    if add.FullDamageReflectRate       ~= nil then player:setUserTable(19, player:getUserTable(19) + add.FullDamageReflectRate) end
    if add.AttackSuccessRate           ~= nil then player:setUserTable(31, player:getUserTable(31) + add.AttackSuccessRate) end
    if add.DamageReduction             ~= nil then player:setUserTable(22, player:getUserTable(22) + add.DamageReduction) end
    if add.ShieldDamageReduction       ~= nil then player:setUserTable(23, player:getUserTable(23) + add.ShieldDamageReduction) end
    if add.ExcellentDamageRate         ~= nil then player:setUserTable(16, player:getUserTable(16) + add.ExcellentDamageRate) end
    if add.ResistExcellentDamageRate   ~= nil then player:setUserTable(17, player:getUserTable(17) + add.ResistExcellentDamageRate) end
    if add.DoubleDamageRate            ~= nil then player:setUserTable(25, player:getUserTable(25) + add.DoubleDamageRate) end
    if add.ResistDoubleDamageRate      ~= nil then player:setUserTable(26, player:getUserTable(26) + add.ResistDoubleDamageRate) end
    if add.CriticalDamageRate          ~= nil then player:setUserTable(28, player:getUserTable(28) + add.CriticalDamageRate) end
    if add.ResistCriticalDamageRate    ~= nil then player:setUserTable(29, player:getUserTable(29) + add.ResistCriticalDamageRate) end
    if add.TripleDamageRate            ~= nil then player:setUserTable(24, player:getUserTable(24) + add.TripleDamageRate) end
    if add.IgnoreShieldGaugeRate       ~= nil then player:setUserTable(32, player:getUserTable(32) + add.IgnoreShieldGaugeRate) end
    if add.ResistIgnoreShieldGaugeRate ~= nil then player:setUserTable(33, player:getUserTable(33) + add.ResistIgnoreShieldGaugeRate) end

    if add.NormalDamage    ~= nil then player:setCacheInt("A_NormalDamage",    player:getCacheInt("A_NormalDamage")    + add.NormalDamage) end
    if add.PerfectDamage   ~= nil then player:setCacheInt("A_PerfectDamage",   player:getCacheInt("A_PerfectDamage")   + add.PerfectDamage) end
    if add.ExcellentDamage ~= nil then player:setCacheInt("A_ExcellentDamage", player:getCacheInt("A_ExcellentDamage") + add.ExcellentDamage) end
    if add.CriticalDamage  ~= nil then player:setCacheInt("A_CriticalDamage",  player:getCacheInt("A_CriticalDamage")  + add.CriticalDamage) end
    if add.ReflectDamage   ~= nil then player:setCacheInt("A_ReflectDamage",   player:getCacheInt("A_ReflectDamage")   + add.ReflectDamage) end
    if add.PoisonDamage    ~= nil then player:setCacheInt("A_PoisonDamage",    player:getCacheInt("A_PoisonDamage")    + add.PoisonDamage) end
end

local function applyMultiply(player, mul)
    if not mul then return end
    if mul.Defense                     ~= nil then player:setUserTable(2,  player:getUserTable(2)  + ((player:getUserTable(1)*mul.Defense)-player:getUserTable(1))) end
    if mul.MissRate                    ~= nil then player:setUserTable(4,  player:getUserTable(4)  + ((player:getUserTable(3)*mul.MissRate)-player:getUserTable(3))) end
    if mul.Life                        ~= nil then player:setUserTable(5,  player:getUserTable(5)  + ((player:getLife()*mul.Life)-player:getLife())) end
    if mul.Mana                        ~= nil then player:setUserTable(6,  player:getUserTable(6)  + ((player:getMana()*mul.Mana)-player:getMana())) end
    if mul.PhysicSpeed                 ~= nil then player:setUserTable(13, player:getUserTable(13) * mul.PhysicSpeed) end
    if mul.MagicSpeed                  ~= nil then player:setUserTable(14, player:getUserTable(14) * mul.MagicSpeed) end
    if mul.FullDamageReflectRate       ~= nil then player:setUserTable(19, player:getUserTable(19) * mul.FullDamageReflectRate) end
    if mul.AttackSuccessRate           ~= nil then player:setUserTable(31, player:getUserTable(31) * mul.AttackSuccessRate) end
    if mul.DamageReduction             ~= nil then player:setUserTable(22, player:getUserTable(22) * mul.DamageReduction) end
    if mul.ShieldDamageReduction       ~= nil then player:setUserTable(23, player:getUserTable(23) * mul.ShieldDamageReduction) end
    if mul.ExcellentDamageRate         ~= nil then player:setUserTable(16, player:getUserTable(16) * mul.ExcellentDamageRate) end
    if mul.ResistExcellentDamageRate   ~= nil then player:setUserTable(17, player:getUserTable(17) * mul.ResistExcellentDamageRate) end
    if mul.DoubleDamageRate            ~= nil then player:setUserTable(25, player:getUserTable(25) * mul.DoubleDamageRate) end
    if mul.ResistDoubleDamageRate      ~= nil then player:setUserTable(26, player:getUserTable(26) * mul.ResistDoubleDamageRate) end
    if mul.CriticalDamageRate          ~= nil then player:setUserTable(28, player:getUserTable(28) * mul.CriticalDamageRate) end
    if mul.ResistCriticalDamageRate    ~= nil then player:setUserTable(29, player:getUserTable(29) * mul.ResistCriticalDamageRate) end
    if mul.TripleDamageRate            ~= nil then player:setUserTable(24, player:getUserTable(24) * mul.TripleDamageRate) end
    if mul.IgnoreShieldGaugeRate       ~= nil then player:setUserTable(32, player:getUserTable(32) * mul.IgnoreShieldGaugeRate) end
    if mul.ResistIgnoreShieldGaugeRate ~= nil then player:setUserTable(33, player:getUserTable(33) * mul.ResistIgnoreShieldGaugeRate) end

    if mul.NormalDamage    ~= nil then player:setCacheInt("M_NormalDamage",    player:getCacheInt("M_NormalDamage")    + ((mul.NormalDamage    - 1) * 10000)) end
    if mul.PerfectDamage   ~= nil then player:setCacheInt("M_PerfectDamage",   player:getCacheInt("M_PerfectDamage")   + ((mul.PerfectDamage   - 1) * 10000)) end
    if mul.ExcellentDamage ~= nil then player:setCacheInt("M_ExcellentDamage", player:getCacheInt("M_ExcellentDamage") + ((mul.ExcellentDamage - 1) * 10000)) end
    if mul.CriticalDamage  ~= nil then player:setCacheInt("M_CriticalDamage",  player:getCacheInt("M_CriticalDamage")  + ((mul.CriticalDamage  - 1) * 10000)) end
    if mul.ReflectDamage   ~= nil then player:setCacheInt("M_ReflectDamage",   player:getCacheInt("M_ReflectDamage")   + ((mul.ReflectDamage   - 1) * 10000)) end
    if mul.PoisonDamage    ~= nil then player:setCacheInt("M_PoisonDamage",    player:getCacheInt("M_PoisonDamage")    + ((mul.PoisonDamage    - 1) * 10000)) end
end



function PvpControl.Init()
    if not PvpControl_Config.Enabled then return end
    GameServerFunctions.CharacterSet(PvpControl.CharacterSet)
    GameServerFunctions.PlayerCheckDamage(PvpControl.CheckDamage)
end

function PvpControl.CharacterSet(aIndex)
    local player = User.new(aIndex)
    local pInv   = Inventory.new(aIndex)

    local slots = {
        0,1,2,3,4,5,6,7,8,8,10,11,
        236,237,238,239,240,241,242,243,244,245,246,247,248,250 
    }

    player:setCacheInt("A_NormalDamage", 0)
    player:setCacheInt("A_PerfectDamage", 0)
    player:setCacheInt("A_ExcellentDamage", 0)
    player:setCacheInt("A_CriticalDamage", 0)
    player:setCacheInt("A_ReflectDamage", 0)
    player:setCacheInt("A_PoisonDamage", 0)

    player:setCacheInt("M_NormalDamage", 0)
    player:setCacheInt("M_PerfectDamage", 0)
    player:setCacheInt("M_ExcellentDamage", 0)
    player:setCacheInt("M_CriticalDamage", 0)
    player:setCacheInt("M_ReflectDamage", 0)
    player:setCacheInt("M_PoisonDamage", 0)

    for _, slot in ipairs(slots) do
        local idx = pInv:getIndex(slot)
        if idx == -1 then goto continue end

        local itemCfg = PvpControl_Config.Itens[idx]
        if not itemCfg then goto continue end

        local level = pInv:getLevel(slot) or 0
        if level < 0 then level = 0 end
        if level > 15 then level = 15 end

        local node = pickLevelNode(itemCfg, level)
        if not node then goto continue end

        if not allowedByVisual(node.Visual, slot) then goto continue end

        applyAdd(player, node.Add)
        applyMultiply(player, node.Multiply)

        ::continue::
    end

    pInv = nil
    player = nil
end

function PvpControl.CheckDamage(aIndex, TargetIndex, Damage, DamageType)
    local newDamage = Damage
    local player = User.new(aIndex)

    local m_damage = 0
    local a_damage = 0
    if DamageType == 0 then
        m_damage = 1 + (player:getCacheInt("M_NormalDamage")    / 10000)
        a_damage = player:getCacheInt("A_NormalDamage")
    elseif DamageType == 1 then
        m_damage = 1 + (player:getCacheInt("M_PerfectDamage")   / 10000)
        a_damage = player:getCacheInt("A_PerfectDamage")
    elseif DamageType == 2 then
        m_damage = 1 + (player:getCacheInt("M_ExcellentDamage") / 10000)
        a_damage = player:getCacheInt("A_ExcellentDamage")
    elseif DamageType == 3 then
        m_damage = 1 + (player:getCacheInt("M_CriticalDamage")  / 10000)
        a_damage = player:getCacheInt("A_CriticalDamage")
    elseif DamageType == 4 then
        m_damage = 1 + (player:getCacheInt("M_ReflectDamage")   / 10000)
        a_damage = player:getCacheInt("A_ReflectDamage")
    elseif DamageType == 5 then
        m_damage = 1 + (player:getCacheInt("M_PoisonDamage")    / 10000)
        a_damage = player:getCacheInt("A_PoisonDamage")
    end

    if m_damage > 0 then newDamage = Damage * m_damage end
    if a_damage > 0 then newDamage = newDamage + a_damage end

    player = nil
    return newDamage
end

PvpControl.Init()

return PvpControl
