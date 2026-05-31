CustomRanking = {}
local CR_Open = 0
local CR_TotalRankings = 0
local CR_RankingNames = {}
local CR_CurrentIndex = 1
local CR_Rows = {}
local CR_Loading = false
local CR_KEY_OPEN = Keys.F8
local CR_PACKET = 0x56
local CR_PACKET_NAME = "CUSTOM_RANKING_PACKET"

local CUSTOM_CLASS_NAMES = {
    [0] = "DW", [1] = "SM", [2] = "GrM",
    [16] = "DK", [17] = "BK", [18] = "BM",
    [32] = "ELF", [33] = "ME", [34] = "HE",
    [48] = "MG", [50] = "DM",
    [64] = "DL", [66] = "LE",
    [80] = "SUM", [81] = "BS", [82] = "DM/SUM",
}
function CustomRanking.RequestMeta()
    local pkt = string.format("%s_REQ_META_%s", CR_PACKET_NAME, UserGetName())
    CreatePacket(pkt, CR_PACKET)
    SendPacket(pkt)
    ClearPacket(pkt)
    CR_Loading = true
end
function CustomRanking.RequestData(rankingIndex)
    local pkt = string.format("%s_REQ_DATA_%s", CR_PACKET_NAME, UserGetName())
    CreatePacket(pkt, CR_PACKET)
    SetBytePacket(pkt, rankingIndex)
    SendPacket(pkt)
    ClearPacket(pkt)
    CR_Loading = true
end
function CustomRanking.Protocol(Packet, PacketName)
    if Packet ~= CR_PACKET then return end
    local nome = UserGetName()
    if PacketName == string.format("%s_META_%s", CR_PACKET_NAME, nome) then
        local sub = GetBytePacket(PacketName, -1)
        if sub ~= 0x01 then ClearPacket(PacketName) return end
        CR_TotalRankings = GetBytePacket(PacketName, -1)
        CR_RankingNames = {}
        for i = 1, CR_TotalRankings do
            local len = GetBytePacket(PacketName, -1)
            local str = ""
            for c = 1, len do str = str .. string.char(GetBytePacket(PacketName, -1)) end
            CR_RankingNames[i] = str
        end
        ClearPacket(PacketName)
        CR_Loading = false
        CR_Open = 1
        return
    end
    if string.sub(PacketName, 1, #CR_PACKET_NAME + 6) == CR_PACKET_NAME .. "_DATA_" then
        local sub = GetBytePacket(PacketName, -1)
        if sub ~= 0x02 then ClearPacket(PacketName) return end
        local idx = GetBytePacket(PacketName, -1)
        local count = GetBytePacket(PacketName, -1)
        if idx == CR_CurrentIndex then
            CR_Rows = {}
            for i = 1, count do
                local name = GetCharPacket(PacketName, -1)
                local level = GetWordPacket(PacketName, -1)
                local class = GetBytePacket(PacketName, -1)
                local online = GetBytePacket(PacketName, -1)
                local score = GetDwordPacket(PacketName, -1)
                name = string.gsub(name, "%z", "")
                name = string.sub(name, 1, 10)
                CR_Rows[i] = {name = name, level = level, class = class, online = online, score = score}
            end
        end
        ClearPacket(PacketName)
        CR_Loading = false
        return
    end
end
function CustomRanking.Open()
    if CR_Open == 1 then return end
    CR_CurrentIndex = 1
    CR_Rows = {}
    CR_Loading = true
    CustomRanking.RequestMeta()
end
function CustomRanking.Close()
    CR_Open = 0
    CR_Loading = false
    CR_Rows = {}
end
function CustomRanking.NextRanking()
    if CR_TotalRankings <= 0 then return end
    CR_CurrentIndex = CR_CurrentIndex + 1
    if CR_CurrentIndex > CR_TotalRankings then CR_CurrentIndex = 1 end
    CR_Rows = {}
    CustomRanking.RequestData(CR_CurrentIndex)
end
function CustomRanking.PrevRanking()
    if CR_TotalRankings <= 0 then return end
    CR_CurrentIndex = CR_CurrentIndex - 1
    if CR_CurrentIndex < 1 then CR_CurrentIndex = CR_TotalRankings end
    CR_Rows = {}
    CustomRanking.RequestData(CR_CurrentIndex)
end

local DIV_X = {20, 71, 101, 142, 179}

local COL = {
    hash     = {10,  14, 1},
    name     = {26,  64, 1},
    level    = {92,  28, 7},
    class_   = {122, 36, 1},
    online   = {160, 36, 7},
    score    = {198, 24, 7},
}

local function DrawHLine(x, y, width, r, g, b, a)
    glColor4f(r/255, g/255, b/255, a)
    RenderImage2(31319, x, y, width, 1, 2, 1.0, 0.0, 1.0, 1.0, 1, 1.0)
    glColor4f(1.0, 1.0, 1.0, 1.0)
end
local function DrawVLine(x, y, height, r, g, b, a)
    glColor4f(r/255, g/255, b/255, a)
    RenderImage2(31319, x, y, 1, height, 2, 1.0, 0.0, 1.0, 1.0, 1, 1.0)
    glColor4f(1.0, 1.0, 1.0, 1.0)
end

local function DrawRowDividers(x, rowY, rowH, r, g, b, a)
    for _, dx in ipairs(DIV_X) do
        DrawVLine(x + dx, rowY, rowH, r, g, b, a)
    end
end

function CustomRanking.MainProc()
    if CR_Open == 0 then return end
    EnableAlphaTest()
    glColor4f(1.0, 1.0, 1.0, 1.0)
    CustomRanking.RenderFrame()
end
function CustomRanking.RenderFrame()
    local PosX = (640 / 2) - (230 / 2) + GetWideX()
    local PosY = 80
    RenderImage(31319, PosX, PosY + 2.0, 222.0, 257.0)
    RenderImage(31328, PosX, PosY, 230, 67)
    PosY = PosY + 45.0
    for i = 1, 11 do
        RenderImage(31317, PosX, PosY, 230.0, 15.0)
        PosY = PosY + 15.0
    end
    RenderImage(31317, PosX, PosY, 230.0, 5.0)
    PosY = PosY + 5.0
    RenderImage(31318, PosX, PosY, 230.0, 45.0)
    CustomRanking.RenderText(PosX, 80)
    if CR_TotalRankings > 0 then
        local btnY = 80 + 240
        if MousePosX() >= PosX + 70 and MousePosX() <= PosX + 95 and MousePosY() >= btnY and MousePosY() <= btnY + 16 then
            RenderImage2(32343, PosX + 75, btnY, 18, 13, 0, 0.18, 0.52, 0.18, 1, 1, 1.0)
        else
            RenderImage2(32343, PosX + 75, btnY, 18, 13, 0, 0.0, 0.52, 0.18, 1, 1, 1.0)
        end
        if MousePosX() >= PosX + 135 and MousePosX() <= PosX + 160 and MousePosY() >= btnY and MousePosY() <= btnY + 16 then
            RenderImage2(32344, PosX + 135, btnY, 18, 13, 0, 0.18, 0.52, 0.18, 1, 1, 1.0)
        else
            RenderImage2(32344, PosX + 135, btnY, 18, 13, 0, 0.0, 0.52, 0.18, 1, 1, 1.0)
        end
        SetTextColor(255, 189, 25, 255)
        RenderText3(PosX, btnY - 2, string.format("%d / %d", CR_CurrentIndex, CR_TotalRankings), 230, 3)
    end
    DisableAlphaBlend()
end
function CustomRanking.RenderText(x, y)
    SetFontType(2)
    SetTextBg(0, 0, 0, 0)

    SetTextColor(255, 189, 25, 255)
    local title = CR_RankingNames[CR_CurrentIndex] or "Ranking"
    RenderText3(x, y + 6, title, 230, 3)
    
    SetFontType(1)
    SetTextBg(0, 0, 0, 0)

    DrawHLine(x + 8, y + 46, 214, 255, 189, 25, 0.25)
    DrawHLine(x + 8, y + 60, 214, 255, 189, 25, 0.6)

    DrawRowDividers(x, y + 46, 14, 255, 189, 25, 0.5)

    SetTextColor(255, 189, 25, 255)
    RenderText3(x + COL.hash[1],   y + 48, "#",          COL.hash[2],   COL.hash[3])
    RenderText3(x + COL.name[1],   y + 48, "Personagem", COL.name[2],   COL.name[3])
    RenderText3(x + COL.level[1] + 2,  y + 48, "Level",      COL.level[2],  COL.level[3])
    RenderText3(x + COL.class_[1] - 10, y + 48, "Classe",     COL.class_[2], COL.class_[3])
    RenderText3(x + COL.online[1] + 10, y + 48, "Online",     COL.online[2], COL.online[3])
    RenderText3(x + COL.score[1] + 5,  y + 48, "Score",      COL.score[2],  COL.score[3])

    if CR_Loading then
        SetTextColor(255, 220, 80, 255)
        RenderText3(x, y + 100, "Carregando...", 230, 3)
        return
    end
    if #CR_Rows == 0 then
        SetTextColor(180, 180, 180, 255)
        RenderText3(x, y + 100, "Sem resultados", 230, 3)
        return
    end

    for i, row in ipairs(CR_Rows) do
        local rowY = y + 61 + (i-1)*15

        DrawHLine(x + 8, rowY + 14, 214, 255, 255, 255, 0.1)

        DrawRowDividers(x, rowY, 15, 255, 255, 255, 0.18)

        local className  = CUSTOM_CLASS_NAMES[row.class] or string.format("C%d", row.class)
        local onlineText = row.online == 1 and "Online" or "Offline"
        local onlineColor = row.online == 1 and {0, 255, 0} or {255, 100, 100}

        if i == 1 then SetTextColor(255, 215, 0, 255)
        elseif i == 2 then SetTextColor(192, 192, 192, 255)
        elseif i == 3 then SetTextColor(205, 127, 50, 255)
        else SetTextColor(255, 255, 255, 255) end
        RenderText3(x + COL.hash[1], rowY + 3, string.format("%d", i), COL.hash[2], COL.hash[3])

        SetTextColor(255, 255, 255, 255)
        RenderText3(x + COL.name[1],   rowY + 3, row.name,                    COL.name[2],   COL.name[3])
        RenderText3(x + COL.level[1],  rowY + 3, string.format("%d", row.level), COL.level[2], COL.level[3])
        RenderText3(x + COL.class_[1] - 10, rowY + 3, className,                   COL.class_[2], COL.class_[3])

        SetTextColor(onlineColor[1], onlineColor[2], onlineColor[3], 255)
        RenderText3(x + COL.online[1] + 10, rowY + 3, onlineText, COL.online[2], COL.online[3])

        SetTextColor(255, 255, 255, 255)
        RenderText3(x + COL.score[1] + 13, rowY + 3, string.format("%d", row.score), COL.score[2], COL.score[3])
    end
end

function CustomRanking.UpdateMouse()
    if CR_Open == 0 then return end
    local PosX = (640 / 2) - (230 / 2) + GetWideX()
    local PosY = 80
    local btnY = PosY + 240
    if MousePosX() >= PosX + 70 and MousePosX() <= PosX + 95 and MousePosY() >= btnY and MousePosY() <= btnY + 16 then
        if CheckClickClient() == 1 then DisableClickClient() CustomRanking.PrevRanking() end
    end
    if MousePosX() >= PosX + 135 and MousePosX() <= PosX + 160 and MousePosY() >= btnY and MousePosY() <= btnY + 16 then
        if CheckClickClient() == 1 then DisableClickClient() CustomRanking.NextRanking() end
    end
    if MousePosX() >= PosX and MousePosX() <= PosX + 230 and MousePosY() >= PosY and MousePosY() <= PosY + 257 then
        DisableClickClient()
    end
end
function CustomRanking.UpdateKeyEvent()
    if CR_Open == 0 then return end
    if CheckPressedKey(Keys.Escape) == 1 then CustomRanking.Close() return end
    if CheckWindowOpen(UIChatWindow) == 1 or CheckWindowOpen(UICashShop) == 1 or
       CheckWindowOpen(UIFullMap) == 1 or CheckWindowOpen(UIFriendList) == 1 or
       CheckWindowOpen(UINPC_Dialog) == 1 or CheckWindowOpen(UIStore) == 1 or
       CheckWindowOpen(UITrade) == 1 or CheckWindowOpen(UIChaosBox) == 1 or
       CheckWindowOpen(UIWarehouse) == 1 then
        CustomRanking.Close()
    end
end
function CustomRanking.MainProcWorldKey(key)
    if key ~= CR_KEY_OPEN then return end
    if CR_Open == 1 then
        CustomRanking.Close()
    else
        CustomRanking.Open()
    end
end
function CustomRanking.Init()
    InterfaceController.MainProc(CustomRanking.MainProc)
    InterfaceController.ClientProtocol(CustomRanking.Protocol)
    InterfaceController.UpdateMouse(CustomRanking.UpdateMouse)
    InterfaceController.UpdateKey(CustomRanking.UpdateKeyEvent)
    InterfaceController.MainProcWorldKey(CustomRanking.MainProcWorldKey)
end
CustomRanking.Init()
return CustomRanking