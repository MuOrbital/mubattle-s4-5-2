--[[
    ╔════════════════════════════════════════════════════════════════════╗
    ║               MuFC Event Script - MuOrbital Season 4               ║
    ║════════════════════════════════════════════════════════════════════║
    ║ Criado por: Mineiro MuOnline                                       ║
    ║ Site: https://youtube.com/@mineiromuonline                         ║
    ║ 						                         ║
    ║ Emulador: Rox Gaming - MuOnline Developer                          ║
    ║ Site: https://roxgaming.net 		                         ║
    ║ 				 		                         ║
    ║ Servidor: MuOrbital Season 4                                       ║
    ║ Site: https://orbital-muonline.net                                 ║
    ║                                                                    ║
    ║ Descrição: Script para o evento MuFC, um torneio PvP com suporte   ║
    ║ a diferentes classes, configurações automáticas e premiações.      ║
    ╚════════════════════════════════════════════════════════════════════╝
--]]

MUFC_CONFIG = {
MUFC_SWITCH = 1,                    		-- Event Ativo: 0 = desabilitado, 1 = habilitado
COMMAND_OPEN = "/abrirpvp",       		-- Comando para abrir manualmente o evento /abrirmufc index (1, 2, 3...)
COMMAND_CANCEL = "/cancelarpvp",  		-- Comando para cancelar o evento
COMMAND_JOIN = "/irpvp",       			-- Comando para os players entrarem no evento
JOIN_TIME = 120,                  		-- Tempo em segundos que o comando ficará disponível
REMAIN_TIME_MESSAGE = 2,			-- Tempo em segundos que irá repetir comando e tempo no global
WAITING_ROOM = {MAP = 6, X = 50, Y = 216}, 	-- Area de espera (recomendado uma SafeZone)
BATTLE_AREA = { MAP = 6, X = 18, Y = 165}, 	-- Area de Combate 1x1 (recomendado uma área onde jogadores não podem ir sozinhos)   
MIN_USERS = 2,  				-- Quantidade de mínima de usuários para o evento acontecer
ROUNDS = 3,  					-- Quantidade de rounds por x1
MINIMUM_LEVEL = 400,				-- Level Mínimo para participar do Evento
BK_DISABLED_SHIELD = 1, 	   	        -- Bloqueio de Shield no BK (1 = Nao entra com Shield / se Equipar no Evento é desclassificado)
BLOCK_ITEM_VISUAL = 0, --Somente Rox e OgocX    -- Bloqueio de Itens Visuais (1 = Nao entra com Itens Visuais / se Equipar no Evento é desclassificado)
REMOVE_BUFFS = 1,              			-- Remove Buffs ao entrar no Evento

-- Área de configuração dos eventos
-- INDEX:
--	Indes do evento, sempre em ordem crescente sem pular, o comando será usado via index: ex: /abrirmufc 1 (abre o evento de index 1)
-- DAY_OF_MONTH = Dia do Mês, DAY_OF_WEEK = Dia da Semana (1 Seg, 2 Ter,...), HOUR = Hora, MINUTE = Minuto, ANNOUNCE_TIME = Anuncio em minutos (ex: MuFC SM X SM começa em 5 minutos)
-- Class:
--     -1 = "All Classes",
--	0 = "Dark Wizard (DW)",
--	1 = "Dark Knight (DK)",
--	2 = "Fairy Elf (FE)",
--	3 = "Magic Gladiator (MG)",
--	4 = "Dark Lord (DL)",
--	5 = "Summoner (SU)",
--	6 = "Rage Fighter (RF)"
AUTO_START = {
    {INDEX = 1, NAME = "MuFC ELF x ELF", 	DAY_OF_MONTH = -1, DAY_OF_WEEK = 1, HOUR = 20, MINUTE = 30, ANNOUNCE_TIME = 5, CLASS = 2},
    {INDEX = 2, NAME = "MuFC MG x MG", 	 	DAY_OF_MONTH = -1, DAY_OF_WEEK = 2, HOUR = 20, MINUTE = 30, ANNOUNCE_TIME = 5, CLASS = 3},
    {INDEX = 3, NAME = "MuFC SUM x SUM", 	DAY_OF_MONTH = -1, DAY_OF_WEEK = 3, HOUR = 20, MINUTE = 30, ANNOUNCE_TIME = 5, CLASS = 5},
    {INDEX = 4, NAME = "MuFC DL x DL", 	 	DAY_OF_MONTH = -1, DAY_OF_WEEK = 4, HOUR = 20, MINUTE = 30, ANNOUNCE_TIME = 5, CLASS = 4},
    {INDEX = 5, NAME = "MuFC SM x SM", 	 	DAY_OF_MONTH = -1, DAY_OF_WEEK = 5, HOUR = 20, MINUTE = 30, ANNOUNCE_TIME = 5, CLASS = 0},
    {INDEX = 6, NAME = "MuFC BM x BM", 	 	DAY_OF_MONTH = -1, DAY_OF_WEEK = 6, HOUR = 20, MINUTE = 30, ANNOUNCE_TIME = 5, CLASS = 1},
    {INDEX = 7, NAME = "MuFC All Classe", 	DAY_OF_MONTH = -1, DAY_OF_WEEK = 7, HOUR = 20, MINUTE = 30, ANNOUNCE_TIME = 5, CLASS = -1},
},

-- Área de configuração das premiações
REWARDS_BY_INDEX = {
-- MuFC - ELF
    [1] = {
        {Section = 4, ID = 100, Quantity = 1, ItemName = "Sylph Wind V.2 Bow"},
        {Section = 7, ID = 111, Quantity = 1, ItemName = "Sylphid Ray V.2 Helm"},
        {Section = 8, ID = 111, Quantity = 1, ItemName = "Sylphid Ray V.2 Armor"},
        {Section = 9, ID = 111, Quantity = 1, ItemName = "Sylphid Ray V.2 Pants"},
        {Section = 10, ID = 111, Quantity = 1, ItemName = "Sylphid Ray V.2 Gloves"},
        {Section = 11, ID = 111, Quantity = 1, ItemName = "Sylphid Ray V.2 Boots"},
    },
-- MuFC - MG
    [2] = {
        {Section = 0, ID = 110, Quantity = 1, ItemName = "Explosion Blade V.2"},
        {Section = 8, ID = 112, Quantity = 1, ItemName = "Volcano V.2 Armor"},
        {Section = 9, ID = 112, Quantity = 1, ItemName = "Volcano V.2 Pants"},
        {Section = 10, ID = 112, Quantity = 1, ItemName = "Volcano V.2 Gloves"},
        {Section = 11, ID = 112, Quantity = 1, ItemName = "Volcano V.2 Boots"},
    },
-- MuFC - SUM
    [3] = {
        {Section = 5, ID = 110, Quantity = 1, ItemName = "Storm Blitz V.2 Stick"},
        {Section = 6, ID = 110, Quantity = 1, ItemName = "Serpent V.2 Shield"},
        {Section = 7, ID = 114, Quantity = 1, ItemName = "Storm Blitz V.2 Helm"},
        {Section = 8, ID = 114, Quantity = 1, ItemName = "Storm Blitz V.2 Armor"},
        {Section = 9, ID = 114, Quantity = 1, ItemName = "Storm Blitz V.2 Pants"},
        {Section = 10, ID = 114, Quantity = 1, ItemName = "Storm Blitz V.2 Gloves"},
        {Section = 11, ID = 114, Quantity = 1, ItemName = "Storm Blitz V.2 Boots"},
    },
-- MuFC - DL
    [4] = {
        {Section = 2, ID = 109, Quantity = 1, ItemName = "Soleil V.2 Sceptor"},
        {Section = 6, ID = 110, Quantity = 1, ItemName = "Serpent V.2 Shield"},
        {Section = 7, ID = 113, Quantity = 1, ItemName = "Sunlight V.2 Mask"},
        {Section = 8, ID = 113, Quantity = 1, ItemName = "Sunlight V.2 Armor"},
        {Section = 9, ID = 113, Quantity = 1, ItemName = "Sunlight V.2 Pants"},
        {Section = 10, ID = 113, Quantity = 1, ItemName = "Sunlight V.2 Gloves"},
        {Section = 11, ID = 113, Quantity = 1, ItemName = "Sunlight V.2 Boots"},
    },
-- MuFC - SM
    [5] = {
        {Section = 5, ID = 109, Quantity = 1, ItemName = "Grand Viper V.2 Staff"},
        {Section = 6, ID = 109, Quantity = 1, ItemName = "Grand Soul V.2 Shield"},
        {Section = 7, ID = 110, Quantity = 1, ItemName = "Venom Mist V.2 Helm"},
        {Section = 8, ID = 110, Quantity = 1, ItemName = "Venom Mist V.2 Armor"},
        {Section = 9, ID = 110, Quantity = 1, ItemName = "Venom Mist V.2 Pants"},
        {Section = 10, ID = 110, Quantity = 1, ItemName = "Venom Mist V.2 Gloves"},
        {Section = 11, ID = 110, Quantity = 1, ItemName = "Venom Mist V.2 Boots"},
    },
-- MuFC - BM
    [6] = {
        {Section = 0, ID = 109, Quantity = 1, ItemName = "Bone Blade V.2"},
        {Section = 7, ID = 109, Quantity = 1, ItemName = "Dragon Knight V.2 Helm"},
        {Section = 8, ID = 109, Quantity = 1, ItemName = "Dragon Knight V.2 Armor"},
        {Section = 9, ID = 109, Quantity = 1, ItemName = "Dragon Knight V.2 Pants"},
        {Section = 10, ID = 109, Quantity = 1, ItemName = "Dragon Knight V.2 Gloves"},
        {Section = 11, ID = 109, Quantity = 1, ItemName = "Dragon Knight V.2 Boots"},
    },
-- Rei do PvP - All Classes
    [7] = {
        {Section = 4, ID = 111, Quantity = 1, ItemName = "Sylph Wind V.2 Bow"},
        {Section = 7, ID = 111, Quantity = 1, ItemName = "Sylphid Ray V.2 Helm"},
        {Section = 8, ID = 111, Quantity = 1, ItemName = "Sylphid Ray V.2 Armor"},
        {Section = 9, ID = 111, Quantity = 1, ItemName = "Sylphid Ray V.2 Pants"},
        {Section = 10, ID = 111, Quantity = 1, ItemName = "Sylphid Ray V.2 Gloves"},
        {Section = 11, ID = 111, Quantity = 1, ItemName = "Sylphid Ray V.2 Boots"},

        {Section = 0, ID = 110, Quantity = 1, ItemName = "Explosion Blade V.2"},
        {Section = 8, ID = 112, Quantity = 1, ItemName = "Volcano V.2 Armor"},
        {Section = 9, ID = 112, Quantity = 1, ItemName = "Volcano V.2 Pants"},
        {Section = 10, ID = 112, Quantity = 1, ItemName = "Volcano V.2 Gloves"},
        {Section = 11, ID = 112, Quantity = 1, ItemName = "Volcano V.2 Boots"},

        {Section = 5, ID = 110, Quantity = 1, ItemName = "Storm Blitz V.2 Stick"},
        {Section = 6, ID = 110, Quantity = 1, ItemName = "Serpent V.2 Shield"},
        {Section = 7, ID = 114, Quantity = 1, ItemName = "Storm Blitz V.2 Helm"},
        {Section = 8, ID = 114, Quantity = 1, ItemName = "Storm Blitz V.2 Armor"},
        {Section = 9, ID = 114, Quantity = 1, ItemName = "Storm Blitz V.2 Pants"},
        {Section = 10, ID = 114, Quantity = 1, ItemName = "Storm Blitz V.2 Gloves"},
        {Section = 11, ID = 114, Quantity = 1, ItemName = "Storm Blitz V.2 Boots"},
	
        {Section = 2, ID = 109, Quantity = 1, ItemName = "Soleil V.2 Sceptor"},
        {Section = 7, ID = 113, Quantity = 1, ItemName = "Sunlight V.2 Mask"},
        {Section = 8, ID = 113, Quantity = 1, ItemName = "Sunlight V.2 Armor"},
        {Section = 9, ID = 113, Quantity = 1, ItemName = "Sunlight V.2 Pants"},
        {Section = 10, ID = 113, Quantity = 1, ItemName = "Sunlight V.2 Gloves"},
        {Section = 11, ID = 113, Quantity = 1, ItemName = "Sunlight V.2 Boots"},

        {Section = 5, ID = 109, Quantity = 1, ItemName = "Grand Viper V.2 Staff"},
        {Section = 6, ID = 109, Quantity = 1, ItemName = "Grand Soul V.2 Shield"},
        {Section = 7, ID = 110, Quantity = 1, ItemName = "Venom Mist V.2 Helm"},
        {Section = 8, ID = 110, Quantity = 1, ItemName = "Venom Mist V.2 Armor"},
        {Section = 9, ID = 110, Quantity = 1, ItemName = "Venom Mist V.2 Pants"},
        {Section = 10, ID = 110, Quantity = 1, ItemName = "Venom Mist V.2 Gloves"},
        {Section = 11, ID = 110, Quantity = 1, ItemName = "Venom Mist V.2 Boots"},

        {Section = 0, ID = 109, Quantity = 1, ItemName = "Bone Blade V.2"},
        {Section = 7, ID = 109, Quantity = 1, ItemName = "Dragon Knight V.2 Helm"},
        {Section = 8, ID = 109, Quantity = 1, ItemName = "Dragon Knight V.2 Armor"},
        {Section = 9, ID = 109, Quantity = 1, ItemName = "Dragon Knight V.2 Pants"},
        {Section = 10, ID = 109, Quantity = 1, ItemName = "Dragon Knight V.2 Gloves"},
        {Section = 11, ID = 109, Quantity = 1, ItemName = "Dragon Knight V.2 Boots"},
    }
},
  
-- MENSAGENS
MESSAGES = {
    COUNTDOWN_ANNOUNCE = "O Evento [ %s ] começará em %d minutos!",
    OPEN_ANNOUNCE1 = "[ %s ] - ABERTO",
    OPEN_ANNOUNCE = "%s PARA ENTRAR. Tempo restante: %d segundos",
    JOIN_PLAYER = "[ %s ] - Player %s adicionado",
    START_ANNOUNCE = "[ %s ] - está iniciado!",
    MATCH_ANNOUNCE = "[ %s ] - Duelo: [ %s ] X [ %s ]",
    BUFF_MESSAGE = "[ %s ] - Buffem seus personagens!",
    PREPARE_MESSAGE = "[ %s ] - Preparem-se para a contagem!",
    COUNTDOWN = "[ %s ] - Contagem: %d...",
    FIGHT_MESSAGE = "[ %s ] - Ataquem-se! GOOOOOOOOOOOOOOO!",
    SCORE_MESSAGE = "[ %s ] - Placar: %s (%d) X (%d) %s",
    MATCH_WINNER = "[ %s ] - %s venceu a partida!",
    FINAL_WINNER = "[ %s ] - %s venceu o [ %s ]! Derrotou: %d oponentes",
    NOT_ENOUGH_PLAYERS = "[ %s ] - Evento cancelado: número insuficiente de jogadores",
    COMMAND_CLOSED = "[ %s ] - Comando de entrada fechado",
    CLASS_RESTRICTED = "[ %s ] - Este evento é apenas para %s",
    EVENT_NOT_FOUND = "Evento não encontrado",
    EVENT_STARTED = "Evento %d (%s) iniciado com sucesso!",
    REWARD_MESSAGE = "[ %s ] - [ %s ] recebeu um [ %s ]!",
    EVENT_CANCELED = "[ %s ] O evento foi cancelado por um GM!",
    NO_EVENT_TO_CANCEL = "Não há nenhum evento em andamento para cancelar",
    LEVEL_TOO_LOW = "[ %s ] - Você precisa ter Level %d para participar!",
  }
}