#if defined _countdown_included
	#endinput
#endif
#define _countdown_included

#include <a_samp>

#define createCountdown(%1) for(FOR_PLAYERS) {new c_id=findCountdownSlot(); \
	if(%1 == TRUE && IsPlayerConnected(i)){ \
		countdown[c_id][players][countdown[c_id][player_num] = i;
	}} 

enum COUNTDOWN {
	count,
	timerid,
	active,
	players[MAX_PLAYERS],
	player_num
}

new countdown[10][COUNTDOWN];

forward COUNTDOWN_init();
forward COUNTDOWN_tick();

public COUNTDOWN_init() {
    for (new i = 0; i < 10; i++) {
		countdown[i][count] = -1;
		countdown[i][timerid] = -1;
		countdown[i][active] = 0;
		countdown[i][players_num] = 0;
	}
}

public COUNTDOWN_tick(const countdown_id) {
	
}

stock createCountdown111() {
	LOG_write("Countdown create");
}

stock findCountdownSlot() {
	for (new i = 0; i < 10; i++) {
	    if (countdown[i][active] == FALSE) {
			return i;
		}
	}
	return -1;
}