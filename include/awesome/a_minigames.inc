#if defined _minigames_included
	#endinput
#endif
#define _minigames_included

#define MINIGAMES_INTERVAL 7 * 60 * 1000 // 7 minutes
#define MINIGAMES_START_DELAY 20000 // 20 secs

#define MG_STATUS_INACTIVE 0 
#define MG_STATUS_WAITING_PLAYERS 1 
#define MG_STATUS_ACTIVE 2 

#define MG_JOIN_DIALOG 69

#include "include/minigames/fallout.pwn"
#include "include/minigames/hay.pwn"
#include "include/minigames/sumo.pwn"
#include "include/awesome/a_utils.inc"

new minigames[3][] = {"Fallout", "Hay", "Sumo"};
new activeMinigame = -1;
new mgStatus;
new minigamePlayers[MAX_PLAYERS];

//textdraw
new Text:minigameTD;

forward MG_initNewMinigame();
forward MG_terminateCurrentMinigame();
forward MG_startCurrentMinigame();
forward MG_updateTextdraw(textDrawShowCount);

stock MG_initMinigamesSystem()
{
	SetTimer("MG_initNewMinigame", MINIGAMES_INTERVAL, false);
	mgStatus = MG_STATUS_INACTIVE;
	for(new i; i < MAX_PLAYERS; i++)
	{
		minigamePlayers[i] = -1;
	}
	
	minigameTD = TextDrawCreate(399.999725, 0.222215, "test");
	TextDrawLetterSize(minigameTD, 0.271664, 1.107404);
	TextDrawTextSize(minigameTD, 638.000000, 0.000000);
	TextDrawAlignment(minigameTD, 1);
	TextDrawColor(minigameTD, -1);
	TextDrawUseBox(minigameTD, 1);
	TextDrawBoxColor(minigameTD, 13141);
	TextDrawSetShadow(minigameTD, 1);
	TextDrawSetOutline(minigameTD, 0);
	TextDrawBackgroundColor(minigameTD, 1499227944);
	TextDrawFont(minigameTD, 2);
	TextDrawSetProportional(minigameTD, 1);
}

public MG_initNewMinigame()
{
	if(mgStatus == MG_STATUS_INACTIVE)
	{
		new rand = random(sizeof(minigames));
		SendClientMessageToAllFormatted(COLOR_MG, "** Minigame %s is about to start...", minigames[rand]);
		SendClientMessageToAll(COLOR_MG, "* Type /event if you wish to join this minigame");
		
		new tdmsg[128];
		format(tdmsg, sizeof tdmsg, "~w~type_~y~/event_~w~to_join_the_~g~%s~n~~w~starting_in_~r~%d_seconds", minigames[rand], MINIGAMES_START_DELAY/1000);
		TextDrawSetString(minigameTD, tdmsg); 
		TextDrawShowForAll(minigameTD);
		new ccount = MINIGAMES_START_DELAY / 5000;
		SetTimerEx("MG_updateTextdraw", 5000, false, "i", ccount);
		
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

public MG_updateTextdraw(textDrawShowCount)
{
	new cc = textDrawShowCount;
	cc--;
	if(cc > 0) 
	{
		new tdmsg[128];
		format(tdmsg, sizeof tdmsg, "~w~type_~y~event_~w~to_join_the_~g~%s~n~~w~starting_in_~r~%d_seconds", minigames[activeMinigame], 5*cc);
		TextDrawSetString(minigameTD, tdmsg); 
		TextDrawShowForAll(minigameTD);
		SetTimerEx("MG_updateTextdraw", 5000, false, "i", cc);
	}
	else 
	{
		TextDrawHideForAll(minigameTD);
	}
}

public MG_startCurrentMinigame()
{
	new playerCount = 0;
	for(new i = 0; i < MAX_PLAYERS; i++) if(minigamePlayers[i] != -1) playerCount++;
	if(playerCount == 0)
	{
		SendClientMessageToAllFormatted(COLOR_MG, "* Minigame %s does not started, because of nobody comes.", minigames[activeMinigame]);
		MG_OnCurrentMinigameFinish();		
	}
	else 
	{
		mgStatus = MG_STATUS_ACTIVE;
		MG_Start();
	}
	//TextDrawHideForAll(minigameTD);
}

stock MG_OnCurrentMinigameFinish()
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
		case 2:
		{
			MG_SUMO_Init();
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
		case 2:
		{
			MG_SUMO_Destroy();
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
		case 2:
		{
			MG_SUMO_Start();
		}
		default:
		{
			SendClientMessageToAll(COLOR_ERROR, "* Something goes wrong... =(");
		}
	}
}

stock MG_checkCommandAccess(playerid, cmd[])
{
	if(IsPlayerAdmin(playerid)) return TRUE;
	if(minigamePlayers[playerid] != -1)
	{
		switch (activeMinigame)
		{
			case 0:
			{
				if(strcmp(cmd, "kill") != 0 && strcmp(cmd, "leave") != 0) 
				{
					SendClientMessage(playerid, COLOR_ERROR, "* Type /leave or /kill to leave minigame.");
					return FALSE;
				}
			}
			case 1:
			{
				if(strcmp(cmd, "kill") != 0 && strcmp(cmd, "leave") != 0) 
				{
					SendClientMessage(playerid, COLOR_ERROR, "* Type /leave or /kill to leave minigame.");
					return FALSE;
				}
			}
			case 2:
			{
				if(strcmp(cmd, "kill") != 0 && strcmp(cmd, "leave") != 0) 
				{
					SendClientMessage(playerid, COLOR_ERROR, "* Type /leave or /kill to leave minigame.");
					return FALSE;
				}
			}
			default:
			{
				SendClientMessage(playerid, COLOR_ERROR, "* Something goes wrong!");
				return TRUE;
			}
		}
	}
	else
	{
		//minigame specific commands should not be accessible not from minigame
	}
	return TRUE;
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
		SendClientMessageFormatted(playerid, COLOR_MG, "* You joined %s minigame", minigames[activeMinigame]);
		
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
			SendClientMessageFormatted(playerid, COLOR_MG, "* Minigame %s already started", minigames[activeMinigame]);
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
		case 2:
		{
			MG_SUMO_PlayerLeftMinigame(playerid);
		}
		case -1:
		{
			SendClientMessage(playerid, COLOR_ERROR, "* There is no active minigames for now!");
			return;
		}
		default:
		{
			SendClientMessage(playerid, COLOR_ERROR, "* Something goes wrong... =(");
			return;
		}
	}
	if(minigamePlayers[playerid] != -1)
	{
		if(IsPlayerInAnyVehicle(playerid)) RemovePlayerFromVehicle(playerid);
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
		case 2:
		{
			MG_SUMO_PreparePlayer(playerid);
		}
		default:
		{
			SendClientMessage(playerid, COLOR_ERROR, "Something goes wrong... =(");
		}
	}
}

stock MG_OnPlayerDisconnect(playerid)
{
	minigamePlayers[playerid] = -1;
}

stock MG_OnPlayerDeath(playerid)
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
		case 2:
		{
			MG_SUMO_PlayerLeftMinigame(playerid);
		}
	}
	if(minigamePlayers[playerid] != -1)
	{
		SendClientMessageFormatted(playerid, COLOR_ERROR, "* You died and loses %s minigame", minigames[activeMinigame]);
	}
	minigamePlayers[playerid] = -1;
}

stock MG_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	strlen(inputtext); // just to handle compile warnings
	if(listitem == 0) strlen(inputtext);
	if(dialogid == MG_JOIN_DIALOG && response == 1)
	{
		switch (activeMinigame)
		{
			case 2:
			{
				MG_SUMO_OnDialogResp(playerid);
			}
		}
		SetCameraBehindPlayer(playerid);
	}
}
