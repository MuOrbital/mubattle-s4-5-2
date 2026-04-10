--[[
Command Config
]]--
--this command is only staff
--/premiar <nick> <number config> <coins amount>
REWARD_PLAYERS_SWITCH = 1
REWARD_PLAYERS_COMMAND = '/premiar'
REWARD_ANNOUNCE_GLOBAL = 1

REWARD_PLAYERS_CONFIGS = { }

REWARD_PLAYERS_CONFIGS[1] = {
Table = 'CashShopData',
Column = 'WCoinC',
Where = 'AccountID',
CoinName = 'Golds',
MaxReward = 5, --  max coin amount reward -1 = no limit
}

--[[
Command Message
]]--
--Global users message
REWARD_PLAYERS_GLOBAL_MESSAGES = { }

REWARD_PLAYERS_GLOBAL_MESSAGES['Por'] = {
[1] = '[ %s ] Premiou',
[2] = '[ %s ] com [%d %s]',
}

REWARD_PLAYERS_GLOBAL_MESSAGES['Eng'] = {
[1] = '[ %s ] Premiou',
[2] = '[ %s ] com [%d %s]',
}

REWARD_PLAYERS_GLOBAL_MESSAGES['Spn'] = {
[1] = '[ %s ] Premiou',
[2] = '[ %s ] com [%d %s]',
}

--Staff message
REWARD_PLAYERS_MESSAGES = {}

REWARD_PLAYERS_MESSAGES['Por'] = {
[1] = 'Uso incorreto do comando: <nick> <valor do premio> <quantidade>',
[2] = 'O numero dessa premiação não foi encontrado!',
[3] = 'Ocorreu algum problema ao identificar a premiação',
[4] = 'O limite de moedas a ser adiciona é %d',
[5] = 'O jogador %s não está online ou não existe',
[6] = 'Sua premiação %d %s, foi adicionada',
}

REWARD_PLAYERS_MESSAGES['Eng'] = {
[1] = 'Uso incorreto do comando: <nick> <valor do premio> <quantidade>',
[2] = 'O numero dessa premiação não foi encontrado!',
[3] = 'Ocorreu algum problema ao identificar a premiação',
[4] = 'O limite de moedas a ser adiciona é %d',
[5] = 'O jogador %s não está online ou não existe',
[6] = 'Sua premiação %d %s, foi adicionada',
}

REWARD_PLAYERS_MESSAGES['Spn'] = {
[1] = 'Uso incorreto do comando: <nick> <valor do premio> <quantidade>',
[2] = 'O numero dessa premiação não foi encontrado!',
[3] = 'Ocorreu algum problema ao identificar a premiação',
[4] = 'O limite de moedas a ser adiciona é %d',
[5] = 'O jogador %s não está online ou não existe',
[6] = 'Sua premiação %d %s, foi adicionada',
}