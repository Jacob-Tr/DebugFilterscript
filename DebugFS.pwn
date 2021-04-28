/*
    Copyright (C) 2021  TRACEY, Jacob

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <a_samp> // General works of the SA-MP Team (2006-2020)

forward VA_OnScriptInit();
#include <YSI\y_hooks> // y_less' Callback Hooks; allowing easier integration with the library.
#include <YSI\y_commands> // y_less' Command Processor
#include <YSI\y_ini> // y_less' File Handling System.

#include <EUL\Utils\eUtilities>
#include <EUL\Utils\eColors>

#include <Server\Filterscript\DebugMetadata>

public OnFilterScriptInit()
{
	return 1;
}

public OnVehicleDeath(vehicleid)
{
	new playerid = IsPlayerVehicle(vehicleid);
	if(playerid != -1)
	{
	    for(new i = 0; i < MAX_ADMIN_VEHS; i++)
		{
			if(player_vehicles[playerid][i] == vehicleid)
			{
			    new string[128] = "", modelid = GetVehicleModel(vehicleid);
			 	format(string, sizeof(string), "~ Your %s has been destroyed.", GetVehicleName(modelid));
				
				DestroyPlayerVehicle(playerid, i);
				SCM(playerid, COLOR_WHITE, string);
				
				string = "";
				
				format(string, sizeof(string), "%s's %s (Slot: %d | ID: %d) has been destroyed.", GetName(playerid), GetVehicleName(modelid), i, vehicleid);
				SCL(string);
				break;
			}
		}
	}
	else
	{
		new Float:health;
		GetVehicleHealth(vehicleid, health);

		if(health > 250.0) SetVehicleToRespawn(vehicleid);
	}
}

public OnPlayerDisconnect(playerid, reason)
{
	DestroyPlayerVehicles(playerid);
	InitializePlayerVehicles(playerid);
	
	in_debug[playerid] = false;
}

YCMD:v(playerid, params[], help)
{
	#pragma unused help

	if(!IsPlayerInDebug(playerid)) return printf("Not in gamemode");
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
	    SCM(playerid, COLOR_RED, "Usage: /v [Model ID] [Color 1] [Color 2]");
	    return 1;
	}
	
	new slot;
	CreateVehicleForPlayer(modelid, playerid, slot, color1, color2);
	
	new string[128];
	format(string, sizeof(string), "~ %s created. (Slot: %d | ID: %d)", GetVehicleName(modelid), slot, modelid);
	SCM(playerid, COLOR_WHITE, string);
	
	string = "";
	
	format(string, sizeof(string), "%s has created a %s(%d) at their position. (#%d)", GetName(playerid), GetVehicleName(modelid), modelid, slot);
	SCL(string);
	return 1;
}

YCMD:destroyv(playerid, params[], help)
{
	#pragma unused help
	
	if(!IsPlayerInDebug(playerid)) return 0;
	new index, param[255], vehicleid = -1;
	
	param = (strlen(params) > index) ? strtok(params, index) : "";
	vehicleid = isnumeric(param) ? strval(param) : 0;
	
	if(vehicleid < 0 || vehicleid == INVALID_PLAYER_ID)
	{
	    SCM(playerid, COLOR_RED, "Usage: /dv [Vehicle ID]");
	    return 1;
	}
	
	new string[128];
	
	format(string, sizeof(string), "~ You have destroyed vehicle #%d.", vehicleid);
	
	CallRemoteFunction("OnVehicleDeath", "i", vehicleid);
	SCM(playerid, COLOR_WHITE, string);
	
	string = "";
	
	format(string, sizeof(string), "%s has destroyed vehicle #%d.", GetName(playerid), vehicleid);
	SCL(string);
	return 1;
}
	

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
	    	SCM(playerid, COLOR_RED, "Usage: /dv [Player ID] [Slot]");
	    	return 1;
		}
	}
	if(pplayerid == INVALID_PLAYER_ID)
	{
	    SCM(playerid, COLOR_RED, "Error: Invalid Player ID.");
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
	    
	    SCM(playerid, COLOR_WHITE, string);
	    
		string = "";
		format(string, sizeof(string), "%s has destroyed %s's %s. (Slot: %d)", GetName(playerid), GetName(pplayerid), GetVehicleName(modelid), slot);

		SCL(string);
	}
	
	CallRemoteFunction("OnVehicleDeath", "i", GetPlayerVehicle(playerid, slot));
	return 1;
}

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
	        SCM(playerid, COLOR_RED, "Usage: /giveweapon [Player ID] [Weapon ID] [Ammo]");
	        return 1;
		}
	}
	if(pplayerid == INVALID_PLAYER_ID)
	{
	    SCM(playerid, COLOR_RED, "Error: Invalid Player ID.");
	}
	
	if(weaponid < 0 || weaponid > 46 || strlen(weapon_name[weaponid]) == 0)
	{
	    SCM(playerid, COLOR_RED, "Error: Invalid Weapon ID.");
	    return 1;
	}
	
	if(ammo < 0 || ammo > 9999)
	{
	    SCM(playerid, COLOR_RED, "Error: Invalid ammo amount.");
	    return 1;
	}
	
	GivePlayerWeapon(pplayerid, weaponid, ammo);
	
	new string[96];
	if(pplayerid != playerid)
	{
	    format(string, sizeof(string), "~ You have been given a %s with %d ammo.", weapon_name[weaponid], ammo);
	    SCM(pplayerid, COLOR_WHITE, string);
	    
	    format(string, sizeof(string), "~ You have given %s a %s with %d ammo.", GetName(pplayerid), weapon_name[weaponid], ammo);
	    SCM(pplayerid, COLOR_WHITE, string);
	    
	    format(string, sizeof(string), "%s has given %s a %s with %d ammo.", GetName(playerid), GetName(pplayerid), weapon_name[weaponid], ammo);
	    SCL(string);
	}
	else
	{
		format(string, sizeof(string), "~ You have given yourself a %s with %d ammo.", weapon_name[weaponid], ammo);
		SCM(playerid, COLOR_WHITE, string);
		
		format(string, sizeof(string), "%s has given themself a %s with %d ammo.", GetName(playerid), weapon_name[weaponid], ammo);
		SCL(string);
	}
	return 1;
}
