--[[
Configuração de Comandos
]]
TRANSFER_COIN_SWITCH = 0			-- Liga ou Desliga o Sistema
TRANSFER_COIN_COMMAND = "/transferir" 		-- Comando /transferir <value> <coin> <NickDestinatario> Ex: /transferir 10 Golds Admin
TRANSFER_COIN_COMMAND_VIP = 1 			-- Level do Vip que pode usar o comando
TRANSFER_COIN_COMMAND_CONFIRM = "/ok" 		-- Comando para confirmar a transação
TRANSFER_COIN_COMMAND_CANCEL = "/cancelar" 	-- Comando para cancelar a transação
--[[
Configuração de Moedas
]]
TRANSFER_COIN_COUNT = 4 			-- Quantidade de moedas habilitadas (moedas abaixo - Max 4)
TRANSFER_COIN_COUNT_MIN = 100 			-- Valor mínimo para envio de moedas

TRANSFER_COIN_NAME1 = "wc" 			-- Nome da Moeda 1
TRANSFER_COIN_NAME2 = "wp" 			-- Nome da Moeda 2
TRANSFER_COIN_NAME3 = "gp" 			-- Nome da Moeda 3
TRANSFER_COIN_NAME4 = "pcp" 			-- Nome da Moeda 4

TRANSFER_COIN_TABLE1 = "CashShopData" 		-- Tabela da Moeda 1
TRANSFER_COIN_TABLE2 = "CashShopData" 		-- Tabela da Moeda 2
TRANSFER_COIN_TABLE3 = "CashShopData" 		-- Tabela da Moeda 3
TRANSFER_COIN_TABLE4 = "PcPointData" 		-- Tabela da Moeda 4

TRANSFER_COIN_COLUMN_ACCOUNT1 = "AccountID" 	-- Coluna da Conta da Moeda 1
TRANSFER_COIN_COLUMN_ACCOUNT2 = "AccountID" 	-- Coluna da Conta da Moeda 2
TRANSFER_COIN_COLUMN_ACCOUNT3 = "AccountID" 	-- Coluna da Conta da Moeda 3
TRANSFER_COIN_COLUMN_ACCOUNT4 = "AccountID" 	-- Coluna da Conta da Moeda 4

TRANSFER_COIN_COLUMN_COIN1 = "WCoinC" 		-- Coluna de saldo da conta da Moeda 1
TRANSFER_COIN_COLUMN_COIN2 = "WCoinP" 		-- Coluna de saldo da conta da Moeda 2
TRANSFER_COIN_COLUMN_COIN3 = "GoblinPoint" 	-- Coluna de saldo da conta da Moeda 3
TRANSFER_COIN_COLUMN_COIN4 = "PcPoint" 		-- Coluna de saldo da conta da Moeda 4

--[[
Configuração de Taxa de Envio
]]
TRANSFER_COIN_DISCOUNT = 1 			-- Sistema de TAXA irá descontar moeda do remetente ao enviar
TRANSFER_COIN_DISCOUNT_COIN = "WCoinC" 		-- Nome da moeda que será exibido na mensagem
TRANSFER_COIN_DISCOUNT_VALUE = 10 		-- Valor da moeda que irá descontar
TRANSFER_COIN_DISCOUNT_TABLE = "CashShopData" 	-- Tabela da moeda de desconto
TRANSFER_COIN_DISCOUNT_COLUMN = "AccountID" 	-- Coluna da Conta da moeda de desconto
TRANSFER_COIN_DISCOUNT_COLUMN_COIN = "WCoinC" 	-- Coluna da Moeda de desconto

TRANSFER_COIN_MESSAGES = {  }
TRANSFER_COIN_MESSAGES["Por"] = {
    [1] = "[Sistema] Envio de %d - %s para %s.",
    [2] = "[Sistema] Envio de %d - %s para %s | Taxa: %d - %s?.",
    [3] = "[Sistema] Digite %s para confirmar ou %s para cancelar.",
    [4] = "[Sistema] Erro: O Personagem %s não está online.",
    [5] = "[Sistema] Dados para o Destinatário não encontrados.",
    [6] = "[Sistema] O Personagem %s recebeu %d %s enviados por você.",
    [7] = "[Sistema] Descontado %s - %d da sua conta...",
    [8] = "[Sistema] Descontado %s - %d de Taxa de transferência...",
    [9] = "[Sistema] Você recebeu %d %s de %s.",
    [10] = "[Sistema] Relogando Personagem...",
    [11] = "[Sistema] Quantidade mínima para transferência é %d.",
    [12] = "[Sistema] Moeda inválida ou não habilitada.",
    [13] = "[Sistema] Saldo insuficiente para a transferência.",
    [14] = "[Sistema] Saldo insuficiente para a taxa de transferência.",
    [15] = "[Sistema] Transferência cancelada.",
    [16] = "[Sistema] Transferência cancelada devido a desconexão.",
    [17] = "[Sistema] Você já possui uma transação pendente. Confirme ou cancele antes de iniciar outra.",
    [18] = "[Sistema] Somente Players [VIP] podem utilizar este Comando."
}

TRANSFER_COIN_MESSAGES["Eng"] = {
    [1] = "[System] Sending %d - %s to %s.",
    [2] = "[System] Sending %d - %s to %s | Fee: %d - %s?.",
    [3] = "[System] Type %s to confirm or %s to cancel.",
    [4] = "[System] Error: The character %s is not online.",
    [5] = "[System] Recipient data not found.",
    [6] = "[System] The character %s received %d %s sent by you.",
    [7] = "[System] Deducted %s - %d from your account...",
    [8] = "[System] Deducted %s - %d as transfer fee...",
    [9] = "[System] You received %d %s from %s.",
    [10] = "[System] Relogging Character...",
    [11] = "[System] Minimum transfer amount is %d.",
    [12] = "[System] Invalid or disabled coin.",
    [13] = "[System] Insufficient balance for the transfer.",
    [14] = "[System] Insufficient balance for the transfer fee.",
    [15] = "[System] Transfer canceled.",
    [16] = "[System] Transfer canceled due to disconnection.",
    [17] = "[System] You already have a pending transaction. Confirm or cancel before starting another.",
    [18] = "[System] Only [VIP] players can use this command."
}

TRANSFER_COIN_MESSAGES["Spn"] = {
    [1] = "[Sistema] Enviando %d - %s a %s.",
    [2] = "[Sistema] Enviando %d - %s a %s | Tarifa: %d - %s?.",
    [3] = "[Sistema] Escribe %s para confirmar o %s para cancelar.",
    [4] = "[Sistema] Error: El personaje %s no está en línea.",
    [5] = "[Sistema] Datos del destinatario no encontrados.",
    [6] = "[Sistema] El personaje %s recibió %d %s enviados por ti.",
    [7] = "[Sistema] Descontado %s - %d de tu cuenta...",
    [8] = "[Sistema] Descontado %s - %d como tarifa de transferencia...",
    [9] = "[Sistema] Recibiste %d %s de %s.",
    [10] = "[Sistema] Reconectando Personaje...",
    [11] = "[Sistema] La cantidad mínima para la transferencia es %d.",
    [12] = "[Sistema] Moneda inválida o no habilitada.",
    [13] = "[Sistema] Saldo insuficiente para la transferencia.",
    [14] = "[Sistema] Saldo insuficiente para la tarifa de transferencia.",
    [15] = "[Sistema] Transferencia cancelada.",
    [16] = "[Sistema] Transferencia cancelada debido a desconexión.",
    [17] = "[Sistema] Ya tienes una transacción pendiente. Confirma o cancela antes de iniciar otra.",
    [18] = "[Sistema] Solo los jugadores [VIP] pueden usar este comando."
}