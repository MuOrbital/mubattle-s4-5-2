WarDevias_Config = {
    Enable = false, -- true = ativa | false = desativa o evento
    Command = "/startwd", -- Comando para abrir o evento manualmente.
	--Cronograma para o evento abrir automáticamente sem necessidade de Staffer.
	--Day of Week, Day, Hour, Minute
    --Day of Week: 0 a 6: Domingo, segunda, terça, quarta, quinta, sexta, sabado
    --Day of Week: -1 = Day
    --Day = -1 = Horas e minutos.
    Cron = {
        {dayofweek = -1, day = -1, hour = 08, minute = 54},
    },
	
	TeleportX = 50,
	TeleportY = 27,
	
    -- Premiação do evento
    Table = 'CashShopData',
    Column = 'GoblinPoint',
    Where = 'AccountID',
    CoinName = 'WCoinP',

    GuildMasterReward = 15, -- Valor da premiação para o Guild Master da guild vencedora
    AssistenteReward = 10, -- Valor da premiação para o Assistente da guild vencedora
    MestreReward = 5, -- Valor da premiação para o Mestre da guild vencedora
    MembroReward = 5, -- Valor da premiação para o Membro da guild vencedora

    --Configurações do evento em geral
    MonsterSwitch = 224, -- Número do monstro que será responsável pela Switch do evento
	MonsterCordX = 13, --CordX em que o monstro irá nascer
	MonsterCordY = 45, --CordY em que o monstro irá nascer

    MapEventNumber = 100, -- Número em que ocorre o evento
    MapEventRangeX_1 = 10, -- CordX mínima em que o evento ocorre
    MapEventRangeX_2 = 43, -- CordX máxima em que o evento ocorre
    MapEventRangeY_1 = 6, -- CordY mínima em que o evento ocorre
    MapEventRangeY_2 = 45, -- CordY máxima em que o evento ocorre
	
	MapCordXThrone = 10, -- CordX Trono do Castelo
    MapCordYThrone = 26, -- CordY Trono do Castelo

    PlayerRangeSwitchX_1 = 12, -- CordX mínima que o player deve estar para segurar a Switch
    PlayerRangeSwitchX_2 = 15, -- CordX máxima que o player pode estar para segurar a Switch
    PlayerRangeSwitchY_1 = 43, -- CordY mínima que o player deve estar para segurar a Switch
    PlayerRangeSwitchY_2 = 45, -- CordY máxima que o player pode estar para segurar a Switch

	TeleportWinsCordX = 16, -- CordX onde os vencedores serão teleportados ao finalizar o evento.
    TeleportWinsCordY = 25, -- CordY onde os vencedores serão teleportados ao finalizar o evento.

	AllowDamageFriend = false, -- true = ativa | false = desativa  >> FOGO AMIGO << (Responsável pelo dano recebido de membros da mesma guild/guild aliada na área do evento)
	NumberGuilds = 2, -- Número mínimo de guilds cadastradas (Mínimo 2)
	NumberMaxGuilds = 5, -- Número máximo de guilds cadastradas
	
	TimerStart = 1, -- Tempo em minuto(s) que o evento irá iniciar após o comando de abertura (Mínimo 1)
	TimerFinish = 2, -- Tempo em minuto(s) da duração do evento (Mínimo 1)
	TimerNextRound = 3, -- Tempo em segundo(s) que irá liberar o Switch após finalizar o round (Mínimo 1)
	TimerThrone = 30, -- Tempo em segundo(s) que o GM precisa 'dominar' o trono (Mínimo 1)
	TimerAnnounce = 3, -- Tempo em minuto(s) que irá anunciar o ranking/tempo restante do evento (Mínimo 1)
	
	PointsInterval = 5, -- Tempo em segundo(s) que irá adicionar pontos para a guild que está dominando o Castelo (Mínimo 1)
    PointsValue = 1, -- Quantidade de pontos serão adicionados para a guild que está dominando o Castelo (Mínimo 1)
}

return WarDevias_Config