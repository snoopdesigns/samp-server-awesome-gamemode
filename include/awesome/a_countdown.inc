#if defined _countdown_included
	#endinput
#endif
#define _countdown_included

#include <a_samp>

#define MODULE_FUNCTION(%1) forward CD_%1; public CD_%1

#define createCountdown(%1,%2) \
	new c_id=findCountdownSlot(); \
	for(FOR_PLAYERS) { \
	if(%1 == TRUE && IsPlayerConnected(i)){ \
		countdown[c_id][players+countdown[c_id][player_num]] = i; \
		countdown[c_id][player_num]++; \
	}} \
	countdown[c_id][active]=1; \
	countdown[c_id][count]=5; \
	copyString(%2, countdown[c_id][function],32); \
	SetTimerEx("COUNTDOWN_tick",1000,0,"d",c_id)

enum COUNTDOWN {
	count,
	active,
	function[32],
	players[MAX_PLAYERS],
	player_num
}

new countdown[10][COUNTDOWN];

forward COUNTDOWN_init();
forward COUNTDOWN_tick(const c_id);
forward COUNTDOWN_test();

MODULE_FUNCTION(OnGameModeInit()) {
    for (new i = 0; i < 10; i++) {
		countdown[i][count] = -1;
		countdown[i][active] = 0;
		countdown[i][function] = 0;
		countdown[i][player_num] = 0;
	}
}

stock COUNTDOWN_start() {
	createCountdown(1, "COUNTDOWN_test");
}

public COUNTDOWN_test() {
	SendClientMessageToAll(COLOR_MG, "COUNTDOWN DONE");
}

public COUNTDOWN_tick(const c_id) {
	if (countdown[c_id][count] == 0) {
		for (new i=0; i<countdown[c_id][player_num]; i++) {
			TogglePlayerControllable(countdown[c_id][players+i],1);
			PlayerPlaySound(countdown[c_id][players+i], 1057, 0.0, 0.0, 0.0);
			GameTextForPlayer(countdown[c_id][players+i], "~r~GO", 1000, 3);
		}
		CallRemoteFunction(countdown[c_id][function],"");
		countdown[c_id][count] = -1;
		countdown[c_id][active] = 0;
		countdown[c_id][function] = 0;
		countdown[c_id][player_num] = 0;
	} else {
		for (new i=0; i<countdown[c_id][player_num]; i++) {
			new str[16];
			format(str, sizeof(str),"%d",countdown[c_id][count]);
			TogglePlayerControllable(countdown[c_id][players+i],0);
			PlayerPlaySound(countdown[c_id][players+i], 1056, 0.0, 0.0, 0.0);
			GameTextForPlayer(countdown[c_id][players+i], str, 1000, 3);
		}
		countdown[c_id][count]--;
		SetTimerEx("COUNTDOWN_tick",1000,0,"d",c_id);
	}
}

stock createCountdown111() {
	LOG_write("Countdown create");
}

stock findCountdownSlot() {
	for (new i = 0; i < 10; i++) {
	    if (countdown[i][active] == FALSE) {
			return i;
		}
	}
	return -1;
}
MODULE_FUNCTION(OnGameModeExit()) {}
MODULE_FUNCTION(OnPlayerConnect(playerid)) {}
MODULE_FUNCTION(OnPlayerDisconnect(playerid, reason)) {}
MODULE_FUNCTION(OnPlayerRequestClass(playerid, classid)) {}
MODULE_FUNCTION(OnPlayerSpawn(playerid)) {}
MODULE_FUNCTION(OnPlayerCommandText(playerid, cmdtext[])) {}
MODULE_FUNCTION(OnPlayerDeath(playerid, killerid, reason)) {}
MODULE_FUNCTION(OnVehicleSpawn(vehicleid)) {}
MODULE_FUNCTION(OnVehicleDeath(vehicleid, killerid)) {}
MODULE_FUNCTION(OnPlayerText(playerid, text[])) {}
MODULE_FUNCTION(OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)) {}
MODULE_FUNCTION(OnPlayerExitVehicle(playerid, vehicleid)) {}
MODULE_FUNCTION(OnPlayerStateChange(playerid, newstate, oldstate)) {}
MODULE_FUNCTION(OnPlayerEnterCheckpoint(playerid)) {}
MODULE_FUNCTION(OnPlayerLeaveCheckpoint(playerid)) {}
MODULE_FUNCTION(OnPlayerEnterRaceCheckpoint(playerid)) {}
MODULE_FUNCTION(OnPlayerLeaveRaceCheckpoint(playerid)) {}
MODULE_FUNCTION(OnRconCommand(cmd[])) {}
MODULE_FUNCTION(OnPlayerRequestSpawn(playerid)) {}
MODULE_FUNCTION(OnObjectMoved(objectid)) {}
MODULE_FUNCTION(OnPlayerObjectMoved(playerid, objectid)) {}
MODULE_FUNCTION(OnPlayerPickUpPickup(playerid, pickupid)) {}
MODULE_FUNCTION(OnVehicleMod(playerid, vehicleid, componentid)) {}
MODULE_FUNCTION(OnVehiclePaintjob(playerid, vehicleid, paintjobid)) {}
MODULE_FUNCTION(OnVehicleRespray(playerid, vehicleid, color1, color2)) {}
MODULE_FUNCTION(OnPlayerSelectedMenuRow(playerid, row)) {}
MODULE_FUNCTION(OnPlayerExitedMenu(playerid)) {}
MODULE_FUNCTION(OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)) {}
MODULE_FUNCTION(OnPlayerKeyStateChange(playerid, newkeys, oldkeys)) {}
MODULE_FUNCTION(OnRconLoginAttempt(ip[], password[], success)) {}
MODULE_FUNCTION(OnPlayerUpdate(playerid)) {}
MODULE_FUNCTION(OnPlayerStreamIn(playerid, forplayerid)) {}
MODULE_FUNCTION(OnPlayerStreamOut(playerid, forplayerid)) {}
MODULE_FUNCTION(OnVehicleStreamIn(vehicleid, forplayerid)) {}
MODULE_FUNCTION(OnVehicleStreamOut(vehicleid, forplayerid)) {}
MODULE_FUNCTION(OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])) {}
MODULE_FUNCTION(OnPlayerClickPlayer(playerid, clickedplayerid, source)) {}
#undef MODULE_FUNCTION