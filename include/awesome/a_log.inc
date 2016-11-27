#if defined _log_included
	#endinput
#endif
#define _log_included

#include <a_samp>
#include "include/awesome/a_utils.inc"

#define MODULE_FUNCTION(%1) forward LOG_%1; public LOG_%1
#define LOG_MSG_MAX_LEN 512

#define LOG_writeFormatted(%1,%2) format(logFormatttedMsg, sizeof logFormatttedMsg, %1, %2); LOG_write(logFormatttedMsg)
new logFormatttedMsg[LOG_MSG_MAX_LEN];

#define LOG_FILENAME "awesome-gamemode.log"

stock LOG_write(const message[]) {
	new File:handle = fopen(LOG_FILENAME, io_append);
	if(handle)
	{
		new logWithDatetime[1024];
		new datetime[32];
		get_datetime_string(sizeof datetime, datetime);
		format(logWithDatetime, sizeof logWithDatetime, "[%s] * %s", datetime, message);
		fwrite(handle, logWithDatetime);
		fwrite(handle, "\r\n");
		fclose(handle);
	}
	else
	{
		print("Failed to create log file awesome-gamemode.log");
		return;
	}
}

MODULE_FUNCTION(OnGameModeInit()) {}
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