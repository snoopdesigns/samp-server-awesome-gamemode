#if defined _commands_included
	#endinput
#endif
#define _commands_included

#include "include/awesome/a_vehutils.inc"
//#include "include/awesome/a_menus.inc"
#include "include/awesome/a_props.inc"
#include "include/awesome/a_reg.inc"
#include "include/awesome/a_utils.inc"
#include "include/awesome/colors.inc"

#define MODULE_FUNCTION(%1) forward CMDS_%1; public CMDS_%1

#define dcmd(%1,%2,%3,%4) if(!strcmp((%3)[1], #%1, true, (%2)) && ((((%3)[(%2) + 1] == '\0') && (/*RACE_checkCommandAccess(playerid, (#%1)) && MG_checkCommandAccess(playerid, (#%1)) && */CMDS_checkCommandAccess(playerid, (%4)) && dcmd_%1(playerid, ""))) || (((%3)[(%2) + 1] == ' ') && (CMDS_checkCommandAccess(playerid, (%4)) && dcmd_%1(playerid, (%3)[(%2) + 2]))))) return 1

/* public functions declaration */
forward CMDS_checkCommandAccess(const playerid, const level);

MODULE_FUNCTION(OnPlayerCommandText(playerid, cmdtext[])) {
	LOG_writeFormatted("Called OnPlayerCommandText(), playerid = %d, command = %s", playerid, cmdtext);
	dcmd(kill, 4, cmdtext, 0);
	
	//help command
	dcmd(help, 4, cmdtext, 0);

	// register commands
	dcmd(login, 5, cmdtext, 0);
	dcmd(register, 8, cmdtext, 0);
	dcmd(stats, 5, cmdtext, 0);
	
	//vehicle commands
	dcmd(v, 1, cmdtext, 1);
	dcmd(car, 3, cmdtext, 1);
	dcmd(fix, 3, cmdtext, 1);
	
	//properties commands
	dcmd(cprop, 5, cmdtext, 2);
	dcmd(buy, 3, cmdtext, 1);
	dcmd(sell, 4, cmdtext, 1);
	
	//races
	dcmd(join, 4, cmdtext, 1);
	dcmd(leaverace, 9, cmdtext, 1);
	dcmd(races, 5, cmdtext, 1);
	dcmd(lcp, 3, cmdtext, 1);
	dcmd(cr, 2, cmdtext, 2);
	dcmd(crcheck, 7, cmdtext, 2);
	dcmd(crend, 5, cmdtext, 2);
	
	//minigames
	/*dcmd(event, 5, cmdtext, 1);
	dcmd(leave, 5, cmdtext, 1);
	
	//admin commands
	dcmd(sethealth, 9, cmdtext, 2);
	dcmd(setscore, 8, cmdtext, 2);
	dcmd(setmoney, 8, cmdtext, 2);
	dcmd(restart, 7, cmdtext, 2);
	dcmd(setlevel, 8, cmdtext, 2);*/
	SendClientMessage(playerid, COLOR_ERROR, "* Unknown command passed.");
	return 1;
}

public CMDS_checkCommandAccess(const playerid, const level) {
	if(IsPlayerAdmin(playerid)) return TRUE;
	if(REG_getPlayerLevel(playerid) >= level) {
		return TRUE;
	} else {		
		if(REG_getPlayerLevel(playerid) == 0) {
			SendClientMessage(playerid, COLOR_ERROR, "* Sorry, you need to register to access this command!");
		} else {
			SendClientMessage(playerid, COLOR_ERROR, "* Sorry, you have not access to this command!");
			SendClientMessageFormatted(playerid, COLOR_ERROR, "* Required player level: %d", level);
		}
		return FALSE;
	}
}

dcmd_kill(const playerid, const params[]) {
	strlen(params);//just to handle compile warning
	SetPlayerHealth(playerid, 0.0);
	return 1;
}

dcmd_help(const playerid, const params[]) {
	strlen(params);//just to handle compile warning
	//MENUS_showPlayerMenu(playerid, PLAYER_MENU_HELP);
	return 1;
}

//=====================================================================
//========================REGISTER COMMANDS============================
//=====================================================================

dcmd_register(const playerid, params[]) {
	new	password[64];
	if (sscanf(params, "s", password)) {
		SendClientMessage(playerid, COLOR_ERROR, "* Usage: \"/register <password>\"");
	} else {
		REG_registerNewPlayer(playerid, password);
	}
	return 1;
}

dcmd_login(const playerid, params[]) {
	new	password[64];
	if (sscanf(params, "s", password)) {
		SendClientMessage(playerid, COLOR_ERROR, "* Usage: \"/login <password>\"");
	} else {
		REG_loginPlayer(playerid, password);
	}
	return 1;
}

dcmd_stats(const playerid, params[]) {
	strlen(params);//just to handle compile warning
	REG_printPlayerStats(playerid);
	return 1;
}

//=====================================================================
//========================VEHICLE COMMANDS=============================
//=====================================================================

dcmd_car(const playerid, params[]) {
	strlen(params);//just to handle compile warning
	VEH_SpawnPlayerVehicle(playerid, 411);
	return 1;
}

dcmd_v(const playerid, params[]) {
	new	modelid;
	if (sscanf(params, "i", modelid)) {
		SendClientMessage(playerid, COLOR_ERROR, "* Usage: \"/v <modelid>\"");
	} else {
		VEH_SpawnPlayerVehicle(playerid, modelid);
	}
	return 1;
}

dcmd_fix(const playerid, params[]) {
	strlen(params);//just to handle compile warning
	VEH_RepairPlayerVehicle(playerid);
	return 1;
}

//=====================================================================
//========================PROPERTIES COMMANDS==========================
//=====================================================================

dcmd_buy(const playerid, params[]) {
	strlen(params);//just to handle compile warning
	PROPS_buyPropertyForPlayer(playerid);
	return 1;
}

dcmd_sell(const playerid, params[]) {
	strlen(params);//just to handle compile warning
	PROPS_sellProperty(playerid);
	return 1;
}

dcmd_cprop(const playerid, params[]) {
	new	propname[64];
	new cost;
	new earnings;
	if (sscanf(params, "sii", propname, cost, earnings)) {
		SendClientMessage(playerid, COLOR_ERROR, "* Usage: \"/cprop <prop_name> <cost> <earnings>\"");
	} else {
		PROPS_createProperty(playerid, propname, cost, earnings);
	}
	return 1;
}

//=====================================================================
//========================RACES COMMANDS===============================
//=====================================================================

dcmd_join(playerid, params[]) {
	strlen(params);//just to handle compile warning
	RACE_join(playerid);
	return 1;
}

dcmd_leaverace(playerid, params[]) {
	strlen(params);//just to handle compile warning
	RACE_leave(playerid);
	return 1;
}

dcmd_races(playerid, params[]) {
	strlen(params);//just to handle compile warning
	RACE_printRacesInfo(playerid);
	return 1;
}

dcmd_lcp(playerid, params[]) {
	strlen(params);//just to handle compile warning
	RACE_movePlayerToLcp(playerid);
	return 1;
}

dcmd_cr(playerid, params[]) {
	new	racename[64];
	if (sscanf(params, "s", racename)) {
		SendClientMessage(playerid, COLOR_ERROR, "* Usage: \"/cr <racename>\"");
	} else {
		RACE_createRace(playerid, racename);
	}
	return 1;
}

dcmd_crcheck(playerid, params[]) {
	strlen(params);//just to handle compile warning
	RACE_createRaceCheck(playerid);
	return 1;
}

dcmd_crend(playerid, params[]) {
	strlen(params);//just to handle compile warning
	RACE_createRaceEnd(playerid);
	return 1;
}

//=====================================================================
//========================MINIGAMES COMMANDS===========================
//=====================================================================

/*dcmd_event(playerid, params[])
{
	strlen(params);//just to handle compile warning
	MG_registerPlayerForMinigame(playerid);
	return 1;
}

dcmd_leave(playerid, params[])
{
	strlen(params);//just to handle compile warning
	MG_PlayerLeftMinigame(playerid);
	return 1;
}

//=====================================================================
//========================ADMIN COMMANDS===============================
//=====================================================================

dcmd_restart(playerid, params[])
{
	intToStr(playerid);
	strlen(params);//just to handle compile warning
	SendRconCommand("changemode awesome-freeroam");
	return 1;
}

dcmd_sethealth(playerid, params[])
{
	new	health;
	if (sscanf(params, "i", health)) 
	{
		SendClientMessage(playerid, COLOR_ERROR, "* Usage: \"/sethealth <health>\"");
	}
	else
	{
		new vehicle;
		vehicle = GetPlayerVehicleID(playerid);
		if(vehicle > 0)
		{
			SetVehicleHealth(vehicle, health);
		}
		SendClientMessageFormatted(playerid, COLOR_IMPROVEMENT, "* Your vehicle health has been set to %d", health);
	}
	return 1;
}

dcmd_setscore(playerid, params[])
{
	new	id, score;
	if (sscanf(params, "ii", id, score)) 
	{
		SendClientMessage(playerid, COLOR_ERROR, "* Usage: \"/setscore <id> <score>\"");
	}
	else
	{
		if(IsPlayerConnected(id))
		{
			SetPlayerScore(id, score);
			SendClientMessageFormatted(playerid, COLOR_IMPROVEMENT, "* Player %s score has been set to %d", GetPlayerFormattedName(id), score);
			SendClientMessageFormatted(id, COLOR_IMPROVEMENT, "* Admin %s set your score to %d", GetPlayerFormattedName(playerid), score);
		}
		else 
		{
			SendClientMessageFormatted(playerid, COLOR_ERROR, "* Player with ID = %d does not exists!", id);
		}
	}
	return 1;
}

dcmd_setmoney(playerid, params[])
{
	new	id, money;
	if (sscanf(params, "ii", id, money)) 
	{
		SendClientMessage(playerid, COLOR_ERROR, "* Usage: \"/setmoney <id> <money>\"");
	}
	else
	{
		if(IsPlayerConnected(id))
		{
			GivePlayerMoney(id, money);
			SendClientMessageFormatted(playerid, COLOR_IMPROVEMENT, "* Player %s moneys has been increased to %d", GetPlayerFormattedName(id), money);
			SendClientMessageFormatted(id, COLOR_IMPROVEMENT, "* Admin %s gave you money %d", GetPlayerFormattedName(playerid), money);
		}
		else 
		{
			SendClientMessageFormatted(playerid, COLOR_ERROR, "* Player with ID = %d does not exists!", id);
		}
	}
	return 1;
}

dcmd_setlevel(playerid, params[])
{
	new	id, newlevel;
	if (sscanf(params, "ii", id, newlevel)) 
	{
		SendClientMessage(playerid, COLOR_ERROR, "* Usage: \"/setlevel <id> <level>\"");
	}
	else
	{
		if(IsPlayerConnected(id))
		{
			REG_updatePropertyOfPlayer(id, "PlayerLevel", intToStr(newlevel));
			new targetname[MAX_PLAYER_NAME], playername[MAX_PLAYER_NAME];
			GetPlayerName(playerid, playername, sizeof playername);
			GetPlayerName(id, targetname, sizeof targetname);
			SendClientMessageFormatted(playerid, COLOR_IMPROVEMENT, "* Player %s now has level %d", GetPlayerFormattedName(id), newlevel);
			SendClientMessageFormatted(id, COLOR_IMPROVEMENT, "* Admin %s sets you player level to %d", GetPlayerFormattedName(playerid), newlevel);
		}
		else 
		{
			SendClientMessageFormatted(playerid, COLOR_ERROR, "* Player with ID = %d does not exists!", id);
		}
	}
	return 1;
}*/

MODULE_FUNCTION(OnGameModeInit()) {}
MODULE_FUNCTION(OnGameModeExit()) {}
MODULE_FUNCTION(OnPlayerConnect(playerid)) {}
MODULE_FUNCTION(OnPlayerDisconnect(playerid, reason)) {}
MODULE_FUNCTION(OnPlayerRequestClass(playerid, classid)) {}
MODULE_FUNCTION(OnPlayerSpawn(playerid)) {}
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