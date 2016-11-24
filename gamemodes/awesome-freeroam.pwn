#include <a_samp>
#include <a_http>
#include <sccanf>

#include "include/awesome/a_dbutils.inc"
#include "include/awesome/a_utils.inc"
#include "include/awesome/a_reg.inc"
#include "include/awesome/a_consts.inc"
#include "include/awesome/a_commands.inc"
#include "include/awesome/a_menus.inc"
#include "include/awesome/a_vehutils.inc"
#include "include/awesome/a_props.inc"
#include "include/awesome/a_minigames.inc"
#include "include/awesome/a_race.inc"
#include "include/awesome/a_randmsg.inc"
#include "include/awesome/a_classselect.inc"
#include "include/awesome/a_log.inc"

//TODO create stored queries in each module
//TODO auto-repair on-off
//TODO auto-flip on-off
//TODO reorder forwards, constants, etc in all files

#define MODULES(%1); DB_%1; \
	REG_%1; \
	MENUS_%1; \
	VEH_%1; \
	PROPS_%1; \
	RACE_%1; \
	MG_%1; \
	RMSG_%1; \
	CMDS_%1; \
	LOG_%1; \
	/*CS_%1;*/
	
#define MODULES_1(%1); DB_%1; \
	REG_%1; \
	MENUS_%1; \
	VEH_%1; \
	PROPS_%1; \
	RACE_%1;

#define MODULES_2(%1); MG_%1; \
	RMSG_%1; \
	CMDS_%1; \
	LOG_%1; \
	CS_%1;

#define GAMEMODE_VERSION "0.1.001"

main()
{
	print("\n----------------------------------");
	print(" Awesome Freeroam v0.1 by snoopdesigns");
	print("----------------------------------\n");
}

PreloadAnimLib(playerid, animlib[]) 
{
	ApplyAnimation(playerid,animlib,"null",0.0,0,0,0,0,0);
}


public OnGameModeInit()
{
	MODULES(OnGameModeInit());

	LOG_writeFormatted("Awesome Freeroam starting, version: %s", GAMEMODE_VERSION);
	SetGameModeText("Awesome Freeroam v0.1");
	
	UsePlayerPedAnims();
	LOG_write("Awesome Freeroam successfully started!");
	return 1;
}

public OnGameModeExit()
{
    MODULES(OnGameModeExit());
	return 1;
}

public OnPlayerConnect(playerid)
{
	MODULES(OnPlayerConnect(playerid));
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	MODULES(OnPlayerDisconnect(playerid, reason));
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	MODULES(OnPlayerRequestClass(playerid, classid));
	return 1;
}

public OnPlayerSpawn(playerid)
{
	MODULES(OnPlayerSpawn(playerid));
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{	
	MODULES(OnPlayerCommandText(playerid, cmdtext));
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	MODULES(OnPlayerDeath(playerid, killerid, reason));
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
	MODULES(OnPlayerEnterRaceCheckpoint(playerid));
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
	MODULES(OnPlayerPickUpPickup(playerid, pickupid));
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
	MODULES_1(OnPlayerKeyStateChange(playerid, newkeys, oldkeys));
	MODULES_2(OnPlayerKeyStateChange(playerid, newkeys, oldkeys));
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
	MODULES_1(OnDialogResponse(playerid, dialogid, response, listitem, inputtext));
	MODULES_2(OnDialogResponse(playerid, dialogid, response, listitem, inputtext));
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	return 1;
}
