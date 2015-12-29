#if defined _commands_included
	#endinput
#endif
#define _commands_included

#include "include/awesome/a_vehutils.inc"
#include "include/awesome/a_menus.inc"
#include "include/awesome/a_props.inc"

stock CMDS_checkCommandAccess(playerid, level)
{
	if(REG_getPlayerLevel(playerid) >= level)
	{
		return TRUE;
	} else {
		if(REG_getPlayerLevel(playerid) == 0) //player is not registered
		{
			SendClientMessage(playerid, TEXT_COLOR_GREEN, "Sorry, you need to register to access this command!");
		}
		else 
		{
			SendClientMessage(playerid, TEXT_COLOR_GREEN, "Sorry, you have not access to this command!");
			new string[128];
			format(string, sizeof string, "Required player level: %d", level);
			SendClientMessage(playerid, TEXT_COLOR_GREEN, string);
		}
		return FALSE;
	}
}

dcmd_cprop(playerid, params[])
{
	new	propname[64];
	new cost;
	new earnings;
	if (sscanf(params, "sii", propname, cost, earnings)) 
	{
		SendClientMessage(playerid, TEXT_COLOR_GREEN, "Usage: \"/cprop <prop_name> <cost> <earnings>\"");
	}
	else
	{
		//create property
		PROPS_createProperty(playerid, propname, cost, earnings);
	}
	return 1;
}

dcmd_buy(playerid, params[])
{
	PROPS_buyPropertyForPlayer(playerid);
	return 1;
}

dcmd_help(playerid, params[])
{
	MENUS_showPlayerMenu(playerid, PLAYER_MENU_HELP);
	return 1;
}

dcmd_restart(playerid, params[])
{
	SendRconCommand("changemode mygamemode");
	return 1;
}

dcmd_setscore(playerid, params[])
{
	new	score;
	if (sscanf(params, "i", score)) 
	{
		SendClientMessage(playerid, TEXT_COLOR_GREEN, "Usage: \"/setscore <score>\"");
	}
	else
	{
		SetPlayerScore(playerid, score);
	}
	return 1;
}

dcmd_setmoney(playerid, params[])
{
	new	money;
	if (sscanf(params, "i", money)) 
	{
		SendClientMessage(playerid, TEXT_COLOR_GREEN, "Usage: \"/setmoney <money>\"");
	}
	else
	{
		GivePlayerMoney(playerid, money);
	}
	return 1;
}

dcmd_car(playerid, params[])
{
	VEH_SpawnPlayerVehicle(playerid, 411);
	return 1;
}

dcmd_v(playerid, params[])
{
	new	modelid;
	if (sscanf(params, "i", modelid)) 
	{
		SendClientMessage(playerid, TEXT_COLOR_GREEN, "Usage: \"/v <modelid>\"");
	}
	else
	{
		VEH_SpawnPlayerVehicle(playerid, modelid);
	}
	return 1;
}

dcmd_repair(playerid, params[])
{
	VEH_RepairPlayerVehicle(playerid);
	return 1;
}

dcmd_register(playerid, params[])
{
	new	password[64];
	if (sscanf(params, "s", password)) 
	{
		SendClientMessage(playerid, 0xFF0000AA, "Usage: \"/register <password>\"");
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
		SendClientMessage(playerid, 0xFF0000AA, "Usage: \"/login <password>\"");
	}
	else
	{
		REG_loginPlayer(playerid, password);
	}
	return 1;
}