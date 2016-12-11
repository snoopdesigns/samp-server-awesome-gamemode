#if defined _callbacks_included
	#endinput
#endif
#define _callbacks_included

#include <a_samp>
#include "include/awesome/a_utils.inc"

forward callModulesCallback(const callback[],{Float,_}:...);

enum callback_format_enum {
    callback_name[32],
	callback_format[16]
}

enum modules_item {
	short_name[16],
	long_name[64]
}

new callbacks[][callback_format_enum] = {
	{"OnGameModeInit",""},
	{"OnGameModeExit",""},
	{"OnPlayerConnect","d"},
	{"OnPlayerDisconnect","dd"},
	{"OnPlayerCommandText","ds"},
	{"OnPlayerRequestClass","dd"},
	{"OnPlayerSpawn","d"},
	{"OnPlayerKeyStateChange","ddd"},
	{"OnPlayerPickUpPickup","dd"},
	{"OnPlayerEnterRaceCheckpoint","d"},
	{"OnPlayerDeath","ddd"},
	{"OnDialogResponse","dddds"}
};

new modules[][modules_item] = {
	{"DB","Database Layer Module"},
	{"REG","Registration and Player stats Module"},
	{"CMDS","Command Module"},
	{"CS","Player Classes Module"},
	{"VEH","Vehicle Subsystem Module"},
	{"PROPS","Properties Subsystem Module"},
	{"RACE","Race Module"},
	{"CD","Countdown Implementation"},
	{"CFG","Configuration Module"},
	{"RMSG","Random Messages and Announcments Module"},
	{"DIALOG","Ingame Dialog Module"}
	/*{"MG","Minigames Module"}*/
};

public callModulesCallback(const callback[],...) {
	for (new i = 0; i < sizeof(modules); i++) {
		new functionName[64];
		format(functionName, sizeof functionName, "%s_%s", modules[i][short_name], callback);
		if (strlen(functionName) > 31) {
			strdel(functionName, 31, strlen(functionName));
		}
		new callback_format_j = 0;
		for(new j = 0; j < sizeof(callbacks); j++) {
			if (strcmp(callback, callbacks[j][callback_name]) == 0) {
				callback_format_j = j;
			}
		}
		switch (numargs()) {
			case 1: {
				CallRemoteFunction(functionName, callbacks[callback_format_j][callback_format]);
			}
			case 2: {
				new arg1Val[32];
				EXTRACT_ARRAY_ARG(arg1Val, 1);
				CallRemoteFunction(functionName, callbacks[callback_format_j][callback_format], arg1Val);
			}
			case 3: {
				new arg1Val[32],arg2Val[32];
				EXTRACT_ARRAY_ARG(arg1Val, 1);
				EXTRACT_ARRAY_ARG(arg2Val, 2);
				CallRemoteFunction(functionName, callbacks[callback_format_j][callback_format], arg1Val, arg2Val);
			}
			case 4: {
				new arg1Val[32],arg2Val[32],arg3Val[32];
				EXTRACT_ARRAY_ARG(arg1Val, 1);
				EXTRACT_ARRAY_ARG(arg2Val, 2);
				EXTRACT_ARRAY_ARG(arg3Val, 3);
				CallRemoteFunction(functionName, callbacks[callback_format_j][callback_format], arg1Val, arg2Val, arg3Val);
			}
			case 5: {
				new arg1Val[32],arg2Val[32],arg3Val[32],arg4Val[32];
				EXTRACT_ARRAY_ARG(arg1Val, 1);
				EXTRACT_ARRAY_ARG(arg2Val, 2);
				EXTRACT_ARRAY_ARG(arg3Val, 3);
				EXTRACT_ARRAY_ARG(arg4Val, 4);
				CallRemoteFunction(functionName, callbacks[callback_format_j][callback_format], arg1Val, arg2Val, arg3Val, arg4Val);
			}
			case 6: {
				new arg1Val[32],arg2Val[32],arg3Val[32],arg4Val[32],arg5Val[32];
				EXTRACT_ARRAY_ARG(arg1Val, 1);
				EXTRACT_ARRAY_ARG(arg2Val, 2);
				EXTRACT_ARRAY_ARG(arg3Val, 3);
				EXTRACT_ARRAY_ARG(arg4Val, 4);
				EXTRACT_ARRAY_ARG(arg5Val, 5);
				if (strlen(arg5Val)) strins(arg5Val, "a", 0); // temp fix for OnPlayerDialog
				CallRemoteFunction(functionName, callbacks[callback_format_j][callback_format], arg1Val, arg2Val, arg3Val, arg4Val, arg5Val);
			}
		}
	}
}