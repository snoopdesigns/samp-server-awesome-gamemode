#if defined _race_included
	#endinput
#endif
#define _race_included

#include "include/awesome/a_utils.inc"
#include "include/awesome/colors.inc"

#define MODULE_FUNCTION(%1) forward RACE_%1; public RACE_%1

#define RACES_INTERVAL 1 * 10 * 1000 // 4 minutes
#define RACES_START_DELAY 20000 // 20 secs

#define RACE_STATUS_INACTIVE 0 
#define RACE_STATUS_WAITING_PLAYERS 1 
#define RACE_STATUS_ACTIVE 2 

/* DB fields constants */
#define RACE_ID "ID"
#define RACE_NAME "RaceName"
#define RACE_CREATOR_NAME "CreatorName"
#define RACE_RECORD_TIME "RecordTime"
#define RACE_RECORD_HOLDER "RecordHolder"
#define RACE_VEHICLE_MODEL "VehicleModel"
#define RACE_NUM_CHECK "NumCheck"
#define RACE_DISTANCE "Distance"
#define RACE_ACTIVE "Active"

#define CP_X "X"
#define CP_Y "Y"
#define CP_Z "Z"
#define CP_NUM "Num"

enum RACE_CREATION {
    isActive,
	race_id,
	creator_id,
	creator_name[MAX_PLAYER_NAME],
	num_cp,
	Float: distance,
	Float: last_x,
	Float: last_y
};

enum RACE_CHECKPOINT {
	num,
	Float: x,
	Float: y,
	Float: z
}

enum RACE_INFO {
	id,
	name[MAX_PLAYER_NAME],
	creator_name[MAX_PLAYER_NAME],
	record_time,
	record_holder[MAX_PLAYER_NAME],
	vehicle_id,
	num_check,
	Float: distance,
	status,
	start_time,
	total_players,
	finished_players
}

enum PLAYER_RACE_STATUS {
	participate,
	progress,
	PlayerText: td
}

new raceCreation[RACE_CREATION];
new race[RACE_INFO];
new race_checkpoints[100][RACE_CHECKPOINT];
new player_race[MAX_PLAYERS][PLAYER_RACE_STATUS];

new playersInRaceTextdrawTimer = -1;

new raceCountdownTimer = -1;
new raceCountdownTimerCount;

//TODO create player leave race method
//TODO countdown to separate file
//TODO what if race started and player disconnect, who wins?
// method to send gametext for all players that are within race
//TODO common method for countdowns

/* public functions declaration */
forward RACE_createRace(const playerid, const racename[]);
forward RACE_createRaceCheck(const playerid);
forward RACE_createRaceEnd(const playerid);
forward RACE_join(const playerid);
forward RACE_leave(const playerid);

/* public timer callbacks */
forward RACE_initNewRace();
forward RACE_startRace();
forward RACE_finishRace();
forward RACE_CountDown();
forward RACE_playerTdTimer();

MODULE_FUNCTION(OnGameModeInit()) {
	LOG_write("Race Subsystem initializing");
	DBUTILS_clearResults(DBUTILS_runNamedQuery("createRaceTable","NA"));
	DBUTILS_clearResults(DBUTILS_runNamedQuery("createRaceCheckpointsTable","NA"));
	
	SetTimer("RACE_initNewRace", RACES_INTERVAL, 0);
	RACE_join(0);
	RACE_movePlayerToLcp(0);
	RACE_checkCommandAccess(0,"111");
}

MODULE_FUNCTION(OnPlayerEnterRaceCheckpoint(playerid)) {
	if(player_race[playerid][participate] == TRUE && race[status] == RACE_STATUS_ACTIVE) {
		DisablePlayerRaceCheckpoint(playerid);
		if(player_race[playerid][progress] == race[num_check] - 1) {
			RACE_playerFinishRace(playerid, 0);
		} else {
			player_race[playerid][progress] ++;
			if(race[num_check] - 1 == player_race[playerid][progress]) {
				GameTextForPlayer(playerid, "~r~LAST CHECKPOINT...", 1000, 3);
				for(new i = 0; i < MAX_PLAYERS; i++) {
					if(player_race[playerid][participate] == TRUE) {
						new msgFinish[128];
						format(msgFinish, sizeof msgFinish, "~r~%s ~n~~y~is about to cross ~n~the finish line", GetPlayerFormattedName(i));
						GameTextForPlayer(i, msgFinish, 1000, 3); 
					}
				}
			}
			SetPlayerRaceCheckpoint(playerid,1,
				race_checkpoints[player_race[playerid][progress]][x], 
				race_checkpoints[player_race[playerid][progress]][y], 
				race_checkpoints[player_race[playerid][progress]][z],
				race_checkpoints[player_race[playerid][progress]+1][x], 
				race_checkpoints[player_race[playerid][progress]+1][y], 
				race_checkpoints[player_race[playerid][progress]+1][z], 
				10);
		}
	}
}

MODULE_FUNCTION(OnPlayerDisconnect(playerid, reason)) {
	RACE_playerFinishRace(playerid, 1);
}

MODULE_FUNCTION(OnPlayerDeath(playerid, killerid, reason)) {
	SendClientMessage(playerid, COLOR_MG, "* You died and cannot finish the race.");
	RACE_playerFinishRace(playerid, 1);
}

public RACE_join(const playerid) {
	if(race[status] == RACE_STATUS_WAITING_PLAYERS) {
		if(player_race[playerid][participate] == FALSE) {
			SendClientMessage(playerid, COLOR_MG, "* You joined race, it is starting soon.");
			player_race[playerid][participate] = TRUE;
			SetPlayerPos(playerid, race_checkpoints[0][x], race_checkpoints[0][y], race_checkpoints[0][z]);
			//additionaly set player angle to first checkpoint
			/*new Float: angle, Float: facingAngle;
			angle = atan2(raceCheckpoints[0][y] - raceEntryPoint[0][y], raceCheckpoints[0][x] - raceEntryPoint[0][x]);
			facingAngle = 90 - angle;
			SetPlayerFacingAngle(playerid, facingAngle);*/
			GameTextForPlayer(playerid, "Proceed to starting point", 1000, 3);
			
			SetPlayerRaceCheckpoint(playerid,0,
				race_checkpoints[0][x], 
				race_checkpoints[0][y], 
				race_checkpoints[0][z],
				race_checkpoints[1][x], 
				race_checkpoints[1][y], 
				race_checkpoints[1][z], 10);
            if(race[vehicle_id] != 0) {
				VEH_SpawnPlayerVehicle(playerid, race[vehicle_id]);
			}
		} else {
			SendClientMessage(playerid, COLOR_ERROR, "* You are already in race.");
		}
	} else {
		if(race[status] == RACE_STATUS_ACTIVE) {
			SendClientMessage(playerid, COLOR_ERROR, "* Race already started!");
		} else if(race[status] == RACE_STATUS_INACTIVE) {
			SendClientMessage(playerid, COLOR_ERROR, "* There is no active race for now");
		}
	}
}

public RACE_leave(const playerid) {
	if(race[status] == RACE_STATUS_WAITING_PLAYERS || race[status] == RACE_STATUS_ACTIVE) {
		if(player_race[playerid][participate] == TRUE) {
			SendClientMessage(playerid, COLOR_MG, "* You joined race, it is starting soon.");
			player_race[playerid][participate] = FALSE;
			RACE_playerFinishRace(playerid, 1);
			SpawnPlayer(playerid);
		} else {
			SendClientMessage(playerid, COLOR_ERROR, "* You are not in the race.");
		}
	} else {
		SendClientMessage(playerid, COLOR_ERROR, "* There is no active race for now");
	}
}

stock RACE_movePlayerToLcp(const playerid) {
	if(player_race[playerid][participate] == TRUE) {
		if(IsPlayerInAnyVehicle(playerid)) {
			if(player_race[playerid][progress] > 0) {
				new vehicleid = GetPlayerVehicleID(playerid);
				SetVehiclePos(vehicleid, 
					race_checkpoints[player_race[playerid][progress]-1][x], 
					race_checkpoints[player_race[playerid][progress]-1][y], 
					race_checkpoints[player_race[playerid][progress]-1][z]);
				SendClientMessage(playerid, COLOR_MG, "* Teleported to last race checkpoint.");
			}
		}
	}
}

public RACE_createRace(const playerid, const racename[]) {
	if(raceCreation[isActive] == FALSE) {
		new player_name[MAX_PLAYER_NAME];
		GetPlayerName(playerid, player_name, sizeof(player_name));
		SendClientMessageFormatted(playerid, COLOR_IMPROVEMENT, "* Start creating new race: %s", player_name);
		DBUTILS_clearResults(DBUTILS_runNamedQuery("createNewRace","ss",racename,player_name));
		new DBResult:db_result;
		db_result = DBUTILS_runNamedQuery("findRaceByName","s",racename);
		raceCreation[isActive] = TRUE;
		raceCreation[creator_id] = playerid;
		raceCreation[race_id] = DBUTILS_getInt(db_result, RACE_ID);
		raceCreation[creator_name] = player_name;
		raceCreation[num_cp] = 0;
		raceCreation[distance] = 0.0;
		DBUTILS_clearResults(db_result);
	} else {
		SendClientMessageFormatted(playerid, COLOR_ERROR, "Race is already under creation by player: %s", raceCreation[creator_name]);
	}
}

public RACE_createRaceCheck(const playerid) {
	if(raceCreation[isActive] == TRUE && playerid == raceCreation[creator_id]) {
		SendClientMessage(playerid, COLOR_IMPROVEMENT, "* Race checkpoint added");
		new Float:xpos, Float:ypos, Float:zpos;
		GetPlayerPos(playerid, xpos, ypos, zpos);
		SendClientMessageFormatted(playerid, COLOR_IMPROVEMENT, "%f, %f, %f", xpos, ypos, zpos);
		DBUTILS_clearResults(DBUTILS_runNamedQuery("createRaceCheckpoint","dfffd",raceCreation[race_id],xpos,ypos,zpos,raceCreation[num_cp]));
		raceCreation[num_cp]++;
		if(floatround(raceCreation[last_x]) != 0 && floatround(raceCreation[last_y]) != 0) {
			raceCreation[distance] += GetDistanceBetweenPoints(
				raceCreation[last_x], 
				raceCreation[last_y], 
				xpos, 
				ypos);
			raceCreation[last_x] = xpos;
			raceCreation[last_y] = ypos;
		}
	} else {
		SendClientMessage(playerid, COLOR_ERROR, "* No active race creations.. Type /cr <filename> to start race creation");
	}
}

public RACE_createRaceEnd(const playerid) {
	if(raceCreation[isActive] == TRUE && playerid == raceCreation[creator_id]) {	
		new player_name[MAX_PLAYER_NAME];
		GetPlayerName(playerid, player_name, sizeof player_name);
		
		DBUTILS_clearResults(DBUTILS_runNamedQuery("updateRaceInt","sd",RACE_ACTIVE,1,race[id]));
		DBUTILS_clearResults(DBUTILS_runNamedQuery("updateRaceInt","sd",RACE_NUM_CHECK,raceCreation[num_cp],race[id]));
		DBUTILS_clearResults(DBUTILS_runNamedQuery("updateRaceFloat","sf",RACE_DISTANCE,raceCreation[distance],race[id]));
		//TODO calculate distance
		
		SendClientMessageFormatted(playerid, COLOR_IMPROVEMENT, "* New race successfully created, ID = %d", raceCreation[race_id]);
		
		raceCreation[isActive] = FALSE;
		raceCreation[race_id] = -1;
		raceCreation[creator_id] = -1;
		new empty[MAX_PLAYER_NAME];
		raceCreation[creator_name] = empty;
		raceCreation[num_cp] = 0;
	}
	else 
	{
		SendClientMessage(playerid, COLOR_ERROR, "* No active race creations.. Type /cr <filename> to start race creation");
	}
}

public RACE_initNewRace() {
	RACE_clearRaceInfos();
	if(RACE_loadRandomRace() == 0) {
		SetTimer("RACE_initNewRace", RACES_INTERVAL, 0);
		return;
	}
	
	SendClientMessageToAllFormatted(COLOR_MG, "** Race '%s' by '%s' is about to start... type /join if you wish to join", race[name], race[creator_name]);
	SendClientMessageToAllFormatted(COLOR_MG, "* Total checkpoints: %d | Length: %.2fkm", race[num_check], race[distance]/1000);
	if(race[record_time] != 0) {
		new tH,tM,tS;
		ConvertTime(var, race[record_time], tH, tM, tS);
		SendClientMessageToAllFormatted(COLOR_MG, "* Current race record holder is '%s', time is %02d Min %02d Sec", race[record_holder], tH, tM, tS);
	} else {
		SendClientMessageToAll(COLOR_MG, "* There is no current record holder for this race.");
	}
	
	if(race[vehicle_id] == 0)
	{
		SendClientMessageToAll(COLOR_MG, "* Vehicle: ANY");
	} else {
		new vehicleModelName[64];
		VEH_getVehicleNameByModelId(race[vehicle_id], vehicleModelName);
		SendClientMessageToAllFormatted(COLOR_MG, "* Vehicle: %s (ID=%d)", vehicleModelName, race[vehicle_id]);
	}
	
	race[status] = RACE_STATUS_WAITING_PLAYERS;
	SetTimer("RACE_startRace", RACES_START_DELAY, 0);
}

public RACE_startRace() {

	if(race[total_players]) {
		new playerTdMsg[128];
		format(playerTdMsg, sizeof playerTdMsg, "~r~Progress_~w~-/%d~n~~r~Position_~w~-/%d~n~~r~Time_~w~---", race[num_check] , race[total_players]); 
		race[status] = RACE_STATUS_ACTIVE;
		raceCountdownTimerCount = 5;
		raceCountdownTimer = SetTimer("RACE_CountDown", 1000, 1);
		for(FOR_PLAYERS) {
			if(player_race[i][participate] == TRUE) {
				SendClientMessageFormatted(i, COLOR_MG_PROGRESS, "* Race is about to start [%d racers, including you]", race[total_players]);
				GameTextForPlayer(i, "~r~RACE STARTING NOW", 1000, 3);
				//create player textdraw
				player_race[i][td] = CreatePlayerTextDraw(i, 485.554931, 405.037750, playerTdMsg);
				PlayerTextDrawLetterSize(i, player_race[i][td], 0.400000, 1.600000);
				PlayerTextDrawTextSize(i, player_race[i][td], 640.000000, 0.000000);
				PlayerTextDrawAlignment(i, player_race[i][td], 1);
				PlayerTextDrawColor(i, player_race[i][td], -1);
				PlayerTextDrawUseBox(i, player_race[i][td], 1);
				PlayerTextDrawBoxColor(i, player_race[i][td], 13141);
				PlayerTextDrawSetShadow(i, player_race[i][td], 1);
				PlayerTextDrawSetOutline(i, player_race[i][td], 0);
				PlayerTextDrawBackgroundColor(i, player_race[i][td], 1499227944);
				PlayerTextDrawFont(i, player_race[i][td], 2);
				PlayerTextDrawSetProportional(i, player_race[i][td], 1);
				PlayerTextDrawSetShadow(i, player_race[i][td], 0);
				PlayerTextDrawSetSelectable(i, player_race[i][td], true);
				
				PlayerTextDrawShow(i, player_race[i][td]);
			}
		}
	} else {
		SendClientMessageToAll(COLOR_MG_RES, "* Race not started beacause nobody comes");
		race[status] = RACE_STATUS_INACTIVE;
		SetTimer("RACE_initNewRace", RACES_INTERVAL, 0);
	}
}

public RACE_finishRace() {
	for(FOR_PLAYERS) {
		if(player_race[i][participate] == TRUE) {
			DisablePlayerRaceCheckpoint(i);
		}
	}
	RACE_clearRaceInfos();
	KillTimer(playersInRaceTextdrawTimer);
	playersInRaceTextdrawTimer = -1;
	SetTimer("RACE_initNewRace", RACES_INTERVAL, 0);
}

public RACE_CountDown() {
	if(raceCountdownTimerCount == 0) {
		KillTimer(raceCountdownTimer);
		for (FOR_PLAYERS) {
			if(player_race[i][participate] == TRUE && IsPlayerConnected(i)) {
				GameTextForPlayer(i, "GO", 1000, 3);
				PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
				TogglePlayerControllable(i,1);
				DisablePlayerRaceCheckpoint(i);
				SetPlayerRaceCheckpoint(i,1,
					race_checkpoints[player_race[i][progress]][x], 
					race_checkpoints[player_race[i][progress]][y], 
					race_checkpoints[player_race[i][progress]][z],
					race_checkpoints[player_race[i][progress]+1][x], 
					race_checkpoints[player_race[i][progress]+1][y], 
					race_checkpoints[player_race[i][progress]+1][z], 
					10);
			}
		}
		playersInRaceTextdrawTimer = SetTimer("RACE_playerTdTimer", 1000, 1);
		race[start_time] = GetTickCount();
		return 1;
	}

	new string[128];
	format(string, sizeof(string), "%d", raceCountdownTimerCount);
	for (FOR_PLAYERS) {
	    if(player_race[i][participate] == TRUE && IsPlayerConnected(i)) {
			GameTextForPlayer(i, string, 1000, 3);
			PlayerPlaySound(i, 1056, 0.0, 0.0, 0.0);
			TogglePlayerControllable(i,0);
		}
	}

	raceCountdownTimerCount--;
	return 1;
}

public RACE_playerTdTimer() {
	for (FOR_PLAYERS) {
		if(player_race[i][participate] == TRUE && IsPlayerConnected(i)) {
			new tH,tM,tS;
			new timeStamp = GetTickCount();
			new totalRaceTime = timeStamp - race[start_time];
			ConvertTime(var, totalRaceTime, tH, tM, tS);
			new playerTdMsg[128];
			format(playerTdMsg, sizeof playerTdMsg, "~r~Progress_~w~%d/%d~n~~r~Position_~w~%d/%d~n~~r~Time_~w~%02d:%02d", 
				player_race[i][progress], 
				race[num_check], 
				RACE_getPlayerPosition(i),//player position
				race[total_players],
				tH,
				tM,
				tS);
			PlayerTextDrawSetString(i, player_race[i][td], playerTdMsg);
		}
	}
}

stock RACE_clearRaceInfos() {
	new empty_str[MAX_PLAYER_NAME];
	
	for (new i = 0; i < 100; i++) {
	    race_checkpoints[i][num] = 0;
		race_checkpoints[i][x] = 0.0;
		race_checkpoints[i][y] = 0.0;
		race_checkpoints[i][z] = 0.0;
	}
	
	for(FOR_PLAYERS) {
		player_race[i][participate] = FALSE;
		player_race[i][progress] = 0;
		player_race[i][td] = PlayerText: -1;
	}
	
	race[id] = -1;
	race[name] = empty_str;
	race[creator_name] = empty_str;
	race[record_time] = 0;
	race[record_holder] = empty_str;
	race[vehicle_id] = -1;
	race[num_check] = 0;
	race[distance] = 0;
	race[status] = RACE_STATUS_INACTIVE;
	race[start_time] = 0;
	race[total_players] = 0;
	race[finished_players] = 0;
	
	raceCreation[isActive] = FALSE;
	raceCreation[race_id] = -1;
	raceCreation[creator_id] = -1;
	new player_name[MAX_PLAYER_NAME];
	raceCreation[creator_name] = player_name;
	raceCreation[num_cp] = 0;
	raceCreation[distance] = 0.0;
	raceCreation[last_x] = 0.0;
	raceCreation[last_y] = 0.0;
}

stock RACE_loadRandomRace() {
	//TODO load checkpoints
    new DBResult: db_result;
	db_result = DBUTILS_runNamedQuery("findRandomRace","NA");
	if(DBUTILS_getResultSize(db_result) == 0) {
		return 0;
	}
	race[id] = DBUTILS_getInt(db_result, RACE_ID);
	DBUTILS_getString(db_result, RACE_NAME, race[name], sizeof(race[name]));
	DBUTILS_getString(db_result, RACE_CREATOR_NAME, race[creator_name], sizeof(race[creator_name]));
	race[record_time] = DBUTILS_getInt(db_result, RACE_RECORD_TIME);
	DBUTILS_getString(db_result, RACE_RECORD_HOLDER, race[record_holder], sizeof(race[record_holder]));
	race[vehicle_id] = DBUTILS_getInt(db_result, RACE_VEHICLE_MODEL);
	race[num_check] = DBUTILS_getInt(db_result, RACE_NUM_CHECK);
	race[distance] = DBUTILS_getInt(db_result, RACE_DISTANCE);
	race[status] = RACE_STATUS_INACTIVE;
	race[start_time] = 0;
	race[total_players] = 0;
	DBUTILS_clearResults(db_result);
	
	// Loading checkpoints
	db_result = DBUTILS_runNamedQuery("findCheckpointsByRaceID","d", race[id]);
	do {
		new cp_num,Float:cp_x,Float:cp_y,Float:cp_z;
		cp_num = DBUTILS_getInt(db_result, CP_NUM);
		cp_x = DBUTILS_getFloat(db_result, CP_X);
		cp_y = DBUTILS_getFloat(db_result, CP_Y);
		cp_z = DBUTILS_getFloat(db_result, CP_Z);
		race_checkpoints[cp_num][num] = cp_num;
		race_checkpoints[cp_num][x] = cp_x;
		race_checkpoints[cp_num][y] = cp_y;
		race_checkpoints[cp_num][z] = cp_z;
	} while(DBUTILS_nextItem(db_result));
	DBUTILS_clearResults(db_result);
	
	return 1;
}

stock RACE_playerFinishRace(const playerid, const race_finish_status) {
	if(race_finish_status == 0) { // player finished
		new player_name[MAX_PLAYER_NAME];
		GetPlayerName(playerid, player_name, sizeof(player_name));
		race[finished_players]++;
		new tH,tM,tS;
		new timeStamp = GetTickCount();
		new totalRaceTime = timeStamp - race[start_time];
		ConvertTime(var, totalRaceTime, tH, tM, tS);
		SendClientMessageFormatted(playerid, COLOR_MG_RES, "* You finished the race in %d position. Time: %02d Min %02d Sec", race[finished_players], tH, tM, tS);
		if(race[finished_players] == 1) {
			SendClientMessageToAllFormatted(COLOR_MG_RES, "* %s finished the race in %d position. Time: %02d Min %02d Sec", GetPlayerFormattedName(playerid), race[finished_players], tH, tM, tS);
		}
		player_race[playerid][participate] = FALSE;
		DisablePlayerRaceCheckpoint(playerid);
		PlayerTextDrawHide(playerid, player_race[playerid][td]);
		PlayerTextDrawDestroy(playerid, player_race[playerid][td]);
		player_race[playerid][td] = PlayerText: -1;
		
		if(totalRaceTime < race[record_time] || race[record_time] == 0) {
			SendClientMessageFormatted(playerid, COLOR_MG_RES, "* You set up new race record, time: %02d Min %02d Sec", tH, tM, tS);
			SendClientMessageToAllFormatted(COLOR_MG_RES, "* %s set up new record of '%s' race, time: %02d Min %02d Sec", GetPlayerFormattedName(playerid), race[name], tH, tM, tS);
			DBUTILS_clearResults(DBUTILS_runNamedQuery("updateRaceStr","ssd", RACE_RECORD_HOLDER, player_name, race[id]));
			DBUTILS_clearResults(DBUTILS_runNamedQuery("updateRaceInt","ssd", RACE_RECORD_TIME, totalRaceTime, race[id]));
			//gave player something cool
		}
	} else { //player exited
		if(player_race[playerid][participate] == TRUE) {
			if(IsPlayerConnected(playerid)) {
				DisablePlayerRaceCheckpoint(playerid);
				PlayerTextDrawHide(playerid, player_race[playerid][td]);
				PlayerTextDrawDestroy(playerid, player_race[playerid][td]);
			}
			player_race[playerid][participate] = FALSE;
			player_race[playerid][td] = PlayerText: -1;
			race[total_players]--;
		}
	}
	if(race[status] == RACE_STATUS_ACTIVE && race[finished_players] == race[total_players]) {
		RACE_finishRace();
	}
}

stock RACE_getPlayerPosition(const playerid) {
	new position = race[total_players]; // last
	if(player_race[playerid][participate] != TRUE) return -69;
	for (FOR_PLAYERS) {
		if(player_race[i][participate] == TRUE && IsPlayerConnected(i)) {
			if(player_race[i][progress] < player_race[playerid][progress]) position--;
			if(player_race[i][progress] == player_race[playerid][progress]){
				if(GetPlayerDistanceFromPoint(i, 
					race_checkpoints[player_race[i][progress]][x], 
					race_checkpoints[player_race[i][progress]][y], 
					race_checkpoints[player_race[i][progress]][z]) 
				> 
				GetPlayerDistanceFromPoint(playerid, 
					race_checkpoints[player_race[playerid][progress]][x], 
					race_checkpoints[player_race[playerid][progress]][y], 
					race_checkpoints[player_race[playerid][progress]][z])) 
					
					position--;
			}
		}
	}
	return position;
}

stock RACE_checkCommandAccess(const playerid, const cmd[]) {
	if(IsPlayerAdmin(playerid)) return TRUE;
	if(player_race[playerid][participate] == TRUE) {
		if(strcmp(cmd, "kill") != 0 && strcmp(cmd, "lcp") != 0) {
			SendClientMessage(playerid, COLOR_ERROR, "* This command is unavailable while you are in the race.");
			return FALSE;
		}
	} else {
		if(strcmp(cmd, "lcp") == 0) {
			SendClientMessageFormatted(playerid, COLOR_ERROR, "* /%s command available only if you are in the race.", cmd);
			return FALSE;
		}
	}
	return TRUE;
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