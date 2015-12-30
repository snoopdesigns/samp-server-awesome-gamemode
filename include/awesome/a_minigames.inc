#if defined _minigames_included
	#endinput
#endif
#define _minigames_included

#include "include/minigames/fallout.pwn"
#include "include/minigames/hay.pwn"

#define MINIGAMES_INTERVAL 4 * 60 * 1000 // 4 minutes
#define MINIGAMES_START_DELAY 15000 // 15 secs

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
	SetTimer("MG_initNewMinigame", MINIGAMES_INTERVAL, false);
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
		format(msg, sizeof msg, "** Minigame %s is about to start...", minigames[rand]);
		SendClientMessageToAll(COLOR_MG, msg);
		SendClientMessageToAll(COLOR_MG, "* Type /join if you wish to join this minigame");
		activeMinigame = rand;
		mgStatus = MG_STATUS_WAITING_PLAYERS;
		SetTimer("MG_startCurrentMinigame", MINIGAMES_START_DELAY, false);
		MG_InitMinigameEnv();
	}
	else
	{
		SendClientMessageToAll(COLOR_ERROR, "There is still active MG!");
	}
}

public MG_startCurrentMinigame()
{
	new playerCount = 0;
	for(new i = 0; i < MAX_PLAYERS; i++) if(minigamePlayers[i] != -1) playerCount++;
	if(playerCount == 0)
	{
		new msg[128];
		format(msg, sizeof msg, "* Minigame %s does not started, because of nobody comes.", minigames[activeMinigame]); 
		SendClientMessageToAll(COLOR_MG, msg);
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
		SetTimer("MG_initNewMinigame", MINIGAMES_INTERVAL, false);
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
			SendClientMessageToAll(COLOR_ERROR, "* Something goes wrong... =(");
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
			SendClientMessageToAll(COLOR_ERROR, "* Something goes wrong... =(");
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
			SendClientMessageToAll(COLOR_ERROR, "* Something goes wrong... =(");
		}
	}
}

stock MG_registerPlayerForMinigame(playerid)
{
	if(minigamePlayers[playerid] != -1)
	{
		SendClientMessage(playerid, COLOR_MG, "* You already in minigame!");
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
		format(msg, sizeof msg, "* You joined %s minigame", minigames[activeMinigame]);
		SendClientMessage(playerid, COLOR_MG, msg);
		
		// prepare player for minigame
		MG_minigamePreparationForPlayer(playerid);
	}
	else
	{
		if(mgStatus == MG_STATUS_INACTIVE)
		{
			SendClientMessage(playerid, COLOR_ERROR, "* There is no active minigames for now!");
		} 
		else 
		{
			new msg[128];
			format(msg, sizeof msg, "* Minigame %s already started", minigames[activeMinigame]);
			SendClientMessage(playerid, COLOR_MG, msg);
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
			SendClientMessage(playerid, COLOR_ERROR, "* There is no active minigames for now!");
			return 1;
		}
		default:
		{
			SendClientMessage(playerid, COLOR_ERROR, "* Something goes wrong... =(");
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
			SendClientMessage(playerid, COLOR_ERROR, "Something goes wrong... =(");
		}
	}
}