#if defined _race_included
	#endinput
#endif
#define _race_included

#include "include/awesome/a_utils.inc"
#include "include/awesome/colors.inc"

forward RACE_initNewRace();
forward RACE_startRace();
forward RACE_finishRace();

#define RACES_INTERVAL 4 * 60 * 1000 // 3 minutes
#define RACES_START_DELAY 15000 // 15 secs

#define RACE_STATUS_INACTIVE 0 
#define RACE_STATUS_WAITING_PLAYERS 1 
#define RACE_STATUS_ACTIVE 2 

enum raceCheckpointInfo
{
	num,
	Float: x,
	Float: y,
	Float: z
}

new raceCheckpoints[100][raceCheckpointInfo];
new totalRaceCheckpoints = -1;

new playersInRace[MAX_PLAYERS];
new playerInRaceProgress[MAX_PLAYERS];

new raceStatus;
new raceCountdownTimer = -1;
new raceCountdownTimerCount;
new DB:db_handle_race;

//race creation
#define RACE_CREATION_STATUS_INACTIVE 0
#define RACE_CREATION_STATUS_INPROGRESS 1

new raceCreationStatus = RACE_CREATION_STATUS_INACTIVE;
new File: raceCreationFile;
new raceCreationFileName[64];

//TODO check if active player dead or leave the game to finish race
//TODO create player level race method
//TODO race textdraw (position, time)
//TODO race status (times and positions)

stock RACE_initRaceSystem(db_handle)
{
	db_handle_race = db_handle;
	new createTableRaceQuery[512];
	format(createTableRaceQuery, sizeof createTableRaceQuery, "CREATE TABLE IF NOT EXISTS `race`(%s, %s, %s, %s, %s, %s)",
		"`ID` INTEGER PRIMARY KEY NOT NULL",
		"'RaceName' VARCHAR(32) NOT NULL",
		"'CreatorName' VARCHAR(32) NOT NULL",
		"'RecordTime' INTEGER NOT NULL",
		"'RecordHolder' VARCHAR(32) NOT NULL",
		"'filename' VARCHAR(32) NOT NULL");
	DBUTILS_runCreateQuery(db_handle_race, createTableRaceQuery);
	
	SetTimer("RACE_initNewRace", 10000, 0);
	raceStatus = RACE_STATUS_INACTIVE;
}

RACE_createRace(playerid, filename[])
{
	if(raceCreationStatus == RACE_CREATION_STATUS_INACTIVE)
	{
		raceCreationStatus = RACE_CREATION_STATUS_INPROGRESS;
		SendClientMessage(playerid, COLOR_IMPROVEMENT, "* Start creating new race");
		new Float:xpos, Float:ypos, Float:zpos;
		GetPlayerPos(playerid, xpos, ypos, zpos);
		new str[64];
		format(str, sizeof str, "%f, %f, %f ;\n", xpos, ypos, zpos);
		SendClientMessage(playerid, COLOR_MG, str);
		format(raceCreationFileName, sizeof raceCreationFileName, "races/%s", filename);
		raceCreationFile = fopen(raceCreationFileName, filemode:io_append);
		if(!raceCreationFile) 
		{
			SendClientMessage(playerid, COLOR_ERROR, "* Error during creation checkpoints file!");
			raceCreationStatus = RACE_CREATION_STATUS_INACTIVE;
			return 0;
		}
		fwrite(raceCreationFile, str);
	}
	else 
	{
		SendClientMessage(playerid, COLOR_ERROR, "* Race already under creation");
	}
}

RACE_createRaceCheck(playerid)
{
	if(raceCreationStatus == RACE_CREATION_STATUS_INPROGRESS)
	{
		SendClientMessage(playerid, COLOR_IMPROVEMENT, "* Race checkpoint added");
		new Float:xpos, Float:ypos, Float:zpos;
		GetPlayerPos(playerid, xpos, ypos, zpos);
		new str[64];
		format(str, sizeof str, "%f, %f, %f ;\n", xpos, ypos, zpos);
		SendClientMessage(playerid, COLOR_MG, str);
		fwrite(raceCreationFile, str);
	}
	else 
	{
		SendClientMessage(playerid, COLOR_ERROR, "* No active race creations.. Type /cr <filename> to start race creation");
	}
}

RACE_createRaceEnd(playerid, racename[])
{
	if(raceCreationStatus == RACE_CREATION_STATUS_INPROGRESS)
	{
		raceCreationStatus = RACE_CREATION_STATUS_INACTIVE;
		fclose(raceCreationFile);
		
		new player_name[MAX_PLAYER_NAME];
		GetPlayerName(playerid, player_name, sizeof player_name);
		
		static query[256];
		format(query, sizeof query, "INSERT INTO `race` (`RaceName`, `CreatorName`, `RecordTime`, `RecordHolder`, `filename`) VALUES ('%s','%s',0,'','%s')",
			racename,
			player_name,
			raceCreationFileName);
		DBUTILS_runInsertQuery(db_handle_props, query);
		
		SendClientMessage(playerid, COLOR_IMPROVEMENT, "* New race successfully created");
		new msg[128];
		format(msg, sizeof msg, "* Race name: '%s', filename: '%s'", racename, raceCreationFileName);
		SendClientMessage(playerid, COLOR_IMPROVEMENT, msg);
	}
	else 
	{
		SendClientMessage(playerid, COLOR_ERROR, "* No active race creations.. Type /cr <filename> to start race creation");
	}
}

public RACE_initNewRace()
{
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		playersInRace[i] = -1;
		playerInRaceProgress[i] = -1;
	}
	
	for(new i = 0; i < 100; i++)
	{
		raceCheckpoints[i][num] = -1;
		raceCheckpoints[i][x] = -1;
		raceCheckpoints[i][y] = -1;
		raceCheckpoints[i][z] = -1;
	}
	totalRaceCheckpoints = -1;
	
	static DBResult: db_result;
	db_result = DBUTILS_runSelectQuery(db_handle_reg, "SELECT * FROM `race`");
	new racesCounts = DBUTILS_getResultsetSize(db_result);
	if(racesCounts == 0) 
	{
		raceStatus = RACE_STATUS_INACTIVE;
		SetTimer("RACE_initNewRace", RACES_INTERVAL, 0);
		return;
	}
	new rand = random(racesCounts) + 1;
	static query[256];
	new racename[128], filename[128];
	format(query, sizeof query, "SELECT * FROM `race` WHERE `ID`='%d';", rand);
	db_result = DBUTILS_runSelectQuery(db_handle_reg, query);
	racename = DBUTILS_getFieldValueFromResultSet(db_result, "RaceName");
	filename = DBUTILS_getFieldValueFromResultSet(db_result, "filename");
	new msg[256];
	format(msg, sizeof msg, "** Race '%s' is about to start... type /join if you wish to join", racename);
	SendClientMessageToAll(COLOR_MG, msg);
	SendClientMessageToAll(COLOR_MG, "* Total checkpoints: 19 | Length: 5.51km");//TODO
	SendClientMessageToAll(COLOR_MG, "* Current race record holder is 'SnooP', time is '03m13s'"); //TODO
	SendClientMessageToAll(COLOR_MG, "* Vehicle: ANY");//TODO
	
	totalRaceCheckpoints = RACE_loadRaceFromFile(filename);
	raceStatus = RACE_STATUS_WAITING_PLAYERS;
	SetTimer("RACE_startRace", RACES_START_DELAY, 0);
	
	DBUTILS_runSelectQueryClear(db_result); // clear db results
}

public RACE_startRace()
{
	new playerInRaceRemaining = 0;
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if(playersInRace[i] != -1)
		{
			playerInRaceRemaining ++;
		}
	}
	if(playerInRaceRemaining > 0)
	{
		//SendClientMessageToAll(COLOR_MG, "* Race starting..."); // TODO send only to participants
		raceStatus = RACE_STATUS_ACTIVE;
		raceCountdownTimerCount = 5;
		raceCountdownTimer = SetTimer("RACE_CountDown", 1000, 1);
	}
	else 
	{
		SendClientMessageToAll(COLOR_MG, "* Race not started beacause nobody comes");
		raceStatus = RACE_STATUS_INACTIVE;
		SetTimer("RACE_initNewRace", RACES_INTERVAL, 0);
	}
}

public RACE_finishRace()
{
	raceStatus = RACE_STATUS_INACTIVE;
	SetTimer("RACE_initNewRace", RACES_INTERVAL, 0);
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if(playersInRace[i] != -1)
		{
			DisablePlayerRaceCheckpoint(i);
		}
	}
}

public RACE_CountDown()
{
	if(raceCountdownTimerCount == 0) 
	{
		KillTimer(raceCountdownTimer);
		for (new i=0; i<MAX_PLAYERS; i++)
		{
			if(playersInRace[i] != -1 && IsPlayerConnected(i))
			{
				GameTextForPlayer(i, "GO", 1000, 3);
				TogglePlayerControllable(i,1);
				DisablePlayerRaceCheckpoint(i);
				playerInRaceProgress[i] = 0;
				SetPlayerRaceCheckpoint(i, 0, raceCheckpoints[0][x], raceCheckpoints[0][y], raceCheckpoints[0][z],
                         raceCheckpoints[1][x], raceCheckpoints[1][y], raceCheckpoints[1][z], 10);
			}
		}
		return 1;
	}

	new string[128];
	format(string, sizeof(string), "%d", raceCountdownTimerCount);
	for (new i=0; i<MAX_PLAYERS; i++)
	{
	    if(playersInRace[i] != -1 && IsPlayerConnected(i))
    	{
			GameTextForPlayer(i, string, 1000, 3);
			TogglePlayerControllable(i,0);
		}
	}

	raceCountdownTimerCount--;
	return 1;
}

stock RACE_join(playerid)
{
	if(raceStatus == RACE_STATUS_WAITING_PLAYERS)
	{
		if(playersInRace[playerid] == -1)
		{
			SendClientMessage(playerid, COLOR_MG, "* You joined race, it is starting soon.");
			playersInRace[playerid] = 1;
			SetPlayerPos(playerid, raceCheckpoints[0][x], raceCheckpoints[0][y], raceCheckpoints[0][z]);
			SetPlayerRaceCheckpoint(playerid,0,raceCheckpoints[0][x], raceCheckpoints[0][y], raceCheckpoints[0][z],
                         raceCheckpoints[1][x], raceCheckpoints[1][y], raceCheckpoints[1][z], 10);
		}
		else 
		{
			SendClientMessage(playerid, COLOR_ERROR, "* You are already in race.");
		}
	}
	else 
	{
		if(raceStatus == RACE_STATUS_ACTIVE)
		{
			SendClientMessage(playerid, COLOR_ERROR, "* Race already started!");
		} 
		else if(raceStatus == RACE_STATUS_INACTIVE)
		{
			SendClientMessage(playerid, COLOR_ERROR, "* There is no active race for now");
		}
	}
}

stock RACE_OnPlayerEnterRaceCheckpoint(playerid)
{
	if(playersInRace[playerid] != -1 && raceStatus == RACE_STATUS_ACTIVE)
	{
		DisablePlayerRaceCheckpoint(playerid);
		if(playerInRaceProgress[playerid] == totalRaceCheckpoints - 1)
		{
			SendClientMessage(playerid, COLOR_MG, "* You finished the race!"); //TODO get position
			playersInRace[playerid] = -1;
			DisablePlayerRaceCheckpoint(playerid);
			new playerInRaceRemaining = 0;
			for(new i = 0; i < MAX_PLAYERS; i++)
			{
				if(playersInRace[i] != -1)
				{
					playerInRaceRemaining ++;
				}
			}
			if(playerInRaceRemaining == 0)
			{
				RACE_finishRace();
			}
		}
		else 
		{
			playerInRaceProgress[playerid] ++;
			if(totalRaceCheckpoints - 1 == playerInRaceProgress[playerid])
			{
				GameTextForPlayer(playerid, "~r~LAST CHECKPOINT...", 1000, 3);
				SetPlayerRaceCheckpoint(playerid,1,raceCheckpoints[playerInRaceProgress[playerid]][x], raceCheckpoints[playerInRaceProgress[playerid]][y], raceCheckpoints[playerInRaceProgress[playerid]][z],
					raceCheckpoints[playerInRaceProgress[playerid]+1][x], raceCheckpoints[playerInRaceProgress[playerid]+1][y], raceCheckpoints[playerInRaceProgress[playerid]+1][z], 10);
			}
			else 
			{
				SetPlayerRaceCheckpoint(playerid,0,raceCheckpoints[playerInRaceProgress[playerid]][x], raceCheckpoints[playerInRaceProgress[playerid]][y], raceCheckpoints[playerInRaceProgress[playerid]][z],
					raceCheckpoints[playerInRaceProgress[playerid]+1][x], raceCheckpoints[playerInRaceProgress[playerid]+1][y], raceCheckpoints[playerInRaceProgress[playerid]+1][z], 10);
			}
		}
	}
}

stock RACE_loadRaceFromFile(const filename[])
{
	new File:file_ptr;
	new line[256];
	new var_from_line[64];
	new Float:X;
	new Float:Y;
	new Float:Z;
	new index;
	new checkpoints_loaded;

	file_ptr = fopen(filename,filemode:io_read);
	if(!file_ptr) return 0;

	checkpoints_loaded = 0;

	while(fread(file_ptr,line,256) > 0)
	{
	    index = 0;
	    
  		index = token_by_delim(line,var_from_line,',',index);
  		if(index == (-1)) continue;
  		X = floatstr(var_from_line);

  		index = token_by_delim(line,var_from_line,',',index+1);
  		if(index == (-1)) continue;
  		Y = floatstr(var_from_line);

  		index = token_by_delim(line,var_from_line,',',index+1);
  		Z = floatstr(var_from_line);
		
		raceCheckpoints[checkpoints_loaded][num] = checkpoints_loaded;
		raceCheckpoints[checkpoints_loaded][x] = X;
		raceCheckpoints[checkpoints_loaded][y] = Y;
		raceCheckpoints[checkpoints_loaded][z] = Z;
		checkpoints_loaded++;
	}

	fclose(file_ptr);
	return checkpoints_loaded;
}
