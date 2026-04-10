EventVeloz_Config = {
    Enabled = true,
    Command = "/veloz",
    GMLevel = 32,
    MonsterClass = 255,
    NpcNameForMessage = "[Lumen Perdida]",
    MaxDuration = 10,
    ShowMapAndCoords = true,
    TeleportWinnerToGM = true,
    DefaultTeleportMap = 0,
    DefaultTeleportX = 152,
    DefaultTeleportY = 148,
    ReminderInterval = 30,
    Maps = {
        [0] = "Lorencia",
        [2] = "Devias",
        [3] = "Noria",
    },
}

EVENTVELOZ_MESSAGES = {}

EVENTVELOZ_MESSAGES["Por"] = {
    [1]  = "[Evento Veloz] Ainda está ativo! Procure o %s em %s (X:%d Y:%d)",
    [2]  = "[Evento Veloz] O evento terminou sem vencedores.",
    [3]  = "[Evento Veloz] PARABÉNS! %s foi o mais veloz e encontrou o tesouro!",
    [4]  = "Você encontrou o Evento Veloz! Teleportando para o GM...",
    [5]  = "[Evento Veloz] Já existe um Evento Veloz ativo no momento!",
    [6]  = "[Evento Veloz] Iniciado manualmente por GM.",
    [7]  = "[Evento Veloz] O Evento Veloz vai começar em %d minutos!",
    [8]  = "O Evento Veloz já terminou ou não está ativo no momento.",
    [9]  = "[Evento Veloz] Tempo esgotado! Ninguém encontrou o tesouro.",
    [10] = "[Evento Veloz] O vencedor %s foi teleportado para o GM.",   -- não usada mais
    [11] = "[Evento Veloz] O Evento Veloz foi finalizado manualmente.",
}

EVENTVELOZ_MESSAGES["Eng"] = {
    [1]  = "[Fast Event] Still active! Find the %s in %s (X:%d Y:%d)",
    [2]  = "[Fast Event] The event ended without winners.",
    [3]  = "[Fast Event] CONGRATULATIONS! %s was the fastest and found the treasure!",
    [4]  = "You found the Fast Event! Teleporting to the GM...",
    [5]  = "[Fast Event] There is already an active Fast Event!",
    [6]  = "[Fast Event] Manually started by GM.",
    [7]  = "[Fast Event] The Fast Event will start in %d minutes!",
    [8]  = "The Fast Event has already ended or is not active.",
    [9]  = "[Fast Event] Time's up! No one found the treasure.",
    [10] = "[Fast Event] The winner %s has been teleported to the GM.",
    [11] = "[Fast Event] The Fast Event was manually ended.",
}

EVENTVELOZ_MESSAGES["Spn"] = {
    [1]  = "[Evento Veloz] ¡Todavía activo! Busca el %s en %s (X:%d Y:%d)",
    [2]  = "[Evento Veloz] El evento terminó sin ganadores.",
    [3]  = "[Evento Veloz] ¡FELICIDADES! %s fue el más rápido y encontró el tesoro!",
    [4]  = "¡Encontraste el Evento Veloz! Teletransportando al GM...",
    [5]  = "[Evento Veloz] ¡Ya hay un Evento Veloz activo en este momento!",
    [6]  = "[Evento Veloz] Iniciado manualmente por GM.",
    [7]  = "[Evento Veloz] El Evento Veloz comenzará en %d minutos!",
    [8]  = "El Evento Veloz ya terminó o no está activo.",
    [9]  = "[Evento Veloz] ¡Se acabó el tiempo! Nadie encontró el tesoro.",
    [10] = "[Evento Veloz] El ganador %s ha sido teletransportado al GM.",
    [11] = "[Evento Veloz] El Evento Veloz fue finalizado manualmente.",
}