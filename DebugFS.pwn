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

#include <a_samp> // https://sa-mp.com/download.php

#define YSI_NO_HEAP_MALLOC

// https://github.com/pawn-lang/YSI-Includes/releases
#include <YSI_Coding\y_hooks>
#include <YSI_Storage\y_ini>
#include <YSI_Visual\y_commands>

// https://github.com/Jacob-Tr/Elite-User-Lib
#include <EUL\Utils\eColors>
#include <EUL\Utils\eUtilities>

#include <DebugFS\DebugMetadata>
#include <DebugFS\CommandUtilities>
#include <DebugFS\DebugCommands>
#include <DebugFS\TrainBaseBuilder>

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
				SendClientMessage(playerid, COLOR_WHITE, string);
				
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

public OnPlayerConnect(playerid)
{
	SetSpawnInfo(playerid, -1, -1, 2031.17, 1343.00, 10.82, 268.7, 0, 0, 0, 0, 0, 0);
}

public OnPlayerDisconnect(playerid, reason)
{
	DestroyPlayerVehicles(playerid);
	InitializePlayerVehicles(playerid);
	
	in_debug[playerid] = false;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	RespawnPlayerVehicles(playerid);
}
