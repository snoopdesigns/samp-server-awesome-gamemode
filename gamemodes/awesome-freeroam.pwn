#include <a_samp>
#include <a_http>
#include <sccanf>

#include "include/awesome/a_dbutils.inc"
#include "include/awesome/a_utils.inc"
#include "include/awesome/a_reg.inc"
#include "include/awesome/a_consts.inc"
#include "include/awesome/a_commands.inc"
#include "include/awesome/a_classselect.inc"
#include "include/awesome/a_vehutils.inc"
#include "include/awesome/a_props.inc"
#include "include/awesome/a_race.inc"
/*#include "include/awesome/a_menus.inc"
#include "include/awesome/a_minigames.inc"
#include "include/awesome/a_randmsg.inc"*/
#include "include/awesome/a_log.inc"
#include "include/awesome/a_callbacks.inc"

//TODO create stored queries in each module
//TODO auto-repair on-off
//TODO auto-flip on-off
//TODO reorder forwards, constants, etc in all files

main()
{
	print("\n----------------------------------");
	print(" Awesome Freeroam v0.1 by snoopdesigns");
	print("----------------------------------\n");
}

public OnGameModeInit()
{
	LOG_write("===================================================");
	LOG_write(" * Awesome Freeroam Gamemode");
	LOG_writeFormatted(" * Version: %s", GAMEMODE_VERSION);
	for (new i = 0; i < sizeof(modules); i++) {
		LOG_writeFormatted(" * Loaded module: [%s]", modules[i][long_name]);
	}
	LOG_write("===================================================");
	callModulesCallback("OnGameModeInit");

	LOG_writeFormatted("Awesome Freeroam starting, version: %s", GAMEMODE_VERSION);
	SetGameModeText("Awesome Freeroam v0.1");
	
	UsePlayerPedAnims();
	LOG_write("Awesome Freeroam successfully started!");
	return 1;
}

public OnGameModeExit()
{
    callModulesCallback("OnGameModeExit");
	return 1;
}

public OnPlayerConnect(playerid)
{
	callModulesCallback("OnPlayerConnect", playerid);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	callModulesCallback("OnPlayerDisconnect", playerid);
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	callModulesCallback("OnPlayerRequestClass", playerid, classid);
	return 1;
}

public OnPlayerSpawn(playerid)
{
	callModulesCallback("OnPlayerSpawn", playerid);
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{	
	callModulesCallback("OnPlayerCommandText", playerid, cmdtext);
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	callModulesCallback("OnPlayerDeath", playerid, killerid, reason);
	return 1;
}

public OnVehicleSpawn(vehicleid)
{
	callModulesCallback("OnVehicleSpawn", vehicleid);
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	callModulesCallback("OnVehicleDeath", vehicleid, killerid);
	return 1;
}

public OnPlayerText(playerid, text[])
{
	callModulesCallback("OnPlayerText", playerid, text);
	return 1;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	callModulesCallback("OnPlayerEnterVehicle", playerid, vehicleid, ispassenger);
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	callModulesCallback("OnPlayerExitVehicle", playerid, vehicleid);
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	callModulesCallback("OnPlayerStateChange", playerid, newstate, oldstate);
	return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	callModulesCallback("OnPlayerEnterCheckpoint", playerid);
	return 1;
}

public OnPlayerLeaveCheckpoint(playerid)
{
	callModulesCallback("OnPlayerLeaveCheckpoint", playerid);
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid) {
	callModulesCallback("OnPlayerEnterRaceCheckpoint", playerid);
	return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
	callModulesCallback("OnPlayerLeaveRaceCheckpoint", playerid);
	return 1;
}

public OnRconCommand(cmd[])
{
	callModulesCallback("OnRconCommand", cmd);
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
	callModulesCallback("OnPlayerRequestSpawn", playerid);
	return 1;
}

public OnObjectMoved(objectid)
{	
	callModulesCallback("OnObjectMoved", objectid);
	return 1;
}

public OnPlayerObjectMoved(playerid, objectid)
{
	callModulesCallback("OnPlayerObjectMoved", playerid, objectid);
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	LOG_writeFormatted("Pickup id: %d", pickupid);
	callModulesCallback("OnPlayerPickUpPickup", playerid, pickupid);
	return 1;
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
	callModulesCallback("OnVehicleMod", playerid, vehicleid, componentid);
	return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	callModulesCallback("OnVehiclePaintjob", playerid, vehicleid, paintjobid);
	return 1;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	callModulesCallback("OnVehicleRespray", playerid, vehicleid, color1, color2);
	return 1;
}

public OnPlayerSelectedMenuRow(playerid, row)
{
	callModulesCallback("OnPlayerSelectedMenuRow", playerid, row);
	return 1;
}

public OnPlayerExitedMenu(playerid)
{
	callModulesCallback("OnPlayerExitedMenu", playerid);
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	callModulesCallback("OnPlayerInteriorChange", playerid, newinteriorid, oldinteriorid);
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	callModulesCallback("OnPlayerKeyStateChange", playerid, newkeys, oldkeys);
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	callModulesCallback("OnRconLoginAttempt", ip, password, success);
	return 1;
}

public OnPlayerUpdate(playerid)
{
	//callModulesCallback("OnPlayerUpdate", playerid);
	return 1;
}

public OnPlayerStreamIn(playerid, forplayerid)
{
	callModulesCallback("OnPlayerStreamIn", playerid, forplayerid);
	return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
	callModulesCallback("OnPlayerStreamOut", playerid, forplayerid);
	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	callModulesCallback("OnVehicleStreamIn", vehicleid, forplayerid);
	return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
	callModulesCallback("OnVehicleStreamOut", vehicleid, forplayerid);
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	callModulesCallback("OnDialogResponse", playerid, dialogid, response, listitem, inputtext);
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	callModulesCallback("OnPlayerClickPlayer", playerid, clickedplayerid, source);
	return 1;
}
