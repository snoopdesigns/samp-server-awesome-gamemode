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
#define STORED_Q_FILENAME "stored_queries.sql"
#define QUERY_SQL_LEN 512
	
enum stored_query {
	stored_q_name[32],
	stored_q_format[8],
	stored_q_sql[QUERY_SQL_LEN]
}

new stored_queries[16][stored_query];
new stored_queries_num = 0;
	
new DB:db_handle;

/* public functions declaration */
forward DBResult:DBUTILS_runNamedQuery(const queryName[],const sqlFormat[],...);
forward DBUTILS_getResultSize(const DBResult:db_result);
forward DBUTILS_clearResults(const DBResult:db_result);
forward DBUTILS_getString(const DBResult:db_result, const field_name[], result[], max_result_size);
forward DBUTILS_getInt(const DBResult:db_result, const field_name[]);
forward Float:DBUTILS_getFloat(const DBResult:db_result, const field_name[]);
forward DBUTILS_nextItem(DBResult:db_result);

MODULE_FUNCTION(OnGameModeInit()) {
    LOG_write("Database Subsystem initializing");
	if((db_handle = db_open(DB_FILENAME)) == DB:0) {
		LOG_write("Failed to open a connection to Awesome DB");
		SendRconCommand("exit");
	}
	LOG_write("Successfully created a connection to Awesome DB");
	readStoredQueries(STORED_Q_FILENAME);
}

MODULE_FUNCTION(OnGameModeExit()) {
	db_close(db_handle);
}

public DBResult:DBUTILS_runNamedQuery(const queryName[],const sqlFormat[],{Float,_}:...) {
    new q_id,q_name[32],q_format[8],q_sql[QUERY_SQL_LEN];
	q_id = findQueryIdByName(queryName);
	if (q_id == -1) {
		LOG_writeFormatted("Unable to find query by name: %s", queryName);
		return DBResult:0;
	}
	copyString(stored_queries[q_id][stored_q_name], q_name, sizeof(q_name));
	copyString(stored_queries[q_id][stored_q_format], q_format, sizeof(q_format));
	copyString(stored_queries[q_id][stored_q_sql], q_sql, sizeof(q_sql));
	// TODO if query not found - deadlock
	if (str_eq(sqlFormat, q_format)) {
		new sql[QUERY_SQL_LEN];
		copyString(q_sql, sql, sizeof(sql));
		if (str_neq(q_format, "NA")) {
			for (new j = 0; j < strlen(q_format); j++) {
				if (q_format[j] == 's') { // string param
					new stringParameterValue[32];
					EXTRACT_ARRAY_ARG(stringParameterValue, j+2);
					replaceFirst("%s", stringParameterValue, sql, strlen("%s"), sizeof(sql));
				} else if (q_format[j] == 'd') {
					replaceFirst("%d", intToStr(getarg(j+2)), sql, strlen("%d"), sizeof(sql));
				} else if (q_format[j] == 'f') {
					new argValFloatString[32];
					format(argValFloatString, sizeof(argValFloatString), "%f", getarg(j+2));
					LOG_writeFormatted("F arg: %s",argValFloatString);
					replaceFirst("%f", argValFloatString, sql, strlen("%f"), sizeof(sql));
				}
			}
		}
		LOG_writeFormatted("Executing query: %s", sql);
		return db_query(db_handle,sql);
	} else {
		LOG_writeFormatted("Query format is invalid: %s", q_format);
	}
	return DBResult:0;
}

public DBUTILS_getResultSize(const DBResult:db_result) {
	return db_num_rows(db_result);
}

public DBUTILS_clearResults(const DBResult:db_result) {
	db_free_result(db_result);
}

public DBUTILS_getString(const DBResult:db_result, const field_name[], result[], max_result_size) {
	db_get_field_assoc(db_result, field_name, result, max_result_size);
}

public DBUTILS_getInt(const DBResult:db_result, const field_name[]) {
	return db_get_field_assoc_int(db_result, field_name);
}

public Float:DBUTILS_getFloat(const DBResult:db_result, const field_name[]) {
	return db_get_field_assoc_float(db_result, field_name);
}

public DBUTILS_nextItem(DBResult:db_result) {
	return db_next_row(db_result);
}

stock readStoredQueries(const filename[]) {

    new File:file = fopen(filename, io_read), line[QUERY_SQL_LEN];
    while(fread(file, line))
    {
		rtrim(line);
		new queryName[32];
		new queryFormat[8];
		new querySql[QUERY_SQL_LEN];
		sscanf(line, "sss", queryName, queryFormat, querySql);
		LOG_write("==================================");
        LOG_write("Read stored query:");
		LOG_writeFormatted("Name: %s", queryName);
		LOG_writeFormatted("Format: %s", queryFormat);
		LOG_writeFormatted("SQL: %s", querySql);
		LOG_write("==================================");
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