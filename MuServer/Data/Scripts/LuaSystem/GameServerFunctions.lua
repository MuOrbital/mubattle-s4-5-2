GameServerFunctions = {}

-- Quando o player dropa um item
-- Se quiser bloquear o drop, retorne 1 na sua função
local PlayerDropItem_Handles = {}
function PlayerDropItem(aIndex, x, y, slot)
    for i = 1, #PlayerDropItem_Handles do
        if PlayerDropItem_Handles[i].callback(aIndex, x, y, slot) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.PlayerDropItem(callback)
    table.insert(PlayerDropItem_Handles, { callback = callback })
end

-- Quando o player loga ou equipa item
local CharacterSet_Handles = {}
function CharacterSet(aIndex)
    for i = 1, #CharacterSet_Handles do
        CharacterSet_Handles[i].callback(aIndex)
    end
end
function GameServerFunctions.CharacterSet(callback)
    table.insert(CharacterSet_Handles, { callback = callback })
end

-- Manipula o dano do personagem
local PlayerCheckDamage_Handles = {}
function PlayerCheckDamage(aIndex, TargetIndex, Damage, ShieldDamage, DamageType)
    local newDamage = Damage - ShieldDamage
    for i = 1, #PlayerCheckDamage_Handles do
        newDamage = PlayerCheckDamage_Handles[i].callback(aIndex, TargetIndex, newDamage, DamageType)
    end
    newDamage = newDamage + ShieldDamage
    return newDamage
end
function GameServerFunctions.PlayerCheckDamage(callback)
    table.insert(PlayerCheckDamage_Handles, { callback = callback })
end

-- Quando um monstro morre
local MonsterDie_Handles = {}
function MonsterDie(Player, Monster)
    for i = 1, #MonsterDie_Handles do
        MonsterDie_Handles[i].callback(Player, Monster)
    end
end
function GameServerFunctions.MonsterDie(callback)
    table.insert(MonsterDie_Handles, { callback = callback })
end

-- Quando o monstro dropa um item para o player
-- Retorno 1 impede o monstro de dropar o item
local MonsterDieGiveItem_Handles = {}
function MonsterDieGiveItem(Player, Monster)
    for i = 1, #MonsterDieGiveItem_Handles do
        if MonsterDieGiveItem_Handles[i].callback(Player, Monster) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.MonsterDieGiveItem(callback)
    table.insert(MonsterDieGiveItem_Handles, { callback = callback })
end

-- Quando um player mata outro player
local PlayerDie_Handles = {}
function PlayerDie(aIndex, TargetIndex)
    for i = 1, #PlayerDie_Handles do
        PlayerDie_Handles[i].callback(aIndex, TargetIndex)
    end
end
function GameServerFunctions.PlayerDie(callback)
    table.insert(PlayerDie_Handles, { callback = callback })
end

-- Quando um player ataca outro
local PlayerAttack_Handles = {}
function PlayerAttack(aIndex, TargetIndex)
    for i = 1, #PlayerAttack_Handles do
        if PlayerAttack_Handles[i].callback(aIndex, TargetIndex) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.PlayerAttack(callback)
    table.insert(PlayerAttack_Handles, { callback = callback })
end

-- Quando o player tenta adicionar ponto em "c"
local LevelUpPointAdd_Handles = {}
function LevelUpPointAdd(aIndex, Type)
    for i = 1, #LevelUpPointAdd_Handles do
        if LevelUpPointAdd_Handles[i].callback(aIndex, Type) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.LevelUpPointAdd(callback)
    table.insert(LevelUpPointAdd_Handles, { callback = callback })
end

-- Quando um player envia pedido de trade para outro jogador
-- Retorno 1 impede o envio do pedido de trade
local PlayerSendTrade_Handles = {}
function PlayerSendTrade(aIndex, TargetIndex)
    for i = 1, #PlayerSendTrade_Handles do
        if PlayerSendTrade_Handles[i].callback(aIndex, TargetIndex) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.PlayerSendTrade(callback)
    table.insert(PlayerSendTrade_Handles, { callback = callback })
end

-- Quando o player equipa um ring de transformação
-- Retorno 1 não muda a skin do player
local PlayerRingEquiped_Handles = {}
function PlayerRingEquiped(aIndex)
    for i = 1, #PlayerRingEquiped_Handles do
        if PlayerRingEquiped_Handles[i].callback(aIndex) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.PlayerRingEquiped(callback)
    table.insert(PlayerRingEquiped_Handles, { callback = callback })
end

-- Quando dois players em negociação apertam OK no trade
-- Retorno 1 cancela a negociação
local PlayerTradeOk_Handles = {}
function PlayerTradeOk(aIndex, TargetIndex)
    for i = 1, #PlayerTradeOk_Handles do
        if PlayerTradeOk_Handles[i].callback(aIndex, TargetIndex) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.PlayerTradeOk(callback)
    table.insert(PlayerTradeOk_Handles, { callback = callback })
end

-- Quando o player desloga
local PlayerLogout_Handles = {}
function PlayerLogout(aIndex, Name, Account)
    for i = 1, #PlayerLogout_Handles do
        PlayerLogout_Handles[i].callback(aIndex, Name, Account)
    end
end
function GameServerFunctions.PlayerLogout(callback)
    table.insert(PlayerLogout_Handles, { callback = callback })
end

-- Quando duas guilds estão em War e uma delas marca ponto
local GuildWarProc_Handles = {}
function GuildWarProc(GuildName1, GuildPoints1, GuildName2, GuildPoints2)
    for i = 1, #GuildWarProc_Handles do
        GuildWarProc_Handles[i].callback(GuildName1, GuildPoints1, GuildName2, GuildPoints2)
    end
end
function GameServerFunctions.GuildWarProc(callback)
    table.insert(GuildWarProc_Handles, { callback = callback })
end

-- Quando um player entra na sala
local EnterCharacter_Handles = {}
function EnterCharacter(aIndex)
    for i = 1, #EnterCharacter_Handles do
        EnterCharacter_Handles[i].callback(aIndex)
    end
end
function GameServerFunctions.EnterCharacter(callback)
    table.insert(EnterCharacter_Handles, { callback = callback })
end

-- Quando um player deleta um personagem
local DeleteCharacter_Handles = {}
function DeleteCharacter(aIndex, name)
    for i = 1, #DeleteCharacter_Handles do
        DeleteCharacter_Handles[i].callback(aIndex, name)
    end
end
function GameServerFunctions.DeleteCharacter(callback)
    table.insert(DeleteCharacter_Handles, { callback = callback })
end

-- Quando o player é movido usando a função Teleport()
local CharacterMove_Handles = {}
function CharacterMove(aIndex, map, x, y)
    for i = 1, #CharacterMove_Handles do
        if CharacterMove_Handles[i].callback(aIndex, map, x, y) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.CharacterMove(callback)
    table.insert(CharacterMove_Handles, { callback = callback })
end

-- Quando o player caminha
local PlayerMove_Handles = {}
function PlayerMove(aIndex, map, x, y, sx, sy)
    for i = 1, #PlayerMove_Handles do
        PlayerMove_Handles[i].callback(aIndex, map, x, y, sx, sy)
    end
end
function GameServerFunctions.PlayerMove(callback)
    table.insert(PlayerMove_Handles, { callback = callback })
end

-- Quando o player clica em um NPC
-- Retorno 1 não deixa o NPC abrir
local NpcTalk_Handles = {}
function NpcTalk(Npc, Player)
    for i = 1, #NpcTalk_Handles do
        if NpcTalk_Handles[i].callback(Npc, Player) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.NpcTalk(callback)
    table.insert(NpcTalk_Handles, { callback = callback })
end

-- Quando o player vende um item no NPC
-- Retorno 1 impede a venda do item
local PlayerSellItem_Handles = {}
function PlayerSellItem(aIndex, Position)
    for i = 1, #PlayerSellItem_Handles do
        if PlayerSellItem_Handles[i].callback(aIndex, Position) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.PlayerSellItem(callback)
    table.insert(PlayerSellItem_Handles, { callback = callback })
end

-- Quando o player repara um item
-- Retorno 1 impede que o item seja reparado
local PlayerRepairItem_Handles = {}
function PlayerRepairItem(aIndex, Position)
    for i = 1, #PlayerRepairItem_Handles do
        if PlayerRepairItem_Handles[i].callback(aIndex, Position) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.PlayerRepairItem(callback)
    table.insert(PlayerRepairItem_Handles, { callback = callback })
end

-- Quando o player tenta caminhar
-- Retorno 1 impede de caminhar
local PlayerCanMove_Handles = {}
function PlayerCanMove(aIndex)
    for i = 1, #PlayerCanMove_Handles do
        if PlayerCanMove_Handles[i].callback(aIndex) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.PlayerCanMove(callback)
    table.insert(PlayerCanMove_Handles, { callback = callback })
end

-- Quando o player muda o item de lugar
local PlayerMoveItem_Handles = {}
function PlayerMoveItem(aIndex, SourceSlot, TargetSlot, Type)
    for i = 1, #PlayerMoveItem_Handles do
        PlayerMoveItem_Handles[i].callback(aIndex, SourceSlot, TargetSlot, Type)
    end
    return 0
end
function GameServerFunctions.PlayerMoveItem(callback)
    table.insert(PlayerMoveItem_Handles, { callback = callback })
end

-- Quando o player abre o baú
local PlayerVaultOpen_Handles = {}
function PlayerVaultOpen(aIndex)
    for i = 1, #PlayerVaultOpen_Handles do
        PlayerVaultOpen_Handles[i].callback(aIndex)
    end
    return 0
end
function GameServerFunctions.PlayerVaultOpen(callback)
    table.insert(PlayerVaultOpen_Handles, { callback = callback })
end

-- Quando um player declara war a uma guild
-- Retorno 1 impede de declarar war
local DeclareWar_Handles = {}
function DeclareWar(aIndex)
    for i = 1, #DeclareWar_Handles do
        if DeclareWar_Handles[i].callback(aIndex) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.DeclareWar(callback)
    table.insert(DeclareWar_Handles, { callback = callback })
end

-- Quando um player usa uma jóia
-- Retorno 1 não permite o uso da joia
local PlayerUseItem_Handles = {}
function PlayerUseItem(aIndex, SourceSlot, TargetSlot)
    for i = 1, #PlayerUseItem_Handles do
        if PlayerUseItem_Handles[i].callback(aIndex, SourceSlot, TargetSlot) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.PlayerUseItem(callback)
    table.insert(PlayerUseItem_Handles, { callback = callback })
end

-- Quando o player fecha o baú
local PlayerVaultClose_Handles = {}
function PlayerVaultClose(aIndex)
    for i = 1, #PlayerVaultClose_Handles do
        PlayerVaultClose_Handles[i].callback(aIndex)
    end
    return 0
end
function GameServerFunctions.PlayerVaultClose(callback)
    table.insert(PlayerVaultClose_Handles, { callback = callback })
end

-- Quando o player envia pedido de TradeX
-- Retorno 1 impede o envio do pedido
local PlayerSendTradeX_Handles = {}
function PlayerSendTradeX(aIndex, TargetIndex)
    for i = 1, #PlayerSendTradeX_Handles do
        if PlayerSendTradeX_Handles[i].callback(aIndex, TargetIndex) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.PlayerSendTradeX(callback)
    table.insert(PlayerSendTradeX_Handles, { callback = callback })
end

-- Quando dois players usando TradeX clicam aceitar
-- Retorno 1 cancela a negociação
local PlayerTradeXOk_Handles = {}
function PlayerTradeXOk(aIndex, TargetIndex)
    for i = 1, #PlayerTradeXOk_Handles do
        if PlayerTradeXOk_Handles[i].callback(aIndex, TargetIndex) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.PlayerTradeXOk(callback)
    table.insert(PlayerTradeXOk_Handles, { callback = callback })
end

-- Quando o player tenta equipar um item no slot Element
-- Retorno 1 impede de equipar
local ElementSlot_Handles = {}
function CanEquipElementSlot(aIndex)
    for i = 1, #ElementSlot_Handles do
        if ElementSlot_Handles[i].callback(aIndex) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.CanEquipElementSlot(callback)
    table.insert(ElementSlot_Handles, { callback = callback })
end

-- Quando o player abre uma lojinha
-- Retorno 1 impede de abrir
local PlayerOpenShop_Handles = {}
function PlayerOpenShop(aIndex)
    for i = 1, #PlayerOpenShop_Handles do
        if PlayerOpenShop_Handles[i].callback(aIndex) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.PlayerOpenShop(callback)
    table.insert(PlayerOpenShop_Handles, { callback = callback })
end

-- Quando o player fecha uma lojinha
-- Retorno 1 impede de fechar
local PlayerCloseShop_Handles = {}
function PlayerCloseShop(aIndex)
    for i = 1, #PlayerCloseShop_Handles do
        if PlayerCloseShop_Handles[i].callback(aIndex) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.PlayerCloseShop(callback)
    table.insert(PlayerCloseShop_Handles, { callback = callback })
end

-- Quando o player tenta equipar um item
-- Retorno 1 impede de equipar
local PlayerCanEquipItem_Handles = {}
function PlayerCanEquipItem(aIndex, SourceSlot, TargetSlot)
    for i = 1, #PlayerCanEquipItem_Handles do
        if PlayerCanEquipItem_Handles[i].callback(aIndex, SourceSlot, TargetSlot) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.PlayerCanEquipItem(callback)
    table.insert(PlayerCanEquipItem_Handles, { callback = callback })
end

-- Packets que vem do cliente
local GameServerProtocol_Handles = {}
function GameServerProtocol(aIndex, Packet, PacketName)
	for i = 1, #GameServerProtocol_Handles do
		if GameServerProtocol_Handles[i].callback(aIndex, Packet, PacketName)
		then
			return
		end
	end

	ClearPacket(PacketName)
end
function GameServerFunctions.GameServerProtocol(callback)
    table.insert(GameServerProtocol_Handles, { callback = callback })
end

-- Recebe o HWID do player quando ele loga
local MacAddressPlayer_Handles = {}
function MacAddressPlayer(aIndex, HWID)
    for i = 1, #MacAddressPlayer_Handles do
        MacAddressPlayer_Handles[i].callback(aIndex, HWID)
    end
end
function GameServerFunctions.MacAddressPlayer(callback)
    table.insert(MacAddressPlayer_Handles, { callback = callback })
end

-- Detectar quando o player está usando a skill da fenrir
-- Caso retorne 1 não deixará quebrar ou reduzir a durabilidade de itens do target
local PlayerAttackFenrirSkill_Handles = {}
function PlayerAttackFenrirSkill(aIndex, TargetIndex)
    for i = 1, #PlayerAttackFenrirSkill_Handles do
        if PlayerAttackFenrirSkill_Handles[i].callback(aIndex, TargetIndex) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.PlayerAttackFenrirSkill(callback)
    table.insert(PlayerAttackFenrirSkill_Handles, { callback = callback })
end

-- Quando um jogador está renascendo através do Type 5
local RespawnUser_Handles = {}
function RespawnUser(aIndex)
    for i = 1, #RespawnUser_Handles do
        RespawnUser_Handles[i].callback(aIndex)
    end
end
function GameServerFunctions.RespawnUser(callback)
    table.insert(RespawnUser_Handles, { callback = callback })
end

-- Quando o sistema da monster Reload
local MonsterReload_Handles = {}
function MonsterReload()
    for i = 1, #MonsterReload_Handles do
        MonsterReload_Handles[i].callback()
    end
end
function GameServerFunctions.MonsterReload(callback)
    table.insert(MonsterReload_Handles, { callback = callback })
end

-- Chaos Machine Genesis
local ChaosMachineGenesis_Handles = {}
function ChaosMachineGenesisMix(aIndex)
    for i = 1, #ChaosMachineGenesis_Handles do
        ChaosMachineGenesis_Handles[i].callback(aIndex)
    end
end
function GameServerFunctions.ChaosMachineGenesisMix(callback)
    table.insert(ChaosMachineGenesis_Handles, { callback = callback })
end

-- Quando executa apenas o reload do lua
local ReloadLuaMonster_Handles = {}
function ReloadLuaMonster()
    for i = 1, #ReloadLuaMonster_Handles do
        ReloadLuaMonster_Handles[i].callback()
    end
end
function GameServerFunctions.ReloadLuaMonster(callback)
    table.insert(ReloadLuaMonster_Handles, { callback = callback })
end

-- Quando o player envia party
local PlayerSendParty_Handles = {}
function PlayerSendParty(aIndex, TargetIndex)
    for i = 1, #PlayerSendParty_Handles do
        if PlayerSendParty_Handles[i].callback(aIndex, TargetIndex) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.PlayerSendParty(callback)
    table.insert(PlayerSendParty_Handles, { callback = callback })
end

-- Quando BC, DS, CC dá reward ao player
local RewardNativeEvents_Handles = {}
function ReceiveEventReward(aIndex, EventType)
    for i = 1, #RewardNativeEvents_Handles do
        if RewardNativeEvents_Handles[i].callback(aIndex, EventType) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.ReceiveEventReward(callback)
    table.insert(RewardNativeEvents_Handles, { callback = callback })
end

-- Sql Async Querys
local QueryAsyncProcess_Handles = {}
function QueryAsyncProcess(queryName, identification, aIndex)
    for i = 1, #QueryAsyncProcess_Handles do
        if QueryAsyncProcess_Handles[i].callback(queryName, identification, aIndex) == 1 then
            return 1
        end
    end
    QueryAsyncDelete(identification)
end
function GameServerFunctions.QueryAsyncProcess(callback)
    table.insert(QueryAsyncProcess_Handles, { callback = callback })
end

-- Funções adicionadas que estavam faltando no seu script

-- Quando o player pega um item do chão
local PlayerPickItem_Handles = {}
function PlayerPickItem(aIndex, index)
    for i = 1, #PlayerPickItem_Handles do
        if PlayerPickItem_Handles[i].callback(aIndex, index) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.PlayerPickItem(callback)
    table.insert(PlayerPickItem_Handles, { callback = callback })
end

-- Quando o player envia mensagem no chat
local ChatProc_Handles = {}
function ChatProc(aIndex, text)
    for i = 1, #ChatProc_Handles do
        if ChatProc_Handles[i].callback(aIndex, text) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.ChatProc(callback)
    table.insert(ChatProc_Handles, { callback = callback })
end

-- Bloqueia envio de party (caso o servidor use essa função separada)
local BlockSendParty_Handles = {}
function BlockSendParty(aIndex, TargetIndex)
    for i = 1, #BlockSendParty_Handles do
        if BlockSendParty_Handles[i].callback(aIndex, TargetIndex) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.BlockSendParty(callback)
    table.insert(BlockSendParty_Handles, { callback = callback })
end

-- Bloqueia recebimento de party
local BlockRecParty_Handles = {}
function BlockRecParty(aIndex, TargetIndex)
    for i = 1, #BlockRecParty_Handles do
        if BlockRecParty_Handles[i].callback(aIndex, TargetIndex) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.BlockRecParty(callback)
    table.insert(BlockRecParty_Handles, { callback = callback })
end

-- Verifica valor da lojinha pessoal
local CheckPersonalShopValue_Handles = {}
function CheckPersonalShopValue(aIndex, ItemIndex, Coin1, Coin2, Coin3, Coin4)
    for i = 1, #CheckPersonalShopValue_Handles do
        local result = CheckPersonalShopValue_Handles[i].callback(aIndex, ItemIndex, Coin1, Coin2, Coin3, Coin4)
        if result == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.CheckPersonalShopValue(callback)
    table.insert(CheckPersonalShopValue_Handles, { callback = callback })
end

-- Diminui valor da lojinha pessoal
local PersonalShopDecreaseValue_Handles = {}
function PersonalShopDecreaseValue(aIndex, Coin1, Coin2, Coin3, Coin4)
    for i = 1, #PersonalShopDecreaseValue_Handles do
        PersonalShopDecreaseValue_Handles[i].callback(aIndex, Coin1, Coin2, Coin3, Coin4)
    end
end
function GameServerFunctions.PersonalShopDecreaseValue(callback)
    table.insert(PersonalShopDecreaseValue_Handles, { callback = callback })
end

-- Adiciona valor da lojinha pessoal
local PersonalShopAddValue_Handles = {}
function PersonalShopAddValue(aIndex, Coin1, Coin2, Coin3, Coin4)
    for i = 1, #PersonalShopAddValue_Handles do
        PersonalShopAddValue_Handles[i].callback(aIndex, Coin1, Coin2, Coin3, Coin4)
    end
end
function GameServerFunctions.PersonalShopAddValue(callback)
    table.insert(PersonalShopAddValue_Handles, { callback = callback })
end

-- Verifica chaos box customizada
local CheckCustomChaosBox_Handles = {}
function CheckCustomChaosBox(aIndex)
    for i = 1, #CheckCustomChaosBox_Handles do
        CheckCustomChaosBox_Handles[i].callback(aIndex)
    end
end
function GameServerFunctions.CheckCustomChaosBox(callback)
    table.insert(CheckCustomChaosBox_Handles, { callback = callback })
end

-- Recebe moedas no TradeX
local TradeXReceiveCoins_Handles = {}
function TradeXReceiveCoins(aIndex, type, value)
    for i = 1, #TradeXReceiveCoins_Handles do
        TradeXReceiveCoins_Handles[i].callback(aIndex, type, value)
    end
end
function GameServerFunctions.TradeXReceiveCoins(callback)
    table.insert(TradeXReceiveCoins_Handles, { callback = callback })
end

-- Abre lojinha pessoal
local PlayerOpenPersonalShop_Handles = {}
function PlayerOpenPersonalShop(aIndex)
    for i = 1, #PlayerOpenPersonalShop_Handles do
        if PlayerOpenPersonalShop_Handles[i].callback(aIndex) == 1 then
            return 1
        end
    end
    return 0
end
function GameServerFunctions.PlayerOpenPersonalShop(callback)
    table.insert(PlayerOpenPersonalShop_Handles, { callback = callback })
end

-- Processo de auto reset do player
local AutoResetPlayerProc_Handles = {}
function AutoResetPlayerProc(aIndex)
    for i = 1, #AutoResetPlayerProc_Handles do
        AutoResetPlayerProc_Handles[i].callback(aIndex)
    end
end
function GameServerFunctions.AutoResetPlayerProc(callback)
    table.insert(AutoResetPlayerProc_Handles, { callback = callback })
end

function CloseLua()
end

return GameServerFunctions