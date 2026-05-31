DailyReward_Enabled  = true
DailyReward_Packet   = 0x56
DailyReward_WindowID = 191

DailyReward_Text = {
    ["Por"] = {
        [1]  = "Recompensa Diaria",
        [2]  = "Colete sua recompensa todos os dias!",
        [3]  = "Coletar",
        [4]  = "Fechar",
        [5]  = "BONUS DO MES",
        [6]  = "Dia",
        [7]  = "Coletado",
        [8]  = "Perdido",
        [9]  = "PRONTO!",
        [10] = "Voce ja coletou a recompensa de hoje!",
        [11] = "Recompensa coletada com sucesso!",
        [12] = "BONUS do mes coletado!",
        [13] = "Inventario cheio! Libere espaco e tente novamente.",
        MonthNames = {
            [1]="Janeiro",[2]="Fevereiro",[3]="Marco",[4]="Abril",
            [5]="Maio",[6]="Junho",[7]="Julho",[8]="Agosto",
            [9]="Setembro",[10]="Outubro",[11]="Novembro",[12]="Dezembro",
        },
    },
    ["Eng"] = {
        [1]  = "Daily Reward",
        [2]  = "Collect your reward every day!",
        [3]  = "Collect",
        [4]  = "Close",
        [5]  = "MONTH BONUS",
        [6]  = "Day",
        [7]  = "Collected",
        [8]  = "Missed",
        [9]  = "READY!",
        [10] = "You already collected today's reward!",
        [11] = "Reward collected successfully!",
        [12] = "Month BONUS collected!",
        [13] = "Inventory full! Free some space and try again.",
        MonthNames = {
            [1]="January",[2]="February",[3]="March",[4]="April",
            [5]="May",[6]="June",[7]="July",[8]="August",
            [9]="September",[10]="October",[11]="November",[12]="December",
        },
    },
}

DailyReward_Window = {
    W         = 640,
    H         = 510,
    CalCols   = 7,
    CalCellW  = 52,
    CalCellH  = 52,
    CalOffX   = 8,
    CalOffY   = 32,
    PanelX    = 375,
    PanelW    = 140,
    PanelOffY = 35,
    BonusCellX = 375,
    BonusCellY = 152,
    BonusCellW = 140,
    BonusCellH = 160,
}

DailyReward_Colors = {
    Title        = {200, 200, 100, 255},
    SubTitle     = {160, 160, 160, 255},
    DayNormal    = {180, 180, 180, 255},
    DayToday     = {255, 220,  60, 255},
    DayCollected = { 80, 180,  80, 255},
    BtnCollect   = {220, 255, 180, 255},
    BtnClose     = {255, 210, 200, 255},
    BtnDisabled  = {100, 100, 100, 255},
    BonusText    = {255, 215,   0, 255},
    ReadyFlash   = {100, 255, 150, 255},
}