-----------------------------------------
	-- Script:		Potion Inifinta
	-- Criador:		Pedro Borges
	-- Skype:		live:ppborges93
	-- WhatsApp:	+55 51 99507-8287
-----------------------------------------

-- true ativa o sistema, false desativa
PotionInfinita_Ativado = true

PotionInfinita_Index = GET_ITEM(14,40)	-- Potion que sera verificado e atualizado no Inventario

-- Itens que ativam o sistema de potion infinita
-- Grupo  do item no item.txt
-- Id do item no item.txt
-- Slot em que o item é usado (caso seja ring, adicionar o item duas vezes, uma para cada slot)
PotionInfinita_Itens = {
{Grupo = 12, Id = 36, Slot = 7}, --Wing Lvl 3 BM
{Grupo = 12, Id = 37, Slot = 7}, --Wing Lvl 3 SM
{Grupo = 12, Id = 38, Slot = 7}, --Wing Lvl 3 ELF
{Grupo = 12, Id = 39, Slot = 7}, --Wing Lvl 3 MG
{Grupo = 12, Id = 40, Slot = 7}, --Cape Lvl 3 DL
{Grupo = 12, Id = 43, Slot = 7}, --Wing Lvl 3 SUM
}