CUSTOM_MONSTER_GLOW_INFO = 
{
    { MonsterID = 600, GlowLayer = 1, GlowType = 70, GlowR = 255, GlowG = 153, GlowB = 26 },
}

function StartLoadMonsterGlow()
    for i = 1, #CUSTOM_MONSTER_GLOW_INFO do 
        local monsterGlow = CUSTOM_MONSTER_GLOW_INFO[i]

        SetMonsterGlow(monsterGlow.MonsterID, monsterGlow.GlowLayer, monsterGlow.GlowType, monsterGlow.GlowR, monsterGlow.GlowG, monsterGlow.GlowB)
    end
end

StartLoadMonsterGlow()