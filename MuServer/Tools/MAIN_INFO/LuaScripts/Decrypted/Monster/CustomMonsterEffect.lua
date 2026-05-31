CUSTOM_MONSTER_EFFECT = {}

--[MonsterID]
CUSTOM_MONSTER_EFFECT[600] = { 
      { Type = 1, EffectID = 32003, EffectLv = 0, Bone = 41, Size = 0.60, ColorR = 0.05, ColorG = 0.35, ColorB = 0.96, Black = 0, RandTime = 100 }, 
} 


function StartLoadCustomMonsterEffect()
    for monsterID in pairs(CUSTOM_MONSTER_EFFECT) do 
        local monsterEffect = CUSTOM_MONSTER_EFFECT[monsterID]

        for n = 1, #monsterEffect do 
            local effect = monsterEffect[n]
            SetMonsterEffect(monsterID, effect.Type, effect.EffectID, effect.EffectLv, effect.Bone, effect.Size, effect.ColorR, effect.ColorG, effect.ColorB, effect.Black, effect.RandTime)
        end
    end
end

StartLoadCustomMonsterEffect()