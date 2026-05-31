--[[
    +--------------------------------------------------------------------+
    ¦ Mining Stones Script - MuBattle Season 4 				 ¦
    ¦--------------------------------------------------------------------¦
    ¦ Criado por: Mineiro MuOnline 					 ¦
    ¦ Site: https://youtube.com/@mineiromuonline 			 ¦
    ¦ Contato WhatsApp: 31 9 8255 6150 (Mineiro) 			 ¦
    ¦ 									 ¦
    ¦ Emulador: Surce Própria MuBattle - Lua OgocX 			 ¦
    ¦ Site: https://playmubattle.com/site/ 				 ¦
    ¦ 									 ¦
    ¦ Descrição: Script para evento MiningStone, Pedras aparecem no mapa ¦
    ¦ os players mineiram por tempo X e recebem recomensas 	 	 ¦
    +--------------------------------------------------------------------+
--]]

MiningStone_Config = {
    Enabled = true, 				-- true = Ativa / False = Desativa o sistema
    Packet = 0x34, 				-- Packet de informações (do cliente deve ser igual)
    Command = "/mining", 			-- Comando GM para iniciar manualmente
    GMLevel = 2, 				-- Level de permissão de GM para usar o comando 2 = Ctlcode 32
    Duration = 20, 				-- Tempo de duração do Evento em Minutos
    MiningTime = 50, 				-- Tempo necessário para completar a mineração (segundos) - (No cliente deve ser igual)
    MiningRange = 4, 				-- Distância máxima permitida a se afastar da stones
    StonesPerMap = 20, 				-- Quantidade de stones por mapa
    MonsterClass = 708, 			-- ID da classe do monstro (stone)
    MapType = 1, 				-- 0 = spawn em TODOS os mapas acima | 1 = spawn em APENAS 1 mapa aleatório da lista
    ShowMapInStartMessage = 0,      		-- 0 = não exibe nome do mapa na mensagem de início | 1 = exibe Nome (apenas se MapType = 1)
    ShowLocationInMiningMessage = 3,		-- 0 = sem localização | 1 = só mapa | 2 = mapa + X (Y=***) | 3 = mapa + X + Y completo
    ShowRemainingStonesMessage = 1,		-- 0 = não exibe quantas stones faltam | 1 = exibe junto da mensagem de sucesso
    DropsPerStone = 5, 				-- quantidade de itens dropados por pedra minerada com sucesso
    Maps = {
        [0] = "Lorencia",
        [2] = "Devias",
        [3] = "Noria",
    },
-- horários automáticos de início do evento (DAY_OF_WEEK = Dia da Semana (1 Seg, 2 Ter,...) -1 = todos os dias)
-- ANNOUNCE_TIME = tempo para começar notificar no Global (minutos)
    AUTO_START = {
        {DAY_OF_WEEK = -1, HOUR = 00, MINUTE = 00, ANNOUNCE_TIME = 3},
        {DAY_OF_WEEK = -1, HOUR = 06, MINUTE = 00, ANNOUNCE_TIME = 3},
        {DAY_OF_WEEK = -1, HOUR = 12, MINUTE = 00, ANNOUNCE_TIME = 3},
        {DAY_OF_WEEK = -1, HOUR = 18, MINUTE = 00, ANNOUNCE_TIME = 3},
        --{DAY_OF_WEEK = 1, HOUR = 19, MINUTE = 00, ANNOUNCE_TIME = 3},
        -- etc...
    },
  
-- lista de itens dropados ao Mineirar com Sucesso (sorteio por Rate)
    Drops = {
        {Section=14, Index=53, Level=0, Rate=80},
	{Section=14, Index=96, Level=0, Rate=20},
    }
}

MININGSTONE_MESSAGES = {}
MININGSTONE_MESSAGES["Por"] = {
    [1] = "[MiningStone] O evento começou em %s!",
    [2] = "[MiningStone] O evento terminou!",
    [3] = "[MiningStone] %s está minerando uma Stone! (%s - X:%d Y:%d)",
    [4] = "[MiningStone] O jogador %s teve sucesso em minerar uma stone!",
    [5] = "Você está tentando minerar uma stone...",
    [6] = "Você teve sucesso em minerar a stone!",
    [7] = "Esta stone já foi minerada.",
    [8] = "Mineração cancelada! Você morreu.",
    [9] = "Mineração interrompida!",
    [10] = "Mineração cancelada! Você se afastou demais da pedra.",
    [11] = "[MiningStone] O MiningStone vai começar em %d minutos!",
    [12] = "[MiningStone] O MiningStone vai começar em 1 minuto!",
    [13] = "[MiningStone] O MiningStone começou!",
    [14] = "[MiningStone] Já existe um evento MiningStone ativo no momento!",
    [15] = "[MiningStone] Restam %d de %d Stones!"
}
MININGSTONE_MESSAGES["Eng"] = {
    [1] = "[MiningStone] O evento começou em %s!",
    [2] = "[MiningStone] O evento terminou!",
    [3] = "[MiningStone] %s está minerando uma Stone! (%s - X:%d Y:%d)",
    [4] = "[MiningStone] O jogador %s teve sucesso em minerar uma stone!",
    [5] = "Você está tentando minerar uma stone...",
    [6] = "Você teve sucesso em minerar a stone!",
    [7] = "Esta stone já foi minerada.",
    [8] = "Mineração cancelada! Você morreu.",
    [9] = "Mineração interrompida!",
    [10] = "Mineração cancelada! Você se afastou demais da pedra.",
    [11] = "[MiningStone] O MiningStone vai começar em %d minutos!",
    [12] = "[MiningStone] O MiningStone vai começar em 1 minuto!",
    [13] = "[MiningStone] O MiningStone começou!",
    [14] = "[MiningStone] Já existe um evento MiningStone ativo no momento!",
    [15] = "[MiningStone] Restam %d de %d Stones!"
}
MININGSTONE_MESSAGES["Spn"] = {
    [1] = "[MiningStone] O evento começou em %s!",
    [2] = "[MiningStone] O evento terminou!",
    [3] = "[MiningStone] %s está minerando uma Stone! (%s - X:%d Y:%d)",
    [4] = "[MiningStone] O jogador %s teve sucesso em minerar uma stone!",
    [5] = "Você está tentando minerar uma stone...",
    [6] = "Você teve sucesso em minerar a stone!",
    [7] = "Esta stone já foi minerada.",
    [8] = "Mineração cancelada! Você morreu.",
    [9] = "Mineração interrompida!",
    [10] = "Mineração cancelada! Você se afastou demais da pedra.",
    [11] = "[MiningStone] O MiningStone vai começar em %d minutos!",
    [12] = "[MiningStone] O MiningStone vai começar em 1 minuto!",
    [13] = "[MiningStone] O MiningStone começou!",
    [14] = "[MiningStone] Já existe um evento MiningStone ativo no momento!",
    [15] = "[MiningStone] Restam %d de %d Stones!"
}