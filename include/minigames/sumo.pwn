#include <a_samp>

#include "include/awesome/a_minigames.inc"
#include "include/awesome/a_vehutils.inc"

forward MG_SUMO_CountDown();
forward MG_SUMO_TDScore();
forward MG_SUMO_FinishSumo();

new sumoObjects[19];

new playersInSumo[MAX_PLAYERS];
new PlayerText:sumoTextdraw[MAX_PLAYERS]; //textdraws
new playersInSumoTime[MAX_PLAYERS];

new sumoGameTimer = -1;
new sumoCountdownTimer[countdownTimerInfo];

new Float:sumoRandomSpawn[][3] =
{
    {-3554.9407,-868.8414,-0.4249},
    {-3560.7229,-977.4943,-0.2267},
    {-3668.3267,-975.8027,-0.5210},
    {-3608.0212,-791.7404,0.4193},
    {-3469.0007,-876.5690,-0.9225}
};

stock MG_SUMO_Init()
{
	for (new i = 0; i < MAX_PLAYERS; i++) // init playerLevel array with 0
	{
	    playersInSumo[i] = -1;
	    playersInSumoTime[i] = 0;
		sumoTextdraw[i] = PlayerText:-1;
	}
	
	sumoObjects[0] = CreateObject(18299, -3506.79199, -864.69110, 6.07326,   0.00000, 0.00000, 170.03987);
	sumoObjects[1] = CreateObject(18299, -3455.65698, -971.48041, 6.07326,   0.00000, 0.00000, 157.20018);
	sumoObjects[2] = CreateObject(18303, -3507.05957, -794.29736, 40.00000,   0.00000, 0.00000, 199.38008);
	sumoObjects[3] = CreateObject(18304, -3614.42065, -655.34546, 50.00000,   0.00000, 0.00000, 225.48013);
	sumoObjects[4] = CreateObject(18302, -3713.33838, -756.90833, 6.07330,   0.00000, 0.00000, 187.91989);
	sumoObjects[5] = CreateObject(18303, -3738.20703, -780.05078, 20.00000,   0.00000, 0.00000, 320.52017);
	sumoObjects[6] = CreateObject(18304, -3835.77075, -927.22565, 50.00000,   0.00000, 0.00000, 339.60004);
	sumoObjects[7] = CreateObject(18299, -3652.16138, -946.82379, 6.07326,   0.00000, 0.00000, 361.56009);
	sumoObjects[8] = CreateObject(18309, -3633.42944, -1015.10620, 6.07330,   0.00000, 0.00000, 310.37921);
	sumoObjects[9] = CreateObject(18304, -3600.25146, -1138.65222, 50.00000,   0.00000, 0.00000, 400.98029);
	sumoObjects[10] = CreateObject(18304, -3334.65210, -850.46045, 50.00000,   0.00000, 0.00000, 533.82007);
	sumoObjects[11] = CreateObject(18302, -3405.87695, -1042.00623, 6.07330,   0.00000, 0.00000, 392.93979);
	sumoObjects[12] = CreateObject(18303, -3485.45093, -1056.38904, 40.00000,   0.00000, 0.00000, 407.15973);
	sumoObjects[13] = CreateObject(10230, -3500.46973, -942.39203, 3.11180,   0.00000, -10.00000, 24.00000);
	sumoObjects[14] = CreateObject(10231, -3500.89111, -944.21271, 4.38000,   0.00000, -10.00000, 24.00000);
	sumoObjects[15] = CreateObject(17069, -3627.11011, -953.62122, -10.00000,   0.00000, 0.00000, 0.00000);
	sumoObjects[16] = CreateObject(17069, -3676.12549, -909.20624, -10.00000,   0.00000, 0.00000, 104.69999);
	sumoObjects[17] = CreateObject(17029, -3597.57495, -836.72491, -5.00000,   0.00000, 0.00000, 0.00000);
	sumoObjects[18] = CreateObject(17029, -3517.75366, -888.52612, -10.00000,   0.00000, 0.00000, -87.18000);
}

stock MG_SUMO_PreparePlayer(playerid)
{
	if(playersInSumo[playerid] == -1)
    {
        playersInSumo[playerid] = 1;
        playersInSumoTime[playerid] = 0;
        //set players virt world
		if(IsPlayerInAnyVehicle(playerid)) RemovePlayerFromVehicle(playerid);
    	new rand = random(sizeof(sumoRandomSpawn));
    	SetPlayerPos(playerid, sumoRandomSpawn[rand][0], sumoRandomSpawn[rand][1], sumoRandomSpawn[rand][2]);
		//VEH_SpawnPlayerVehicle(playerid, 430);
		ShowPlayerDialog(playerid, MG_JOIN_DIALOG, DIALOG_STYLE_MSGBOX, "Sumo Minigame", "Sumo minigame", "OK", "");
		SetPlayerCameraPos(playerid, -3411.032226, -850.590087, 43.952163);
		SetPlayerCameraLookAt(playerid, -3415.143066, -853.133117, 42.673809);
    }
    else
    {
        SendClientMessageToAll(COLOR_ERROR, "* Something goes wrong...=(");
    }
}

stock MG_SUMO_OnDialogResp(playerid)
{
	VEH_SpawnPlayerVehicle(playerid, 430);
}
	
stock MG_SUMO_PlayerLeftMinigame(playerid)
{
	if(playersInSumo[playerid] != -1)
    {
		playersInSumo[playerid] = -1;
		PlayerTextDrawHide(playerid, sumoTextdraw[playerid]);
		PlayerTextDrawDestroy(playerid, sumoTextdraw[playerid]);
		sumoTextdraw[playerid] = PlayerText:-1;
	}
}
	
stock MG_SUMO_Start()
{
	for (new i=0; i<MAX_PLAYERS; i++)
	{
	    if(playersInSumo[i] != -1 && IsPlayerConnected(i))
    	{
			sumoTextdraw[i] = CreatePlayerTextDraw(i, 473.333435, 334.148162,"~h~~y~Sumo Minigame~n~~r~Health 1000n~~r~Your time -");
			PlayerTextDrawLetterSize(i, sumoTextdraw[i], 0.400000, 1.600000);
			PlayerTextDrawTextSize(i, sumoTextdraw[i], 638.000000, 0.000000);
			PlayerTextDrawAlignment(i, sumoTextdraw[i], 1);
			PlayerTextDrawColor(i, sumoTextdraw[i], -1);
			PlayerTextDrawUseBox(i, sumoTextdraw[i], 1);
			PlayerTextDrawBoxColor(i, sumoTextdraw[i], 13141);
			PlayerTextDrawSetShadow(i, sumoTextdraw[i], 1);
			PlayerTextDrawSetOutline(i, sumoTextdraw[i], 0);
			PlayerTextDrawBackgroundColor(i, sumoTextdraw[i], 1499227944);
			PlayerTextDrawFont(i, sumoTextdraw[i], 2);
			PlayerTextDrawSetProportional(i, sumoTextdraw[i], 1);
			PlayerTextDrawSetShadow(i, sumoTextdraw[i], 0);
			TogglePlayerControllable(i,0);
    	}
	}
	sumoCountdownTimer[count] = 5;
	sumoCountdownTimer[timer] = SetTimer("MG_SUMO_CountDown", 1000, 1);
}
	
stock MG_SUMO_Destroy()
{
	if(sumoGameTimer != -1) 
	{
		KillTimer(sumoGameTimer);
	}
	for (new i = 0; i < 19; i++)
	{
		if(IsValidObject(sumoObjects[i]))
		{
			DestroyObject(sumoObjects[i]);
		}
	}
	
	for (new i = 0; i < MAX_PLAYERS; i++)
	{
		if(playersInSumo[i] != -1 && sumoTextdraw[i] != PlayerText:-1)
		{
			PlayerTextDrawHide(i, sumoTextdraw[i]);
			PlayerTextDrawDestroy(i, sumoTextdraw[i]);
		}
	}
}

public MG_SUMO_FinishSumo()
{
	new topSumoPlayers[3];
	topSumoPlayers = MG_SUMO_getTopPlayers();
	for(new i = 0;i < 3;i++)
	{
		if(topSumoPlayers[i] != -1)
		{
			new name[MAX_PLAYER_NAME];
			GetPlayerName(topSumoPlayers[i], name, sizeof(name));
			if(i == 0)
			{
				SendClientMessageToAllFormatted(COLOR_MG, "* '%s' finished Sumo minigame at 1st position with time: %d sec", name, (GetTickCount()-playersInSumoTime[i])/1000);
			}
			SendClientMessageFormatted(i, COLOR_MG, "* You finished Sumo minigame at %d position. Your time is %d sec", i+1, (GetTickCount()-playersInSumoTime[i])/1000);
		}
	}
	if(topSumoPlayers[0] == -1)
	{
		SendClientMessageToAll(COLOR_MG, "* No winners this time in Sumo minigame");
	}
	MG_OnCurrentMinigameFinish();
}

public MG_SUMO_TDScore()
{
	for (new i=0; i < MAX_PLAYERS; i++)
	{
	    if(playersInSumo[i] != -1 && IsPlayerConnected(i))
    	{
			new playerTextTD[256];
			new Float:health;
			if(IsPlayerInAnyVehicle(i))
			{
				new veh = GetPlayerVehicleID(i);
				GetVehicleHealth(veh, health);
				format(playerTextTD, sizeof(playerTextTD),"~h~~y~Sumo Minigame~n~~r~Health ~y~%d~n~~r~Your time ~y~%ds~n~", floatround(health), (GetTickCount()-playersInSumoTime[i])/1000);
				new topSumoPlayers[3];
				topSumoPlayers = MG_SUMO_getTopPlayers();
				for(new j = 0;j < 3;j++)
				{
					new playerSumoRes[64];
					if(topSumoPlayers[j] != -1)
					{
						new name[MAX_PLAYER_NAME];
						GetPlayerName(topSumoPlayers[j], name, sizeof(name));
						format(playerSumoRes, sizeof playerSumoRes, "~y~%d %s %ds~n~", j+1, name, (GetTickCount()-playersInSumoTime[topSumoPlayers[j]])/1000);
					}
					else
					{
						format(playerSumoRes, sizeof playerSumoRes, "~y~%d - - -~n~", j+1);
					}
					strins(playerTextTD, playerSumoRes, strlen(playerTextTD));
				}
				PlayerTextDrawSetString(i, sumoTextdraw[i], playerTextTD);
				PlayerTextDrawShow(i, sumoTextdraw[i]);
				
				//check player vehicle health
				if(health < 300)
				{
					SendClientMessage(i, COLOR_MG, "* You SUMO timer is restarted.");
					if(IsPlayerInAnyVehicle(i)) RemovePlayerFromVehicle(i);
					new rand = random(sizeof(sumoRandomSpawn));
					SetPlayerPos(i, sumoRandomSpawn[rand][0], sumoRandomSpawn[rand][1], sumoRandomSpawn[rand][2]);
					SetPlayerFacingAngle(i, 251);
					VEH_SpawnPlayerVehicle(i, 430);
					playersInSumoTime[i] = GetTickCount();
				}
			}
			else
			{
				SendClientMessage(i, COLOR_ERROR, "* Do not left the sumo vehicle!");
				VEH_SpawnPlayerVehicle(i, 430);
				playersInSumoTime[i] = GetTickCount();
			}
		}
	}
	return 1;
}

stock MG_SUMO_getTopPlayers()
{
	new sumoTopPlayers[3];
	for(new i = 0;i < 3;i++)
	{
		sumoTopPlayers[i] = -1;
	}
	new first=-1, second=-1, third=-1;
	new maxnum = 0, maxid = -1;
	for (new i=0; i < MAX_PLAYERS; i++)
	{
	    if(playersInSumo[i] != -1 && IsPlayerConnected(i))
    	{
			if((GetTickCount()-playersInSumoTime[i]) > maxnum)
			{
				maxnum = (GetTickCount()-playersInSumoTime[i]);
				maxid = i;
			}
		}
	}
	first = maxid;
	//TODO add second and third calculations
	sumoTopPlayers[0] = first;
	sumoTopPlayers[1] = second;
	sumoTopPlayers[2] = third;
	return sumoTopPlayers;
}

public MG_SUMO_CountDown()
{
	if(sumoCountdownTimer[count] == 0) 
	{
		KillTimer(sumoCountdownTimer[timer]);
		for (new i=0; i<MAX_PLAYERS; i++)
		{
			if(playersInSumo[i] != -1 && IsPlayerConnected(i))
			{
				GameTextForPlayer(i, "GO", 1000, 3);
				TogglePlayerControllable(i,1);
				
				//init player time
				playersInSumoTime[i] = GetTickCount();
				//start timers
				sumoGameTimer = SetTimer("MG_SUMO_TDScore", 1000, 1);
				//finish timer (20s)
				SetTimer("MG_SUMO_FinishSumo", 60 * 1000, 0);
			}
		}
		return 1;
	}

	new string[128], number[8];
	string = "~g~ Starting in ~y~";

	format(number, sizeof(number), "%d", sumoCountdownTimer[count]);
	strins(string, number, strlen(string));
	for (new i=0; i<MAX_PLAYERS; i++)
	{
	    if(playersInSumo[i] != -1 && IsPlayerConnected(i))
    	{
			GameTextForPlayer(i, string, 1000, 3);
		}
	}

	sumoCountdownTimer[count]--;
	return 1;
}
