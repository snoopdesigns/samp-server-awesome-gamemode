#if defined _vehutils_included
	#endinput
#endif
#define _vehutils_included

#include "include/awesome/a_utils.inc"
#include "include/awesome/colors.inc"

#define VEH_CONTROL_TIMER_INTERVAL 3000

//player vars
#define PLAYER_VAR_CURVEHICLE "CURVEHICLE"

forward VEH_VehicleControlTimer();
forward VEH_CreateVehicle(playerid, modelid);

public VEH_VehicleControlTimer()
{
	for(new i = 0; i<MAX_PLAYERS; i++)
    {
		if(IsPlayerConnected(i))
		{
			new Float:health;
			new veh = GetPlayerVehicleID(i);
			if(veh != 0) 
			{
				GetVehicleHealth(veh, health);
				if(health < 500) 
				{
					SetVehicleHealth(veh, 1000);
				}
			}
		}
    }
}

public VEH_CreateVehicle(playerid, modelid)
{
	new Float:x, Float:y, Float:z, Float:angle;
	GetPlayerFacingAngle(playerid, angle);
	GetPlayerPos(playerid, x, y, z);
	new vehicleid = CreateVehicle(modelid, x, y, z, angle, 0, 0, -1, 0);
	if(vehicleid != MAX_VEHICLES)
	{
		PutPlayerInVehicle(playerid, vehicleid, 0);
		AddVehicleComponent(vehicleid, 1087); //hydralistics
		AddVehicleComponent(vehicleid, 1010); //hitros
	}
	TogglePlayerControllable(playerid,1);
	SetCameraBehindPlayer(playerid);
	SetPVarInt(playerid, PLAYER_VAR_CURVEHICLE, vehicleid);
}

stock VEH_initVehiclesSystem()
{
	//vehicles
	AddStaticVehicle(411,-2653.6113,600.3569,14.1353,179.8569,0,0);
	
	new total_vehicles_from_files = LoadStaticVehiclesFromFile("vehicles/sf_law.txt");
    total_vehicles_from_files += LoadStaticVehiclesFromFile("vehicles/sf_airport.txt");
    total_vehicles_from_files += LoadStaticVehiclesFromFile("vehicles/sf_gen.txt");
	
	SetTimer("VEH_VehicleControlTimer", VEH_CONTROL_TIMER_INTERVAL, true);
} 

stock VEH_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(((newkeys & (KEY_FIRE)) == (KEY_FIRE)) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER )
    {
		if(!VEH_IsPlayerInInvalidNosVehicle(playerid,GetPlayerVehicleID(playerid)))
		{
			AddVehicleComponent(GetPlayerVehicleID(playerid), 1010);
		}
    }
    if((((newkeys & (KEY_FIRE)) != (KEY_FIRE)) && ((oldkeys & (KEY_FIRE)) == (KEY_FIRE))) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER )
    {
		if(!VEH_IsPlayerInInvalidNosVehicle(playerid,GetPlayerVehicleID(playerid)))
		{
			RemoveVehicleComponent(GetPlayerVehicleID(playerid), 1010);
		}
    }
}

stock VEH_RepairPlayerVehicle(playerid)
{
	RepairVehicle(GetPlayerVehicleID(playerid));
	SendClientMessage(playerid, COLOR_GREEN, "* Your vehicle has been repaired!");
}

stock VEH_SpawnPlayerVehicle(playerid, modelid) 
{
	new curveh = GetPVarInt(playerid, PLAYER_VAR_CURVEHICLE);
	new oldVehDestroyed = FALSE;
	if(curveh > 0)
	{
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
	if(oldVehDestroyed)
	{
		GameTextForPlayer(playerid, "Re-Spawning vehicle...", 1000, 5);
	} 
	else 
	{
		GameTextForPlayer(playerid, "Spawning vehicle...", 1000, 5);
	}
	SetTimerEx("VEH_CreateVehicle", 600, false, "ii", playerid, modelid); 
}

stock VEH_IsPlayerInInvalidNosVehicle(playerid,vehicleid)
{
	#define MAX_INVALID_NOS_VEHICLES 29
    new InvalidNosVehicles[MAX_INVALID_NOS_VEHICLES] =
    {
        581,523,462,521,463,522,461,448,468,586,
        509,481,510,472,473,493,595,484,430,453,
        452,446,454,590,569,537,538,570,449
    };
    vehicleid = GetPlayerVehicleID(playerid);
    if(IsPlayerInVehicle(playerid,vehicleid))
    {
		for(new i = 0; i < MAX_INVALID_NOS_VEHICLES; i++)
        {
			if(GetVehicleModel(vehicleid) == InvalidNosVehicles[i])	
			{
				return true;
			}
		}
    }
    return false;
}

stock LoadStaticVehiclesFromFile(const filename[])
{
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

	while(fread(file_ptr,line,256) > 0)
	{
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
	printf("Loaded %d vehicles from: %s",vehicles_loaded,filename);
	return vehicles_loaded;
}
