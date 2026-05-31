-- SetPKConfig.lua

SETPK_CONFIG = {
    Enabled = true,
    Command = "/setpk",
    GMLevel = 32,
}

SETPK_MESSAGES = {}

SETPK_MESSAGES["Por"] = {
    [1] = "Você não tem permissão para usar este comando.",
    [2] = "Uso correto: /setpk nome pk",
    [3] = "Jogador não encontrado.",
    [4] = "PK alterado com sucesso para %s (PK: %d).",
    [5] = "Seu nível PK foi alterado para %d por um GM.",
}

SETPK_MESSAGES["Eng"] = {
    [1] = "You do not have permission to use this command.",
    [2] = "Correct usage: /setpk name pk",
    [3] = "Player not found.",
    [4] = "PK successfully changed for %s (PK: %d).",
    [5] = "Your PK level has been changed to %d by a GM.",
}

SETPK_MESSAGES["Spn"] = {
    [1] = "No tienes permiso para usar este comando.",
    [2] = "Uso correcto: /setpk nombre pk",
    [3] = "Jugador no encontrado.",
    [4] = "PK cambiado correctamente para %s (PK: %d).",
    [5] = "Tu nivel PK ha sido cambiado a %d por un GM.",
}