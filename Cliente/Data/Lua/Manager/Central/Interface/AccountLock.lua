AccountLock = {}
AccountLock.Open = 0
AccountLock.Mode = 0
AccountLock.InputPwd = ""
AccountLock.InputPwd2 = ""
AccountLock.TextFocus = 0
AccountLock.CheckTrade = 0
AccountLock.CheckWare = 0
AccountLock.CheckMove = 0
AccountLock.CheckPvP = 0
AccountLock.CheckStore = 0
AccountLock.CheckItem = 0
AccountLock.CheckDrop = 0
AccountLock.CheckPick = 0
AccountLock.CheckParty = 0
AccountLock.CheckCommand = 0
AccountLock.LastKeyTime = 0

local IMAGE_ID_REGISTER_SCREEN = 40006
local IMAGE_ID_BUTTON = 40007
local IMAGE_ID_LOGIN_SCREEN = 40008
local IMAGE_ID_SETTINGS_SCREEN = 40009
local IMAGE_ID_CHECKBOX = 40010

local UI = {
    Register = { X = 160 + 1 * GetWideX(), Y = 60, Width = 350, Height = 350 },
    Login = { X = 160 + 1 * GetWideX(), Y = 100, Width = 350, Height = 250 },
    Settings = { X = 160 + 1 * GetWideX(), Y = -10, Width = 300, Height = 460 }
}

local Alfabeto = {
    [48] = "0", [49] = "1", [50] = "2", [51] = "3", [52] = "4",
    [53] = "5", [54] = "6", [55] = "7", [56] = "8", [57] = "9",
    [96] = "0", [97] = "1", [98] = "2", [99] = "3", [100] = "4",
    [101] = "5", [102] = "6", [103] = "7", [104] = "8", [105] = "9"
}

AccountLock.Buttons = {}
AccountLock.ButtonID = {
    Register = 1,
    Login = 2,
    Save = 3
}

function AccountLock.CreateButton(ButtonID, x, y, w, h)
    if AccountLock.Buttons[ButtonID] == nil then
        AccountLock.Buttons[ButtonID] = { x = x, y = y, w = w, h = h, clicked = false, timer = 0 }
    else
        AccountLock.Buttons[ButtonID].x = x
        AccountLock.Buttons[ButtonID].y = y
        AccountLock.Buttons[ButtonID].w = w
        AccountLock.Buttons[ButtonID].h = h
    end
end

function AccountLock.ButtonMouseOver(ButtonID, mx, my)
    local btn = AccountLock.Buttons[ButtonID]
    if not btn then return false end
    return mx >= btn.x and mx <= btn.x + btn.w and my >= btn.y and my <= btn.y + btn.h
end

function AccountLock.ButtonClicked(ButtonID)
    local btn = AccountLock.Buttons[ButtonID]
    if not btn then return false end
    if btn.clicked then
        if btn.timer >= 2 then
            btn.timer = 0
            btn.clicked = false
            return false
        end
        btn.timer = btn.timer + 1
        return true
    end
    return false
end

function AccountLock.GetMask(str)
    local mask = ""
    for i = 1, #str do mask = mask .. "*" end
    return mask
end

function AccountLock.Close()
    AccountLock.Open = 0
    AccountLock.InputPwd = ""
    AccountLock.InputPwd2 = ""
    AccountLock.TextFocus = 0
end

function AccountLock.BlockInterfaces()
    if AccountLock.Open == 0 then return end

    if CheckWindowOpen(UIInventory)     == 1 then CloseWindow(UIInventory)     	 end
    if CheckWindowOpen(UIFriendList)     == 1 then CloseWindow(UIFriendList)     end
    if CheckWindowOpen(UIMoveList)       == 1 then CloseWindow(UIMoveList)       end
    if CheckWindowOpen(UIParty)          == 1 then CloseWindow(UIParty)          end
    if CheckWindowOpen(UIQuest)          == 1 then CloseWindow(UIQuest)          end
    if CheckWindowOpen(UIGuild)          == 1 then CloseWindow(UIGuild)          end
    if CheckWindowOpen(UIGuildNpc)       == 1 then CloseWindow(UIGuildNpc)       end
    if CheckWindowOpen(UITrade)          == 1 then CloseWindow(UITrade)          end
    if CheckWindowOpen(UIWarehouse)      == 1 then CloseWindow(UIWarehouse)      end
    if CheckWindowOpen(UIChaosBox)       == 1 then CloseWindow(UIChaosBox)       end
    if CheckWindowOpen(UICommandWindow)  == 1 then CloseWindow(UICommandWindow)  end
    if CheckWindowOpen(UIPetInfo)        == 1 then CloseWindow(UIPetInfo)        end
    if CheckWindowOpen(UIShop)           == 1 then CloseWindow(UIShop)           end
    if CheckWindowOpen(UIStore)          == 1 then CloseWindow(UIStore)          end
    if CheckWindowOpen(UIOtherStore)     == 1 then CloseWindow(UIOtherStore)     end
    if CheckWindowOpen(UICharacter)      == 1 then CloseWindow(UICharacter)      end
    if CheckWindowOpen(UIOptions)        == 1 then CloseWindow(UIOptions)        end
    if CheckWindowOpen(UIHelp)           == 1 then CloseWindow(UIHelp)           end
    if CheckWindowOpen(UIFastDial)       == 1 then CloseWindow(UIFastDial)       end
    if CheckWindowOpen(UISkillTree)      == 1 then CloseWindow(UISkillTree)      end
    if CheckWindowOpen(UINPC_Titus)      == 1 then CloseWindow(UINPC_Titus)      end
    if CheckWindowOpen(UICashShop)       == 1 then CloseWindow(UICashShop)       end
    if CheckWindowOpen(UIFullMap)        == 1 then CloseWindow(UIFullMap)        end
    if CheckWindowOpen(UINPC_Dialog)     == 1 then CloseWindow(UINPC_Dialog)     end
    if CheckWindowOpen(UIGensInfo)       == 1 then CloseWindow(UIGensInfo)       end
    if CheckWindowOpen(UINPC_Julia)      == 1 then CloseWindow(UINPC_Julia)      end

    if CheckWindowOpen(UIChatWindow) == 1 then
        CloseWindow(UIChatWindow)
    end
end

function AccountLock.MainProc()
    if AccountLock.Open == 0 then return end

    local ui = UI.Register
    if AccountLock.Mode == 1 then ui = UI.Login
    elseif AccountLock.Mode == 2 then ui = UI.Settings end

    local x = ui.X
    local y = ui.Y
    local w = ui.Width
    local h = ui.Height

    SetBlend()
    glColor4f(1.0, 1.0, 1.0, 1.0)

    if AccountLock.Mode == 0 then
        RenderImage2(IMAGE_ID_REGISTER_SCREEN, x, y, w, h, 0, 0, 1.0, 1.0, 1, 1, 1.0)
    elseif AccountLock.Mode == 1 then
        RenderImage2(IMAGE_ID_LOGIN_SCREEN, x + 50, y, w-120, h, 0, 0, 1.0, 0.85, 1, 1, 1.0)
    elseif AccountLock.Mode == 2 then
        RenderImage2(IMAGE_ID_SETTINGS_SCREEN, x-73, y, w+140, h+180, 0, 0, 1.0, 1.0, 1, 1, 1.0)
    end

    SetTextBg(0, 0, 0, 0)

    if AccountLock.Mode == 0 then
        SetFontType(2)
        SetTextColor(255, 215, 0, 255)
        RenderText3(x, y + 85, AccountLock_Config.Title or "", w, 3)
        SetFontType(0)
        SetTextColor(255, 255, 255, 255)
        RenderText3(x, y + 105, AccountLock_Config.RegisterTitle or "", w, 3)
        SetFontType(1)
        SetTextColor(255, 255, 255, 255)
        RenderText3(x + 70, y + 118, AccountLock_Config.RegisterPasswordLabel or "", 100, 1)
        RenderText3(x + 70, y + 163, AccountLock_Config.RegisterRepeatLabel or "", 100, 1)

        SetFontType(2)
        SetTextColor(255, 255, 100, 255)
        if AccountLock.InputPwd ~= "" then
            RenderText3(x + 85, y + 138, AccountLock.GetMask(AccountLock.InputPwd), 180, 1)
        end
        if AccountLock.InputPwd2 ~= "" then
            RenderText3(x + 85, y + 183, AccountLock.GetMask(AccountLock.InputPwd2), 180, 1)
        end

        local btnX = x + (w/2) - 80
        local btnY = y + 215
        AccountLock.CreateButton(AccountLock.ButtonID.Register, btnX, btnY, 160, 35)

        if AccountLock.ButtonClicked(AccountLock.ButtonID.Register) then
            RenderImage2(IMAGE_ID_BUTTON, btnX, btnY-1, 160, 66, 0, 0.50, 1.0, 0.50, 1, 1, 1.0)
        elseif AccountLock.ButtonMouseOver(AccountLock.ButtonID.Register, MousePosX(), MousePosY()) then
            RenderImage2(IMAGE_ID_BUTTON, btnX, btnY-1, 160, 36, 0, 0.20, 1.0, 0.26, 1, 1, 1.0)
        else
            RenderImage2(IMAGE_ID_BUTTON, btnX, btnY-1, 160, 66, 0, 0.50, 1.0, 0.50, 1, 1, 1.0)
        end

        SetFontType(1)
        SetTextColor(255, 255, 255, 255)
        RenderText3(x, y + 225, AccountLock_Config.RegisterButton or "", w, 3)

    elseif AccountLock.Mode == 1 then
        SetFontType(1)
        SetTextColor(255, 215, 0, 255)
        RenderText3(x, y + 85, AccountLock_Config.Title or "", w, 3)
        SetFontType(0)
        SetTextColor(255, 255, 255, 255)
        RenderText3(x, y + 100, AccountLock_Config.LoginTitle or "", w, 3)
        SetFontType(1)
        SetTextColor(255, 255, 0, 255)
        RenderText3(x + 80, y + 114, AccountLock_Config.LoginPasswordLabel or "", 100, 1)

        SetFontType(2)
        SetTextColor(255, 255, 100, 255)
        if AccountLock.InputPwd ~= "" then
            RenderText3(x + 128, y + 132, AccountLock.GetMask(AccountLock.InputPwd), 180, 1)
        end

        local btnX = x + (w/2) - 80
        local btnY = y + 165
        AccountLock.CreateButton(AccountLock.ButtonID.Login, btnX, btnY, 160, 35)

        if AccountLock.ButtonClicked(AccountLock.ButtonID.Login) then
            RenderImage2(IMAGE_ID_BUTTON, btnX-10, btnY, 160, 66, 0, 0.50, 1.0, 0.50, 1, 1, 1.0)
        elseif AccountLock.ButtonMouseOver(AccountLock.ButtonID.Login, MousePosX(), MousePosY()) then
            RenderImage2(IMAGE_ID_BUTTON, btnX-6, btnY, 160, 36, 0, 0.20, 1.0, 0.26, 1, 1, 1.0)
        else
            RenderImage2(IMAGE_ID_BUTTON, btnX-6, btnY, 160, 66, 0, 0.50, 1.0, 0.50, 1, 1, 1.0)
        end

        SetFontType(2)
        SetTextColor(255, 255, 255, 255)
        RenderText3(x-5, y + 168, AccountLock_Config.LoginButton or "", w, 3)

    elseif AccountLock.Mode == 2 then
        SetFontType(2)
        SetTextColor(255, 215, 0, 255)
        RenderText3(x, y + 40, AccountLock_Config.Title or "", w, 3)

        SetFontType(0)
        SetTextColor(255, 255, 255, 255)
        RenderText3(x, y + 65, AccountLock_Config.SettingsTitle or "", w, 3)

        local startY = y + 82
        local checkbox_offset_x = 235
        local lineHeight = 28
        local checkboxSize = 25

        for i = 1, 10 do
            local cy = startY + (i-1) * lineHeight

            GLSwitch()
            GLSwitchBlend()
            SetBlend(1)
            glColor4f(0.4, 0.4, 0.4, 0.5)
            DrawBar(x + 35, cy + lineHeight - 2, 230, 1)
            EndDrawBar()

            SetFontType(1)
            SetTextColor(255, 255, 255, 255)
            RenderText3(x + 40, cy + 7, AccountLock_Config.Options[i] or "", 200, 1)

            local isChecked = 0
            if i == 1 then isChecked = AccountLock.CheckTrade
            elseif i == 2 then isChecked = AccountLock.CheckWare
            elseif i == 3 then isChecked = AccountLock.CheckMove
            elseif i == 4 then isChecked = AccountLock.CheckPvP
            elseif i == 5 then isChecked = AccountLock.CheckStore
            elseif i == 6 then isChecked = AccountLock.CheckItem
            elseif i == 7 then isChecked = AccountLock.CheckDrop
            elseif i == 8 then isChecked = AccountLock.CheckPick
            elseif i == 9 then isChecked = AccountLock.CheckParty
            elseif i == 10 then isChecked = AccountLock.CheckCommand
            end

            local vOffset = isChecked == 1 and 0.41 or 0.0
            RenderImage2(IMAGE_ID_CHECKBOX, x + checkbox_offset_x, cy, checkboxSize+15, checkboxSize, 0.2, vOffset, 1.0, 0.5, 1, 1, 1.0)
        end

        local btnX = x + (w/2) - 80
        local btnY = y + h - 50
        AccountLock.CreateButton(AccountLock.ButtonID.Save, btnX, btnY, 160, 35)

        if AccountLock.ButtonClicked(AccountLock.ButtonID.Save) then
            RenderImage2(IMAGE_ID_BUTTON, btnX-6, btnY-15, 160, 66, 0, 0.50, 1.0, 0.50, 1, 1, 1.0)
        elseif AccountLock.ButtonMouseOver(AccountLock.ButtonID.Save, MousePosX(), MousePosY()) then
            RenderImage2(IMAGE_ID_BUTTON, btnX-6, btnY-15, 160, 36, 0, 0.20, 1.0, 0.26, 1, 1, 1.0)
        else
            RenderImage2(IMAGE_ID_BUTTON, btnX-6, btnY-15, 160, 66, 0, 0.50, 1.0, 0.50, 1, 1, 1.0)
        end

        SetFontType(1)
        SetTextColor(255, 255, 255, 255)
        RenderText3(x-5, y + h - 55, AccountLock_Config.SaveButton or "", w, 3)
    end

    DisableAlphaBlend()
end

function AccountLock.UpdateMouse()
    if AccountLock.Open == 0 then return end

    local ui = UI.Register
    if AccountLock.Mode == 1 then ui = UI.Login
    elseif AccountLock.Mode == 2 then ui = UI.Settings end

    local x = ui.X
    local y = ui.Y
    local w = ui.Width
    local h = ui.Height
    local mx = MousePosX()
    local my = MousePosY()

    if AccountLock.Mode == 0 then
        if mx >= x + 75 and mx <= x + 275 and my >= y + 130 and my <= y + 155 then
            if CheckClickClient() == 1 then DisableClickClient() AccountLock.TextFocus = 1 end
        end
        if mx >= x + 75 and mx <= x + 275 and my >= y + 175 and my <= y + 200 then
            if CheckClickClient() == 1 then DisableClickClient() AccountLock.TextFocus = 2 end
        end
    elseif AccountLock.Mode == 1 then
        if mx >= x + 140 and mx <= x + 300 and my >= y + 100 and my <= y + 125 then
            if CheckClickClient() == 1 then DisableClickClient() AccountLock.TextFocus = 1 end
        end
    end

    if AccountLock.Mode == 0 then
        if AccountLock.ButtonMouseOver(AccountLock.ButtonID.Register, mx, my) and CheckClickClient() == 1 then
            DisableClickClient()
            AccountLock.Buttons[AccountLock.ButtonID.Register].clicked = true
            if AccountLock.InputPwd == AccountLock.InputPwd2 and AccountLock.InputPwd ~= "" then
                CreatePacket("AC_REGISTER_PWD", ACCOUNT_LOCK_PACKET_REGISTER)
                SetCharPacket("AC_REGISTER_PWD", AccountLock.InputPwd)
                SetCharPacket("AC_REGISTER_PWD", AccountLock.InputPwd2)
                SendPacket("AC_REGISTER_PWD")
                ClearPacket("AC_REGISTER_PWD")
                AccountLock.Close()
            end
        end
    end

    if AccountLock.Mode == 1 then
        if mx >= x + 100 and mx <= x + 290 and my >= y + 130 and my <= y + 165 then
            if CheckClickClient() == 1 then DisableClickClient() AccountLock.TextFocus = 1 end
        end
        if AccountLock.ButtonMouseOver(AccountLock.ButtonID.Login, mx, my) and CheckClickClient() == 1 then
            DisableClickClient()
            AccountLock.Buttons[AccountLock.ButtonID.Login].clicked = true
            if AccountLock.InputPwd ~= "" then
                CreatePacket("AC_LOGIN_PWD", ACCOUNT_LOCK_PACKET_LOGIN)
                SetCharPacket("AC_LOGIN_PWD", AccountLock.InputPwd)
                SendPacket("AC_LOGIN_PWD")
                ClearPacket("AC_LOGIN_PWD")
            end
        end
    end

    if AccountLock.Mode == 2 then
        local startY = y + 82
        local checkbox_offset_x = 235
        local lineHeight = 28
        local checkboxSize = 22

        for i = 1, 10 do
            local cy = startY + (i-1) * lineHeight
            local checkX = x + checkbox_offset_x
            local checkY = cy - 4

            if mx >= checkX and mx <= checkX + checkboxSize and my >= checkY and my <= checkY + checkboxSize then
                if CheckClickClient() == 1 then
                    DisableClickClient()
                    if     i == 1 then AccountLock.CheckTrade = 1 - AccountLock.CheckTrade
                    elseif i == 2 then AccountLock.CheckWare  = 1 - AccountLock.CheckWare
                    elseif i == 3 then AccountLock.CheckMove  = 1 - AccountLock.CheckMove
                    elseif i == 4 then AccountLock.CheckPvP   = 1 - AccountLock.CheckPvP
                    elseif i == 5 then AccountLock.CheckStore = 1 - AccountLock.CheckStore
                    elseif i == 6 then AccountLock.CheckItem  = 1 - AccountLock.CheckItem
                    elseif i == 7 then AccountLock.CheckDrop  = 1 - AccountLock.CheckDrop
                    elseif i == 8 then AccountLock.CheckPick  = 1 - AccountLock.CheckPick
                    elseif i == 9 then AccountLock.CheckParty = 1 - AccountLock.CheckParty
                    elseif i == 10 then AccountLock.CheckCommand = 1 - AccountLock.CheckCommand
                    end
                end
            end
        end

        if AccountLock.ButtonMouseOver(AccountLock.ButtonID.Save, mx, my) and CheckClickClient() == 1 then
            DisableClickClient()
            AccountLock.Buttons[AccountLock.ButtonID.Save].clicked = true
            CreatePacket("AC_SAVE_LOCKS", ACCOUNT_LOCK_PACKET_SAVE)
            SetBytePacket("AC_SAVE_LOCKS", AccountLock.CheckTrade)
            SetBytePacket("AC_SAVE_LOCKS", AccountLock.CheckWare)
            SetBytePacket("AC_SAVE_LOCKS", AccountLock.CheckMove)
            SetBytePacket("AC_SAVE_LOCKS", AccountLock.CheckPvP)
            SetBytePacket("AC_SAVE_LOCKS", AccountLock.CheckStore)
            SetBytePacket("AC_SAVE_LOCKS", AccountLock.CheckItem)
            SetBytePacket("AC_SAVE_LOCKS", AccountLock.CheckDrop)
            SetBytePacket("AC_SAVE_LOCKS", AccountLock.CheckPick)
            SetBytePacket("AC_SAVE_LOCKS", AccountLock.CheckParty)
            SetBytePacket("AC_SAVE_LOCKS", AccountLock.CheckCommand)
            SendPacket("AC_SAVE_LOCKS")
            ClearPacket("AC_SAVE_LOCKS")
            AccountLock.Close()
        end
    end

    if mx >= x and mx <= x + w and my >= y and my <= y + h then
        DisableClickClient()
    end
end

function AccountLock.Protocol(Packet, PacketName)
    if string.find(PacketName, "AC_LOCK_OPEN") then
        local mode = GetBytePacket(PacketName, -1) or 0

        if CheckWindowOpen(UIInventory)     == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIFriendList)    == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIMoveList)      == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIParty)         == 1 then CloseWindow(UIParty)    ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIQuest)         == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIGuild)         == 1 then CloseWindow(UIGuild)    ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIGuildNpc)      == 1 then CloseWindow(UIGuildNpc) ClearPacket(PacketName) return true end
        if CheckWindowOpen(UITrade)         == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIWarehouse)     == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIChaosBox)      == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UICommandWindow) == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIPetInfo)       == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIShop)          == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIStore)         == 1 then CloseWindow(UIStore)    ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIOtherStore)    == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UICharacter)     == 1 then CloseWindow(UICharacter)ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIOptions)       == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIHelp)          == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIFastDial)      == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UISkillTree)     == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UINPC_Titus)     == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UICashShop)      == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIFullMap)       == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UINPC_Dialog)    == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIGensInfo)      == 1 then CloseWindow(UIGensInfo) ClearPacket(PacketName) return true end
        if CheckWindowOpen(UINPC_Julia)     == 1 then ClearPacket(PacketName) return true end

        AccountLock.Open = 1
        AccountLock.Mode = mode
        AccountLock.InputPwd = ""
        AccountLock.InputPwd2 = ""
        AccountLock.TextFocus = 0
        ClearPacket(PacketName)
        return true
    end

    if string.find(PacketName, "AC_LOCK_LOAD_SETTINGS") then
        if CheckWindowOpen(UIInventory)     == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIFriendList)    == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIMoveList)      == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIParty)         == 1 then CloseWindow(UIParty)    ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIQuest)         == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIGuild)         == 1 then CloseWindow(UIGuild)    ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIGuildNpc)      == 1 then CloseWindow(UIGuildNpc) ClearPacket(PacketName) return true end
        if CheckWindowOpen(UITrade)         == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIWarehouse)     == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIChaosBox)      == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UICommandWindow) == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIPetInfo)       == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIShop)          == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIStore)         == 1 then CloseWindow(UIStore)    ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIOtherStore)    == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UICharacter)     == 1 then CloseWindow(UICharacter)ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIOptions)       == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIHelp)          == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIFastDial)      == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UISkillTree)     == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UINPC_Titus)     == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UICashShop)      == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIFullMap)       == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UINPC_Dialog)    == 1 then ClearPacket(PacketName) return true end
        if CheckWindowOpen(UIGensInfo)      == 1 then CloseWindow(UIGensInfo) ClearPacket(PacketName) return true end
        if CheckWindowOpen(UINPC_Julia)     == 1 then ClearPacket(PacketName) return true end

        AccountLock.Open = 1
        AccountLock.Mode = 2
        AccountLock.InputPwd = ""
        AccountLock.InputPwd2 = ""
        AccountLock.TextFocus = 0
        AccountLock.CheckTrade = GetBytePacket(PacketName, -1) or 0
        AccountLock.CheckWare  = GetBytePacket(PacketName, -1) or 0
        AccountLock.CheckMove  = GetBytePacket(PacketName, -1) or 0
        AccountLock.CheckPvP   = GetBytePacket(PacketName, -1) or 0
        AccountLock.CheckStore = GetBytePacket(PacketName, -1) or 0
        AccountLock.CheckItem  = GetBytePacket(PacketName, -1) or 0
        AccountLock.CheckDrop  = GetBytePacket(PacketName, -1) or 0
        AccountLock.CheckPick  = GetBytePacket(PacketName, -1) or 0
        AccountLock.CheckParty = GetBytePacket(PacketName, -1) or 0
        AccountLock.CheckCommand = GetBytePacket(PacketName, -1) or 0
        ClearPacket(PacketName)
        return true
    end
    return false
end

function AccountLock.UpdateKey()
    if AccountLock.Open == 0 then return end
    local now = os.clock()

    if CheckPressedKey(Keys.Escape) == 1 then
        AccountLock.Close()
        return
    end

    if CheckPressedKey(8) == 1 and (now - AccountLock.LastKeyTime > 0.05) then
        if AccountLock.TextFocus == 1 then
            AccountLock.InputPwd = string.sub(AccountLock.InputPwd, 1, -2)
        elseif AccountLock.TextFocus == 2 then
            AccountLock.InputPwd2 = string.sub(AccountLock.InputPwd2, 1, -2)
        end
        AccountLock.LastKeyTime = now
        return
    end

    for k, char in pairs(Alfabeto) do
        if CheckPressedKey(k) == 1 and (now - AccountLock.LastKeyTime > 0.1) then
            if (AccountLock.Mode == 0 or AccountLock.Mode == 1) then
                if AccountLock.TextFocus == 1 and #AccountLock.InputPwd < 8 then
                    AccountLock.InputPwd = AccountLock.InputPwd .. char
                elseif AccountLock.TextFocus == 2 and #AccountLock.InputPwd2 < 8 then
                    AccountLock.InputPwd2 = AccountLock.InputPwd2 .. char
                end
            end
            AccountLock.LastKeyTime = now
            return
        end
    end
end

function AccountLock.Init()
    if ACCOUNT_LOCK_SWITCH ~= 1 then return end

    InterfaceController.BeforeMainProc(AccountLock.BlockInterfaces)
    InterfaceController.MainProc(AccountLock.MainProc)
    InterfaceController.ClientProtocol(AccountLock.Protocol)
    InterfaceController.UpdateMouse(AccountLock.UpdateMouse)
    InterfaceController.UpdateKey(AccountLock.UpdateKey)
end

AccountLock.Init()
return AccountLock