#if defined _minigames_included
	#endinput
#endif
#define _minigames_included

#include "include/minigames/fallout.pwn"
#include "include/minigames/hay.pwn"

#define MINIGAMES_INTERVAL_IN_MIN 1

#define MG_STATUS_INACTIVE 0 
#define MG_STATUS_WAITING_PLAYERS 1 
#define MG_STATUS_ACTIVE 2 

new DB:db_handle_mg;
new minigames[2][] = {"Fallout", "Hay"};
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
		format(msg, sizeof msg, "Minigame %s is about to start!", minigames[rand]);
		SendClientMessageToAll(TEXT_COLOR_GREEN, msg);
		SendClientMessageToAll(TEXT_COLOR_GREEN, "Type /join to join minigame");
		activeMinigame = rand;
		mgStatus = MG_STATUS_WAITING_PLAYERS;
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
	new playerCount = 0;
	for(new i = 0; i < MAX_PLAYERS; i++) if(minigamePlayers[i] != -1) playerCount++;
	if(playerCount == 0)
	{
		new msg[128];
		format(msg, sizeof msg, "Minigame %s does not started, because no players", minigames[activeMinigame]); 
		SendClientMessageToAll(TEXT_COLOR_GREEN, msg);
		MG_OnCurrentMinigameFinish();		
	}
	else 
	{
		mgStatus = MG_STATUS_ACTIVE;
		MG_Start();
	}
}

public MG_OnCurrentMinigameFinish()
{
	if(activeMinigame != -1)
	{
		for(new i; i < MAX_PLAYERS; i++)
		{
			MG_PlayerLeftMinigame(i);
		}
		MG_DestroyMinigameEnv();
		activeMinigame = -1;		
		mgStatus = MG_STATUS_INACTIVE;
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
			MG_HAY_Init();
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
			MG_HAY_Destroy();
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
			MG_HAY_Start();
		}
		default:
		{
			SendClientMessageToAll(TEXT_COLOR_GREEN, "Something goes wrong... =(");
		}
	}
}

stock MG_registerPlayerForMinigame(playerid)
{
	if(minigamePlayers[playerid] != -1)
	{
		SendClientMessage(playerid, TEXT_COLOR_GREEN, "You already in minigame!");
		return;
	}
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

stock MG_PlayerLeftMinigame(playerid)
{
	switch (activeMinigame)
	{
		case 0:
		{
			MG_FALLOUT_PlayerLeftMinigame(playerid);
		}
		case 1:
		{
			MG_HAY_PlayerLeftMinigame(playerid);
		}
		case -1:
		{
			SendClientMessage(playerid, TEXT_COLOR_GREEN, "There is no active minigame.");
			return 1;
		}
		default:
		{
			SendClientMessage(playerid, TEXT_COLOR_GREEN, "Something goes wrong... =(");
			return 1;
		}
	}
	if(minigamePlayers[playerid] != -1)
	{
		RemovePlayerFromVehicle(playerid);
		SetPlayerPos(playerid, 0,0,0);
		SpawnPlayer(playerid);
	}
	minigamePlayers[playerid] = -1;
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
			MG_HAY_PreparePlayer(playerid);
		}
		default:
		{
			SendClientMessage(playerid, TEXT_COLOR_GREEN, "Something goes wrong... =(");
		}
	}
}