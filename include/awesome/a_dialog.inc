#if defined _dialog_included
	#endinput
#endif
#define _dialog_included

#include "include/awesome/a_commands.inc"
#include "include/awesome/colors.inc"
#include "include/awesome/a_utils.inc"

#define MODULE_FUNCTION(%1) forward DIALOG_%1; public DIALOG_%1

#define PLAYER_DIALOG_HELP 0
#define PLAYER_DIALOG_VEHICLES 1
#define PLAYER_DIALOG_PROPS 2

enum DIALOG_INFO {
	playerDialogHeading[32],
	playerDialogText[256],
	playerDialogType
}

new dialogs[3][DIALOG_INFO];

/* public functions declaration */
forward MENUS_showPlayerMenu(const playerid, const dialogid);

MODULE_FUNCTION(OnGameModeInit()) {
	LOG_write("Dialog Subsystem initializing");
	
	//help menu
	static dialog[256];
	static heading[32];
	format(dialog, sizeof dialog, "%s\n%s\n%s", 
		"Vehicle commands",
		"Player commands",
		"Properties commands");
	heading = "Available commands";
	dialogs[PLAYER_DIALOG_HELP][playerDialogHeading] = heading;
	dialogs[PLAYER_DIALOG_HELP][playerDialogText] = dialog;
	dialogs[PLAYER_DIALOG_HELP][playerDialogType] = DIALOG_STYLE_LIST;
	
	//vehicle commands
	format(dialog, sizeof dialog, "%s\n%s", 
		"Spawn Infernus: /car",
		"Repair vehicle: /fix");
	heading = "Vehicle commands";
	dialogs[PLAYER_DIALOG_VEHICLES][playerDialogHeading] = heading;
	dialogs[PLAYER_DIALOG_VEHICLES][playerDialogText] = dialog;
	dialogs[PLAYER_DIALOG_VEHICLES][playerDialogType] = DIALOG_STYLE_LIST;
	
	//props info
	format(dialog, sizeof dialog, "%s\n%s\n%s\n%s", 
		"This is description of properties system",
		"Available commands:",
		"Type /buy within 5 meters of property marker to buy it",
		"Type /sell within 5 meters of property marker to sell it");
	heading = "Properties";
	dialogs[PLAYER_DIALOG_PROPS][playerDialogHeading] = heading;
	dialogs[PLAYER_DIALOG_PROPS][playerDialogText] = dialog;
	dialogs[PLAYER_DIALOG_PROPS][playerDialogType] = DIALOG_STYLE_MSGBOX;
}

MODULE_FUNCTION(OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])) {
	LOG_writeFormatted("OnDialogResponse called: %d, %d, %d, %d, %s", playerid, dialogid, response, listitem, inputtext);
	strlen(inputtext);//just to handle compile warning
	if(response) {
		switch(dialogid)  {
			case PLAYER_DIALOG_HELP: {
				switch(listitem) {
					case 0:
					{
						MENUS_showPlayerMenu(playerid, PLAYER_DIALOG_VEHICLES);
					}
					case 1:
					{
						SendClientMessage(playerid, COLOR_ERROR, "Sorry, nothing here :(");
					}
					case 2:
					{
						MENUS_showPlayerMenu(playerid, PLAYER_DIALOG_PROPS);
					}
				}
			}
			case PLAYER_DIALOG_VEHICLES:
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

public MENUS_showPlayerMenu(const playerid, const dialogid) {
	if(dialogs[dialogid][playerDialogType] == DIALOG_STYLE_LIST) {
		ShowPlayerDialog(playerid, dialogid, dialogs[dialogid][playerDialogType], dialogs[dialogid][playerDialogHeading], dialogs[dialogid][playerDialogText], "Select", "Cancel");
	} else if(dialogs[dialogid][playerDialogType] == DIALOG_STYLE_MSGBOX) {
		ShowPlayerDialog(playerid, dialogid, dialogs[dialogid][playerDialogType], dialogs[dialogid][playerDialogHeading], dialogs[dialogid][playerDialogText], "OK", "");
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
