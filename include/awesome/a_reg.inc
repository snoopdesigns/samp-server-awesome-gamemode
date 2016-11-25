#if defined _reg_included
	#endinput
#endif
#define _reg_included

#include <a_samp>
#include <a_sampdb>
#include "include/awesome/a_dbutils.inc"
#include "include/awesome/a_consts.inc"
#include "include/awesome/a_utils.inc"
#include "include/awesome/colors.inc"

#define MODULE_FUNCTION(%1) forward REG_%1; public REG_%1

//player vars
#define PLAYER_VAR_LEVEL "LEVEL"

//interval of saving player data to DB
#define DB_SAVE_INTERVAL 5 * 60 * 1000 //5 minutes

#define PLAYER_PROP_PASSWORD "PlayerPassword"
#define PLAYER_PROP_IP "PlayerIP"
#define PLAYER_PROP_LEVEL "PlayerLevel"
#define PLAYER_PROP_SCORE "PlayerScore"
#define PLAYER_PROP_MONEY "PlayerMoney"

/* public timer callbacks */
forward REG_savePlayerDataTimerFunc();

MODULE_FUNCTION(OnGameModeInit())
{
	LOG_write("Registration Subsystem initializing");
	//create player info table
	//DBUTILS_runDropQuery("players"); //uncomment this to drop table every server restart
	new createTablePlayersQuery[512];
	format(createTablePlayersQuery, sizeof createTablePlayersQuery, "CREATE TABLE IF NOT EXISTS `players`(%s, %s, %s, %s, %s, %s)",
		"`PlayerName` VARCHAR(24) PRIMARY KEY NOT NULL",
		"'PlayerPassword' VARCHAR(32) NOT NULL",
		"'PlayerIP' VARCHAR(32) NOT NULL",
		"'PlayerLevel' VARCHAR(32) NOT NULL",
		"'PlayerScore' VARCHAR(32) NOT NULL",
		"'PlayerMoney' VARCHAR(32) NOT NULL");
	//"CREATE TABLE IF NOT EXISTS `players`(`PlayerName` PRIMARY KEY VARCHAR(24) NOT NULL, 'PlayerPassword' VARCHAR(32) NOT NULL, 'PlayerIP' VARCHAR(32) NOT NULL, 'PlayerScore' VARCHAR(32) NOT NULL)";
	DBUTILS_runCreateQuery(createTablePlayersQuery);
	SetTimer("REG_savePlayerDataTimerFunc", DB_SAVE_INTERVAL, true);
}

MODULE_FUNCTION(OnPlayerConnect(playerid))
{
	LOG_writeFormatted("Called OnPlayerConnect() in REG, playerid = %d", playerid);
	SetPVarInt(playerid, PLAYER_VAR_LEVEL, 0); // set default level to 0
	
	new player_ip[128], player_name[256];
   	GetPlayerIp(playerid, player_ip, sizeof(player_ip));
	GetPlayerName(playerid, player_name, sizeof(player_name));
	
	if(REG_checkPlayerRegistered(playerid) == TRUE)
	{
		//player with this name is registered
		new ip_from_db[128];
		ip_from_db = REG_getPropertyOfPlayer(playerid, PLAYER_PROP_IP);
		if(strcmp(ip_from_db, player_ip) == 0)
		{
			REG_restoreSinglePlayerData(playerid);
			SendClientMessageFormatted(playerid, COLOR_REG, "* IP auto-login successfull. Welcome back, %s!", player_name);
		} else {
			SendClientMessage(playerid, COLOR_ERROR, "* IP auto-login failed. Please /login using your password.");
		}
	}
	else 
	{
		SendClientMessage(playerid, COLOR_REG, "* Your name is not registered. Please register using /register <password>");
	}
}

MODULE_FUNCTION(OnPlayerDisconnect(playerid, reason)) {
	LOG_writeFormatted("Called OnPlayerDisconnect() in REG, playerid = %d, reason = %d", playerid, reason);
}

public REG_savePlayerDataTimerFunc()
{
	REG_savePlayersData();
}

stock REG_savePlayersData()
{
	for(new i; i < MAX_PLAYERS; i++)
	{
		if(IsPlayerConnected(i) && REG_isPlayerAuthenticated(i))
		{
			REG_saveSinglePlayerData(i);
		}
	}
}

stock REG_saveSinglePlayerData(playerid)
{
	REG_updatePropertyOfPlayer(playerid, PLAYER_PROP_SCORE, intToStr(GetPlayerScore(playerid)));
	REG_updatePropertyOfPlayer(playerid, PLAYER_PROP_MONEY, intToStr(GetPlayerMoney(playerid)));
	REG_updatePropertyOfPlayer(playerid, PLAYER_PROP_LEVEL, intToStr(GetPVarInt(playerid, PLAYER_VAR_LEVEL)));
	SendClientMessage(playerid, COLOR_GREEN, "* Your player stats has been saved to the database.");
}

stock REG_restoreSinglePlayerData(playerid)
{
	SetPlayerScore(playerid, strToInt(REG_getPropertyOfPlayer(playerid, PLAYER_PROP_SCORE)));
	GivePlayerMoney(playerid, strToInt(REG_getPropertyOfPlayer(playerid, PLAYER_PROP_MONEY)));
	SetPVarInt(playerid, PLAYER_VAR_LEVEL, strToInt(REG_getPropertyOfPlayer(playerid, PLAYER_PROP_LEVEL)));
}

stock REG_registerNewPlayer(playerid, password[])
{
	new name[MAX_PLAYER_NAME], ip[128];
	GetPlayerName(playerid, name, sizeof(name));
	GetPlayerIp(playerid, ip, sizeof(ip));
	if(REG_checkPlayerRegistered(playerid) == FALSE) 
	{
		static query[256];
		format(query, sizeof query, "INSERT INTO `players` (`PlayerName`,`PlayerPassword`, `PlayerIP`, `PlayerLevel`, `PlayerScore`, `PlayerMoney`) VALUES ('%s','%s','%s','%s','%s','%s')", 
			name,
			password,
			ip,
			"1",
			intToStr(GetPlayerScore(playerid)),
			intToStr(GetPlayerMoney(playerid)));
		DBUTILS_runInsertQuery(query);
		if(REG_checkPlayerRegistered(playerid) == TRUE)
		{
			// player successfully registered
			SetPVarInt(playerid, PLAYER_VAR_LEVEL, 1);
			SendClientMessageFormatted(playerid, COLOR_REG, "* New account %s has been registerred successfully", name);
		}
	} 
	else 
	{
		if(REG_isPlayerAuthenticated(playerid) == TRUE)
		{
			// player is autheticated and requests register - bad
			SendClientMessage(playerid, COLOR_ERROR, "* You are already registered.");
		} 
		else 
		{
			// player is not autheticated and requests register under existing name - bad
			SendClientMessageFormatted(playerid, COLOR_REG, "* This nickname: '%s' is registered already. If this is you nickname, please /login, otherwise please choose other nickname", name);
		}
	}
}

stock REG_loginPlayer(playerid, password[])
{
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	
	if(REG_isPlayerAuthenticated(playerid) == FALSE)
	{
		if(REG_checkPlayerRegistered(playerid) == TRUE) 
		{
			new password_from_db[128];
			password_from_db = REG_getPropertyOfPlayer(playerid, PLAYER_PROP_IP);
			if(strcmp(password_from_db, password) == 0)
			{
				SendClientMessage(playerid, COLOR_LAWNGREEN, "* Login using password successfull.");
				//restore player props from db
				REG_restoreSinglePlayerData(playerid);
				//restore player props from db end
			} else {
				SendClientMessage(playerid, COLOR_ERROR, "* Login using password unsuccessfull.");
			}
		}
		else 
		{
			// players nickname is not registered in db
			SendClientMessageFormatted(playerid, COLOR_REG, "* Your nickname: '%s' is not registered. Please use /register to register your nickname", name);
		}
	}
	else 
	{
		// player already authenticated
		SendClientMessage(playerid, COLOR_REG, "* You are already logged in.");
	}
}

stock REG_checkPlayerRegistered(playerid)
{
	new player_name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, player_name, sizeof(player_name));
	static query[256], DBResult: db_result;
	format(query, sizeof query, "SELECT * FROM `players` WHERE `PlayerName`='%s';", player_name);
	db_result = DBUTILS_runSelectQuery(query);
	if(DBUTILS_getResultsetSize(db_result) > 0)
	{
		DBUTILS_runSelectQueryClear(db_result);
		return TRUE;
	}
	else 
	{
		DBUTILS_runSelectQueryClear(db_result);
		return FALSE;
	}
}

stock REG_isPlayerAuthenticated(playerid)
{
	if(GetPVarInt(playerid, PLAYER_VAR_LEVEL) > 0) return TRUE;
	return FALSE;
}

stock REG_getPlayerLevel(playerid)
{
	return GetPVarInt(playerid, PLAYER_VAR_LEVEL);
}

stock REG_getPropertyOfPlayer(playerid, property[])
{
	new player_name[MAX_PLAYER_NAME], result[128];
   	GetPlayerName(playerid, player_name, sizeof(player_name));
	static query[256], DBResult: db_result;
	format(query, sizeof query, "SELECT * FROM `players` WHERE `PlayerName`='%s';", player_name);
	db_result = DBUTILS_runSelectQuery(query);
	result = DBUTILS_getFieldFromResult(db_result, property);
	DBUTILS_runSelectQueryClear(db_result); // clear db results
	return result;
}

stock REG_updatePropertyOfPlayer(playerid, property[], property_value[])
{
	new player_name[MAX_PLAYER_NAME];
   	GetPlayerName(playerid, player_name, sizeof(player_name));
	static query[256];
	format(query, sizeof query, "UPDATE `players` SET '%s'='%s' WHERE `PlayerName`='%s';", property, property_value, player_name);
	DBUTILS_runUpdateQuery(query);
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
