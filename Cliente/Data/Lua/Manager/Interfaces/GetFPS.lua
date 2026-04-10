InterfaceInfos = {}
function InterfaceInfos.Render()
    if CheckWindowOpen(UIFriendList) == 1 then return end
    if CheckWindowOpen(UIMoveList) == 1 then return end
    if CheckWindowOpen(UIParty) == 1 then return end
    if CheckWindowOpen(UIQuest) == 1 then return end
    if CheckWindowOpen(UIGuild) == 1 then return end
    if CheckWindowOpen(UITrade) == 1 then return end
    if CheckWindowOpen(UIWarehouse) == 1 then return end
    if CheckWindowOpen(UIChaosBox) == 1 then return end
    if CheckWindowOpen(UICommandWindow) == 1 then return end
    if CheckWindowOpen(UIPetInfo) == 1 then return end
    if CheckWindowOpen(UIShop) == 1 then return end
    if CheckWindowOpen(UIStore) == 1 then return end
    if CheckWindowOpen(UIOtherStore) == 1 then return end
    if CheckWindowOpen(UIOptions) == 1 then return end
    if CheckWindowOpen(UIHelp) == 1 then return end
    if CheckWindowOpen(UIFastDial) == 1 then return end
    if CheckWindowOpen(UISkillTree) == 1 then return end
    if CheckWindowOpen(UINPC_Titus) == 1 then return end
    if CheckWindowOpen(UINPC_Dialog) == 1 then return end
    if CheckWindowOpen(UINPC_Julia) == 1 then return end
    if CheckWindowOpen(UIInventory) == 1 then return end
    if CheckWindowOpen(UICharacter) == 1 then return end
    
    EnableAlphaTest()
    SetBlend()
    
    local fps = getFPS()
    local r, g, b = 1.0, 1.0, 1.0
    
    if fps >= 45 then
        r, g, b = 0.0, 1.0, 0.0
    elseif fps >= 30 then
        r, g, b = 1.0, 0.65, 0.0
    else
        r, g, b = 1.0, 0.0, 0.0
    end
    
    glColor3f(r, g, b)
    SetFontType(1)
    SetTextBg(0, 0, 0, 0)
    SetTextColor(math.floor(r*255), math.floor(g*255), math.floor(b*255), 255)
    RenderText3(550 + (2*GetWideX()), 1, string.format("FPS: %d", fps), 120, 3)
    
    GLSwitch()
    DisableAlphaBlend()
end

InterfaceController.MainProc(InterfaceInfos.Render)
return InterfaceInfos