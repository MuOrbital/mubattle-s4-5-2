CUSTOM_RANKING_SWITCH = 0
CUSTOM_RANKING_PACKET = 0x56
CUSTOM_RANKING_PACKET_NAME = "CUSTOM_RANKING_PACKET"
CUSTOM_RANKING_MAX_RESULT = 12
CUSTOM_RANKING_MAX_RANKING = 5

CUSTOM_RANKINGS = {
    [1] = { Nome = "Reset Diário", Table = "Character", Score = "ResetDay", Name = "Name" },
    [2] = { Nome = "Reset Semanal", Table = "Character", Score = "ResetWeek", Name = "Name" },
    [3] = { Nome = "Reset Mensal", Table = "Character", Score = "ResetMonth", Name = "Name" },
    [4] = { Nome = "Reset Geral", Table = "Character", Score = "ResetCount", Name = "Name" },   
    [5] = { Nome = "Top M.Resets", Table = "Character", Score = "MasterResetCount", Name = "Name" },
}

CUSTOM_CLASS_NAMES = {
    [0]  = "DW",    [1]  = "SM",    [2]  = "GrM",
    [16] = "DK",    [17] = "BK",    [18] = "BM",
    [32] = "ELF",   [33] = "ME",    [34] = "HE",
    [48] = "MG",    [50] = "DM",
    [64] = "DL",    [66] = "LE",
    [80] = "SUM",   [81] = "BS",    [82] = "DM/SUM",
}

CUSTOM_RANKING_TEXTS = {}
CUSTOM_RANKING_TEXTS["Por"] = {
    [1] = "Ranking", 
    [2] = "Posição", 
    [3] = "Personagem",
    [4] = "Level", 
    [5] = "Classe", 
    [6] = "Online", 
    [7] = "Score", 
    [8] = "Sistema desativado",
}
CUSTOM_RANKING_TEXTS["Eng"] = {
    [1] = "Ranking", 
    [2] = "Position", 
    [3] = "Character",
    [4] = "Level", 
    [5] = "Class", 
    [6] = "Online", 
    [7] = "Score", 
    [8] = "System disabled",
}
CUSTOM_RANKING_TEXTS["Spn"] = {
    [1] = "Ranking", 
    [2] = "Posición", 
    [3] = "Personaje",
    [4] = "Nivel", 
    [5] = "Clase", 
    [6] = "Online", 
    [7] = "Puntos", 
    [8] = "Sistema desactivado",
}

return true