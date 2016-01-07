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

//TODO auto-repair on-off
//TODO auto-flip on-off

#define dcmd(%1,%2,%3,%4) if(!strcmp((%3)[1], #%1, true, (%2)) && ((((%3)[(%2) + 1] == '\0') && (RACE_checkCommandAccess(playerid, (#%1)) && MG_checkCommandAccess(playerid, (#%1)) && CMDS_checkCommandAccess(playerid, (%4)) && dcmd_%1(playerid, ""))) || (((%3)[(%2) + 1] == ' ') && (CMDS_checkCommandAccess(playerid, (%4)) && dcmd_%1(playerid, (%3)[(%2) + 2]))))) return 1

new gPlayerClass[MAX_PLAYERS];
new Text: txtClassSelHelper;

new DB:db_handle;

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
	SetGameModeText("Awesome Freeroam v0.1");
	
	UsePlayerPedAnims();
	
	db_handle = DBUTILS_initDatabase("test.db");
	REG_initRegSystem(db_handle);
	MENUS_initMenusSystem();
	VEH_initVehiclesSystem();
	PROPS_initPropsSystem(db_handle);
	MG_initMinigamesSystem();
	RACE_initRaceSystem(db_handle);
	RMSG_InitRandomMsg();
	
	//player classes
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
	return 1;
}

public OnGameModeExit()
{
    DBUTILS_shutdown(db_handle);
	return 1;
}

public OnPlayerConnect(playerid)
{
    REG_authenticatePlayerOnConnect(playerid);
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
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	MG_OnPlayerDisconnect(playerid);
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	SetPlayerInterior(playerid,3);
	SetPlayerPos(playerid,-2673.8381,1399.7424,918.3516);
	SetPlayerFacingAngle(playerid,181.0);
    SetPlayerCameraPos(playerid,-2673.2776,1394.3859,918.3516);
	SetPlayerCameraLookAt(playerid,-2673.8381,1399.7424,918.3516);
	ApplyAnimation(playerid,"DANCING","DNCE_M_B",4.0,1,0,0,0,-1);
	
	new tdmsg[128];
	switch(classid)
	{
		case 0..2:
		{
			gPlayerClass[playerid] = 0; // medic
			format(tdmsg, sizeof tdmsg, "~w~class_~r~%s~n~~w~weapons~n~~r~sawn-off~n~tec9", "MEDIC");
		}
		case 3..4:
		{
			gPlayerClass[playerid] = 1;
			format(tdmsg, sizeof tdmsg, "~w~class_~r~%s~n~~w~weapons~n~~r~pistol~n~sniper", "PIMP");
		}
		case 5..8:
		{
			gPlayerClass[playerid] = 2;
			format(tdmsg, sizeof tdmsg, "~w~class_~r~%s~n~~w~weapons~n~~r~pistol~n~sniper", "POLICE");
		}
		case 9:
		{
			gPlayerClass[playerid] = 3;
			format(tdmsg, sizeof tdmsg, "~w~class_~r~%s~n~~w~weapons~n~~r~pistol~n~sniper", "ARMY");
		}
		case 10..12:
		{
			gPlayerClass[playerid] = 4;
			format(tdmsg, sizeof tdmsg, "~w~class_~r~%s~n~~w~weapons~n~~r~pistol~n~sniper", "FASTFOOD");
		}
		case 13..14:
		{
			gPlayerClass[playerid] = 5;
			format(tdmsg, sizeof tdmsg, "~w~class_~r~%s~n~~w~weapons~n~~r~pistol~n~sniper", "GOLFERS");
		}
		case 15..17:
		{
			gPlayerClass[playerid] = 6;
			format(tdmsg, sizeof tdmsg, "~w~class_~r~%s~n~~w~weapons~n~~r~pistol~n~sniper", "WORKERS");
		}
		default:
		{
			
		}
	}
	TextDrawHideForPlayer(playerid,txtClassSelHelper);
	TextDrawSetString(txtClassSelHelper, tdmsg); 
	TextDrawShowForPlayer(playerid,txtClassSelHelper);
	return 1;
}

public OnPlayerSpawn(playerid)
{
	ClearAnimations(playerid);
	SetPlayerInterior(playerid,0);
	TextDrawHideForPlayer(playerid,txtClassSelHelper);
	switch(gPlayerClass[playerid])
	{
		case 0:
		{
			SetPlayerPos(playerid, -2658.8489,627.7543,14.4531);
			SetPlayerFacingAngle(playerid, 179.5835);
		}
		case 1:
		{
			SetPlayerPos(playerid, -2632.3511,1393.5183,7.1016);
			SetPlayerFacingAngle(playerid, 189.5099);
		}
		case 2:
		{
			SetPlayerPos(playerid, -1634.8334,664.4601,7.1875);
			SetPlayerFacingAngle(playerid, 267.4529);
		}
		case 3:
		{
			SetPlayerPos(playerid, -1374.6626,457.9957,7.1875);
			SetPlayerFacingAngle(playerid, 0.4291);
		}
		case 4:
		{
			SetPlayerPos(playerid, -2674.9612,271.9800,4.3359);
			SetPlayerFacingAngle(playerid, 2.4552);
		}
		case 5:
		{
			SetPlayerPos(playerid, -2711.0923,-302.6165,7.1807);
			SetPlayerFacingAngle(playerid, 47.6342);
		}
		case 6:
		{
			SetPlayerPos(playerid, -2102.5830,163.5523,35.0998);
			SetPlayerFacingAngle(playerid, 359.6911);
		}
		default:
		{
		}
	}
	SetCameraBehindPlayer(playerid);
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	dcmd(kill, 4, cmdtext, 0);
	
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
	dcmd(sell, 4, cmdtext, 1);
	
	//minigames
	dcmd(event, 5, cmdtext, 1);
	dcmd(leave, 5, cmdtext, 1);
	
	//races
	dcmd(join, 4, cmdtext, 1);
	dcmd(lcp, 3, cmdtext, 1);
	dcmd(cr, 2, cmdtext, 2);
	dcmd(crcheck, 7, cmdtext, 2);
	dcmd(crend, 5, cmdtext, 2);
	
	//admin commands
	dcmd(setscore, 8, cmdtext, 2);
	dcmd(setmoney, 8, cmdtext, 2);
	dcmd(restart, 7, cmdtext, 2);
	dcmd(setlevel, 8, cmdtext, 2);	
	SendClientMessage(playerid, COLOR_ERROR, "* Unknown command passed.");
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	MG_OnPlayerDeath(playerid);
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
	RACE_OnEnterCheckpoint(playerid);
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
