OpenFolder("Definitions")
---Requisitos de Resets para entrar no Eventos
BLOOD_CASTLE = {
	{Index = 0, CommonMinLevel = 0, CommonMaxLevel = 5, SpecialMinLevel = 1, SpecialMaxLevel = 4},
	{Index = 1, CommonMinLevel = 6, CommonMaxLevel = 10, SpecialMinLevel = 5, SpecialMaxLevel = 8},
	{Index = 2, CommonMinLevel = 11, CommonMaxLevel = 20, SpecialMinLevel = 9, SpecialMaxLevel = 17},
	{Index = 3, CommonMinLevel = 21, CommonMaxLevel = 40, SpecialMinLevel = 18, SpecialMaxLevel = 36},
	{Index = 4, CommonMinLevel = 41, CommonMaxLevel = 70, SpecialMinLevel = 37, SpecialMaxLevel = 65},
	{Index = 5, CommonMinLevel = 71, CommonMaxLevel = 110, SpecialMinLevel = 66, SpecialMaxLevel = 100},
	{Index = 6, CommonMinLevel = 111, CommonMaxLevel = 500, SpecialMinLevel = 101, SpecialMaxLevel = 500},
}

DEVIL_SQUARE = {
	{Index = 0, CommonMinLevel = 1, CommonMaxLevel = 5, SpecialMinLevel = 1, SpecialMaxLevel = 4},
	{Index = 1, CommonMinLevel = 6, CommonMaxLevel = 10, SpecialMinLevel = 5, SpecialMaxLevel = 8},
	{Index = 2, CommonMinLevel = 11, CommonMaxLevel = 20, SpecialMinLevel = 9, SpecialMaxLevel = 17},
	{Index = 3, CommonMinLevel = 21, CommonMaxLevel = 40, SpecialMinLevel = 18, SpecialMaxLevel = 36},
	{Index = 4, CommonMinLevel = 41, CommonMaxLevel = 70, SpecialMinLevel = 37, SpecialMaxLevel = 65},
	{Index = 5, CommonMinLevel = 71, CommonMaxLevel = 500, SpecialMinLevel = 66, SpecialMaxLevel = 500},
}

CHAOS_CASTLE = {
	{Index = 0, CommonMinLevel = 1, CommonMaxLevel = 5, SpecialMinLevel = 1, SpecialMaxLevel = 4},
	{Index = 1, CommonMinLevel = 6, CommonMaxLevel = 10, SpecialMinLevel = 5, SpecialMaxLevel = 8},
	{Index = 2, CommonMinLevel = 11, CommonMaxLevel = 20, SpecialMinLevel = 9, SpecialMaxLevel = 17},
	{Index = 3, CommonMinLevel = 21, CommonMaxLevel = 40, SpecialMinLevel = 18, SpecialMaxLevel = 36},
	{Index = 4, CommonMinLevel = 41, CommonMaxLevel = 70, SpecialMinLevel = 37, SpecialMaxLevel = 65},
	{Index = 5, CommonMinLevel = 71, CommonMaxLevel = 500, SpecialMinLevel = 66, SpecialMaxLevel = 500},
}
------------------------------------

---Requisitos de Level para entrar no Illusion Temple
ILLUSION_TEMPLE = {
	{Index = 0, MinLevel = 1, MaxLevel = 5},
	{Index = 1, MinLevel = 6, MaxLevel = 10},
	{Index = 2, MinLevel = 11, MaxLevel = 20},
	{Index = 3, MinLevel = 21, MaxLevel = 40},
	{Index = 4, MinLevel = 41, MaxLevel = 500},
}

function LoadEventEntryLevel()
	for _, event in ipairs(BLOOD_CASTLE) do
		SetBloodCastleInfo(event.Index, event.CommonMinLevel, event.CommonMaxLevel, event.SpecialMinLevel, event.SpecialMaxLevel)
	end
	
	for _, event in ipairs(DEVIL_SQUARE) do
		SetDevilSquareInfo(event.Index, event.CommonMinLevel, event.CommonMaxLevel, event.SpecialMinLevel, event.SpecialMaxLevel)
	end
	
	for _, event in ipairs(CHAOS_CASTLE) do
		SetChaosCastleInfo(event.Index, event.CommonMinLevel, event.CommonMaxLevel, event.SpecialMinLevel, event.SpecialMaxLevel)
	end
	
	for _, event in ipairs(ILLUSION_TEMPLE) do
		SetIllusionTempleInfo(event.Index, event.MinLevel, event.MaxLevel)
	end
end