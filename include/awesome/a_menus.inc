#if defined _menus_included
	#endinput
#endif
#define _menus_included

#include "include/awesome/a_commands.inc"
#include "include/awesome/colors.inc"
#include "include/awesome/a_utils.inc"

#define MODULE_FUNCTION(%1) forward MENUS_%1; public MENUS_%1

#define PLAYER_MENU_HELP 0
#define PLAYER_MENU_VEHICLES 1
#define PLAYER_MENU_PROPS 2

enum playerMenuInfo
{
	playerMenuHeading[128],
	playerMenuText[256],
	playerMenuType
	
}

new menus[3][playerMenuInfo];

MODULE_FUNCTION(OnGameModeInit())
{
	LOG_write("Menus Subsystem initializing");
	//help menu
	static dialog[256];
	static heading[128];
	format(dialog, sizeof dialog, "%s\n%s\n%s", 
		"Vehicle commands",
		"Player commands",
		"Properties commands");
	heading = "Available commands";
	menus[PLAYER_MENU_HELP][playerMenuHeading] = heading;
	menus[PLAYER_MENU_HELP][playerMenuText] = dialog;
	menus[PLAYER_MENU_HELP][playerMenuType] = DIALOG_STYLE_LIST;
	
	//vehicle commands
	format(dialog, sizeof dialog, "%s\n%s", 
		"Spawn Infernus: /car",
		"Repair vehicle: /fix");
	heading = "Vehicle commands";
	menus[PLAYER_MENU_VEHICLES][playerMenuHeading] = heading;
	menus[PLAYER_MENU_VEHICLES][playerMenuText] = dialog;
	menus[PLAYER_MENU_VEHICLES][playerMenuType] = DIALOG_STYLE_LIST;
	
	//props info
	format(dialog, sizeof dialog, "%s\n%s\n%s\n%s", 
		"This is description of properties system",
		"Available commands:",
		"Type /buy within 5 meters of property marker to buy it",
		"Type /sell within 5 meters of property marker to sell it");
	heading = "Properties";
	menus[PLAYER_MENU_PROPS][playerMenuHeading] = heading;
	menus[PLAYER_MENU_PROPS][playerMenuText] = dialog;
	menus[PLAYER_MENU_PROPS][playerMenuType] = DIALOG_STYLE_MSGBOX;
}

MODULE_FUNCTION(OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]))
{
	strlen(inputtext);//just to handle compilte warning
	if(response)
    {
		switch(dialogid) 
		{
			case PLAYER_MENU_HELP:
			{
				switch(listitem)
				{
					case 0:
					{
						MENUS_showPlayerMenu(playerid, PLAYER_MENU_VEHICLES);
					}
					case 1:
					{
						SendClientMessage(playerid, COLOR_ERROR, "Sorry, nothing here :(");
					}
					case 2:
					{
						MENUS_showPlayerMenu(playerid, PLAYER_MENU_PROPS);
					}
				}
			}
			case PLAYER_MENU_VEHICLES:
				switch(listitem)
				{
					case 0:
					{
						dcmd_car(playerid, "");
					}
					case 1:
					{
						dcmd_fix(playerid, "");
					}
				}
		}
	}
}

stock MENUS_showPlayerMenu(playerid, menuid)
{
	if(menus[menuid][playerMenuType] == DIALOG_STYLE_LIST)
	{
		ShowPlayerDialog(playerid, menuid, menus[menuid][playerMenuType], menus[menuid][playerMenuHeading], menus[menuid][playerMenuText], "Select", "Cancel");
	}
	else if(menus[menuid][playerMenuType] == DIALOG_STYLE_MSGBOX)
	{
		ShowPlayerDialog(playerid, menuid, menus[menuid][playerMenuType], menus[menuid][playerMenuHeading], menus[menuid][playerMenuText], "OK", "");
	}
}

MODULE_FUNCTION(OnGameModeExit()) {}
MODULE_FUNCTION(OnPlayerConnect(playerid)) {}
MODULE_FUNCTION(OnPlayerDisconnect(playerid, reason)) {}
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
MODULE_FUNCTION(OnPlayerClickPlayer(playerid, clickedplayerid, source)) {}
#undef MODULE_FUNCTION
