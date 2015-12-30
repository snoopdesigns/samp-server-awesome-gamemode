#include <a_samp>

#include "include/awesome/a_minigames.inc"

forward CountDown();
forward StartFalling();
forward SolarFall();
forward LoseGame();
forward SquareShake(id);

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
	timer
}

new panel[100][panelInfo];
new countdownTimer[countdownTimerInfo];
new game[gameInfo];

new players[MAX_PLAYERS];

public MG_FALLOUT_Init()
{
	SendClientMessageToAll(TEXT_COLOR_GREEN, "Fallout minigame initializing...");
	
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
	
	new msg[64];format(msg, sizeof msg, "Total objects: %d", cc);
	SendClientMessageToAll(TEXT_COLOR_GREEN, msg);
	
	for(new i=0; i < 100; i++)
	{
		panel[i][panelState] = PANEL_STATE_ACTIVE;
		panel[i][shakecount] = 0;
		panel[i][shaketimer] = -1;
	}
}

public MG_FALLOUT_Destroy()
{
	SendClientMessageToAll(TEXT_COLOR_GREEN, "Fallout minigame destroying...");
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
	SendClientMessageToAll(TEXT_COLOR_GREEN, "Fallout minigame starting...");
	//set player pos random
	//controllable false
	countdownTimer[timer] = SetTimer("CountDown", 1000, 1);
	SetTimer("StartFalling", countdownTimer[count] * 1000, 0);
	
}

public MG_FALLOUT_PreparePlayer(playerid)
{
	SendClientMessage(playerid, TEXT_COLOR_GREEN, "You are now prepared to minigame Fallout");
	SetPlayerPos(playerid, 2473.2869, -1617.5422, 162.0000);
}

public StartFalling()
{
	game[timer] = SetTimer("SolarFall", 500, 1);
	//SetTimer("LoseGame", 500, 1);
	return 1;
}

public SolarFall()
{
	new go;
	for(new i = 0; i < 100; i++) if(panel[i][panelState] == PANEL_STATE_ACTIVE) go++;
	new msg[64];format(msg, sizeof msg, "Panels remained: %d", go);
	SendClientMessageToAll(TEXT_COLOR_GREEN, msg);
	if(go == 3)
	{
		KillTimer(game[timer]);
		MG_OnCurrentMinigameFinish();
		return 1;
		//decide winners
	}
	start:
	new objid = random(100);
	
	if(panel[objid][panelState] == PANEL_STATE_DESTROYED) goto start;
	
	panel[objid][panelState] = PANEL_STATE_DESTROYED;

	if(panel[objid][shaketimer] == -1)
	{
		panel[objid][shaketimer] = SetTimerEx("SquareShake", 100, 1, "i", objid);
	}
	else
	{
		SendClientMessageToAll(TEXT_COLOR_GREEN, "FUUUUUUU");
	}
	return 1;
}

public SquareShake(id)
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
			if(panel[id][shaketimer] != -1)
			{
				KillTimer(panel[id][shaketimer]);
			} else 
			{
				SendClientMessageToAll(TEXT_COLOR_GREEN, "TRATATATATATA");
			}
			panel[id][shaketimer] = -1;
		}
	}

	panel[id][shakecount]++;
	return 1;
}

public CountDown()
{
	if(countdownTimer[count] == 0) 
	{
		KillTimer(countdownTimer[timer]);
		GameTextForAll("GO", 1000, 3);
		return 1;
	}

	new string[128], number[8];
	string = "~g~ Starting in ~y~";

	format(number, sizeof(number), "%d", countdownTimer[count]);
	strins(string, number, strlen(string));
	GameTextForAll(string, 1000, 3);

	countdownTimer[count]--;
	return 1;
}