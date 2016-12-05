#if defined _reg_included
	#endinput
#endif
#define _reg_included

#include <a_samp>
#include <a_sampdb>
#include "include/awesome/a_dbutils.inc"
#include "include/awesome/a_consts.inc"
#include "include/awesome/a_utils.inc"
#include "include/awesome/a_config.inc"
#include "include/awesome/colors.inc"

/* modules callback declaration */
#define MODULE_FUNCTION(%1) forward REG_%1; public REG_%1

//player vars
#define PLAYER_VAR_LEVEL "LEVEL"
#define DEFAULT_DB_SAVE_INTERVAL 5 * 60 * 1000 //5 minutes

/* DB fields constants */
#define PLAYER_PROP_NAME "PlayerName"
#define PLAYER_PROP_PASSWORD "PlayerPassword"
#define PLAYER_PROP_IP "PlayerIP"
#define PLAYER_PROP_LEVEL "PlayerLevel"
#define PLAYER_PROP_SCORE "PlayerScore"
#define PLAYER_PROP_MONEY "PlayerMoney"

/* public functions declaration */
forward REG_registerNewPlayer(const playerid, const password[]);
forward REG_loginPlayer(const playerid, const password[]);
forward REG_printPlayerStats(const playerid);
forward REG_getPlayerLevel(const playerid);
forward REG_setPlayerLevel(const playerid, const level);

/* public timer callbacks */
forward REG_savePlayerDataTimerFunc();

MODULE_FUNCTION(OnGameModeInit()) {
	LOG_write("Registration Subsystem initializing");
	//DBUTILS_clearResults(DBUTILS_runNamedQuery("dropPlayerTable","NA"));
	DBUTILS_clearResults(DBUTILS_runNamedQuery("createPlayerTable","NA"));
	SetTimer("REG_savePlayerDataTimerFunc", CFG_getIntValue(STATS_SAVE_DELAY_KEY, DEFAULT_DB_SAVE_INTERVAL), true);
}

MODULE_FUNCTION(OnPlayerConnect(playerid)) {
	SetPVarInt(playerid, PLAYER_VAR_LEVEL, 0); // set default level to 0
	
	new player_ip[32], player_name[32];
   	GetPlayerIp(playerid, player_ip, sizeof(player_ip));
	GetPlayerName(playerid, player_name, sizeof(player_name));
	
	if(REG_isPlayerRegistered(playerid) == TRUE) {
		new ip_from_db[32];
		ip_from_db = REG_getPropertyOfPlayer(playerid, PLAYER_PROP_IP);
		if(str_eq(ip_from_db, player_ip)) {
			REG_loadPlayerData(playerid);
			SendClientMessageFormatted(playerid, COLOR_REG, "* IP auto-login successfull. Welcome back, %s!", player_name);
		} else {
			SendClientMessage(playerid, COLOR_ERROR, "* IP auto-login failed. Please /login using your password.");
		}
	} else {
		SendClientMessage(playerid, COLOR_REG, "* Your name is not registered. Please register using /register <password>");
	}
}

MODULE_FUNCTION(OnPlayerDisconnect(playerid, reason)) {
	LOG_writeFormatted("Called OnPlayerDisconnect() in REG, playerid = %d, reason = %d", playerid, reason);
}

public REG_registerNewPlayer(const playerid, const password[]) {
	new name[MAX_PLAYER_NAME], ip[32];
	GetPlayerName(playerid, name, sizeof(name));
	GetPlayerIp(playerid, ip, sizeof(ip));
	if(REG_isPlayerRegistered(playerid) == FALSE) {
		new DBResult:db_result = DBUTILS_runNamedQuery("createPlayer","sssddd",name,password,ip,1,GetPlayerScore(playerid),GetPlayerMoney(playerid));
		DBUTILS_clearResults(db_result);
		if(REG_isPlayerRegistered(playerid) == TRUE) {
			// player successfully registered
			SetPVarInt(playerid, PLAYER_VAR_LEVEL, 1);
			SendClientMessageFormatted(playerid, COLOR_REG, "* New account %s has been registerred successfully", name);
		}
	} else {
		if(REG_isPlayerAuthenticated(playerid) == TRUE) {
			// player is autheticated and requests register - bad
			SendClientMessage(playerid, COLOR_ERROR, "* You are already registered.");
		} else {
			// player is not autheticated and requests register under existing name - bad
			SendClientMessageFormatted(playerid, COLOR_REG, "* This nickname: '%s' is registered already. If this is you nickname, please /login, otherwise please choose other nickname", name);
		}
	}
}

public REG_loginPlayer(const playerid, const password[]) {
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	if(REG_isPlayerAuthenticated(playerid) == FALSE) {
		if(REG_isPlayerRegistered(playerid) == TRUE) {
			new password_from_db[32];
			password_from_db = REG_getPropertyOfPlayer(playerid, PLAYER_PROP_IP);
			if(str_eq(password_from_db, password)) {
				SendClientMessage(playerid, COLOR_LAWNGREEN, "* Login using password successfull.");
				//restore player props from db
				REG_loadPlayerData(playerid);
			} else {
				SendClientMessage(playerid, COLOR_ERROR, "* Login using password unsuccessfull.");
			}
		} else {
			// players nickname is not registered in db
			SendClientMessageFormatted(playerid, COLOR_REG, "* Your nickname: '%s' is not registered. Please use /register to register your nickname", name);
		}
	} else {
		// player already authenticated
		SendClientMessage(playerid, COLOR_REG, "* You are already logged in.");
	}
}

public REG_printPlayerStats(const playerid) {
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	if(REG_isPlayerAuthenticated(playerid) == TRUE) {
		SendClientMessage(playerid, COLOR_REG, "=======================================");
		SendClientMessageFormatted(playerid, COLOR_REG, "* Player stats for player: %s", name);
		SendClientMessageFormatted(playerid, COLOR_REG, "* Player level: %s",REG_getPropertyOfPlayer(playerid, PLAYER_PROP_LEVEL));
		SendClientMessageFormatted(playerid, COLOR_REG, "* Player score: %s",REG_getPropertyOfPlayer(playerid, PLAYER_PROP_SCORE));
		SendClientMessageFormatted(playerid, COLOR_REG, "* Player money: %s",REG_getPropertyOfPlayer(playerid, PLAYER_PROP_MONEY));
		SendClientMessage(playerid, COLOR_REG, "=======================================");
		
	} else {
		SendClientMessage(playerid, COLOR_REG, "* You are not authenticated, Please use /login to authenticate, of /register new accout");
	}
}

public REG_getPlayerLevel(const playerid) {
	return GetPVarInt(playerid, PLAYER_VAR_LEVEL);
}

public REG_setPlayerLevel(const playerid, const level) {
	SetPVarInt(playerid, PLAYER_VAR_LEVEL, level);
	REG_updatePropertyOfPlayer(playerid, PLAYER_PROP_LEVEL, intToStr(GetPVarInt(playerid, PLAYER_VAR_LEVEL)));
}

/** save stats timer function */
public REG_savePlayerDataTimerFunc() {
	REG_savePlayersData();
}

stock REG_isPlayerRegistered(const playerid) {
	new player_name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, player_name, sizeof(player_name));
	new DBResult:db_result = DBUTILS_runNamedQuery("getPlayerProperty","ss",PLAYER_PROP_NAME,player_name);
	if(DBUTILS_getResultSize(db_result) > 0) {
		DBUTILS_clearResults(db_result);
		return TRUE;
	} else {
		DBUTILS_clearResults(db_result);
		return FALSE;
	}
}

stock REG_loadPlayerData(const playerid) {
	SetPlayerScore(playerid, strToInt(REG_getPropertyOfPlayer(playerid, PLAYER_PROP_SCORE)));
	GivePlayerMoney(playerid, strToInt(REG_getPropertyOfPlayer(playerid, PLAYER_PROP_MONEY)));
	SetPVarInt(playerid, PLAYER_VAR_LEVEL, strToInt(REG_getPropertyOfPlayer(playerid, PLAYER_PROP_LEVEL)));
}

stock REG_savePlayersData() {
	for(FOR_PLAYERS) {
		if(IsPlayerConnected(i) && REG_isPlayerAuthenticated(i)) {
			REG_saveSinglePlayerData(i);
		}
	}
}

stock REG_saveSinglePlayerData(const playerid) {
	REG_updatePropertyOfPlayer(playerid, PLAYER_PROP_SCORE, intToStr(GetPlayerScore(playerid)));
	REG_updatePropertyOfPlayer(playerid, PLAYER_PROP_MONEY, intToStr(GetPlayerMoney(playerid)));
	REG_updatePropertyOfPlayer(playerid, PLAYER_PROP_LEVEL, intToStr(GetPVarInt(playerid, PLAYER_VAR_LEVEL)));
	SendClientMessage(playerid, COLOR_GREEN, "* Your player stats has been saved to the database.");
}

stock REG_isPlayerAuthenticated(const playerid) {
	if(GetPVarInt(playerid, PLAYER_VAR_LEVEL) > 0) return TRUE;
	return FALSE;
}

stock REG_getPropertyOfPlayer(const playerid, const property[]) {
	new player_name[MAX_PLAYER_NAME], result[32];
   	GetPlayerName(playerid, player_name, sizeof(player_name));
	new DBResult:db_result = DBUTILS_runNamedQuery("getPlayerProperty","ss",property,player_name);
	DBUTILS_getString(db_result, property, result, sizeof(result));
	DBUTILS_clearResults(db_result);
	return result;
}

stock REG_updatePropertyOfPlayer(const playerid, const property[], const property_value[]) {
	new player_name[MAX_PLAYER_NAME];
   	GetPlayerName(playerid, player_name, sizeof(player_name));
	DBUTILS_clearResults(DBUTILS_runNamedQuery("updatePlayerPropertyString","sss",property,property_value,player_name));
}

MODULE_FUNCTION(OnGameModeExit()) {}
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
