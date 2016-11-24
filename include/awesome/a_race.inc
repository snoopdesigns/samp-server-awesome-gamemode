#if defined _race_included
	#endinput
#endif
#define _race_included

#include "include/awesome/a_utils.inc"
#include "include/awesome/colors.inc"

#define MODULE_FUNCTION(%1) forward RACE_%1; public RACE_%1

forward RACE_initNewRace();
forward RACE_startRace();
forward RACE_finishRace();
forward RACE_CountDown();
forward RACE_playerTdTimer();

#define RACES_INTERVAL 1 * 10 * 1000 // 4 minutes
#define RACES_START_DELAY 20000 // 20 secs

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
new raceEntryPoint[1][raceCheckpointInfo];
new totalRaceCheckpoints = -1;
new raceVehicleModel = -1;
new currentRaceIdentifier = -1;

new currentRaceStartTime;

new playersInRace[MAX_PLAYERS];
new playerInRaceProgress[MAX_PLAYERS];
new playersInRaceFinished = 0;

new PlayerText: playersInRaceTextdraw[MAX_PLAYERS];
new playersInRaceTextdrawTimer = -1;

new raceStatus;
new raceCountdownTimer = -1;
new raceCountdownTimerCount;

//race creation
#define RACE_CREATION_STATUS_INACTIVE 0
#define RACE_CREATION_STATUS_INPROGRESS 1

new raceCreationStatus = RACE_CREATION_STATUS_INACTIVE;
new File: raceCreationFile;
new raceCreationFileName[64];

//TODO check if active player dead or leave the game to finish race
//TODO create player leave race method

MODULE_FUNCTION(OnGameModeInit())
{
	LOG_write("Race Subsystem initializing");
	new createTableRaceQuery[512];
	format(createTableRaceQuery, sizeof createTableRaceQuery, "CREATE TABLE IF NOT EXISTS `race`(%s, %s, %s, %s, %s, %s, %s)",
		"`ID` INTEGER PRIMARY KEY NOT NULL",
		"'RaceName' VARCHAR(32) NOT NULL",
		"'CreatorName' VARCHAR(32) NOT NULL",
		"'RecordTime' INTEGER NOT NULL",
		"'RecordHolder' VARCHAR(32) NOT NULL",
		"'VehicleModel' INTEGER NOT NULL",
		"'filename' VARCHAR(32) NOT NULL");
	DBUTILS_runCreateQuery(createTableRaceQuery);
	
	SetTimer("RACE_initNewRace", RACES_INTERVAL, 0);
	raceStatus = RACE_STATUS_INACTIVE;
	
	//init player vars
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		playersInRace[i] = -1;
		playerInRaceProgress[i] = -1;
		playersInRaceTextdraw[i] = PlayerText: -1;
	}
}

MODULE_FUNCTION(OnPlayerEnterRaceCheckpoint(playerid))
{
	if(playersInRace[playerid] != -1 && raceStatus == RACE_STATUS_ACTIVE)
	{
		DisablePlayerRaceCheckpoint(playerid);
		if(playerInRaceProgress[playerid] == totalRaceCheckpoints - 1)
		{
			RACE_playerFinishRace(playerid, 0);
		}
		else 
		{
			playerInRaceProgress[playerid] ++;
			if(totalRaceCheckpoints - 1 == playerInRaceProgress[playerid])
			{
				GameTextForPlayer(playerid, "~r~LAST CHECKPOINT...", 1000, 3);
				SetPlayerRaceCheckpoint(playerid,1,raceCheckpoints[playerInRaceProgress[playerid]][x], raceCheckpoints[playerInRaceProgress[playerid]][y], raceCheckpoints[playerInRaceProgress[playerid]][z],
					raceCheckpoints[playerInRaceProgress[playerid]+1][x], raceCheckpoints[playerInRaceProgress[playerid]+1][y], raceCheckpoints[playerInRaceProgress[playerid]+1][z], 10);
				for(new i = 0; i < MAX_PLAYERS; i++)
				{
					if(playersInRace[i] != -1)
					{
						new msgFinish[128];
						format(msgFinish, sizeof msgFinish, "~r~%s ~n~~y~is about to cross ~n~the finish line", GetPlayerFormattedName(i));
						GameTextForPlayer(i, msgFinish, 1000, 3); 
					}
				}
			}
			else 
			{
				SetPlayerRaceCheckpoint(playerid,0,raceCheckpoints[playerInRaceProgress[playerid]][x], raceCheckpoints[playerInRaceProgress[playerid]][y], raceCheckpoints[playerInRaceProgress[playerid]][z],
					raceCheckpoints[playerInRaceProgress[playerid]+1][x], raceCheckpoints[playerInRaceProgress[playerid]+1][y], raceCheckpoints[playerInRaceProgress[playerid]+1][z], 10);
			}
		}
	}
}

MODULE_FUNCTION(OnPlayerDisconnect(playerid, reason))
{
	RACE_playerFinishRace(playerid, 1);
}

MODULE_FUNCTION(OnPlayerDeath(playerid, killerid, reason))
{
	SendClientMessage(playerid, COLOR_MG, "* You died and cannot finish the race.");
	RACE_playerFinishRace(playerid, 1);
}

stock RACE_createRace(playerid, filename[])
{
	if(raceCreationStatus == RACE_CREATION_STATUS_INACTIVE)
	{
		raceCreationStatus = RACE_CREATION_STATUS_INPROGRESS;
		SendClientMessage(playerid, COLOR_IMPROVEMENT, "* Start creating new race");
		new Float:xpos, Float:ypos, Float:zpos;
		GetPlayerPos(playerid, xpos, ypos, zpos);
		SendClientMessageFormatted(playerid, COLOR_IMPROVEMENT, "%f, %f, %f ;\n", xpos, ypos, zpos);
		format(raceCreationFileName, sizeof raceCreationFileName, "races/%s", filename);
		raceCreationFile = fopen(raceCreationFileName, filemode:io_append);
		if(!raceCreationFile) 
		{
			SendClientMessage(playerid, COLOR_ERROR, "* Error during creation checkpoints file!");
			raceCreationStatus = RACE_CREATION_STATUS_INACTIVE;
			return;
		}
		new str[64];
		format(str, sizeof str, "%f, %f, %f ;\n", xpos, ypos, zpos);
		fwrite(raceCreationFile, str);
	}
	else 
	{
		SendClientMessage(playerid, COLOR_ERROR, "* Race already under creation");
	}
}

stock RACE_createRaceCheck(playerid)
{
	if(raceCreationStatus == RACE_CREATION_STATUS_INPROGRESS)
	{
		SendClientMessage(playerid, COLOR_IMPROVEMENT, "* Race checkpoint added");
		new Float:xpos, Float:ypos, Float:zpos;
		GetPlayerPos(playerid, xpos, ypos, zpos);
		new str[64];
		format(str, sizeof str, "%f, %f, %f ;\n", xpos, ypos, zpos);
		SendClientMessageFormatted(playerid, COLOR_IMPROVEMENT, "%f, %f, %f ;\n", xpos, ypos, zpos);
		fwrite(raceCreationFile, str);
	}
	else 
	{
		SendClientMessage(playerid, COLOR_ERROR, "* No active race creations.. Type /cr <filename> to start race creation");
	}
}

stock RACE_createRaceEnd(playerid, racename[])
{
	if(raceCreationStatus == RACE_CREATION_STATUS_INPROGRESS)
	{
		raceCreationStatus = RACE_CREATION_STATUS_INACTIVE;
		fclose(raceCreationFile);
		
		new player_name[MAX_PLAYER_NAME];
		GetPlayerName(playerid, player_name, sizeof player_name);
		
		static query[256];
		format(query, sizeof query, "INSERT INTO `race` (`RaceName`, `CreatorName`, `RecordTime`, `RecordHolder`, 'VehicleModel', `filename`) VALUES ('%s','%s',0,'',411,'%s')",
			racename,
			player_name,
			raceCreationFileName);
		DBUTILS_runInsertQuery(query);
		
		SendClientMessage(playerid, COLOR_IMPROVEMENT, "* New race successfully created");
		SendClientMessageFormatted(playerid, COLOR_IMPROVEMENT, "* Race name: '%s', filename: '%s'", racename, raceCreationFileName);
	}
	else 
	{
		SendClientMessage(playerid, COLOR_ERROR, "* No active race creations.. Type /cr <filename> to start race creation");
	}
}

public RACE_initNewRace()
{
	currentRaceStartTime = 0;
	playersInRaceFinished = 0;
	currentRaceIdentifier = -1;
	raceVehicleModel = -1;
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		playersInRace[i] = -1;
		playerInRaceProgress[i] = -1;
		playersInRaceTextdraw[i] = PlayerText: -1;
	}
	
	for(new i = 0; i < 100; i++)
	{
		raceCheckpoints[i][num] = -1;
		raceCheckpoints[i][x] = -1;
		raceCheckpoints[i][y] = -1;
		raceCheckpoints[i][z] = -1;
	}
	raceEntryPoint[0][num] = -1;
	raceEntryPoint[0][x] = -1;
	raceEntryPoint[0][y] = -1;
	raceEntryPoint[0][z] = -1;
	totalRaceCheckpoints = -1;
	
	static DBResult: db_result;
	db_result = DBUTILS_runSelectQuery("SELECT * FROM `race`");
	new racesCounts = DBUTILS_getResultsetSize(db_result);
	if(racesCounts == 0) 
	{
		raceStatus = RACE_STATUS_INACTIVE;
		SetTimer("RACE_initNewRace", RACES_INTERVAL, 0);
		return;
	}
	currentRaceIdentifier = 2;//random(racesCounts) + 1;
	static query[256];
	new racename[128], filename[128], recordHolder[128], creatorName[128];
	new recordTime, vehicleModel;
	format(query, sizeof query, "SELECT * FROM `race` WHERE `ID`='%d';", currentRaceIdentifier);
	db_result = DBUTILS_runSelectQuery(query);
	racename = DBUTILS_getFieldFromResult(db_result, "RaceName");
	creatorName = DBUTILS_getFieldFromResult(db_result, "CreatorName");
	filename = DBUTILS_getFieldFromResult(db_result, "filename");
	recordHolder = DBUTILS_getFieldFromResult(db_result, "RecordHolder");
	recordTime = strToInt(DBUTILS_getFieldFromResult(db_result, "RecordTime"));
	vehicleModel = strToInt(DBUTILS_getFieldFromResult(db_result, "VehicleModel"));
	totalRaceCheckpoints = RACE_loadRaceFromFile(filename);
	raceVehicleModel = vehicleModel;
	
	new Float:totalRaceDistance = 0.0;
	for(new i = 0; i< totalRaceCheckpoints - 1; i++)
	{
		totalRaceDistance += GetDistanceBetweenPoints(raceCheckpoints[i][x], raceCheckpoints[i][y], raceCheckpoints[i+1][x], raceCheckpoints[i+1][y]);
	}
	
	SendClientMessageToAllFormatted(COLOR_MG, "** Race '%s' by '%s' is about to start... type /join if you wish to join", racename, creatorName);
	SendClientMessageToAllFormatted(COLOR_MG, "* Total checkpoints: %d | Length: %.2fkm", totalRaceCheckpoints, totalRaceDistance/1000);
	if(recordTime != 0) 
	{
		new tH,tM,tS;
		ConvertTime(var, recordTime, tH, tM, tS);
		SendClientMessageToAllFormatted(COLOR_MG, "* Current race record holder is '%s', time is %02d Min %02d Sec", recordHolder, tH, tM, tS);
	}
	else 
	{
		SendClientMessageToAll(COLOR_MG, "* There is no current record holder for this race.");
	}
	new vehicleModelName[64];
	if(vehicleModel == 0)
	{
		SendClientMessageToAll(COLOR_MG, "* Vehicle: ANY");
	}
	else
	{
		VEH_getVehicleNameByModelId(vehicleModel, vehicleModelName);
		SendClientMessageToAllFormatted(COLOR_MG, "* Vehicle: %s (ID=%d)", vehicleModelName, vehicleModel);
	}
	
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
		new playerTdMsg[128];
		format(playerTdMsg, sizeof playerTdMsg, "~r~Progress_~w~-/%d~n~~r~Position_~w~-/%d~n~~r~Time_~w~---", totalRaceCheckpoints, playerInRaceRemaining + playersInRaceFinished); 
		raceStatus = RACE_STATUS_ACTIVE;
		raceCountdownTimerCount = 5;
		raceCountdownTimer = SetTimer("RACE_CountDown", 1000, 1);
		for(new i = 0; i < MAX_PLAYERS; i++)
		{
			if(playersInRace[i] != -1)
			{
				SendClientMessageFormatted(i, COLOR_MG_PROGRESS, "* Race is about to start [%d racers, including you]", playerInRaceRemaining);
				GameTextForPlayer(i, "~r~RACE STARTING NOW", 1000, 3);
				//create player textdraw
				playersInRaceTextdraw[i] = CreatePlayerTextDraw(i, 485.554931, 405.037750, playerTdMsg);
				PlayerTextDrawLetterSize(i, playersInRaceTextdraw[i], 0.400000, 1.600000);
				PlayerTextDrawTextSize(i, playersInRaceTextdraw[i], 640.000000, 0.000000);
				PlayerTextDrawAlignment(i, playersInRaceTextdraw[i], 1);
				PlayerTextDrawColor(i, playersInRaceTextdraw[i], -1);
				PlayerTextDrawUseBox(i, playersInRaceTextdraw[i], 1);
				PlayerTextDrawBoxColor(i, playersInRaceTextdraw[i], 13141);
				PlayerTextDrawSetShadow(i, playersInRaceTextdraw[i], 1);
				PlayerTextDrawSetOutline(i, playersInRaceTextdraw[i], 0);
				PlayerTextDrawBackgroundColor(i, playersInRaceTextdraw[i], 1499227944);
				PlayerTextDrawFont(i, playersInRaceTextdraw[i], 2);
				PlayerTextDrawSetProportional(i, playersInRaceTextdraw[i], 1);
				PlayerTextDrawSetShadow(i, playersInRaceTextdraw[i], 0);
				PlayerTextDrawSetSelectable(i, playersInRaceTextdraw[i], true);
				
				PlayerTextDrawShow(i, playersInRaceTextdraw[i]);
			}
		}
	}
	else 
	{
		SendClientMessageToAll(COLOR_MG_RES, "* Race not started beacause nobody comes");
		raceStatus = RACE_STATUS_INACTIVE;
		SetTimer("RACE_initNewRace", RACES_INTERVAL, 0);
	}
}

public RACE_finishRace()
{
	raceStatus = RACE_STATUS_INACTIVE;
	currentRaceIdentifier = -1;
	KillTimer(playersInRaceTextdrawTimer);
	playersInRaceTextdrawTimer = -1;
	SetTimer("RACE_initNewRace", RACES_INTERVAL, 0);
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if(playersInRace[i] != -1)
		{
			DisablePlayerRaceCheckpoint(i);
		}
	}
}

stock RACE_playerFinishRace(playerid, status)
{
	if(status == 0) //player finished
	{
		new player_name[MAX_PLAYER_NAME];
		GetPlayerName(playerid, player_name, sizeof(player_name));
		playersInRaceFinished++;
		new tH,tM,tS;
		new timeStamp = GetTickCount();
		new totalRaceTime = timeStamp - currentRaceStartTime;
		ConvertTime(var, totalRaceTime, tH, tM, tS);
		SendClientMessageFormatted(playerid, COLOR_MG_RES, "* You finished the race in %d position. Time: %02d Min %02d Sec", playersInRaceFinished, tH, tM, tS);
		if(playersInRaceFinished == 1) 
		{
			SendClientMessageToAllFormatted(COLOR_MG_RES, "* %s finished the race in %d position. Time: %02d Min %02d Sec", GetPlayerFormattedName(playerid), playersInRaceFinished, tH, tM, tS);
		}
		playersInRace[playerid] = -1;
		DisablePlayerRaceCheckpoint(playerid);
		PlayerTextDrawHide(playerid, playersInRaceTextdraw[playerid]);
		PlayerTextDrawDestroy(playerid, playersInRaceTextdraw[playerid]);
		playersInRaceTextdraw[playerid] = PlayerText: -1;
		
		//check is new time record occured
		static query[256], DBResult: db_result;
		new recordHolder[128], racename[128];
		new recordTime;
		format(query, sizeof query, "SELECT * FROM `race` WHERE `ID`='%d';", currentRaceIdentifier);
		db_result = DBUTILS_runSelectQuery(query);
		racename = DBUTILS_getFieldFromResult(db_result, "RaceName");
		recordHolder = DBUTILS_getFieldFromResult(db_result, "RecordHolder");
		recordTime = strToInt(DBUTILS_getFieldFromResult(db_result, "RecordTime"));
		if(recordTime > 0)
		{
			if(totalRaceTime < recordTime)
			{
				SendClientMessageFormatted(playerid, COLOR_MG_RES, "* You set up new race record, time: %02d Min %02d Sec", tH, tM, tS);
				SendClientMessageToAllFormatted(COLOR_MG_RES, "* %s set up new record of '%s' race, time: %02d Min %02d Sec", GetPlayerFormattedName(playerid), racename, tH, tM, tS);
				static queryUpdate[128];
				format(queryUpdate, sizeof queryUpdate, "UPDATE `race` SET `RecordHolder`='%s' WHERE `ID`=%d", player_name, currentRaceIdentifier);
				DBUTILS_runUpdateQuery(queryUpdate);
				format(queryUpdate, sizeof queryUpdate, "UPDATE `race` SET `RecordTime`=%d WHERE `ID`=%d", totalRaceTime, currentRaceIdentifier);
				DBUTILS_runUpdateQuery(queryUpdate);
				//gave player something cool
			}
		}
		else //no current record
		{
			SendClientMessageFormatted(playerid, COLOR_MG_RES, "* You set up new race record, time: %02d Min %02d Sec", tH, tM, tS);
			SendClientMessageToAllFormatted(COLOR_MG_RES, "* '%s' set up new record of '%s' race, time: %02d Min %02d Sec", player_name, racename, tH, tM, tS);
			static queryUpdate[128];
			format(queryUpdate, sizeof queryUpdate, "UPDATE `race` SET `RecordHolder`='%s' WHERE `ID`=%d", player_name, currentRaceIdentifier);
			DBUTILS_runUpdateQuery(queryUpdate);
			format(queryUpdate, sizeof queryUpdate, "UPDATE `race` SET `RecordTime`=%d WHERE `ID`=%d", totalRaceTime, currentRaceIdentifier);
			DBUTILS_runUpdateQuery(queryUpdate);
			//gave player something cool
		}
	}
	else//player exited 
	{
		if(playersInRace[playerid] != -1)
		{
			if(IsPlayerConnected(playerid))
			{
				DisablePlayerRaceCheckpoint(playerid);
				PlayerTextDrawHide(playerid, playersInRaceTextdraw[playerid]);
				PlayerTextDrawDestroy(playerid, playersInRaceTextdraw[playerid]);
			}
			playersInRace[playerid] = -1;
			playersInRaceTextdraw[playerid] = PlayerText: -1;
		}
	}
	
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
				PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
				TogglePlayerControllable(i,1);
				DisablePlayerRaceCheckpoint(i);
				playerInRaceProgress[i] = 0;
				SetPlayerRaceCheckpoint(i, 0, raceCheckpoints[0][x], raceCheckpoints[0][y], raceCheckpoints[0][z],
                         raceCheckpoints[1][x], raceCheckpoints[1][y], raceCheckpoints[1][z], 10);
			}
		}
		playersInRaceTextdrawTimer = SetTimer("RACE_playerTdTimer", 1000, 1);
		currentRaceStartTime = GetTickCount();
		return 1;
	}

	new string[128];
	format(string, sizeof(string), "%d", raceCountdownTimerCount);
	for (new i=0; i<MAX_PLAYERS; i++)
	{
	    if(playersInRace[i] != -1 && IsPlayerConnected(i))
    	{
			GameTextForPlayer(i, string, 1000, 3);
			PlayerPlaySound(i, 1056, 0.0, 0.0, 0.0);
			TogglePlayerControllable(i,0);
		}
	}

	raceCountdownTimerCount--;
	return 1;
}

public RACE_playerTdTimer()
{
	new playerInRaceRemaining = 0;
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if(playersInRace[i] != -1)
		{
			playerInRaceRemaining ++;
		}
	}
	for (new i=0; i<MAX_PLAYERS; i++)
	{
		if(playersInRace[i] != -1 && IsPlayerConnected(i))
		{
			new tH,tM,tS;
			new timeStamp = GetTickCount();
			new totalRaceTime = timeStamp - currentRaceStartTime;
			ConvertTime(var, totalRaceTime, tH, tM, tS);
			new playerTdMsg[128];
			new playerProgress = playerInRaceProgress[i];
			if(playerProgress < 0) playerProgress = 0;
			format(playerTdMsg, sizeof playerTdMsg, "~r~Progress_~w~%d/%d~n~~r~Position_~w~%d/%d~n~~r~Time_~w~%02d:%02d", 
				playerInRaceProgress[i], 
				totalRaceCheckpoints, 
				RACE_getPlayerPosition(i),//player position
				playerInRaceRemaining + playersInRaceFinished,
				tH,
				tM,
				tS);
			PlayerTextDrawSetString(i, playersInRaceTextdraw[i], playerTdMsg);
		}
	}
}

stock RACE_getPlayerPosition(playerid)
{
	new playerInRaceRemaining = 0;
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if(playersInRace[i] != -1)
		{
			playerInRaceRemaining ++;
		}
	}
	new position = playerInRaceRemaining + playersInRaceFinished; // last
	if(playersInRace[playerid] == -1) return -69;
	for (new i=0; i<MAX_PLAYERS; i++)
	{
		if(playersInRace[i] != -1 && IsPlayerConnected(i))
		{
			if(playerInRaceProgress[i] < playerInRaceProgress[playerid]) position--;
			if(playerInRaceProgress[i] == playerInRaceProgress[playerid])
			{
				if(GetPlayerDistanceFromPoint(i, raceCheckpoints[playerInRaceProgress[i]][x], raceCheckpoints[playerInRaceProgress[i]][y], raceCheckpoints[playerInRaceProgress[i]][z]) 
					> GetPlayerDistanceFromPoint(playerid, raceCheckpoints[playerInRaceProgress[i]][x], raceCheckpoints[playerInRaceProgress[i]][y], raceCheckpoints[playerInRaceProgress[i]][z])) position--;
			}
		}
	}
	return position;
}

stock RACE_join(playerid)
{
	if(raceStatus == RACE_STATUS_WAITING_PLAYERS)
	{
		if(playersInRace[playerid] == -1)
		{
			SendClientMessage(playerid, COLOR_MG, "* You joined race, it is starting soon.");
			playersInRace[playerid] = 1;
			SetPlayerPos(playerid, raceEntryPoint[0][x], raceEntryPoint[0][y], raceEntryPoint[0][z]);
			//additionaly set player angle to first checkpoint
			new Float: angle, Float: facingAngle;
			angle = atan2(raceCheckpoints[0][y] - raceEntryPoint[0][y], raceCheckpoints[0][x] - raceEntryPoint[0][x]);
			facingAngle = 90 - angle;
			SetPlayerFacingAngle(playerid, facingAngle);
			GameTextForPlayer(playerid, "Proceed to starting point", 1000, 3);
			
			SetPlayerRaceCheckpoint(playerid,0,raceCheckpoints[0][x], raceCheckpoints[0][y], raceCheckpoints[0][z],
                         raceCheckpoints[1][x], raceCheckpoints[1][y], raceCheckpoints[1][z], 10);
            if(raceVehicleModel != 0)
            {
				VEH_SpawnPlayerVehicle(playerid, raceVehicleModel);
			}
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

stock RACE_movePlayerToLcp(playerid)
{
	if(playersInRace[playerid] != -1)
	{
		//move player to lcp (including player vehicle)
		if(IsPlayerInAnyVehicle(playerid))
		{
			if(playerInRaceProgress[playerid] > 0)
			{
				new vehicleid = GetPlayerVehicleID(playerid);
				SetVehiclePos(vehicleid, 
					raceCheckpoints[playerInRaceProgress[playerid]-1][x], 
					raceCheckpoints[playerInRaceProgress[playerid]-1][y], 
					raceCheckpoints[playerInRaceProgress[playerid]-1][z]);
				SendClientMessage(playerid, COLOR_MG, "* Teleported to last race checkpoint.");
			}
		}
	}
}

stock RACE_checkCommandAccess(playerid, cmd[])
{
	if(IsPlayerAdmin(playerid)) return TRUE;
	if(playersInRace[playerid] != -1)
	{
		if(strcmp(cmd, "kill") != 0 && strcmp(cmd, "lcp") != 0) 
		{
			SendClientMessage(playerid, COLOR_ERROR, "* This command is unavailable while you are in the race.");
			return FALSE;
		}
	}
	else
	{
		if(strcmp(cmd, "lcp") == 0) 
		{
			SendClientMessageFormatted(playerid, COLOR_ERROR, "* /%s command available only if you are in the race.", cmd);
			return FALSE;
		}
	}
	return TRUE;
}

stock RACE_OnEnterCheckpoint(playerid)
{
	if(playersInRace[playerid] != -1 && raceStatus == RACE_STATUS_ACTIVE)
	{
		DisablePlayerRaceCheckpoint(playerid);
		if(playerInRaceProgress[playerid] == totalRaceCheckpoints - 1)
		{
			RACE_playerFinishRace(playerid, 0);
		}
		else 
		{
			playerInRaceProgress[playerid] ++;
			if(totalRaceCheckpoints - 1 == playerInRaceProgress[playerid])
			{
				GameTextForPlayer(playerid, "~r~LAST CHECKPOINT...", 1000, 3);
				SetPlayerRaceCheckpoint(playerid,1,raceCheckpoints[playerInRaceProgress[playerid]][x], raceCheckpoints[playerInRaceProgress[playerid]][y], raceCheckpoints[playerInRaceProgress[playerid]][z],
					raceCheckpoints[playerInRaceProgress[playerid]+1][x], raceCheckpoints[playerInRaceProgress[playerid]+1][y], raceCheckpoints[playerInRaceProgress[playerid]+1][z], 10);
				for(new i = 0; i < MAX_PLAYERS; i++)
				{
					if(playersInRace[i] != -1)
					{
						new msgFinish[128];
						format(msgFinish, sizeof msgFinish, "~r~%s ~n~~y~is about to cross ~n~the finish line", GetPlayerFormattedName(i));
						GameTextForPlayer(i, msgFinish, 1000, 3); 
					}
				}
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
	
	//read race entry point
	fread(file_ptr,line,256);
	index = 0;
  	index = token_by_delim(line,var_from_line,',',index);
  	X = floatstr(var_from_line);
	index = token_by_delim(line,var_from_line,',',index+1);
  	Y = floatstr(var_from_line);
  	index = token_by_delim(line,var_from_line,',',index+1);
  	Z = floatstr(var_from_line);
	raceEntryPoint[0][num] = 0;
	raceEntryPoint[0][x] = X;
	raceEntryPoint[0][y] = Y;
	raceEntryPoint[0][z] = Z;

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

MODULE_FUNCTION(OnGameModeExit()) {}
MODULE_FUNCTION(OnPlayerConnect(playerid)) {}
MODULE_FUNCTION(OnPlayerRequestClass(playerid, classid)) {}
MODULE_FUNCTION(OnPlayerSpawn(playerid)) {}
MODULE_FUNCTION(OnPlayerCommandText(playerid, cmdtext[])) {}
MODULE_FUNCTION(OnVehicleSpawn(vehicleid)) {}
MODULE_FUNCTION(OnVehicleDeath(vehicleid, killerid)) {}
MODULE_FUNCTION(OnPlayerText(playerid, text[])) {}
MODULE_FUNCTION(OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)) {}
MODULE_FUNCTION(OnPlayerExitVehicle(playerid, vehicleid)) {}
MODULE_FUNCTION(OnPlayerStateChange(playerid, newstate, oldstate)) {}
MODULE_FUNCTION(OnPlayerEnterCheckpoint(playerid)) {}
MODULE_FUNCTION(OnPlayerLeaveCheckpoint(playerid)) {}
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