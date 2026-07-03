CUSTOM_WORLD_INFO =
{
	{ MapIndex = 85, WorldFolder = 86, Effect = -1, TextIndex = 4014, TerrainExt = 0, ImageName = "ArenaVIP.tga", Mp3Name = "Data//Music//Aida.mp3" },  	--Arena VIP
}

function StartLoadCustomWorld()
	for _, world in ipairs(CUSTOM_WORLD_INFO) do
		LoadCustomWorld(world.MapIndex, world.WorldFolder, world.Effect, world.TextIndex, world.TerrainExt, world.ImageName, world.Mp3Name)
	end
end