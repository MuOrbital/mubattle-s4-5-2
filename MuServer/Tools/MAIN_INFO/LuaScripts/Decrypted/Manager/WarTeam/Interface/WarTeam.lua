WarTeam = {}
WarTeam.Active = false
WarTeam.RegActive = false
WarTeam.Players = {}
WarTeam.ShortPlayers = {}
WarTeam.PointsBlue = 0
WarTeam.PointsRed = 0
WarTeam.CapturingBlue = false
WarTeam.CaptureStartBlue = 0
WarTeam.CapturingBaseBlue = 0
WarTeam.TargetBaseBlue = 0
WarTeam.CapturingRed = false
WarTeam.CaptureStartRed = 0
WarTeam.CapturingBaseRed = 0
WarTeam.TargetBaseRed = 0

WarTeam.CaptureDuration = WarTeam_Config.CaptureTime

WarTeam.BaseOwnerA = 1
WarTeam.BaseOwnerB = 2
WarTeam.Buttons = {}

local ScoreUI = { PosX = 200 + 1*GetWideX(), PosY = 20, Width = 260, Height = 200 }
local UI = {
    TitleX = 20,
    ListStartY = 105,
    LineHeight = 19,
    ButtonRegisterX = 75,
    ButtonRegisterY = 290,
    ButtonRegisterW = 80,
    ButtonCloseX = 193,
    ButtonCloseY = 290,
    ButtonCloseW = 80
}

local ConfigUI = {
    OffsetX = 160 + 1 * GetWideX(),
    PosY = 35,
    Width = 350,
    Height = 520
}

WarTeam.ListConfig = {
    Offset = 0,
    Count = 8,
    Steps = 0,
    CurrentStep = 0
}

function WarTeam.CreateButton(id, x, y, w, h)
    WarTeam.Buttons[id] = { x = x, y = y, w = w, h = h }
end

function WarTeam.MouseOver(id)
    local b = WarTeam.Buttons[id]
    if not b then return false end
    local mx = MousePosX()
    local my = MousePosY()
    return mx >= b.x and mx <= (b.x + b.w) and my >= b.y and my <= (b.y + b.h)
end

function WarTeam.BaseNumberToTeam(v)
    if v == 1 then return "Blue"
    elseif v == 2 then return "Red"
    end
    return "?"
end

function WarTeam.GerarScrollBarPlayers()
    WarTeam.ListConfig.Steps = math.max(0, #WarTeam.Players - WarTeam.ListConfig.Count)
end

function WarTeam.GerarListaPlayers()
    WarTeam.ShortPlayers = {}
    local count = 1
    for i = 1, #WarTeam.Players do
        if i > WarTeam.ListConfig.Offset then
            if count <= WarTeam.ListConfig.Count then
                table.insert(WarTeam.ShortPlayers, WarTeam.Players[i])
                count = count + 1
            end
        end
    end
end

function WarTeam.Protocol(Packet, PacketName)
    if string.find(PacketName, "AC_LIST_OPEN") or string.find(PacketName, "AC_LIST_UPDATE") then
        WarTeam.Players = {}
        local count = GetBytePacket(PacketName, -1)
        for i = 1, count do
            local name = GetCharPacketLength(PacketName, -1, 11)
            if name and name ~= "" then
                name = string.gsub(name, "%z", "")
                name = string.gsub(name, "%s+$", "")
                table.insert(WarTeam.Players, name)
            end
        end
        if string.find(PacketName, "AC_LIST_OPEN") then
            WarTeam.RegActive = true
            WarTeam.ListConfig.Offset = 0
            WarTeam.ListConfig.CurrentStep = 0
        end
        WarTeam.GerarListaPlayers()
        WarTeam.GerarScrollBarPlayers()
        return true
    end

    if string.find(PacketName, "AC_START") then
        WarTeam.RegActive = false
        WarTeam.Active = true
        WarTeam.PointsBlue = 0
        WarTeam.PointsRed = 0
        WarTeam.CapturingBlue = false
        WarTeam.CapturingRed = false
        return true
    end

    if string.find(PacketName, "AC_SCORE_UPDATE") then
        WarTeam.PointsBlue = GetDwordPacket(PacketName, -1)
        WarTeam.PointsRed = GetDwordPacket(PacketName, -1)
        return true
    end

    if string.find(PacketName, "AC_END") then
        WarTeam.Active = false
        WarTeam.CapturingBlue = false
        WarTeam.CapturingRed = false
        WarTeam.Players = {}
        WarTeam.ShortPlayers = {}
        return true
    end

    if string.find(PacketName, "AC_BASE_STATE") then
        WarTeam.BaseOwnerA = GetBytePacket(PacketName, -1)
        WarTeam.BaseOwnerB = GetBytePacket(PacketName, -1)
        return true
    end

if string.find(PacketName, "AC_CAPTURE_BAR_CANCEL") then

    local active = GetBytePacket(PacketName, -1)
    local capturingBase = GetBytePacket(PacketName, -1)

    if capturingBase == 1 then
        WarTeam.CapturingBlue = false
        WarTeam.CaptureStartBlue = 0
        WarTeam.CapturingBaseBlue = 0
        WarTeam.TargetBaseBlue = 0
    elseif capturingBase == 2 then
        WarTeam.CapturingRed = false
        WarTeam.CaptureStartRed = 0
        WarTeam.CapturingBaseRed = 0
        WarTeam.TargetBaseRed = 0
    else
        WarTeam.CapturingBlue = false
        WarTeam.CapturingRed = false
    end

    return true
end

if string.find(PacketName, "AC_CAPTURE_BAR") then

    local active = GetBytePacket(PacketName, -1)
    local capturingBase = GetBytePacket(PacketName, -1)
    local targetBase = GetBytePacket(PacketName, -1)
    local duration = GetDwordPacket(PacketName, -1)

    local isBlueCapture = (capturingBase == 1)

    if active == 1 then

        if isBlueCapture then
            WarTeam.CapturingBlue = true
            WarTeam.CaptureStartBlue = os.clock()
            WarTeam.CapturingBaseBlue = capturingBase
            WarTeam.TargetBaseBlue = targetBase
        else
            WarTeam.CapturingRed = true
            WarTeam.CaptureStartRed = os.clock()
            WarTeam.CapturingBaseRed = capturingBase
            WarTeam.TargetBaseRed = targetBase
        end

    end

    return true
end

    return false
end

InterfaceController.ClientProtocol(WarTeam.Protocol)

function WarTeam.DrawRegister()
    if not WarTeam.RegActive then return end
    local x = ConfigUI.OffsetX
    local y = ConfigUI.PosY

    SetBlend()
    RenderImage2(40004, x, y, ConfigUI.Width, ConfigUI.Height, 0, 0, 1.0, 1.0, 1, 1, 1.0)

    SetFontType(1)
    SetTextColor(255, 255, 255, 255)
    SetTextBg(0, 0, 0, 0)
    local py = y + UI.ListStartY
    for i, name in ipairs(WarTeam.ShortPlayers) do
        RenderText(x + 95, py, string.format("%d. %s", WarTeam.ListConfig.Offset + i, name), 260, 1)
        py = py + UI.LineHeight
    end

    local btnRegX = x + UI.ButtonRegisterX
    local btnRegY = y + UI.ButtonRegisterY
    WarTeam.CreateButton(1, btnRegX, btnRegY, UI.ButtonRegisterW, 85)
    if WarTeam.MouseOver(1) then
        RenderImage2(40005, btnRegX - 3, btnRegY - 3, UI.ButtonRegisterW + 6, 105, 0, 0, 1.0, 1.0, 1, 1, 1.0)
    else
        RenderImage2(40005, btnRegX, btnRegY, UI.ButtonRegisterW, 105, 0, 0, 1.0, 1.0, 1, 1, 1.0)
    end
    SetFontType(1)
    SetTextColor(255, 0, 0, 255)
    SetTextBg(0, 0, 0, 0)
    RenderText(btnRegX + 3, btnRegY + 44, WarTeam_Config.RegisterButton, 80, 3)

    local btnCloseX = x + UI.ButtonCloseX
    local btnCloseY = y + UI.ButtonCloseY
    WarTeam.CreateButton(2, btnCloseX, btnCloseY, UI.ButtonCloseW, 85)
    if WarTeam.MouseOver(2) then
        RenderImage2(40005, btnCloseX - 3, btnCloseY - 3, UI.ButtonCloseW + 6, 105, 0, 0, 1.0, 1.0, 1, 1, 1.0)
    else
        RenderImage2(40005, btnCloseX, btnCloseY, UI.ButtonCloseW, 105, 0, 0, 1.0, 1.0, 1, 1, 1.0)
    end
    SetFontType(1)
    SetTextColor(255, 0, 0, 255)
    SetTextBg(0, 0, 0, 0)
    RenderText(btnCloseX + 4, btnCloseY + 44, WarTeam_Config.CloseButton, 70, 3)

    if #WarTeam.Players > WarTeam.ListConfig.Count then
        local scrollX = x + 295
        local scrollY = y + 115
        local scrollHeight = 280
        SetBlend()
        glColor4f(0.3, 0.3, 0.3, 0.6)
        DrawBar(scrollX, scrollY, 6, scrollHeight)
        EndDrawBar()
     
        local percent = WarTeam.ListConfig.CurrentStep / math.max(1, WarTeam.ListConfig.Steps)
        local cursorH = 40
        local cursorY = scrollY + (scrollHeight - cursorH) * percent
        SetBlend()
        glColor4f(0.7, 0.7, 0.7, 0.9)
        DrawBar(scrollX, cursorY, 6, cursorH)
        EndDrawBar()
    end
end

function WarTeam.DrawScore()
    if not WarTeam.Active then return end
    local x = ScoreUI.PosX
    local y = ScoreUI.PosY
  
    SetBlend()
    RenderImage2(40003, x+3, y -80, ScoreUI.Width + 80, ScoreUI.Height, 0, 0, 1.0, 1.0, 1, 1, 1.0)

    SetFontType(1)
    SetTextColor(100, 200, 255, 255)
    SetTextBg(0, 0, 0, 0)
    RenderText(x + 70, y + 3.5, tostring(WarTeam.PointsBlue), 60, 1)

    SetFontType(1)
    SetTextColor(255, 100, 100, 255)
    SetTextBg(0, 0, 0, 0)
    RenderText(x + 183, y + 3.5, tostring(WarTeam.PointsRed), 60, 1)
end

function WarTeam.DrawCaptureBar()
    if not WarTeam.Active then return end

    local barWidth = 260
    local barHeight = 22
    local barX = 220 + GetWideX()

    if WarTeam.CapturingBlue then
        local elapsed = os.clock() - WarTeam.CaptureStartBlue
        local percent = math.min(elapsed / WarTeam.CaptureDuration, 1.0)
        local barY = 52

        local attacker = WarTeam.BaseNumberToTeam(WarTeam.CapturingBaseBlue)
        local defender = WarTeam.BaseNumberToTeam(WarTeam.TargetBaseBlue)
        local text = (attacker == defender) and ("Time " .. attacker .. " Recuperando Base") or ("Time " .. attacker .. " Capturando Base " .. defender .. "!")

        SetBlend()
        glColor4f(0.16, 0.45, 0.95, 1.0)
        DrawBar(barX, barY, barWidth, barHeight)
        EndDrawBar()

        SetBlend()
        glColor4f(0.85, 0.10, 0.10, 1.0)
        DrawBar(barX + 2, barY + 2, (barWidth - 4) * percent, barHeight - 4)
        EndDrawBar()

        SetFontType(1)
        SetTextColor(255, 255, 255, 255)
        SetTextBg(0, 0, 0, 0)
        RenderText3(barX + 25, barY + 4, text, 210, 3)
    end

    if WarTeam.CapturingRed then
        local elapsed = os.clock() - WarTeam.CaptureStartRed
        local percent = math.min(elapsed / WarTeam.CaptureDuration, 1.0)
        local barY = 52 + 35

        local attacker = WarTeam.BaseNumberToTeam(WarTeam.CapturingBaseRed)
        local defender = WarTeam.BaseNumberToTeam(WarTeam.TargetBaseRed)
        local text = (attacker == defender) and ("Time " .. attacker .. " Recuperando Base") or ("Time " .. attacker .. " Capturando Base " .. defender .. "!")

        SetBlend()
        glColor4f(0.16, 0.45, 0.95, 1.0)
        DrawBar(barX, barY, barWidth, barHeight)
        EndDrawBar()

        SetBlend()
        glColor4f(0.85, 0.10, 0.10, 1.0)
        DrawBar(barX + 2, barY + 2, (barWidth - 4) * percent, barHeight - 4)
        EndDrawBar()

        SetFontType(1)
        SetTextColor(255, 255, 255, 255)
        SetTextBg(0, 0, 0, 0)
        RenderText3(barX + 25, barY + 4, text, 210, 3)
    end
end

function WarTeam.MainDraw()
    WarTeam.DrawRegister()
    WarTeam.DrawScore()
    WarTeam.DrawCaptureBar()
end

InterfaceController.MainProc(WarTeam.MainDraw)

function WarTeam.ScrollMouse(Direction)
    if not WarTeam.RegActive then return end
    local x = MousePosX()
    local y = MousePosY()
    local winX = ConfigUI.OffsetX
    local winY = ConfigUI.PosY
    if x >= winX + 30 and x <= winX + 310 and y >= winY + 100 and y <= winY + 400 then
        if Direction > 0 then
            if WarTeam.ListConfig.CurrentStep > 0 then
                WarTeam.ListConfig.CurrentStep = WarTeam.ListConfig.CurrentStep - 1
                WarTeam.ListConfig.Offset = WarTeam.ListConfig.Offset - 1
                WarTeam.GerarListaPlayers()
            end
        else
            if WarTeam.ListConfig.CurrentStep < WarTeam.ListConfig.Steps then
                WarTeam.ListConfig.CurrentStep = WarTeam.ListConfig.CurrentStep + 1
                WarTeam.ListConfig.Offset = WarTeam.ListConfig.Offset + 1
                WarTeam.GerarListaPlayers()
            end
        end
        return true
    end
end

InterfaceController.ScrollMouse(WarTeam.ScrollMouse)

function WarTeam.ClickEvent()
    if not WarTeam.RegActive then return false end
    if WarTeam.MouseOver(1) then
        CreatePacket("AC_REGISTER", WarTeam_Config.PacketRegisterUI)
        SendPacket("AC_REGISTER")
        ClearPacket("AC_REGISTER")
        return true
    end
    if WarTeam.MouseOver(2) then
        WarTeam.RegActive = false
        return true
    end
    return false
end

InterfaceController.InterfaceClickEvent(WarTeam.ClickEvent)

function WarTeam.UpdateMouse()
    if not WarTeam.RegActive then return end
    local x = ConfigUI.OffsetX
    if MousePosX() >= x and MousePosX() <= (x + ConfigUI.Width) then
        DisableClickClient()
    end
end

InterfaceController.UpdateMouse(WarTeam.UpdateMouse)

return WarTeam