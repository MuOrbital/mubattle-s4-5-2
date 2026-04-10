TimerEvents = {}

function TimerEvents.OpenEventWindow(player)
    if player:getInterfaceUse() ~= 0 or player:getInterfaceType() ~= 0 or player:getState() == 32 or player:getDieRegen() ~= 0 or player:getTeleport() ~= 0
	then
		return
	end

    if TIMER_EVENTS_COUNT <= 0
    then
        return
    end

    local packetIdentification = string.format("%s_%s",TIMER_EVENTS_PACKET_NAME, player:getName())

    CreatePacket(packetIdentification, TIMER_EVENTS_PACKET)

    SetWordPacket(packetIdentification, TIMER_EVENTS_COUNT)

    SetDwordPacket(packetIdentification, GetTick())

    for i = 1, TIMER_EVENTS_COUNT do
        local eventInfo = TIMER_EVENTS_INFO[i]

        timer = TimerEvents.GetNextEventTime(eventInfo)

        SetDwordPacket(packetIdentification, timer)
    end

    SendPacket(packetIdentification, player:getIndex())
    ClearPacket(packetIdentification)
end

function TimerEvents.Protocol(aIndex, Packet, PacketName)
    if Packet == TIMER_EVENTS_PACKET
    then
        local player = User.new(aIndex)

        if string.format("%s_%s",TIMER_EVENTS_PACKET_NAME, player:getName()) == PacketName
        then
            ClearPacket(PacketName)
            TimerEvents.OpenEventWindow(player)
        end

        player = nil
    end
end

function TimerEvents.GetNextEventTime(eventInfo)
    local timer = 0

    for n = 1, #eventInfo do
        local eventTime = eventInfo[n]

        if eventTime.tickTime >= 0
        then
            if timer == 0
            then
                timer = eventTime.tickTime
            else
                if timer > eventTime.tickTime
                then
                    timer = eventTime.tickTime
                end
            end
        end
    end

    return timer
end

function TimerEvents.CreateTimerTick()
    for i = 1, TIMER_EVENTS_COUNT do
        local eventInfo = TIMER_EVENTS_INFO[i]

        for n = 1, #eventInfo do
            local eventTime = eventInfo[n]

            eventTime.tickTime = GetTimeDays(eventTime.dayOfWeek, eventTime.day, eventTime.hour, eventTime.minute, eventTime.second)
        end
    end
end

function TimerEvents.MainProc()
    for i = 1, TIMER_EVENTS_COUNT do
        local eventInfo = TIMER_EVENTS_INFO[i]

        for n = 1, #eventInfo do
            local eventTime = eventInfo[n]

            eventTime.tickTime = eventTime.tickTime - 1

            if (eventTime.tickTime < 0)
            then
                eventTime.tickTime = GetTimeDays(eventTime.dayOfWeek, eventTime.day, eventTime.hour, eventTime.minute, eventTime.second)
            end
        end
    end
end

function TimerEvents.Init()
    if TIMER_EVENTS_SWITCH == 0
    then
        return
    end

    GameServerFunctions.GameServerProtocol(TimerEvents.Protocol)
    TimerEvents.CreateTimerTick()
    Timer.Interval(1, TimerEvents.MainProc)
end

TimerEvents.Init()

return TimerEvents