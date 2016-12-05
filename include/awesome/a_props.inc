#if defined _props_included
	#endinput
#endif
#define _props_included

#include "include/awesome/a_utils.inc"
#include "include/awesome/a_dbutils.inc"
#include "include/awesome/a_config.inc"
#include "include/awesome/colors.inc"

#define MODULE_FUNCTION(%1) forward PROPS_%1; public PROPS_%1

#define PROP_UNBUYABLE_TIME_IN_MIN 5
#define PROP_EARNINGS_INTERVAL_IN_MIN 5 //5 mins

#define SHOW_3D_LABELS TRUE

/* DB fields constants */
#define PROPERTY_PICKUP_ID "ID"
#define PROPERTY_NAME "PropName"
#define PROPERTY_COST "PropCost"
#define PROPERTY_EARNINGS "PropEarnings"
#define PROPERTY_OWNER "PropOwner"
#define PROPERTY_PURCHASE_TIME "PropPurchaseTime"
#define PROPERTY_LOCX "PropLocX"
#define PROPERTY_LOCY "PropLocY"
#define PROPERTY_LOCZ "PropLocZ"

new PlayerText:playerTDs[MAX_PLAYERS];
new playerTDsTimer[MAX_PLAYERS];

/* public functions declaration */
forward PROPS_createProperty(const playerid, propname[], const cost, const earnings);
forward PROPS_buyPropertyForPlayer(const playerid);
forward PROPS_sellProperty(const playerid);

/* public timer callbacks */
forward PROPS_OnPlayerExitProperty(const playerid, const Float:x, const Float:y, const Float:z);
forward PROPS_PlayerEarningsTimer();

MODULE_FUNCTION(OnGameModeInit()) {
	LOG_write("Properties Subsystem initializing");
	//DBUTILS_clearResults(DBUTILS_runNamedQuery("dropPropertiesTable","NA"));
	DBUTILS_clearResults(DBUTILS_runNamedQuery("createPropertiesTable","NA"));
	new DBResult:db_result;
	db_result = DBUTILS_runNamedQuery("findAllProperties","NA");
	if(DBUTILS_getResultSize(db_result) > 0) {
		do {
			new propname[32];
			new Float:x = DBUTILS_getFloat(db_result, PROPERTY_LOCX);
			new Float:y = DBUTILS_getFloat(db_result, PROPERTY_LOCY);
			new Float:z = DBUTILS_getFloat(db_result, PROPERTY_LOCZ);
			DBUTILS_getString(db_result, PROPERTY_NAME, propname, sizeof(propname));
			new oldid = DBUTILS_getInt(db_result, "ID");
			
			new id = CreatePickup(1273, 1, x, y, z, -1);
			if (SHOW_3D_LABELS == TRUE) Create3DTextLabel(propname, 0x008080FF, x, y, z+0.7, 40.0, 0, 0);
			DBUTILS_clearResults(DBUTILS_runNamedQuery("updatePropertyId","dd",id,oldid));
			
		} while(DBUTILS_nextItem(db_result));
	}
	DBUTILS_clearResults(db_result);
	
	for(new i = 0; i < MAX_PLAYERS; i++) {
		playerTDs[i] = PlayerText:-1;
		playerTDsTimer[i] = -1;
	}
	SetTimer("PROPS_PlayerEarningsTimer", PROP_EARNINGS_INTERVAL_IN_MIN * 60 * 1000, true);
}

MODULE_FUNCTION(OnPlayerPickUpPickup(playerid, pickupid)) {
	LOG_writeFormatted("Entered onto pickup: %d", pickupid);
	new DBResult:db_result;
	db_result = DBUTILS_runNamedQuery("findPropertyByPickupID","d",pickupid);
	if(DBUTILS_getResultSize(db_result) > 0) {
		new propname[32], propowner[32];
		DBUTILS_getString(db_result, PROPERTY_NAME, propname, sizeof(propname));
		new cost = DBUTILS_getInt(db_result, PROPERTY_COST);
		new earnings = DBUTILS_getInt(db_result, PROPERTY_EARNINGS);
		DBUTILS_getString(db_result, PROPERTY_OWNER, propowner, sizeof(propowner));
		
		new Float:x = DBUTILS_getFloat(db_result, PROPERTY_LOCX);
		new Float:y = DBUTILS_getFloat(db_result, PROPERTY_LOCY);
		new Float:z = DBUTILS_getFloat(db_result, PROPERTY_LOCZ);
		
		PROPS_ShowPropertyInfoToPlayer(playerid, x, y, z, propname, cost, earnings, propowner);
	}
	DBUTILS_clearResults(db_result);
}

public PROPS_createProperty(const playerid, propname[], const cost, const earnings) {
	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	new id = CreatePickup(1273, 1, x, y, z, -1);
	if (SHOW_3D_LABELS == TRUE) Create3DTextLabel(propname, 0x008080FF, x, y, z+0.7, 40.0, 0, 0);
	SendClientMessage(playerid, COLOR_PROPS, "* Property created successfully!");
	DBUTILS_clearResults(DBUTILS_runNamedQuery("createProperty","dsddfff",id,propname,cost,earnings,x,y,z));
}

public PROPS_buyPropertyForPlayer(const playerid) {
	new DBResult:db_result, success = FALSE;
	db_result = DBUTILS_runNamedQuery("findAllProperties","NA");
	if(DBUTILS_getResultSize(db_result) > 0) {
		do {
			new propname[32], propowner[32];
			DBUTILS_getString(db_result, PROPERTY_NAME, propname, sizeof(propname));
			new cost = DBUTILS_getInt(db_result, PROPERTY_COST);
			DBUTILS_getString(db_result, PROPERTY_OWNER, propowner, sizeof(propowner));
			new Float:x = DBUTILS_getFloat(db_result, PROPERTY_LOCX);
			new Float:y = DBUTILS_getFloat(db_result, PROPERTY_LOCY);
			new Float:z = DBUTILS_getFloat(db_result, PROPERTY_LOCZ);
			new propid = DBUTILS_getInt(db_result, PROPERTY_PICKUP_ID);
			
			if(GetPlayerDistanceFromPoint(playerid, x, y, z) <= 0.5) {
				new player_name[MAX_PLAYER_NAME];
				GetPlayerName(playerid, player_name, sizeof(player_name));
				if(strlen(propowner) != 0 && strcmp(player_name, propowner, false) == 0) {
					SendClientMessage(playerid, COLOR_ERROR, "* This property is already yours!");
					return;
				}
				new purchaseTime = DBUTILS_getInt(db_result, PROPERTY_PURCHASE_TIME);
				if((gettime() - purchaseTime) > PROP_UNBUYABLE_TIME_IN_MIN * 60) {
					success = TRUE;
					if(GetPlayerMoney(playerid) >= cost) {
						GivePlayerMoney(playerid, -1 * cost);
						DBUTILS_clearResults(DBUTILS_runNamedQuery("updateProperty","sdd",player_name,gettime(),propid));
						SendClientMessageFormatted(playerid, COLOR_PROPS, "* Property %s successfully purchased!", propname);
						SendClientMessageToAllFormatted(COLOR_PROPS, "* %s purchased property '%s' for %d$", GetPlayerFormattedName(playerid), propname, cost);
					} else {
						SendClientMessage(playerid, COLOR_ERROR, "* You have not enough money to buy this property!");
					}
				} else {
					SendClientMessage(playerid, COLOR_ERROR, "* You are not allowed to buy this prop yet.");
				}
			}
		} while(DBUTILS_nextItem(db_result));
		if (success == FALSE) SendClientMessage(playerid, COLOR_ERROR, "* You must be standing within 5m of property marker to buy it");
	}
	DBUTILS_clearResults(db_result);
}

public PROPS_sellProperty(const playerid) {
	new DBResult:db_result, success = FALSE;
	db_result = DBUTILS_runNamedQuery("findAllProperties","NA");
	if(DBUTILS_getResultSize(db_result) > 0) {
		do {
			new propname[32], propowner[32];
			DBUTILS_getString(db_result, PROPERTY_NAME, propname, sizeof(propname));
			new cost = DBUTILS_getInt(db_result, PROPERTY_COST);
			DBUTILS_getString(db_result, PROPERTY_OWNER, propowner, sizeof(propowner));
			new Float:x = DBUTILS_getFloat(db_result, PROPERTY_LOCX);
			new Float:y = DBUTILS_getFloat(db_result, PROPERTY_LOCY);
			new Float:z = DBUTILS_getFloat(db_result, PROPERTY_LOCZ);
			new propid = DBUTILS_getInt(db_result, PROPERTY_PICKUP_ID);
			
			if(GetPlayerDistanceFromPoint(playerid, x, y, z) <= 0.5) {
				success = TRUE;
				new player_name[MAX_PLAYER_NAME];
				GetPlayerName(playerid, player_name, sizeof(player_name));
				if(strlen(propowner) != 0 && strcmp(player_name, propowner, false) == 0) {
					GivePlayerMoney(playerid, cost);
					DBUTILS_clearResults(DBUTILS_runNamedQuery("updateProperty","sdd","",0,propid));
					SendClientMessageFormatted(playerid, COLOR_PROPS, "* You sold your property %s for %d$!", propname, cost);
					SendClientMessageToAllFormatted(COLOR_PROPS, "* %s sells property '%s' for %d$", GetPlayerFormattedName(playerid), propname, cost);
				} else {
					SendClientMessage(playerid, COLOR_ERROR, "* This property is not yours!");
				}
			}
		} while(DBUTILS_nextItem(db_result));
		if (success == FALSE) SendClientMessage(playerid, COLOR_ERROR, "* You must be standing within 5m of property marker to sell it");
	}
	DBUTILS_clearResults(db_result);
}

public PROPS_OnPlayerExitProperty(const playerid, const Float:x, const Float:y, const Float:z) { 
	if(GetPlayerDistanceFromPoint(playerid, x, y, z) > 1) {
		if(playerTDs[playerid] != PlayerText:-1) {
			PlayerTextDrawDestroy(playerid, playerTDs[playerid]);
			playerTDs[playerid] = PlayerText:-1;
			if(playerTDsTimer[playerid] != -1) {
				KillTimer(playerTDsTimer[playerid]);
				playerTDs[playerid] = PlayerText:-1;
			}
		}
	}
}

public PROPS_PlayerEarningsTimer() {
	new DBResult:db_result;
	db_result = DBUTILS_runNamedQuery("findAllProperties","NA");
	if(DBUTILS_getResultSize(db_result) > 0) {
		new earns[MAX_PLAYERS];
		new earnCount[MAX_PLAYERS];
		FILL_PLAYERS_ARRAY_NULL(earns);
		FILL_PLAYERS_ARRAY_NULL(earnCount);
		do {
			new propowner[32];
			new earnings = DBUTILS_getInt(db_result, PROPERTY_EARNINGS);
			DBUTILS_getString(db_result, PROPERTY_OWNER, propowner, sizeof(propowner));
			new playerid = GetPlayerIdByName(propowner);
			if(playerid != -1) {
				earns[playerid] += earnings;
				earnCount[playerid] ++;
			}
		} while(DBUTILS_nextItem(db_result));
		
		for(FOR_PLAYERS) {
			if(earnCount[i] > 0) {
				SendClientMessageFormatted(i, COLOR_PROPS, "* You earned %d$ from your [%d] properties!", earns[i], earnCount[i]);
			}
		}
	}
	DBUTILS_clearResults(db_result);
}

stock PROPS_ShowPropertyInfoToPlayer(playerid, Float:x, Float:y, Float:z, propname[], cost, earnings, owner[])
{
	if(playerTDs[playerid] == PlayerText:-1)
	{
		new string[256];
		if(strlen(owner) == 0)
		{
			strins(owner, "nobody", 0, 256);
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

MODULE_FUNCTION(OnGameModeExit()) {}
MODULE_FUNCTION(OnPlayerConnect(playerid)) {}
MODULE_FUNCTION(OnPlayerDisconnect(playerid, reason)) {}
MODULE_FUNCTION(OnPlayerRequestClass(playerid, classid)) {}
MODULE_FUNCTION(OnPlayerSpawn(playerid)) {}
MODULE_FUNCTION(OnPlayerCommandText(playerid, cmdtext[])) {}
MODULE_FUNCTION(OnPlayerDeath(playerid, killerid, reason)) {}
MODULE_FUNCTION(OnVehicleSpawn(vehicleid)) {}
MODULE_FUNCTION(OnVehicleDeath(vehicleid, killerid)) {}
MODULE_FUNCTION(OnPlayerText(playerid, text[])) {}
MODULE_FUNCTION(OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)) {}
MODULE_FUNCTION(OnPlayerExitVehicle(playerid, vehicleid)) {}
MODULE_FUNCTION(OnPlayerStateChange(playerid, newstate, oldstate)) {}
MODULE_FUNCTION(OnPlayerEnterCheckpoint(playerid)) {}
MODULE_FUNCTION(OnPlayerLeaveCheckpoint(playerid)) {}
MODULE_FUNCTION(OnPlayerEnterRaceCheckpoint(playerid)) {}
MODULE_FUNCTION(OnPlayerLeaveRaceCheckpoint(playerid)) {}
MODULE_FUNCTION(OnRconCommand(cmd[])) {}
MODULE_FUNCTION(OnPlayerRequestSpawn(playerid)) {}
MODULE_FUNCTION(OnObjectMoved(objectid)) {}
MODULE_FUNCTION(OnPlayerObjectMoved(playerid, objectid)) {}
MODULE_FUNCTION(OnVehicleMod(playerid, vehicleid, componentid)) {}
MODULE_FUNCTION(OnVehiclePaintjob(playerid, vehicleid, paintjobid)) {}
MODULE_FUNCTION(OnVehicleRespray(playerid, vehicleid, color1, color2)) {}
MODULE_FUNCTION(OnPlayerSelectedMenuRow(playerid, row)) {}
MODULE_FUNCTION(OnPlayerExitedMenu(playerid)) {}
MODULE_FUNCTION(OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)) {}
MODULE_FUNCTION(OnPlayerKeyStateChange(playerid, newkeys, oldkeys)) {}
MODULE_FUNCTION(OnRconLoginAttempt(ip[], password[], success)) {}
MODULE_FUNCTION(OnPlayerUpdate(playerid)) {}
MODULE_FUNCTION(OnPlayerStreamIn(playerid, forplayerid)) {}
MODULE_FUNCTION(OnPlayerStreamOut(playerid, forplayerid)) {}
MODULE_FUNCTION(OnVehicleStreamIn(vehicleid, forplayerid)) {}
MODULE_FUNCTION(OnVehicleStreamOut(vehicleid, forplayerid)) {}
MODULE_FUNCTION(OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])) {}
MODULE_FUNCTION(OnPlayerClickPlayer(playerid, clickedplayerid, source)) {}
#undef MODULE_FUNCTION
