#include <a_samp>

#include "include/awesome/a_minigames.inc"

#define HAY_X 4 // count of hays in X
#define HAY_Y 4 // count of hays in Y
#define HAY_Z 30 // count of hays in Z (hay height)
#define HAY_B 146 // total numbers of hays
#define HAY_R 4
#define SPEED_FACTOR 3000.0
#define ID_HAY_OBJECT 3374

forward MG_HAY_TimerMove();
forward MG_HAY_FinishTimer(id, xq, yq, zq);
forward MG_HAY_TDScore();
forward MG_HAY_CountDown();

new hayMatrix[HAY_X][HAY_Y][HAY_Z]; // if element is 0, then object does not exists, otherwise exists
new hayObjects[HAY_B]; // array of hay objects

new playersInHay[MAX_PLAYERS];
new playerLevel[MAX_PLAYERS];
new PlayerText:hayTextdraw[MAX_PLAYERS];

new speed_xy; // moving speed across x-y axis
new speed_z; // moving speed across z axis
new Center_x;
new Center_y; //WTF?

new hayStartTime;
new gameTimer = -1;

new hayCountdownTimer[countdownTimerInfo];

new Float:hayRandomSpawn[][3] =
{
    {7.4116,-3.7491,3.1172},
    {1.3451,-16.2426,3.1172},
    {-10.7744,-19.3668,3.1172},
    {-20.4327,-12.6451,3.1172},
    {0.3451,6.5426,3.2172}
};

stock MG_HAY_Init()
{
	new xq, yq, zq;

	speed_xy = 2000 / (HAY_Z + 1);
	speed_z = 1500 / (HAY_Z + 1);
	
	for(xq = 0; xq < HAY_X ; xq++)
	{
		for(yq = 0; yq < HAY_Y ; yq++)
		{
			for(zq = 0; zq < HAY_Z ; zq++)
			{
				hayMatrix[xq][yq][zq] = 0;
			}
		}
	}
	
	for (new i = 0; i < MAX_PLAYERS; i++) // init playerLevel array with 0
	{
	    playerLevel[i] = 0;
	    playersInHay[i] = -1;
		hayTextdraw[i] = PlayerText:-1;
	}
	
	for (new i = 0; i < HAY_B; i++)
	{
		do
		{
			xq = random(HAY_X);
			yq = random(HAY_Y);
			zq = random(HAY_Z);
  		}
		while(hayMatrix[xq][yq][zq] != 0); // search for empty place in hay
		hayMatrix[xq][yq][zq] = 1;
		hayObjects[i] = CreateObject(ID_HAY_OBJECT, xq*(-4), yq*(-4), (zq+1)*3, 0.0, 0.0, random(2)*180, 50);
	}
	Center_x = (HAY_X + 1) * -2;
	Center_y = (HAY_Y + 1) * -2;
	CreateObject(ID_HAY_OBJECT, Center_x, Center_y, HAY_Z*3 + 3, 0, 0, 0,50);
	
}

stock MG_HAY_PreparePlayer(playerid) // prepare player for MG
{
    if(playersInHay[playerid] == -1)
    {
        playersInHay[playerid] = 1;
        //set players virt world
		if(IsPlayerInAnyVehicle(playerid)) RemovePlayerFromVehicle(playerid);
		new rand = random(sizeof(hayRandomSpawn));
    	SetPlayerPos(playerid, hayRandomSpawn[rand][0], hayRandomSpawn[rand][1], hayRandomSpawn[rand][2]);
		ShowPlayerDialog(playerid, MG_JOIN_DIALOG, DIALOG_STYLE_MSGBOX, "Hay Minigame", "Hay Minigame by R@f and ScRaT", "OK", "");
		SetPlayerCameraPos(playerid, -27.894733, 25.938961, 34.944526);
		SetPlayerCameraLookAt(playerid, -25.195003, 22.448459, 32.593387);
    }
    else
    {
        SendClientMessageToAll(COLOR_ERROR, "* Something goes wrong...=(");
    }
}

stock MG_HAY_PlayerLeftMinigame(playerid)
{
	if(playersInHay[playerid] != -1)
    {
		playersInHay[playerid] = -1;
		PlayerTextDrawHide(playerid, hayTextdraw[playerid]);
		PlayerTextDrawDestroy(playerid, hayTextdraw[playerid]);
		hayTextdraw[playerid] = PlayerText:-1;
	}
}

stock MG_HAY_Start()
{
    for (new i=0; i<MAX_PLAYERS; i++)
	{
	    if(playersInHay[i] != -1 && IsPlayerConnected(i))
    	{
            new rand = random(sizeof(hayRandomSpawn));
			SetPlayerPos(i, hayRandomSpawn[rand][0], hayRandomSpawn[rand][1], hayRandomSpawn[rand][2]);
			SetPlayerFacingAngle(i, 135);
			hayTextdraw[i] = CreatePlayerTextDraw(i, 473.333435, 334.148162,"~h~~y~Hay Minigame~n~~r~Level: ~y~0/31 ~n~~r~Time: ~y~00:00:00");
			PlayerTextDrawLetterSize(i, hayTextdraw[i], 0.400000, 1.600000);
			PlayerTextDrawTextSize(i, hayTextdraw[i], 638.000000, 0.000000);
			PlayerTextDrawAlignment(i, hayTextdraw[i], 1);
			PlayerTextDrawColor(i, hayTextdraw[i], -1);
			PlayerTextDrawUseBox(i, hayTextdraw[i], 1);
			PlayerTextDrawBoxColor(i, hayTextdraw[i], 13141);
			PlayerTextDrawSetShadow(i, hayTextdraw[i], 1);
			PlayerTextDrawSetOutline(i, hayTextdraw[i], 0);
			PlayerTextDrawBackgroundColor(i, hayTextdraw[i], 1499227944);
			PlayerTextDrawFont(i, hayTextdraw[i], 2);
			PlayerTextDrawSetProportional(i, hayTextdraw[i], 1);
			PlayerTextDrawSetShadow(i, hayTextdraw[i], 0);
			
			TogglePlayerControllable(i,0);
    	}
	}
	hayCountdownTimer[count] = 5;
	hayCountdownTimer[timer] = SetTimer("MG_HAY_CountDown", 1000, 1);
	
    SetTimer("MG_HAY_TimerMove", 100, 0); // move hays
}

stock MG_HAY_Destroy()
{
	if(gameTimer != -1) 
	{
		KillTimer(gameTimer);
	}
	for (new i = 0; i < HAY_B; i++)
	{
		if(IsValidObject(hayObjects[i]))
		{
			DestroyObject(hayObjects[i]);
		}
	}
	
	for (new i = 0; i < MAX_PLAYERS; i++)
	{
		if(playersInHay[i] != -1 && hayTextdraw[i] != PlayerText:-1)
		{
			PlayerTextDrawHide(i, hayTextdraw[i]);
			PlayerTextDrawDestroy(i, hayTextdraw[i]);
		}
	}
}

public MG_HAY_TimerMove()
{
	new rand;
	new hayId; // inque id of hay object
	new move = -1; // move type (left, right, up, etc)
	new xq, yq, zq; // current number of hay in matrix
	new Float:x2, Float:y2, Float:z2; // current coords of hay
	new timeFinish; // time when hay stops moving
	new Float:moveSpeed; // hay moving speed

	rand = random(HAY_B);
	hayId = hayObjects[rand];
	if(IsObjectMoving(hayId))
	{
		SetTimer("MG_HAY_TimerMove", 200, 0);
		return 1;
	}
	move = random(6);
	GetObjectPos(hayId, x2, y2, z2);
	
	xq = floatround(x2/-4.0);
	yq = floatround(y2/-4.0);
	zq = floatround(z2/3.0)-1;
	if((move == 0) && (xq != 0) && (hayMatrix[xq-1][yq][zq] == 0))
	{
		timeFinish = 4000 - speed_xy * zq;
		moveSpeed = SPEED_FACTOR / float(timeFinish);
		SetTimerEx("FinishTimer", timeFinish, 0, "iiii", rand, xq, yq, zq);
		xq = xq - 1;
		hayMatrix[xq][yq][zq] = 1;
		MoveObject(hayId, x2+4.0, y2, z2, moveSpeed);
	}

	else if((move == 1) && (xq != HAY_X-1) && (hayMatrix[xq+1][yq][zq] == 0))
	{
		timeFinish = 4000 - speed_xy * zq;
		moveSpeed = SPEED_FACTOR / float(timeFinish);
		SetTimerEx("FinishTimer", timeFinish, 0, "iiii", rand, xq, yq, zq);
		xq = xq + 1;
		hayMatrix[xq][yq][zq] = 1;
		MoveObject(hayId, x2-4.0, y2, z2, moveSpeed);
	}

	else if((move == 2) && (yq != 0) && (hayMatrix[xq][yq-1][zq] == 0))
	{
		timeFinish = 4000 - speed_xy * zq;
		moveSpeed = SPEED_FACTOR / float(timeFinish);
		SetTimerEx("FinishTimer", timeFinish, 0, "iiii", rand, xq, yq, zq);
		yq = yq - 1;
		hayMatrix[xq][yq][zq] = 1;
		MoveObject(hayId, x2, y2+4.0, z2, moveSpeed);
	}


	else if((move == 3) && (yq != HAY_Y-1) && (hayMatrix[xq][yq+1][zq] == 0))
	{
		timeFinish = 4000 - speed_xy * zq;
		moveSpeed = SPEED_FACTOR / float(timeFinish);
		SetTimerEx ("FinishTimer", timeFinish, 0, "iiii", rand, xq, yq, zq);
		yq = yq + 1;
		hayMatrix[xq][yq][zq] = 1;
		MoveObject (hayId, x2, y2-4.0, z2, moveSpeed);
	}

	else if((move == 4) && (zq != 0) && (hayMatrix[xq][yq][zq-1] == 0))
	{
		timeFinish = 3000 - speed_xy * zq;
		moveSpeed = SPEED_FACTOR / float(timeFinish);
		SetTimerEx("FinishTimer", timeFinish, 0, "iiii", rand, xq, yq, zq);
		zq = zq - 1;
		hayMatrix[xq][yq][zq] = 1;
		MoveObject(hayId, x2, y2, z2-3.0, moveSpeed);
	}

	else if(((move == 5) || (move == 6)) && (zq != HAY_Z-1) && (hayMatrix[xq][yq][zq+1] == 0))
	{
		timeFinish = 3000 - speed_z * zq;
		moveSpeed = SPEED_FACTOR / float(timeFinish);
		SetTimerEx("FinishTimer", timeFinish, 0, "iiii", rand, xq, yq, zq);
		zq = zq + 1;
		hayMatrix[xq][yq][zq] = 1;
		MoveObject(hayId, x2, y2, z2+3.0, moveSpeed);
	}
	SetTimer("MG_HAY_TimerMove", 200, 0);
	return 1;
}

public MG_HAY_FinishTimer(id, xq, yq, zq) // set empty place in hay
{
	hayMatrix[xq][yq][zq] = 0;
}

public MG_HAY_TDScore()
{
    MG_HAY_updatePlayersLevel();
	new level, string[256], playersRemaining;
	for(new i = 0; i < MAX_PLAYERS; i++) if(playersInHay[i] != -1) playersRemaining++;
	if(playersRemaining == 0) 
	{
		SendClientMessageToAll(COLOR_MG_RES, "* No winners this time in Hay minigame");
		MG_OnCurrentMinigameFinish();
	}
	for (new i=0; i<MAX_PLAYERS; i++)
	{
	    if(playersInHay[i] != -1 && IsPlayerConnected(i))
    	{
			new tH,tM,tS;
			new timeStamp = GetTickCount();
			new totalRaceTime = timeStamp - hayStartTime;
			ConvertTime(var, totalRaceTime, tH, tM, tS);
			level = playerLevel[i];
			format(string,sizeof(string),"~y~Hay Minigame~n~~r~Level: ~y~%d/31 ~n~~r~Time: ~y~%02d:%02d",level,tH,tM,tS);
			PlayerTextDrawSetString(i, hayTextdraw[i], string);
	      	PlayerTextDrawShow(i, hayTextdraw[i]);
   			if(playerLevel[i] == 31)
			{
				format(string, sizeof(string),"* %s finished Hay minigame in %02d min %02d sec", GetPlayerFormattedName(i),tH,tM,tS);
				SendClientMessageToAll(COLOR_MG_RES, string);
				MG_OnCurrentMinigameFinish();
   			}
		}
	}
	return 1;
}

stock MG_HAY_updatePlayersLevel()
{
	new Float:xq, Float:yq, Float:zq;
	for (new i=0; i<MAX_PLAYERS; i++)
	{
	    if(playersInHay[i] != -1 && IsPlayerConnected(i))
    	{
	        GetPlayerPos(i, xq, yq, zq);
			if (xq<=2.0 && xq>=-15.0 && yq<=2.0 && yq>=-15.0)
			{
				new level = (floatround (zq)/3) - 1;
				playerLevel[i] = level;
			}
			else
			{
				playerLevel[i] = 0;
			}
	    }
	}
}

public MG_HAY_CountDown()
{
	if(hayCountdownTimer[count] == 0) 
	{
		KillTimer(hayCountdownTimer[timer]);
		for (new i=0; i<MAX_PLAYERS; i++)
		{
			if(playersInHay[i] != -1 && IsPlayerConnected(i))
			{
				PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
				GameTextForPlayer(i, "GO", 1000, 3);
				TogglePlayerControllable(i,1);
				
				hayStartTime = GetTickCount(); // set game start time
				gameTimer = SetTimer("MG_HAY_TDScore", 1000, 1);
			}
		}
		return 1;
	}

	new string[128], number[8];
	string = "~g~ Starting in ~y~";

	format(number, sizeof(number), "%d", hayCountdownTimer[count]);
	strins(string, number, strlen(string));
	for (new i=0; i<MAX_PLAYERS; i++)
	{
	    if(playersInHay[i] != -1 && IsPlayerConnected(i))
    	{
			PlayerPlaySound(i, 1056, 0.0, 0.0, 0.0);
			GameTextForPlayer(i, string, 1000, 3);
		}
	}

	hayCountdownTimer[count]--;
	return 1;
}
