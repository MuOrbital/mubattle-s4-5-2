WarTeam_Config = {
    -- ================== ESTE EVENTO AINDA NÃO ESTÁ EM FUNCIONAMENTO ==================

    -- ================== ATIVAÇÃO DO EVENTO ==================
    Enabled = false,                    -- true = evento ativo | false = desativado

    -- ================== COMANDOS GM ==================
    GMCommand = "/warteam",            -- Comando que o GM usa para iniciar o evento manualmente
    GMLevel = 2,                       -- Nível mínimo de GM necessário para usar o comando

    -- ================== PACOTES (NÃO ALTERAR SE NÃO SOUBER) ==================
    PacketRegisterUI     = 100,        -- Pacote enviado quando abre a interface de registro
    PacketUpdateLists    = 101,        -- Pacote que atualiza a lista de jogadores registrados
    PacketStartEvent     = 102,        -- Pacote enviado quando o evento começa
    PacketScoreUpdate    = 103,        -- Pacote que atualiza o placar (pontos Blue x Red)
    PacketEventEnd       = 104,        -- Pacote enviado quando o evento termina
    PacketCaptureBar     = 105,        -- Pacote da barra de captura de base
    PacketCaptureState   = 106,        -- Pacote que informa quem é o dono atual de cada base

    -- ================== TEMPO DO EVENTO ==================
    Duration = 5,                      -- Duração total do evento em MINUTOS (ex: 3 = 3 minutos)

    -- ================== REGISTRO DE JOGADORES ==================
    MinPlayers = 2,                    -- Quantidade mínima de jogadores para o evento iniciar
    MaxPlayers = 50,                   -- Quantidade máxima de jogadores permitidos no evento

    -- ================== MAPA E COORDENADAS DOS TIMES ==================
    -- Time Blue
    MapA = 6,      Xa = 38,   Ya = 114,     -- Onde o Time Blue nasce no início do evento
    BaseMapA = 6,  BaseXa = 35, BaseYa = 114, -- Localização da Base do Time Blue
    BaseNpcA = 710,                         -- Classe do NPC da Base Blue
    BaseDirA = 1,                           -- Direção que o NPC da Base Blue fica olhando

    -- Time Red
    MapB = 6,      Xb = 82,   Yb = 114,     -- Onde o Time Red nasce no início do evento
    BaseMapB = 6,  BaseXb = 84, BaseYb = 114, -- Localização da Base do Time Red
    BaseNpcB = 711,                         -- Classe do NPC da Base Red
    BaseDirB = 1,                           -- Direção que o NPC da Base Red fica olhando

    -- ================== NPC DE REGISTRO ==================
    RegisterNpcClass = 255,                -- Classe do NPC onde os jogadores se registram
    RegisterMap = 0,                       -- Mapa do NPC de registro (0 = Lorencia)
    RegisterX = 137,                       -- Coordenada X do NPC de registro
    RegisterY = 122,                       -- Coordenada Y do NPC de registro
    RegisterDir = 3,                       -- Direção que o NPC de registro fica olhando

    -- ================== TEMPO DE REGISTRO ==================
    RegisterDuration = 1,                  -- Tempo de registro em MINUTOS (ex: 1 = 1 minuto)
    RegisterHour = 20,                     -- Hora automática que o evento inicia (formato 24h)
    RegisterMinute = 0,                    -- Minuto automático que o evento inicia

    -- ================== SISTEMA DE PONTOS ==================
    PointsPerKill = 1,                     -- Quantos pontos o time ganha por kill (matar inimigo)

    -- ================== CAPTURA DE BASE ==================
    CaptureTime = 10,                      -- Tempo necessário para capturar uma base (em SEGUNDOS)
    CaptureRange = 5,                      -- Raio da área de captura (agora é circular - quanto maior, maior o círculo)

    -- ================== MANUTENÇÃO DE BASE (HOLD) ==================
    HoldPoints = 2,                        -- Quantos pontos o time ganha por intervalo ao manter a base inimiga
    HoldPointsInterval = 10,               -- Intervalo em SEGUNDOS para dar os pontos de manutenção
    ShowHoldPointsMessage = false,         -- true = mostra mensagem toda vez que der pontos de hold | false = silencioso

    -- ================== TELEPORTES ==================
    DefeatedTeleportMap = 6,               -- Mapa para onde o time derrotado é teleportado
    DefeatedTeleportX = 63,                -- Coordenada X do teleport derrotado
    DefeatedTeleportY = 114,               -- Coordenada Y do teleport derrotado

    ExitMap = 0,                           -- Mapa para onde todos são teleportados ao final do evento
    ExitX = 130,                           -- Coordenada X de saída
    ExitY = 130,                           -- Coordenada Y de saída
}

-- =====================================================
-- ================== MENSAGENS DO EVENTO ================
-- =====================================================

WarTeam_Messages = {}

WarTeam_Messages["Por"] = {
    [1]  = "[WarTeam] O período de registro começou! Fale com o NPC em Atlans.",
    [2]  = "[WarTeam] Inscrições encerradas, escolhendo TIMES",
    [3]  = "[WarTeam] %s matou um inimigo! +%d ponto(s) para o Time %s.",
    [4]  = "[WarTeam] O evento começou! Boa sorte!",
    [5]  = "[WarTeam] Time %s venceu o evento com %d pontos!",
    [6]  = "Registrado com sucesso!",
    [7]  = "Evento WarTeam terminou! Vencedor: Time %s.",
    [8]  = "Você já está registrado e não pode abandonar o evento.",
    [9]  = "Quantidade mínima de jogadores não atingida (%d/%d). Evento cancelado.",
    [10] = "Você já possui um personagem registrado neste evento.",
    [11] = "Já existe um personagem registrado com esta conta.",
    [12] = "O evento já atingiu o limite máximo de jogadores.",
    [13] = "Você não está participando do evento.",
    [14] = "[WarTeam] Já existe um evento WarTeam em andamento!",
    [15] = "Você não pode atacar aliados!",
    [16] = "Sua base está dominada. Recupere sua própria base antes de tentar atacar a inimiga.",
    [17] = "Sua base já está segura. Você deve atacar a base inimiga.",
    [18] = "Esta base já pertence ao seu time.",
    [19] = "[WarTeam] %s ponto(s) adicionados ao Time %s por manter o domínio da base inimiga.",
    [20] = "[WarTeam] Time %s Capturando Base %s...",
    [21] = "Captura da base cancelada!",
    [22] = "[WarTeam] Time %s teve sucesso em capturar a Base %s!",
    [23] = "Sua base foi capturada pelo Time %s!",
    [24] = "Você já está capturando uma base!",
    [25] = "Você está longe demais da base para continuar a captura.",
    [26] = "Você morreu e sua captura foi cancelada.",
    [27] = "Recuperação concluída! A Base %s voltou para o Time %s.",
    [28] = "A captura foi interrompida porque o evento terminou.",
    [29] = "[WarTeam] Base %s Recuperando Base...",
    [30] = "[WarTeam] Time Azul %d membros / Time Red %d membros",
    [31] = "Você não pode usar o comando /pk durante o evento WarTeam!",
}

WarTeam_Messages["Eng"] = {
    [1]  = "[WarTeam] Registration period started! Talk to the NPC in Atlans.",
    [2]  = "[WarTeam] Registrations closed, choosing TEAMS",
    [3]  = "[WarTeam] %s killed an enemy! +%d point(s) for Team %s.",
    [4]  = "[WarTeam] The event has started! Good luck!",
    [5]  = "[WarTeam] Team %s won the event with %d points!",
    [6]  = "Registered successfully!",
    [7]  = "WarTeam event has ended! Winner: Team %s.",
    [8]  = "You are already registered and cannot leave the event.",
    [9]  = "Minimum number of players not reached (%d/%d). Event canceled.",
    [10] = "You already have a character registered in this event.",
    [11] = "There is already a character registered with this account.",
    [12] = "The event has already reached the maximum number of players.",
    [13] = "You are not participating in the event.",
    [14] = "[WarTeam] There is already a WarTeam event running!",
    [15] = "You cannot attack allies!",
    [16] = "Your base is dominated. Recover your own base before trying to attack the enemy base.",
    [17] = "Your base is already secure. You must attack the enemy base.",
    [18] = "This base already belongs to your team.",
    [19] = "[WarTeam] %s point(s) added to Team %s for holding the enemy base.",
    [20] = "[WarTeam] Team %s Capturing Base %s...",
    [21] = "Base capture canceled!",
    [22] = "[WarTeam] Team %s successfully captured Base %s!",
    [23] = "Your base was captured by Team %s!",
    [24] = "You are already capturing a base!",
    [25] = "You are too far from the base to continue capturing.",
    [26] = "You died and your capture was canceled.",
    [27] = "Recovery completed! Base %s returned to Team %s.",
    [28] = "The capture was interrupted because the event ended.",
    [29] = "[WarTeam] Base %s Recovering Base...",
    [30] = "[WarTeam] Blue Team %d members / Red Team %d members",
    [31] = "You cannot use the /pk command during the WarTeam event!",
}

WarTeam_Messages["Spn"] = {
    [1]  = "[WarTeam] ¡El período de registro ha comenzado! Habla con el NPC en Atlans.",
    [2]  = "[WarTeam] ¡Inscripciones cerradas, eligiendo EQUIPOS!",
    [3]  = "[WarTeam] ¡%s mató a un enemigo! +%d punto(s) para el Equipo %s.",
    [4]  = "[WarTeam] ¡El evento ha comenzado! ¡Buena suerte!",
    [5]  = "[WarTeam] ¡El Equipo %s ganó el evento con %d puntos!",
    [6]  = "¡Registrado con éxito!",
    [7]  = "¡El evento WarTeam ha terminado! Ganador: Equipo %s.",
    [8]  = "Ya estás registrado y no puedes abandonar el evento.",
    [9]  = "No se alcanzó la cantidad mínima de jugadores (%d/%d). Evento cancelado.",
    [10] = "Ya tienes un personaje registrado en este evento.",
    [11] = "Ya existe un personaje registrado con esta cuenta.",
    [12] = "El evento ya alcanzó el límite máximo de jugadores.",
    [13] = "No estás participando en el evento.",
    [14] = "[WarTeam] ¡Ya hay un evento WarTeam en curso!",
    [15] = "¡No puedes atacar a tus aliados!",
    [16] = "Tu base está dominada. Recupera tu propia base antes de intentar atacar la enemiga.",
    [17] = "Tu base ya está segura. Debes atacar la base enemiga.",
    [18] = "Esta base ya pertenece a tu equipo.",
    [19] = "[WarTeam] %s punto(s) agregados al Equipo %s por mantener el dominio de la base enemiga.",
    [20] = "[WarTeam] Equipo %s Capturando Base %s...",
    [21] = "¡Captura de base cancelada!",
    [22] = "[WarTeam] ¡El Equipo %s logró capturar la Base %s!",
    [23] = "¡Tu base fue capturada por el Equipo %s!",
    [24] = "¡Ya estás capturando una base!",
    [25] = "Estás demasiado lejos de la base para continuar la captura.",
    [26] = "Moriste y tu captura fue cancelada.",
    [27] = "¡Recuperación completada! La Base %s volvió al Equipo %s.",
    [28] = "La captura fue interrumpida porque el evento terminó.",
    [29] = "[WarTeam] Base %s Recuperando Base...",
    [30] = "[WarTeam] Equipo Azul %d miembros / Equipo Rojo %d miembros",
    [31] = "¡No puedes usar el comando /pk durante el evento WarTeam!",
}