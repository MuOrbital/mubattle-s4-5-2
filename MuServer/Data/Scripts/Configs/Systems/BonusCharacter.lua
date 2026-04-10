-----------------------------------------------------
-- Character Reward By: Waal
-----------------------------------------------------

CharacterReward = {}

CharacterReward.Config = {
    Enabled = false,

    ZenReward = 500000,

    Stats = {
        [0] = {Str = 18, Agi = 18, Vit = 15, Ene = 30},  -- Dark Wizard
        [1] = {Str = 28, Agi = 20, Vit = 25, Ene = 10},  -- Dark Knight
        [2] = {Str = 22, Agi = 25, Vit = 20, Ene = 15},  -- Elf
        [3] = {Str = 26, Agi = 26, Vit = 26, Ene = 26},  -- Magic Gladiator
        [4] = {Str = 26, Agi = 20, Vit = 20, Ene = 15},  -- Dark Lord
        [5] = {Str = 21, Agi = 21, Vit = 18, Ene = 23},  -- Summoner
    },

    Items = {
        [0] = { -- DW
            {Section = 7, Index = 2, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 8, Index = 2, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 9, Index = 2, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 10, Index = 2, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 11, Index = 2, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 5, Index = 0, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 6, Index = 4, Level = 9, Luck = 1, Skill = 1, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 12, Index = 133, Level = 0, Luck = 0, Skill = 0, Option = 0, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 13, Index = 76, Level = 0, Luck = 0, Skill = 0, Option = 0, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 13, Index = 80, Level = 0, Luck = 0, Skill = 0, Option = 0, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
        },
        [1] = { -- DK
            {Section = 7, Index = 0, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 8, Index = 0, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 9, Index = 0, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 10, Index = 0, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 11, Index = 0, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 0, Index = 4, Level = 9, Luck = 1, Skill = 1, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 6, Index = 1, Level = 9, Luck = 1, Skill = 1, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 12, Index = 134, Level = 0, Luck = 0, Skill = 0, Option = 0, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 13, Index = 76, Level = 0, Luck = 0, Skill = 0, Option = 0, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 13, Index = 80, Level = 0, Luck = 0, Skill = 0, Option = 0, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
        },
        [2] = { -- ELF
            {Section = 7, Index = 10, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 8, Index = 10, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 9, Index = 10, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 10, Index = 10, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 11, Index = 10, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 4, Index = 1, Level = 9, Luck = 1, Skill = 1, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 12, Index = 132, Level = 0, Luck = 0, Skill = 0, Option = 0, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 13, Index = 76, Level = 0, Luck = 0, Skill = 0, Option = 0, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 13, Index = 80, Level = 0, Luck = 0, Skill = 0, Option = 0, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
        },
        [3] = { -- MG
            {Section = 8, Index = 15, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 9, Index = 15, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 10, Index = 15, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 11, Index = 15, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 0, Index = 4, Level = 9, Luck = 1, Skill = 1, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 6, Index = 1, Level = 9, Luck = 1, Skill = 1, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 12, Index = 134, Level = 0, Luck = 0, Skill = 0, Option = 0, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 13, Index = 76, Level = 0, Luck = 0, Skill = 0, Option = 0, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 13, Index = 80, Level = 0, Luck = 0, Skill = 0, Option = 0, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
        },
        [4] = { -- DL
            {Section = 7, Index = 25, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 8, Index = 25, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 9, Index = 25, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 10, Index = 25, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 11, Index = 25, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 2, Index = 8, Level = 9, Luck = 1, Skill = 1, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 6, Index = 5, Level = 9, Luck = 1, Skill = 1, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 12, Index = 130, Level = 0, Luck = 0, Skill = 0, Option = 0, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 13, Index = 76, Level = 0, Luck = 0, Skill = 0, Option = 0, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 13, Index = 80, Level = 0, Luck = 0, Skill = 0, Option = 0, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
        },
        [5] = { -- SUM
            {Section = 7, Index = 39, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 8, Index = 39, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 9, Index = 39, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 10, Index = 39, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 11, Index = 39, Level = 9, Luck = 1, Skill = 0, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 5, Index = 14, Level = 9, Luck = 1, Skill = 1, Option = 7, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 12, Index = 13, Level = 0, Luck = 0, Skill = 0, Option = 0, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 13, Index = 76, Level = 0, Luck = 0, Skill = 0, Option = 0, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
            {Section = 13, Index = 80, Level = 0, Luck = 0, Skill = 0, Option = 0, Dur = 255, Time = 259200, Exc = 0, Ancient = 0, JoL = 0, Socket = {0, 0, 0, 0, 0, 0 } },
        },
    },
}

function CharacterReward.Init()
    if not CharacterReward.Config.Enabled then return end
    GameServerFunctions.EnterCharacter(CharacterReward.OnEnterCharacter)
end

function CharacterReward.OnEnterCharacter(aIndex)
    local player = User.new(aIndex)
    if not player then return end

    local name = player:getName()
    local level = player:getLevel()
    local bonus = tonumber(DataBase.GetValue("Character", "BonusIniciante", "Name", name)) or 0
    if bonus > 0 then return end

    if level <= 10 then
        DataBase.Query(string.format("UPDATE Character SET BonusIniciante = 1 WHERE Name = '%s'", name))
        CharacterReward.ApplyReward(aIndex)
    end
end

function CharacterReward.ApplyReward(aIndex)
    local player = User.new(aIndex)
    if not player then return end

    local classe = player:getClass()
    local confStats = CharacterReward.Config.Stats[classe]
    local confItems = CharacterReward.Config.Items[classe]

    if confStats then
        player:setStrength(confStats.Str)
        player:setDexterity(confStats.Agi)
        player:setVitality(confStats.Vit)
        player:setEnergy(confStats.Ene)
    end

    if confItems then
        for _, item in ipairs(confItems) do
            local itemIndex = Utils.GetItem(item.Section, item.Index)
            CreateItemInventory(
                aIndex, itemIndex,
                item.Level or 0, item.Dur or 0,
                item.Skill or 0, item.Luck or 0, item.Option or 0,
                item.Exc or 0, item.Ancient or 0, item.JoL or 0,
                item.Socket[1] or 255, item.Socket[2] or 255,
                item.Socket[3] or 255, item.Socket[4] or 255,
                item.Socket[5] or 255, item.Socket[6] or 255,
                item.Time or 0 
            )
        end
    end

    if CharacterReward.Config.ZenReward and CharacterReward.Config.ZenReward > 0 then
        local newMoney = player:getMoney() + CharacterReward.Config.ZenReward
        player:setMoney(newMoney)
        MoneySend(aIndex)
    end

    player:updateStats()
    player:updateAll()
end

CharacterReward.Init()
return CharacterReward
