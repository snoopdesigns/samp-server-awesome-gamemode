#if defined _commands_included
	#endinput
#endif
#define _commands_included

#include "include/awesome/a_vehutils.inc"
#include "include/awesome/a_menus.inc"
#include "include/awesome/a_props.inc"
#include "include/awesome/a_utils.inc"

#include "include/awesome/colors.inc"

stock CMDS_checkCommandAccess(playerid, level)
{
	if(IsPlayerAdmin(playerid)) return TRUE;
	if(REG_getPlayerLevel(playerid) >= level)
	{
		return TRUE;
	} else {		
		if(REG_getPlayerLevel(playerid) == 0) //player is not registered
		{
			SendClientMessage(playerid, COLOR_ERROR, "* Sorry, you need to register to access this command!");
		}
		else 
		{
			SendClientMessage(playerid, COLOR_ERROR, "* Sorry, you have not access to this command!");
			SendClientMessageFormatted(playerid, COLOR_ERROR, "* Required player level: %d", level);
		}
		return FALSE;
	}
}

//=====================================================================
//========================COMMON COMMANDS==============================
//=====================================================================

dcmd_kill(playerid, params[])
{
	strlen(params);//just to handle compile warning
	SetPlayerHealth(playerid, 0.0);
	return 1;
}

dcmd_help(playerid, params[])
{
	strlen(params);//just to handle compile warning
	MENUS_showPlayerMenu(playerid, PLAYER_MENU_HELP);
	return 1;
}

//=====================================================================
//========================REGISTER COMMANDS============================
//=====================================================================

dcmd_register(playerid, params[])
{
	new	password[64];
	if (sscanf(params, "s", password)) 
	{
		SendClientMessage(playerid, COLOR_ERROR, "* Usage: \"/register <password>\"");
	}
	else
	{
		REG_registerNewPlayer(playerid, password);
	}
	return 1;
}

dcmd_login(playerid, params[])
{
	new	password[64];
	if (sscanf(params, "s", password)) 
	{
		SendClientMessage(playerid, COLOR_ERROR, "* Usage: \"/login <password>\"");
	}
	else
	{
		REG_loginPlayer(playerid, password);
	}
	return 1;
}

//=====================================================================
//========================VEHICLE COMMANDS=============================
//=====================================================================

dcmd_car(playerid, params[])
{
	strlen(params);//just to handle compile warning
	VEH_SpawnPlayerVehicle(playerid, 411);
	return 1;
}

dcmd_v(playerid, params[])
{
	new	modelid;
	if (sscanf(params, "i", modelid)) 
	{
		SendClientMessage(playerid, COLOR_ERROR, "* Usage: \"/v <modelid>\"");
	}
	else
	{
		VEH_SpawnPlayerVehicle(playerid, modelid);
	}
	return 1;
}

dcmd_repair(playerid, params[])
{
	strlen(params);//just to handle compile warning
	VEH_RepairPlayerVehicle(playerid);
	return 1;
}

//=====================================================================
//========================PROPERTIES COMMANDS==========================
//=====================================================================

dcmd_buy(playerid, params[])
{
	strlen(params);//just to handle compile warning
	PROPS_buyPropertyForPlayer(playerid);
	return 1;
}

dcmd_sell(playerid, params[])
{
	strlen(params);//just to handle compile warning
	PROPS_sellProperty(playerid);
	return 1;
}

dcmd_cprop(playerid, params[])
{
	new	propname[64];
	new cost;
	new earnings;
	if (sscanf(params, "sii", propname, cost, earnings)) 
	{
		SendClientMessage(playerid, COLOR_ERROR, "* Usage: \"/cprop <prop_name> <cost> <earnings>\"");
	}
	else
	{
		//create property
		PROPS_createProperty(playerid, propname, cost, earnings);
	}
	return 1;
}

//=====================================================================
//========================MINIGAMES COMMANDS===========================
//=====================================================================

dcmd_event(playerid, params[])
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
//========================RACES COMMANDS===============================
//=====================================================================

dcmd_join(playerid, params[])
{
	strlen(params);//just to handle compile warning
	RACE_join(playerid);
	return 1;
}

dcmd_lcp(playerid, params[])
{
	strlen(params);//just to handle compile warning
	RACE_movePlayerToLcp(playerid);
	return 1;
}

dcmd_cr(playerid, params[])
{
	new	filename[64];
	if (sscanf(params, "s", filename)) 
	{
		SendClientMessage(playerid, COLOR_ERROR, "* Usage: \"/cr <filename>\"");
	}
	else
	{
		RACE_createRace(playerid, filename);
	}
	return 1;
}

dcmd_crcheck(playerid, params[])
{
	strlen(params);//just to handle compile warning
	RACE_createRaceCheck(playerid);
	return 1;
}

dcmd_crend(playerid, params[])
{
	new	racename[64];
	if (sscanf(params, "s", racename)) 
	{
		SendClientMessage(playerid, COLOR_ERROR, "* Usage: \"/crend <name>\"");
	}
	else
	{
		RACE_createRaceEnd(playerid, racename);
	}
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
		SendClientMessageFormatted(playerid, COLOR_IMPROVEMENT, "* Your health has been set to %d", health);
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
			new targetname[MAX_PLAYER_NAME], playername[MAX_PLAYER_NAME];
			GetPlayerName(playerid, playername, sizeof playername);
			GetPlayerName(id, targetname, sizeof targetname);
			SendClientMessageFormatted(playerid, COLOR_IMPROVEMENT, "* Player '%s' score has been set to %d", targetname, score);
			SendClientMessageFormatted(id, COLOR_IMPROVEMENT, "* Admin '%s' set your score to %d", playername, score);
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
			new targetname[MAX_PLAYER_NAME], playername[MAX_PLAYER_NAME];
			GetPlayerName(playerid, playername, sizeof playername);
			GetPlayerName(id, targetname, sizeof targetname);
			SendClientMessageFormatted(playerid, COLOR_IMPROVEMENT, "* Player '%s' moneys has been increased to %d", targetname, money);
			SendClientMessageFormatted(id, COLOR_IMPROVEMENT, "* Admin '%s' gave you money %d", playername, money);
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
			SendClientMessageFormatted(playerid, COLOR_IMPROVEMENT, "* Player '%s' now has level %d", targetname, newlevel);
			SendClientMessageFormatted(id, COLOR_IMPROVEMENT, "* Admin '%s' sets you player level to %d", playername, newlevel);
		}
		else 
		{
			SendClientMessageFormatted(playerid, COLOR_ERROR, "* Player with ID = %d does not exists!", id);
		}
	}
	return 1;
}
