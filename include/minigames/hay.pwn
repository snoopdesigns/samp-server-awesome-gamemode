#include <a_samp>

#define ConvertTime(%0,%1,%2,%3,%4) \
	new \
	    Float: %0 = floatdiv(%1, 60000) \
	;\
	%2 = floatround(%0, floatround_tozero); \
	%3 = floatround(floatmul(%0 - %2, 60), floatround_tozero); \
	%4 = floatround(floatmul(floatmul(%0 - %2, 60) - %3, 1000), floatround_tozero)

#define ORANGE 0xDB881AAA
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

new hayMatrix[HAY_X][HAY_Y][HAY_Z]; // if element is 0, then object does not exists, otherwise exists
new hayObjects[HAY_B]; // array of hay objects

new playersInHay[MAX_PLAYERS];
new playerLevel[MAX_PLAYERS];

new speed_xy; // moving speed across x-y axis
new speed_z; // moving speed across z axis
new Center_x;
new Center_y; //WTF?

new hayStartTime;

public OnFilterScriptInit()
{
	MG_HAY_Init();
	MG_HAY_PreparePlayer(0);
	MG_HAY_Start();
	MG_HAY_Destroy();
	MG_HAY_TimerMove();
	MG_HAY_FinishTimer(0,0,0,0);
	return 1;
}

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
    	SetPlayerPos(playerid, 0, 6.5, 3.2);
		SetPlayerFacingAngle(playerid, 135);
    }
    else
    {
        SendClientMessageToAll(ORANGE, "Something goes wrong...=(");
    }
}

stock MG_HAY_Start()
{
    for (new i=0; i<MAX_PLAYERS; i++)
	{
	    if(playersInHay[i] != -1 && IsPlayerConnected(i))
    	{
            SetPlayerPos(i, 0, 6.5, 3.2);
			SetPlayerFacingAngle(i, 135);
    	}
	}
	hayStartTime = GetTickCount(); // set game start time
    SetTimer ("MG_HAY_TimerMove", 100, 0); // move hays
	SetTimer ("MG_HAY_TDScore", 1000, 1);
}

stock MG_HAY_Destroy()
{
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
		SetTimer("TimerMove", 200, 0);
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
	SetTimer("TimerMove", 200, 0);
	return 1;
}

public MG_HAY_FinishTimer(id, xq, yq, zq) // set empty place in hay
{
	hayMatrix[xq][yq][zq] = 0;
}

public MG_HAY_TDScore()
{
    MG_HAY_updatePlayersLevel();
	new level, string[256], PlayerN[MAX_PLAYER_NAME];
	for (new i=0; i<MAX_PLAYERS; i++)
	{
	    if(playersInHay[i] != -1 && IsPlayerConnected(i))
    	{
			new tH,tM,tS;
			new timeStamp = GetTickCount();
			new totalRaceTime = timeStamp - hayStartTime;
			ConvertTime(var, totalRaceTime, tH, tM, tS);
			level = playerLevel[i];
			format(string,sizeof(string),"Hay Minigame~n~Level: %d/31 ~n~Time: %02d:%02d",level,tH,tM,tS);
			GameTextForPlayer(i, string, 1000, 2);
   			if(playerLevel[i] == 31)
			{
				GetPlayerName(i, PlayerN, sizeof(PlayerN));
				format(string, sizeof(string),"%s Finished The Hay Minigame In %02d Min %02d Sec", PlayerN,tH,tM,tS);
				SendClientMessageToAll(ORANGE, string);
				SetPlayerPos(i,0,0,0);
				SpawnPlayer(i);
				//here we need to end minigame TODO
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