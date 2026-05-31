-- =====================================================
-- CUSTOM RANKING - SERVIDOR (Com Desempates Inteligentes + Online)
-- =====================================================
CustomRanking = {}

-- =====================================================
-- DBQuery - Com Regras Avançadas de Desempate
-- =====================================================
function CustomRanking.DBQuery(rankingIndex)
    local cfg = CUSTOM_RANKINGS[rankingIndex]
    if not cfg then return {} end

    local results = {}
    local pos = 1

    -- ====================== TABELA CHARACTER ======================
    if string.lower(cfg.Table) == "character" then
        
        local orderBy = ""

        -- Rankings com desempate inteligente
        if cfg.Score == "ResetDay" or cfg.Score == "ResetWeek" or cfg.Score == "ResetMonth" or 
           cfg.Score == "ResetCount" or cfg.Score == "MasterResetCount" then
            
            if cfg.Score == "ResetCount" then
                orderBy = "ResetCount DESC, cLevel DESC"
                
            elseif cfg.Score == "MasterResetCount" then
                orderBy = "MasterResetCount DESC, ResetCount DESC, cLevel DESC"
                
            else
                -- ResetDay, ResetWeek, ResetMonth
                orderBy = cfg.Score .. " DESC, ResetCount DESC, cLevel DESC"
            end

        else
            -- Outros rankings da tabela Character (se houver)
            orderBy = cfg.Score .. " DESC, cLevel DESC"
        end

        local sql = string.format(
            "SELECT TOP %d Name, cLevel, Class, %s FROM %s WHERE CtlCode <= 0 ORDER BY %s",
            CUSTOM_RANKING_MAX_RESULT,
            cfg.Score,
            cfg.Table,
            orderBy
        )

        local ret = db:exec(sql)
        if ret == 0 then
            db:clear()
            return {}
        end

        while true do
            local row = db:fetch()
            if row == 100 then break end

            local charName = db:getStr("Name") or "???"
            local isOnline = (GetIndex(charName) ~= -1) and 1 or 0

            results[pos] = {
                name   = charName,
                level  = tonumber(db:getStr("cLevel")) or 0,
                class  = tonumber(db:getStr("Class")) or 0,
                score  = tonumber(db:getStr(cfg.Score)) or 0,
                online = isOnline
            }
            pos = pos + 1
        end

        db:clear()

    -- ====================== TABELAS SECUNDÁRIAS (Maratona, etc) ======================
    else
        -- Ordenação direta e mais segura
        local sqlRanking = string.format(
            "SELECT TOP %d %s as CharName, %s as ScorePoint FROM %s ORDER BY %s DESC",
            CUSTOM_RANKING_MAX_RESULT,
            cfg.Name,
            cfg.Score,
            cfg.Table,
            cfg.Score
        )

        local ret = db:exec(sqlRanking)
        if ret == 0 then
            db:clear()
            return {}
        end

        local tempList = {}

        while true do
            local row = db:fetch()
            if row == 100 then break end

            local charName = db:getStr("CharName") or "???"
            local scoreVal = tonumber(db:getStr("ScorePoint")) or 0

            table.insert(tempList, {name = charName, score = scoreVal})
        end
        db:clear()

        if #tempList == 0 then return {} end

        -- Busca informações adicionais (Level, Class, Online)
        local nameList = {}
        for _, v in ipairs(tempList) do
            table.insert(nameList, "'" .. v.name .. "'")
        end

        local sqlChar = string.format(
            "SELECT Name, cLevel, Class FROM Character WHERE Name IN (%s) AND CtlCode <= 0",
            table.concat(nameList, ",")
        )

        ret = db:exec(sqlChar)
        if ret == 0 then db:clear() return {} end

        local charInfo = {}
        while true do
            local row = db:fetch()
            if row == 100 then break end
            local name = db:getStr("Name") or "???"
            charInfo[name] = {
                level = tonumber(db:getStr("cLevel")) or 0,
                class = tonumber(db:getStr("Class")) or 0
            }
        end
        db:clear()

        -- Monta resultado final mantendo a ordem original do ranking
        for _, data in ipairs(tempList) do
            local info = charInfo[data.name] or {level = 0, class = 0}
            local isOnline = (GetIndex(data.name) ~= -1) and 1 or 0

            results[pos] = {
                name   = data.name,
                level  = info.level,
                class  = info.class,
                score  = data.score,
                online = isOnline
            }
            pos = pos + 1
        end
    end

    return results
end

-- =====================================================
-- Envia Meta / Dados / Protocolo / Init (sem alterações)
-- =====================================================
function CustomRanking.SendMeta(aIndex)
    local player = User.new(aIndex)
    local nome = player:getName()
    local pkt = string.format("%s_META_%s", CUSTOM_RANKING_PACKET_NAME, nome)

    CreatePacket(pkt, CUSTOM_RANKING_PACKET)
    SetBytePacket(pkt, 0x01)
    SetBytePacket(pkt, CUSTOM_RANKING_MAX_RANKING)

    for i = 1, CUSTOM_RANKING_MAX_RANKING do
        local rNome = CUSTOM_RANKINGS[i].Nome or ("Ranking " .. i)
        SetBytePacket(pkt, #rNome)
        for c = 1, #rNome do
            SetBytePacket(pkt, string.byte(rNome, c))
        end
    end
    SendPacket(pkt, aIndex)
    ClearPacket(pkt)
end

function CustomRanking.SendRankingData(aIndex, rankingIndex)
    if rankingIndex < 1 or rankingIndex > CUSTOM_RANKING_MAX_RANKING then
        rankingIndex = 1
    end

    local rows = CustomRanking.DBQuery(rankingIndex)
    local player = User.new(aIndex)
    local nome = player:getName()

    local pkt = string.format("%s_DATA_%s_%d", CUSTOM_RANKING_PACKET_NAME, nome, rankingIndex)

    CreatePacket(pkt, CUSTOM_RANKING_PACKET)
    SetBytePacket(pkt, 0x02)
    SetBytePacket(pkt, rankingIndex)
    SetBytePacket(pkt, #rows)

    for _, row in ipairs(rows) do
        local name = string.sub(tostring(row.name), 1, 10)
        SetCharPacket(pkt, name)
        SetWordPacket(pkt, row.level)
        SetBytePacket(pkt, row.class)
        SetBytePacket(pkt, row.online)
        SetDwordPacket(pkt, row.score)
    end

    SendPacket(pkt, aIndex)
    ClearPacket(pkt)
end

function CustomRanking.Protocol(aIndex, Packet, PacketName)
    if Packet ~= CUSTOM_RANKING_PACKET then return false end
    local nome = User.new(aIndex):getName()

    if PacketName == string.format("%s_REQ_META_%s", CUSTOM_RANKING_PACKET_NAME, nome) then
        ClearPacket(PacketName)
        if CUSTOM_RANKING_SWITCH ~= 1 then return true end
        CustomRanking.SendMeta(aIndex)
        CustomRanking.SendRankingData(aIndex, 1)
        return true
    end

    if string.find(PacketName, string.format("%s_REQ_DATA_%s", CUSTOM_RANKING_PACKET_NAME, nome)) then
        local rankingIndex = GetBytePacket(PacketName, -1)
        ClearPacket(PacketName)
        if CUSTOM_RANKING_SWITCH ~= 1 then return true end
        CustomRanking.SendRankingData(aIndex, rankingIndex)
        return true
    end
    return false
end

function CustomRanking.Init()
    if CUSTOM_RANKING_SWITCH ~= 1 then
        LogAdd("[CustomRanking] Sistema desativado.")
        return
    end
    GameServerFunctions.GameServerProtocol(CustomRanking.Protocol)
    LogAddC(3, "[CustomRanking] Sistema iniciado com " .. CUSTOM_RANKING_MAX_RANKING .. " ranking(s).")
end

CustomRanking.Init()
return CustomRanking