#if defined _minigames_included
	#endinput
#endif
#define _minigames_included

#include "include/minigames/fallout.pwn"

#define MINIGAMES_INTERVAL_IN_MIN 1

#define MG_STATUS_INACTIVE 0 
#define MG_STATUS_WAITING_PLAYERS 1 
#define MG_STATUS_ACTIVE 2 

//TODO check player already in minigame on join

new DB:db_handle_mg;
new minigames[1][] = {"Fallout"};
new activeMinigame = -1;
new mgStatus;
new minigamePlayers[MAX_PLAYERS];

forward MG_initNewMinigame();
forward MG_terminateCurrentMinigame();
forward MG_startCurrentMinigame();

stock MG_initMinigamesSystem(db_handle)
{
	db_handle_mg = db_handle;
	SetTimer("MG_initNewMinigame", 10000/*MINIGAMES_INTERVAL_IN_MIN * 60 * 1000*/, false);
	mgStatus = MG_STATUS_INACTIVE;
	for(new i; i < MAX_PLAYERS; i++)
	{
		minigamePlayers[i] = -1;
	}
}

public MG_initNewMinigame()
{
	if(mgStatus == MG_STATUS_INACTIVE)
	{
		new rand = random(sizeof(minigames));
		new msg[128];
		format(msg, sizeof msg, "Minigame %s initialized!", minigames[rand]);
		activeMinigame = rand;
		mgStatus = MG_STATUS_WAITING_PLAYERS;
		SendClientMessageToAll(TEXT_COLOR_GREEN, msg);
		SetTimer("MG_startCurrentMinigame", 10000, false);
		MG_InitMinigameEnv();
	}
	else
	{
		SendClientMessageToAll(TEXT_COLOR_GREEN, "There is still active MG!");
	}
}

public MG_startCurrentMinigame()
{
	mgStatus = MG_STATUS_ACTIVE;
	new msg[128];
	format(msg, sizeof msg, "Minigame %s started!", minigames[activeMinigame]);
	SendClientMessageToAll(TEXT_COLOR_GREEN, msg);
	MG_Start();
}

public MG_OnCurrentMinigameFinish()
{
	if(activeMinigame != -1)
	{
		new msg[128];
		format(msg, sizeof msg, "Minigame %s ended!", minigames[activeMinigame]);
		SendClientMessageToAll(TEXT_COLOR_GREEN, msg);
		MG_DestroyMinigameEnv();
		activeMinigame = -1;		
		mgStatus = MG_STATUS_INACTIVE;
		for(new i; i < MAX_PLAYERS; i++)
		{
			if(minigamePlayers[i] != -1)
			{
				RemovePlayerFromVehicle(i);
				SpawnPlayer(i);
			}
			minigamePlayers[i] = -1;
		}
		
		//schedule new minigame
		SetTimer("MG_initNewMinigame", 10000/*MINIGAMES_INTERVAL_IN_MIN * 60 * 1000*/, false);
	}
}

stock MG_InitMinigameEnv()
{
	switch (activeMinigame)
	{
		case 0:
		{
			MG_FALLOUT_Init();
		}
		case 1:
		{
			SendClientMessageToAll(TEXT_COLOR_GREEN, "init minigame env 1");
		}
		default:
		{
			SendClientMessageToAll(TEXT_COLOR_GREEN, "Something goes wrong... =(");
		}
	}
}

stock MG_DestroyMinigameEnv()
{
	switch (activeMinigame)
	{
		case 0:
		{
			MG_FALLOUT_Destroy();
		}
		case 1:
		{
			SendClientMessageToAll(TEXT_COLOR_GREEN, "destroy minigame env 1");
		}
		default:
		{
			SendClientMessageToAll(TEXT_COLOR_GREEN, "Something goes wrong... =(");
		}
	}
}

stock MG_Start()
{
	switch (activeMinigame)
	{
		case 0:
		{
			MG_FALLOUT_Start();
		}
		case 1:
		{
			SendClientMessageToAll(TEXT_COLOR_GREEN, "start minigame 1");
		}
		default:
		{
			SendClientMessageToAll(TEXT_COLOR_GREEN, "Something goes wrong... =(");
		}
	}
}

stock MG_registerPlayerForMinigame(playerid)
{
	if(mgStatus == MG_STATUS_WAITING_PLAYERS)
	{
		for(new i; i < MAX_PLAYERS; i++)
		{
			if(minigamePlayers[i] == -1)
			{
				minigamePlayers[i] = playerid;
			}
		}
		new msg[128];
		format(msg, sizeof msg, "You joined %s minigame", minigames[activeMinigame]);
		SendClientMessage(playerid, TEXT_COLOR_GREEN, msg);
		
		// prepare player for minigame
		MG_minigamePreparationForPlayer(playerid);
	}
	else
	{
		if(mgStatus == MG_STATUS_INACTIVE)
		{
			SendClientMessage(playerid, TEXT_COLOR_GREEN, "No active minigames for now!");
		} 
		else 
		{
			SendClientMessage(playerid, TEXT_COLOR_GREEN, "Minigame already started!");
		}
	}
}

stock MG_minigamePreparationForPlayer(playerid)
{
	switch (activeMinigame)
	{
		case 0:
		{
			MG_FALLOUT_PreparePlayer(playerid);
		}
		case 1:
		{
			SendClientMessage(playerid, TEXT_COLOR_GREEN, "Preparation for minigame 1");
		}
		default:
		{
			SendClientMessage(playerid, TEXT_COLOR_GREEN, "Something goes wrong... =(");
		}
	}
}