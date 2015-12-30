#include <a_samp>
#include <a_http>
#include <sccanf>

#include "include/awesome/a_dbutils.inc"
#include "include/awesome/a_reg.inc"
#include "include/awesome/a_consts.inc"
#include "include/awesome/a_commands.inc"
#include "include/awesome/a_menus.inc"
#include "include/awesome/a_vehutils.inc"
#include "include/awesome/a_props.inc"
#include "include/awesome/a_minigames.inc"

//TODO add a define to simplify format strings
//TODO camera up when re-spawning vehicle
//TODO auto-repair on-off
//TODO auto-flip on-off

#define dcmd(%1,%2,%3,%4) if(!strcmp((%3)[1], #%1, true, (%2)) && ((((%3)[(%2) + 1] == '\0') && (CMDS_checkCommandAccess(playerid, (%4)) && dcmd_%1(playerid, ""))) || (((%3)[(%2) + 1] == ' ') && (CMDS_checkCommandAccess(playerid, (%4)) && dcmd_%1(playerid, (%3)[(%2) + 2]))))) return 1

new DB:db_handle;

main()
{
	print("\n----------------------------------");
	print(" Awesome Freeroam v0.1 by snoopdesigns");
	print("----------------------------------\n");
}

public OnGameModeInit()
{
	SetGameModeText("Awesome Freeroam v0.1");
	
	db_handle = DBUTILS_initDatabase("test.db");
	REG_initRegSystem(db_handle);
	MENUS_initMenusSystem();
	VEH_initVehiclesSystem();
	PROPS_initPropsSystem(db_handle);
	MG_initMinigamesSystem(db_handle);
	
	//player classes
	AddPlayerClass(0,-2658.8489,627.7543,14.4531,179.5835,0,0,0,0,0,0);
	AddPlayerClass(1,-2658.8489,627.7543,14.4531,179.5835,0,0,0,0,0,0);
	return 1;
}

public OnGameModeExit()
{
    DBUTILS_shutdown(db_handle);
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	SetPlayerPos(playerid, -2658.8489,627.7543,14.4531);
	SetPlayerFacingAngle(playerid, 179.5835);
	SetPlayerCameraPos(playerid, -2658.8489,622.7543,14.4531);
	SetPlayerCameraLookAt(playerid, -2658.8489,627.7543,14.4531);
	return 1;
}

public OnPlayerConnect(playerid)
{
    //SendClientMessage(playerid, TEXT_COLOR_GREEN, "Welcome to our new server!");
    REG_authenticatePlayerOnConnect(playerid);
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	//help command
	dcmd(help, 4, cmdtext, 0);

	// register commands
	dcmd(login, 5, cmdtext, 0);
	dcmd(register, 8, cmdtext, 0);
	
	//vehicle commands
	dcmd(v, 1, cmdtext, 1);
	dcmd(car, 3, cmdtext, 1);
	dcmd(repair, 6, cmdtext, 1);
	
	//properties commands
	dcmd(cprop, 5, cmdtext, 2);
	dcmd(buy, 3, cmdtext, 1);
	
	//minigames
	dcmd(join, 4, cmdtext, 1);
	dcmd(leave, 5, cmdtext, 1);
	
	//admin commands
	dcmd(setscore, 8, cmdtext, 2);
	dcmd(setmoney, 8, cmdtext, 2);
	dcmd(restart, 7, cmdtext, 2);
	dcmd(setlevel, 8, cmdtext, 2);	
	return 0;
}

public OnPlayerDisconnect(playerid, reason)
{
	return 1;
}

public OnPlayerSpawn(playerid)
{
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	return 1;
}

public OnVehicleSpawn(vehicleid)
{
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	return 1;
}

public OnPlayerText(playerid, text[])
{
	return 1;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveCheckpoint(playerid)
{
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
	return 1;
}

public OnRconCommand(cmd[])
{
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
	return 1;
}

public OnObjectMoved(objectid)
{
	return 1;
}

public OnPlayerObjectMoved(playerid, objectid)
{
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	PROPS_OnPlayerPickUpPickup(playerid, pickupid);
	return 1;
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
	return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	return 1;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	return 1;
}

public OnPlayerSelectedMenuRow(playerid, row)
{
	return 1;
}

public OnPlayerExitedMenu(playerid)
{
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	VEH_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	return 1;
}

public OnPlayerUpdate(playerid)
{
	return 1;
}

public OnPlayerStreamIn(playerid, forplayerid)
{
	return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	MENUS_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	return 1;
}
