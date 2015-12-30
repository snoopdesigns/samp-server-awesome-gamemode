#if defined _menus_included
	#endinput
#endif
#define _menus_included

#include "include/awesome/a_commands.inc"
#include "include/awesome/colors.inc"

#define PLAYER_MENU_HELP 0
#define PLAYER_MENU_VEHICLES 1

new menus[2][2][256];

stock MENUS_initMenusSystem()
{
	//help menu
	static dialog[256];
	format(dialog, sizeof dialog, "%s\n%s\n%s", 
		"Vehicle commands",
		"Player commands",
		"Properties commands");
	menus[PLAYER_MENU_HELP][0] = "Available commands";
	menus[PLAYER_MENU_HELP][1] = dialog;
	
	//vehicle commands
	format(dialog, sizeof dialog, "%s\n%s", 
		"Spawn Infernus: /car",
		"Repair vehicle: /repair");
	menus[PLAYER_MENU_VEHICLES][0] = "Vehicle commands";
	menus[PLAYER_MENU_VEHICLES][1] = dialog;
}

stock MENUS_showPlayerMenu(playerid, menuid)
{
	ShowPlayerDialog(playerid, menuid, DIALOG_STYLE_LIST, menus[menuid][0], menus[menuid][1], "Select", "Cancel");
}

stock MENUS_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
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
						SendClientMessage(playerid, COLOR_ERROR, "Sorry, nothing here :(");
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
						dcmd_repair(playerid, "");
					}
				}
		}
	}
}