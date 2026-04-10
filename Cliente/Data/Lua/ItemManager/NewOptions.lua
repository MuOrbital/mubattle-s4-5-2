local C = { WHITE  = 0, BLUE   = 1, RED    = 2, YELLOW = 3, GREEN  = 4, PURPLE = 5, ORANGE = 6, }

function StartLoadNewOptions()
  for cat = 0, 5 do
    for id = 0, 512 do
      local Katana = GET_ITEM_MODEL(cat, id)
      --NewOptionItem(Katana, 10, "Attack Rate + 1%", "Dano Crítico +1%", 1, 1, true, true)
      --NewOptionItem(Katana, 11, "Attack Rate + 2%", "Dano Crítico +2%", 2, 2, true, true)
      --NewOptionItem(Katana, 12, "Attack Rate + 3%", "Dano Crítico +3%", 4, 4, true, true)
      NewOptionItem(Katana, 13, "Attack Rate + 4%", "Dano Crítico +4%", 6, 6, true, true)
    end
  end
  
  for cat = 6, 11 do
    for id = 0, 512 do
      local Katana = GET_ITEM_MODEL(cat, id)
      --NewOptionItem(Katana, 10, "Aumenta HP +100", "Aumenta Mana +100", 1, 1, true, true)
      --NewOptionItem(Katana, 11, "Aumenta HP +150", "Aumenta Mana +150", 2, 2, true, true)
      --NewOptionItem(Katana, 12, "Aumenta HP +450", "Aumenta Mana +450", 4, 4, true, true)
      NewOptionItem(Katana, 13, "Aumenta HP +700", "Aumenta Mana +700", 6, 6, true, true)
    end
   end
  
  for cat = 12, 12 do
    for id = 36, 40 do
      local Katana = GET_ITEM_MODEL(cat, id)
      --NewOptionItem(Katana, 10, "Aumenta HP +100", "Attack Rate + 1%", 1, 1, true, true)
      --NewOptionItem(Katana, 11, "Aumenta HP +150", "Attack Rate + 2%", 2, 2, true, true)
      --NewOptionItem(Katana, 12, "Aumenta HP +450", "Attack Rate + 3%", 4, 4, true, true)
      NewOptionItem(Katana, 13, "Aumenta HP +700", "Attack Rate + 4%", 6, 6, true, true)
    end
  end
  
  for cat = 12, 12 do
    for id = 43, 43 do
      local Katana = GET_ITEM_MODEL(cat, id)
      --NewOptionItem(Katana, 10, "Aumenta HP +100", "Attack Rate + 1%", 1, 1, true, true)
      --NewOptionItem(Katana, 11, "Aumenta HP +150", "Attack Rate + 2%", 2, 2, true, true)
      --NewOptionItem(Katana, 12, "Aumenta HP +450", "Attack Rate + 3%", 4, 4, true, true)
      NewOptionItem(Katana, 13, "Aumenta HP +700", "Attack Rate + 4%", 6, 6, true, true)
    end
  end
end