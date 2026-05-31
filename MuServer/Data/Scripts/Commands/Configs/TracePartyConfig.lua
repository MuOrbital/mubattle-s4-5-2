-- =================================================
-- Script de Configs - Party Trace System
-- =================================================

PT_TRACE_SWITCH = 1
PT_TRACE_COMMAND_TRACE = "/puxarpt"
PT_TRACE_COMMAND_TRACK = "/irpt"

PT_TRACE_TRACE_VIP = 1
PT_TRACE_TRACK_VIP = 1

-- ================== MAPAS BLOQUEADOS ==================
-- Coloque = false nos mapas onde os comandos /puxarpt e /irpt NÃO devem funcionar
NO_TRACE_MAPS = {}
NO_TRACE_MAPS[24] = false --Kalima 1
NO_TRACE_MAPS[25] = false --Kalima 2
NO_TRACE_MAPS[26] = false --Kalima 3
NO_TRACE_MAPS[27] = false --Kalima 4
NO_TRACE_MAPS[28] = false --Kalima 5
NO_TRACE_MAPS[29] = false --Kalima 6
NO_TRACE_MAPS[36] = false --Kalima 7
NO_TRACE_MAPS[9]  =  false --Devil Square 1
NO_TRACE_MAPS[32] =  false --Devil Square 2
NO_TRACE_MAPS[10] = false --Icarus
NO_TRACE_MAPS[30] = false --Castle Siege
NO_TRACE_MAPS[31] = false --Land of Trials
NO_TRACE_MAPS[40] = false --Silent
NO_TRACE_MAPS[41] = false --Barracks
NO_TRACE_MAPS[42] = false --Refuge
NO_TRACE_MAPS[58] = false --RakilionBoss (Selupan)
NO_TRACE_MAPS[62] = false --Santa Town
NO_TRACE_MAPS[64] = false --Duel Arena (Coliseu Vulcanus)
NO_TRACE_MAPS[65] = false --Double Goer 1
NO_TRACE_MAPS[66] = false --Double Goer 2
NO_TRACE_MAPS[67] = false --Double Goer 3
NO_TRACE_MAPS[68] = false --Double Goer 4
NO_TRACE_MAPS[69] = false --Imperial Guardian 1
NO_TRACE_MAPS[70] = false --Imperial Guardian 2
NO_TRACE_MAPS[71] = false --Imperial Guardian 3
NO_TRACE_MAPS[72] = false --Imperial Guardian 4
NO_TRACE_MAPS[18] = false --Chaos Castle 1
NO_TRACE_MAPS[19] = false --Chaos Castle 2
NO_TRACE_MAPS[20] = false --Chaos Castle 3
NO_TRACE_MAPS[21] = false --Chaos Castle 4
NO_TRACE_MAPS[22] = false --Chaos Castle 5
NO_TRACE_MAPS[23] = false --Chaos Castle 6
NO_TRACE_MAPS[53] = false --Chaos Castle 7
NO_TRACE_MAPS[11] = false --Blood Castle 1
NO_TRACE_MAPS[12] = false --Blood Castle 2
NO_TRACE_MAPS[13] = false --Blood Castle 3
NO_TRACE_MAPS[14] = false --Blood Castle 4
NO_TRACE_MAPS[15] = false --Blood Castle 5
NO_TRACE_MAPS[16] = false --Blood Castle 6
NO_TRACE_MAPS[17] = false --Blood Castle 7
NO_TRACE_MAPS[52] = false --Blood Castle 7
NO_TRACE_MAPS[39] = false --Kanturu 3 (Maya e Torre de Refinaria)

-- =================================================

PT_TRACE_MESSAGES = {}
PT_TRACE_MESSAGES["Por"] = {
    [1] = "Você não é o Líder da Party.",
    [2] = "Você não tem o VIP necessário para usar este comando.",
    [3] = "Puxando membros da Party.",
    [4] = "Movendo você até o Líder da Party.",
    [5] = "Você não está em nenhuma Party.",
    [6] = "Você é o Lider da Party.",
    [7] = "Você não pode ir até este mapa.",
    [8] = "Você não pode puxar para este mapa."
}

PT_TRACE_MESSAGES["Eng"] = {
    [1] = "You are not the Party Leader.",
    [2] = "You do not have the required VIP to use this command.",
    [3] = "Pulling Party members.",
    [4] = "Moving you to the Party Leader.",
    [5] = "You are not in any Party.",
    [6] = "You are the Party Leader.",
    [7] = "You cannot go to this map.",
    [8] = "You cannot pull to this map."
}

PT_TRACE_MESSAGES["Spn"] = {
    [1] = "No eres el Líder del Party.",
    [2] = "No tienes el VIP necesario para usar este comando.",
    [3] = "Trayendo a los miembros del Party.",
    [4] = "Moviendo hacia el Líder del Party.",
    [5] = "No estás en ningún Party.",
    [6] = "Tú eres el líder del partido.",
    [7] = "No puedes ir a este mapa.",
    [8] = "No puedes traer al party a este mapa."
}