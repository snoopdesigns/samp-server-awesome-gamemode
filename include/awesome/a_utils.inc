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

new argValIndex = 0;
new argValChar;
	
#define str_eq(%1,%2) strcmp(%1,%2)==0
#define str_neq(%1,%2) strcmp(%1,%2)!=0
#define FOR_PLAYERS new i=0;i<MAX_PLAYERS;i++
#define FILL_PLAYERS_ARRAY_NULL(%1) for(new i=0;i<MAX_PLAYERS;i++) %1[i]=0
	
#define EXTRACT_ARRAY_ARG(%1,%2) \
	for (argValIndex = 0; argValIndex < 255; argValIndex++) { \
		argValChar = getarg(%2, argValIndex); \
		%1[argValIndex] = argValChar; \
		if (argValChar == 0) break; \
	} \
	argValIndex=0
	
new clientFormattedMessage[128];

stock rtrim(sSource[])
{
	new
		iEnd,
		iInputLength = strlen(sSource);

	for(iEnd = (iInputLength - 1); iEnd > 0; --iEnd)
	{
		switch(sSource[iEnd])
		{
			case ' ', '\t', '\r', '\n':
			{
				continue;
			}
			default:
			{
				break;
			}
		}
	}

	strdel(sSource, (iEnd + 1), iInputLength);
}

stock copyString(const source[], dest[], destlen) {
	format(dest, destlen, "%s", source);
}

stock replaceFirst(const toSearch[], const toReplace[], src[], search_len, src_len) {
	new beginIndex = strfind(src, toSearch);
	strdel(src, beginIndex, beginIndex + search_len);
	strins(src, toReplace, beginIndex, src_len);
}

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

stock floatToStr(const Float:val, result[], res_size) {
	format(result, res_size, "%.2f", val);
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

stock GetPlayerFormattedName(playerid)
{
	new playerMsg[MAX_PLAYER_NAME];
	if(IsPlayerConnected(playerid))
	{
		new player_name[MAX_PLAYER_NAME];
		GetPlayerName(playerid, player_name, sizeof(player_name));
		format(playerMsg, sizeof playerMsg, "%s(ID:%d)", player_name, playerid);
	}
	return playerMsg;
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

stock get_datetime_string(const size, result[]) {
	new y,m,d,h,mi,s; 
	getdate(y,m,d);
	gettime(h,mi,s);
	new y_str[5];
	new m_str[3];
	new d_str[3];
	new h_str[3];
	new mi_str[3];
	new s_str[3];
	convert_int_to_2char_str(y, sizeof y_str, y_str);
	convert_int_to_2char_str(m, sizeof m_str, m_str);
	convert_int_to_2char_str(d, sizeof d_str, d_str);
	convert_int_to_2char_str(h, sizeof h_str, h_str);
	convert_int_to_2char_str(mi, sizeof mi_str, mi_str);
	convert_int_to_2char_str(s, sizeof s_str, s_str);
	format(result, size, "%s.%s.%s %s:%s:%s", y_str, m_str, d_str, h_str, mi_str, s_str);
}

stock convert_int_to_2char_str(const i, const size, result[]) {
	if (i < 10) {
	    format(result, size, "0%d", i);
	} else {
		format(result, size, "%d", i);
	}
}
