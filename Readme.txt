Generally useful SA-MP Gamemode setup and debug in-game utilities.

    ~ This filterscript requires Y_less' YSI library and my EliteUserLib to run.

    (Parameters marked with '*' are optional)
 - GiveWeapon               /giveweapon [Player ID]* [Weapon ID] [Ammo] ~ Gives the player or specified player the specified weapon with 'Ammo" amount of ammunition.
 - Vehicle                  /v [Vehicle Model ID] [Color 1]* [Color 2]* ~ Spawns a vehicle of the specified Model ID with either random colors or those specified in the following paramterers.
 - Destroy Player Vehicle   /dpv [Player ID]* [Slot] ~ Destroys the specified player's vehicle which is in the specified slot; then reduces the slot of each proceeding player vehicle by 1.
 - Destroy Vehicle          /destroyv [Vehicle ID] ~ Destroys the vehicle if it is spawned by a player, then reduced the slot of each of that players proceeding vehicles by 1. If it is a server vehicle it will be respawned.
 
 Use call 'CallRemoteFunction("SetPlayerInDebug", "ii", playerid, value)' from your gamemode to allow the specified player to use debug features; where 'value' may be 1 for true and 0 for false.
