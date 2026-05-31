CUSTOM_SERVER_NAME = 
{
	{ ServerID = 1, ServerName = "Sala - Easy Principal" },
}

function StartLoadCustomServerName()
	for i = 1, #CUSTOM_SERVER_NAME do
		SetServerName(CUSTOM_SERVER_NAME[i].ServerID, CUSTOM_SERVER_NAME[i].ServerName)
	end
end