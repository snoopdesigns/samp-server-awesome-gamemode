#if defined _dbutils_included
	#endinput
#endif
#define _dbutils_included

#include <a_sampdb>
#include <a_samp>
#include "include/awesome/a_log.inc"
#include "include/awesome/a_utils.inc"
#include "include/awesome/strlib.inc"
	
#define MODULE_FUNCTION(%1) forward DB_%1; public DB_%1

#define DB_FILENAME "awesome-freeroam.db"
	
enum stored_query {
	stored_q_name[32],
	stored_q_format[8],
	stored_q_sql[32]
}

new stored_queries[4][stored_query];
new stored_queries_num = 0;
	
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
	readStoredQueries("stored_queries.sql");
	DBUtils_runSelectNamedQuery("findAllPlayers","ss","FUUU","TEST");
}

MODULE_FUNCTION(OnGameModeExit())
{
	db_close(db_handle);
}

stock readStoredQueries(const filename[]) {

    new File:file = fopen(filename, io_read), line[128];
    while(fread(file, line))
    {
		new queryName[32];
		new queryFormat[8];
		new querySql[32];
		sscanf(rtrim(line), "sss", queryName, queryFormat, querySql);
        LOG_writeFormatted("Read stored query: name = %s, format = %s, query = %s", queryName, queryFormat, querySql);
		stored_queries[stored_queries_num][stored_q_name] = queryName;
		stored_queries[stored_queries_num][stored_q_format] = queryFormat;
		stored_queries[stored_queries_num][stored_q_sql] = querySql;
		stored_queries_num++;
    }
}

stock findQueryIdByName(const name[]) {
	for (new i = 0; i < stored_queries_num; i++) {
	    if (strcmp(name, stored_queries[i][stored_q_name]) == 0) {
			return i;
		}
	}
	return -1;
}



stock DBResult:DBUtils_runSelectNamedQuery(const queryName[],const sqlFormat[],...) {
    new query_id;
	query_id = findQueryIdByName(queryName);
	LOG_writeFormatted("Found query by name: %s, query: %s", stored_queries[query_id][stored_q_name], stored_queries[query_id][stored_q_sql]);
	if (strcmp(sqlFormat, stored_queries[query_id][stored_q_format]) == 0) {
		new sql[32];
		new sql_format[8];
		format(sql_format, sizeof(sql_format), "%s", stored_queries[query_id][stored_q_format]);
		LOG_writeFormatted("Query format: %s, %d", sql_format, strlen(sql_format));
		if (strcmp(stored_queries[query_id][stored_q_format], "NA") != 0) {
			format(sql, sizeof(sql), "%s", stored_queries[query_id][stored_q_sql]);
			for (new j = 0; j < strlen(sql_format); j++) {
				if (sql_format[j] == 's') { // string param
					LOG_writeFormatted("Found S param: numargs = %d", numargs());
					new stringParameterValue[8];
					EXTRACT_ARRAY_ARG(stringParameterValue, j+2);
					new strToReplace[8];
					strToReplace = "%s";
					new beginIndex = strfind(sql, strToReplace);
					strdel(sql, beginIndex, beginIndex + 2);
					strins(sql, stringParameterValue, beginIndex);
				}
			}
		}
		LOG_writeFormatted("Query after modify: %s", sql);
		return DBUTILS_runSelectQuery(sql);
	}
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