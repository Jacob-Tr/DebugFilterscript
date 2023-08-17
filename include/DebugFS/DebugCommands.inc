/*
	Move a player to another player's location.
*/
bool:sendPlayerToPlayer(const playerid, const pplayerid)
{
	if(pplayerid == INVALID_PLAYER_ID || !IsPlayerConnected(pplayerid) || playerid == INVALID_PLAYER_ID || !IsPlayerConnected(playerid)) return false;

	new Float:x, Float:y, Float:z;
	GetPlayerPos(pplayerid, x, y, z);
	SetPlayerPos(playerid, x, y, z);

	return true;
}

/*
	Put player into debug mode.
*/
YCMD:debug(playerid, params[], help)
{
	#pragma unused help
	#pragma unused params

	SetPlayerInDebug(playerid, true);

	SendClientMessage(playerid, COLOR_BLUE, "~ Debug enabled.");
	return 1;
}

/*
	Print current position to screen and console.
*/
YCMD:pos(playerid, params[], help)
{
	#pragma unused help
	#pragma unused params
	
	if(!IsPlayerInDebug(playerid)) return 0;

	new Float:x, Float:y, Float:z, Float:a; // Player position.
	new string[64]; // Output buffer.
	new in_veh = IsPlayerInAnyVehicle(playerid);

	getPlayerPos(playerid, x, y, z, a); // Populate variables.

	format(string, sizeof(string), "~ Position (%s): X: %.2f Y: %.2f Z: %.2f A: %.2f", (in_veh ? "Vehicle" : "Foot"),  x, y, z, a);

	// Produce output.
	SendClientMessage(playerid, COLOR_WHITE, string);
	print(string);

	return 1;
}

/*
	Set your respawn point.
*/
YCMD:updatespawn(playerid, params[], help)
{
	#pragma unused help
	#pragma unused params
	
	if(!IsPlayerInDebug(playerid)) return 0;

	new Float:x, Float:y, Float:z, Float:a;

	getPlayerPos(playerid, x, y, z, a); // Populate variables.

	SetSpawnInfo(playerid, -1, GetPlayerSkin(playerid), x, y, z, a, 0, 0, 0, 0, 0, 0);

	// Reserve buffer and format output & send it.
	new string[64];
	format(string, sizeof(string), "~ Spawn moved to X: %.2f, Y: %.2f, Z: %.2f.", x, y, z);
	SendClientMessage(playerid, COLOR_WHITE, string);

	return 1;
}

/*
	Move yourself to another player.
*/
YCMD:goto(playerid, params[], help)
{
	#pragma unused help

	if(!IsPlayerInDebug(playerid)) return 0;

	new index = 0, pplayerid = -1;

	if(!getNumericParam(params, index, pplayerid)) return SendClientMessage(playerid, COLOR_RED, "Usage: /goto [Player ID]");
	if(!sendPlayerToPlayer(playerid, pplayerid)) return SendClientMessage(playerid, COLOR_RED, "Error: Invalid player ID.");

	return 1;
}


/*
	Create a vehicle.
*/
YCMD:v(playerid, params[], help)
{
	#pragma unused help

	if(!IsPlayerInDebug(playerid)) return 0;
	new index = 0, modelid = -1, color1 = -1, color2 = -1;
	new param[255] = "";

	param = (strlen(params) > index) ? strtok(params, index) : "";
	modelid = isnumeric(param) ? strval(param) : 0;
    if(strlen(params) >= index)
	{
		param = strtok(params, index);
    	color1 = isnumeric(param) ? strval(param) : 0;
	}
    if(strlen(params) >= index)
	{
		param = strtok(params, index);
    	color2 = isnumeric(param) ? strval(param) : 0;
	}

	if(modelid < 400 || modelid > 611)
	{
	    SendClientMessage(playerid, COLOR_RED, "Usage: /v [Model ID] [Color 1] [Color 2]");
	    return 1;
	}

	new slot;
	CreateVehicleForPlayer(modelid, playerid, slot, color1, color2);

	new string[128];
	format(string, sizeof(string), "~ %s created. (Slot: %d | ID: %d)", GetVehicleName(modelid), slot, modelid);
	SendClientMessage(playerid, COLOR_WHITE, string);

	string = "";

	format(string, sizeof(string), "%s has created a %s(%d) at their position. (#%d)", GetName(playerid), GetVehicleName(modelid), modelid, slot);
	SCL(string);
	return 1;
}

/*
	Destroy a created vehicle.
*/
YCMD:destroyv(playerid, params[], help)
{
	#pragma unused help

	if(!IsPlayerInDebug(playerid)) return 0;
	new index, param[255], vehicleid = -1;

	param = (strlen(params) > index) ? strtok(params, index) : "";
	vehicleid = isnumeric(param) ? strval(param) : 0;

	if(vehicleid < 0 || vehicleid == INVALID_PLAYER_ID)
	{
	    SendClientMessage(playerid, COLOR_RED, "Usage: /dv [Vehicle ID]");
	    return 1;
	}

	new string[128];

	format(string, sizeof(string), "~ You have destroyed vehicle #%d.", vehicleid);

	CallRemoteFunction("OnVehicleDeath", "i", vehicleid);
	SendClientMessage(playerid, COLOR_WHITE, string);

	string = "";

	format(string, sizeof(string), "%s has destroyed vehicle #%d.", GetName(playerid), vehicleid);
	SCL(string);
	return 1;
}

/*
	Remove a player's vehicle.
*/
YCMD:dpv(playerid, params[], help)
{
	#pragma unused help

	if(!IsPlayerInDebug(playerid)) return 0;
	new index, pplayerid = -1, slot = -1;
	new param[255] = "";

	param = (strlen(params) > index) ? strtok(params, index) : "";
	pplayerid = isnumeric(param) ? strval(param) : 0;

	param = (strlen(params) > index) ? strtok(params, index) : "";
	slot = isnumeric(param) ? strval(param) : -1;

	if(pplayerid == -1 || slot == -1)
	{
	    if(pplayerid != -1)
	    {
	        slot = pplayerid;
	        pplayerid = playerid;
		}
		else
		{
	    	SendClientMessage(playerid, COLOR_RED, "Usage: /dv [Player ID] [Slot]");
	    	return 1;
		}
	}
	if(pplayerid == INVALID_PLAYER_ID)
	{
	    SendClientMessage(playerid, COLOR_RED, "Error: Invalid Player ID.");
	    return 1;
	}

	new string[128], modelid = GetVehicleModel(GetPlayerVehicle(playerid, slot));
	if(playerid == pplayerid)
	{
		string = "";
		format(string, sizeof(string), "%s has destroyed their %s. (Slot: %d)", GetName(playerid), GetVehicleName(modelid), slot);

		SCL(string);
	}
	else
	{
	    string = "";
	    format(string, sizeof(string), "~ You have destroyed %s's %s.", GetName(pplayerid), GetVehicleName(modelid));

	    SendClientMessage(playerid, COLOR_WHITE, string);

		string = "";
		format(string, sizeof(string), "%s has destroyed %s's %s. (Slot: %d)", GetName(playerid), GetName(pplayerid), GetVehicleName(modelid), slot);

		SCL(string);
	}

	CallRemoteFunction("OnVehicleDeath", "i", GetPlayerVehicle(playerid, slot));
	return 1;
}

/*
	Give yourself or another player a weapon.
*/
YCMD:giveweapon(playerid, params[], help)
{
	#pragma unused help

	if(!IsPlayerInDebug(playerid)) return 0;
	new index, pplayerid = -1, weaponid = -1, ammo = -1;
	new param[255] = "";

	param = (strlen(params) > index) ? strtok(params, index) : "";
	pplayerid = isnumeric(param) ? strval(param) : -1;

	param = (strlen(params) > index) ? strtok(params, index) : "";
	weaponid = isnumeric(param) ? strval(param) : -1;

	param = (strlen(params) > index) ? strtok(params, index) : "";
	ammo = isnumeric(param) ? strval(param) : -1;

	if(pplayerid == -1 || weaponid == -1 || ammo == -1)
	{
	    if(pplayerid > -1 && weaponid > -1)
	    {
	        ammo = weaponid;
	        weaponid = pplayerid;
	        pplayerid = playerid;
	    }
	    else
	    {
	        SendClientMessage(playerid, COLOR_RED, "Usage: /giveweapon [Player ID] [Weapon ID] [Ammo]");
	        return 1;
		}
	}
	if(pplayerid == INVALID_PLAYER_ID)
	{
	    SendClientMessage(playerid, COLOR_RED, "Error: Invalid Player ID.");
	}

	if(weaponid < 0 || weaponid > 46 || strlen(weapon_name[weaponid]) == 0)
	{
	    SendClientMessage(playerid, COLOR_RED, "Error: Invalid Weapon ID.");
	    return 1;
	}

	if(ammo < 0 || ammo > 9999)
	{
	    SendClientMessage(playerid, COLOR_RED, "Error: Invalid ammo amount.");
	    return 1;
	}

	GivePlayerWeapon(pplayerid, weaponid, ammo);

	new string[96];
	if(pplayerid != playerid)
	{
	    format(string, sizeof(string), "~ You have been given a %s with %d ammo.", weapon_name[weaponid], ammo);
	    SendClientMessage(pplayerid, COLOR_WHITE, string);

	    format(string, sizeof(string), "~ You have given %s a %s with %d ammo.", GetName(pplayerid), weapon_name[weaponid], ammo);
	    SendClientMessage(pplayerid, COLOR_WHITE, string);

	    format(string, sizeof(string), "%s has given %s a %s with %d ammo.", GetName(playerid), GetName(pplayerid), weapon_name[weaponid], ammo);
	    SCL(string);
	}
	else
	{
		format(string, sizeof(string), "~ You have given yourself a %s with %d ammo.", weapon_name[weaponid], ammo);
		SendClientMessage(playerid, COLOR_WHITE, string);

		format(string, sizeof(string), "%s has given themself a %s with %d ammo.", GetName(playerid), weapon_name[weaponid], ammo);
		SCL(string);
	}
	return 1;
}
