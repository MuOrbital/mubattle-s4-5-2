DailyReward            = {}
DailyReward_Buttons    = {}
DailyReward_IconButton = { X1 = 0, X2 = 0, Y1 = 0, Y2 = 0 }

DailyReward_State = {
    month      = 0,
    day        = 0,
    collected  = false,
    bonusReady = false,
}

DailyReward_CalData = {
    month     = 0,
    totalDays = 0,
    days      = {},
    bonus     = nil,
}

function DailyReward.T(key)
    local lang = GetLanguage and GetLanguage() or "Por"
    local t = DailyReward_Text[lang] or DailyReward_Text["Por"]
    return t[key] or tostring(key)
end

function DailyReward.Open()
    OpenWindow(UIInventory)
    UICustomInterface = DailyReward_WindowID
    DailyReward_CalData.totalDays = 0
    DailyReward_CalData.days = {}
    DailyReward_CalData.bonus = nil
    DailyReward.RequestData()
end

function DailyReward.Close()
    CloseWindow(UIInventory)
    UICustomInterface = 0
end

function DailyReward.CheckInterfaces()
    if not DailyReward.IsOpen() then return end

    if CheckWindowOpen(UIInventory)     == 0 then DailyReward.Close() return end
    if CheckWindowOpen(UIFriendList)    == 1 then DailyReward.Close() return end
    if CheckWindowOpen(UIMoveList)      == 1 then DailyReward.Close() return end
    if CheckWindowOpen(UIParty)         == 1 then DailyReward.Close() return end
    if CheckWindowOpen(UIQuest)         == 1 then DailyReward.Close() return end
    if CheckWindowOpen(UIGuild)         == 1 then DailyReward.Close() return end
    if CheckWindowOpen(UIGuildNpc)      == 1 then DailyReward.Close() return end
    if CheckWindowOpen(UITrade)         == 1 then DailyReward.Close() return end
    if CheckWindowOpen(UIWarehouse)     == 1 then DailyReward.Close() return end
    if CheckWindowOpen(UIChaosBox)      == 1 then DailyReward.Close() return end
    if CheckWindowOpen(UICommandWindow) == 1 then DailyReward.Close() return end
    if CheckWindowOpen(UIPetInfo)       == 1 then DailyReward.Close() return end
    if CheckWindowOpen(UIShop)          == 1 then DailyReward.Close() return end
    if CheckWindowOpen(UIStore)         == 1 then DailyReward.Close() return end
    if CheckWindowOpen(UIOtherStore)    == 1 then DailyReward.Close() return end
    if CheckWindowOpen(UICharacter)     == 1 then DailyReward.Close() return end
    if CheckWindowOpen(UIOptions)       == 1 then DailyReward.Close() return end
    if CheckWindowOpen(UIHelp)          == 1 then DailyReward.Close() return end
    if CheckWindowOpen(UIFastDial)      == 1 then DailyReward.Close() return end
    if CheckWindowOpen(UISkillTree)     == 1 then DailyReward.Close() return end
    if CheckWindowOpen(UINPC_Titus)     == 1 then DailyReward.Close() return end
    if CheckWindowOpen(UICashShop)      == 1 then DailyReward.Close() return end
    if CheckWindowOpen(UIFullMap)       == 1 then DailyReward.Close() return end
    if CheckWindowOpen(UINPC_Dialog)    == 1 then DailyReward.Close() return end
    if CheckWindowOpen(UIGensInfo)      == 1 then DailyReward.Close() return end
    if CheckWindowOpen(UINPC_Julia)     == 1 then DailyReward.Close() return end
end

function DailyReward.IsOpen()
    return UICustomInterface == DailyReward_WindowID
end

function DailyReward.CreateButton(id, x, y, w, h)
    DailyReward_Buttons[id] = { X1 = x, X2 = x + w, Y1 = y, Y2 = y + h }
end

function DailyReward.CheckButton(id)
    local b = DailyReward_Buttons[id]
    if not b then return false end
    local mx, my = MousePosX(), MousePosY()
    return (mx >= b.X1 and mx <= b.X2 and my >= b.Y1 and my <= b.Y2)
end

function DailyReward.GetWindowPos()
    local cfg = DailyReward_Window
    local wx  = 20 + GetWideX()
    local wy  = 60
    return wx, wy, cfg.W, cfg.H
end

function DailyReward.RequestData()
    local nome = UserGetName()
    local pkt  = "DR_RequestData_" .. nome
    CreatePacket(pkt, DailyReward_Packet)
    SendPacket(pkt)
    ClearPacket(pkt)
end

function DailyReward.RequestCollect()
    local nome = UserGetName()
    local pkt  = "DR_Collect_" .. nome
    CreatePacket(pkt, DailyReward_Packet)
    SendPacket(pkt)
    ClearPacket(pkt)
end

function DailyReward.ShowTooltip(mx, my, item)
    SetBlend()
    ShowDescriptionComplete(
        mx, my,
        GET_ITEM(item.section, item.index),
        item.level   or 0,
        item.dur     or 255,
        item.skill   or 0,
        item.luck    or 0,
        item.opt     or 0,
        item.exc     or 0,
        item.ancient or 0,
        item.option  or 0,
        0, 0, 255, 0, 0, 0, 0
    )
    SetBlend()
    glColor4f(1, 1, 1, 1)
end

function DailyReward.DrawIcon()
    local ix = 100 + GetWideX()
    local iy = 3
    local iw = 25
    local ih = 25
 
    DailyReward_IconButton = { X1 = ix, X2 = ix + iw, Y1 = iy, Y2 = iy + ih }
 
    local mx, my = MousePosX(), MousePosY()
    local hover  = (mx >= ix and mx <= ix + iw and my >= iy and my <= iy + ih)
 
    local hwidJaColetou = DailyReward_State.collected and not DailyReward_State.bonusReady
    local temRecompensa = DailyReward_State.day ~= 0 and not hwidJaColetou
 
    local alpha = 0.85
    if temRecompensa then
        local flash = math.floor(os.clock() * 2) % 2 == 0
        alpha = flash and 1.0 or 0.4
    elseif hover then
        alpha = 1.0
    end
 
    SetBlend()
    glColor4f(1, 1, 1, alpha)
    EnableAlphaTest()
    RenderImage2(40011, ix, iy, iw, ih, 0, 0.1, 1.0, 1.0, 1, 1, alpha)
    DisableAlphaBlend()
    glColor4f(1, 1, 1, 1)
end

function DailyReward.Draw()
    if not DailyReward.IsOpen() then return end
 
    local cfg  = DailyReward_Window
    local col  = DailyReward_Colors
    local wx, wy, ww, wh = DailyReward.GetWindowPos()
 
    SetBlend()
    EnableAlphaTest()
    RenderImage2(31592, wx - 18, wy - 10, ww + 40, wh + 80, 0, 0, 1.0, 1.0, 1, 1, 1.0)
    DisableAlphaBlend()
 
    SetFontType(2)
    SetTextBg(0, 0, 0, 1)
    SetTextColor(col.Title[1], col.Title[2], col.Title[3], col.Title[4])
    RenderText2(wx + 120, wy + 8, DailyReward.T(1), 140, 3)
 
    local langData  = DailyReward_Text[GetLanguage and GetLanguage() or "Por"] or DailyReward_Text["Por"]
    local monthName = langData.MonthNames[DailyReward_State.month] or ("Mes " .. tostring(DailyReward_State.month))
    SetFontType(1)
    SetTextColor(col.SubTitle[1], col.SubTitle[2], col.SubTitle[3], col.SubTitle[4])
    RenderText2(wx + ww / 2 - 110, wy + 16, monthName, 160, 3)
 
    if DailyReward_CalData.totalDays == 0 then
        SetFontType(1)
        SetTextBg(0, 0, 0, 1)
        SetTextColor(200, 200, 100, 255)
        RenderText2(wx + ww / 2 - 80, wy + wh / 2 - 10, "Carregando...", 160, 3)
        return
    end
 
    local calData = DailyReward_CalData
    local cellW   = cfg.CalCellW
    local cellH   = cfg.CalCellH
    local calX    = wx + cfg.CalOffX
    local calY    = wy + cfg.CalOffY
 
    local tooltipItem = nil
    local tooltipMX   = 0
    local tooltipMY   = 0
 
    for d = 1, calData.totalDays do
        local col_    = (d - 1) % cfg.CalCols
        local row_    = math.floor((d - 1) / cfg.CalCols)
        local cx      = calX + col_ * cellW
        local cy      = calY + row_ * (cellH + 2)
        local dayInfo = calData.days[d]
        local isToday = (d == DailyReward_State.day)
        local isDone  = dayInfo and dayInfo.collected == 1
        local isPast  = (d < DailyReward_State.day) and not isDone
 
        RenderImage2(31592, cx, cy + 5, cellW + 30, cellH + 43, 0.0002, 0.00009, 1.0, 1.0, 1, 1, 1.0)
        DisableAlphaBlend()
 
        if dayInfo and dayInfo.section ~= 255 then
            local itemCode = GET_ITEM(dayInfo.section, dayInfo.index)
            if dayInfo.section == 12 then
                CreateItem(cx + 5, cy + 20, cellW - 12, cellH - 20, itemCode, dayInfo.level or 0, 0, 0, dayInfo.exc or 0)
            else
                CreateItem(cx + 5, cy + 15, cellW - 12, cellH - 20, itemCode, dayInfo.level or 0, 0, 0, dayInfo.exc or 0)
            end
        end
 
        SetFontType(1)
        SetTextBg(0, 0, 0, 1)
        if isDone then
            SetTextColor(col.DayCollected[1], col.DayCollected[2], col.DayCollected[3], col.DayCollected[4])
        elseif isToday then
            SetTextColor(col.DayToday[1], col.DayToday[2], col.DayToday[3], col.DayToday[4])
        else
            SetTextColor(col.DayNormal[1], col.DayNormal[2], col.DayNormal[3], col.DayNormal[4])
        end
        RenderText(cx + 4, cy + cellH - 3, tostring(d), cellW - 4, 0)

        if dayInfo and dayInfo.section ~= 255 then
            SetFontType(0)
            SetTextBg(0, 0, 0, 0)
            SetTextColor(220, 220, 100, 255)
            RenderText(cx + 35, cy + 50, "x" .. tostring(dayInfo.count or 1), cellW - 4, 0)
        end
 
        if isDone then
            SetFontType(0)
            SetTextBg(0, 0, 0, 0)
            SetTextColor(80, 140, 255, 220)
            RenderText2(cx, cy + 12, DailyReward.T(7), cellW - 4, 3)
        elseif isPast then
            SetFontType(0)
            SetTextBg(0, 0, 0, 0)
            SetTextColor(220, 50, 50, 220)
            RenderText2(cx, cy + 12, DailyReward.T(8), cellW - 4, 3)
        end
 
        local mx_, my_ = MousePosX(), MousePosY()
        if mx_ >= cx and mx_ <= cx + cellW and my_ >= cy and my_ <= cy + cellH then
            if dayInfo and dayInfo.section ~= 255 then
                tooltipItem = dayInfo
                tooltipMX   = mx_
                tooltipMY   = my_
            end
        end
    end
 
    local bonX = wx + cfg.BonusCellX - 4
    local bonY = wy + cfg.BonusCellY - 110
    local bonW = cfg.BonusCellW
    local bonH = cfg.BonusCellH
 
    EnableAlphaTest()
    RenderImage2(31592, bonX - 207, bonY + 211, bonW - 55, bonH - 65, 0.0002, 0.00009, 1.0, 1.0, 1, 1, 1.0)
    DisableAlphaBlend()
 
    SetFontType(1)
    SetTextBg(0, 0, 0, 1)
    SetTextColor(col.BonusText[1], col.BonusText[2], col.BonusText[3], col.BonusText[4])
    RenderText(bonX - 247, bonY + 268, DailyReward.T(5), bonW - 8, 3)
 
    if calData.bonus then
        local bon = calData.bonus
        if bon.section and bon.section ~= 255 then
            local bonCode = GET_ITEM(bon.section, bon.index)
            CreateItem(bonX - 198, bonY + 220, 30, 30, bonCode, bon.level or 0, 0, 0, bon.exc or 0)
        end
        if bon.section and bon.section ~= 255 and (bon.count or 1) > 1 then
            SetFontType(0)
            SetTextBg(0, 0, 0, 1)
            SetTextColor(220, 220, 100, 255)
            RenderText(bonX - 188, bonY + 254, "x" .. tostring(bon.count), 40, 3)
        end
        if bon.collected == 1 then
            SetFontType(0)
            SetTextBg(0, 0, 0, 0)
            SetTextColor(220, 220, 100, 255)
            RenderText(bonX + 4, bonY + bonH - 12, DailyReward.T(7), bonW - 8, 3)
        end
        local mx_, my_ = MousePosX(), MousePosY()
        local imgX1 = bonX + 30
        local imgY1 = bonY + 230
        local imgX2 = bonX + 60
        local imgY2 = bonY + 260
        if mx_ >= imgX1 - 220 and mx_ <= imgX2 - 220 and my_ >= imgY1 and my_ <= imgY2 then
            if bon.section ~= 255 then
                tooltipItem = bon
                tooltipMX   = mx_
                tooltipMY   = my_
            end
        end
    end
 
    if tooltipItem then
        DailyReward.ShowTooltip(tooltipMX, tooltipMY, tooltipItem)
    end
 
    local btnCX = wx + cfg.PanelX - 130
    local btnCY = 330
    local btnCW = cfg.PanelW - 40
    local btnCH = 28
 
    local hwidJaColetou = DailyReward_State.collected and not DailyReward_State.bonusReady
    local canCollect    = not hwidJaColetou
 
    DailyReward.CreateButton("collect", btnCX, btnCY, btnCW, btnCH)
 
    EnableAlphaTest()
    if not canCollect then
        RenderImage2(31323, btnCX, btnCY, btnCW, btnCH, 0, 0, 1.0, 0.2245212, 1, 1, 0.35)
    elseif DailyReward.CheckButton("collect") then
        RenderImage2(31323, btnCX, btnCY, btnCW, btnCH, 0, 0.2264566, 1.0, 0.2245212, 1, 1, 1.0)
    else
        RenderImage2(31323, btnCX, btnCY, btnCW, btnCH, 0, 0, 1.0, 0.2245212, 1, 1, 1.0)
    end
    DisableAlphaBlend()
 
    SetFontType(2)
    SetTextBg(0, 0, 0, 1)
    if canCollect then
        SetTextColor(col.BtnCollect[1], col.BtnCollect[2], col.BtnCollect[3], col.BtnCollect[4])
    else
        SetTextColor(col.BtnDisabled[1], col.BtnDisabled[2], col.BtnDisabled[3], col.BtnDisabled[4])
    end
    RenderText(btnCX - 8, btnCY + 5, DailyReward.T(3), btnCW + 8, 3)
 
    DisableAlphaBlend()
end

function DailyReward.Click()
    local b  = DailyReward_IconButton
    local mx = MousePosX()
    local my = MousePosY()
    if mx >= b.X1 and mx <= b.X2 and my >= b.Y1 and my <= b.Y2 then
        if DailyReward.IsOpen() then
            DailyReward.Close()
        else
            DailyReward.Open()
        end
        return
    end
 
    if not DailyReward.IsOpen() then return end
 
    if DailyReward.CheckButton("collect") then
        local hwidJaColetou = DailyReward_State.collected and not DailyReward_State.bonusReady
        if hwidJaColetou then
            SendMessageClient(DailyReward.T(10))
        else
            DailyReward.RequestCollect()
        end
        return
    end
end

function DailyReward.Protocol(Packet, PacketName)
    if Packet ~= DailyReward_Packet then return false end
    local nome = UserGetName()

    if PacketName == "DR_State_" .. nome then
        local _sub = GetBytePacket(PacketName, -1)
        DailyReward_State.month      = GetBytePacket(PacketName, -1)
        DailyReward_State.day        = GetBytePacket(PacketName, -1)
        DailyReward_State.collected  = GetBytePacket(PacketName, -1) == 1
        DailyReward_State.bonusReady = GetBytePacket(PacketName, -1) == 1
        ClearPacket(PacketName)
        return true
    end

    if PacketName == "DR_Calendar_" .. nome then
        local _sub = GetBytePacket(PacketName, -1)
        DailyReward_CalData.month     = GetBytePacket(PacketName, -1)
        DailyReward_CalData.totalDays = GetBytePacket(PacketName, -1)
        DailyReward_CalData.days      = {}
        DailyReward_CalData.bonus     = nil
        for _ = 1, DailyReward_CalData.totalDays do
            local d       = GetBytePacket(PacketName, -1)
            local section = GetBytePacket(PacketName, -1)
            local index   = GetWordPacket(PacketName, -1)
            local level   = GetBytePacket(PacketName, -1)
            local dur     = GetBytePacket(PacketName, -1)
            local skill   = GetBytePacket(PacketName, -1)
            local luck    = GetBytePacket(PacketName, -1)
            local opt     = GetBytePacket(PacketName, -1)
            local exc     = GetBytePacket(PacketName, -1)
            local ancient = GetBytePacket(PacketName, -1)
            local option  = GetBytePacket(PacketName, -1)
            local count   = GetBytePacket(PacketName, -1)
            local col_    = GetBytePacket(PacketName, -1)
            DailyReward_CalData.days[d] = {
                section   = section,
                index     = index,
                level     = level,
                dur       = dur,
                skill     = skill,
                luck      = luck,
                opt       = opt,
                exc       = exc,
                ancient   = ancient,
                option    = option,
                count     = count,
                collected = col_,
            }
        end
        local hasBonus = GetBytePacket(PacketName, -1) == 1
        if hasBonus then
            DailyReward_CalData.bonus = {
                section   = GetBytePacket(PacketName, -1),
                index     = GetWordPacket(PacketName, -1),
                level     = GetBytePacket(PacketName, -1),
                dur       = GetBytePacket(PacketName, -1),
                skill     = GetBytePacket(PacketName, -1),
                luck      = GetBytePacket(PacketName, -1),
                opt       = GetBytePacket(PacketName, -1),
                exc       = GetBytePacket(PacketName, -1),
                ancient   = GetBytePacket(PacketName, -1),
                option    = GetBytePacket(PacketName, -1),
                count     = GetBytePacket(PacketName, -1),
                collected = GetBytePacket(PacketName, -1),
            }
        end
        ClearPacket(PacketName)
        return true
    end

    if PacketName == "DR_Open_" .. nome then
        local _sub = GetBytePacket(PacketName, -1)
        ClearPacket(PacketName)
        return true
    end

    if PacketName == "DR_NoSpace_" .. nome then
        local _sub = GetBytePacket(PacketName, -1)
        ClearPacket(PacketName)
        SendMessageClient(DailyReward.T(13))
        return true
    end

    if PacketName == "DR_CollectOK_" .. nome then
        local _sub       = GetBytePacket(PacketName, -1)
        local isBonus    = GetBytePacket(PacketName, -1) == 1
        local bonusReady = GetBytePacket(PacketName, -1) == 1
        ClearPacket(PacketName)
        if isBonus then
            SendMessageClient(DailyReward.T(12))
            DailyReward_State.bonusReady = false
            DailyReward_State.collected  = true
            if DailyReward_CalData.bonus then
                DailyReward_CalData.bonus.collected = 1
            end
        else
            SendMessageClient(DailyReward.T(11))
            local d = DailyReward_State.day
            if DailyReward_CalData.days[d] then
                DailyReward_CalData.days[d].collected = 1
            end
            DailyReward_State.collected  = not bonusReady
            DailyReward_State.bonusReady = bonusReady
        end
        return true
    end

    return false
end

function DailyReward.UpdateMouse()
    if DailyReward.IsOpen() then
        DisableClickClient()
        return
    end
    local b  = DailyReward_IconButton
    local mx = MousePosX()
    local my = MousePosY()
    if mx >= b.X1 and mx <= b.X2 and my >= b.Y1 and my <= b.Y2 then
        DisableClickClient()
    end
end

function DailyReward.Update()
    DailyReward.CheckInterfaces()
end

InterfaceController.BeforeMainProc(DailyReward.DrawIcon)
InterfaceController.BeforeMainProc(DailyReward.Draw)
InterfaceController.MainProc(DailyReward.Update)
InterfaceController.ClientProtocol(DailyReward.Protocol)
InterfaceController.InterfaceClickEvent(DailyReward.Click)
InterfaceController.UpdateMouse(DailyReward.UpdateMouse)

return DailyReward