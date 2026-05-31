CUSTOM_MOVE_CONFIG = {
    Enabled = true,
    GMLevelOverride = 32,                -- GM com esse nível ou superior ignora restrições
    ShowSuccessMsg = true,               -- mostra mensagem de sucesso ao teleportar
    SuccessMessageColor = 3,             -- 1=amarelo, 2=vermelho, 3=azul, etc
    LogTeleports = true,                 -- registra no log cada teleporte usado
}

CUSTOM_MOVES = {
    --comando 		mapa   x   	y 	minLevel  maxLevel 	AccountLevel  allowPKMove  requiredGMLevel
    { "/bar",           0,     124, 	131,  	1, 	  400, 		0, 	      0, 	   0 },
    { "/igreja",        2,     209, 	27,  	1, 	  400, 		0, 	      0, 	   0 },
    { "/chaosmachine",  3,     179, 	102,  	1, 	  400, 		0, 	      0,  	   0 },
    { "/arena1",   	6,     62, 	121,	50, 	  400, 		0, 	      0,  	   0 },
    { "/arena2",   	6,     65, 	48,	50, 	  400, 		0, 	      0,  	   0 },
    { "/ferreiro",      0,     117, 	141, 	10, 	  400, 		0, 	      0,  	   0 },
    { "/mago",      	0,     120, 	113, 	10, 	  400, 		0, 	      0,  	   0 },
    { "/pista",      	0,     132, 	85, 	10, 	  400, 		0, 	      0,  	   0 },
    { "/baucima",      	0,     147, 	143, 	10, 	  400, 		0, 	      0,  	   0 },
    { "/baubaixo",      0,     147, 	110, 	10, 	  400, 		0, 	      0,  	   0 },
    { "/cemiterio",     0,     140, 	215, 	10, 	  400, 		0, 	      0,  	   0 },
    { "/refinaria",     38,    141, 	186, 	400, 	  400, 		1, 	      0,  	   0 },
    { "/selupan",       57,    177, 	24, 	400, 	  400, 		1, 	      0,  	   0 },
}

CUSTOM_MOVE_MESSAGES = {}
CUSTOM_MOVE_MESSAGES["Por"] = {
    [1] = "Você não tem permissão para usar este comando.",
    [2] = "Seu nível (%d) não está permitido para este teleporte (%s ~ %s).",
    [3] = "Este teleporte exige nível VIP / conta %d ou superior.",
    [4] = "Jogadores com PK nível 6 ou superior não podem usar este teleporte.",
    [5] = "O sistema de teleportes customizados está desativado.",
    [6] = "Teleportado para %s (%d, %d)",
    [8] = "Comando inválido ou não configurado.",
}

CUSTOM_MOVE_MESSAGES["Eng"] = {
    [1] = "You do not have permission to use this command.",
    [2] = "Your level (%d) is not allowed for this teleport (%s ~ %s).",
    [3] = "This teleport requires VIP / account level %d or higher.",
    [4] = "Players with PK level 6 or higher cannot use this teleport.",
    [5] = "The custom teleport system is disabled.",
    [6] = "Teleported to %s (%d, %d)",
    [8] = "Invalid or unconfigured command.",
}

CUSTOM_MOVE_MESSAGES["Spn"] = {
    [1] = "No tienes permiso para usar este comando.",
    [2] = "Tu nivel (%d) no está permitido para este teletransporte (%s ~ %s).",
    [3] = "Este teletransporte requiere nivel VIP / cuenta %d o superior.",
    [4] = "Los jugadores con PK nivel 6 o superior no pueden usar este teletransporte.",
    [5] = "El sistema de teletransportes personalizados está desactivado.",
    [6] = "Teletransportado a %s (%d, %d)",
    [8] = "Comando inválido o no configurado.",
}