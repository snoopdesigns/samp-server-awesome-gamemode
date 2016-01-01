#include <a_samp>

#include "include/awesome/a_minigames.inc"

forward MG_FALLOUT_CountDown();
forward MG_FALLOUT_StartFalling();
forward MG_FALLOUT_SolarFall();
forward MG_FALLOUT_LoseGame();
forward MG_FALLOUT_SquareShake(id);

#define PANEL_STATE_ACTIVE 0
#define PANEL_STATE_DESTROYED 1

enum panelInfo
{
	objectid,
	timer,
	panelState,
	shaketimer,
	shakecount
}

enum countdownTimerInfo
{
	count,
	timer
}

enum gameInfo
{
	timer,
	losetimer
}

new panel[100][panelInfo];
new countdownTimer[countdownTimerInfo];
new game[gameInfo];

new playersInFallout[MAX_PLAYERS];

public MG_FALLOUT_Init()
{	
	countdownTimer[count] = 5;
	new cc = 0;
	
	panel[cc][objectid] = CreateObject(1697, 2482.1921, -1660.4783, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2477.7395, -1660.4783, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2473.2869, -1660.4783, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2468.8343, -1660.4783, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2464.3817, -1660.4783, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2459.9291, -1660.4783, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2455.4765, -1660.4783, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2451.0239, -1660.4783, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2446.5713, -1660.4783, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2442.1187, -1660.4783, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2482.1921, -1655.1112, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2477.7395, -1655.1112, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2473.2869, -1655.1112, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2468.8343, -1655.1112, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2464.3817, -1655.1112, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2459.9291, -1655.1112, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2455.4765, -1655.1112, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2451.0239, -1655.1112, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2446.5713, -1655.1112, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2442.1187, -1655.1112, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2482.1921, -1649.7442, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2477.7395, -1649.7442, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2473.2869, -1649.7442, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2468.8343, -1649.7442, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2464.3817, -1649.7442, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2459.9291, -1649.7442, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2455.4765, -1649.7442, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2451.0239, -1649.7442, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2446.5713, -1649.7442, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2442.1187, -1649.7442, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2482.1921, -1644.3772, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2477.7395, -1644.3772, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2473.2869, -1644.3772, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2468.8343, -1644.3772, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2464.3817, -1644.3772, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2459.9291, -1644.3772, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2455.4765, -1644.3772, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2451.0239, -1644.3772, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2446.5713, -1644.3772, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2442.1187, -1644.3772, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2482.1921, -1639.0102, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2477.7395, -1639.0102, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2473.2869, -1639.0102, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2468.8343, -1639.0102, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2464.3817, -1639.0102, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2459.9291, -1639.0102, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2455.4765, -1639.0102, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2451.0239, -1639.0102, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2446.5713, -1639.0102, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2442.1187, -1639.0102, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2482.1921, -1633.6432, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2477.7395, -1633.6432, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2473.2869, -1633.6432, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2468.8343, -1633.6432, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2464.3817, -1633.6432, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2459.9291, -1633.6432, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2455.4765, -1633.6432, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2451.0239, -1633.6432, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2446.5713, -1633.6432, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2442.1187, -1633.6432, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2482.1921, -1628.2762, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2477.7395, -1628.2762, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2473.2869, -1628.2762, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2468.8343, -1628.2762, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2464.3817, -1628.2762, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2459.9291, -1628.2762, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2455.4765, -1628.2762, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2451.0239, -1628.2762, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2446.5713, -1628.2762, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2442.1187, -1628.2762, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2482.1921, -1622.9092, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2477.7395, -1622.9092, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2473.2869, -1622.9092, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2468.8343, -1622.9092, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2464.3817, -1622.9092, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2459.9291, -1622.9092, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2455.4765, -1622.9092, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2451.0239, -1622.9092, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2446.5713, -1622.9092, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2442.1187, -1622.9092, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2482.1921, -1617.5422, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2477.7395, -1617.5422, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2473.2869, -1617.5422, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2468.8343, -1617.5422, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2464.3817, -1617.5422, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2459.9291, -1617.5422, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2455.4765, -1617.5422, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2451.0239, -1617.5422, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2446.5713, -1617.5422, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2442.1187, -1617.5422, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2482.1921, -1612.1752, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2477.7395, -1612.1752, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2473.2869, -1612.1752, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2468.8343, -1612.1752, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2464.3817, -1612.1752, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2459.9291, -1612.1752, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2455.4765, -1612.1752, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2451.0239, -1612.1752, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2446.5713, -1612.1752, 160.0000, 31.8000, 0.0000, 0.0000);
	cc++;panel[cc][objectid] = CreateObject(1697, 2442.1187, -1612.1752, 160.0000, 31.8000, 0.0000, 0.0000);
	
	for(new i=0; i < 100; i++)
	{
		panel[i][panelState] = PANEL_STATE_ACTIVE;
		panel[i][shakecount] = 0;
		panel[i][shaketimer] = -1;
	}
	
	for (new i = 0; i < MAX_PLAYERS; i++) // init players array
	{
	    playersInFallout[i] = -1;
	}
}

public MG_FALLOUT_Destroy()
{
	for(new i = 0; i < 100; i++) 
	{
		if(IsValidObject(panel[i][objectid])) 
		{
			DestroyObject(panel[i][objectid]);
		}
		if(panel[i][shaketimer] != -1)
		{
			KillTimer(panel[i][shaketimer]);
		}
	}
}

public MG_FALLOUT_Start()
{
	//set player pos random 2442-2482, -1660 = -1612
	//controllable false
	for (new i=0; i<MAX_PLAYERS; i++)
	{
	    if(playersInFallout[i] != -1 && IsPlayerConnected(i))
    	{
			//rand position
			new distx = random(15);
			new distxdirection = random(10);
			new disty = random(15)
			new distydirection = random(10);
			new x = 2442;
			new y = -1612;
			if(distxdirection <= 4) x -= distx; else x += distx;
			if(distydirection <= 4) y -= disty; else y += disty;
			SetPlayerPos(i, float(x), float(y), 162.0000);
			TogglePlayerControllable(i,0);
		}
	}
	countdownTimer[timer] = SetTimer("MG_FALLOUT_CountDown", 1000, 1);
	SetTimer("MG_FALLOUT_StartFalling", countdownTimer[count] * 1000, 0);
	
}

public MG_FALLOUT_PreparePlayer(playerid)
{
	if(playersInFallout[playerid] == -1)
    {
		playersInFallout[playerid] = 1;
		if(IsPlayerInAnyVehicle(playerid)) RemovePlayerFromVehicle(playerid);
		SetPlayerPos(playerid, 2473.2869, -1617.5422, 162.0000);
		ShowPlayerDialog(playerid, 69, DIALOG_STYLE_MSGBOX, "Fallout Minigame", "Fallout Minigame by Lazarus", "OK", "");
	}
	else
	{
		SendClientMessageToAll(COLOR_ERROR, "* Something goes wrong...=( player already in game");
	}
}

stock MG_FALLOUT_PlayerLeftMinigame(playerid)
{
	if(playersInFallout[playerid] != -1)
    {
		playersInFallout[playerid] = -1;
	}
}

public MG_FALLOUT_StartFalling()
{
	game[timer] = SetTimer("MG_FALLOUT_SolarFall", 500, 1);
	game[losetimer] = SetTimer("MG_FALLOUT_LoseGame", 500, 1);
	return 1;
}

public MG_FALLOUT_SolarFall()
{
	new go;
	new playersRemaining;
	new string[256], PlayerN[MAX_PLAYER_NAME];
	for(new i = 0; i < 100; i++) if(panel[i][panelState] == PANEL_STATE_ACTIVE) go++;
	for(new i = 0; i < MAX_PLAYERS; i++) if(playersInFallout[i] != -1) playersRemaining++;
	
	if(go == 3)
	{
		KillTimer(game[timer]);
		KillTimer(game[losetimer]);
		if(playersRemaining > 0)
		{
			for(new i = 0; i < MAX_PLAYERS; i++)
			{
				if(playersInFallout[i] != -1) {
					GetPlayerName(i, PlayerN, sizeof(PlayerN));
					format(string, sizeof(string),"* %s Wins The Fallout Minigame", PlayerN);
					SendClientMessageToAll(COLOR_MG, string);
				}
			}
		}
		else 
		{
			SendClientMessageToAll(COLOR_MG, "* No winners this time in Fallout minigame");
		}
		MG_OnCurrentMinigameFinish();
		return 1;
		//decide winners
	}
	start:
	new objid = random(100);
	
	if(panel[objid][panelState] == PANEL_STATE_DESTROYED) goto start;
	
	panel[objid][panelState] = PANEL_STATE_DESTROYED;
	panel[objid][shaketimer] = SetTimerEx("MG_FALLOUT_SquareShake", 100, 1, "i", objid);
	return 1;
}

public MG_FALLOUT_SquareShake(id)
{
	switch(panel[id][shakecount])
	{
		case 0, 5:
		{
			SetObjectRot(panel[id][objectid], 31.8, 2, 0);
		}
		case 1, 6:
		{
			SetObjectRot(panel[id][objectid], 33.8, 0, 0);
		}
		case 2, 7:
		{
			SetObjectRot(panel[id][objectid], 31.8, -2, 0);
		}
		case 3, 8:
		{
			SetObjectRot(panel[id][objectid], 29.8, 0, 0);
		}
		case 4, 9:
		{
			SetObjectRot(panel[id][objectid], 31.8, 0, 0);
		}
		case 10:
		{
			new Float:X, Float:Y, Float:Z;
			GetObjectPos(panel[id][objectid], X, Y, Z);
			MoveObject(panel[id][objectid], X, Y, Z - 100, 4);
		}
		case 11..99:
		{
		   SetObjectRot(panel[id][objectid], 31.8 - ((panel[id][shakecount] * 2) - 20), 0, 0);
		}
		case 100:
		{
			DestroyObject(panel[id][objectid]);
			KillTimer(panel[id][shaketimer]);
			panel[id][shaketimer] = -1;
		}
	}

	panel[id][shakecount]++;
	return 1;
}

public MG_FALLOUT_LoseGame()
{
	new Float:X, Float:Y, Float:Z;
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if(playersInFallout[i] != -1 && IsPlayerConnected(i))
		{
			GetPlayerPos(i, X, Y, Z);

			if(Z <= 158)
			{
				GameTextForPlayer(i, "~r~You lose!", 5000, 3);
				playersInFallout[i] = -1;
				MG_PlayerLeftMinigame(i);
			}
		}
	}
	return 1;
}

public MG_FALLOUT_CountDown()
{
	if(countdownTimer[count] == 0) 
	{
		KillTimer(countdownTimer[timer]);
		for (new i=0; i<MAX_PLAYERS; i++)
		{
			if(playersInFallout[i] != -1 && IsPlayerConnected(i))
			{
				GameTextForAll("GO", 1000, 3);
				TogglePlayerControllable(i,1);
			}
		}
		return 1;
	}

	new string[128], number[8];
	string = "~g~ Starting in ~y~";

	format(number, sizeof(number), "%d", countdownTimer[count]);
	strins(string, number, strlen(string));
	for (new i=0; i<MAX_PLAYERS; i++)
	{
	    if(playersInFallout[i] != -1 && IsPlayerConnected(i))
    	{
			GameTextForPlayer(i, string, 1000, 3);
		}
	}

	countdownTimer[count]--;
	return 1;
}