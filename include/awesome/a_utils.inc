#if defined _utils_included
	#endinput
#endif
#define _utils_included

#define valstr FIX_valstr

#define SendClientMessageFormatted(%0,%1,%2,%3) format(clientFormattedMessage, sizeof clientFormattedMessage, %2, %3); SendClientMessage(%0, %1, clientFormattedMessage)
#define SendClientMessageToAllFormatted(%0,%1,%2) format(clientFormattedMessage, sizeof clientFormattedMessage, %1, %2); SendClientMessageToAll(%0, clientFormattedMessage)


//0 - var, 1 - unix time, 2 - hours, 3 - minutes, 4 - seconds
#define ConvertTime(%0,%1,%2,%3,%4) \
	new \
	    Float: %0 = floatdiv(%1, 60000) \
	;\
	%2 = floatround(%0, floatround_tozero); \
	%3 = floatround(floatmul(%0 - %2, 60), floatround_tozero); \
	%4 = floatround(floatmul(floatmul(%0 - %2, 60) - %3, 1000), floatround_tozero)

new clientFormattedMessage[128];

stock intToStr(int)
{
	new string[12];
	valstr(string,int);
	return string;
}

stock strToInt(str[])
{
	return strval(str);
}

stock strToFloat(floatstring[])
{
	return floatstr(floatstring);
}

stock FIX_valstr(dest[], value, bool:pack = false)
{
    // format can't handle cellmin properly
    static const cellmin_value[] = !"-2147483648";
 
    if (value == cellmin)
        pack && strpack(dest, cellmin_value, 12) || strunpack(dest, cellmin_value, 12);
    else
        format(dest, 12, "%d", value), pack && strpack(dest, dest, 12);
}

stock Float:GetDistanceBetweenPoints(Float:x1,Float:y1,Float:x2,Float:y2)
{
	return floatsqroot(((x1-x2)*(x1-x2))+((y1-y2)*(y1-y2)));
}

stock GetPlayerIdByName(name[])
{
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if(IsPlayerConnected(i))
		{
			new player_name[MAX_PLAYER_NAME];
			GetPlayerName(i, player_name, sizeof(player_name));
			if(strcmp(player_name, name) == 0) return i;
		}
	}
	return -1;
}

stock strrest(const string[], &index)
{
	new length = strlen(string);
	while ((index < length) && (string[index] <= ' '))
	{
		index++;
	}
	new offset = index;
	new result[128];
	while ((index < length) && ((index - offset) < (sizeof(result) - 1)))
	{
		result[index - offset] = string[index];
		index++;
	}
	result[index - offset] = EOS;
	return result;
}

stock token_by_delim(const string[], return_str[], delim, start_index)
{
	new x=0;
	while(string[start_index] != EOS && string[start_index] != delim) {
	    return_str[x] = string[start_index];
	    x++;
	    start_index++;
	}
	return_str[x] = EOS;
	if(string[start_index] == EOS) start_index = (-1);
	return start_index;
}
