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

new bool:in_debug[MAX_PLAYERS] = {false, false, ...};

forward SetPlayerInDebug(playerid, value);
public SetPlayerInDebug(playerid, value)
{
	if(value == 1)
	{
	    printf("Set Debug In FilterScript");
		in_debug[playerid] = true;
		return 1;
	}
	else in_debug[playerid] = false;
	return 1;
}

stock IsPlayerInDebug(playerid) {return in_debug[playerid];}

new total_vehicle_count = 0;

new server_vehicle_count = 0;
new server_vehicles[MAX_VEHICLES] = {INVALID_VEHICLE_ID, INVALID_VEHICLE_ID, ...};
new bool:server_vehicle_spawned[MAX_VEHICLES] = {false, false, ...};

stock bool:IsVehicleSpawned(vehicleid) {return server_vehicle_spawned[vehicleid];}



stock AddServerVehicle(modelid, Float:x, Float:y, Float:z, Float:angle, color1=-1, color2=-1, bool:spawn=true, respawn_delay=0, addsiren=0)
{
	new vehicleid = ++server_vehicle_count;
	if(spawn)
	{
		total_vehicle_count++;
		server_vehicles[vehicleid] = AddStaticVehicleEx(modelid, x, y, z, angle, color1, color2, respawn_delay, addsiren);
	}
	server_vehicle_spawned[vehicleid] = spawn;
}


new player_vehicle[MAX_VEHICLES] = {-1, -1, ...};

stock IsPlayerVehicle(vehicleid) {return (player_vehicle[vehicleid] != -1);}

#define MAX_ADMIN_VEHS 1000
new player_vehicle_count[MAX_PLAYERS] = {0, 0, ...};
new player_vehicles[MAX_PLAYERS][MAX_ADMIN_VEHS] = {{INVALID_VEHICLE_ID, INVALID_VEHICLE_ID, ...}, {INVALID_VEHICLE_ID, INVALID_VEHICLE_ID, ...}, ...};

stock InitializePlayerVehicles(playerid)
{
	player_vehicle_count[playerid] = 0;
	for(new i = 0; i < MAX_ADMIN_VEHS; i++) player_vehicles[playerid][i] = INVALID_VEHICLE_ID;
}

stock GetPlayerVehicle(playerid, slot) {return player_vehicles[playerid][slot];}
stock SetPlayerVehicle(playerid, slot, vehicleid) 
{
	player_vehicles[playerid][slot] = vehicleid;
	player_vehicle[vehicleid] = playerid;
} 

stock CreateVehicleForPlayer(modelid, playerid, &slot, color1=-1, color2=-1)
{
	new Float:x, Float:y, Float:z, Float:rot;
	GetPlayerPos(playerid, x, y, z);
	
	GetXYInFrontOfPlayer(playerid, x, y);
	GetPlayerFacingAngle(playerid, rot);
	
	slot = ++player_vehicle_count[playerid];

	total_vehicle_count++;
	SetPlayerVehicle(playerid, slot, CreateVehicle(modelid, x, y, z, rot, color1, color2, -1));
}

stock DestroyPlayerVehicle(playerid, slot)
{
	if(player_vehicles[playerid][slot] != INVALID_VEHICLE_ID)
	{
		DestroyVehicle(player_vehicles[playerid][slot]);
		player_vehicle_count[playerid]--;
		player_vehicle[player_vehicles[playerid][slot]] = -1;
		player_vehicles[playerid][slot] = INVALID_PLAYER_ID;
		
		total_vehicle_count--;

		new new_slot = slot;
		for(new i = new_slot + 1; i < MAX_ADMIN_VEHS; i++)
		{
			if(player_vehicles[playerid][i] == INVALID_VEHICLE_ID) break;
			
			player_vehicles[playerid][new_slot] = player_vehicles[playerid][i];
			player_vehicles[playerid][i] = INVALID_VEHICLE_ID;

			new_slot = i;
		}
	}
	return 1;
}

stock RespawnPlayerVehicle(playerid, slot) {SetVehicleToRespawn(player_vehicles[playerid][slot]);}

stock RespawnPlayerVehicles(playerid) {for(new i = 0; i < player_vehicle_count[playerid]; i++) RespawnPlayerVehicle(playerid, i);}

stock DestroyPlayerVehicles(playerid)
{
	for(new i = 1; i < player_vehicle_count[playerid]; i++) DestroyPlayerVehicle(playerid, 1);
	return 1;
}
