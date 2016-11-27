#if defined _classselect_included
	#endinput
#endif
#define _classselect_included

#include <a_samp>
#include "include/awesome/a_utils.inc"

#define MODULE_FUNCTION(%1) forward CS_%1; public CS_%1

new Text: txtClassSelHelper;
new gPlayerClass[MAX_PLAYERS];

MODULE_FUNCTION(OnGameModeInit()) {
	//MEDIC
	AddPlayerClass(274,-2658.8489,627.7543,14.4531,179.5835,0,0,0,0,0,0);
	AddPlayerClass(275,-2658.8489,627.7543,14.4531,179.5835,0,0,0,0,0,0);
	AddPlayerClass(276,-2658.8489,627.7543,14.4531,179.5835,0,0,0,0,0,0);
	
	//PIMP
	AddPlayerClass(249,-2658.8489,627.7543,14.4531,179.5835,0,0,0,0,0,0);
	AddPlayerClass(296,-2658.8489,627.7543,14.4531,179.5835,0,0,0,0,0,0);
	
	//POLICE
	AddPlayerClass(280,-2658.8489,627.7543,14.4531,179.5835,0,0,0,0,0,0);
	AddPlayerClass(282,-2658.8489,627.7543,14.4531,179.5835,0,0,0,0,0,0);
	AddPlayerClass(283,-2658.8489,627.7543,14.4531,179.5835,0,0,0,0,0,0);
	AddPlayerClass(284,-2658.8489,627.7543,14.4531,179.5835,0,0,0,0,0,0);
	
	//ARMY
	AddPlayerClass(287,-2658.8489,627.7543,14.4531,179.5835,0,0,0,0,0,0);
	
	//Fastfood
	AddPlayerClass(167,-2658.8489,627.7543,14.4531,179.5835,0,0,0,0,0,0);
	AddPlayerClass(155,-2658.8489,627.7543,14.4531,179.5835,0,0,0,0,0,0);
	AddPlayerClass(209,-2658.8489,627.7543,14.4531,179.5835,0,0,0,0,0,0);
	
	//GOLFERS
	AddPlayerClass(258,-2658.8489,627.7543,14.4531,179.5835,0,0,0,0,0,0);
	AddPlayerClass(259,-2658.8489,627.7543,14.4531,179.5835,0,0,0,0,0,0);
	
	//Workers
	AddPlayerClass(260,-2658.8489,627.7543,14.4531,179.5835,0,0,0,0,0,0);
	AddPlayerClass(16,-2658.8489,627.7543,14.4531,179.5835,0,0,0,0,0,0);
	AddPlayerClass(27,-2658.8489,627.7543,14.4531,179.5835,0,0,0,0,0,0);
	
	txtClassSelHelper = TextDrawCreate(23.333354, 331.555511, "");
	TextDrawLetterSize(txtClassSelHelper, 0.400000, 1.600000);
	TextDrawTextSize(txtClassSelHelper, 162.000000, 0.000000);
	TextDrawAlignment(txtClassSelHelper, 1);
	TextDrawColor(txtClassSelHelper, -1);
	TextDrawUseBox(txtClassSelHelper, 1);
	TextDrawBoxColor(txtClassSelHelper, 13141);
	TextDrawSetShadow(txtClassSelHelper, 1);
	TextDrawSetOutline(txtClassSelHelper, 0);
	TextDrawBackgroundColor(txtClassSelHelper, 1499227944);
	TextDrawFont(txtClassSelHelper, 2);
	TextDrawSetProportional(txtClassSelHelper, 1);
	
}

MODULE_FUNCTION(OnPlayerConnect(playerid)) {
	gPlayerClass[playerid] = -1;
	PreloadAnimLib(playerid,"BOMBER");
  	PreloadAnimLib(playerid,"RAPPING");
  	PreloadAnimLib(playerid,"SHOP");
  	PreloadAnimLib(playerid,"BEACH");
  	PreloadAnimLib(playerid,"SMOKING");
  	PreloadAnimLib(playerid,"FOOD");
  	PreloadAnimLib(playerid,"ON_LOOKERS");
  	PreloadAnimLib(playerid,"DEALER");
	PreloadAnimLib(playerid,"CRACK");
	PreloadAnimLib(playerid,"CARRY");
	PreloadAnimLib(playerid,"COP_AMBIENT");
	PreloadAnimLib(playerid,"PARK");
	PreloadAnimLib(playerid,"INT_HOUSE");
	PreloadAnimLib(playerid,"FOOD");
	PreloadAnimLib(playerid,"PED");
    ApplyAnimation(playerid,"DANCING","DNCE_M_B",4.0,1,0,0,0,-1);
}

MODULE_FUNCTION(OnPlayerRequestClass(playerid, classid)) {
	LOG_writeFormatted("Requesting class: %d for player: %d", classid, playerid);
	SetPlayerInterior(playerid,3);
	SetPlayerPos(playerid,-2673.8381,1399.7424,918.3516);
	SetPlayerFacingAngle(playerid,181.0);
    SetPlayerCameraPos(playerid,-2673.2776,1394.3859,918.3516);
	SetPlayerCameraLookAt(playerid,-2673.8381,1399.7424,918.3516);
	ApplyAnimation(playerid,"DANCING","DNCE_M_B",4.0,1,0,0,0,-1);
	
	new tdmsg[128];
	switch(classid) {
		case 0..2: {
			gPlayerClass[playerid] = 0; // medic
			format(tdmsg, sizeof tdmsg, "~w~class_~r~%s~n~~w~weapons~n~~r~sawn-off~n~tec9", "MEDIC");
		}
		case 3..4: {
			gPlayerClass[playerid] = 1;
			format(tdmsg, sizeof tdmsg, "~w~class_~r~%s~n~~w~weapons~n~~r~pistol~n~sniper", "PIMP");
		}
		case 5..8: {
			gPlayerClass[playerid] = 2;
			format(tdmsg, sizeof tdmsg, "~w~class_~r~%s~n~~w~weapons~n~~r~pistol~n~sniper", "POLICE");
		}
		case 9: {
			gPlayerClass[playerid] = 3;
			format(tdmsg, sizeof tdmsg, "~w~class_~r~%s~n~~w~weapons~n~~r~pistol~n~sniper", "ARMY");
		}
		case 10..12: {
			gPlayerClass[playerid] = 4;
			format(tdmsg, sizeof tdmsg, "~w~class_~r~%s~n~~w~weapons~n~~r~pistol~n~sniper", "FASTFOOD");
		}
		case 13..14: {
			gPlayerClass[playerid] = 5;
			format(tdmsg, sizeof tdmsg, "~w~class_~r~%s~n~~w~weapons~n~~r~pistol~n~sniper", "GOLFERS");
		}
		case 15..17: {
			gPlayerClass[playerid] = 6;
			format(tdmsg, sizeof tdmsg, "~w~class_~r~%s~n~~w~weapons~n~~r~pistol~n~sniper", "WORKERS");
		}
		default:{}
	}
	TextDrawHideForPlayer(playerid,txtClassSelHelper);
	TextDrawSetString(txtClassSelHelper, tdmsg); 
	TextDrawShowForPlayer(playerid,txtClassSelHelper);
}

MODULE_FUNCTION(OnPlayerSpawn(playerid)) {
	ClearAnimations(playerid);
	SetPlayerInterior(playerid,0);
	TextDrawHideForPlayer(playerid,txtClassSelHelper);
	switch(gPlayerClass[playerid]) {
		case 0: {
			SetPlayerPos(playerid, -2658.8489,627.7543,14.4531);
			SetPlayerFacingAngle(playerid, 179.5835);
		}
		case 1: {
			SetPlayerPos(playerid, -2632.3511,1393.5183,7.1016);
			SetPlayerFacingAngle(playerid, 189.5099);
		}
		case 2: {
			SetPlayerPos(playerid, -1634.8334,664.4601,7.1875);
			SetPlayerFacingAngle(playerid, 267.4529);
		}
		case 3: {
			SetPlayerPos(playerid, -1374.6626,457.9957,7.1875);
			SetPlayerFacingAngle(playerid, 0.4291);
		}
		case 4: {
			SetPlayerPos(playerid, -2674.9612,271.9800,4.3359);
			SetPlayerFacingAngle(playerid, 2.4552);
		}
		case 5: {
			SetPlayerPos(playerid, -2711.0923,-302.6165,7.1807);
			SetPlayerFacingAngle(playerid, 47.6342);
		}
		case 6: {
			SetPlayerPos(playerid, -2102.5830,163.5523,35.0998);
			SetPlayerFacingAngle(playerid, 359.6911);
		}
		default: {}
	}
	SetCameraBehindPlayer(playerid);
}

stock PreloadAnimLib(playerid, animlib[]) {
	ApplyAnimation(playerid,animlib,"null",0.0,0,0,0,0,0);
}

MODULE_FUNCTION(OnGameModeExit()) {}
MODULE_FUNCTION(OnPlayerDisconnect(playerid, reason)) {}
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
MODULE_FUNCTION(OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])) {}
MODULE_FUNCTION(OnPlayerClickPlayer(playerid, clickedplayerid, source)) {}
#undef MODULE_FUNCTION