if not ESCESC_CONFIG or not ESCESC_CONFIG.Enabled then
    return
end
EscEsc = {}
EscEsc.GMIndex = nil
EscEsc.Active = false
EscEsc.EndTime = 0
EscEsc.TimerId = nil
EscEsc.ReminderTimerId = nil
math.randomseed(os.time() + os.clock() * 1000)
local function EE_Global(id, ...)
    local msg = ESCESC_MESSAGES["Por"][id]
    if msg then
        SendMessageGlobal(string.format(msg, ...), 0)
    end
end
local function EE_Private(aIndex, id, ...)
    local msg = ESCESC_MESSAGES["Por"][id]
    if msg then
        SendMessage(aIndex, 1, string.format(msg, ...))
    end
end
function EscEsc.Start(aIndex)
    local p = User.new(aIndex)
   
    if p:getAuthority() < ESCESC_CONFIG.GMLevel then
        SendMessage("Sem permissão para usar este comando.", aIndex, 1)
        return
    end
   
    if EscEsc.Active then
        EE_Global(6)
        SendMessage("Evento Esconde-Esconde cancelado.", aIndex, 1)
        EscEsc.End()
        return
    end
   
    EscEsc.Active = true
    EscEsc.GMIndex = aIndex
    EscEsc.EndTime = os.time() + (ESCESC_CONFIG.MaxDuration * 60)
   
    EE_Global(2, p:getName(), ESCESC_CONFIG.MaxDuration)
   
    if EscEsc.TimerId then Timer.Cancel(EscEsc.TimerId) end
    EscEsc.TimerId = Timer.TimeOut(ESCESC_CONFIG.MaxDuration * 60, function()
        if EscEsc.Active then
            EE_Global(8)
            EscEsc.End()
        end
    end)
   
    if ESCESC_CONFIG.ReminderInterval > 0 then
        if EscEsc.ReminderTimerId then Timer.Cancel(EscEsc.ReminderTimerId) end
        EscEsc.ReminderTimerId = Timer.TimeOut(ESCESC_CONFIG.ReminderInterval * 60, EscEsc.SendReminder)
    end
end
function EscEsc.SendReminder()
    if not EscEsc.Active or not EscEsc.GMIndex then return end
   
    local gm = User.new(EscEsc.GMIndex)
    if gm:getConnected() <= 0 then
        EscEsc.End()
        return
    end
   
    local remaining = math.ceil((EscEsc.EndTime - os.time()) / 60)
    
    -- CORREÇÃO: Não exibe mais o lembrete quando o tempo estiver acabando
    if remaining > 0 then
        EE_Global(3, remaining)
    end
   
    EscEsc.ReminderTimerId = Timer.TimeOut(ESCESC_CONFIG.ReminderInterval * 60, EscEsc.SendReminder)
end
function EscEsc.End()
    EscEsc.Active = false
    if EscEsc.TimerId then
        Timer.Cancel(EscEsc.TimerId)
        EscEsc.TimerId = nil
    end
    if EscEsc.ReminderTimerId then
        Timer.Cancel(EscEsc.ReminderTimerId)
        EscEsc.ReminderTimerId = nil
    end
    EscEsc.GMIndex = nil
end
-- ==================== FUNÇÃO DE TRADE (CORRIGIDA) ====================
function EscEsc.PlayerSendTrade(aIndex, targetIndex)
    -- IMPORTANTE: Só interfere enquanto o evento estiver ATIVO
    if not EscEsc.Active then
        return 0 -- permite todos os trades normais
    end
   
    -- Vitória: jogador deu trade no GM
    if targetIndex == EscEsc.GMIndex then
        local player = User.new(aIndex)
        local winnerName = player:getName()
        local gm = User.new(EscEsc.GMIndex)
       
        local gmX = gm:getX()
        local gmY = gm:getY()
       
        EE_Global(4, winnerName, gmX, gmY)
        EE_Private(aIndex, 5)
       
        EscEsc.End()
        return 0
    end
   
    return 0 -- permite trade normal entre jogadores
end
-- ===================================================================
function EscEsc.Command(aIndex)
    EscEsc.Start(aIndex)
end
function EscEsc.Init()
    if not ESCESC_CONFIG.Enabled then return end
   
    GameServerFunctions.PlayerSendTrade(EscEsc.PlayerSendTrade)
    Commands.Register(ESCESC_CONFIG.Command, EscEsc.Command)
end
EscEsc.Init()
return EscEsc