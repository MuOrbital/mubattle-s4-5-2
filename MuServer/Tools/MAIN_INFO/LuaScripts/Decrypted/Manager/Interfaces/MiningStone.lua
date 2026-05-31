MiningClient = {}
MiningClient.Active = false
MiningClient.Start = 0
MiningClient.Time = 50  --Deve ser o Mesmo do Servidor
MiningClient.LastPacket = ""

function MiningClient.Protocol(Packet, PacketName)
    if string.find(PacketName, "MININGSTART") then
        MiningClient.Active = true
        MiningClient.Start = os.clock()
        MiningClient.LastPacket = PacketName
        return true
    elseif string.find(PacketName, "MININGCANCEL") then
        MiningClient.Active = false
        MiningClient.Start = 0
        MiningClient.LastPacket = PacketName
        return true
    end
    return false
end
InterfaceController.ClientProtocol(MiningClient.Protocol)

function MiningClient.Draw()
    if not MiningClient.Active then return end

    local elapsed = os.clock() - MiningClient.Start
    local percent = elapsed / MiningClient.Time

    if percent >= 1 then
        MiningClient.Active = false
        local pname = string.format("MININGFINISH-%s-%d", UserGetName(), UserGetIndex())
        CreatePacket(pname, 0x34)
        SendPacket(pname)
        ClearPacket(pname)
        return
    end

    SetBlend()
    glColor4f(0, 0, 0, 0.5)
    DrawBar(0, 0, 2560, 1440)
    EndDrawBar()

    local barX = 220 + GetWideX() * 1 --Altere para    GetWideX() * 2    se for usar na Rox Gaming
    local barY = 20
    local barWidth = 260
    local barHeight = 22

    SetBlend()
    glColor4f(0, 0, 0, 1.0)
    DrawBar(barX, barY, barWidth, barHeight)
    EndDrawBar()

    SetBlend()
    glColor4f(0.9, 0.9, 0.2, 1.0)
    DrawBar(barX + 2, barY + 2, 256 * percent, 18)
    EndDrawBar()

    SetFontType(2)
    SetTextColor(255, 0, 0, 255)
    SetTextBg(0, 0, 0, 0)
    RenderText3(barX + 50, barY + 4, string.format("Minerando... %d%%", math.floor(percent * 100)), 150, 3)
end

InterfaceController.MainProc(MiningClient.Draw)

return MiningClient