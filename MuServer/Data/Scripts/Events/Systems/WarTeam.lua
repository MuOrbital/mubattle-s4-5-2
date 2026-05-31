-- ================== ESTE EVENTO AINDA NÃO ESTÁ EM FUNCIONAMENTO ==================

WarTeam = {}
WarTeam.Players = {}
WarTeam.TeamBlue = {}
WarTeam.TeamRed = {}
WarTeam.PointsBlue = 0
WarTeam.PointsRed = 0
WarTeam.Active = false
WarTeam.RegActive = false
WarTeam.RegNpc = -1
WarTeam.BaseA = -1
WarTeam.BaseB = -1
WarTeam.BaseOwnerA = "Blue"
WarTeam.BaseOwnerB = "Red"
WarTeam.BaseClickers = { Blue = {}, Red = {} }
WarTeam.BaseCaptureStart = { Blue = 0, Red = 0 }
WarTeam.HoldTimerRunning = false
local function GetLang(aIndex)
    return "Por"
end
local function MsgGlobal(id, ...)
    local msg = WarTeam_Messages["Por"][id]
    if msg then
        SendMessageGlobal(string.format(msg, ...), 0)
    end
end
local function Msg(aIndex, id, ...)
    local lang = GetLang(aIndex)
    local msg = WarTeam_Messages[lang][id]
    if msg then
        SendMessage(string.format(msg, ...), aIndex, 1)
    end
end
function WarTeam.GetBaseLetterByNpc(npc)
    if npc == WarTeam.BaseA then return "Blue" end
    if npc == WarTeam.BaseB then return "Red" end
    return nil
end
function WarTeam.GetBaseOwner(baseLetter)
    if baseLetter == "Blue" then return WarTeam.BaseOwnerA end
    return WarTeam.BaseOwnerB
end
function WarTeam.SetBaseOwner(baseLetter, owner)
    if baseLetter == "Blue" then
        WarTeam.BaseOwnerA = owner
    else
        WarTeam.BaseOwnerB = owner
    end
    WarTeam.SendBaseState()
end

-- ==================== NOVA FUNÇÃO: TROCA DE NPC DA BASE ====================
function WarTeam.ChangeBaseNPC(baseLetter, newOwner)
    local baseNpc = (baseLetter == "Blue") and WarTeam.BaseA or WarTeam.BaseB
    if baseNpc == -1 then return end

    local map, x, y = WarTeam.GetBaseCoords(baseLetter)
    local newNpcClass = (newOwner == "Blue") and WarTeam_Config.BaseNpcA or WarTeam_Config.BaseNpcB
    local dir = (baseLetter == "Blue") and WarTeam_Config.BaseDirA or WarTeam_Config.BaseDirB

    -- Remove NPC antigo
    gObjDel(baseNpc)

    -- Cria novo NPC com a classe do dono atual
    local idx = AddMonster(map)
    if idx == -1 then return end

    SetMapMonster(idx, map, x, y)
    local npc = User.new(idx)
    npc:setDir(dir)
    SetMonster(idx, newNpcClass)
    npc:setType(3)

    -- Atualiza a variável da base
    if baseLetter == "Blue" then
        WarTeam.BaseA = idx
    else
        WarTeam.BaseB = idx
    end
end
-- =====================================================================

function WarTeam.GetPlayerTeam(aIndex)
    if not WarTeam.Players[aIndex] then return nil end
    return WarTeam.Players[aIndex].team
end
function WarTeam.GetBaseCoords(baseLetter)
    if baseLetter == "Blue" then
        return WarTeam_Config.BaseMapA, WarTeam_Config.BaseXa, WarTeam_Config.BaseYa
    end
    return WarTeam_Config.BaseMapB, WarTeam_Config.BaseXb, WarTeam_Config.BaseYb
end
function WarTeam.SendUpdateLists()
    CreatePacket("AC_LIST_UPDATE", WarTeam_Config.PacketUpdateLists)
    local list = {}
    for _, data in pairs(WarTeam.Players) do
        table.insert(list, data.name)
    end
    SetBytePacket("AC_LIST_UPDATE", #list)
    for i = 1, #list do
        SetCharPacketLength("AC_LIST_UPDATE", list[i], 11)
    end
    for aIndex in pairs(WarTeam.Players) do
        SendPacket("AC_LIST_UPDATE", aIndex)
    end
    ClearPacket("AC_LIST_UPDATE")
end
function WarTeam.SendScoreUpdate()
    if not WarTeam.Active then return end
    CreatePacket("AC_SCORE_UPDATE", WarTeam_Config.PacketScoreUpdate)
    SetDwordPacket("AC_SCORE_UPDATE", WarTeam.PointsBlue)
    SetDwordPacket("AC_SCORE_UPDATE", WarTeam.PointsRed)
    for aIndex in pairs(WarTeam.Players) do
        SendPacket("AC_SCORE_UPDATE", aIndex)
    end
    ClearPacket("AC_SCORE_UPDATE")
end
function WarTeam.SendBaseState()
    CreatePacket("AC_BASE_STATE", WarTeam_Config.PacketCaptureState)
    SetBytePacket("AC_BASE_STATE", WarTeam.BaseOwnerA == "Blue" and 1 or 2)
    SetBytePacket("AC_BASE_STATE", WarTeam.BaseOwnerB == "Blue" and 1 or 2)
    for aIndex in pairs(WarTeam.Players) do
        SendPacket("AC_BASE_STATE", aIndex)
    end
    ClearPacket("AC_BASE_STATE")
end
function WarTeam.SendCaptureBar(capturingBase, targetBase, startMode)
    CreatePacket("AC_CAPTURE_BAR", WarTeam_Config.PacketCaptureBar)
    SetBytePacket("AC_CAPTURE_BAR", startMode and 1 or 0)
    SetBytePacket("AC_CAPTURE_BAR", capturingBase == "Blue" and 1 or 2)
    SetBytePacket("AC_CAPTURE_BAR", targetBase == "Blue" and 1 or 2)
    SetDwordPacket("AC_CAPTURE_BAR", WarTeam_Config.CaptureTime)
  
    for aIndex in pairs(WarTeam.Players) do
        SendPacket("AC_CAPTURE_BAR", aIndex)
    end
    ClearPacket("AC_CAPTURE_BAR")
end
function WarTeam.CancelCapturePacket(team)
    CreatePacket("AC_CAPTURE_BAR_CANCEL", WarTeam_Config.PacketCaptureBar)
    SetBytePacket("AC_CAPTURE_BAR_CANCEL", 0) -- inactive
    if team == "Blue" then
        SetBytePacket("AC_CAPTURE_BAR_CANCEL", 1)
    elseif team == "Red" then
        SetBytePacket("AC_CAPTURE_BAR_CANCEL", 2)
    else
        SetBytePacket("AC_CAPTURE_BAR_CANCEL", 0)
    end
    SetBytePacket("AC_CAPTURE_BAR_CANCEL", 0)
    SetDwordPacket("AC_CAPTURE_BAR_CANCEL", 0)
    for aIndex in pairs(WarTeam.Players) do
        SendPacket("AC_CAPTURE_BAR_CANCEL", aIndex)
    end
    ClearPacket("AC_CAPTURE_BAR_CANCEL")
end
function WarTeam.IsAccountAlreadyRegistered(accountID)
    local query = "SELECT COUNT(*) as Total FROM WarTeam WHERE AccountID = '" .. accountID .. "'"
    local ret = db:exec(query)
    if ret == 0 then db:clear() return false end
    db:fetch()
    local count = db:getInt("Total")
    db:clear()
    return count > 0
end
function WarTeam.IsCharacterAlreadyRegistered(name)
    local query = "SELECT COUNT(*) as Total FROM WarTeam WHERE Name = '" .. name .. "'"
    local ret = db:exec(query)
    if ret == 0 then db:clear() return false end
    db:fetch()
    local count = db:getInt("Total")
    db:clear()
    return count > 0
end
function WarTeam.InsertPlayerToDB(accountID, name)
    local query = string.format("INSERT INTO WarTeam (AccountID, Name, Team) VALUES ('%s', '%s', 0)", accountID, name)
    db:exec(query)
    db:clear()
end
function WarTeam.UpdatePlayerTeamInDB(name, team)
    local teamNum = (team == "Blue") and 1 or 2
    local query = string.format("UPDATE WarTeam SET Team = %d WHERE Name = '%s'", teamNum, name)
    db:exec(query)
    db:clear()
end
function WarTeam.ClearDatabase()
    local query = "DELETE FROM WarTeam"
    db:exec(query)
    db:clear()
end
function WarTeam.ReconnectPlayer(aIndex)
    if not WarTeam.Active and not WarTeam.RegActive then return end
    local player = User.new(aIndex)
    local name = player:getName()
    local query = "SELECT Team FROM WarTeam WHERE Name = '" .. name .. "'"
    local ret = db:exec(query)
    if ret == 0 then
        db:clear()
        return
    end
    db:fetch()
    local team = db:getInt("Team")
    db:clear()
    if team == 0 then return end
    local teamName = (team == 1) and "Blue" or "Red"
    WarTeam.Players[aIndex] = { team = teamName, name = name }
    if teamName == "Blue" then
        table.insert(WarTeam.TeamBlue, aIndex)
    else
        table.insert(WarTeam.TeamRed, aIndex)
    end

    if WarTeam.Active then
        if teamName == "Blue" then
            if WarTeam.BaseOwnerB == "Blue" then
                local map, x, y = WarTeam.GetBaseCoords("Red")
                Teleport(aIndex, map, x, y)
            elseif WarTeam.BaseOwnerA == "Blue" then
                local map, x, y = WarTeam.GetBaseCoords("Blue")
                Teleport(aIndex, map, x, y)
            else
                Teleport(aIndex,
                    WarTeam_Config.DefeatedTeleportMap,
                    WarTeam_Config.DefeatedTeleportX,
                    WarTeam_Config.DefeatedTeleportY
                )
            end
        else
            if WarTeam.BaseOwnerA == "Red" then
                local map, x, y = WarTeam.GetBaseCoords("Blue")
                Teleport(aIndex, map, x, y)
            elseif WarTeam.BaseOwnerB == "Red" then
                local map, x, y = WarTeam.GetBaseCoords("Red")
                Teleport(aIndex, map, x, y)
            else
                Teleport(aIndex,
                    WarTeam_Config.DefeatedTeleportMap,
                    WarTeam_Config.DefeatedTeleportX,
                    WarTeam_Config.DefeatedTeleportY
                )
            end
        end
        CreatePacket("AC_START", WarTeam_Config.PacketStartEvent)
        SendPacket("AC_START", aIndex)
        ClearPacket("AC_START")
        WarTeam.SendScoreUpdate()
        WarTeam.SendBaseState()
    end
    WarTeam.SendUpdateLists()
end
function WarTeam.CriarNpcFixo()
    if WarTeam.RegNpc ~= -1 then
        gObjDel(WarTeam.RegNpc)
        WarTeam.RegNpc = -1
    end
    local index = AddMonster(WarTeam_Config.RegisterMap)
    if index == -1 then return end
    SetMapMonster(index, WarTeam_Config.RegisterMap, WarTeam_Config.RegisterX, WarTeam_Config.RegisterY)
    local npc = User.new(index)
    npc:setDir(WarTeam_Config.RegisterDir)
    SetMonster(index, WarTeam_Config.RegisterNpcClass)
    npc:setType(3)
    WarTeam.RegNpc = index
end
function WarTeam.RegisterNpcTalk(npc, aIndex)
    if npc ~= WarTeam.RegNpc or not WarTeam.RegActive then return 0 end
    CreatePacket("AC_LIST_OPEN", WarTeam_Config.PacketUpdateLists)
    local list = {}
    for _, data in pairs(WarTeam.Players) do
        table.insert(list, data.name)
    end
    SetBytePacket("AC_LIST_OPEN", #list)
    for i = 1, #list do
        SetCharPacketLength("AC_LIST_OPEN", list[i], 11)
    end
    SendPacket("AC_LIST_OPEN", aIndex)
    ClearPacket("AC_LIST_OPEN")
    return 1
end
function WarTeam.RegisterPlayer(aIndex, packet, packetName)
    if packetName ~= "AC_REGISTER" and packetName ~= nil then return 0 end
    if not WarTeam.RegActive then return 0 end
    local player = User.new(aIndex)
    local total = 0
    for _ in pairs(WarTeam.Players) do total = total + 1 end
    if total >= WarTeam_Config.MaxPlayers then
        Msg(aIndex, 12) return 1
    end
    if WarTeam.Players[aIndex] then
        Msg(aIndex, 8) return 1
    end
    local rawName = player:getName()
    local name = tostring(rawName or "")
    name = string.gsub(name, "%z", "")
    name = string.gsub(name, "[^%w]", "")
    name = string.sub(name, 1, 10)
    if WarTeam.IsAccountAlreadyRegistered(player:getAccountID()) then
        Msg(aIndex, 11) return 1
    end
    if WarTeam.IsCharacterAlreadyRegistered(name) then
        Msg(aIndex, 10) return 1
    end
    WarTeam.Players[aIndex] = { team = 0, name = name }
    WarTeam.InsertPlayerToDB(player:getAccountID(), name)
    Msg(aIndex, 6)
    WarTeam.SendUpdateLists()
    return 1
end
function WarTeam.DivideTeams()
    local participants = {}
    for aIndex, data in pairs(WarTeam.Players) do
        table.insert(participants, {index = aIndex, name = data.name})
    end
    local total = #participants
    if total == 0 then return end
    for i = total, 2, -1 do
        local j = math.random(1, i)
        participants[i], participants[j] = participants[j], participants[i]
    end
    local half = math.floor(total / 2)
    WarTeam.TeamBlue = {}
    WarTeam.TeamRed = {}
    for i = 1, half do
        local entry = participants[i]
        local idx = entry.index
        WarTeam.Players[idx].team = "Blue"
        table.insert(WarTeam.TeamBlue, idx)
        WarTeam.UpdatePlayerTeamInDB(entry.name, "Blue")
    end
    for i = half + 1, total do
        local entry = participants[i]
        local idx = entry.index
        WarTeam.Players[idx].team = "Red"
        table.insert(WarTeam.TeamRed, idx)
        WarTeam.UpdatePlayerTeamInDB(entry.name, "Red")
    end
    MsgGlobal(30, #WarTeam.TeamBlue, #WarTeam.TeamRed)
end
function WarTeam.StartRegistration()
    if WarTeam.RegActive or WarTeam.Active then return end
    WarTeam.RegActive = true
    WarTeam.Active = false
    WarTeam.Players = {}
    WarTeam.TeamBlue = {}
    WarTeam.TeamRed = {}
    WarTeam.PointsBlue = 0
    WarTeam.PointsRed = 0
    WarTeam.BaseOwnerA = "Blue"
    WarTeam.BaseOwnerB = "Red"
    WarTeam.BaseClickers = { Blue = {}, Red = {} }
    WarTeam.BaseCaptureStart = { Blue = 0, Red = 0 }
    WarTeam.CriarNpcFixo()
    MsgGlobal(1, "Atlans")
    Timer.TimeOut(WarTeam_Config.RegisterDuration * 60, WarTeam.EndRegistration)
end
function WarTeam.EndRegistration()
    if not WarTeam.RegActive then return end
    WarTeam.RegActive = false
    if WarTeam.RegNpc ~= -1 then
        gObjDel(WarTeam.RegNpc)
        WarTeam.RegNpc = -1
    end
    local total = 0
    for _ in pairs(WarTeam.Players) do total = total + 1 end
    if total < WarTeam_Config.MinPlayers then
        MsgGlobal(9, total, WarTeam_Config.MinPlayers)
        WarTeam.Players = {}
        return
    end
    MsgGlobal(2)
    Timer.TimeOut(5, function()
        WarTeam.DivideTeams()
        Timer.TimeOut(1, WarTeam.StartEvent)
    end)
end
function WarTeam.CreateBaseNPC(baseLetter)
    local map, x, y = WarTeam.GetBaseCoords(baseLetter)
    local npcClass = baseLetter == "Blue" and WarTeam_Config.BaseNpcA or WarTeam_Config.BaseNpcB
    local dir = baseLetter == "Blue" and WarTeam_Config.BaseDirA or WarTeam_Config.BaseDirB
    local idx = AddMonster(map)
    if idx == -1 then return -1 end
    SetMapMonster(idx, map, x, y)
    local npc = User.new(idx)
    npc:setDir(dir)
    SetMonster(idx, npcClass)
    npc:setType(3)
    return idx
end
function WarTeam.StartEvent()
    WarTeam.Active = true
    WarTeam.RegActive = false
    WarTeam.PointsBlue = 0
    WarTeam.PointsRed = 0
    WarTeam.BaseOwnerA = "Blue"
    WarTeam.BaseOwnerB = "Red"
    WarTeam.BaseClickers = { Blue = {}, Red = {} }
    WarTeam.BaseCaptureStart = { Blue = 0, Red = 0 }
    WarTeam.BaseA = WarTeam.CreateBaseNPC("Blue")
    WarTeam.BaseB = WarTeam.CreateBaseNPC("Red")
    for _, idx in ipairs(WarTeam.TeamBlue) do
        if WarTeam.Players[idx] then Teleport(idx, WarTeam_Config.MapA, WarTeam_Config.Xa, WarTeam_Config.Ya) end
    end
    for _, idx in ipairs(WarTeam.TeamRed) do
        if WarTeam.Players[idx] then Teleport(idx, WarTeam_Config.MapB, WarTeam_Config.Xb, WarTeam_Config.Yb) end
    end
    CreatePacket("AC_START", WarTeam_Config.PacketStartEvent)
    for aIndex in pairs(WarTeam.Players) do
        SendPacket("AC_START", aIndex)
    end
    ClearPacket("AC_START")
    MsgGlobal(4)
    WarTeam.SendScoreUpdate()
    WarTeam.SendBaseState()
    WarTeam.CheckCaptureProgress()
    WarTeam.StartHoldPointsTimer()
    Timer.TimeOut(WarTeam_Config.Duration * 60, WarTeam.EndEvent)
end
function WarTeam.StartHoldPointsTimer()
    WarTeam.HoldTimerRunning = true
    local function Tick()
        if not WarTeam.Active then
            WarTeam.HoldTimerRunning = false
            return
        end
        local added = false
        if WarTeam.BaseOwnerB == "Blue" then
            WarTeam.PointsBlue = WarTeam.PointsBlue + WarTeam_Config.HoldPoints
            if WarTeam_Config.ShowHoldPointsMessage then MsgGlobal(19, WarTeam_Config.HoldPoints, "Blue") end
            added = true
        end
        if WarTeam.BaseOwnerA == "Red" then
            WarTeam.PointsRed = WarTeam.PointsRed + WarTeam_Config.HoldPoints
            if WarTeam_Config.ShowHoldPointsMessage then MsgGlobal(19, WarTeam_Config.HoldPoints, "Red") end
            added = true
        end
        if added then WarTeam.SendScoreUpdate() end
        Timer.TimeOut(WarTeam_Config.HoldPointsInterval, Tick)
    end
    Timer.TimeOut(WarTeam_Config.HoldPointsInterval, Tick)
end
function WarTeam.EndEvent()
    if not WarTeam.Active then return end
    WarTeam.Active = false
    WarTeam.BaseClickers = { Blue = {}, Red = {} }
    WarTeam.BaseCaptureStart = { Blue = 0, Red = 0 }
    local winner = "Blue"
    local pts = WarTeam.PointsBlue
    if WarTeam.PointsRed > WarTeam.PointsBlue then
        winner = "Red"
        pts = WarTeam.PointsRed
    end
    MsgGlobal(5, winner, pts)
    MsgGlobal(7, winner)
    CreatePacket("AC_END", WarTeam_Config.PacketEventEnd)
    SetCharPacket("AC_END", winner)
    SetDwordPacket("AC_END", pts)
    for aIndex in pairs(WarTeam.Players) do
        SendPacket("AC_END", aIndex)
        Teleport(aIndex, WarTeam_Config.ExitMap, WarTeam_Config.ExitX, WarTeam_Config.ExitY)
    end
    ClearPacket("AC_END")
    if WarTeam.BaseA ~= -1 then gObjDel(WarTeam.BaseA) WarTeam.BaseA = -1 end
    if WarTeam.BaseB ~= -1 then gObjDel(WarTeam.BaseB) WarTeam.BaseB = -1 end
    WarTeam.ClearDatabase()
   
    WarTeam.Players = {}
    WarTeam.TeamBlue = {}
    WarTeam.TeamRed = {}
    WarTeam.PointsBlue = 0
    WarTeam.PointsRed = 0
    WarTeam.BaseOwnerA = "Blue"
    WarTeam.BaseOwnerB = "Red"
end
function WarTeam.GMCommand(aIndex)
    local player = User.new(aIndex)
    if CheckGameMasterLevel(player:getAccountID(), player:getName(), WarTeam_Config.GMLevel) == 0 then return end
    if WarTeam.Active or WarTeam.RegActive then
        Msg(aIndex, 14)
        return
    end
    WarTeam.StartRegistration()
end
function WarTeam.BlockAlliedAttack(aIndex, TargetIndex)
    local ta = WarTeam.GetPlayerTeam(aIndex)
    local tb = WarTeam.GetPlayerTeam(TargetIndex)
    if ta and tb and ta == tb then
        Msg(aIndex, 15)
        return 1
    end
    return 0
end
function WarTeam.CanInteractWithBase(aIndex, baseLetter)
    local playerTeam = WarTeam.GetPlayerTeam(aIndex)
    if not playerTeam then Msg(aIndex, 13) return false end
    local owner = WarTeam.GetBaseOwner(baseLetter)
    if baseLetter == playerTeam then
        if owner == playerTeam then Msg(aIndex, 17) return false end
        return true
    end
    if WarTeam.GetBaseOwner(playerTeam) ~= playerTeam then Msg(aIndex, 16) return false end
    if owner == playerTeam then Msg(aIndex, 18) return false end
    return true
end
function WarTeam.BaseNpcTalk(npc, aIndex)
    if not WarTeam.Active then return 0 end
    if not WarTeam.Players[aIndex] then return 0 end
    local baseLetter = WarTeam.GetBaseLetterByNpc(npc)
    if not baseLetter then return 0 end
    if not WarTeam.CanInteractWithBase(aIndex, baseLetter) then return 1 end
    local playerTeam = WarTeam.GetPlayerTeam(aIndex)
    WarTeam.BaseClickers[baseLetter][aIndex] = true
    if WarTeam.BaseCaptureStart[baseLetter] == 0 then
        WarTeam.BaseCaptureStart[baseLetter] = os.time()
        if playerTeam == baseLetter then
            MsgGlobal(29, playerTeam)
        else
            MsgGlobal(20, playerTeam, baseLetter)
        end
        WarTeam.SendCaptureBar(playerTeam, baseLetter, true)
    end
    return 1
end
function WarTeam.CancelCaptureForTeam(baseLetter)
    if WarTeam.BaseCaptureStart[baseLetter] == 0 then
        return
    end
    local attacker = nil
    for aIndex, _ in pairs(WarTeam.BaseClickers[baseLetter]) do
        attacker = WarTeam.GetPlayerTeam(aIndex)
        break
    end
    if not attacker then
        attacker = (baseLetter == "Blue") and "Red" or "Blue"
    end
    WarTeam.BaseClickers[baseLetter] = {}
    WarTeam.BaseCaptureStart[baseLetter] = 0
    WarTeam.CancelCapturePacket(attacker)
end
function WarTeam.CancelOtherCapture(completedBaseLetter)
    local otherBase = (completedBaseLetter == "Blue") and "Red" or "Blue"
    if WarTeam.BaseCaptureStart[otherBase] > 0 then
        WarTeam.CancelCaptureForTeam(otherBase)
    end
end
function WarTeam.FinishCapture(baseLetter)
    local attacker = nil
    for aIndex, _ in pairs(WarTeam.BaseClickers[baseLetter]) do
        attacker = WarTeam.GetPlayerTeam(aIndex)
        break
    end
    if not attacker then return end
    local ownerBefore = WarTeam.GetBaseOwner(baseLetter)
    local isRecovery = (baseLetter == attacker and ownerBefore ~= attacker)

    WarTeam.CancelCaptureForTeam(baseLetter)
    WarTeam.CancelOtherCapture(baseLetter)

    -- ====================== MODIFICAÇÃO SOLICITADA ======================
    -- Troca o NPC da base imediatamente quando o dono muda
    WarTeam.SetBaseOwner(baseLetter, attacker)
    WarTeam.ChangeBaseNPC(baseLetter, attacker)
    -- ====================================================================

    if isRecovery then
        MsgGlobal(27, baseLetter, attacker)
        WarTeam.TeleportTeamToBase(attacker, attacker)
        local enemyTeam = attacker == "Blue" and "Red" or "Blue"
        WarTeam.TeleportTeamToBase(enemyTeam, enemyTeam)
    else
        MsgGlobal(22, attacker, baseLetter)
        local defeatedTeam = baseLetter
        local list = defeatedTeam == "Blue" and WarTeam.TeamBlue or WarTeam.TeamRed
        for _, idx in ipairs(list) do
            if WarTeam.Players[idx] then Msg(idx, 23, attacker) end
        end
        WarTeam.TeleportDefeatedTeam(defeatedTeam)
        WarTeam.TeleportTeamToBase(attacker, baseLetter)
    end
    WarTeam.SendBaseState()
end

function WarTeam.CheckCaptureProgress()
    if not WarTeam.Active then 
        return 
    end

    for baseLetter, clickers in pairs(WarTeam.BaseClickers) do
        local numClickers = 0
        for _ in pairs(clickers) do 
            numClickers = numClickers + 1 
        end

        if numClickers > 0 then
            local startTime = WarTeam.BaseCaptureStart[baseLetter]
            if startTime > 0 then
                local elapsed = os.time() - startTime
                local speed = math.max(1, numClickers)
                local requiredTime = math.floor(WarTeam_Config.CaptureTime / speed)

                -- Verificação extra: garante que pelo menos 1 jogador ainda está dentro do range circular
                local hasPlayerInRange = false
                for clickerIndex, _ in pairs(clickers) do
                    local p = User.new(clickerIndex)
                    local baseNpc = (baseLetter == "Blue") and WarTeam.BaseA or WarTeam.BaseB
                    local b = User.new(baseNpc)

                    if p:getMapNumber() == b:getMapNumber() then
                        local dx = p:getX() - b:getX()
                        local dy = p:getY() - b:getY()
                        local dist = math.sqrt(dx * dx + dy * dy)

                        if dist <= WarTeam_Config.CaptureRange then
                            hasPlayerInRange = true
                            break
                        end
                    end
                end

                if not hasPlayerInRange then
                    WarTeam.CancelCaptureForTeam(baseLetter)
                elseif elapsed >= requiredTime then
                    WarTeam.FinishCapture(baseLetter)
                end
            end
        else
            -- Se não tem mais ninguém clicando, cancela
            if WarTeam.BaseCaptureStart[baseLetter] > 0 then
                WarTeam.CancelCaptureForTeam(baseLetter)
            end
        end
    end

    -- Repete a cada 1 segundo
    Timer.TimeOut(1, WarTeam.CheckCaptureProgress)
end

function WarTeam.TeleportTeamToBase(team, baseLetter)
    local map, x, y = WarTeam.GetBaseCoords(baseLetter)
    local list = team == "Blue" and WarTeam.TeamBlue or WarTeam.TeamRed
    for _, idx in ipairs(list) do
        if WarTeam.Players[idx] then Teleport(idx, map, x, y) end
    end
end
function WarTeam.TeleportDefeatedTeam(team)
    local list = team == "Blue" and WarTeam.TeamBlue or WarTeam.TeamRed
    for _, idx in ipairs(list) do
        if WarTeam.Players[idx] then
            Teleport(idx, WarTeam_Config.DefeatedTeleportMap, WarTeam_Config.DefeatedTeleportX, WarTeam_Config.DefeatedTeleportY)
        end
    end
end

function WarTeam.OnPlayerMove(aIndex)
    local team = WarTeam.GetPlayerTeam(aIndex)
    if not team then 
        return 
    end

    for baseLetter, clickers in pairs(WarTeam.BaseClickers) do
        if clickers[aIndex] then
            local baseNpc = (baseLetter == "Blue") and WarTeam.BaseA or WarTeam.BaseB
            
            if baseNpc == -1 then
                clickers[aIndex] = nil
                goto continue
            end

            local player = User.new(aIndex)
            local base   = User.new(baseNpc)

            -- Verifica se está no mesmo mapa
            if player:getMapNumber() ~= base:getMapNumber() then
                clickers[aIndex] = nil
                goto continue
            end

            -- CÁLCULO DE DISTÂNCIA CIRCULAR (EUCLIDIANA)
            local dx = player:getX() - base:getX()
            local dy = player:getY() - base:getY()
            local distance = math.sqrt(dx * dx + dy * dy)

            -- Se saiu do range circular, remove do clique
            if distance > WarTeam_Config.CaptureRange then
                clickers[aIndex] = nil
            end

            ::continue::
        end
    end
end

function WarTeam.OnPlayerDie(aIndex, TargetIndex)
    for baseLetter, clickers in pairs(WarTeam.BaseClickers) do
        if clickers[TargetIndex] then
            clickers[TargetIndex] = nil
        end
    end
    if not WarTeam.Active then return end
    local killerTeam = WarTeam.GetPlayerTeam(aIndex)
    local deadTeam = WarTeam.GetPlayerTeam(TargetIndex)
    if killerTeam and deadTeam and killerTeam ~= deadTeam then
        if killerTeam == "Blue" then
            WarTeam.PointsBlue = WarTeam.PointsBlue + WarTeam_Config.PointsPerKill
        else
            WarTeam.PointsRed = WarTeam.PointsRed + WarTeam_Config.PointsPerKill
        end
        MsgGlobal(3, User.new(aIndex):getName(), WarTeam_Config.PointsPerKill, killerTeam)
        WarTeam.SendScoreUpdate()
    end
    if deadTeam == "Blue" then
        if WarTeam.BaseOwnerB == "Blue" then
            local map, x, y = WarTeam.GetBaseCoords("Red")
            Teleport(TargetIndex, map, x, y)
        elseif WarTeam.BaseOwnerA == "Blue" then
            local map, x, y = WarTeam.GetBaseCoords("Blue")
            Teleport(TargetIndex, map, x, y)
        else
            Teleport(TargetIndex,
                WarTeam_Config.DefeatedTeleportMap,
                WarTeam_Config.DefeatedTeleportX,
                WarTeam_Config.DefeatedTeleportY
            )
        end
    elseif deadTeam == "Red" then
        if WarTeam.BaseOwnerA == "Red" then
            local map, x, y = WarTeam.GetBaseCoords("Blue")
            Teleport(TargetIndex, map, x, y)
        elseif WarTeam.BaseOwnerB == "Red" then
            local map, x, y = WarTeam.GetBaseCoords("Red")
            Teleport(TargetIndex, map, x, y)
        else
            Teleport(TargetIndex,
                WarTeam_Config.DefeatedTeleportMap,
                WarTeam_Config.DefeatedTeleportX,
                WarTeam_Config.DefeatedTeleportY
            )
        end
    end
end
function WarTeam.OnPlayerLogout(aIndex)
    for baseLetter, clickers in pairs(WarTeam.BaseClickers) do
        clickers[aIndex] = nil
    end
    WarTeam.Players[aIndex] = nil
    WarTeam.SendUpdateLists()
end
function WarTeam.OnCharacterMove(aIndex)
    WarTeam.OnPlayerMove(aIndex)
end
function WarTeam.Init()
    if not WarTeam_Config.Enabled then return end
    GameServerFunctions.NpcTalk(WarTeam.RegisterNpcTalk)
    GameServerFunctions.NpcTalk(WarTeam.BaseNpcTalk)
    GameServerFunctions.PlayerDie(WarTeam.OnPlayerDie)
    GameServerFunctions.PlayerAttack(WarTeam.BlockAlliedAttack)
    GameServerFunctions.GameServerProtocol(WarTeam.RegisterPlayer)
    GameServerFunctions.PlayerMove(WarTeam.OnPlayerMove)
    GameServerFunctions.CharacterMove(WarTeam.OnCharacterMove)
    GameServerFunctions.EnterCharacter(WarTeam.ReconnectPlayer)
    GameServerFunctions.PlayerLogout(WarTeam.OnPlayerLogout)
    Commands.Register(WarTeam_Config.GMCommand, WarTeam.GMCommand)
    Schedule.SetHourAndMinute(WarTeam_Config.RegisterHour, WarTeam_Config.RegisterMinute, WarTeam.StartRegistration)
end

WarTeam.Init()