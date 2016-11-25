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
	{"OnPlayerDisconnect","dd"}
};

new modules[][modules_item] = {
	{"DB","Database Layer Module"},
	{"CMDS","Command Module"},
	{"MENUS","Ingame Menus Module"},
	{"MG","Minigames Module"},
	{"PROPS","Properties Subsystem Module"},
	{"REG","Registration and Player stats Module"},
	{"RACE","Race Module"},
	{"RMSG","Random Messages and Announcments Module"},
	{"VEH","Vehicle Subsystem Module"},
	{"CS","Player Classes Module"}
};

public callModulesCallback(const callback[],{Float,_}:...) {
	for (new i = 0; i < sizeof(modules); i++) {
		new functionName[64];
		format(functionName, sizeof functionName, "%s_%s", modules[i][short_name], callback);
		new callback_format_j = 0;
		for(new j = 0; j < sizeof(callbacks); j++) {
			if (strcmp(callback, callbacks[j][callback_name]) == 0) {
				callback_format_j = j;
			}
		}
		LOG_writeFormatted("Calling function: %s, format: %s, numargs: %d", functionName, callbacks[callback_format_j][callback_format], numargs()-1);
		switch (numargs()) {
			case 1: {
				CallRemoteFunction(functionName, callbacks[callback_format_j][callback_format]);
			}
			case 2: {
				CallRemoteFunction(functionName, callbacks[callback_format_j][callback_format], getarg(1));
			}
			case 3: {
				CallRemoteFunction(functionName, callbacks[callback_format_j][callback_format], getarg(1), getarg(2));
			}
		}
	}
}