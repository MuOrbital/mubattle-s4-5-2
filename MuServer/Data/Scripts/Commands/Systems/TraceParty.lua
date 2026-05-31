local TraceParty = {}

function TraceParty.TraceCommand(aIndex, Arguments)
    if PT_TRACE_SWITCH == 0 then
        return
    end

    local player = User.new(aIndex)
    local Language = player:getLanguage()

    if player:getVip() < PT_TRACE_TRACE_VIP then
        SendMessage(PT_TRACE_MESSAGES[Language][2], aIndex, 1)
        return
    end

    local partyNumber = player:getPartyNumber()
    if IsParty(partyNumber) == 0 then
        SendMessage(PT_TRACE_MESSAGES[Language][5], aIndex, 1)
        return
    end

    if IsLeaderParty(partyNumber, aIndex) == 0 then
        SendMessage(PT_TRACE_MESSAGES[Language][1], aIndex, 1)
        return
    end

    local leaderMap = player:getMapNumber()

    if NO_TRACE_MAPS[leaderMap] == false then
        SendMessage(PT_TRACE_MESSAGES[Language][8], aIndex, 1)
        return
    end

    SendMessage(PT_TRACE_MESSAGES[Language][3], aIndex, 1)

    local leaderX = player:getX()
    local leaderY = player:getY()

    for i = 0, GetMemberCountParty(partyNumber) - 1 do
        local memberIndex = GetMemberIndexParty(partyNumber, i)
        if memberIndex ~= aIndex and gObjIsConnectedGP(memberIndex) == 1 then
            Teleport(memberIndex, leaderMap, leaderX, leaderY)
        end
    end

    player = nil
end

function TraceParty.TrackCommand(aIndex, Arguments)
    if PT_TRACE_SWITCH == 0 then
        return
    end

    local player = User.new(aIndex)
    local Language = player:getLanguage()

    if player:getVip() < PT_TRACE_TRACK_VIP then
        SendMessage(PT_TRACE_MESSAGES[Language][2], aIndex, 1)
        return
    end

    local partyNumber = player:getPartyNumber()
    if IsParty(partyNumber) == 0 then
        SendMessage(PT_TRACE_MESSAGES[Language][5], aIndex, 1)
        return
    end

    if IsLeaderParty(partyNumber, aIndex) == 1 then
        SendMessage(PT_TRACE_MESSAGES[Language][6], aIndex, 1)
        return
    end

    for i = 0, GetMemberCountParty(partyNumber) - 1 do
        local memberIndex = GetMemberIndexParty(partyNumber, i)
        if IsLeaderParty(partyNumber, memberIndex) == 1 and gObjIsConnectedGP(memberIndex) == 1 then
            local leader = User.new(memberIndex)
            local leaderMap = leader:getMapNumber()

            if NO_TRACE_MAPS[leaderMap] == false then
                SendMessage(PT_TRACE_MESSAGES[Language][7], aIndex, 1)
                leader = nil
                return
            end

            local leaderX = leader:getX()
            local leaderY = leader:getY()

            SendMessage(PT_TRACE_MESSAGES[Language][4], aIndex, 1)
            Teleport(aIndex, leaderMap, leaderX, leaderY)

            leader = nil
            break
        end
    end

    player = nil
end

Commands.Register(PT_TRACE_COMMAND_TRACE, TraceParty.TraceCommand)
Commands.Register(PT_TRACE_COMMAND_TRACK, TraceParty.TrackCommand)

return TraceParty