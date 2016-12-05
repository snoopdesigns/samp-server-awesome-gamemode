#if defined _config_included
	#endinput
#endif
#define _config_included

#define MODULE_FUNCTION(%1) forward CFG_%1; public CFG_%1

#include "include/awesome/a_utils.inc"
#include "include/awesome/a_log.inc"
#include "include/awesome/a_dbutils.inc"

/* DB fields constants */
#define CFG_KEY "CfgKey"
#define CFG_VALUE "CfgValue"

forward CFG_getStrValue(const key[], result[], const result_size, const default_value[]);
forward CFG_getIntValue(const key[], const default_value);
forward CFG_updateStrValue(const key[], const value[]);

MODULE_FUNCTION(OnGameModeInit()) {
	DBUTILS_clearResults(DBUTILS_runNamedQuery("createConfigurationTable","NA"));
	new result[32], result_int;
	CFG_getStrValue("test-key", result, sizeof(result), "FUUU");
	result_int = CFG_getIntValue("test-key-int", 42);
	LOG_writeFormatted("CONFIG: %s", result);
	LOG_writeFormatted("CONFIG: %d", result_int);
}

public CFG_getStrValue(const key[], result[], const result_size, const default_value[]) {
	LOG_writeFormatted("Configuration value requested: %s", key);
	new DBResult:db_result,cfg_value[32];
	db_result = DBUTILS_runNamedQuery("findConfigurationByKey","s",key);
	if(DBUTILS_getResultSize(db_result) != 0) {
		if(DBUTILS_getResultSize(db_result) == 1) {
			DBUTILS_getString(db_result, CFG_VALUE, cfg_value, sizeof(cfg_value));			
		} else {
			LOG_writeFormatted("Found more then one configuration value for key: %s", key);
		}
	} else {
		LOG_writeFormatted("No configuration found for key: %s, using default: %s", key, default_value);
		strins(cfg_value, default_value, 0);
	}
	DBUTILS_clearResults(db_result);
	strins(result, cfg_value, 0, result_size);
}

public CFG_getIntValue(const key[], const default_value) {
	new cfg_value[32];
	CFG_getStrValue(key, cfg_value, sizeof(cfg_value), intToStr(default_value));
	return strToInt(cfg_value);
}

public CFG_updateStrValue(const key[], const value[]) {
	new DBResult:db_result;
	db_result = DBUTILS_runNamedQuery("findConfigurationByKey","s",key);
	if(DBUTILS_getResultSize(db_result) < 1) {
		// NOT found, needs to be created
		DBUTILS_clearResults(DBUTILS_runNamedQuery("createNewConfiguration","ss",key,value));
	} else {
		// value found, needs to be updated
		DBUTILS_clearResults(DBUTILS_runNamedQuery("updateConfigurationValue","ss",key,value));
	}
	DBUTILS_clearResults(db_result);
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