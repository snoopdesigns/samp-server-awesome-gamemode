#if defined _log_included
	#endinput
#endif
#define _log_included

#include <a_samp>
#include "include/awesome/a_utils.inc"

#define LOG_writeFormatted(%1,%2) format(logFormatttedMsg, sizeof logFormatttedMsg, %1, %2); LOG_write(logFormatttedMsg)
new logFormatttedMsg[128];

#define LOG_FILENAME "awesome-gamemode.log"

stock LOG_write(const message[]) {
	new File:handle = fopen(LOG_FILENAME, io_append);
	if(handle)
	{
		new logWithDatetime[1024];
		new datetime[32];
		get_datetime_string(sizeof datetime, datetime);
		format(logWithDatetime, sizeof logWithDatetime, "[%s] * %s", datetime, message);
		fwrite(handle, logWithDatetime);
		fwrite(handle, "\r\n");
		fclose(handle);
	}
	else
	{
		print("Failed to create log file awesome-gamemode.log");
		return;
	}
}