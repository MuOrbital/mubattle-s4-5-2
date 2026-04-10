--[[
    Command Config
]]
SET_PLAYER_COMMAND_SWITCH = 0
SET_PLAYER_COMMAND_GAME_MASTER_LEVEL = 2

SET_PLAYER_COMMAND_LEVELPOINT_SYNTAX = '/setpoints' --/setlevelpoint <player> <value>
SET_PLAYER_COMMAND_STRENGTH_SYNTAX = '/setfor' --/setstr <player> <value>
SET_PLAYER_COMMAND_AGILITY_SYNTAX = '/setagi' --/setagi <player> <value>
SET_PLAYER_COMMAND_VITALITY_SYNTAX = '/setvit' --/setvit <player> <value>
SET_PLAYER_COMMAND_ENERGY_SYNTAX = '/setene' --/setene <player> <value>
SET_PLAYER_COMMAND_COMMAND_SYNTAX = '/setcmd'  --/setcmd <player> <value>
SET_PLAYER_COMMAND_MASTERLEVEL_SYNTAX = '/setmasterlevel' --/setmasterlevel <player> <value>
SET_PLAYER_COMMAND_MASTERPOINT_SYNTAX = '/setmasterpoint' --/setmasterpoint <player> <value>
SET_PLAYER_COMMAND_EFFECT_SYNTAX = '/setbuff' --/seteffect <player> <effectid> <time>

--[[
    Command Messages
]]

SET_PLAYER_COMMAND_MESSAGES = {}

SET_PLAYER_COMMAND_MESSAGES['Por'] = {
[1] = 'O jogador %s não existe ou não está online',
[2] = 'Coloque um valor válido!',
[3] = 'Player %s atualizado com sucesso!'
}

SET_PLAYER_COMMAND_MESSAGES['Eng'] = {
[1] = 'Player %s does not exist or is not online',
[2] = 'Please enter a valid value!',
[3] = 'Player %s updated successfully!'
}

SET_PLAYER_COMMAND_MESSAGES['Spn'] = {
[1] = 'El jugador %s no existe o no está en línea',
[2] = '¡Ingrese un valor válido!',
[3] = '¡Jugador %s actualizado correctamente!'
}