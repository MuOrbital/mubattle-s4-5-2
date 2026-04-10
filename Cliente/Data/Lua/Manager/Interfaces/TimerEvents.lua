TIMER_EVENTS_SWITCH = 1
TIMER_EVENTS_PACKET = 5
TIMER_EVENTS_PACKET_NAME = 'TIMER_EVENTS_PACKET_OPEN'
TIMER_EVENTS_KEY_OPEN = Keys.H

TIMER_EVENTS_TEXTS = {}

TIMER_EVENTS_TEXTS['Por'] = {
[1] = 'Hor�rio dos eventos',
[2] = '%d dia(s)+',
[3] = 'Evento aberto',
[4] = 'Atualizar pagina',
}

TIMER_EVENTS_TEXTS['Eng'] = {
[1] = 'Time of events',
[2] = '%d days(s)+',
[3] = 'Open event',
[4] = 'Update Page',
}

TIMER_EVENTS_TEXTS['Spn'] = {
[1] = 'Hora de los eventos',
[2] = '%d dia(s)+',
[3] = 'Evento abierto',
[4] = 'Actualizar p�gina',
}

--need increase here if have more events
TIMER_EVENTS_COUNT = 21

TIMER_EVENTS_INFO = {}

TIMER_EVENTS_INFO[1] = {
    eventNameEng = '[Event] Blood Castle', eventNamePor = '[Event] Blood Castle', eventNameSpn = '[Event] Blood Castle'
}

TIMER_EVENTS_INFO[2] = {
    eventNameEng = '[Event] Devil Square', eventNamePor = '[Event] Devil Square', eventNameSpn = '[Event] Devil Square'
}

TIMER_EVENTS_INFO[3] = {
    eventNameEng = '[Event] Chaos Castle', eventNamePor = '[Event] Chaos Castle', eventNameSpn = '[Event] Chaos Castle'
}

TIMER_EVENTS_INFO[4] = {
    eventNameEng = '[Event] Illusion Temple', eventNamePor = '[Event] Illusion Temple', eventNameSpn = '[Event] Illusion Temple'
}

TIMER_EVENTS_INFO[5] = {
    eventNameEng = '[Invasão] Esqueleto King', eventNamePor = '[Invasão] Esqueleto King', eventNameSpn = '[Invasão] Esqueleto King'
}

TIMER_EVENTS_INFO[6] = {
    eventNameEng = '[Invasão] Red Dragon', eventNamePor = '[Invasão] Red Dragon', eventNameSpn = '[Invasão] Red Dragon'
}

TIMER_EVENTS_INFO[7] = {
    eventNameEng = '[Invasão] ]Dourados', eventNamePor = '[Invasão] Dourados', eventNameSpn = '[Invasão] Dourados'
}

TIMER_EVENTS_INFO[8] = {
    eventNameEng = '[Invasão] White Wizard', eventNamePor = '[Invasão] White Wizard', eventNameSpn = '[Invasão] White Wizard'
}

TIMER_EVENTS_INFO[9] = {
    eventNameEng = '[Invasão] New Year', eventNamePor = '[Invasão] New Year', eventNameSpn = '[Invasão] New Year'
}

TIMER_EVENTS_INFO[10] = {
    eventNameEng = '[Invasão] Rabbits', eventNamePor = '[Invasão] Rabbits', eventNameSpn = '[Invasão] Rabbits'
}

TIMER_EVENTS_INFO[11] = {
    eventNameEng = '[Invasão] Summer', eventNamePor = '[Invasão] Summer', eventNameSpn = '[Invasão] Summer'
}

TIMER_EVENTS_INFO[12] = {
    eventNameEng = '[Invasão] Christmas', eventNamePor = '[Invasão] Christmas', eventNameSpn = '[Invasão] Christmas'
}

TIMER_EVENTS_INFO[13] = {
    eventNameEng = '[Special] Mining Stones', eventNamePor = '[Special] Mining Stones', eventNameSpn = '[Special] Mining Stones'
}

TimerEvents = {}

local TimerEventsOpen = 0
local TimerEventsSystemTime = 0
local TimerEventsInfo = {}

local TimerEventsCurrentPage = 0
local TimerEventsMaxPages = 0
local TimerEventsStartLine = 0
local TimerEventsEndLine = 0
local TimerEventsMaxLines = 0

function TimerEvents.MainProc()
    if TimerEventsOpen == 0
    then
        return
    end

    EnableAlphaTest()

    glColor4f(1.0, 1.0, 1.0, 1.0)

    TimerEvents.RenderFrame()
end

function TimerEvents.RenderFrame()
    EnableAlphaTest()

    glColor4f(1.0, 1.0, 1.0, 1.0)

    local PosX = (640 / 2) - (230 / 2) + GetWideX()
    local PosY = 80

    --Render Window
    RenderImage(31319, PosX, PosY + 2.0, 222.0, 257.0)

    RenderImage(31328, PosX, PosY, 230, 67)

    --Close
    --RenderImage(32451, PosX + (230 - 36), PosY, 36, 29)

    PosY = PosY + 45.0

	for i = 1, 11 do
		RenderImage(31317, PosX, PosY, 230.0, 15.0)

        PosY = PosY + 15.0
    end

    RenderImage(31317, PosX, PosY, 230.0, 5.0)
    PosY = PosY + 5.0

	RenderImage(31318, PosX, PosY, 230.0, 45.0)

    PosY = 80

    --Render Texts
    TimerEvents.RenderText(PosX, PosY)


    if TimerEventsMaxPages > 0
    then
        --left
        if (MousePosX() >= (PosX + 70) and MousePosX() <= (PosX + 70) + 25)
            and (MousePosY() >= (PosY + 240) and MousePosY() <= (PosY + 240) + 16)
        then
            RenderImage2(32343, PosX + 75, PosY + 240, 18.0, 13.0, 0, 0.18, 0.52, 0.18, 1, 1, 1.0)
        else
            RenderImage2(32343, PosX + 75, PosY + 240, 18.0, 13.0, 0, 0.0, 0.52, 0.18, 1, 1, 1.0)
        end

        --right
        if (MousePosX() >= (PosX + 135) and MousePosX() <= (PosX + 135) + 25)
            and (MousePosY() >= (PosY + 240) and MousePosY() <= (PosY + 240) + 16)
        then
            RenderImage2(32344, PosX + 135, PosY + 240, 18.0, 13.0, 0, 0.18, 0.52, 0.18, 1, 1, 1.0)
        else
            RenderImage2(32344, PosX + 135, PosY + 240, 18.0, 13.0, 0, 0.0, 0.52, 0.18, 1, 1, 1.0)
        end
    end

    if TimerEventsMaxPages > 0
    then
        SetTextColor(255, 189, 25, 255)
        local stringPages = string.format('%d / %d', TimerEventsCurrentPage + 1, TimerEventsMaxPages + 1)
        RenderText3(PosX, PosY + 243, stringPages, 230, 3)
    end

    DisableAlphaBlend()
end

function TimerEvents.RenderText(x, y)
    SetFontType(1)
	
	SetTextColor(255, 189, 25, 255)
	
	SetTextBg(0, 0, 0, 0)
	
	RenderText3(x, y + 10, TIMER_EVENTS_TEXTS[GetLanguage()][1], 230, 3)

    local Language = string.lower(GetLanguage())
    local posY = 0.0

    if #TimerEventsInfo > 0
    then
        line = 0

        for i = 1, #TimerEventsInfo do
            if line >= TimerEventsStartLine and line < TimerEventsEndLine
            then
                SetTextColor(255, 255, 255, 255)

                local eventInfo = TIMER_EVENTS_INFO[i]

                if eventInfo ~= nil
                then
                    if Language == string.lower('Por')
                    then
                        RenderText3(x + 40, y + 50 + posY, string.format('%s', eventInfo.eventNamePor), 200, 1)
                    elseif Language == string.lower('Spn')
                    then
                        RenderText3(x + 40, y + 50 + posY, string.format('%s', eventInfo.eventNameSpn), 200, 1)
                    else
                        RenderText3(x + 40, y + 50 + posY, string.format('%s', eventInfo.eventNameEng), 200, 1)
                    end
                end

                if math.abs((os.clock() - TimerEventsInfo[i].tickDec)) > 1
                then
                    TimerEventsInfo[i].eventTick = TimerEventsInfo[i].eventTick - 1
                    TimerEventsInfo[i].tickDec = os.clock()
                end

                local timer_check = TimerEventsInfo[i].eventTick

                if (timer_check < -60)
                then
                    SetTextColor(225, 95, 45, 255)

                    RenderText3(x + 175, y + 50 + posY, string.format(TIMER_EVENTS_TEXTS[GetLanguage()][4]), 200, 7)
                elseif (timer_check > -60 and timer_check < 0)
                then
                    SetTextColor(0, 225, 139, 255)

                    RenderText3(x + 175, y + 50 + posY, string.format(TIMER_EVENTS_TEXTS[GetLanguage()][3]), 200, 7)
                else
                    if (timer_check / 3600) > 23
                    then
                        RenderText3(x + 175, y + 50 + posY, string.format(TIMER_EVENTS_TEXTS[GetLanguage()][2], (timer_check / 3600) / 24), 200, 7)
                    else
                        local h = string.format("0%d", math.floor(timer_check / 3600))
                        local m = string.format("0%d", math.floor((timer_check/60) % 60))
                        local s = string.format("0%d", math.floor(timer_check % 60))
                        h = string.sub(h, #h-1, #h)
                        m = string.sub(m, #m-1, #m)
                        s = string.sub(s, #s-1, #s)
                        RenderText3(x + 175, y + 50 + posY, string.format("%s:%s:%s", h, m, s), 200, 7)
                    end
                end

                posY = posY + 15
            end

            line = line + 1
        end
    end
end

function TimerEvents.ProcessTimerEvent(PacketName)
    TimerEventsInfo = {}

    local count = GetWordPacket(PacketName, -1)

    TimerEventsSystemTime = GetDwordPacket(PacketName, -1)

    for i = 1, count do
        TimerEventsInfo[i] = { eventTick = GetDwordPacket(PacketName, -1), tickDec = os.clock() }
    end

    TimerEvents.CalcPages()

    TimerEventsOpen = 1
end

function TimerEvents.CalcPages()
    TimerEventsStartLine = 0
    TimerEventsEndLine = 12
    TimerEventsMaxLines = #TimerEventsInfo
    TimerEventsMaxPages = math.floor(TimerEventsMaxLines / 12)

    local diffPage = math.floor(TimerEventsMaxPages * 12)

    if diffPage == TimerEventsMaxLines
    then
        TimerEventsMaxPages = TimerEventsMaxPages - 1
    end
end

function TimerEvents.ProcessNextPage(type)
    if type == 1
    then
        if TimerEventsCurrentPage < TimerEventsMaxPages
        then
            TimerEventsStartLine = TimerEventsStartLine + 12
            TimerEventsEndLine = TimerEventsEndLine + 12
            TimerEventsCurrentPage = TimerEventsCurrentPage + 1

            if TimerEventsEndLine > TimerEventsMaxLines
            then
                TimerEventsEndLine = TimerEventsMaxLines
            end
        end
    end

    if type == 2
    then
        if TimerEventsCurrentPage > 0
        then
            TimerEventsCurrentPage = TimerEventsCurrentPage - 1
            TimerEventsStartLine = TimerEventsStartLine - 12

            if (TimerEventsCurrentPage + 1) == TimerEventsMaxPages
            then
                local difLines = ((TimerEventsMaxPages+1) * 12)

                if difLines >= TimerEventsEndLine
                then
                    TimerEventsEndLine = TimerEventsEndLine - (12 - (difLines - TimerEventsEndLine))
                else
                    TimerEventsEndLine = TimerEventsEndLine - 12
                end
            else
                TimerEventsEndLine = TimerEventsEndLine - 12
            end

            if TimerEventsStartLine <= 0
            then
                TimerEventsStartLine = 0
            end
        end
    end
end

function TimerEvents.Protocol(Packet, PacketName)
    if Packet ~= TIMER_EVENTS_PACKET
    then
        return
    end

    if PacketName == string.format("%s_%s", TIMER_EVENTS_PACKET_NAME, UserGetName())
    then
        TimerEvents.ProcessTimerEvent(PacketName)
        ClearPacket(PacketName)
    end
end

function TimerEvents.Open()
    if TimerEventsOpen == 1
    then
        return
    end

    local packetString = string.format("%s_%s", TIMER_EVENTS_PACKET_NAME, UserGetName())
    CreatePacket(packetString, TIMER_EVENTS_PACKET)
    SendPacket(packetString)
    ClearPacket(packetString)
end

function TimerEvents.Close()
    TimerEventsOpen = 0
    TimerEventsInfo = {}
end

function TimerEvents.UpdateKeyEvent()
    if TimerEventsOpen == 0
    then
        return
    end

    if CheckPressedKey(Keys.Escape) == 1
    then
        TimerEvents.Close()
    end

    if CheckWindowOpen(UIChatWindow) == 1
        or CheckWindowOpen(UICashShop) == 1
        or CheckWindowOpen(UIFullMap) == 1
        or CheckWindowOpen(UIFriendList) == 1
        or CheckWindowOpen(UINPC_Dialog) == 1
        or CheckWindowOpen(UIStore) == 1
        or CheckWindowOpen(UITrade) == 1
        or CheckWindowOpen(UIChaosBox) == 1
        or CheckWindowOpen(UIWarehouse) == 1
    then
        TimerEvents.Close()
    end
end

function TimerEvents.UpdateMouse()
    if TimerEventsOpen == 0
    then
        return
    end

    local PosX = (640 / 2) - (230 / 2) + (1*GetWideX())
    local PosY = 80

    local MouseX = MousePosX()
    local MouseY = MousePosY()

    if (MouseX >= (PosX + 230 - 25) and MouseX <= (PosX + 230 - 25) + 18)
        and (MouseY >= (PosY + 5) and MouseY <= (PosY + 5) + 18)
    then
        if (CheckClickClient() == 1)
        then
            DisableClickClient()
            TimerEvents.Close()
        end
    end

    if TimerEventsMaxPages > 0
    then
        --left
        if (MouseX >= (PosX + 70) and MouseX <= (PosX + 70) + 25)
            and (MouseY >= (PosY + 240) and MouseY <= (PosY + 240) + 16)
        then
            if (CheckClickClient() == 1)
            then
                DisableClickClient()
                TimerEvents.ProcessNextPage(2)
            end
        end

        --right
        if (MouseX >= (PosX + 135) and MouseX <= (PosX + 135) + 25)
            and (MouseY >= (PosY + 240) and MouseY <= (PosY + 240) + 16)
        then
            if (CheckClickClient() == 1)
            then
                DisableClickClient()
                TimerEvents.ProcessNextPage(1)
            end
        end
    end

    if MouseX >= PosX and MouseX <= PosX + 230
        and MouseY >= PosY and MouseY <= PosY + 257
    then
        DisableClickClient()
    end
end

function TimerEvents.MainProcWorldKey(key)
    if key == TIMER_EVENTS_KEY_OPEN
    then
        if	CheckWindowOpen(UIChatWindow)		== 1	then	return	end
        if	CheckWindowOpen(UIInventory) 		== 1	then	return	end
		if	CheckWindowOpen(UIFriendList) 		== 1	then	return	end
		if	CheckWindowOpen(UIMoveList)			== 1	then	return	end
		if	CheckWindowOpen(UIParty) 			== 1	then	return	end
		if	CheckWindowOpen(UIQuest) 			== 1	then	return	end
		if	CheckWindowOpen(UIGuild) 			== 1	then	return	end
		if	CheckWindowOpen(UITrade) 			== 1	then	return	end
		if	CheckWindowOpen(UIWarehouse) 		== 1	then	return	end
		if	CheckWindowOpen(UIChaosBox)			== 1	then	return	end
		if	CheckWindowOpen(UICommandWindow) 	== 1	then	return	end
		if	CheckWindowOpen(UIPetInfo)	 		== 1	then	return	end
		if	CheckWindowOpen(UIShop)				== 1	then	return	end
		if	CheckWindowOpen(UIStore) 			== 1	then	return	end
		if	CheckWindowOpen(UIOtherStore) 		== 1	then	return	end
		if	CheckWindowOpen(UICharacter) 		== 1	then	return	end
		if	CheckWindowOpen(UIOptions) 			== 1	then	return	end
		if	CheckWindowOpen(UIHelp)				== 1	then	return	end
		if	CheckWindowOpen(UIFastDial)			== 1	then	return	end
		if	CheckWindowOpen(UISkillTree) 		== 1	then	return	end
		if	CheckWindowOpen(UINPC_Titus) 		== 1	then	return	end
		if	CheckWindowOpen(UICashShop)			== 1	then	return	end
		if	CheckWindowOpen(UIFullMap) 			== 1	then	return	end
		if	CheckWindowOpen(UINPC_Dialog)		== 1	then	return	end
		if	CheckWindowOpen(UIGensInfo)			== 1	then	return	end
		if	CheckWindowOpen(UINPC_Julia)		== 1	then	return	end

        TimerEvents.Open()
    end
end

function TimerEvents.Init()
    if TIMER_EVENTS_SWITCH ~= 1
    then
        return
    end
    
    InterfaceController.MainProc(TimerEvents.MainProc)
    InterfaceController.ClientProtocol(TimerEvents.Protocol)
    InterfaceController.UpdateMouse(TimerEvents.UpdateMouse)
    InterfaceController.UpdateKey(TimerEvents.UpdateKeyEvent)
    InterfaceController.MainProcWorldKey(TimerEvents.MainProcWorldKey)
end

TimerEvents.Init()

return TimerEvents