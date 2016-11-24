#if defined _dbutils_included
	#endinput
#endif
#define _dbutils_included

#include <a_sampdb>
#include <a_samp>
#include "include/awesome/a_log.inc"
#include "include/awesome/a_utils.inc"
	
#define MODULE_FUNCTION(%1) forward DB_%1; public DB_%1

#define DB_FILENAME "awesome-freeroam.db"
	
new DB:db_handle;

MODULE_FUNCTION(OnGameModeInit()) 
{
    LOG_write("Database Subsystem initializing");
	if((db_handle = db_open(DB_FILENAME)) == DB:0)
	{
		LOG_write("Failed to open a connection to Awesome DB");
		SendRconCommand("exit");
	}
	LOG_write("Successfully created a connection to Awesome DB");
}

MODULE_FUNCTION(OnGameModeExit())
{
	db_close(db_handle);
}

stock DBResult:DBUTILS_runSelectQuery(query[])
{
	return db_query(db_handle, query);
}

stock DBUTILS_runSelectQueryClear(DBResult: db_result) 
{
	db_free_result(db_result);
}

stock DBUTILS_runCreateQuery(query[])
{
	DBUTILS_logQuery(query);
	db_free_result(db_query(db_handle, query));
}

stock DBUTILS_runDropQuery(table_name[])
{
	new query[256];
	format(query, sizeof(query), "DROP TABLE IF EXISTS '%s'", table_name);
	DBUTILS_logQuery(query);
	db_free_result(db_query(db_handle, query));
}

stock DBUTILS_runInsertQuery(query[])
{
	DBUTILS_runCreateQuery(query);
}

stock DBUTILS_runUpdateQuery(query[])
{
	DBUTILS_runCreateQuery(query);
}

stock DBUTILS_getResultsetSize(DBResult:db_result)
{
	return db_num_rows(db_result);
}

stock DBUTILS_nextItem(DBResult:db_result)
{
	return db_next_row(db_result);
}

stock DBUTILS_getFieldFromResult(DBResult:db_result, field_name[]) 
{
	new field[128];
	db_get_field_assoc(db_result, field_name, field, sizeof field);
	return field;
}

DBUTILS_logQuery(query[]) 
{
	new string[1024];
	format(string,sizeof(string),"[DB_QUERY]: %s",query);
	LOG_write(string);
}

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