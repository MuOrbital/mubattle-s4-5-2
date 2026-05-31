--[[
SetStats System - By: Victor Jardim

Configurações Comando SetStatus
-- /setstats <name> <type> <value>

-- Types:
	1: força
	2: agi
	3: vit
	4: ene
	5: cmd
	6: all
	
	/setstats Admin 1 65000 -- Add 65k Status in Strength for Char Admin
]]--	

VKSETSTATS_COMMAND = "/setstats"
VKSETSTATS_COMMAND_SWITCH = 1
VKSETSTATS_COMMAND_GAME_MASTER_LEVEL = 2
VKSETSTATS_COMMAND_MESSAGES = {}

VKSETSTATS_COMMAND_MESSAGES["Por"] = {
[1]  = "[Sistema] Você não tem permissão para usar este comando.",
[2]  = "[Sistema] O comando não está ativo.",
[3]  = "[Sistema] Insira o nome do player",
[4]  = "[Sistema] O player %s está offline ou não existe.",
[5]  = "[Sistema] Insira um valor válido!",
[6]  = "[Sistema] Seu status de Força foi atualizado.",
[7]  = "[Sistema] Seu status de Agilidade foi atualizado.",
[8]  = "[Sistema] Seu status de Vitalidade foi atualizado.",
[9]  = "[Sistema] Seu status de Energia foi atualizado.",
[10] = "[Sistema] Seu status de Comando foi atualizado.",
[11] = "[Sistema] O personagem não é DL/LE.",
[12] = "[Sistema] Todos os seus Status foram atualizados.",
[13] = "[Sistema] Insira um Status válido.",
[14] = "[Sistema] Status de %s atualizado com sucesso.",
}

VKSETSTATS_COMMAND_MESSAGES["Eng"] = {
[1]  = "[System] You do not have permission to use this command.",
[2]  = "[System] The command is not active.",
[3]  = "[System] Enter the player's name.",
[4]  = "[System] The player %s is offline or does not exist.",
[5]  = "[System] Enter a valid value!",
[6]  = "[System] Your Strength status has been updated.",
[7]  = "[System] Your Agility status has been updated.",
[8]  = "[System] Your Vitality status has been updated.",
[9]  = "[System] Your Energy status has been updated.",
[10] = "[System] Your Command status has been updated.",
[11] = "[System] The character is not DL/LE.",
[12] = "[System] All of your statuses have been updated.",
[13] = "[System] Enter a valid status.",
[14] = "[System] %s status successfully updated.",
}

VKSETSTATS_COMMAND_MESSAGES["Spn"] = {
[1]  = "[Sistema] No tienes permiso para usar este comando.",
[2]  = "[Sistema] El comando no está activo.",
[3]  = "[Sistema] Introduce el nombre del jugador.",
[4]  = "[Sistema] El jugador %s está desconectado o no existe.",
[5]  = "[Sistema] ¡Introduce un valor válido!",
[6]  = "[Sistema] Tu estado de Fuerza ha sido actualizado.",
[7]  = "[Sistema] Tu estado de Agilidad ha sido actualizado.",
[8]  = "[Sistema] Tu estado de Vitalidad ha sido actualizado.",
[9]  = "[Sistema] Tu estado de Energía ha sido actualizado.",
[10] = "[Sistema] Tu estado de Comando ha sido actualizado.",
[11] = "[Sistema] El personaje no es DL/LE.",
[12] = "[Sistema] Todos tus estados han sido actualizados.",
[13] = "[Sistema] Introduce un estado válido.",
[14] = "[Sistema] Estado de %s actualizado con éxito.",
}