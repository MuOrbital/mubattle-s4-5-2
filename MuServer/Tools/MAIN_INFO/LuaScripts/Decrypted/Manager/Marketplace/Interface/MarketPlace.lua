MarketPlace = {}
MarketPlace_Categories = {}
MarketPlace_Coins = {}
MarketPlace_Jewels = {} 
MarketPlace_Balance = {}
MarketPlace_JewelBalance = {}
MarketPlace_Listings = {}
MarketPlace_CurrentCategory = 0
MarketPlace_SelectedListing = nil
MarketPlace_SelectedIndex = 0
MarketPlace_ScrollOffset = 0
MarketPlace_VisibleItems = 0
MarketPlace_Buttons = {}
MarketPlace_CoinNames = {}
MarketPlace_ShowPriceInput = false
MarketPlace_PriceInput = ""
MarketPlace_PriceCoinIndex = 1
MarketPlace_PriceSlot = nil
MarketPlace_ScrollBar = nil
MarketPlace_FilterIndex    = 1
MarketPlace_FilterOpen     = false
MarketPlace_PriceComboOpen = false
MarketPlace_PriceComboScroll = 0
MarketPlace_FilterScroll = 0
local MP_COMBO_MAX_VISIBLE = 7
local MP_FILTER_MAX_VISIBLE = 16
local MP_GRID_COLS    = 3
local MP_CARD_W       = 78
local MP_CARD_H       = 72
local MP_CARD_GAP     = 3
local MP_ITEM_SIZE    = 30

local MP_BLOCKING_WINDOWS = {
    UIFriendList, UIMoveList, UIParty, UIQuest, UIGuild, UITrade,
    UIWarehouse, UIChaosBox, UICommandWindow, UIPetInfo, UIShop,
    UIInventory, UIStore, UIOtherStore, UICharacter, UIOptions,
    UIHelp, UIFastDial, UISkillTree, UINPC_Titus, UICashShop,
    UIFullMap, UINPC_Dialog, UIGensInfo, UINPC_Julia, UIGuildNpc, UINPCGuild
}

local function MP_AnyBlockingWindowOpen()
    for _, wnd in ipairs(MP_BLOCKING_WINDOWS) do
        if CheckWindowOpen(wnd) == 1 then return true end
    end
    return false
end

local function MP_CloseAllBlockingWindows()
    for _, wnd in ipairs(MP_BLOCKING_WINDOWS) do
        if CheckWindowOpen(wnd) == 1 then CloseWindow(wnd) end
    end
end

local function MP_BuildFilterOptions()
    local opts = {}
    opts[#opts + 1] = {label = "Todos",      filterType = "all",      coinIndex = nil}
    opts[#opts + 1] = {label = "Meus Itens", filterType = "owner",    coinIndex = nil}
    for i = 1, #MarketPlace_Coins do
        local coin = MarketPlace_Coins[i]
        opts[#opts + 1] = {
            label      = (coin and coin.Display) or ("Moeda " .. i),
            filterType = "coin",
            coinIndex  = i,
        }
    end
    local coinCount = #MarketPlace_Coins
    for i = 1, #MarketPlace_Jewels do
        local jewel = MarketPlace_Jewels[i]
        opts[#opts + 1] = {
            label      = (jewel and jewel.Display) or ("Joia " .. i),
            filterType = "coin",
            coinIndex  = coinCount + i,
        }
    end
    return opts
end


function MarketPlace.Open()
    if not MarketPlace_Enabled then return end
    if UICustomInterface == MarketPlace_WindowID then return end

    if CheckWindowOpen(UIChatWindow) == 1 then return end

    if MP_AnyBlockingWindowOpen() then return end

    MP_CloseAllBlockingWindows()

    local name = UserGetName()
    CreatePacket("MP_Open_" .. name, MarketPlace_Packet)
    SendPacket("MP_Open_" .. name)
    ClearPacket("MP_Open_" .. name)
end

function MarketPlace.Close()
    UICustomInterface = 0
    MarketPlace_SelectedListing = nil
    MarketPlace_SelectedIndex   = 0
    MarketPlace_ScrollOffset    = 0
    MarketPlace_CurrentCategory = 0
    MarketPlace_Listings        = {}
    MarketPlace_ScrollBar       = nil
    CloseWindow(UIInventory)
end


function MarketPlace.CreateButton(id, x, y, w, h)
    MarketPlace_Buttons[id] = {X1 = x, X2 = x + w, Y1 = y, Y2 = y + h}
end

function MarketPlace.CheckButton(id)
    local btn = MarketPlace_Buttons[id]
    if not btn then return false end
    local mx, my = MousePosX(), MousePosY()
    return (mx >= btn.X1 and mx <= btn.X2 and my >= btn.Y1 and my <= btn.Y2)
end


function MarketPlace.DrawWindow()
    local w = MarketPlace_Window
    local x = w.X + GetWideX()
    local y = w.Y

    EnableAlphaTest()
    RenderImage2(31592, x, y, w.W + 280, w.H + 290, 0, 0, 1.0, 1.0, 1, 1, 1.0)
    DisableAlphaBlend()

    SetFontType(2)
    SetTextBg(0, 0, 0, 1)
    SetTextColor(200, 200, 100, 255)
    RenderText2(x + w.W/2 - 45, y + 22, MarketPlace_Texts[GetLanguage()][1], 90, 3)

    local bxX = x + w.W - 19
    local bxY = y + 17
    MarketPlace.CreateButton(1, bxX, bxY, 22, 22)


    SetFontType(1)
    SetTextBg(0, 0, 0, 0)
    EnableAlphaTest()
    if MarketPlace.CheckButton(1) then
        SetTextBg(0, 0, 0, 0)
        SetTextColor(200, 100, 100, 255)
        RenderText2(bxX - 45, bxY + 10, " CLOSE ", 90, 3)
    else
        SetTextBg(0, 0, 0, 0)
        SetTextColor(200, 200, 100, 255)
        RenderText2(bxX - 45, bxY + 10, " CLOSE ", 90, 3)
    end
    DisableAlphaBlend()
end


function MarketPlace.DrawCategoryButtons()
    local cfg = MarketPlace_CategoryButtons
    local w   = MarketPlace_Window
    local x   = w.X + GetWideX() + cfg.StartX
    local y   = w.Y + cfg.StartY

    for i = 0, 13 do
        local cat = MarketPlace_Categories[i]
        if cat then
            local btnY = y + (i * (cfg.ButtonH + cfg.Gap))

            MarketPlace.CreateButton(100 + i, x, btnY, cfg.ButtonW, cfg.ButtonH)

            EnableAlphaTest()
            if MarketPlace_CurrentCategory == i then
                RenderImage2(31323, x, btnY, cfg.ButtonW, cfg.ButtonH, 0, 0.2264566, 1.0, 0.2245212, 1, 1, 1.0)
            elseif MarketPlace.CheckButton(100 + i) then
                RenderImage2(31323, x, btnY, cfg.ButtonW, cfg.ButtonH, 0, 0.2264566, 1.0, 0.2245212, 1, 1, 1.0)
            else
                RenderImage2(31323, x, btnY, cfg.ButtonW, cfg.ButtonH, 0, 0, 1.0, 0.2245212, 1, 1, 1.0)
            end
            DisableAlphaBlend()

            SetFontType(1)
            SetTextBg(0, 0, 0, 1)
            if MarketPlace_CurrentCategory == i then
                SetTextColor(255, 255, 100, 255)
            else
                SetTextColor(255, 255, 255, 255)
            end
            RenderText(x - 2, btnY + 5, cat.Name, cfg.ButtonW - 6, 3)
        end
    end
end

function MarketPlace.DrawFilters()
    local w        = MarketPlace_Window
    local panelW   = 84
    local coinCount  = #MarketPlace_Coins
    local balanceH = 14 + (coinCount * 12)
    local x        = w.X + GetWideX() + 320
    local y        = w.Y + 57 + balanceH + 4

    local filterOptions = MP_BuildFilterOptions()

    local comboW   = panelW - 4
    local comboH   = 17
    local cbX      = x + 2
    local cbY      = y + 14

    SetFontType(1)
    SetTextBg(0, 0, 0, 0)
    SetTextColor(255, 255, 255, 255)
    RenderText(x + 3, y + 3, "Filtro:", panelW, 0)

    local mx, my = MousePosX(), MousePosY()
    local hoverCombo = (mx >= cbX and mx <= cbX + comboW and my >= cbY and my <= cbY + comboH)

    glColor4f(0.10, 0.10, 0.10, 1)
    DrawBar(cbX,          cbY,           comboW, 1)
    DrawBar(cbX,          cbY + 16.15,  comboW, 1)
    DrawBar(cbX,          cbY,           1,      comboH )
    DrawBar(cbX + comboW, cbY,           1,      comboH )

    if hoverCombo and not MarketPlace_FilterOpen then
        glColor4f(0.10, 0.10, 0.10, 1)
    elseif MarketPlace_FilterOpen then
        glColor4f(0.10, 0.10, 0.10, 1)
    else
        glColor4f(0.10, 0.10, 0.10, 0.8)
    end
    DrawBar(cbX + 1, cbY + 1, comboW - 2, comboH - 2)

    local arrowAreaW = 14
    local arrowAreaX = cbX + comboW - arrowAreaW

    glColor4f(1, 1, 1, 0.4)
    DrawBar(arrowAreaX, cbY + 1, 1, comboH - 2)

    if hoverCombo or MarketPlace_FilterOpen then
        glColor4f(1, 1, 1, 0.4)
    else
        glColor4f(1, 1, 1, 0.4)
    end
    DrawBar(arrowAreaX + 1, cbY + 1, arrowAreaW - 1, comboH - 2)

    local aX  = arrowAreaX + 3
    local aY  = cbY + 5

    glColor4f(0.10, 0.10, 0.10, 0.5)
    if MarketPlace_FilterOpen then
        DrawBar(aX + 3, aY,     2, 1)
        DrawBar(aX + 2, aY + 1, 4, 1)
        DrawBar(aX + 1, aY + 2, 6, 1)
        DrawBar(aX,     aY + 3, 8, 1)
    else
        DrawBar(aX,     aY,     8, 1)
        DrawBar(aX + 1, aY + 1, 6, 1)
        DrawBar(aX + 2, aY + 2, 4, 1)
        DrawBar(aX + 3, aY + 3, 2, 1)
    end

    MarketPlace.CreateButton(200, cbX, cbY, comboW, comboH)

    if MarketPlace_FilterOpen then
        local itemH        = 11
        local totalOpts    = #filterOptions
        local visibleCount = math.min(totalOpts, MP_FILTER_MAX_VISIBLE)
        local dropW        = comboW
        local dropX        = cbX
        local dropY        = cbY + comboH + 1
        local dropH        = visibleCount * itemH + 2

        local maxScroll = math.max(0, totalOpts - visibleCount)
        if MarketPlace_FilterScroll > maxScroll then MarketPlace_FilterScroll = maxScroll end
        if MarketPlace_FilterScroll < 0 then MarketPlace_FilterScroll = 0 end

        glColor4f(0.10, 0.10, 0.10, 0.8)
        DrawBar(dropX + 2, dropY + 2, dropW, dropH)

        glColor4f(0.10, 0.10, 0.10, 0.8)
        DrawBar(dropX, dropY, dropW, dropH)

        glColor4f(0.10, 0.10, 0.10, 0.8)
        DrawBar(dropX,          dropY,          dropW, 1)
        DrawBar(dropX,          dropY + dropH,  dropW, 1)
        DrawBar(dropX,          dropY,          1,     dropH)
        DrawBar(dropX + dropW,  dropY,          1,     dropH)

        if totalOpts > visibleCount then
            local sbW    = 3
            local sbX    = dropX + dropW - sbW - 1
            local sbH    = dropH - 2
            local sbY    = dropY + 1
            local thumbH = math.max(8, math.floor(sbH * visibleCount / totalOpts))
            local thumbY = sbY
            if maxScroll > 0 then
                thumbY = sbY + math.floor((sbH - thumbH) * MarketPlace_FilterScroll / maxScroll)
            end

            glColor4f(0.15, 0.15, 0.15, 0.8)
            DrawBar(sbX, sbY, sbW, sbH)
            glColor4f(0.70, 0.70, 0.70, 1)
            DrawBar(sbX, thumbY, sbW, thumbH)
        end

        for slot = 1, visibleCount do
            local i   = slot + MarketPlace_FilterScroll
            local opt = filterOptions[i]
            if not opt then break end

            local iy = dropY + 1 + (slot - 1) * itemH
            local hoverItem = (mx >= dropX + 1 and mx <= dropX + dropW - 1 and my >= iy and my <= iy + itemH)
            local isSelected = (MarketPlace_FilterIndex == i)

            if isSelected then
                glColor4f(0.06, 0.06, 0.06, 1)
                DrawBar(dropX + 2, iy, dropW - 3, itemH + 2)
            elseif hoverItem then
                glColor4f(1.0, 1.0, 1.0, 0.5)
                DrawBar(dropX + 1, iy, dropW - 3, itemH + 2)
            end

            SetFontType(3)
            SetTextBg(0, 0, 0, 1)

            if isSelected then
                SetTextBg(0, 0, 0, 100)
                SetTextColor(255, 255, 255, 255)
            elseif hoverItem then
                SetTextBg(0, 0, 0, 100)
                SetTextColor(200, 200, 200, 255)
            else
                SetTextBg(0, 0, 0, 100)
                SetTextColor(150, 150, 150, 255)
            end

            RenderText(dropX + 5, iy + 3, opt.label, dropW - 12, 0)

            MarketPlace.CreateButton(200 + i, dropX + 1, iy, dropW - 2, itemH)
        end
    end

    local selLabel = (filterOptions[MarketPlace_FilterIndex] or filterOptions[1]).label

    SetFontType(1)
    SetTextBg(0, 0, 0, 1)
    SetTextColor(255, 255, 255, 255)
    RenderText(cbX + 10, cbY + 4, selLabel, comboW - arrowAreaW - 4, 2)

    SetTextBg(0, 0, 0, 0)
end

function MarketPlace.DrawBalancePanel()
    local w      = MarketPlace_Window
    local panelW = 80
    local coinCount  = #MarketPlace_Coins
    local panelH = 14 + (coinCount * 12)
    local x      = w.X + GetWideX() + 322
    local y      = w.Y + 53

    glColor4f(0.10, 0.10, 0.10, 0.8)
    DrawBar(x, y, panelW, panelH)
    glColor4f(1, 1, 1, 0.4)
    DrawBar(x,          y,           panelW, 1)
    DrawBar(x,          y + panelH,  panelW, 1)
    DrawBar(x,          y,           1,      panelH)
    DrawBar(x + panelW, y,           1,      panelH)

    SetFontType(0)
    SetTextBg(0, 0, 0, 1)
    SetTextColor(255, 255, 255, 255)
    RenderText(x + 6, y + 2, "Saldo:", 60, 3)

    local coinColors = {{255,200,60},{100,255,140},{100,200,255},{255,140,255}}
    for i = 1, coinCount do
        local coin    = MarketPlace_Coins[i]
        if coin and coin.Display then
            local color   = coinColors[i] or {255,255,255}
            local balance = MarketPlace_Balance[i] or 0
            SetFontType(0)
            SetTextBg(0, 0, 0, 0)
            SetTextColor(color[1], color[2], color[3], 255)
            RenderText(x + 10, y + 13 + ((i - 1) * 12), string.format("%s: %d", coin.Display, balance), panelW - 6, 0)
        end
    end
end


function MarketPlace.DrawListings()
    local area = MarketPlace_ListArea
    local w    = MarketPlace_Window

    if not area or not w then return end

    local areaX = w.X + GetWideX() + area.X
    local areaY = w.Y + area.Y

    local cols      = MP_GRID_COLS
    local cardW     = MP_CARD_W
    local cardH     = MP_CARD_H
    local cardGap   = MP_CARD_GAP
    local startX    = areaX + 4
    local startY    = areaY + 4
    local itemSize  = MP_ITEM_SIZE

    local rowsVisible = math.floor((area.H - 8) / (cardH + cardGap))
    if rowsVisible < 1 then rowsVisible = 1 end
    MarketPlace_VisibleItems = rowsVisible * cols

    local filterOptions = MP_BuildFilterOptions()
    local fi  = MarketPlace_FilterIndex or 1
    local opt = filterOptions[fi] or filterOptions[1]

    local filteredListings = {}
    for _, v in ipairs(MarketPlace_Listings) do
        local ok = true

        if opt.filterType == "owner" then
            if not v.IsOwner then ok = false end
        elseif opt.filterType == "coin" then
            if (v.CoinIndex or 1) ~= opt.coinIndex then ok = false end
        end

        if ok then filteredListings[#filteredListings + 1] = v end
    end

    local tooltipX, tooltipY, tooltipListing = nil, nil, nil
    local mx = MousePosX()
    local my = MousePosY()

    for slot = 1, MarketPlace_VisibleItems do
        local col  = (slot - 1) % cols
        local row  = math.floor((slot - 1) / cols)
        local cx   = startX + col * (cardW + cardGap)
        local cy   = startY + row * (cardH + cardGap)

        EnableAlphaTest()
        RenderImage2(31592, cx, cy, cardW + 50, cardH + 63, 0, 0.0064566, 1.0, 1.0, 1, 1, 1.0)
        DisableAlphaBlend()
    end

    for slot = 1, MarketPlace_VisibleItems do
        local listIdx = MarketPlace_ScrollOffset + slot
        local listing = filteredListings[listIdx]

        local col  = (slot - 1) % cols
        local row  = math.floor((slot - 1) / cols)
        local cx   = startX + col * (cardW + cardGap)
        local cy   = startY + row * (cardH + cardGap)

        if listing then
            if mx >= cx and mx <= cx + cardW and my >= cy and my <= cy + cardH then
                tooltipX       = mx
                tooltipY       = my
                tooltipListing = listing
            end

            local modelX = cx + cardW / 2 - itemSize / 2
            local modelY = cy + 38

            if listing.ItemIndexFull and listing.ItemIndexFull > 0 then
                CreateItem(
                    modelX, modelY - 5,
                    itemSize, itemSize,
                    listing.ItemIndexFull,
                    listing.ItemLevel or 0,
                    listing.ItemOpt3  or 0,
                    listing.ItemExc   or 0
                )
            end

            local textY = cy + 20 + itemSize + 1

            local sellerName = listing.SellerName or ""
            if #sellerName > 10 then sellerName = sellerName:sub(1, 9) .. ".." end

            SetFontType(1)
            SetTextBg(0, 0, 0, 1)
            SetTextColor(255, 255, 255, 255)
            RenderText(cx - 18, textY - 41, sellerName, cardW - 4, 3)

            local coinIdx    = listing.CoinIndex or 1
            local coinCount  = #MarketPlace_Coins
            local coinDisplay
            local coinColors2 = {{255,200,60},{100,255,140},{100,200,255},{255,140,255}}
	    
	    
            local cc

            if coinIdx <= coinCount then
                local coin = MarketPlace_Coins[coinIdx]
                coinDisplay = (coin and coin.Display) or "?"
                cc = coinColors2[coinIdx] or {255,200,60}
            else
                local jewelIdx = coinIdx - coinCount
                local jewel = MarketPlace_Jewels[jewelIdx]
                coinDisplay = (jewel and jewel.Display) or "?"
                cc = {220, 160, 255}
            end

            SetFontType(1)
            SetTextBg(0, 0, 0, 0)
            SetTextColor(cc[1], cc[2], cc[3], 255)
            local priceText = string.format("%d %s", listing.Price or 0, coinDisplay)
            RenderText(cx + 2, textY + 13, priceText, cardW - 4, 3)
        end
    end

    if tooltipListing then
        SetBlend()
        MarketPlace.ShowItemTooltip(tooltipX, tooltipY, tooltipListing)
        SetBlend()
        glColor4f(1, 1, 1, 1)
    end
end

function MarketPlace.DrawActionButtons()
    local w  = MarketPlace_Window
    local bx = w.X + GetWideX() + MarketPlace_CategoryButtons.StartX
    local catCount = 0
    for i = 0, 13 do
        if MarketPlace_Categories[i] then catCount = catCount + 1 end
    end
    local cfg  = MarketPlace_CategoryButtons
    local by   = w.Y + cfg.StartY + catCount * (cfg.ButtonH + cfg.Gap) + 5
    local btnW = cfg.ButtonW
    local btnH = cfg.ButtonH

    local hasSelectedItem = (MarketPlace_SelectedListing ~= nil)
    local isOwner = false

    if hasSelectedItem then
        isOwner = MarketPlace_SelectedListing.IsOwner
    end

    local cancelX = bx
    local cancelY = by
    MarketPlace.CreateButton(2, cancelX+195, cancelY, btnW, btnH)

    EnableAlphaTest()
    if hasSelectedItem and isOwner and MarketPlace.CheckButton(2) then
        RenderImage2(31323, cancelX + 195, cancelY, btnW, btnH, 0, 0.2264566, 0.9, 0.2245212, 1, 1, 1.0)
    elseif hasSelectedItem and isOwner then
        RenderImage2(31323, cancelX + 195, cancelY, btnW, btnH, 0, 0, 0.9, 0.2245212, 1, 1, 1.0)
    else
        RenderImage2(31323, cancelX + 195, cancelY, btnW, btnH, 0, 0, 0.9, 0.2245212, 1, 1, 0.5)
    end
    DisableAlphaBlend()

    SetFontType(1)
    SetTextBg(0, 0, 0, 1)
    if hasSelectedItem and isOwner then
        SetTextColor(255, 100, 100, 255)
    else
        SetTextColor(100, 100, 100, 255)
    end
    RenderText(cancelX + 195, cancelY + 5, MarketPlace_Texts[GetLanguage()][5], btnW - 6, 3)

    local buyX = bx
    local buyY = by
    MarketPlace.CreateButton(3, buyX+125, buyY, btnW, btnH)

    EnableAlphaTest()
    if hasSelectedItem and not isOwner and MarketPlace.CheckButton(3) then
        RenderImage2(31323, buyX + 125, buyY, btnW, btnH, 0, 0.2264566, 0.9, 0.2245212, 1, 1, 1.0)
    elseif hasSelectedItem and not isOwner then
        RenderImage2(31323, buyX + 125, buyY, btnW, btnH, 0, 0, 0.9, 0.2245212, 1, 1, 1.0)
    else
        RenderImage2(31323, buyX + 125, buyY, btnW, btnH, 0, 0, 0.9, 0.2245212, 1, 1, 0.5)
    end
    DisableAlphaBlend()

    SetFontType(1)
    SetTextBg(0, 0, 0, 1)
    if hasSelectedItem and not isOwner then
        SetTextColor(200, 255, 200, 255)
    else
        SetTextColor(100, 150, 100, 255)
    end
    RenderText(buyX + 125, buyY + 5, MarketPlace_Texts[GetLanguage()][4], btnW - 6, 3)
end


function MarketPlace.ShowItemTooltip(x, y, listing)
    if not x or not y or not listing then return end

    local itemIndex = listing.ItemIndexFull
    if not itemIndex or itemIndex == 0 then
        local sec = listing.ItemSection or 0
        local idx = listing.ItemIndex2  or 0
        itemIndex = GET_ITEM(sec, idx)
    end
    if not itemIndex then return end

    ShowDescriptionComplete(
        x, y,
        itemIndex,
        listing.ItemLevel   or 0,
        listing.ItemDur     or 254,
        listing.ItemSkill   or 0,
        listing.ItemLuck    or 0,
        listing.ItemOpt3    or 0,
        listing.ItemExc     or 0,
        listing.ItemAncient or 0,
        listing.ItemJoH     or 0,
        listing.ItemOp380   or 0,
        listing.ItemDur     or 255,
        listing.ItemSock1   or 0,
        listing.ItemSock2   or 0,
        listing.ItemSock3   or 0,
        listing.ItemSock4   or 0,
        listing.ItemSock5   or 0
    )
end

function MarketPlace.DrawPriceInput()
    if not MarketPlace_ShowPriceInput then return end

    local lang  = GetLanguage()
    local texts = MarketPlace_Texts[lang]
    local w     = MarketPlace_Window
    local mx, my = MousePosX(), MousePosY()

    local currencyList = {}
    for i = 1, #MarketPlace_Coins do
        local coin = MarketPlace_Coins[i]
        if coin and coin.Enable == 1 then
            currencyList[#currencyList + 1] = {label = coin.Display or "?", CoinIndex = i}
        end
    end
    local coinCount = #MarketPlace_Coins
    for i = 1, #MarketPlace_Jewels do
        local jewel = MarketPlace_Jewels[i]
        if jewel and jewel.Enable == 1 then
            currencyList[#currencyList + 1] = {label = jewel.Display or "?", CoinIndex = coinCount + i}
        end
    end

    local selectedLabel = "?"
    for _, opt in ipairs(currencyList) do
        if opt.CoinIndex == MarketPlace_PriceCoinIndex then
            selectedLabel = opt.label
            break
        end
    end

    local itemH      = 11
    local totalItems = #currencyList
    local visibleMax = MP_COMBO_MAX_VISIBLE
    local visibleCount = math.min(totalItems, visibleMax)
    local dropH      = visibleCount * itemH + 2

    local panelW = 80
    local panelH = 105
    local px = w.X + GetWideX() + math.floor(w.W / 2) - math.floor(panelW / 2) + 155
    local py = w.Y + math.floor(w.H / 2) - math.floor(panelH / 2) + 60

    local comboW = panelW - 24
    local comboH = 17
    local cbX    = px + 12
    local cbY    = py + 68

    local dropX  = cbX
    local dropY  = cbY + comboH + 1

    local maxScroll = math.max(0, totalItems - visibleCount)
    if MarketPlace_PriceComboScroll > maxScroll then MarketPlace_PriceComboScroll = maxScroll end
    if MarketPlace_PriceComboScroll < 0 then MarketPlace_PriceComboScroll = 0 end

    local modalH = panelH
    if MarketPlace_PriceComboOpen then
        modalH = (dropY + dropH + 4) - py
    end

    glColor4f(0.10, 0.10, 0.10, 0.8)
    DrawBar(px - 3, py - 3, panelW + 6, modalH + 6)
    glColor4f(0.10, 0.10, 0.10, 0.8)
    DrawBar(px, py, panelW, modalH)

    glColor4f(0.10, 0.10, 0.10, 0.8)
    DrawBar(px, py, panelW, 24)
    glColor4f(0.10, 0.10, 0.10, 0.8)
    DrawBar(px, py + 24, panelW, 1)

    local btnY = py + 4
    MarketPlace.CreateButton(10, px + 1, btnY, 44, 18)
    MarketPlace.CreateButton(11, px + 41, btnY, 44, 18)

    EnableAlphaTest()
    if MarketPlace.CheckButton(10) then
        RenderImage2(31323, px + 1, btnY, 44, 18, 0, 0.2264566, 1.0, 0.2245212, 1, 1, 1.0)
    else
        RenderImage2(31323, px + 1, btnY, 44, 18, 0, 0, 1.0, 0.2245212, 1, 1, 1.0)
    end
    if MarketPlace.CheckButton(11) then
        RenderImage2(31323, px + 41, btnY, 44, 18, 0, 0.2264566, 1.0, 0.2245212, 1, 1, 1.0)
    else
        RenderImage2(31323, px + 41, btnY, 44, 18, 0, 0, 1.0, 0.2245212, 1, 1, 1.0)
    end
    DisableAlphaBlend()

    SetFontType(1)
    SetTextBg(0, 0, 0, 1)
    SetTextColor(180, 255, 180, 255)
    RenderText(px - 1, btnY + 3, "OK", 44, 3)
    SetTextColor(255, 80, 80, 255)
    RenderText(px + 35, btnY + 3, "Cancel", 48, 3)

    local fieldY  = py + 30
    local fieldW  = panelW - 24
    local fieldH  = 16
    local fieldX  = px + 12

    SetFontType(1)
    SetTextBg(0, 0, 0, 1)
    SetTextColor(255, 255, 255, 255)
    RenderText(fieldX, fieldY - 10, "Valor (max 4 dig.):", panelW, 0)

    glColor4f(0.05, 0.05, 0.05, 1)
    DrawBar(fieldX, fieldY, fieldW, fieldH)

    glColor4f(0.55, 0.42, 0.10, 1)
    DrawBar(fieldX,            fieldY,           fieldW, 1)
    DrawBar(fieldX,            fieldY + fieldH,  fieldW, 1)
    DrawBar(fieldX,            fieldY,           1,      fieldH)
    DrawBar(fieldX + fieldW,   fieldY,           1,      fieldH)

    local displayVal = MarketPlace_PriceInput ~= "" and MarketPlace_PriceInput or "0"
    SetFontType(2)
    SetTextBg(0, 0, 0, 0)
    SetTextColor(255, 220, 100, 255)
    RenderText(fieldX + 4, fieldY + 1, displayVal, fieldW - 6, 0)

    SetFontType(1)
    SetTextBg(0, 0, 0, 1)
    SetTextColor(255, 255, 255, 255)
    RenderText(cbX, cbY - 10, "Moeda / Joia:", comboW, 0)

    local hoverCombo = (mx >= cbX and mx <= cbX + comboW and my >= cbY and my <= cbY + comboH)

    glColor4f(0.55, 0.42, 0.10, 1)
    DrawBar(cbX,          cbY,          comboW, 1)
    DrawBar(cbX,          cbY + comboH, comboW, 1)
    DrawBar(cbX,          cbY,          1,      comboH)
    DrawBar(cbX + comboW, cbY,          1,      comboH)

    if hoverCombo and not MarketPlace_PriceComboOpen then
        glColor4f(0.10, 0.10, 0.10, 0.8)
    elseif MarketPlace_PriceComboOpen then
        glColor4f(0.10, 0.10, 0.10, 0.8)
    else
        glColor4f(0.10, 0.10, 0.10, 0.8)
    end
    DrawBar(cbX + 1, cbY + 1, comboW - 2, comboH - 2)

    local arrowAreaW = 14
    local arrowAreaX = cbX + comboW - arrowAreaW

    glColor4f(0.55, 0.42, 0.10, 0.5)
    DrawBar(arrowAreaX, cbY + 1, 1, comboH - 2)

    if hoverCombo or MarketPlace_PriceComboOpen then
        glColor4f(0.30, 0.23, 0.05, 0.8)
    else
        glColor4f(0.18, 0.14, 0.03, 0.8)
    end
    DrawBar(arrowAreaX + 1, cbY + 1, arrowAreaW - 1, comboH - 2)

    local aX = arrowAreaX + 3
    local aY = cbY + 5
    glColor4f(1.10, 1.10, 1.10, 1)
    if MarketPlace_PriceComboOpen then
        DrawBar(aX + 3, aY,     2, 1)
        DrawBar(aX + 2, aY + 1, 4, 1)
        DrawBar(aX + 1, aY + 2, 6, 1)
        DrawBar(aX,     aY + 3, 8, 1)
    else
        DrawBar(aX,     aY,     8, 1)
        DrawBar(aX + 1, aY + 1, 6, 1)
        DrawBar(aX + 2, aY + 2, 4, 1)
        DrawBar(aX + 3, aY + 3, 2, 1)
    end

    MarketPlace.CreateButton(300, cbX, cbY, comboW, comboH)

    SetFontType(1)
    SetTextBg(0, 0, 0, 0)
    SetTextColor(255, 255, 255, 255)
    local displayText = selectedLabel .. (MarketPlace_PriceComboOpen and "      ▲" or "      ▼")
    RenderText(cbX + 3, cbY + 4, displayText, comboW - arrowAreaW - 4, 0)
    SetTextBg(0, 0, 0, 0)

    if MarketPlace_PriceComboOpen then
        glColor4f(0, 0, 0, 0.6)
        DrawBar(dropX + 2, dropY + 2, comboW, dropH)

        glColor4f(0.10, 0.08, 0.01, 0.98)
        DrawBar(dropX, dropY, comboW, dropH)

        glColor4f(0.65, 0.50, 0.12, 1)
        DrawBar(dropX,          dropY,         comboW, 1)
        DrawBar(dropX,          dropY + dropH, comboW, 1)
        DrawBar(dropX,          dropY,         1,      dropH)
        DrawBar(dropX + comboW, dropY,         1,      dropH)

        if totalItems > visibleCount then
            local sbW    = 3
            local sbX    = dropX + comboW - sbW - 1
            local sbH    = dropH - 2
            local sbY    = dropY + 1
            local thumbH = math.max(8, math.floor(sbH * visibleCount / totalItems))
            local thumbY = sbY
            if maxScroll > 0 then
                thumbY = sbY + math.floor((sbH - thumbH) * MarketPlace_PriceComboScroll / maxScroll)
            end

            glColor4f(0.25, 0.20, 0.05, 0.8)
            DrawBar(sbX, sbY, sbW, sbH)
            glColor4f(0.80, 0.65, 0.20, 1)
            DrawBar(sbX, thumbY, sbW, thumbH)
        end

        for slot = 1, visibleCount do
            local i   = slot + MarketPlace_PriceComboScroll
            local opt = currencyList[i]
            if not opt then break end

            local iy        = dropY + 1 + (slot - 1) * itemH
            local hoverItem = (mx >= dropX + 1 and mx <= dropX + comboW - 1 and my >= iy and my <= iy + itemH)
            local isSelected = (MarketPlace_PriceCoinIndex == opt.CoinIndex)

            if isSelected then
                glColor4f(0.45, 0.34, 0.06, 1)
                DrawBar(dropX + 1, iy, comboW - 2, itemH)
            elseif hoverItem then
                glColor4f(0.28, 0.21, 0.04, 1)
                DrawBar(dropX + 1, iy, comboW - 2, itemH)
            end

            if isSelected then
                glColor4f(1.0, 0.85, 0.30, 1)
                DrawBar(dropX + 3, iy + 4, 1, 6)
                DrawBar(dropX + 4, iy + 5, 1, 4)
                DrawBar(dropX + 5, iy + 6, 1, 2)
            end

            if slot < visibleCount then
                glColor4f(0.35, 0.27, 0.07, 0.4)
                DrawBar(dropX + 8, iy + itemH - 1, comboW - 16, 1)
            end

            SetFontType(1)
            SetTextBg(0, 0, 0, 1)
            if isSelected then
                SetTextColor(255, 255, 255, 255)
            elseif hoverItem then
                SetTextColor(100, 100, 220, 255)
            else
                SetTextColor(255, 255, 255, 255)
            end
            RenderText(dropX + 10, iy + 3, opt.label, comboW - 12, 0)

            MarketPlace.CreateButton(400 + i, dropX + 1, iy, comboW - 2, itemH)
        end
    end
end

function MarketPlace.Draw()
    if UICustomInterface ~= MarketPlace_WindowID then return end

    for _, wnd in ipairs(MP_BLOCKING_WINDOWS) do
        if wnd ~= UIInventory and CheckWindowOpen(wnd) == 1 then
            MarketPlace.Close()
            return
        end
    end

    if CheckWindowOpen(UIInventory) == 0 then
        MarketPlace.Close()
        return
    end

    SetBlend()
    MarketPlace.DrawWindow()
    SetBlend()
    MarketPlace.DrawCategoryButtons()
    SetBlend()
    MarketPlace.DrawActionButtons()
    SetBlend()
    MarketPlace.DrawBalancePanel()
    SetBlend()
    MarketPlace.DrawFilters()
    SetBlend()
    MarketPlace.DrawListings()
    SetBlend()
    MarketPlace.DrawPriceInput()

    DisableAlphaBlend()
end


function MarketPlace.ClickCategory(mx, my)
    for i = 0, 13 do
        if MarketPlace_Categories[i] then
            if MarketPlace.CheckButton(100 + i) then
                if MarketPlace_CurrentCategory ~= i then
                    MarketPlace_CurrentCategory = i
                    MarketPlace_ScrollOffset    = 0
                    MarketPlace_SelectedListing = nil
                    MarketPlace_SelectedIndex   = 0
                    local name = UserGetName()
                    CreatePacket("MP_Category_" .. name, MarketPlace_Packet)
                    SetBytePacket("MP_Category_" .. name, i)
                    SendPacket("MP_Category_" .. name)
                    ClearPacket("MP_Category_" .. name)
                end
                return true
            end
        end
    end
    return false
end

function MarketPlace.ClickListing(mx, my)
    local area  = MarketPlace_ListArea
    local w     = MarketPlace_Window
    local areaX = w.X + GetWideX() + area.X
    local areaY = w.Y + area.Y

    local cols    = MP_GRID_COLS
    local cardW   = MP_CARD_W
    local cardH   = MP_CARD_H
    local cardGap = MP_CARD_GAP
    local startX  = areaX + 4
    local startY  = areaY + 4

    for slot = 1, MarketPlace_VisibleItems do
        local listIdx = MarketPlace_ScrollOffset + slot
        local listing = MarketPlace_Listings[listIdx]
        if listing then
            local col = (slot - 1) % cols
            local row = math.floor((slot - 1) / cols)
            local cx  = startX + col * (cardW + cardGap)
            local cy  = startY + row * (cardH + cardGap)

            if mx >= cx and mx <= cx + cardW and my >= cy and my <= cy + cardH then
                MarketPlace_SelectedIndex   = listIdx
                MarketPlace_SelectedListing = listing
                return true
            end
        end
    end
    return false
end

function MarketPlace.ClickScroll(mx, my)
    if not MarketPlace_ScrollBar then return false end
    local sb = MarketPlace_ScrollBar
    if mx >= sb.X and mx <= sb.X + sb.W and my >= sb.Y and my <= sb.Y + sb.H then
        local total   = sb.Total
        local visible = sb.Visible
        local maxOff  = total - visible
        if maxOff > 0 then
            local rel     = my - sb.Y
            local pct     = rel / sb.H
            local newOff  = math.floor(pct * maxOff)
            if newOff < 0      then newOff = 0      end
            if newOff > maxOff then newOff = maxOff end
            MarketPlace_ScrollOffset = newOff
        end
        return true
    end
    return false
end

function MarketPlace.ClickBuy()
    if MarketPlace_SelectedListing and not MarketPlace_SelectedListing.IsOwner then
        local name = UserGetName()
        CreatePacket("MP_Buy_" .. name, MarketPlace_Packet)
        SetDwordPacket("MP_Buy_" .. name, MarketPlace_SelectedListing.ID)
        SendPacket("MP_Buy_" .. name)
        ClearPacket("MP_Buy_" .. name)
        return true
    end
    return false
end

function MarketPlace.ClickCancel()
    if MarketPlace_SelectedListing and MarketPlace_SelectedListing.IsOwner then
        local name = UserGetName()
        CreatePacket("MP_Cancel_" .. name, MarketPlace_Packet)
        SetDwordPacket("MP_Cancel_" .. name, MarketPlace_SelectedListing.ID)
        SendPacket("MP_Cancel_" .. name)
        ClearPacket("MP_Cancel_" .. name)
        return true
    end
    return false
end

function MarketPlace.ClickEvent()
    if UICustomInterface ~= MarketPlace_WindowID then return end

    if MarketPlace_ShowPriceInput then
        if MarketPlace_PriceComboOpen then
            local currencyList = {}
            for i = 1, #MarketPlace_Coins do
                local coin = MarketPlace_Coins[i]
                if coin and coin.Enable == 1 then
                    currencyList[#currencyList + 1] = {CoinIndex = i}
                end
            end
            local coinCount = #MarketPlace_Coins
            for i = 1, #MarketPlace_Jewels do
                local jewel = MarketPlace_Jewels[i]
                if jewel and jewel.Enable == 1 then
                    currencyList[#currencyList + 1] = {CoinIndex = coinCount + i}
                end
            end
            local totalItems   = #currencyList
            local visibleCount = math.min(totalItems, MP_COMBO_MAX_VISIBLE)
            for slot = 1, visibleCount do
                local i = slot + MarketPlace_PriceComboScroll
                local opt = currencyList[i]
                if opt and MarketPlace.CheckButton(400 + i) then
                    MarketPlace_PriceCoinIndex = opt.CoinIndex
                    MarketPlace_PriceComboOpen = false
                    return
                end
            end
            if not MarketPlace.CheckButton(300) then
                MarketPlace_PriceComboOpen = false
            end
            return
        end

        if MarketPlace.CheckButton(300) then
            MarketPlace_PriceComboOpen = not MarketPlace_PriceComboOpen
            return
        end

        if MarketPlace.CheckButton(10) then
            local price = tonumber(MarketPlace_PriceInput)
            if price and price > 0 then
                MarketPlace.RequestPrice(price, MarketPlace_PriceCoinIndex, MarketPlace_PriceSlot)
            else
                SendMessageClient("Valor invalido!")
            end
            MarketPlace_ShowPriceInput = false
            MarketPlace_PriceInput     = ""
            MarketPlace_PriceComboOpen = false
            return
        end

        if MarketPlace.CheckButton(11) then
            MarketPlace_ShowPriceInput = false
            MarketPlace_PriceInput     = ""
            MarketPlace_PriceComboOpen = false
            return
        end
        return
    end

    if MarketPlace.CheckButton(1) then
        MarketPlace.Close()
        return
    end

    if MarketPlace.CheckButton(4) then
        MarketPlace.Close()
        return
    end

    if MarketPlace.CheckButton(2) then
        MarketPlace.ClickCancel()
        return
    end

    if MarketPlace.CheckButton(3) then
        MarketPlace.ClickBuy()
        return
    end

    local filterOptions = MP_BuildFilterOptions()

    if MarketPlace_FilterOpen then
        local totalOpts    = #filterOptions
        local visibleCount = math.min(totalOpts, MP_FILTER_MAX_VISIBLE)
        for slot = 1, visibleCount do
            local i = slot + MarketPlace_FilterScroll
            if MarketPlace.CheckButton(200 + i) then
                MarketPlace_FilterIndex     = i
                MarketPlace_FilterOpen      = false
                MarketPlace_FilterScroll    = 0
                MarketPlace_ScrollOffset    = 0
                MarketPlace_SelectedListing = nil
                MarketPlace_SelectedIndex   = 0
                return
            end
        end
    end

    if MarketPlace.CheckButton(200) then
        MarketPlace_FilterOpen = not MarketPlace_FilterOpen
        return
    end

    local mx, my = MousePosX(), MousePosY()
    if MarketPlace.ClickCategory(mx, my) then return end
    if MarketPlace.ClickScroll(mx, my)   then return end
    MarketPlace.ClickListing(mx, my)
end

function MarketPlace.ScrollMouse(direction)
    if UICustomInterface ~= MarketPlace_WindowID then return end

    if MarketPlace_ShowPriceInput and MarketPlace_PriceComboOpen then
        local currencyList = {}
        for i = 1, #MarketPlace_Coins do
            currencyList[#currencyList + 1] = {CoinIndex = i}
        end
        local coinCount = #MarketPlace_Coins
        for i = 1, #MarketPlace_Jewels do
            currencyList[#currencyList + 1] = {CoinIndex = coinCount + i}
        end
        local totalItems   = #currencyList
        local visibleCount = math.min(totalItems, MP_COMBO_MAX_VISIBLE)
        local maxScroll    = math.max(0, totalItems - visibleCount)
        if direction > 0 then
            MarketPlace_PriceComboScroll = math.max(0, MarketPlace_PriceComboScroll - 1)
        else
            MarketPlace_PriceComboScroll = math.min(maxScroll, MarketPlace_PriceComboScroll + 1)
        end
        return
    end

    if MarketPlace_FilterOpen then
        local filterOptions = MP_BuildFilterOptions()
        local totalOpts    = #filterOptions
        local visibleCount = math.min(totalOpts, MP_FILTER_MAX_VISIBLE)
        local maxScroll    = math.max(0, totalOpts - visibleCount)
        if direction > 0 then
            MarketPlace_FilterScroll = math.max(0, MarketPlace_FilterScroll - 1)
        else
            MarketPlace_FilterScroll = math.min(maxScroll, MarketPlace_FilterScroll + 1)
        end
        return
    end

    local area  = MarketPlace_ListArea
    local w     = MarketPlace_Window
    local listX = w.X + GetWideX() + area.X
    local listY = w.Y + area.Y
    local mx, my = MousePosX(), MousePosY()

    if mx >= listX and mx <= listX + area.W and my >= listY and my <= listY + area.H then
        local total   = #MarketPlace_Listings
        local visible = MarketPlace_VisibleItems
        local maxOff  = total - visible
        if maxOff > 0 then
            if direction > 0 then
                MarketPlace_ScrollOffset = math.max(0, MarketPlace_ScrollOffset - MP_GRID_COLS)
            else
                MarketPlace_ScrollOffset = math.min(maxOff, MarketPlace_ScrollOffset + MP_GRID_COLS)
            end
        end
    end
end

function MarketPlace.RequestPrice(price, coinIndex, serial)
    local name = UserGetName()
    CreatePacket("MP_Price_" .. name, MarketPlace_Packet)
    SetDwordPacket("MP_Price_" .. name, price)
    SetBytePacket("MP_Price_" .. name, coinIndex)
    SetDwordPacket("MP_Price_" .. name, serial)
    SendPacket("MP_Price_" .. name)
    ClearPacket("MP_Price_" .. name)
end

function MarketPlace.InputPrice(serial)
    MarketPlace_PriceSlot      = serial
    MarketPlace_ShowPriceInput = true
    MarketPlace_PriceInput     = ""
    MarketPlace_PriceCoinIndex = 1
    MarketPlace_PriceComboOpen = false
end

function MarketPlace.Protocol(Packet, PacketName)
    if Packet ~= MarketPlace_Packet then return false end

    local name = UserGetName()

    if PacketName == "MP_Categories_" .. name then
        local catCount = GetBytePacket(PacketName, -1)
        MarketPlace_Categories = {}
        for i = 1, catCount do
            local len     = GetBytePacket(PacketName, -1)
            local catName = ""
            for _ = 1, len do
                catName = catName .. string.char(GetBytePacket(PacketName, -1))
            end
            MarketPlace_Categories[i - 1] = {Name = catName, Index = i - 1}
        end

        local coinCount = GetBytePacket(PacketName, -1)
        MarketPlace_Coins     = {}
        MarketPlace_CoinNames = {}
        for i = 1, coinCount do
            local len      = GetBytePacket(PacketName, -1)
            local coinName = ""
            for _ = 1, len do
                coinName = coinName .. string.char(GetBytePacket(PacketName, -1))
            end
            local enabled = GetBytePacket(PacketName, -1)
            MarketPlace_Coins[i]     = {Display = coinName, Key = coinName, Enable = enabled}
            MarketPlace_CoinNames[i] = coinName
        end

        local jewelCount = GetBytePacket(PacketName, -1)
        MarketPlace_Jewels = {}
        for i = 1, jewelCount do
            local len       = GetBytePacket(PacketName, -1)
            local jewelName = ""
            for _ = 1, len do
                jewelName = jewelName .. string.char(GetBytePacket(PacketName, -1))
            end
            local enabled = GetBytePacket(PacketName, -1)
            MarketPlace_Jewels[i] = {Display = jewelName, Enable = enabled}
        end

        ClearPacket(PacketName)
        UICustomInterface = MarketPlace_WindowID
        OpenWindow(UIInventory)
        return true
    end

    if PacketName == "MP_Balance_" .. name then
        for i = 1, #MarketPlace_Coins do
            MarketPlace_Balance[i] = GetDwordPacket(PacketName, -1)
        end
        for i = 1, #MarketPlace_Jewels do
            MarketPlace_JewelBalance[i] = GetDwordPacket(PacketName, -1)
        end
        ClearPacket(PacketName)
        return true
    end

    if PacketName == "MP_Listings_" .. name then
        local category = GetBytePacket(PacketName, -1)
        local count    = GetWordPacket(PacketName, -1)

        MarketPlace_Listings = {}
        for i = 1, count do
            local listing = {
                ID            = GetDwordPacket(PacketName, -1),
                SellerName    = GetCharPacketLength(PacketName, -1, 10),
                ItemName      = GetCharPacketLength(PacketName, -1, 50),
                ItemLevel     = GetBytePacket(PacketName, -1),
                ItemLuck      = GetBytePacket(PacketName, -1),
                ItemOpt3      = GetBytePacket(PacketName, -1),
                ItemExc       = GetWordPacket(PacketName, -1),
                Price         = GetDwordPacket(PacketName, -1),
                CoinIndex     = GetBytePacket(PacketName, -1),
                ItemSection   = GetBytePacket(PacketName, -1),
                ItemIndex2    = GetWordPacket(PacketName, -1),
                ItemIndexFull = GetDwordPacket(PacketName, -1),
                ItemSkill     = GetBytePacket(PacketName, -1),
                ItemAncient   = GetBytePacket(PacketName, -1),
                ItemJoH       = GetBytePacket(PacketName, -1),
                ItemOp380     = GetBytePacket(PacketName, -1),
                ItemSock1     = GetBytePacket(PacketName, -1),
                ItemSock2     = GetBytePacket(PacketName, -1),
                ItemSock3     = GetBytePacket(PacketName, -1),
                ItemSock4     = GetBytePacket(PacketName, -1),
                ItemSock5     = GetBytePacket(PacketName, -1),
                ItemDur       = GetBytePacket(PacketName, -1),
                IsOwner       = (GetBytePacket(PacketName, -1) == 1),
            }
            MarketPlace_Listings[i] = listing
        end

        MarketPlace_CurrentCategory = category
        MarketPlace_ScrollOffset    = 0
        MarketPlace_SelectedListing = nil
        MarketPlace_SelectedIndex   = 0

        ClearPacket(PacketName)
        return true
    end

    if PacketName == "MP_RequestPrice_" .. name then
        local serial = GetDwordPacket(PacketName, -1)
        ClearPacket(PacketName)
        MarketPlace.InputPrice(serial)
        return true
    end

    return false
end

function MarketPlace.KeyListener(key)
    if MarketPlace_ShowPriceInput then
        if key == Keys.Escape then
            MarketPlace_ShowPriceInput = false
            MarketPlace_PriceInput     = ""
            MarketPlace_PriceComboOpen = false
            return
        end
        if key >= 48 and key <= 57 then
            if #MarketPlace_PriceInput < 4 then
                MarketPlace_PriceInput = MarketPlace_PriceInput .. tostring(key - 48)
            end
            return
        end
        if key >= 96 and key <= 105 then
            if #MarketPlace_PriceInput < 4 then
                MarketPlace_PriceInput = MarketPlace_PriceInput .. tostring(key - 96)
            end
            return
        end
        if key == 8 then
            MarketPlace_PriceInput = MarketPlace_PriceInput:sub(1, -2)
            return
        end
        if key == 13 then
            local price = tonumber(MarketPlace_PriceInput)
            if price and price > 0 then
                MarketPlace.RequestPrice(price, MarketPlace_PriceCoinIndex, MarketPlace_PriceSlot)
            else
                SendMessageClient("Valor invalido!")
            end
            MarketPlace_ShowPriceInput = false
            MarketPlace_PriceInput     = ""
            MarketPlace_PriceComboOpen = false
            return
        end
        return
    end

    if key == Keys.L then
        if CheckWindowOpen(UIChatWindow) == 1 then return end

        if UICustomInterface == MarketPlace_WindowID then
            MarketPlace.Close()
        elseif UICustomInterface == 0 then
            MarketPlace.Open()
        end
        return
    end

    if key == Keys.Escape then
        if UICustomInterface == MarketPlace_WindowID then
            MarketPlace.Close()
        end
    end
end


function MarketPlace.IsMouseInsideWindow()
    local w  = MarketPlace_Window
    local x  = w.X + GetWideX()
    local y  = w.Y
    local mx, my = MousePosX(), MousePosY()
    return (mx >= x and mx <= x + w.W and my >= y and my <= y + w.H + 50)
end

function MarketPlace.UpdateMouse()
    if UICustomInterface == MarketPlace_WindowID then
        if MarketPlace.IsMouseInsideWindow() then
            DisableClickClient()
        end
    end

    if CheckRepeatKey(Keys.MButton) ~= 0 then
        if MarketPlace_Enabled then
            local slot = GetInventoryMouseSlot()
            if slot >= 12 then
                local name = UserGetName()
                CreatePacket("MP_Deposit_" .. name, MarketPlace_Packet)
                SetBytePacket("MP_Deposit_" .. name, slot)
                SendPacket("MP_Deposit_" .. name)
                ClearPacket("MP_Deposit_" .. name)
            end
        end
    end
end

InterfaceController.BeforeMainProc(MarketPlace.Draw)
InterfaceController.MainProcWorldKey(MarketPlace.KeyListener)
InterfaceController.ClientProtocol(MarketPlace.Protocol)
InterfaceController.InterfaceClickEvent(MarketPlace.ClickEvent)
InterfaceController.UpdateMouse(MarketPlace.UpdateMouse)
InterfaceController.ScrollMouse(MarketPlace.ScrollMouse)

return MarketPlace