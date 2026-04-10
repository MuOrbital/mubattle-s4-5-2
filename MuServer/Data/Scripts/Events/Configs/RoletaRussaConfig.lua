--[[
    +--------------------------------------------------------------------+
    | Roleta Russa - Configurações                                       |
    |--------------------------------------------------------------------|
    | Criado por: Mineiro                                                |
    | Descrição: Configurações e mensagens do evento Roleta Russa       |
    +--------------------------------------------------------------------+
--]]

RoletaRussa_Config = {
    Enabled         = true,
    CommandOpen     = "/roletarussa",
    CommandJoin     = "/queroir",
    JoinTime        = 120,
    MinPlayers      = 2,
    MaxPlayers      = 50,
    ChanceSurvive   = 40,           -- % de chance de sobreviver a cada rodada
    GMLevel         = 2,            -- nível mínimo de autoridade/ctlcode para abrir
    EventMap        = 33,           -- Número do mapa
    EventX          = 250,          -- Coordenada X
    EventY          = 155,          -- Coordenada Y
    DeathEffectID   = 58,           -- ID do efeito
    AUTO_START = nil,               -- Evento Automático (in dev)
}

RoletaRussa_Messages = {}
RoletaRussa_Messages["Por"] = {
    [1]  = "==== Roleta Russa ====",
    [2]  = "Comando para participar: %s",
    [3]  = "Jogador [%d] %s entrou! (%d/%d)",
    [4]  = "Inscrições encerradas! %d jogadores participam.",
    [5]  = "Não há jogadores suficientes. Evento cancelado. (mínimo %d)",
    [6]  = "Jogador [%d] %s foi selecionado!",
    [7]  = "Colocando a bala no tambor...",
    [8]  = "Girando o tambor...",
    [9]  = "Engatilhando...",
    [10] = "FOGO!",
    [11] = "Jogador [%d] %s ESCAPOU ILESO!",
    [12] = "Jogador [%d] %s morreu! ELIMINADO!",
    [13] = "Parabéns! %s é o campeão da Roleta Russa!",
    [14] = "Evento Roleta Russa iniciado! Inscrições abertas por %d segundos.",
    [15] = "Tempo restante para entrar: %d segundos...",
    [16] = "Você já está participando!",
    [17] = "Evento já está em andamento ou inscrições fechadas.",
    [18] = "Bem Vindo(a) ao Roleta Russa!",
}

RoletaRussa_Messages["Eng"] = {
    [1]  = "==== Russian Roulette ====",
    [2]  = "Join command: %s",
    [3]  = "Player [%d] %s has joined! (%d/%d)",
    [4]  = "Registrations closed! %d players participating.",
    [5]  = "Not enough players. Event canceled. (minimum %d)",
    [6]  = "Player [%d] %s has been selected!",
    [7]  = "Loading the bullet...",
    [8]  = "Spinning the cylinder...",
    [9]  = "Pulling the trigger...",
    [10] = "BANG!",
    [11] = "Player [%d] %s ESCAPED UNHARMED!",
    [12] = "Player [%d] %s died! ELIMINATED!",
    [13] = "Congratulations! %s is the Russian Roulette champion!",
    [14] = "Russian Roulette event started! Registrations open for %d seconds.",
    [15] = "Time left to join: %d seconds...",
    [16] = "You are already participating!",
    [17] = "Event already running or registrations closed.",
    [18] = "Welcome to Russian Roulette!",
}

RoletaRussa_Messages["Spn"] = {
    [1]  = "==== Ruleta Rusa ====",
    [2]  = "Comando para participar: %s",
    [3]  = "Jugador [%d] %s entró! (%d/%d)",
    [4]  = "Inscripciones cerradas! %d jugadores participan.",
    [5]  = "No hay suficientes jugadores. Evento cancelado. (mínimo %d)",
    [6]  = "¡Jugador [%d] %s ha sido seleccionado!",
    [7]  = "Colocando la bala en el tambor...",
    [8]  = "Girando el tambor...",
    [9]  = "Amartillando...",
    [10] = "¡BANG!",
    [11] = "¡Jugador [%d] %s ESCAPÓ ILESO!",
    [12] = "¡Jugador [%d] %s murió! ELIMINADO!",
    [13] = "¡Felicidades! %s es el campeón de la Ruleta Rusa!",
    [14] = "¡Evento Ruleta Rusa iniciado! Inscripciones abiertas por %d segundos.",
    [15] = "Tiempo restante para entrar: %d segundos...",
    [16] = "¡Ya estás participando!",
    [17] = "Evento ya en curso o inscripciones cerradas.",
    [18] = "¡Bienvenido(a) a la Ruleta Rusa!",
}