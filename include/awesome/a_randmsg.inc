#include <a_samp>

#include "include/awesome/colors.inc"

#define MSG_INTERVAL 60000

forward SendRandMSG();

new RandomMSG[][] =
{
    "* Use /help to view server commands and teleports",
    "* Visit our website: http://snoopdesigns.co.nr/",
    "* Cheats are forbidden on this server and will results a ban"
};

stock RMSG_InitRandomMsg()
{
	SetTimer("SendRandMSG", MSG_INTERVAL, true);
}

public SendRandMSG()
{
    new randMSG = random(sizeof(RandomMSG));
    SendClientMessageToAll(COLOR_RAND_MSG, RandomMSG[randMSG]);
}