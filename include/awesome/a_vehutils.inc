#if defined _vehutils_included
	#endinput
#endif
#define _vehutils_included

#include "include/awesome/a_utils.inc"
#include "include/awesome/a_log.inc"
#include "include/awesome/colors.inc"

#define MODULE_FUNCTION(%1) forward VEH_%1; public VEH_%1

#define VEH_CONTROL_TIMER_INTERVAL 3000

//player vars
#define PLAYER_VAR_CURVEHICLE "CURVEHICLE"

new VehicleNames[][] = {
    "Landstalker", "Bravura", "Buffalo", "Linerunner", "Perrenial", "Sentinel",
	"Dumper", "Firetruck", "Trashmaster", "Stretch", "Manana", "Infernus",
	"Voodoo", "Pony", "Mule", "Cheetah", "Ambulance", "Leviathan", "Moonbeam",
    "Esperanto", "Taxi", "Washington", "Bobcat", "Whoopee", "BF Injection",
	"Hunter", "Premier", "Enforcer", "Securicar", "Banshee", "Predator", "Bus",
	"Rhino", "Barracks", "Hotknife", "Trailer", "Previon", "Coach", "Cabbie",
	"Stallion", "Rumpo", "RC Bandit", "Romero", "Packer", "Monster", "Admiral",
	"Squalo", "Seasparrow", "Pizzaboy", "Tram", "Trailer", "Turismo", "Speeder",
	"Reefer", "Tropic", "Flatbed", "Yankee", "Caddy", "Solair", "Berkley's RC Van",
	"Skimmer", "PCJ-600", "Faggio", "Freeway", "RC Baron", "RC Raider", "Glendale",
	"Oceanic","Sanchez", "Sparrow", "Patriot", "Quad", "Coastguard", "Dinghy",
	"Hermes", "Sabre", "Rustler", "ZR-350", "Walton", "Regina", "Comet", "BMX",
	"Burrito", "Camper", "Marquis", "Baggage", "Dozer", "Maverick", "News Chopper",
	"Rancher", "FBI Rancher", "Virgo", "Greenwood", "Jetmax", "Hotring", "Sandking",
	"Blista Compact", "Police Maverick", "Boxville", "Benson", "Mesa", "RC Goblin",
	"Hotring Racer A", "Hotring Racer B", "Bloodring Banger", "Rancher", "Super GT",
	"Elegant", "Journey", "Bike", "Mountain Bike", "Beagle", "Cropduster", "Stunt",
 	"Tanker", "Roadtrain", "Nebula", "Majestic", "Buccaneer", "Shamal", "Hydra",
 	"FCR-900", "NRG-500", "HPV1000", "Cement Truck", "Tow Truck", "Fortune",
 	"Cadrona", "FBI Truck", "Willard", "Forklift", "Tractor", "Combine", "Feltzer",
 	"Remington", "Slamvan", "Blade", "Freight", "Streak", "Vortex", "Vincent",
    "Bullet", "Clover", "Sadler", "Firetruck", "Hustler", "Intruder", "Primo",
	"Cargobob", "Tampa", "Sunrise", "Merit", "Utility", "Nevada", "Yosemite",
	"Windsor", "Monster", "Monster", "Uranus", "Jester", "Sultan", "Stratium",
	"Elegy", "Raindance", "RC Tiger", "Flash", "Tahoma", "Savanna", "Bandito",
    "Freight Flat", "Streak Carriage", "Kart", "Mower", "Dune", "Sweeper",
	"Broadway", "Tornado", "AT-400", "DFT-30", "Huntley", "Stafford", "BF-400",
	"News Van", "Tug", "Trailer", "Emperor", "Wayfarer", "Euros", "Hotdog", "Club",
	"Freight Box", "Trailer", "Andromada", "Dodo", "RC Cam", "Launch", "Police Car",
 	"Police Car", "Police Car", "Police Ranger", "Picador", "S.W.A.T", "Alpha",
 	"Phoenix", "Glendale", "Sadler", "Luggage", "Luggage", "Stairs", "Boxville",
 	"Tiller", "Utility Trailer"
};

/* public functions declaration */
forward VEH_RepairPlayerVehicle(const playerid);
forward VEH_SpawnPlayerVehicle(const playerid, const modelid);
forward VEH_getVehicleModel(const playerid);

/* public timer callbacks */
forward VEH_VehicleControlTimer();
forward VEH_CreateVehicle(const playerid, const modelid);

MODULE_FUNCTION(OnGameModeInit()) {
	LOG_write("Vehicles Subsystem initializing");
	//vehicles: infernus at clinic
	AddStaticVehicle(411,-2653.6113,600.3569,14.1353,179.8569,0,0);
	
	new total_vehicles_from_files = LoadStaticVehiclesFromFile("vehicles/sf_law.txt");
    total_vehicles_from_files += LoadStaticVehiclesFromFile("vehicles/sf_airport.txt");
    total_vehicles_from_files += LoadStaticVehiclesFromFile("vehicles/sf_gen.txt");
	
	SetTimer("VEH_VehicleControlTimer", VEH_CONTROL_TIMER_INTERVAL, true);
} 

MODULE_FUNCTION(OnPlayerKeyStateChange(playerid, newkeys, oldkeys)) {
	if(((newkeys & (KEY_FIRE)) == (KEY_FIRE)) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
		if(!VEH_IsPlayerInInvalidNosVehicle(playerid,GetPlayerVehicleID(playerid))) {
			AddVehicleComponent(GetPlayerVehicleID(playerid), 1010);
		}
    }
    if((((newkeys & (KEY_FIRE)) != (KEY_FIRE)) && ((oldkeys & (KEY_FIRE)) == (KEY_FIRE))) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER ) {
		if(!VEH_IsPlayerInInvalidNosVehicle(playerid,GetPlayerVehicleID(playerid))) {
			RemoveVehicleComponent(GetPlayerVehicleID(playerid), 1010);
		}
    }
}

MODULE_FUNCTION(OnPlayerDisconnect(playerid, reason)) {
	new curveh = GetPVarInt(playerid, PLAYER_VAR_CURVEHICLE);
	if (curveh != 0) {
		SetVehicleToRespawn(curveh);
		DestroyVehicle(curveh);
		DeletePVar(playerid, PLAYER_VAR_CURVEHICLE);
	}
}

public VEH_RepairPlayerVehicle(const playerid) {
	RepairVehicle(GetPlayerVehicleID(playerid));
	SendClientMessage(playerid, COLOR_GREEN, "* Your vehicle has been repaired!");
}

public VEH_SpawnPlayerVehicle(const playerid, const modelid) {
	new curveh = GetPVarInt(playerid, PLAYER_VAR_CURVEHICLE);
	new oldVehDestroyed = FALSE;
	if(curveh > 0) {
		//need to remove old player vehicle
		DestroyVehicle(curveh);
		DeletePVar(playerid, PLAYER_VAR_CURVEHICLE);
		oldVehDestroyed = TRUE;
	}
	new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
	SetPlayerCameraPos(playerid, x,y,z+10);
	SetPlayerCameraLookAt(playerid, x,y,z);
	TogglePlayerControllable(playerid,0);
	if(oldVehDestroyed) {
		GameTextForPlayer(playerid, "Re-Spawning vehicle...", 1000, 5);
	} else {
		GameTextForPlayer(playerid, "Spawning vehicle...", 1000, 5);
	}
	SetTimerEx("VEH_CreateVehicle", 600, false, "ii", playerid, modelid); 
}

public VEH_getVehicleModel(const playerid) {
	if (IsPlayerInAnyVehicle(playerid)) {
		new modelname[64], modelid;
		modelid = GetVehicleModel(GetPlayerVehicleID(playerid));
		modelname = VEH_getVehicleNameByModelId(modelid);
		SendClientMessageFormatted(playerid, COLOR_GREEN, "* You are in %s [VehicleModel = %d]", modelname, modelid);
	}
}

/* public timer function for creating player vehicle */
public VEH_CreateVehicle(const playerid, const modelid) {
	new Float:x, Float:y, Float:z, Float:angle;
	GetPlayerFacingAngle(playerid, angle);
	GetPlayerPos(playerid, x, y, z);
	new color = random(256);
	SendClientMessageFormatted(playerid, COLOR_GREEN, "Spawning with color = %d", color);
	new vehicleid = CreateVehicle(modelid, x, y, z, angle, color, 0, -1, 0);
	if(vehicleid != MAX_VEHICLES) {
		PutPlayerInVehicle(playerid, vehicleid, 0);
		AddVehicleComponent(vehicleid, 1087); //hydralistics
		AddVehicleComponent(vehicleid, 1010); //hitros
	}
	TogglePlayerControllable(playerid,1);
	SetCameraBehindPlayer(playerid);
	SetPVarInt(playerid, PLAYER_VAR_CURVEHICLE, vehicleid);
}

//TODO enable disable auto repair
public VEH_VehicleControlTimer() {
	for(FOR_PLAYERS) {
		if(IsPlayerConnected(i)) {
			new Float:health;
			new veh = GetPlayerVehicleID(i);
			if(veh != 0 && GetVehicleModel(veh) != 430/*sumo boat*/) {
				GetVehicleHealth(veh, health);
				if(health < 500) {
					SetVehicleHealth(veh, 1000);
				}
			}
		}
    }
}

stock VEH_IsPlayerInInvalidNosVehicle(const playerid, const vehicleid) {
    new InvalidNosVehicles[29] = {
        581,523,462,521,463,522,461,448,468,586,
        509,481,510,472,473,493,595,484,430,453,
        452,446,454,590,569,537,538,570,449
    };
    if(IsPlayerInVehicle(playerid,vehicleid)) {
		for(new i = 0; i < sizeof(InvalidNosVehicles); i++) {
			if(GetVehicleModel(vehicleid) == InvalidNosVehicles[i])	{
				return true;
			}
		}
    }
    return false;
}

stock VEH_getVehicleNameByModelId(const modelid) {
	new vehicleModelName[64];
	format(vehicleModelName,sizeof(vehicleModelName),"%s",VehicleNames[modelid - 400]);
	return vehicleModelName;
}

stock LoadStaticVehiclesFromFile(const filename[]) {
	new File:file_ptr;
	new line[256];
	new var_from_line[64];
	new vehicletype;
	new Float:SpawnX;
	new Float:SpawnY;
	new Float:SpawnZ;
	new Float:SpawnRot;
    new Color1, Color2;
	new index;
	new vehicles_loaded;

	file_ptr = fopen(filename,filemode:io_read);
	if(!file_ptr) return 0;

	vehicles_loaded = 0;

	while(fread(file_ptr,line,256) > 0) {
	    index = 0;
	    // Read type
  		index = token_by_delim(line,var_from_line,',',index);
  		if(index == (-1)) continue;
  		vehicletype = strval(var_from_line);
   		if(vehicletype < 400 || vehicletype > 611) continue;

  		// Read X, Y, Z, Rotation
  		index = token_by_delim(line,var_from_line,',',index+1);
  		if(index == (-1)) continue;
  		SpawnX = floatstr(var_from_line);

  		index = token_by_delim(line,var_from_line,',',index+1);
  		if(index == (-1)) continue;
  		SpawnY = floatstr(var_from_line);

  		index = token_by_delim(line,var_from_line,',',index+1);
  		if(index == (-1)) continue;
  		SpawnZ = floatstr(var_from_line);

  		index = token_by_delim(line,var_from_line,',',index+1);
  		if(index == (-1)) continue;
  		SpawnRot = floatstr(var_from_line);

  		index = token_by_delim(line,var_from_line,',',index+1);
  		if(index == (-1)) continue;
  		Color1 = strval(var_from_line);

  		index = token_by_delim(line,var_from_line,';',index+1);
  		Color2 = strval(var_from_line);
  		
  		AddStaticVehicleEx(vehicletype,SpawnX,SpawnY,SpawnZ,SpawnRot,Color1,Color2,(30*60));
		vehicles_loaded++;
	}

	fclose(file_ptr);
	LOG_writeFormatted("Loaded %d vehicles from: %s",vehicles_loaded,filename);
	return vehicles_loaded;
}

MODULE_FUNCTION(OnGameModeExit()) {}
MODULE_FUNCTION(OnPlayerConnect(playerid)) {}
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
MODULE_FUNCTION(OnPlayerPickUpPickup(playerid, pickupid)) {}
MODULE_FUNCTION(OnVehicleMod(playerid, vehicleid, componentid)) {}
MODULE_FUNCTION(OnVehiclePaintjob(playerid, vehicleid, paintjobid)) {}
MODULE_FUNCTION(OnVehicleRespray(playerid, vehicleid, color1, color2)) {}
MODULE_FUNCTION(OnPlayerSelectedMenuRow(playerid, row)) {}
MODULE_FUNCTION(OnPlayerExitedMenu(playerid)) {}
MODULE_FUNCTION(OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)) {}
MODULE_FUNCTION(OnRconLoginAttempt(ip[], password[], success)) {}
MODULE_FUNCTION(OnPlayerUpdate(playerid)) {}
MODULE_FUNCTION(OnPlayerStreamIn(playerid, forplayerid)) {}
MODULE_FUNCTION(OnPlayerStreamOut(playerid, forplayerid)) {}
MODULE_FUNCTION(OnVehicleStreamIn(vehicleid, forplayerid)) {}
MODULE_FUNCTION(OnVehicleStreamOut(vehicleid, forplayerid)) {}
MODULE_FUNCTION(OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])) {}
MODULE_FUNCTION(OnPlayerClickPlayer(playerid, clickedplayerid, source)) {}
#undef MODULE_FUNCTION