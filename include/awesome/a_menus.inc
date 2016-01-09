#if defined _menus_included
	#endinput
#endif
#define _menus_included

#include "include/awesome/a_commands.inc"
#include "include/awesome/colors.inc"
#include "include/awesome/a_utils.inc"

#define PLAYER_MENU_HELP 0
#define PLAYER_MENU_VEHICLES 1
#define PLAYER_MENU_PROPS 2

enum playerMenuInfo
{
	playerMenuHeading[128],
	playerMenuText[256],
	playerMenuType
	
}

new menus[3][playerMenuInfo];

stock MENUS_initMenusSystem()
{
	//help menu
	static dialog[256];
	static heading[128];
	format(dialog, sizeof dialog, "%s\n%s\n%s", 
		"Vehicle commands",
		"Player commands",
		"Properties commands");
	heading = "Available commands";
	menus[PLAYER_MENU_HELP][playerMenuHeading] = heading;
	menus[PLAYER_MENU_HELP][playerMenuText] = dialog;
	menus[PLAYER_MENU_HELP][playerMenuType] = DIALOG_STYLE_LIST;
	
	//vehicle commands
	format(dialog, sizeof dialog, "%s\n%s", 
		"Spawn Infernus: /car",
		"Repair vehicle: /fix");
	heading = "Vehicle commands";
	menus[PLAYER_MENU_VEHICLES][playerMenuHeading] = heading;
	menus[PLAYER_MENU_VEHICLES][playerMenuText] = dialog;
	menus[PLAYER_MENU_VEHICLES][playerMenuType] = DIALOG_STYLE_LIST;
	
	//props info
	format(dialog, sizeof dialog, "%s\n%s\n%s\n%s", 
		"This is description of properties system",
		"Available commands:",
		"Type /buy within 5 meters of property marker to buy it",
		"Type /sell within 5 meters of property marker to sell it");
	heading = "Properties";
	menus[PLAYER_MENU_PROPS][playerMenuHeading] = heading;
	menus[PLAYER_MENU_PROPS][playerMenuText] = dialog;
	menus[PLAYER_MENU_PROPS][playerMenuType] = DIALOG_STYLE_MSGBOX;
}

stock MENUS_showPlayerMenu(playerid, menuid)
{
	if(menus[menuid][playerMenuType] == DIALOG_STYLE_LIST)
	{
		ShowPlayerDialog(playerid, menuid, menus[menuid][playerMenuType], menus[menuid][playerMenuHeading], menus[menuid][playerMenuText], "Select", "Cancel");
	}
	else if(menus[menuid][playerMenuType] == DIALOG_STYLE_MSGBOX)
	{
		ShowPlayerDialog(playerid, menuid, menus[menuid][playerMenuType], menus[menuid][playerMenuHeading], menus[menuid][playerMenuText], "OK", "");
	}
}

stock MENUS_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	strlen(inputtext);//just to handle compilte warning
	if(response)
    {
		switch(dialogid) 
		{
			case PLAYER_MENU_HELP:
			{
				switch(listitem)
				{
					case 0:
					{
						MENUS_showPlayerMenu(playerid, PLAYER_MENU_VEHICLES);
					}
					case 1:
					{
						SendClientMessage(playerid, COLOR_ERROR, "Sorry, nothing here :(");
					}
					case 2:
					{
						MENUS_showPlayerMenu(playerid, PLAYER_MENU_PROPS);
					}
				}
			}
			case PLAYER_MENU_VEHICLES:
				switch(listitem)
				{
					case 0:
					{
						dcmd_car(playerid, "");
					}
					case 1:
					{
						dcmd_fix(playerid, "");
					}
				}
		}
	}
}
