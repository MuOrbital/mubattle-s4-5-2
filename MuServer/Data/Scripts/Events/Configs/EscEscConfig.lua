-- EscEscConfig.lua
-- Configuração do Evento Esconde-Esconde

ESCESC_CONFIG = {
    Enabled = true,
    Command = "/escesc",
    GMLevel = 32,
    MaxDuration = 5,           -- minutos
    ReminderInterval = 1,       -- minutos entre lembretes
}

ESCESC_MESSAGES = {}
ESCESC_MESSAGES["Por"] = {
    [1]  = "[Esconde-Esconde] Já existe um evento ativo! Tempo restante: %d minutos.",
    [2]  = "[Esconde-Esconde] %s Abriu o Evento Esconde Esconde! Procure o GM e dê trade para vencer! Tempo restante: %d minutos.",
    [3]  = "[Esconde-Esconde] GM Escondido! Procure o GM e dê trade! Tempo restante: %d minutos.",
    [4]  = "[Esconde-Esconde] PARABÉNS! %s encontrou o GM e venceu o evento! Coordenadas: X:%d Y:%d",
    [5]  = "Você encontrou o GM! Trade aceito → Você venceu!",
    [6]  = "[Esconde-Esconde] Evento cancelado!",
    [7]  = "O Esconde-Esconde já terminou ou não está ativo no momento.",
    [8]  = "[Esconde-Esconde] Tempo esgotado! Ninguém encontrou o GM.",
}

ESCESC_MESSAGES["Eng"] = {
    [1]  = "[Hide and Seek] An event is already active! Remaining time: %d minutes.",
    [2]  = "[Hide and Seek] %s opened Hide and Seek Event! Find the GM and trade to win! Remaining time: %d minutes.",
    [3]  = "[Hide and Seek] GM is hidden! Find the GM and trade! Remaining time: %d minutes.",
    [4]  = "[Hide and Seek] CONGRATULATIONS! %s found the GM and won! Coordinates: X:%d Y:%d",
    [5]  = "You found the GM! Trade accepted → You won!",
    [6]  = "[Hide and Seek] Event cancelled!",
    [7]  = "Hide and Seek has already ended or is not active.",
    [8]  = "[Hide and Seek] Time's up! No one found the GM.",
}

ESCESC_MESSAGES["Spn"] = {
    [1]  = "[Esconde-Esconde] ¡Ya hay un evento activo! Tiempo restante: %d minutos.",
    [2]  = "[Esconde-Esconde] %s abrió el Evento Esconde-Esconde! ¡Busca al GM y haz trade para ganar! Tiempo restante: %d minutos.",
    [3]  = "[Esconde-Esconde] ¡GM escondido! ¡Busca al GM y haz trade! Tiempo restante: %d minutos.",
    [4]  = "[Esconde-Esconde] ¡FELICIDADES! %s encontró al GM y ganó! Coordenadas: X:%d Y:%d",
    [5]  = "¡Encontraste al GM! Trade aceptado → ¡Ganaste!",
    [6]  = "[Esconde-Esconde] ¡Evento cancelado!",
    [7]  = "El Esconde-Esconde ya terminó o no está activo.",
    [8]  = "[Esconde-Esconde] ¡Se acabó el tiempo! Nadie encontró al GM.",
}