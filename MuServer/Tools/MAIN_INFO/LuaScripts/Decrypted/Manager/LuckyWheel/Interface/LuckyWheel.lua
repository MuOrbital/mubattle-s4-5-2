LuckyWheel = {}
LuckyWheel_Selected     = 1
LuckyWheel_Running      = false
LuckyWheel_Result       = -1
LuckyWheel_Target       = -1
LuckyWheel_SpinTime     = 0
LuckyWheel_SpinDuration = 8
LuckyWheel_StepAccum    = 0
LuckyWheel_Buttons      = {}
LuckyWheel_ActiveWheel  = 1
LuckyWheel_AutoCollect  = false
LuckyWheel_HasPending   = false
LuckyWheel_PendingWheel = 1
LuckyWheel_PendingSlot  = 1
LuckyWheel_Balance = {0, 0, 0, 0}
LuckyWheel_WheelCost = {}
LuckyWheel_CoinNames  = {"", "", "", ""}
LuckyWheel_WheelNames = {}
LuckyWheel_ActiveWheelCount = nil

LuckyWheel_Window = {
    Y = 75,
    W = 380,
    H = 290,
}
function LuckyWheel.GetWheelCount()
    local n = LuckyWheel_ActiveWheelCount or LuckyWheel_WheelCount or 7
    if n < 1 then n = 1 end
    if n > 19 then n = 19 end
    return n
end

function LuckyWheel.GetItemScale()
    local s = LuckyWheel_ItemScale or 1.0
    if s <= 0 then s = 1.0 end
    return s
end

function LuckyWheel.Open()
    UICustomInterface = LuckyWheel_WindowID
end

function LuckyWheel.Close()
    UICustomInterface = 0
end

function LuckyWheel.CreateButton(id, x, y, w, h)
    LuckyWheel_Buttons[id] = {X1 = x, X2 = x + w, Y1 = y, Y2 = y + h}
end

function LuckyWheel.CheckButton(id)
    local b = LuckyWheel_Buttons[id]
    if not b then return false end
    local mx, my = MousePosX(), MousePosY()
    return (mx >= b.X1 and mx <= b.X2 and my >= b.Y1 and my <= b.Y2)
end

function LuckyWheel.GetWheelCost(wheel)
    return LuckyWheel_WheelCost and LuckyWheel_WheelCost[wheel] or nil
end

function LuckyWheel.GetWheelCostLines(wheel)
    local costs = LuckyWheel.GetWheelCost(wheel)
    if not costs or #costs == 0 then
        return {}
    end
    local lines = {}
    for _, cost in ipairs(costs) do
        table.insert(lines, string.format("%d %s", cost.Amount, cost.Coin))
    end
    return lines
end

function LuckyWheel.CanAffordWheel(wheel)
    local costs = LuckyWheel.GetWheelCost(wheel)
    if not costs or #costs == 0 then
        return true
    end
    for _, cost in ipairs(costs) do
        local idx = cost.CoinIdx or 0
        if idx > 0 and LuckyWheel_Balance[idx] < cost.Amount then
            return false
        end
    end
    return true
end

function LuckyWheel.RenderWheelItem(x, y, baseSize, itemData)
    local scale = itemData.Scale or 1.0
    if scale <= 0 then scale = 1.0 end

    local size = baseSize * scale

    if size < 10 then size = 10 end

    local offset = size / 2

    CreateItem(
        x - offset,
        y - offset,
        size,
        size,
        itemData.Index,
        itemData.Level or 0,
        itemData.Dur or 0,
        itemData.Opt or 0,
        itemData.Exc or 0
    )
end

function LuckyWheel.ShowItemTooltip(x, y, itemData)
    SetBlend()

    ShowDescriptionComplete(
        x,
        y,
        itemData.Index,
        itemData.Level or 0,
        itemData.Dur or 0,
        itemData.Skill or 0,
        itemData.Luck or 0,
        itemData.Opt or 0,
        itemData.Exc or 0,
        itemData.Ancient or 0,
        0, 0, 0, 255, 0, 0, 0, 0
    )

    SetBlend()
    glColor4f(1, 1, 1, 1)
end

function LuckyWheel.Draw()
    if UICustomInterface ~= LuckyWheel_WindowID then return end
    local cfg = LuckyWheel_Window
    local w = {
        X = 130 + 1 * GetWideX(),
        Y = cfg.Y,
        W = cfg.W,
        H = cfg.H,
    }
    local wheelCount = LuckyWheel.GetWheelCount()
    local wheel = LuckyWheel_HasPending and LuckyWheel_PendingWheel or LuckyWheel_ActiveWheel
    local items = LuckyWheel_Wheels[wheel]
    SetBlend()

    local btnSpin_X = w.X + 308
    local btnSpin_Y = w.Y + 204
    local btnSpin_W = 66
    local btnSpin_H = 30

    local btnAction_X = w.X + 308
    local btnAction_Y = w.Y + 240
    local btnAction_W = 66
    local btnAction_H = 30

    glColor4f(0.72, 0.58, 0.18, 1)
    DrawBar(w.X - 2, w.Y - 2, w.W + 4, w.H + 4)

    glColor4f(0.06, 0.06, 0.10, 0.98)
    DrawBar(w.X, w.Y, w.W, w.H)

    glColor4f(0.18, 0.14, 0.04, 1)
    DrawBar(w.X, w.Y, w.W, 36)
    glColor4f(0.72, 0.58, 0.18, 1)
    DrawBar(w.X, w.Y + 36, w.W, 2)

    local balX = w.X + 310
    local balY = w.Y + 42
    local balW = 60
    local balH = 65
    glColor4f(0.10, 0.08, 0.02, 0.95)
    DrawBar(balX, balY, balW, balH)
    glColor4f(0.72, 0.58, 0.18, 0.7)
    DrawBar(balX, balY, balW, 1)
    DrawBar(balX, balY + balH, balW, 1)
    DrawBar(balX, balY, 1, balH)
    DrawBar(balX + balW, balY, 1, balH)

    local costX = w.X + 310
    local costY = w.Y + 112
    local costW = 60
    local costH = 65
    glColor4f(0.08, 0.04, 0.10, 0.95)
    DrawBar(costX, costY, costW, costH)
    glColor4f(0.55, 0.20, 0.72, 0.7)
    DrawBar(costX, costY, costW, 1)
    DrawBar(costX, costY + costH, costW, 1)
    DrawBar(costX, costY, 1, costH)
    DrawBar(costX + costW, costY, 1, costH)

    local centerX = w.X + 188
    local centerY = w.Y + 157

    glColor4f(0.15, 0.12, 0.03, 1)
    DrawBar(centerX - 115, centerY - 115, 230, 230)
    glColor4f(0.06, 0.06, 0.10, 1)
    DrawBar(centerX - 108, centerY - 108, 216, 216)

    glColor4f(0.20, 0.16, 0.04, 1)
    DrawBar(centerX - 26, centerY - 26, 52, 52)
    glColor4f(0.72, 0.58, 0.18, 1)
    DrawBar(centerX - 28, centerY - 28, 56, 2)
    DrawBar(centerX - 28, centerY + 26, 56, 2)
    DrawBar(centerX - 28, centerY - 28, 2, 56)
    DrawBar(centerX + 26, centerY - 28, 2, 56)
    LuckyWheel.RenderWheelItem(centerX, centerY, 40, items[LuckyWheel_Selected])

    local tooltipX, tooltipY, tooltipItem = nil, nil, nil
    
    for i = 1, 12 do
        local angle  = (i * 30) * 0.0174533
        local radius = 90
        local ix     = centerX + math.cos(angle) * radius
        local iy     = centerY + math.sin(angle) * radius
        local baseSize = 20
    
        if i == LuckyWheel_Selected then
            glColor4f(0.72, 0.58, 0.18, 0.85)
            DrawBar(ix - 14, iy - 14, 28, 28)
            glColor4f(0.06, 0.06, 0.10, 1)
            DrawBar(ix - 12, iy - 12, 24, 24)
            baseSize = 22
        end
    
        LuckyWheel.RenderWheelItem(ix, iy, baseSize, items[i])
    
        local scale = items[i].Scale or 1.0
        if scale <= 0 then scale = 1.0 end
    
        local size = math.floor(baseSize * scale)
        if size < 1 then size = 1 end
    
        local half = math.floor(size / 2)
        local mx = MousePosX()
        local my = MousePosY()
    
        if mx >= (ix - half) and mx <= (ix + half) and my >= (iy - half) and my <= (iy + half) then
            tooltipX = mx
            tooltipY = my
            tooltipItem = items[i]
        end
    end
    
    if tooltipItem then
        LuckyWheel.ShowItemTooltip(tooltipX, tooltipY, tooltipItem)
    end
    DisableAlphaBlend()
    SetBlend()
    glColor4f(1, 1, 1, 1)
    local tabX = w.X + w.W - 376
    local tabY = w.Y + 43
    local tabH = 14.2
    local tabGap = 5.3
    for i = 1, wheelCount do
        LuckyWheel.CreateButton(10 + i, tabX, tabY + (i - 1) * (tabH + tabGap), 66, tabH)

        local canAfford = LuckyWheel.CanAffordWheel(i)

        if LuckyWheel_HasPending and LuckyWheel_PendingWheel == i then
            glColor4f(0.72, 0.58, 0.18, 1)
            DrawBar(tabX, tabY + (i - 1) * (tabH + tabGap), 66, tabH)
            glColor4f(0.10, 0.08, 0.02, 1)
            DrawBar(tabX + 1, tabY + (i - 1) * (tabH + tabGap) + 1, 66, tabH - 2)
        elseif LuckyWheel_ActiveWheel == i and not LuckyWheel_HasPending then
            glColor4f(0.72, 0.58, 0.18, 1)
            DrawBar(tabX, tabY + (i - 1) * (tabH + tabGap), 66, tabH)
            glColor4f(0.10, 0.08, 0.02, 1)
            DrawBar(tabX, tabY + (i - 1) * (tabH + tabGap) + 1, 66, tabH - 2)
        elseif not canAfford and not LuckyWheel_HasPending then
            glColor4f(0.18, 0.06, 0.06, 1)
            DrawBar(tabX, tabY + (i - 1) * (tabH + tabGap), 66, tabH)
        elseif LuckyWheel.CheckButton(10 + i) and not LuckyWheel_HasPending then
            glColor4f(0.35, 0.28, 0.08, 1)
            DrawBar(tabX, tabY + (i - 1) * (tabH + tabGap), 66, tabH)
        else
            glColor4f(0.14, 0.11, 0.03, 1)
            DrawBar(tabX, tabY + (i - 1) * (tabH + tabGap), 66, tabH)
        end
    end

    LuckyWheel.CreateButton(1, btnSpin_X, btnSpin_Y, btnSpin_W, btnSpin_H)

    local spinBlocked = LuckyWheel_Target ~= -1 or LuckyWheel_Running or LuckyWheel_HasPending

    if not spinBlocked then
        if not LuckyWheel.CanAffordWheel(LuckyWheel_ActiveWheel) then
            spinBlocked = true
        end
    end

    if spinBlocked then
        glColor4f(0.20, 0.20, 0.20, 1)
        DrawBar(btnSpin_X, btnSpin_Y, btnSpin_W, btnSpin_H)
        glColor4f(0.35, 0.35, 0.35, 1)
        DrawBar(btnSpin_X, btnSpin_Y, btnSpin_W, 1)
    else
        if LuckyWheel.CheckButton(1) then
            glColor4f(0.20, 0.60, 0.10, 1)
        else
            glColor4f(0.10, 0.38, 0.06, 1)
        end
        DrawBar(btnSpin_X, btnSpin_Y, btnSpin_W, btnSpin_H)
        glColor4f(0.40, 0.90, 0.20, 1)
        DrawBar(btnSpin_X, btnSpin_Y, btnSpin_W, 1)
    end

    if LuckyWheel_HasPending then
        LuckyWheel.CreateButton(3, btnAction_X, btnAction_Y, btnAction_W, btnAction_H)
        if LuckyWheel.CheckButton(3) then
            glColor4f(0.10, 0.55, 0.60, 1)
        else
            glColor4f(0.05, 0.32, 0.36, 1)
        end
        DrawBar(btnAction_X, btnAction_Y, btnAction_W, btnAction_H)
        glColor4f(0.20, 0.90, 0.90, 0.6)
        DrawBar(btnAction_X, btnAction_Y, btnAction_W, 1)
    else
        LuckyWheel.CreateButton(2, btnAction_X, btnAction_Y, btnAction_W, btnAction_H)
        if LuckyWheel.CheckButton(2) then
            glColor4f(0.65, 0.10, 0.06, 1)
        else
            glColor4f(0.38, 0.06, 0.04, 1)
        end
        DrawBar(btnAction_X, btnAction_Y, btnAction_W, btnAction_H)
        glColor4f(0.90, 0.30, 0.20, 0.6)
        DrawBar(btnAction_X, btnAction_Y, btnAction_W, 1)
    end

    SetFontType(2)
    SetTextBg(0, 0, 0, 1)
    SetTextColor(255, 220, 80, 255)
    RenderText(w.X + 90, w.Y + 10, LuckyWheel_Text["Por"][1], 200, 3)

    SetFontType(1)
    SetTextBg(0, 0, 0, 0)
    SetTextColor(255, 220, 80, 255)
    RenderText(balX + 4, balY + 2, LuckyWheel_Text["Por"][6], balW - 8, 3)

    local coinColors = {{255,200,60},{100,200,255},{100,255,140},{255,140,255}}
    for i = 1, 4 do
        local c = coinColors[i]
        SetTextColor(c[1], c[2], c[3], 255)
        SetFontType(0)
        SetTextBg(0, 0, 0, 0)
        local label = string.format("%s: %d", LuckyWheel_CoinNames[i], LuckyWheel_Balance[i])
        RenderText(balX + 4, balY + 14 + (i-1)*13, label, balW - 8, 0)
    end

    SetFontType(1)
    SetTextBg(0, 0, 0, 0)
    SetTextColor(210, 160, 255, 255)
    RenderText(costX + 4, costY + 2, LuckyWheel_Text["Por"][5], costW - 8, 3)

    local lines = LuckyWheel.GetWheelCostLines(wheel)

    SetFontType(0)
    SetTextBg(0, 0, 0, 0)
    SetTextColor(255, 255, 180, 255)

    for i, line in ipairs(lines) do
        RenderText(costX + 4, costY + 18 + (i - 1) * 12, line, costW - 8, 0)
    end

    if not LuckyWheel_Running and LuckyWheel_Target ~= -1 then
        SetFontType(1)
        SetTextBg(0, 0, 0, 0)
        SetTextColor(255, 220, 60, 255)
        RenderText(centerX - 110, centerY + 100, items[LuckyWheel_Selected].Nome, 220, 3)
    end

    for i = 1, wheelCount do
        local activeTab = LuckyWheel_HasPending and (LuckyWheel_PendingWheel == i) or (LuckyWheel_ActiveWheel == i)
        local canAfford = LuckyWheel.CanAffordWheel(i)
        if activeTab then
            SetTextColor(255, 220, 80, 255)
        elseif not canAfford and not LuckyWheel_HasPending then
            SetTextColor(120, 60, 60, 255)
        else
            SetTextColor(160, 140, 60, 255)
        end
        SetFontType(1)
        SetTextBg(0, 0, 0, 0)
        local name = (LuckyWheel_WheelNames and LuckyWheel_WheelNames[i]) or ("R" .. i)
        RenderText(tabX + 10, tabY + (i - 1) * (tabH + tabGap) + 4, name, 52, 3)
    end

    if LuckyWheel_HasPending then
        SetFontType(1)
        SetTextBg(0, 0, 0, 0)
        SetTextColor(255, 200, 60, 255)
        RenderText(w.X, w.Y + 280, "Premio pendente! Clique em COLETAR PREMIO.", 370, 3)
    elseif not LuckyWheel_Running and LuckyWheel_Target ~= -1 then
        SetFontType(1)
        SetTextBg(0, 0, 0, 0)
        SetTextColor(255, 160, 60, 255)
        RenderText(w.X, w.Y + 280, "Sem espaco! ALT + BOTAO DIREITO p/ coletar", 370, 3)
    elseif not LuckyWheel_Running and LuckyWheel_Target == -1 then
        SetFontType(1)
        SetTextBg(0, 0, 0, 0)
        SetTextColor(160, 160, 160, 255)
        RenderText(w.X, w.Y + 280, "Clique em GIRAR para tentar a sorte!", 370, 3)
    else
        SetFontType(0)
        SetTextBg(0, 0, 0, 0)
        SetTextColor(255, 200, 60, 255)
        RenderText(w.X, w.Y + 280, "Girando... boa sorte!", 370, 3)
    end

    SetFontType(2)
    SetTextBg(0, 0, 0, 0)
    if spinBlocked then
        SetTextColor(100, 100, 100, 255)
    else
        SetTextColor(220, 255, 180, 255)
    end
    RenderText(btnSpin_X - 4, btnSpin_Y + 8, LuckyWheel_Text["Por"][2], 80, 3)

    if LuckyWheel_HasPending then
        SetTextColor(180, 255, 255, 255)
        RenderText(btnAction_X - 4, btnAction_Y + 8, LuckyWheel_Text["Por"][4], 80, 3)
    else
        SetTextColor(255, 210, 200, 255)
        RenderText(btnAction_X - 4, btnAction_Y + 8, LuckyWheel_Text["Por"][3], 80, 3)
    end

    DisableAlphaBlend()
end

function LuckyWheel.Spin()
    if LuckyWheel_Running      then return end
    if LuckyWheel_Target ~= -1 then return end
    if LuckyWheel_HasPending   then return end

    LuckyWheel_Running     = true
    LuckyWheel_SpinTime    = 0
    LuckyWheel_StepAccum   = 0
    LuckyWheel_Target      = -1
    LuckyWheel_AutoCollect = false

    local nome = UserGetName()
    CreatePacket("LW_SpinW_" .. nome, LuckyWheel_Packet)
    SetBytePacket("LW_SpinW_" .. nome, LuckyWheel_ActiveWheel)
    SendPacket("LW_SpinW_" .. nome)
    ClearPacket("LW_SpinW_" .. nome)
end

function LuckyWheel.TryCollect()
    local nome = UserGetName()
    CreatePacket("LW_Get_" .. nome, LuckyWheel_Packet)
    SendPacket("LW_Get_" .. nome)
    ClearPacket("LW_Get_" .. nome)
end

function LuckyWheel.Update()
    if not LuckyWheel_Running then return end

    LuckyWheel_SpinTime = LuckyWheel_SpinTime + 0.016

    local progress = LuckyWheel_SpinTime / LuckyWheel_SpinDuration
    if progress > 1 then progress = 1 end

    local stepInterval   = 0.08 + (progress * progress) * 0.47
    LuckyWheel_StepAccum = LuckyWheel_StepAccum + 0.016

    if LuckyWheel_StepAccum >= stepInterval then
        LuckyWheel_StepAccum = LuckyWheel_StepAccum - stepInterval
        if progress < 1 or LuckyWheel_Target == -1 then
            LuckyWheel_Selected = LuckyWheel_Selected + 1
            if LuckyWheel_Selected > 12 then LuckyWheel_Selected = 1 end
        end
    end

    if LuckyWheel_Result ~= -1 then
        LuckyWheel_Target = LuckyWheel_Result
        LuckyWheel_Result = -1
    end

    if progress >= 1 and LuckyWheel_Target ~= -1 then
        if LuckyWheel_Selected ~= LuckyWheel_Target then
            if LuckyWheel_StepAccum >= 0.35 then
                LuckyWheel_StepAccum = 0
                LuckyWheel_Selected  = LuckyWheel_Selected + 1
                if LuckyWheel_Selected > 12 then LuckyWheel_Selected = 1 end
            end
        else
            LuckyWheel_Running   = false
            LuckyWheel_StepAccum = 0
            if not LuckyWheel_AutoCollect then
                LuckyWheel_AutoCollect = true
                LuckyWheel.TryCollect()
            end
        end
    end
end

function LuckyWheel.Click()
    if UICustomInterface ~= LuckyWheel_WindowID then return end

    if LuckyWheel_HasPending then
        if LuckyWheel.CheckButton(3) then
            LuckyWheel.TryCollect()
        end
        return
    end

    if LuckyWheel.CheckButton(1) then
        if LuckyWheel_Running or LuckyWheel_Target ~= -1 or LuckyWheel_HasPending then
            return
        end
        if LuckyWheel.CanAffordWheel(LuckyWheel_ActiveWheel) then
            LuckyWheel.Spin()
        end
        return
    end

    if not LuckyWheel_Running and LuckyWheel_Target == -1 then
        local wheelCount = LuckyWheel.GetWheelCount()
        for i = 1, wheelCount do
            if LuckyWheel.CheckButton(10 + i) then
                LuckyWheel_ActiveWheel = i
                LuckyWheel_Selected = 1
                return
            end
        end
    end

    if LuckyWheel.CheckButton(2) then
        LuckyWheel.Close()
        return
    end
end

function LuckyWheel.RightClick()
    if UICustomInterface ~= LuckyWheel_WindowID then return end
    if LuckyWheel_Running then return end
    if LuckyWheel_Target == -1 and not LuckyWheel_HasPending then return end
    if CheckRepeatKey(Keys.Menu) == 0 then return end
    LuckyWheel.TryCollect()
end

function LuckyWheel.UpdateMouse()
    if UICustomInterface == LuckyWheel_WindowID then
        DisableClickClient()
    end
end

function LuckyWheel.Protocol(Packet, PacketName)
    if Packet ~= LuckyWheel_Packet then return false end

    local nome = UserGetName()

    if PacketName == "LW_Meta_" .. nome then
        local wheelCount = GetBytePacket(PacketName, -1)
        LuckyWheel_ActiveWheelCount = wheelCount

        for i = 1, 4 do
            local len = GetBytePacket(PacketName, -1)
            local name = ""
            for _ = 1, len do
                local b = GetBytePacket(PacketName, -1)
                name = name .. string.char(b)
            end
            LuckyWheel_CoinNames[i] = name
        end

        LuckyWheel_WheelNames = {}
        for i = 1, wheelCount do
            local len = GetBytePacket(PacketName, -1)
            local name = ""
            for _ = 1, len do
                local b = GetBytePacket(PacketName, -1)
                name = name .. string.char(b)
            end
            LuckyWheel_WheelNames[i] = name
        end

        ClearPacket(PacketName)
        return true
    end

    if PacketName == "LW_Balance_" .. nome then
        LuckyWheel_Balance[1] = GetDwordPacket(PacketName, -1)
        LuckyWheel_Balance[2] = GetDwordPacket(PacketName, -1)
        LuckyWheel_Balance[3] = GetDwordPacket(PacketName, -1)
        LuckyWheel_Balance[4] = GetDwordPacket(PacketName, -1)
        ClearPacket(PacketName)
        return true
    end

    if PacketName == "LW_Open_" .. nome then
        LuckyWheel_HasPending = false
        LuckyWheel.Open()
        ClearPacket(PacketName)
        return true
    end

    if PacketName == "LW_Pending_" .. nome then
        local wNum = GetBytePacket(PacketName, -1)
        local slot = GetBytePacket(PacketName, -1)
        ClearPacket(PacketName)
        LuckyWheel_HasPending   = true
        LuckyWheel_PendingWheel = (wNum >= 1 and wNum <= 19) and wNum or 1
        LuckyWheel_PendingSlot  = (slot >= 1 and slot <= 12) and slot or 1
        LuckyWheel_ActiveWheel  = LuckyWheel_PendingWheel
        LuckyWheel_Selected     = LuckyWheel_PendingSlot
        LuckyWheel_Target       = LuckyWheel_PendingSlot
        LuckyWheel_Running      = false
        LuckyWheel.Open()
        return true
    end

    if PacketName == "LW_Result_" .. nome then
        LuckyWheel_Result = GetBytePacket(PacketName, -1)
        ClearPacket(PacketName)
        return true
    end

    if PacketName == "LW_GetOK_" .. nome then
        LuckyWheel_Target       = -1
        LuckyWheel_Result       = -1
        LuckyWheel_Running      = false
        LuckyWheel_SpinTime     = 0
        LuckyWheel_StepAccum    = 0
        LuckyWheel_AutoCollect  = false
        LuckyWheel_HasPending   = false
        LuckyWheel_PendingSlot  = 1
        LuckyWheel_PendingWheel = 1
        ClearPacket(PacketName)
        return true
    end

    if PacketName == "LW_NoSpace_" .. nome then
        LuckyWheel_AutoCollect = false
        SendMessageClient("Sem espaco! Libere o inventario e use ALT + botao direito para coletar.")
        ClearPacket(PacketName)
        return true
    end

    local wheelCount = LuckyWheel.GetWheelCount()
    for w = 1, wheelCount do
        if PacketName == "LW_Cost_" .. nome .. "_" .. w then
            local count = GetBytePacket(PacketName, -1)
            LuckyWheel_WheelCost[w] = {}
            for i = 1, count do
                local coinIndex = GetBytePacket(PacketName, -1)
                local amount    = GetDwordPacket(PacketName, -1)
                local coinName = LuckyWheel_CoinNames[coinIndex] or ("Moeda"..coinIndex)
                table.insert(LuckyWheel_WheelCost[w], {
                    Coin    = coinName,
                    CoinIdx = coinIndex,
                    Amount  = amount,
                })
            end
            ClearPacket(PacketName)
            return true
        end
    end

    return false
end

InterfaceController.BeforeMainProc(LuckyWheel.Draw)
InterfaceController.MainProc(LuckyWheel.Update)
InterfaceController.ClientProtocol(LuckyWheel.Protocol)
InterfaceController.InterfaceClickEvent(LuckyWheel.Click)
InterfaceController.InterfaceClickRightEvent(LuckyWheel.RightClick)
InterfaceController.UpdateMouse(LuckyWheel.UpdateMouse)

return LuckyWheel