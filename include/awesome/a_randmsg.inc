#include <a_samp>

#include "include/awesome/a_config.inc"
#include "include/awesome/colors.inc"

#define MODULE_FUNCTION(%1) forward RMSG_%1; public RMSG_%1

#define DEFAULT_MSG_INTERVAL 4 * 60 * 1000 // 8 mins

forward SendRandMSG();

new RandomMSG[][] = {
    "* Use /help to view server commands and teleports",
    "* Visit our website: http://snoopdesigns.co.nr/",
    "* Cheats are forbidden on this server and will results a ban"
};

MODULE_FUNCTION(OnGameModeInit()) {
	LOG_write("RandomMSG Subsystem initializing");
	SetTimer("SendRandMSG", CFG_getIntValue(RMSG_DELAY_KEY, DEFAULT_MSG_INTERVAL), false);
}

public SendRandMSG() {
    SendClientMessageToAll(COLOR_RAND_MSG, RandomMSG[random(sizeof(RandomMSG))]);
	SetTimer("SendRandMSG", CFG_getIntValue(RMSG_DELAY_KEY, DEFAULT_MSG_INTERVAL), false);
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
