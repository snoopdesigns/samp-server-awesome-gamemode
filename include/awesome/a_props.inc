#if defined _props_included
	#endinput
#endif
#define _props_included

#include "include/awesome/a_utils.inc"

#define SHOW_3D_LABELS FALSE
#define PROP_UNBUYABLE_TIME_IN_MIN 5
#define PROP_EARNINGS_INTERVAL_IN_MIN 5

//TODO add map markers on properties

forward PROPS_OnPlayerExitProperty(playerid, Float:x, Float:y, Float:z);
forward PROPS_PlayerEarningsTimer();

new PlayerText:playerTDs[MAX_PLAYERS];
new playerTDsTimer[MAX_PLAYERS];

new DB:db_handle_props;

public PROPS_OnPlayerExitProperty(playerid, Float:x, Float:y, Float:z)
{
	if(GetPlayerDistanceFromPoint(playerid, x, y, z) > 1)
	{
		if(playerTDs[playerid] != -1)
		{
			PlayerTextDrawDestroy(playerid, playerTDs[playerid]);
			playerTDs[playerid] = -1;
			if(playerTDsTimer[playerid] != -1)
			{
				KillTimer(playerTDsTimer[playerid]);
				playerTDs[playerid] = -1;
			}
		}
	}
}

public PROPS_PlayerEarningsTimer()
{
	static DBResult: db_result;
	db_result = DBUTILS_runSelectQuery(db_handle_reg, "SELECT * FROM `properties`");
	if(DBUTILS_getResultsetSize(db_result) > 0)
	{
		do
		{
			new propowner[128];
			new earnings = strToInt(DBUTILS_getFieldValueFromResultSet(db_result, "PropEarnings"));
			propowner = DBUTILS_getFieldValueFromResultSet(db_result, "PropOwner");
			new playerid = GetPlayerIdByName(propowner);
			if(playerid != -1) 
			{
				GivePlayerMoney(playerid, earnings);
				new msg[128];
				format(msg, sizeof msg, "You earned %d$ from your property!", earnings);
				//TODO summ earnings for player
				SendClientMessage(playerid, TEXT_COLOR_GREEN, msg);
			}
		} while(DBUTILS_nextItem(db_result));
	}
	DBUTILS_runSelectQueryClear(db_result);
}

stock PROPS_initPropsSystem(db_handle)
{
	db_handle_props = db_handle;
	new createTablePropsQuery[512];
	format(createTablePropsQuery, sizeof createTablePropsQuery, "CREATE TABLE IF NOT EXISTS `properties`(%s, %s, %s, %s, %s, %s, %s, %s, %s)",
		"`ID` INTEGER PRIMARY KEY NOT NULL",
		"'PropName' VARCHAR(32) NOT NULL",
		"'PropCost' INTEGER NOT NULL",
		"'PropEarnings' INTEGER NOT NULL",
		"'PropOwner' VARCHAR(32) NOT NULL",
		"'PropPurchaseTime' INTEGER",
		"'PropLocX' VARCHAR(32) NOT NULL",
		"'PropLocY' VARCHAR(32) NOT NULL",
		"'PropLocZ' VARCHAR(32) NOT NULL");
	DBUTILS_runCreateQuery(db_handle_props, createTablePropsQuery);
	
	//fetch all props from DB
	static DBResult: db_result;
	db_result = DBUTILS_runSelectQuery(db_handle_reg, "SELECT * FROM `properties`");
	if(DBUTILS_getResultsetSize(db_result) > 0)
	{
		do
		{
			new propname[128];
			new Float:x = floatstr(DBUTILS_getFieldValueFromResultSet(db_result, "PropLocX"));
			new Float:y = floatstr(DBUTILS_getFieldValueFromResultSet(db_result, "PropLocY"));
			new Float:z = floatstr(DBUTILS_getFieldValueFromResultSet(db_result, "PropLocZ"));
			propname = DBUTILS_getFieldValueFromResultSet(db_result, "PropName");
			new oldid = strToInt(DBUTILS_getFieldValueFromResultSet(db_result, "ID"));
			
			new id = CreatePickup(1273, 1, x, y, z, -1);
			if(SHOW_3D_LABELS) Create3DTextLabel(propname, 0x008080FF, x, y, z+0.7, 40.0, 0, 0);
			static queryUpdate[128];
			format(queryUpdate, sizeof queryUpdate, "UPDATE `properties` SET `ID`=%d WHERE `ID`=%d", id, oldid);
			DBUTILS_runUpdateQuery(db_handle_props, queryUpdate);
		} while(DBUTILS_nextItem(db_result));
	}
	DBUTILS_runSelectQueryClear(db_result);
	
	//fill player td with nulls
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		playerTDs[i] = -1;
		playerTDsTimer[i] = -1;
	}
	SetTimer("PROPS_PlayerEarningsTimer", PROP_EARNINGS_INTERVAL_IN_MIN * 60 * 1000, true);
}

stock PROPS_createProperty(playerid, propname[], cost, earnings)
{
	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	new id = CreatePickup(1273, 1, x, y, z, -1);
	if(SHOW_3D_LABELS) Create3DTextLabel(propname, 0x008080FF, x, y, z+0.7, 40.0, 0, 0);
	SendClientMessage(playerid, TEXT_COLOR_GREEN, "Property created successfully!");
	
	static query[256];
	format(query, sizeof query, "INSERT INTO `properties` (`ID`,`PropName`, `PropCost`, `PropEarnings`, `PropOwner`, `PropLocX`, `PropLocY`, `PropLocZ`) VALUES (%d,'%s',%d,%d,'',%f,%f,%f)", 
		id,
		propname,
		cost,
		earnings,
		x,
		y,
		z);
	DBUTILS_runInsertQuery(db_handle_props, query);
}

stock PROPS_buyPropertyForPlayer(playerid)
{
	static DBResult: db_result;
	db_result = DBUTILS_runSelectQuery(db_handle_reg, "SELECT * FROM `properties`");
	if(DBUTILS_getResultsetSize(db_result) > 0)
	{
		do
		{
			new propname[128], propowner[128];
			new Float:x = floatstr(DBUTILS_getFieldValueFromResultSet(db_result, "PropLocX"));
			new Float:y = floatstr(DBUTILS_getFieldValueFromResultSet(db_result, "PropLocY"));
			new Float:z = floatstr(DBUTILS_getFieldValueFromResultSet(db_result, "PropLocZ"));
			new cost = strToInt(DBUTILS_getFieldValueFromResultSet(db_result, "PropCost"));
			propname = DBUTILS_getFieldValueFromResultSet(db_result, "PropName");
			propowner = DBUTILS_getFieldValueFromResultSet(db_result, "PropOwner");
			new propid = strToInt(DBUTILS_getFieldValueFromResultSet(db_result, "ID"));
			
			if(GetPlayerDistanceFromPoint(playerid, x, y, z) <= 0.5)
			{
				new player_name[MAX_PLAYER_NAME];
				GetPlayerName(playerid, player_name, sizeof(player_name));
				if(strlen(propowner) != 0 && strcmp(player_name, propowner, false) == 0) 
				{
					SendClientMessage(playerid, TEXT_COLOR_GREEN, "This property is already yours!");
					return;
				}
				new purchaseTime = strToInt(DBUTILS_getFieldValueFromResultSet(db_result, "PropPurchaseTime"));
				if((gettime() - purchaseTime) > PROP_UNBUYABLE_TIME_IN_MIN * 60)
				{
					if(GetPlayerMoney(playerid) >= cost)
					{
						GivePlayerMoney(playerid, -1 * cost);
						static queryUpdate[128];
						format(queryUpdate, sizeof queryUpdate, "UPDATE `properties` SET `PropOwner`='%s',`PropPurchaseTime`=%d WHERE `ID`=%d", player_name, gettime(), propid);
						DBUTILS_runUpdateQuery(db_handle_props, queryUpdate);
						new msg[128];
						format(msg, sizeof msg, "Property %s successfully purchased!", propname);
						SendClientMessage(playerid, TEXT_COLOR_GREEN, msg);
					}
					else {
						SendClientMessage(playerid, TEXT_COLOR_GREEN, "You have not enough money to buy this property!");
					}
				}
				else 
				{
					SendClientMessage(playerid, TEXT_COLOR_GREEN, "You are not allowed to buy this prop yet.");
				}
			}
		} while(DBUTILS_nextItem(db_result));
	}
	DBUTILS_runSelectQueryClear(db_result);
}

stock PROPS_OnPlayerPickUpPickup(playerid, pickupid)
{
	static query[256], DBResult: db_result;
	format(query, sizeof query, "SELECT * FROM `properties` WHERE `ID`=%d;", pickupid);
	db_result = DBUTILS_runSelectQuery(db_handle_reg, query);
	if(DBUTILS_getResultsetSize(db_result) > 0)
	{
		new propname[128], propowner[128];
		propname = DBUTILS_getFieldValueFromResultSet(db_result, "PropName");
		new cost = strToInt(DBUTILS_getFieldValueFromResultSet(db_result, "PropCost"));
		new earnings = strToInt(DBUTILS_getFieldValueFromResultSet(db_result, "PropEarnings"));
		propowner = DBUTILS_getFieldValueFromResultSet(db_result, "PropOwner");
		
		new Float:x = floatstr(DBUTILS_getFieldValueFromResultSet(db_result, "PropLocX"));
		new Float:y = floatstr(DBUTILS_getFieldValueFromResultSet(db_result, "PropLocY"));
		new Float:z = floatstr(DBUTILS_getFieldValueFromResultSet(db_result, "PropLocZ"));
		
		PROPS_ShowPropertyInfoToPlayer(playerid, x, y, z, propname, cost, earnings, propowner);
	}
	DBUTILS_runSelectQueryClear(db_result);
	
}

stock PROPS_ShowPropertyInfoToPlayer(playerid, Float:x, Float:y, Float:z, propname[], cost, earnings, owner[])
{
	if(playerTDs[playerid] == -1)
	{
		new string[256];
		if(strlen(owner) == 0)
		{
			strins(owner,"nobody",0);
		}
		format(string, sizeof string, "~y~%s~n~~r~Cost ~w~%d$~n~~r~Earninigs ~w~%d$~n~~r~Owner ~w~%s~n~~n~~y~/buy ~w~to purchase", propname, cost, earnings, owner);
		new PlayerText:td;
		td = CreatePlayerTextDraw(playerid, 17.832714, 270.926025, string);
		PlayerTextDrawLetterSize(playerid, td, 0.400000, 1.600000);
		PlayerTextDrawTextSize(playerid, td, 230.000000, 0.000000);
		PlayerTextDrawAlignment(playerid, td, 1);
		PlayerTextDrawColor(playerid, td, 0xFFFFFFFF);
		PlayerTextDrawUseBox(playerid, td, 1);
		PlayerTextDrawBoxColor(playerid, td, 0x00000077);
		PlayerTextDrawSetShadow(playerid, td, 0);
		PlayerTextDrawSetOutline(playerid, td, 0);
		PlayerTextDrawBackgroundColor(playerid, td, 0);
		PlayerTextDrawFont(playerid, td, 2);
		PlayerTextDrawSetProportional(playerid, td, 1);
		PlayerTextDrawSetShadow(playerid, td, 0);
		PlayerTextDrawShow(playerid, td);
		new timerId = SetTimerEx("PROPS_OnPlayerExitProperty", 500, true, "ifff", playerid, x, y, z); // create timer to check is player within property
		playerTDs[playerid] = td;
		playerTDsTimer[playerid] = timerId;
	}
}