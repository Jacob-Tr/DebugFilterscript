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

new frozen[MAX_PLAYERS] = {0, 0, ...};
new frozen_timer[MAX_PLAYERS];

forward DebugFreezeTimer(const playerid, const Float:x, const Float:y, const Float:z);
public DebugFreezeTimer(const playerid, const Float:x, const Float:y, const Float:z)
{
	// Since we run the timer in 20 milisecond intervals, running it 25 times will freeze the player for 500 miliseconds.
	if(frozen[playerid] >= 25)
	{
	    frozen[playerid] = 0;
	    KillTimer(frozen_timer[playerid]);

		SetPlayerPos(playerid, x, y, (z + 2));
	    return;
	}

	SetPlayerPos(playerid, x, y, z);

	frozen[playerid]++;
}

new vehicle_shake_timer[MAX_VEHICLES];
new shake[MAX_VEHICLES] = {0, 0, ...};

forward ShakeV(vehicleid, const Float:x, const Float:y, const Float:z, const Float:a);
public ShakeV(vehicleid, const Float:x, const Float:y, const Float:z, const Float:a)
{
	if(shake[vehicleid] >= 25)
	{
		SetVehiclePos(vehicleid, x, y, z);
		SetVehicleZAngle(vehicleid, a);

	    shake[vehicleid] = 0;
	    KillTimer(vehicle_shake_timer[vehicleid]);

	    return;
	}

	new Float:xx, Float:yy, Float:zz, Float:aa;

	GetVehiclePos(vehicleid, xx, yy, zz);
	GetVehicleZAngle(vehicleid, aa);

	xx = (x == xx) ? xx + 1 : (x < xx) ? xx - 1 : xx;
	yy = (y == yy) ? yy + 1 : (y < yy) ? yy - 1 : yy;
	zz = (z == zz) ? zz + 1 : (z < zz) ? zz - 1 : zz;
	aa = (a == aa) ? aa + 1 : (a < aa) ? aa - 1 : aa;

	SetVehiclePos(vehicleid, xx, yy, zz);
	SetVehicleZAngle(vehicleid, aa);

	shake[vehicleid]++;
}

new bool:player_fort_vehicles[MAX_PLAYERS][MAX_ADMIN_VEHS] = {{false, false, ...}, {false, false, ...}, ...};

bool:IsPlayerFortVehicle(playerid, slot) {return player_fort_vehicles[playerid][slot];}
SetPlayerFortVehicle(playerid, slot, bool:value) {player_fort_vehicles[playerid][slot] = value;}

YCMD:add(playerid, params[], help)
{
	#pragma unused help
	#pragma unused params
	
	if(frozen[playerid] != 0) return SendClientMessage(playerid, COLOR_RED, "Error: Cooldown active.");
	
	new Float:x, Float:y, Float:z, Float:a;
	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngleFix(playerid, a);
	
	frozen[playerid] = 1;
	frozen_timer[playerid] = SetTimerEx("DebugFreezeTimer", 20, 1, "iffff", playerid, x, y, z, a);
	
	new slot;
	CreateVehicleForPlayer(590, playerid, slot, 0, 0);
	
	SetPlayerFortVehicle(playerid, slot, true);
	
	new string[32];
	format(string, sizeof(string), "~ Fort trailer added in slot %d.", slot);
	
	SendClientMessage(playerid, COLOR_WHITE, string);
	
	return 1;
}

YCMD:remove(playerid, params[], help)
{
	#pragma unused help

	if(!IsPlayerInDebug(playerid)) return 0;
	new index, param[255], slot = -1;

	param = (strlen(params) > index) ? strtok(params, index) : "";
	slot = isnumeric(param) ? strval(param) : 0;
	
	if(slot < 0)
	{
	    SendClientMessage(playerid, COLOR_RED, "Usage: /probe [Slot]");
	    return 1;
	}

	if(!IsPlayerFortVehicle(playerid, slot)) return SendClientMessage(playerid, COLOR_RED, "Error: That vehicle is not part of your fort.");

	DestroyPlayerVehicle(playerid, slot);
	for(new i = slot; i < player_vehicle_count[playerid]; i++) SetPlayerFortVehicle(playerid, i, IsPlayerFortVehicle(playerid, (i + 1)));
	
	return 1;
}
	
YCMD:probe(playerid, params[], help)
{
	#pragma unused help

	if(!IsPlayerInDebug(playerid)) return 0;
	new index, param[255], slot = -1, by_id;
	
	param = (strlen(params) > index) ? strtok(params, index) : "";
	slot = isnumeric(param) ? strval(param) : 0;
	
	param = (strlen(params) > index) ? strtok(params, index) : "";
	by_id = isnumeric(param) ? strval(param) : 0;
	
	if(slot < 0)
	{
	    SendClientMessage(playerid, COLOR_RED, "Usage: /probe [Slot]");
	    return 1;
	}
	
	new vehicleid = (by_id) ? slot : GetPlayerVehicle(playerid, slot);
	new Float:x, Float:y, Float:z, Float:a;
	
	GetVehiclePos(vehicleid, x, y, z);
	GetVehicleZAngle(vehicleid, a);
	
	vehicle_shake_timer[vehicleid] = SetTimerEx("ShakeV", 20, 1, "iffff", vehicleid, x, y, z, a);
	
	return 1;
}
