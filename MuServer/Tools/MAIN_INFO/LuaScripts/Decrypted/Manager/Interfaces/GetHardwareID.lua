---Mantenha este script sempre Encriptado no Cliente
local HEADCODE_HWID = 0xF1

local hardwareID = GetHardwareId() or "N/A"

function HandleClientProtocol(Packet, PacketName)
    if Packet == HEADCODE_HWID then
        local subcode = GetBytePacket(PacketName, -1)
        
        if subcode == 0x01 then
            ClearPacket(PacketName)
            
            local playerName = UserGetName()
            local responsePacketName = string.format("ResponseHWID_%s", playerName)
            
            CreatePacket(responsePacketName, HEADCODE_HWID)
            SetBytePacket(responsePacketName, 0x02)
            SetCharPacketLength(responsePacketName, hardwareID, 36)
            
            SendPacket(responsePacketName)
            ClearPacket(responsePacketName)
            
            return true
        end
    end
    
    return false
end

InterfaceController.ClientProtocol(HandleClientProtocol)