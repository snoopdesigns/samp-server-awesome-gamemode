#if defined _reg_included
	#endinput
#endif
#define _reg_included

#include <a_samp>
#include <a_sampdb>
#include "include/awesome/a_dbutils.inc"
#include "include/awesome/a_consts.inc"
#include "include/awesome/a_utils.inc"

//player vars
#define PLAYER_VAR_LEVEL "LEVEL"

//interval of saving player data to DB
#define DB_SAVE_INTERVAL 60000

#define PLAYER_PROP_PASSWORD "PlayerPassword"
#define PLAYER_PROP_IP "PlayerIP"
#define PLAYER_PROP_LEVEL "PlayerLevel"
#define PLAYER_PROP_SCORE "PlayerScore"
#define PLAYER_PROP_MONEY "PlayerMoney"

forward REG_savePlayerDataTimerFunc();

new DB:db_handle_reg;

stock REG_initRegSystem(db_handle)
{
	db_handle_reg = db_handle;
	//create player info table
	//DBUTILS_runDropQuery(db_handle_reg, "players"); //uncomment this to drop table every server restart
	new createTablePlayersQuery[512];
	format(createTablePlayersQuery, sizeof createTablePlayersQuery, "CREATE TABLE IF NOT EXISTS `players`(%s, %s, %s, %s, %s, %s)",
		"`PlayerName` VARCHAR(24) PRIMARY KEY NOT NULL",
		"'PlayerPassword' VARCHAR(32) NOT NULL",
		"'PlayerIP' VARCHAR(32) NOT NULL",
		"'PlayerLevel' VARCHAR(32) NOT NULL",
		"'PlayerScore' VARCHAR(32) NOT NULL",
		"'PlayerMoney' VARCHAR(32) NOT NULL");
	//"CREATE TABLE IF NOT EXISTS `players`(`PlayerName` PRIMARY KEY VARCHAR(24) NOT NULL, 'PlayerPassword' VARCHAR(32) NOT NULL, 'PlayerIP' VARCHAR(32) NOT NULL, 'PlayerScore' VARCHAR(32) NOT NULL)";
	DBUTILS_runCreateQuery(db_handle_reg, createTablePlayersQuery);
	SetTimer("REG_savePlayerDataTimerFunc", DB_SAVE_INTERVAL, true);
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
	SendClientMessage(playerid, TEXT_COLOR_GREEN, "Your stats has been successfully saved to database.");
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
			"0",
			intToStr(GetPlayerScore(playerid)),
			intToStr(GetPlayerMoney(playerid)));
		DBUTILS_runInsertQuery(db_handle_reg, query);
		if(REG_checkPlayerRegistered(playerid) == TRUE)
		{
			// player successfully registered
			SetPVarInt(playerid, PLAYER_VAR_LEVEL, 1);
			SendClientMessage(playerid, TEXT_COLOR_GREEN, "Registerred successfully!");
		}
	} 
	else 
	{
		if(REG_isPlayerAuthenticated(playerid) == TRUE)
		{
			// player is autheticated and requests register - bad
			SendClientMessage(playerid, TEXT_COLOR_GREEN, "You are already registered.");
		} 
		else 
		{
			// player is not autheticated and requests register under existing name - bad
			new string[256];
			format(string, sizeof string, "This nickname: %s is registered already. If this is you nickname, please /login, otherwise please choose other nickname", name);
			SendClientMessage(playerid, TEXT_COLOR_GREEN, string);
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
			new string[128];
			format(string, sizeof string, "pwd from DB: '%s', entered: '%s'", password_from_db, password);
			SendClientMessage(playerid, TEXT_COLOR_GREEN, string);
			if(strcmp(password_from_db, password) == 0)
			{
				SendClientMessage(playerid, TEXT_COLOR_GREEN, "Login using password successfull.");
				//restore player props from db
				REG_restoreSinglePlayerData(playerid);
				//restore player props from db end
			} else {
				SendClientMessage(playerid, TEXT_COLOR_GREEN, "Login using password unsuccessfull.");
			}
		}
		else 
		{
			// players nickname is not registered in db
			new string[256];
			format(string, sizeof string, "This nickname: %s is not registered. Please use /register to register your nickname", name);
			SendClientMessage(playerid, TEXT_COLOR_GREEN, string);
		}
	}
	else 
	{
		// player already authenticated
		SendClientMessage(playerid, TEXT_COLOR_GREEN, "You are already logged in.");
	}
}

stock REG_checkPlayerRegistered(playerid)
{
	new player_name[MAX_PLAYER_NAME], ip[128];
	GetPlayerName(playerid, player_name, sizeof(player_name));
	static query[256], DBResult: db_result;
	format(query, sizeof query, "SELECT * FROM `players` WHERE `PlayerName`='%s';", player_name);
	db_result = DBUTILS_runSelectQuery(db_handle_reg, query);
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

stock REG_authenticatePlayerOnConnect(playerid)
{
	SetPVarInt(playerid, PLAYER_VAR_LEVEL, 0); // set default level to 0
	
	new player_ip[128];
   	GetPlayerIp(playerid, player_ip, sizeof(player_ip));
	
	if(REG_checkPlayerRegistered(playerid) == TRUE)
	{
		//player with this name is registered
		new ip_from_db[128];
		ip_from_db = REG_getPropertyOfPlayer(playerid, PLAYER_PROP_IP);
		if(strcmp(ip_from_db, player_ip) == 0)
		{
			REG_restoreSinglePlayerData(playerid);
			SendClientMessage(playerid, TEXT_COLOR_GREEN, "IP auto-login successfull");
		} else {
			SendClientMessage(playerid, TEXT_COLOR_GREEN, "IP auto-login failed. Please type login using your password.");
		}
	}
	else 
	{
		SendClientMessage(playerid, TEXT_COLOR_GREEN, "Your name is not registered. Please register using /register [password]");
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
	db_result = DBUTILS_runSelectQuery(db_handle_reg, query);
	result = DBUTILS_getFieldValueFromResultSet(db_result, property);
	DBUTILS_runSelectQueryClear(db_result); // clear db results
	return result;
}

stock REG_updatePropertyOfPlayer(playerid, property[], property_value[])
{
	new player_name[MAX_PLAYER_NAME], result[128];
   	GetPlayerName(playerid, player_name, sizeof(player_name));
	static query[256], DBResult: db_result;
	format(query, sizeof query, "UPDATE `players` SET '%s'='%s' WHERE `PlayerName`='%s';", property, property_value, player_name);
	DBUTILS_runUpdateQuery(db_handle_reg, query);
}