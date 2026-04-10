GuardiaoBrindes = {}

local GuardiaoBrindesImage1 = -1
local GuardiaoBrindesImage2 = -1 

--#region REGISTRA FUN��ES DO SISTEMA
function GuardiaoBrindes.Init()
	InterfaceController.BeforeMainProc(GuardiaoBrindes.Interface)
	InterfaceController.MainProc(GuardiaoBrindes.AfterInterface)
	InterfaceController.ClientProtocol(GuardiaoBrindes.Protocol)
	InterfaceController.MainProcWorldKey(GuardiaoBrindes.KeyListener)
	InterfaceController.InterfaceClickEvent(GuardiaoBrindes.ClickEvent)
	InterfaceController.ScrollMouse(GuardiaoBrindes.ScrollMouse)
	InterfaceController.UpdateMouse(GuardiaoBrindes.UpdateMouse)
	InterfaceController.UpdateKey(GuardiaoBrindes.UpdateKey)

	GuardiaoBrindes.Define()
end
--#endregion

--#region DEFINE VARIAVEIS
function GuardiaoBrindes.Define()
	GuardiaoBrindes_Config.WorkArea = {X1 = 260, X2 = 450, Y1 = 0, Y2 = 429}
	GuardiaoBrindes_Config.WindowOffset = 0
	GuardiaoBrindes_Config.TextArea = {onFocus = false, timer = 0, cursor = "|"}
	GuardiaoBrindes_Config.Codigo = ""	
	GuardiaoBrindes_Config.AlertWindow = {Show = false, Type = 0}	
	GuardiaoBrindes_Config.Lista ={
		Codigo = {},
		Item = {}
	}
	GuardiaoBrindes_Config.ShortLista = {
		Codigo = {},
		Item = {}
	}
	GuardiaoBrindes_Config.ShortListaConfig = {
		Codigo = {Offset = 0, Count = 8, Steps = 0, CurrentStep = 0, Selected = {}},
		Item = {Offset = 0, Count = 7, Steps = 0, CurrentStep = 0, Selected = {}}
	}	
	GuardiaoBrindes_Config.Page = 0
	GuardiaoBrindes_Config.Area = {sx = -100, ex = 450 + GetWideX()*2}
	GuardiaoBrindes_Config.Button = {}
	GuardiaoBrindes_Config.ButtonID = {
		fechar = 1,
		acessar = 2,
		meuscodigos = 3,
		textarea = 4,
		ok = 5
	}
	GuardiaoBrindes_Config.Alfabeto = {
		[48] = "0",[49] = "1",[50] = "2",[51] = "3",[52] = "4",[53] = "5",[54] = "6",[55] = "7",[56] = "8",[57] = "9",[65] = "A",[66] = "B",[67] = "C",[68] = "D",[69] = "E",[70] = "F",[71] = "G",[72] = "H",[73] = "I",[74] = "J",[75] = "K",[76] = "L",[77] = "M",[78] = "N",[79] = "O",[80] = "P",[81] = "Q",[82] = "R",[83] = "S",[84] = "T",[85] = "U",[86] = "V",[87] = "W",[88] = "X",[89] = "Y",[90] = "Z",[96] = "0",[97] = "1",[98] = "2",[99] = "3",[100] = "4",[101] = "5",[102] = "6",[103] = "7",[104] = "8",[105] = "9",[109] = "-",[189] = "-"
	}
end
--#endregion

--#region ABRE A JANELA
function GuardiaoBrindes.Open()
	if	CheckWindowOpen(UIFriendList)		== 1	then	return							end
	if	CheckWindowOpen(UIMoveList)			== 1	then	return							end
	if	CheckWindowOpen(UIParty)			== 1	then	CloseWindow(UIParty)			end
	if	CheckWindowOpen(UIQuest)			== 1	then	return							end
	if	CheckWindowOpen(UIGuild)			== 1	then	CloseWindow(UIGuild)			end
	if	CheckWindowOpen(UIGuildNpc)			== 1	then	CloseWindow(UIGuild)			end
	if	CheckWindowOpen(UITrade)			== 1	then	return							end
	if	CheckWindowOpen(UIWarehouse)		== 1	then	return							end
	if	CheckWindowOpen(UIChaosBox)			== 1	then	return							end
	if	CheckWindowOpen(UICommandWindow)	== 1	then	return							end
	if	CheckWindowOpen(UIPetInfo)			== 1	then	return							end
	if	CheckWindowOpen(UIShop)				== 1	then	return							end
	if	CheckWindowOpen(UIStore)			== 1	then	CloseWindow(UIStore)			end
	if	CheckWindowOpen(UIOtherStore)		== 1	then	return							end
	if	CheckWindowOpen(UICharacter)		== 1	then	CloseWindow(UICharacter)		end
	if	CheckWindowOpen(UIOptions)			== 1	then	return							end
	if	CheckWindowOpen(UIHelp)				== 1	then	return							end
	if	CheckWindowOpen(UIFastDial)			== 1	then	return							end
	if	CheckWindowOpen(UISkillTree)		== 1	then	return							end
	if	CheckWindowOpen(UINPC_Titus)		== 1	then	return							end
	if	CheckWindowOpen(UICashShop)			== 1	then	return							end
	if	CheckWindowOpen(UIFullMap)			== 1	then	return							end
	if	CheckWindowOpen(UINPC_Dialog)		== 1	then	return							end
	if	CheckWindowOpen(UIGensInfo)			== 1	then	CloseWindow(UIGensInfo)			end
	if	CheckWindowOpen(UINPC_Julia)		== 1	then	return							end
	OpenWindow(UIInventory)
	UICustomInterface = GuardiaoBrindes_Config.WindowID
end
--#endregion

--#region FECHA A JANELA
function GuardiaoBrindes.Close()
	CloseWindow(UIInventory)
	UICustomInterface = 0
	GuardiaoBrindes.Define()
	local pname = string.format("2-%s-%d", UserGetName(), UserGetIndex())
	CreatePacket(pname, GuardiaoBrindes_Config.Packet)
	SendPacket(pname)
	ClearPacket(pname)
end
--#endregion

--#region VERIFICA JANELA ABERTA E AREA PROTEGIDA DA TELA
function GuardiaoBrindes.CheckOpen()
	if UICustomInterface == GuardiaoBrindes_Config.WindowID then return true else return false end
end

function GuardiaoBrindes.CheckArea(x)
	if x >= GuardiaoBrindes_Config.Area.sx-GuardiaoBrindes_Config.WindowOffset and x <= GuardiaoBrindes_Config.Area.ex-GuardiaoBrindes_Config.WindowOffset then
		return true
	end
	return false
end
--#endregion

--#region FUN��ES DE BOT�O
function GuardiaoBrindes.CreateButton(ButtonID, x, y, w, h)
	if GuardiaoBrindes_Config.Button[ButtonID] == nil then
		table.insert(GuardiaoBrindes_Config.Button, ButtonID)
		GuardiaoBrindes_Config.Button[ButtonID] = {
			x = x,
			y = y,
			w = w,
			h = h,
			clicked = false,
			timer = 0
		}
	else
		GuardiaoBrindes_Config.Button[ButtonID].x = x
		GuardiaoBrindes_Config.Button[ButtonID].y = y
		GuardiaoBrindes_Config.Button[ButtonID].w = w
		GuardiaoBrindes_Config.Button[ButtonID].h = h
	end
end

function GuardiaoBrindes.ButtonClicked(ButtonID)
	if GuardiaoBrindes_Config.Button[ButtonID] == nil then return false end
	if GuardiaoBrindes_Config.Button[ButtonID].clicked then
		if GuardiaoBrindes_Config.Button[ButtonID].timer >= 2 then
			GuardiaoBrindes_Config.Button[ButtonID].timer = 0
			GuardiaoBrindes_Config.Button[ButtonID].clicked = false
			return false
		end
		GuardiaoBrindes_Config.Button[ButtonID].timer = GuardiaoBrindes_Config.Button[ButtonID].timer + 1
		return true
	end
	return false
end

function GuardiaoBrindes.ButtonMouseOver(ButtonID, x, y)
	if GuardiaoBrindes_Config.Button[ButtonID] == nil then return false end
	if x >= GuardiaoBrindes_Config.Button[ButtonID].x and x <= (GuardiaoBrindes_Config.Button[ButtonID].x+GuardiaoBrindes_Config.Button[ButtonID].w) then
		if y >= GuardiaoBrindes_Config.Button[ButtonID].y and y <= (GuardiaoBrindes_Config.Button[ButtonID].y+GuardiaoBrindes_Config.Button[ButtonID].h) then
			return true
		end
	end
	return false
end
--#endregion

--#region FUN��ES BARRA DE ROLAGEM CODIGOS
function GuardiaoBrindes.GerarScrollBarCodigos()
	GuardiaoBrindes_Config.ShortListaConfig.Codigo.Steps = #GuardiaoBrindes_Config.Lista.Codigo-#GuardiaoBrindes_Config.ShortLista.Codigo
end

function GuardiaoBrindes.GerarListaCodigos()
	local count = 1
	GuardiaoBrindes_Config.ShortLista.Codigo = {}
	for i in ipairs(GuardiaoBrindes_Config.Lista.Codigo) do
		if i > GuardiaoBrindes_Config.ShortListaConfig.Codigo.Offset then
			if count <= GuardiaoBrindes_Config.ShortListaConfig.Codigo.Count then
				table.insert(GuardiaoBrindes_Config.ShortLista.Codigo, GuardiaoBrindes_Config.Lista.Codigo[i])
				count = count + 1
			end
		end
	end

	if GuardiaoBrindes_Config.Page ~= 1 then
		GuardiaoBrindes_Config.Page = 1
	end
end
--#endregion

--#region BARRA DE ROLAGEM ITENS
function GuardiaoBrindes.GerarScrollBarItem(tipo)
	if tipo > 0 then
		GuardiaoBrindes_Config.ShortListaConfig.Item.CurrentStep = 0
	end
	GuardiaoBrindes_Config.ShortListaConfig.Item.Steps = #GuardiaoBrindes_Config.Lista.Item-#GuardiaoBrindes_Config.ShortLista.Item
end

function GuardiaoBrindes.GerarListaItem(tipo)
	local count = 1
	if tipo > 0 then
		GuardiaoBrindes_Config.ShortListaConfig.Item.Offset = 0	
		GuardiaoBrindes_Config.ShortListaConfig.Item.Selected = {}	
	end

	GuardiaoBrindes_Config.ShortLista.Item = {}
	for i in ipairs(GuardiaoBrindes_Config.Lista.Item) do
		if i > GuardiaoBrindes_Config.ShortListaConfig.Item.Offset then
			if count <= GuardiaoBrindes_Config.ShortListaConfig.Item.Count then
				table.insert(GuardiaoBrindes_Config.ShortLista.Item, GuardiaoBrindes_Config.Lista.Item[i])
				count = count + 1
			end
		end
	end
	

	if GuardiaoBrindes_Config.Page ~= 2 then
		GuardiaoBrindes_Config.Page = 2
	end
end
--#endregion

--#region INTERFACE AFTER
function GuardiaoBrindes.AfterInterface()
	if not GuardiaoBrindes.CheckOpen() then return end
	
	--#region Check outras interfaces
	if	CheckWindowOpen(UIInventory) 		== 0	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIFriendList) 		== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIMoveList)			== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIParty) 			== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIQuest) 			== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIGuild) 			== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIGuildNpc) 		== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UITrade) 			== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIWarehouse) 		== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIChaosBox)			== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UICommandWindow) 	== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIPetInfo)	 		== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIShop)				== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIStore) 			== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIOtherStore) 		== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UICharacter) 		== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIOptions) 			== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIHelp)				== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIFastDial)			== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UISkillTree) 		== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UINPC_Titus) 		== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UICashShop)			== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIFullMap) 			== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UINPC_Dialog)		== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIGensInfo)			== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UINPC_Julia)		== 1	then	GuardiaoBrindes.Close()	end
	--#endregion

	EnableAlphaTest()

	--#region PAGINA INICIAL E CODIGOS
	if GuardiaoBrindes_Config.Page == 0 or GuardiaoBrindes_Config.Page == 1 then
		if GuardiaoBrindes_Config.AlertWindow.Show then
			if GuardiaoBrindes_Config.AlertWindow.Type == 0 then
				RenderImage2(31136, 210  + 2*GetWideX(), 70, 220, 90, 0, 0, 0.83, 1, 1, 1, 1.0)
				GuardiaoBrindes.CreateButton(GuardiaoBrindes_Config.ButtonID.ok, 278 + GetWideX()*2, 120, 40, 20)			
				if GuardiaoBrindes.ButtonMouseOver(GuardiaoBrindes_Config.ButtonID.ok, MousePosX(), MousePosY()) then
					RenderImage2(31497, 278 + 2*GetWideX(), 120, 50, 20,0,0, 1.0, 1.0,1,1, 1.0)
				else
					RenderImage2(31496, 278 + 2*GetWideX(), 120, 50, 20,0,0, 1.0, 1.0,1,1, 1.0)
				end
				SetFontType(0)			
				SetTextBg(0, 0, 0, 0)				
				SetTextColor(255, 255, 255, 255)
				RenderText3(302 + 2*GetWideX(), 90, string.format(GuardiaoBrindes_Config.Texts[GetLanguage()][20]), 200, 8)
				RenderText3(302 + 2*GetWideX(), 100, string.format(GuardiaoBrindes_Config.Texts[GetLanguage()][21]), 200, 8)
			end

			if GuardiaoBrindes_Config.AlertWindow.Type == 1 then
				RenderImage2(31136, 210 + (2*GetWideX()), 70, 220, 90, 0, 0, 0.83, 1, 1, 1, 1.0)
				GuardiaoBrindes.CreateButton(GuardiaoBrindes_Config.ButtonID.ok, 278 + GetWideX()*2, 120, 40, 20)			
				if GuardiaoBrindes.ButtonMouseOver(GuardiaoBrindes_Config.ButtonID.ok, MousePosX(), MousePosY()) then
					RenderImage2(31497, 278 + 2*GetWideX(), 120, 50, 20,0,0, 1.0, 1.0,1,1, 1.0)
				else
					RenderImage2(31496, 278 + 2*GetWideX(), 120, 50, 20,0,0, 1.0, 1.0,1,1, 1.0)
				end
				SetFontType(0)			
				SetTextBg(0, 0, 0, 0)				
				SetTextColor(255, 255, 255, 255)
				RenderText3(302 + 2*GetWideX(), 100, string.format(GuardiaoBrindes_Config.Texts[GetLanguage()][23]), 200, 8)
			end
		end
	end
	--#endregion
	DisableAlphaBlend()

	--#region PAGINA ITENS
	if GuardiaoBrindes_Config.Page == 2 then
		DisableAlphaBlend()

		if GuardiaoBrindes_Config.ShortListaConfig.Item.Selected.ListID ~= nil then
			CreateItem(GuardiaoBrindes_Config.ShortListaConfig.Item.Selected.X-GuardiaoBrindes_Config.WindowOffset+2*GetWideX(), GuardiaoBrindes_Config.ShortListaConfig.Item.Selected.Y, GuardiaoBrindes_Config.ShortListaConfig.Item.Selected.W, GuardiaoBrindes_Config.ShortListaConfig.Item.Selected.H, GuardiaoBrindes_Config.ShortListaConfig.Item.Selected.Index, GuardiaoBrindes_Config.ShortListaConfig.Item.Selected.Level, 0, 0, 1)
			EndDrawBar()
			if GuardiaoBrindes_Config.Description then
				if MousePosX() >= GuardiaoBrindes_Config.ShortListaConfig.Item.Selected.X+GetWideX()*2 and MousePosX() <= GuardiaoBrindes_Config.ShortListaConfig.Item.Selected.X+GuardiaoBrindes_Config.ShortListaConfig.Item.Selected.W+GetWideX()*2 then
					if MousePosY() >= GuardiaoBrindes_Config.ShortListaConfig.Item.Selected.Y and MousePosY() <= GuardiaoBrindes_Config.ShortListaConfig.Item.Selected.Y+GuardiaoBrindes_Config.ShortListaConfig.Item.Selected.H then
						SetBlend()
						ShowItemDescription(MousePosX()+100, MousePosY()+90, GuardiaoBrindes_Config.ShortListaConfig.Item.Selected.Index, GuardiaoBrindes_Config.ShortListaConfig.Item.Selected.Level, GuardiaoBrindes_Config.ShortListaConfig.Item.Selected.Skill, GuardiaoBrindes_Config.ShortListaConfig.Item.Selected.Luck, GuardiaoBrindes_Config.ShortListaConfig.Item.Selected.Opt, GuardiaoBrindes_Config.ShortListaConfig.Item.Selected.Dur, GuardiaoBrindes_Config.ShortListaConfig.Item.Selected.Exc, 0)
						EndDrawBar()
					end
				end
			end
		end
		EnableAlphaTest()


		if GuardiaoBrindes_Config.AlertWindow.Show then
			if GuardiaoBrindes_Config.AlertWindow.Type >= 0 then
				RenderImage2(31136, 210 + 2*GetWideX(), 70, 220, 90, 0, 0, 0.83, 1, 1, 1, 1.0)
				GuardiaoBrindes.CreateButton(GuardiaoBrindes_Config.ButtonID.ok, 278 + GetWideX()*2, 120, 40, 20)			
				if GuardiaoBrindes.ButtonMouseOver(GuardiaoBrindes_Config.ButtonID.ok, MousePosX(), MousePosY()) then
					RenderImage2(31497, 278 + 2*GetWideX(), 120, 50, 20,0,0, 1.0, 1.0,1,1, 1.0)
				else
					RenderImage2(31496, 278 + 2*GetWideX(), 120, 50, 20,0,0, 1.0, 1.0,1,1, 1.0)
				end
				SetFontType(0)			
				SetTextBg(0, 0, 0, 0)				
				SetTextColor(255, 255, 255, 255)
				
				if 	GuardiaoBrindes_Config.AlertWindow.Type < 2 then
					RenderText3(302 + 2*GetWideX(), 100, string.format(GuardiaoBrindes_Config.Texts[GetLanguage()][17]), 200, 8)
				end

				if GuardiaoBrindes_Config.AlertWindow.Type == 2 then
					RenderText3(302 + 2*GetWideX(), 100, string.format(GuardiaoBrindes_Config.Texts[GetLanguage()][18]), 200, 8)
				end

				if GuardiaoBrindes_Config.AlertWindow.Type == 3 then
					RenderText3(302 + 2*GetWideX(), 100, string.format(GuardiaoBrindes_Config.Texts[GetLanguage()][24]), 200, 8)
				end
			end
		end
		DisableAlphaBlend()
	end
	--#endregion

end
--#endregion

--#region INTERFACE BEFORE
function GuardiaoBrindes.Interface()						
	if not GuardiaoBrindes.CheckOpen() then return end
	
	--#region Check outras interfaces
	if	CheckWindowOpen(UIInventory) 		== 0	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIFriendList) 		== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIMoveList)			== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIParty) 			== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIQuest) 			== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIGuild) 			== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIGuildNpc) 		== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UITrade) 			== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIWarehouse) 		== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIChaosBox)			== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UICommandWindow) 	== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIPetInfo)	 		== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIShop)				== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIStore) 			== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIOtherStore) 		== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UICharacter) 		== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIOptions) 			== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIHelp)				== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIFastDial)			== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UISkillTree) 		== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UINPC_Titus) 		== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UICashShop)			== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIFullMap) 			== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UINPC_Dialog)		== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UIGensInfo)			== 1	then	GuardiaoBrindes.Close()	end
	if	CheckWindowOpen(UINPC_Julia)		== 1	then	GuardiaoBrindes.Close()	end
	--#endregion

	--#region BASE JANELA
	EnableAlphaTest()
	RenderImage(31319, 260 + (2*GetWideX())-GuardiaoBrindes_Config.WindowOffset, 0, 190, 428)
	RenderImage(31347, 260 + (2*GetWideX())-GuardiaoBrindes_Config.WindowOffset, 0, 190, 64)
	RenderImage(31349, 260 + (2*GetWideX())-GuardiaoBrindes_Config.WindowOffset, 64, 21, 320)
	RenderImage(31350, 429 + (2*GetWideX())-GuardiaoBrindes_Config.WindowOffset, 64, 21, 320)
	RenderImage(31351, 260 + (2*GetWideX())-GuardiaoBrindes_Config.WindowOffset, 384, 190, 45)
	
	SetFontType(1)
	SetTextBg(0, 0, 0, 0)
	SetTextColor(255, 255, 255, 240)
	RenderText3(355 + (2*GetWideX())-GuardiaoBrindes_Config.WindowOffset, 12, string.format(GuardiaoBrindes_Config.Texts[GetLanguage()][1]), 80, 8)
	
	GuardiaoBrindes.CreateButton(GuardiaoBrindes_Config.ButtonID.fechar,470 + GetWideX()*3, 395, 24, 24)

	DisableAlphaBlend()
	--#endregion

	--#region JANELA INICIAL
	if GuardiaoBrindes_Config.Page == 0 then		
		SetBlend()
		glColor4f(0.0, 0.0, 0.0, 0.6)
		DrawBar(276 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 50, 159, 358)
		EndDrawBar()
		
		EnableAlphaTest(1)		
				
		RenderImage2(40000, 273 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 60, 154, 135, 0, 0, 0.92982659, 0.8181264, 1,1, 1.0)
	
		RenderImage(31337, 274 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 50, 14, 14) -- topo esquerdo
		RenderImage(31338, 425 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 50, 14, 14) -- topo direito
		RenderImage(31339, 274 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 400, 14, 14) -- base esquerdo
		RenderImage(31340, 425 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 400, 14, 14) -- base direito

		for i = 1, 137 do
			RenderImage(31341, 287 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset+i, 50, 1, 14) -- horizontal superior
			RenderImage(31342, 287 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset+i, 400, 1, 14) -- horizontal superior
		end

		for i = 1, 340 do
			RenderImage(31343, 274 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 63+i, 14, 1) -- vertical esquerda
			RenderImage(31344, 425 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 63+i, 14, 1) -- vertical direita
		end	
		
		GuardiaoBrindes.CreateButton(GuardiaoBrindes_Config.ButtonID.acessar, 310 + GetWideX()*2-GuardiaoBrindes_Config.WindowOffset, 330, 90, 25)
		RenderImage2(31323, 310 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 330, 110, 25,0, 0, 1.0, 0.2245212, 1, 1, 1.0)
		GuardiaoBrindes.CreateButton(GuardiaoBrindes_Config.ButtonID.meuscodigos, 310 + GetWideX()*2-GuardiaoBrindes_Config.WindowOffset, 368, 90, 25)
		RenderImage2(31323, 310 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 368, 110, 25,0, 0, 1.0, 0.2245212, 1, 1, 1.0)
		GuardiaoBrindes.CreateButton(GuardiaoBrindes_Config.ButtonID.textarea, 300 + GetWideX()*2-GuardiaoBrindes_Config.WindowOffset, 290, 110, 20)
		RenderImage2(31137, 300 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 290, 110, 20,0, 0, 0.6682579, 0.714924204, 1, 1, 1.0)
		
		if not GuardiaoBrindes.ButtonClicked(GuardiaoBrindes_Config.ButtonID.acessar) then
			if GuardiaoBrindes.ButtonMouseOver(GuardiaoBrindes_Config.ButtonID.acessar, MousePosX(), MousePosY()) then
				RenderImage2(31323, 310 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 330, 110, 25,0, 0.2264566, 1.0, 0.2245212, 1, 1, 1.0)
			end
		end

		if not GuardiaoBrindes.ButtonClicked(GuardiaoBrindes_Config.ButtonID.meuscodigos) then
			if GuardiaoBrindes.ButtonMouseOver(GuardiaoBrindes_Config.ButtonID.meuscodigos, MousePosX(), MousePosY()) then
				RenderImage2(31323, 310 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 368, 110, 25,0, 0.2264566, 1.0, 0.2245212, 1, 1, 1.0)
			end
		end

		SetFontType(1)			
		SetTextBg(0, 0, 0, 50)			
		SetTextColor(255, 85, 0, 255)
		RenderText3(355 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 190, GuardiaoBrindes_Config.Texts[GetLanguage()][2], 150, 8)
		
		SetFontType(1)			
		SetTextBg(0, 0, 0, 0)			
		SetTextColor(255, 255, 255, 255)
		RenderText3(355 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 220, GuardiaoBrindes_Config.Texts[GetLanguage()][3], 150, 8)
		RenderText3(355 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 230, GuardiaoBrindes_Config.Texts[GetLanguage()][4], 150, 8)
		RenderText3(355 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 240, GuardiaoBrindes_Config.Texts[GetLanguage()][5], 150, 8)
		
		RenderText3(355 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 260, GuardiaoBrindes_Config.Texts[GetLanguage()][6], 150, 8)
		RenderText3(355 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 270, GuardiaoBrindes_Config.Texts[GetLanguage()][7], 150, 8)
		
		RenderText3(315-GuardiaoBrindes_Config.WindowOffset, 296, "                   ", 80, 1)
		SetTextColor(255, 255, 255, 255)
		if GuardiaoBrindes_Config.TextArea.onFocus then
			if GuardiaoBrindes_Config.TextArea.timer >= 8 then
				if GuardiaoBrindes_Config.TextArea.timer >= 16 then				
					GuardiaoBrindes_Config.TextArea.timer = 0
				end
				RenderText3(312 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 296, GuardiaoBrindes_Config.Codigo, 90, 1)
			else
				RenderText3(312 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 296, GuardiaoBrindes_Config.Codigo..GuardiaoBrindes_Config.TextArea.cursor,90, 1)
			end
			GuardiaoBrindes_Config.TextArea.timer = GuardiaoBrindes_Config.TextArea.timer + 1
		else
			GuardiaoBrindes_Config.TextArea.timer = 0
			RenderText3(312 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 296, GuardiaoBrindes_Config.Codigo, 90, 1)
		end

		RenderText3(355 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 337, GuardiaoBrindes_Config.Texts[GetLanguage()][8], 80, 8)
		RenderText3(355 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 375, GuardiaoBrindes_Config.Texts[GetLanguage()][9], 80, 8)		
		
		DisableAlphaBlend()
	end
	--#endregion

	--#region JANELA MEUS CODIGOS
	if GuardiaoBrindes_Config.Page == 1 then
		SetBlend()
		glColor4f(0.0, 0.0, 0.0, 0.6)
		DrawBar(278 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 50, 155, 84)
		DrawBar(278 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 50, 155, 25)
		DrawBar(278 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 180, 155, 230)
		DrawBar(278 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 180, 155, 20)
		glColor4f(0.8, 0.8, 0.8, 0.1)
		local line = 229
		local c = 1
		while c < #GuardiaoBrindes_Config.ShortLista.Codigo do
			DrawBar(290 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, line, 110, 1)
			line = line+25
			c = c+1
		end
		c = nil
		
		EndDrawBar()
		
		EnableAlphaTest(1)
		--#region CRIA BORDAS
		RenderImage(31337, 274 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 50, 14, 14) -- topo esquerdo
		RenderImage(31338, 425 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 50, 14, 14) -- topo direito
		RenderImage(31339, 274 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 400, 14, 14) -- base esquerdo
		RenderImage(31340, 425 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 400, 14, 14) -- base direito
		RenderImage(31337, 274 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 179, 14, 14) -- topo esquerdo
		RenderImage(31338, 425 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 179, 14, 14) -- topo direito
		RenderImage(31339, 274 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 124, 14, 14) -- base esquerdo
		RenderImage(31340, 425 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 124, 14, 14) -- base direito
		for i = 1, 137 do
			RenderImage(31341, 287 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset+i, 50, 1, 14) -- horizontal superior
			RenderImage(31341, 287 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset+i, 179, 1, 14) -- horizontal superior
			
			RenderImage(31342, 287 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset+i, 124, 1, 14) -- horizontal superior
			RenderImage(31342, 287 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset+i, 400, 1, 14) -- horizontal superior
		end
		for i = 1, 60 do
			RenderImage(31343, 274 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 63+i, 14, 1) -- vertical esquerda
			RenderImage(31344, 425 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 63+i, 14, 1) -- vertical direita
		end	
		for i = 1, 207 do
			RenderImage(31343, 274 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 192+i, 14, 1) -- vertical esquerda
			RenderImage(31344, 425 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 192+i, 14, 1) -- vertical direita
		end
		--#endregion

		
		RenderImage(31269, 413 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset,228, 7, 3) -- topo	
		for i = 230, 350, 15 do
			RenderImage(31270, 413 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, i, 7, 15) -- meio
		end
		RenderImage(31271, 413 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 365, 7, 3) -- baixo
		
		local cursorPos = 215 + (GuardiaoBrindes_Config.ShortListaConfig.Codigo.CurrentStep*(138/(#GuardiaoBrindes_Config.Lista.Codigo-8)))
		if tostring(cursorPos) == "nan" or tostring(cursorPos) == "-nan" then cursorPos = 215 end
		RenderImage(31272, 409 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, cursorPos, 15, 30)

		GuardiaoBrindes.CreateButton(GuardiaoBrindes_Config.ButtonID.acessar, 310 + GetWideX()*2-GuardiaoBrindes_Config.WindowOffset, 146, 90, 24)

		if GuardiaoBrindes_Config.ShortListaConfig.Codigo.Selected.ListID ~= nil then
			if not GuardiaoBrindes.ButtonClicked(GuardiaoBrindes_Config.ButtonID.acessar) then			
				if GuardiaoBrindes.ButtonMouseOver(GuardiaoBrindes_Config.ButtonID.acessar, MousePosX(), MousePosY()) then
					RenderImage2(31323, 310 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 146, 110, 25,0, 0.2264566, 1.0, 0.2245212, 1, 1, 1.0)
				else
					RenderImage2(31323, 310 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 146, 110, 25,0, 0, 1.0, 0.2245212, 1, 1, 1.0)
				end
			else
				RenderImage2(31323, 310 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 146, 110, 25,0, 0, 1.0, 0.2245212, 1, 1, 1.0)
			end
		else
			RenderImage2(31323, 310 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 146, 110, 25,0, 0.4509778, 1.0, 0.2245212, 1, 1, 1.0)
		end
		
		DisableAlphaBlend()
 
		if MousePosX() >= 290 + GetWideX()*2-GuardiaoBrindes_Config.WindowOffset and MousePosX() <= 400 + GetWideX()*2-GuardiaoBrindes_Config.WindowOffset then
			local line = 204
			for i in ipairs(GuardiaoBrindes_Config.ShortLista.Codigo) do
				if MousePosY() >= line+1 and MousePosY() <= line+24 then
					SetBlend()
					glColor4f(1.0, 1.0, 1.0, 0.1)
					DrawBar(290 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, line+2, 110, 22)
					EndDrawBar()
				end
				line = line+25
			end
		end

		EnableAlphaTest(1)

		SetFontType(0)			
		SetTextBg(0, 0, 0, 0)
		if GuardiaoBrindes_Config.ShortListaConfig.Codigo.Selected.ListID ~= nil then
			SetTextColor(255, 255, 255, 255)
		else
			SetTextColor(255, 255, 255, 100)
		end

		RenderText3(355 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 153, GuardiaoBrindes_Config.Texts[GetLanguage()][12], 150, 8)

		if GuardiaoBrindes_Config.ShortListaConfig.Codigo.Selected.ListID ~= nil then
			SetTextColor(255, 85, 0, 255)
			RenderText3(355 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 60, GuardiaoBrindes_Config.ShortListaConfig.Codigo.Selected.Nome, 150, 8)
			SetTextColor(255, 208, 0, 180)
			RenderText3(355 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 79, GuardiaoBrindes_Config.Texts[GetLanguage()][13], 150, 8)
			RenderText3(355 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 109, GuardiaoBrindes_Config.Texts[GetLanguage()][14], 150, 8)
			SetTextColor(255, 255, 255, 255)
			RenderText3(355 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 89, GuardiaoBrindes_Config.ShortListaConfig.Codigo.Selected.Codigo, 150, 8)
			if GuardiaoBrindes_Config.ShortListaConfig.Codigo.Selected.TemValidade == 1 then
				RenderText3(355 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 119, GuardiaoBrindes_Config.ShortListaConfig.Codigo.Selected.Data, 150, 8)
			else
				RenderText3(355 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 119, "-", 30 , 8)
			end
		end
		SetTextColor(255, 85, 0, 255)
		RenderText3(355 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 188, GuardiaoBrindes_Config.Texts[GetLanguage()][11], 150, 8)
		SetTextColor(255, 255, 255, 255)
		local line = 212
		for i in ipairs(GuardiaoBrindes_Config.ShortLista.Codigo) do
			RenderImage2(40001, 290 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, line+2, 5, 5, 0, 0, 0.591827401425252121, 0.591827401425252121, 1, 1, 1.0)
			RenderText3(300 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, line, string.format("%s", GuardiaoBrindes_Config.ShortLista.Codigo[i].Nome), 150, ALIGN_LEFT)
			line = line+25
		end 
	
		DisableAlphaBlend()
	end
	--#endregion
	
	--#region JANELA ITENS	
	if GuardiaoBrindes_Config.Page == 2 then
		SetBlend()
		glColor4f(0.0, 0.0, 0.0, 0.6)
		DrawBar(278 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 50, 155, 114)
		DrawBar(278 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 210, 155, 202)
		DrawBar(278 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 210, 155, 20)
		glColor4f(0.8, 0.8, 0.8, 0.1)

		
		local line = 259
		local c = 1
		while c < #GuardiaoBrindes_Config.ShortLista.Item do
			DrawBar(290 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, line, 110, 1)
			line = line+25
			c = c+1
		end
		c = nil
		
		
		EndDrawBar()

		EnableAlphaTest(1)

		RenderImage(31337, 274 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 50, 14, 14) -- topo esquerdo
		RenderImage(31338, 425 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 50, 14, 14) -- topo direito
		RenderImage(31339, 274 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 154, 14, 14) -- base esquerdo
		RenderImage(31340, 425 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 154, 14, 14) -- base direito
		
		
		RenderImage(31337, 274 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 210, 14, 14) -- topo esquerdo
		RenderImage(31338, 425 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 210, 14, 14) -- topo direito
		RenderImage(31339, 274 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 400, 14, 14) -- base esquerdo
		RenderImage(31340, 425 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 400, 14, 14) -- base direito

		for i = 1, 137 do
			RenderImage(31341, 287 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset+i, 50, 1, 14) -- horizontal superior
			RenderImage(31341, 287 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset+i, 210, 1, 14) -- horizontal superior
			
			RenderImage(31342, 287 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset+i, 154, 1, 14) -- horizontal superior
			RenderImage(31342, 287 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset+i, 400, 1, 14) -- horizontal superior
		end

		for i = 1, 90 do
			RenderImage(31343, 274 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 63+i, 14, 1) -- vertical esquerda
			RenderImage(31344, 425 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 63+i, 14, 1) -- vertical direita
		end	
		for i = 1, 180 do
			RenderImage(31343, 274 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 223+i, 14, 1) -- vertical esquerda
			RenderImage(31344, 425 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 223+i, 14, 1) -- vertical direita
		end

		RenderImage(31269, 413 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset,242, 7, 3) -- topo	
		for i = 245, 365, 15 do
			RenderImage(31270, 413 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, i, 7, 15) -- meio
		end
		RenderImage(31271, 413 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 380, 7, 3) -- baixo
		
		local cursorPos = 230 + (GuardiaoBrindes_Config.ShortListaConfig.Item.CurrentStep*(138/(#GuardiaoBrindes_Config.Lista.Item-7)))
		if tostring(cursorPos) == "nan" or tostring(cursorPos) == "-nan" then cursorPos = 230 end
		RenderImage(31272, 409 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, cursorPos, 15, 30)


		GuardiaoBrindes.CreateButton(GuardiaoBrindes_Config.ButtonID.acessar, 310 + GetWideX()*2-GuardiaoBrindes_Config.WindowOffset, 175, 90, 24)

		if GuardiaoBrindes_Config.ShortListaConfig.Item.Selected.ListID ~= nil then
			if not GuardiaoBrindes.ButtonClicked(GuardiaoBrindes_Config.ButtonID.acessar) then			
				if GuardiaoBrindes.ButtonMouseOver(GuardiaoBrindes_Config.ButtonID.acessar, MousePosX(), MousePosY()) then
					RenderImage2(31323, 310 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 175, 110, 25,0, 0.2264566, 1.0, 0.2245212, 1, 1, 1.0)
				else
					RenderImage2(31323, 310 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 175, 110, 25,0, 0, 1.0, 0.2245212, 1, 1, 1.0)
				end
			else
				RenderImage2(31323, 310 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 175, 110, 25,0, 0, 1.0, 0.2245212, 1, 1, 1.0)
			end
		else
			RenderImage2(31323, 310 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 175, 110, 25,0, 0.4509778, 1.0, 0.2245212, 1, 1, 1.0)
		end

		DisableAlphaBlend()

		if MousePosX() >= 290 + GetWideX()*2-GuardiaoBrindes_Config.WindowOffset and MousePosX() <= 400 + GetWideX()*2-GuardiaoBrindes_Config.WindowOffset then
			local line = 234
			for i in ipairs(GuardiaoBrindes_Config.ShortLista.Item) do
				if MousePosY() >= line+1 and MousePosY() <= line+24 then
					SetBlend()
					glColor4f(1.0, 1.0, 1.0, 0.1)
					DrawBar(290 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, line+2, 110, 22)
					EndDrawBar()
				end
				line = line+25
			end
		end

		EnableAlphaTest()
		
		SetFontType(0)			
		SetTextBg(0, 0, 0, 0)
		if GuardiaoBrindes_Config.ShortListaConfig.Item.Selected.ListID ~= nil then
			SetTextColor(255, 255, 255, 255)
		else
			SetTextColor(255, 255, 255, 100)
		end

		RenderText3(355 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 182, GuardiaoBrindes_Config.Texts[GetLanguage()][15], 150, 8)

		SetTextColor(255, 85, 0, 255)
		RenderText3(355 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, 217, GuardiaoBrindes_Config.Texts[GetLanguage()][16], 150, 8)
		SetTextColor(255, 255, 255, 255)
		local line = 244
		for i in ipairs(GuardiaoBrindes_Config.ShortLista.Item) do
			RenderImage2(GuardiaoBrindesImage2, 290 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, line+2, 5, 5, 0, 0, 0.591827401425252121, 0.591827401425252121, 1, 1, 1.0)
			RenderText3(300 + 2*GetWideX()-GuardiaoBrindes_Config.WindowOffset, line, string.format("%s", GuardiaoBrindes_Config.ShortLista.Item[i].Nome), 150, ALIGN_LEFT)
			line = line+25
		end 
		
		DisableAlphaBlend()
	end
	
	--#endregion

end
--#endregion

--#region DETECTA TECLADO
function GuardiaoBrindes.KeyListener(key)
	if CheckWindowOpen(UIChatWindow) == 1 then return false end

	if not GuardiaoBrindes.CheckOpen() then return false end
		
	if not GuardiaoBrindes_Config.TextArea.onFocus then
		if key == Keys.I or key == Keys.V or key == Keys.Escape then
			GuardiaoBrindes.Close()
			return true
		end
	else
		if key == Keys.Escape then
			GuardiaoBrindes.Close()
			return true
		end
	end

	if GuardiaoBrindes_Config.Page == 0 then
		if GuardiaoBrindes_Config.TextArea.onFocus then
			if CheckWindowOpen(UIChatWindow) ~= 1 then
				if GuardiaoBrindes_Config.Alfabeto[key] ~= nil then
					if string.len(GuardiaoBrindes_Config.Codigo) < 16 then
						GuardiaoBrindes_Config.Codigo = GuardiaoBrindes_Config.Codigo..GuardiaoBrindes_Config.Alfabeto[key]
						GuardiaoBrindes_Config.TextArea.timer = 0
						return true
					end
				end	
			end
			
			if key == Keys.Return then
				if string.len(GuardiaoBrindes_Config.Codigo) ~= 16 then
					--GuardiaoBrindes_Config.TextArea.onFocus = false
					GuardiaoBrindes_Config.Codigo = ""
					SendMessageClient(GuardiaoBrindes_Config.Texts[GetLanguage()][19])
					return true
				end
				GuardiaoBrindes.RequestItens()
			end
		end
	end
	
end
--#endregion

--#region DETECA CLIQUE ESQUERDO MOUSE
function GuardiaoBrindes.ClickEvent()
	if not GuardiaoBrindes.CheckOpen() then return false end
	local x = MousePosX()
	local y = MousePosY()

	if GuardiaoBrindes.ButtonMouseOver(GuardiaoBrindes_Config.ButtonID.fechar, (x+GuardiaoBrindes_Config.WindowOffset), y) then
		GuardiaoBrindes.Close()
		return true
	end

	if GuardiaoBrindes_Config.Page == 0 then
		if GuardiaoBrindes_Config.AlertWindow.Show then
			if GuardiaoBrindes_Config.AlertWindow.Type <= 1 then
				if GuardiaoBrindes.ButtonMouseOver(GuardiaoBrindes_Config.ButtonID.ok, x, y) then
					GuardiaoBrindes_Config.Codigo = ""
					GuardiaoBrindes_Config.AlertWindow.Show = false
					GuardiaoBrindes_Config.AlertWindow.Type = 0			
					return true
				end
			end
			
			return true
		end

		if GuardiaoBrindes.ButtonMouseOver(GuardiaoBrindes_Config.ButtonID.acessar, x, y) then
			if GuardiaoBrindes_Config.TextArea.onFocus then
				GuardiaoBrindes_Config.TextArea.onFocus = false				
			end
			GuardiaoBrindes_Config.Button[GuardiaoBrindes_Config.ButtonID.acessar].clicked = true
			if string.len(GuardiaoBrindes_Config.Codigo) ~= 16 then
				GuardiaoBrindes_Config.Codigo = ""
				LogDebug(GuardiaoBrindes_Config.Texts[GetLanguage()][19])
				return true
			end
			GuardiaoBrindes.RequestItens()
			return true
		end

		if GuardiaoBrindes.ButtonMouseOver(GuardiaoBrindes_Config.ButtonID.meuscodigos, x, y) then
			if GuardiaoBrindes_Config.TextArea.onFocus then
				GuardiaoBrindes_Config.TextArea.onFocus = false
			end
			local pname = string.format("3-%s-%d", UserGetName(), UserGetIndex())
			CreatePacket(pname, GuardiaoBrindes_Config.Packet)
			SendPacket(pname)
			ClearPacket(pname)
			GuardiaoBrindes_Config.Button[GuardiaoBrindes_Config.ButtonID.meuscodigos].clicked = true
			return true
		end

		if GuardiaoBrindes.ButtonMouseOver(GuardiaoBrindes_Config.ButtonID.textarea, x, y) then
			GuardiaoBrindes_Config.TextArea.onFocus = true
			return true
		else
			GuardiaoBrindes_Config.TextArea.onFocus = false
			return true
		end
	end

	if GuardiaoBrindes_Config.Page == 1 then
		if GuardiaoBrindes_Config.AlertWindow.Show then
			if GuardiaoBrindes_Config.AlertWindow.Type == 0 then
				if GuardiaoBrindes.ButtonMouseOver(GuardiaoBrindes_Config.ButtonID.ok, x, y) then
					GuardiaoBrindes_Config.Codigo = ""
					GuardiaoBrindes_Config.AlertWindow.Show = false
					GuardiaoBrindes_Config.AlertWindow.Type = 0
				end
			end
			return true
		end

		if GuardiaoBrindes.ButtonMouseOver(GuardiaoBrindes_Config.ButtonID.acessar, x, y) then
			if GuardiaoBrindes_Config.ShortListaConfig.Codigo.Selected.ListID ~= nil then
				GuardiaoBrindes_Config.Button[GuardiaoBrindes_Config.ButtonID.acessar].clicked = true
				GuardiaoBrindes.RequestItens()
			end
			return true
		end

		if x >= 290 + GetWideX()*2 -GuardiaoBrindes_Config.WindowOffset and x <= 400 + GetWideX()*2 -GuardiaoBrindes_Config.WindowOffset then
			local line = 204
			for i in ipairs(GuardiaoBrindes_Config.ShortLista.Codigo) do
				if y >= line+1 and y <= line+24 then
					GuardiaoBrindes_Config.ShortListaConfig.Codigo.Selected = GuardiaoBrindes_Config.ShortLista.Codigo[i]
					return true
				end
				line = line+25
			end
		end
		
	end

	if GuardiaoBrindes_Config.Page == 2 then
		if GuardiaoBrindes_Config.AlertWindow.Show then
			if GuardiaoBrindes_Config.AlertWindow.Type > 0 then
				if GuardiaoBrindes.ButtonMouseOver(GuardiaoBrindes_Config.ButtonID.ok, x, y) then
					GuardiaoBrindes_Config.AlertWindow.Show = false
					GuardiaoBrindes_Config.AlertWindow.Type = 0
					return true
				end
			end

			if GuardiaoBrindes_Config.AlertWindow.Type == 0 then
				if GuardiaoBrindes.ButtonMouseOver(GuardiaoBrindes_Config.ButtonID.ok, x, y) then
					GuardiaoBrindes.Define()
					return true
				end
			end
			return true
		end

		if GuardiaoBrindes.ButtonMouseOver(GuardiaoBrindes_Config.ButtonID.acessar, x, y) then
			if GuardiaoBrindes_Config.ShortListaConfig.Item.Selected.ListID ~= nil then
				GuardiaoBrindes_Config.Button[GuardiaoBrindes_Config.ButtonID.acessar].clicked = true
				GuardiaoBrindes.RetirarItem()
			end
			return true
		end
		

		if x >= 290 + GetWideX()*2-GuardiaoBrindes_Config.WindowOffset and x <= 400 + GetWideX()*2-GuardiaoBrindes_Config.WindowOffset then
			local line = 234
			for i in ipairs(GuardiaoBrindes_Config.ShortLista.Item) do
				if y >= line+1 and y <= line+24 then
					GuardiaoBrindes_Config.ShortListaConfig.Item.Selected = GuardiaoBrindes_Config.ShortLista.Item[i]
					return true
				end
				line = line+25
			end
		end
		
	end
end
--#endregion

--#region DETECTA SCROLL DO MOUSE
function GuardiaoBrindes.ScrollMouse(Direction)
	local x = MousePosX()
	local y = MousePosY()
	if GuardiaoBrindes.CheckOpen() then
		if GuardiaoBrindes_Config.AlertWindow.Show then return true end

		if GuardiaoBrindes_Config.Page == 1 then
			local area = {x1 = 285 + GetWideX()*2, x2 = 425 + GetWideX()*2, y1 = 205, y2 = 415}
			if x >= area.x1-GuardiaoBrindes_Config.WindowOffset and x <= area.x2-GuardiaoBrindes_Config.WindowOffset and y >= area.y1 and y <= area.y2 then
				if Direction > 0 then
					if GuardiaoBrindes_Config.ShortListaConfig.Codigo.CurrentStep > 0 then
						GuardiaoBrindes_Config.ShortListaConfig.Codigo.CurrentStep = GuardiaoBrindes_Config.ShortListaConfig.Codigo.CurrentStep - 1
						GuardiaoBrindes_Config.ShortListaConfig.Codigo.Offset = GuardiaoBrindes_Config.ShortListaConfig.Codigo.Offset - 1
						GuardiaoBrindes.GerarListaCodigos()
					end
				else
					if GuardiaoBrindes_Config.ShortListaConfig.Codigo.CurrentStep < GuardiaoBrindes_Config.ShortListaConfig.Codigo.Steps then
						GuardiaoBrindes_Config.ShortListaConfig.Codigo.CurrentStep = GuardiaoBrindes_Config.ShortListaConfig.Codigo.CurrentStep + 1
						GuardiaoBrindes_Config.ShortListaConfig.Codigo.Offset = GuardiaoBrindes_Config.ShortListaConfig.Codigo.Offset + 1
						GuardiaoBrindes.GerarListaCodigos()
					end
				end
				return true
			end
		end

		if GuardiaoBrindes_Config.Page == 2 then
			local area = {x1 = 285 + GetWideX()*2, x2 = 425 + GetWideX()*2, y1 = 230, y2 = 415}
			if x >= area.x1-GuardiaoBrindes_Config.WindowOffset and x <= area.x2-GuardiaoBrindes_Config.WindowOffset and y >= area.y1 and y <= area.y2 then
				if Direction > 0 then
					if GuardiaoBrindes_Config.ShortListaConfig.Item.CurrentStep > 0 then
						GuardiaoBrindes_Config.ShortListaConfig.Item.CurrentStep = GuardiaoBrindes_Config.ShortListaConfig.Item.CurrentStep - 1
						GuardiaoBrindes_Config.ShortListaConfig.Item.Offset = GuardiaoBrindes_Config.ShortListaConfig.Item.Offset - 1
						GuardiaoBrindes.GerarListaItem(0)
					end
				else
					if GuardiaoBrindes_Config.ShortListaConfig.Item.CurrentStep < GuardiaoBrindes_Config.ShortListaConfig.Item.Steps then
						GuardiaoBrindes_Config.ShortListaConfig.Item.CurrentStep = GuardiaoBrindes_Config.ShortListaConfig.Item.CurrentStep + 1
						GuardiaoBrindes_Config.ShortListaConfig.Item.Offset = GuardiaoBrindes_Config.ShortListaConfig.Item.Offset + 1
						GuardiaoBrindes.GerarListaItem(0)
					end
				end
				return true
			end
		end
	end
end
--#endregion

--#region RECEBE PACKETS DO SERVIDOR
function GuardiaoBrindes.Protocol(Packet, PacketName)
	if Packet == GuardiaoBrindes_Config.Packet then
		if PacketName == string.format("1-%s-%d", UserGetName(), UserGetIndex()) then
			ClearPacket(PacketName)
			GuardiaoBrindes.Open()
			return true
		end
		
		if PacketName == string.format("4-%s-%d", UserGetName(), UserGetIndex()) then
			GuardiaoBrindes_Config.Lista.Codigo = {}
			local qtd = GetDwordPacket(PacketName, -1)
			for l = 1, qtd do
				local lenCodigo = GetBytePacket(PacketName, -1)
				local codigo = GetCharPacketLength(PacketName, -1, lenCodigo)
				local lenNome = GetBytePacket(PacketName, -1)
				local nomecodigo = GetCharPacketLength(PacketName, -1, lenNome)
				local temvalidade = GetBytePacket(PacketName, -1)
				local data = os.date("%d/%m/%Y %X", GetDwordPacket(PacketName, -1))
				table.insert(GuardiaoBrindes_Config.Lista.Codigo, {ListID = l, Codigo = codigo, Nome = nomecodigo, TemValidade = temvalidade, Data = data})
			end
			ClearPacket(PacketName)
			GuardiaoBrindes.GerarListaCodigos()
			GuardiaoBrindes.GerarScrollBarCodigos()
			return true
		end

		if PacketName == string.format("6-%s-%d", UserGetName(), UserGetIndex()) then
			GuardiaoBrindes_Config.Lista.Item = {}
			local qtd = GetBytePacket(PacketName, -1)
			
			for l = 1, qtd do
				local itemid = GetDwordPacket(PacketName, -1)
				local lenNome = GetBytePacket(PacketName, -1)
				local nomeitem = GetCharPacketLength(PacketName, -1, lenNome)
				local index = GetWordPacket(PacketName, -1)
				local level = GetBytePacket(PacketName, -1)
				local dur = GetBytePacket(PacketName, -1)
				local skill = GetBytePacket(PacketName, -1)
				local luck = GetBytePacket(PacketName, -1)
				local opt = GetBytePacket(PacketName, -1)
				local exc = GetBytePacket(PacketName, -1)
				local x = GetWordPacket(PacketName, -1)
				local y = GetWordPacket(PacketName, -1)
				local w = GetWordPacket(PacketName, -1)
				local h = GetWordPacket(PacketName, -1)
				table.insert(GuardiaoBrindes_Config.Lista.Item, 
				{
					ListID = l,
					ItemID = itemid,
					Nome = nomeitem,
					Index = index,
					Level = level,
					Dur = dur,
					Skill = skill,
					Luck = luck,
					Opt = opt,
					Exc = exc,
					X = x,
					Y = y,
					W = w,
					H = h
				})
			end
			local tipo = GetBytePacket(PacketName, -1)
			ClearPacket(PacketName)
			if tipo > 0 then
				GuardiaoBrindes_Config.AlertWindow.Type = tipo
				GuardiaoBrindes_Config.AlertWindow.Show = true
			end
			GuardiaoBrindes.GerarListaItem(tipo)
			GuardiaoBrindes.GerarScrollBarItem(tipo)
			return true
		end

		if PacketName == string.format("8-%s-%d", UserGetName(), UserGetIndex()) then
			ClearPacket(PacketName)
			GuardiaoBrindes_Config.AlertWindow.Type = 0
			GuardiaoBrindes_Config.AlertWindow.Show = true
			return true
		end

		if PacketName == string.format("9-%s-%d", UserGetName(), UserGetIndex()) then
			ClearPacket(PacketName)
			GuardiaoBrindes_Config.AlertWindow.Type = 1
			GuardiaoBrindes_Config.AlertWindow.Show = true
			return true
		end

		if PacketName == string.format("10-%s-%d", UserGetName(), UserGetIndex()) then
			ClearPacket(PacketName)
			GuardiaoBrindes_Config.AlertWindow.Type = 3
			GuardiaoBrindes_Config.AlertWindow.Show = true
			return true
		end

	end	
end
--#endregion

--#region verifica se o mouse est� sobre a area da janela
function GuardiaoBrindes.UpdateMouse()
	if GuardiaoBrindes_Config.TextArea.onFocus then
		DisableClickClient()
	end
	
	if UICustomInterface == GuardiaoBrindes_Config.WindowID then
		if GuardiaoBrindes.CheckArea(MousePosX()) then
			DisableClickClient()
			return
		end
	end	
	
end
--#endregion

--#region VERIFICA SE A TEXTAREA ESTA EM FOCO
function GuardiaoBrindes.UpdateKey()
	if GuardiaoBrindes_Config.TextArea.onFocus then
		DisableClickClient()

		if CheckWindowOpen(UIChatWindow) ~= 1 then
			if CheckRepeatKey(8) == 1 then
				if string.len(GuardiaoBrindes_Config.Codigo) > 0 then
					GuardiaoBrindes_Config.Codigo = GuardiaoBrindes_Config.Codigo:sub(1, -2)
					GuardiaoBrindes_Config.TextArea.timer = 0
					return
				end
			end
		end
	end	
end
--#endregion

--#region SOLICITA LISTA DE ITENS
function GuardiaoBrindes.RequestItens()
	if GuardiaoBrindes_Config.Page == 1 then
		GuardiaoBrindes_Config.Codigo = GuardiaoBrindes_Config.ShortListaConfig.Codigo.Selected.Codigo
	end

	local pname = string.format("5-%s-%d", UserGetName(), UserGetIndex())
	CreatePacket(pname, GuardiaoBrindes_Config.Packet)
	local len = string.len(GuardiaoBrindes_Config.Codigo)
	SetBytePacket(pname, len)
	SetCharPacketLength(pname, GuardiaoBrindes_Config.Codigo, len)
	SendPacket(pname)
	ClearPacket(pname)
end
--#endregion

--#region RETIRA UM ITEM
function GuardiaoBrindes.RetirarItem()
	local pname = string.format("7-%s-%d", UserGetName(), UserGetIndex())
	CreatePacket(pname, GuardiaoBrindes_Config.Packet)
	local len = string.len(GuardiaoBrindes_Config.Codigo)
	SetBytePacket(pname, len)
	SetCharPacketLength(pname, GuardiaoBrindes_Config.Codigo, len)
	SetDwordPacket(pname, GuardiaoBrindes_Config.ShortListaConfig.Item.Selected.ItemID)
	SendPacket(pname)
	ClearPacket(pname)
end
--#endregion

GuardiaoBrindes.Init()

return GuardiaoBrindes
