#ifndef LINT
/*@unused@*/static char rcsid[] = "$Id: gui_cb.c,v 1.2 2000/05/24 21:51:38 marisa Exp $";
#endif

/*
 * $Id: gui_cb.c,v 1.2 2000/05/24 21:51:38 marisa Exp $
 *
 * Handles the callbacks for ImpFe
 *
 * $Log: gui_cb.c,v $
 * Revision 1.2  2000/05/24 21:51:38  marisa
 * Fix bugs with TRUE/FALSE testing
 *
 * Revision 1.1.1.1  2000/05/17 19:22:13  marisa
 * First CVS checkin
 *
 * Revision 1.1.1.10  1997/05/03 09:42:35  marisag
 * patch1: Daily checkin
 *
 * Revision 1.1.1.9  1997/04/27 23:23:30  marisag
 * patch1: Daily checkin
 *
 * Revision 1.1.1.8  1997/04/27 16:32:21  marisag
 * patch1: Fix bug with browsers
 *
 * Revision 1.1.1.7  1997/04/25 06:39:10  marisag
 * patch1: Code for the planet & ship detail forms
 *
 * Revision 1.1.1.6  1997/04/24 04:46:06  marisag
 * patch1: Many changes
 *
 * Revision 1.1.1.5  1997/04/16 03:07:02  marisag
 * patch1: New callbacks
 *
 * Revision 1.1.1.4  1997/04/14 02:51:25  marisag
 * patch1: New functions
 *
 * Revision 1.1.1.3  1997/04/12 16:50:14  marisag
 * patch1: Much changes - handle saving data on exit, not asking for file
 * patch1: name when saving already opened data, clearing vars
 * patch1: when making new config...
 *
 * Revision 1.1.1.2  1997/04/10 06:28:48  marisag
 * patch1: Makes use of the new config & database functions.
 *
 * Revision 1.1.1.1  1997/04/09 03:21:52  marisag
 * patch1: New changes...
 *
 * Revision 1.1  1997/04/08 07:07:36  marisag
 * Initial revision
 *
 *
 */

#include "config.h"
#include "forms.h"
#include "ImpFeGui.h"
#include "gui.h"
#include "impfe.h"
#include "version.h"

static char OutBuf[2048];

/*
 * close_form - generic call-back for closing a form
 */
int close_form(/*@unused@*/FL_FORM *form, /*@unused@*/void *unused)
{
	/*fl_hide_form(form);*/
	return(FL_OK);
}

/* callbacks for form ImpFeMain */
void MainInputBox_cb(FL_OBJECT *ob, long data)
{
	/* If flag is 0 (not from HdnBtn) just return */
	if (data == 0)
	{
		return;
	}
	/* Copy out what they typed */
	strcpy(OutBuf, fl_get_input(ob));
	/* Clear the input box */
	fl_set_input(ob, "");
	/* Send it on to the server */
	if (GameSocket != -1)
	{
		(void) write_data(GameSocket, OutBuf, (int) strlen(OutBuf));
		(void) write_data(GameSocket, "\n", 1);
	}
	fl_addto_browser_chars(fd_ImpFeMain->MainBrowser, OutBuf);
	fl_addto_browser_chars(fd_ImpFeMain->MainBrowser, "\n");
}

void HdnRtn_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
    /* Just pass this on... */
    MainInputBox_cb(fd_ImpFeMain->MainInputBox, 1);
}

void MainHelpMenu_cb(FL_OBJECT *ob, /*@unused@*/long data)
{
	static int item;

	item = fl_get_menu(ob);
	switch(item)
	{
	    case MAIN_HELP_MENU_ABOUT:
			fl_set_object_label(fd_ImpFeAbout->ImpVer, "ImpFE v" FE_VER "." FE_REV "." FE_PATCH);
			fl_set_object_label(fd_ImpFeAbout->MainAboutText,
								"(C) 1997 Marisa Giancarla\n\n"
								"For more information contact <marisa@andromedia.com>\n"
								"or go to the Imperium home page at:\n"
								"<http://www.ZetaCom.com/imperium/>");
			(void) fl_set_form_atclose(fd_ImpFeAbout->ImpFeAbout, close_form,
								&item);
			(void) fl_show_form(fd_ImpFeAbout->ImpFeAbout, FL_PLACE_SIZE,
						 FL_TRANSIENT, "About ImpFE");
			break;
	    case MAIN_HELP_MENU_INDEX:
			cb_help_button(ob, CHAPTER_START);
			break;
	    default:
			break;
	}
}

void PrivMsgBtn_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
  /* fill-in code for callback */
}

void StatusBox_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
  /* fill-in code for callback */
}

void MainFileMenu_cb(FL_OBJECT *ob, /*@unused@*/long data)
{
	int item;

	item = fl_get_menu(ob);
	switch(item)
	{
	    case MAIN_FILE_MENU_OPEN:
			handleOpenCfg();
			break;
	    case MAIN_FILE_MENU_SAVE:
			handleSaveCfg();
			break;
	    case MAIN_FILE_MENU_CLOSE:
			handleSaveCfg();
			handleCloseCfg();
			break;
	    case MAIN_FILE_MENU_NEW:
			handleNewCfg();
			break;
	    case MAIN_FILE_MENU_EXIT:
			(void) cmd_connect(TRUE);
			handleSaveCfg();
			handleCloseCfg();
			freeCmdQueues();
			impfeCleanup();
			fl_finish();
			exit(0);
			/*@notreached@*/ break; /* Never get here */
	    default:
			break;
	}
}

void MainServerMenu_cb(FL_OBJECT *ob, /*@unused@*/long data)
{
	int item;

	item = fl_get_menu(ob);
	switch(item)
	{
	    case MAIN_SERVER_MENU_OPEN:
			if (cmd_connect(FALSE))
			{
				fl_set_menu_item_mode(ob,
									  MAIN_SERVER_MENU_OPEN, FL_PUP_GREY);
				fl_set_menu_item_mode(ob,
									  MAIN_SERVER_MENU_CLOSE, FL_PUP_NONE);
				fl_set_object_label(fd_ImpFeMain->StatusBox, "CONNECTED");
			}
			break;
	    case MAIN_SERVER_MENU_CLOSE:
			if (cmd_connect(TRUE))
			{
				fl_set_menu_item_mode(ob,
									  MAIN_SERVER_MENU_CLOSE, FL_PUP_GREY);
				fl_set_menu_item_mode(ob,
									  MAIN_SERVER_MENU_OPEN, FL_PUP_NONE);
				fl_set_object_label(fd_ImpFeMain->StatusBox, "Offline");
			}
			break;
	    default:
			break;
	}
}

void PrivMsgWho_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
  /* fill-in code for callback */
}

void MainViewMenu_cb(FL_OBJECT *ob, /*@unused@*/long data)
{
	static int item;

	item = fl_get_menu(ob);
	switch(item)
	{
	    case MAIN_VIEW_MENU_MAP:
			(void) fl_set_form_atclose(fd_MapForm->MapForm, close_form,
								&item);
			buildMapDisp();
			(void) fl_show_form(fd_MapForm->MapForm, FL_PLACE_SIZE,
				FL_FULLBORDER, "ImpFE Map Display");
			break;
		case MAIN_VIEW_MENU_SHIP:
			(void) fl_set_form_atclose(fd_ShipCensusForm->ShipCensusForm, close_form,
								&item);
			buildShCensus();
			(void) fl_show_form(fd_ShipCensusForm->ShipCensusForm, FL_PLACE_SIZE,
				FL_FULLBORDER, "Ship Census");
			break;
		case MAIN_VIEW_MENU_PLANET:
			(void) fl_set_form_atclose(fd_PlanetCensusForm->PlanetCensusForm, close_form,
								&item);
			buildPlCensus();
			(void) fl_show_form(fd_PlanetCensusForm->PlanetCensusForm, FL_PLACE_SIZE,
				FL_FULLBORDER, "Planet Census");
			break;
		case MAIN_VIEW_MENU_CMDS:
			(void) fl_set_form_atclose(fd_CommandForm->CommandForm, close_form,
								&item);
			buildCmdList();
			(void) fl_show_form(fd_CommandForm->CommandForm, FL_PLACE_SIZE,
				FL_FULLBORDER, "Queued Commands");
			break;
	    case MAIN_VIEW_MENU_CLEAR:
			fl_clear_browser(fd_ImpFeMain->MainBrowser);
			break;
	    default:
			break;
	}
}

void ServCfgCancelBtn_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	fl_activate_form(fd_ImpFeMain->ImpFeMain);
	fl_hide_form(fd_ServCfgForm->ServCfgForm);
}

int close_cfg_form(/*@unused@*/FL_FORM *form, /*@unused@*/void *unused)
{
	fl_activate_form(fd_ImpFeMain->ImpFeMain);
	/*fl_hide_form(form);*/
	return(FL_OK);
}


void MainConfigMenu_cb(FL_OBJECT *ob, /*@unused@*/long data)
{
	static int item;

	item = fl_get_menu(ob);
	switch(item)
	{
	    case MAIN_CONFIG_MENU_HOST:
			/* Set up the exit handler */
			(void) fl_set_form_atclose(fd_ServCfgForm->ServCfgForm, close_cfg_form,
								&item);
			/*fl_set_form_atclose(fd_ServCfgForm->ServCfgForm, close_form,
			  &item);*/
			/* Load in current values */
			fl_set_input(fd_ServCfgForm->ServName,
						 game_host);
			fl_set_input(fd_ServCfgForm->ServPort,
						 game_port);
			fl_set_input(fd_ServCfgForm->PlayerName,
						 game_player);
			fl_set_input(fd_ServCfgForm->PlayerPswd,
						 game_player_pswd);
			fl_set_input(fd_ServCfgForm->GameDesc,
						 game_desc);
			(void) fl_show_form(fd_ServCfgForm->ServCfgForm, FL_PLACE_SIZE,
						 FL_FULLBORDER, "ImpFE Host Config");
			fl_deactivate_form(fd_ImpFeMain->ImpFeMain);
			break;
	    default:
			break;
	}
}

/* callbacks for form ImpFeAbout */
void AboutOk_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	fl_hide_form(fd_ImpFeAbout->ImpFeAbout);
}



/* callbacks for form MapForm */
/* This lets the user click on a sector to get additional info */
void MB_cb(/*@unused@*/FL_OBJECT *ob, long data)
{
	static int item;
	ULONG tmp;
	USHORT row, col;
	int button;

	/* data holds the code for the tile they clicked */
	if (data == 0)
	{
		return;
	}
	data--;
	row = (USHORT) data / 10;
	col = (USHORT) data % 10;
#ifdef DEBUG
printf("Row = %u, col = %u\n", row, col);
#endif
	/* See which mouse button they clicked */
	button = (int) fl_mouse_button();
	switch(button)
	{
		case FL_LEFT_MOUSE:
			break;
		case FL_MIDDLE_MOUSE:
#ifdef DEBUG
printf("Middle pressed\n");
#endif
			tmp = getSectPlanet(row + LastY, col + LastX);
			if (tmp != IT_NONE)
			{
#ifdef DEBUG
printf("Selecting planet %u\n", tmp);
#endif
				/* Pull up planet in detailed planet display window */
				(void) fl_set_form_atclose(fd_PlanetDetailForm->PlanetDetailForm, close_form,
									&item);
				LastPlanet=tmp;
				buildPlDetDisp();
				(void) fl_show_form(fd_PlanetDetailForm->PlanetDetailForm,
							 FL_PLACE_SIZE, FL_FULLBORDER,
							 "Detailed Planet Information");
			}
			break;
		case FL_RIGHT_MOUSE:
			break;
		default:
			break;
	}
}

void JmpBtn_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	int row, col;

  /* fill-in code for callback */ /*@ignore@*/
	row=atoi(fl_get_input(fd_MapForm->JmRow));
	col=atoi(fl_get_input(fd_MapForm->JmCol)); /*@end@*/
    if (row > ((game_sizey * 10) - 7))
	{
		row = (game_sizey * 10) - 7;
	}
	else if (row < 0)
	{
		row = 0;
	}
    if (col > (((game_sizex - 1) * 10) - 1))
	{
		col = (((game_sizex - 1) * 10) - 1);
	}
	else if (col < 0)
	{
		col = 0;
	}
	/* If coords are the same, don't bother updating map */
	if (((int) LastX != col) || ((int) LastY != row))
	{
	    LastX=(USHORT) col;
	    LastY=(USHORT) row;
	    buildMapDisp();
	}
}

void DirBtn_cb(/*@unused@*/FL_OBJECT *ob, long data)
{
    int mult=1;

    if (data > 9)
    {
	mult=10;
	data /= 10;
    }
    switch(data)
    {
	case 1:
	    scrollFeMap(-1 * mult, 1 * mult);
	    break;
	case 2:
	    scrollFeMap(0, 1 * mult);
	    break;
	case 3:
	    scrollFeMap(1 * mult, 1 * mult);
	    break;
	case 4:
	    scrollFeMap(-1 * mult, 0);
	    break;
	case 5:
	    LastX=HomeCol;
	    if (LastX > 5)
	    {
		LastX -= 5;
	    }
	    LastY=HomeRow;
	    if (LastY > 3)
	    {
		LastY -= 3;
	    }
	    buildMapDisp();
	    break;
	case 6:
	    scrollFeMap(1 * mult, 0);
	    break;
	case 7:
	    scrollFeMap(-1 * mult, -1 * mult);
	    break;
	case 8:
	    scrollFeMap(0, -1 * mult);
	    break;
	case 9:
	    scrollFeMap(1 * mult, -1 * mult);
	    break;
	default:
	    fprintf(stderr, "*** Unknown directional button value %ld\n",
		data);
	    break;
    }
}

void MapHelpMenu_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
  /* fill-in code for callback */
}

void MapFileMenu_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
  /* fill-in code for callback */
}

void SmMapCanvas_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
  /* fill-in code for callback */
}

void MapOk_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	fl_hide_form(fd_MapForm->MapForm);
}



/* callbacks for form TasksForm */
void TasksFileMenu_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
  /* fill-in code for callback */
}

void TasksHelpMenu_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
  /* fill-in code for callback */
}

void TasksTasksMenu_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
  /* fill-in code for callback */
}

void TasksUpdtBtn_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
  /* fill-in code for callback */
}

void TasksKillBtn_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
  /* fill-in code for callback */
}

void TasksDoneBtn_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
  /* fill-in code for callback */
}



/* callbacks for form ServCfgForm */
void ServName_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
  /* fill-in code for callback */
}

void ServPort_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
  /* fill-in code for callback */
}

void PlayerName_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
  /* fill-in code for callback */
}

void PlayerPswd_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
  /* fill-in code for callback */
}

void GameDesc_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
  /* fill-in code for callback */
}

void ServCfgOkBtn_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	strcpy(game_host, fl_get_input(fd_ServCfgForm->ServName));
	strcpy(game_port, fl_get_input(fd_ServCfgForm->ServPort));
	strcpy(game_player, fl_get_input(fd_ServCfgForm->PlayerName));
	strcpy(game_player_pswd, fl_get_input(fd_ServCfgForm->PlayerPswd));
	strcpy(game_desc, fl_get_input(fd_ServCfgForm->GameDesc));
	fl_activate_form(fd_ImpFeMain->ImpFeMain);
	fl_hide_form(fd_ServCfgForm->ServCfgForm);
}

/*
 * addToInput - adds the string to the input box, if NL is TRUE,
 *              it is the end of line and the line should go to the
 *              server
 */
void addToInput(const char *str, BOOL NL)
{
	char work[1024];

	strcpy(work, fl_get_input(fd_ImpFeMain->MainInputBox));
	strcat(work, str);
	work[256]='\0';
	fl_set_input(fd_ImpFeMain->MainInputBox, work);
	if (NL)
	{
		MainInputBox_cb(fd_ImpFeMain->MainInputBox, 1);
	}
}

/* callbacks for form ShipCensusForm */
/* ShBrowse_cb - called when the user clicks on one of the ships in the
 *               census window
 */
void ShBrowse_cb(FL_OBJECT *ob, /*@unused@*/long data)
{
	static int item;
	ULONG shNum;
	int line, button;
	char lBuff[85];

	/* See which mouse button they clicked */
	button = (int) fl_mouse_button();
	/* Get the line # they clicked on */
	line = fl_get_browser(ob);
	/* Copy out the contents of the line they clicked on */
	strcpy(lBuff, fl_get_browser_line(ob, line));
	lBuff[9]='\0';
	/* Get the ship number */
	shNum=(ULONG) atoi(lBuff);
	/* Put it back in the buffer to chop off leading space */
	sprintf(lBuff, " %lu", shNum);

	switch(button)
	{
		case FL_LEFT_MOUSE:
			/* Add ship number to line */
			addToInput(lBuff, FALSE);
			break;
		case FL_MIDDLE_MOUSE:
			/* Add ship number to line, and send it */
			addToInput(lBuff, TRUE);
			break;
		case FL_RIGHT_MOUSE:
			/* Pull up ship in detailed ship display window */
			(void) fl_set_form_atclose(fd_ShipDetailForm->ShipDetailForm, close_form,
								&item);
			LastShip=shNum;
			buildShDetDisp();
			(void) fl_show_form(fd_ShipDetailForm->ShipDetailForm, FL_PLACE_SIZE,
				FL_FULLBORDER, "Detailed Ship Information");
			break;
		default:
			break;
	}
}

void ShCenUpdate_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	buildShCensus();
}

void ShCenOk_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	fl_hide_form(fd_ShipCensusForm->ShipCensusForm);
}


/* callbacks for form PlanetCensusForm */
/* PlBrowse_cb - called when the user clicks on one of the planets in the
 *               census window
 */
void PlBrowse_cb(FL_OBJECT *ob, /*@unused@*/long data)
{
	static int item;
	ULONG plNum;
	int line, button;
	char lBuff[120];

	/* See which mouse button they clicked */
	button = (int) fl_mouse_button();
	/* Get the line # they clicked on */
	line = fl_get_browser(ob);
	/* Copy out the contents of the line they clicked on */
	strcpy(lBuff, fl_get_browser_line(ob, line));
	lBuff[9]='\0';
	/* Get the planet number */
	plNum=(ULONG) atoi(lBuff);
	/* Put it back in the buffer to chop off leading space */
	sprintf(lBuff, " %lu", plNum);

	switch(button)
	{
		case FL_LEFT_MOUSE:
			/* Add planet number to line */
			addToInput(lBuff, FALSE);
			break;
		case FL_MIDDLE_MOUSE:
			/* Add planet number to line, and send it */
			addToInput(lBuff, TRUE);
			break;
		case FL_RIGHT_MOUSE:
			/* Pull up planet in detailed planet display window */
			(void) fl_set_form_atclose(fd_PlanetDetailForm->PlanetDetailForm, close_form,
								&item);
			LastPlanet=plNum;
			buildPlDetDisp();
			(void) fl_show_form(fd_PlanetDetailForm->PlanetDetailForm, FL_PLACE_SIZE,
				FL_FULLBORDER, "Detailed Planet Information");
			break;
		default:
			break;
	}
}

void PlCenUpdate_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	buildPlCensus();
}

void PlCenOk_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	fl_hide_form(fd_PlanetCensusForm->PlanetCensusForm);
}



/* callbacks for form ShipDetailForm */
void ShDetUpdBtn_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	buildShDetDisp();
}

void ShDetStatBtn_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	char workBuf[256];

	sprintf(workBuf, "ship status %lu\n", LastShip);
	(void) addCmd(workBuf, QUE_IMMEDIATE);
}

void ShDetCargoBtn_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	char workBuf[256];

	sprintf(workBuf, "ship cargo %lu\n", LastShip);
	(void) addCmd(workBuf, QUE_IMMEDIATE);
}

void ShDetBigBtn_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	char workBuf[256];

	sprintf(workBuf, "ship big %lu\n", LastShip);
	(void) addCmd(workBuf, QUE_IMMEDIATE);
}

/*
 * ShDetActionBtn_cb - called when the user presses the install,
 *                     uninstall, or unload action buttons
 */
void ShDetActionBtn_cb(/*@unused@*/FL_OBJECT *ob, long data)
{
	int line;
	ULONG itNum;
	char lBuff[85];

	/* Get the line # they clicked on */
	line = fl_get_browser(fd_ShipDetailForm->ShDetItem);
	/* Make sure they have an item selected */
	if (line == 0)
	{
		/* Nope */
		return;
	}
	/* Copy out the contents of the line they clicked on */
	strcpy(lBuff, fl_get_browser_line(fd_ShipDetailForm->ShDetItem, line));
	lBuff[9]='\0';
	/* Get the item number */
	itNum = (ULONG) atoi(lBuff);
	switch (data)
	{
		case 1:
			sprintf(lBuff, "install %lu %lu\n", LastShip, itNum);
			(void) addCmd(lBuff, QUE_IMMEDIATE);
			break;
		case 2:
			sprintf(lBuff, "remove %lu %lu\n", LastShip, itNum);
			(void) addCmd(lBuff, QUE_IMMEDIATE);
			break;
		case 3:
			sprintf(lBuff, "unload big %lu %lu\n", LastShip, itNum);
			(void) addCmd(lBuff, QUE_IMMEDIATE);
			break;
		default:
			break;
	}
}

/*
 * PlDetActionBtn_cb - called when the user presses the install,
 *                     uninstall, or unload action buttons
 */
void PlDetActionBtn_cb(/*@unused@*/FL_OBJECT *ob, long data)
{
	int line;
	ULONG itNum;
	char lBuff[85];

	/* Get the line # they clicked on */
	line = fl_get_browser(fd_PlanetDetailForm->PlDetItem);
	/* Make sure they have an item selected */
	if (line == 0)
	{
		/* Nope */
		return;
	}
	/* Copy out the contents of the line they clicked on */
	strcpy(lBuff, fl_get_browser_line(fd_PlanetDetailForm->PlDetItem, line));
	lBuff[9]='\0';
	/* Get the item number */
	itNum = (ULONG) atoi(lBuff);
	switch (data)
	{
		case 1:
			sprintf(lBuff, "refurbish item %lu\n", itNum);
			(void) addCmd(lBuff, QUE_IMMEDIATE);
			break;
		default:
			break;
	}
}

void ShDetOk_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	fl_hide_form(fd_ShipDetailForm->ShipDetailForm);
}

/* callbacks for form PlanetDetailForm */
void PlDetUpdBtn_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	buildPlDetDisp();
}

void PlDetDumpBtn_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	char workBuf[256];

	sprintf(workBuf, "dump %lu\n", LastPlanet);
	(void) addCmd(workBuf, QUE_IMMEDIATE);
}

void PlDetProdBtn_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	char workBuf[256];

	sprintf(workBuf, "census product %lu\n", LastPlanet);
	(void) addCmd(workBuf, QUE_IMMEDIATE);
}

void PlDetGeoBtn_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	char workBuf[256];

	sprintf(workBuf, "census geolog %lu\n", LastPlanet);
	(void) addCmd(workBuf, QUE_IMMEDIATE);
}

void PlDetPopBtn_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	char workBuf[256];

	sprintf(workBuf, "census population %lu\n", LastPlanet);
	(void) addCmd(workBuf, QUE_IMMEDIATE);
}

void PlDetBigBtn_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	char workBuf[256];

	sprintf(workBuf, "census big %lu\n", LastPlanet);
	(void) addCmd(workBuf, QUE_IMMEDIATE);
}

/*
 * NameUpdate_cb - updates the database with the name requested
 */
void NameUpdate_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	char workBuf[256], plName[PL_NAME_LEN];
	FePlanet_t tmpPlanet;

	/* Read the planet in */
	(void) readPlanet(&tmpPlanet, LastPlanet);
	/* Copy out the value of the field */
	strncpy(plName, fl_get_input(fd_PlanetDetailForm->Name), (PL_NAME_LEN - 1) * sizeof(char));
	plName[PL_NAME_LEN]='\0';
	/* Put the name into the planet struct */
	strcpy(tmpPlanet.name, plName);
	/* Write it back to the database */
	(void) writePlanet(&tmpPlanet, LastPlanet);
	/* If we don't own the planet, no point in sending command to host */
	if ((USHORT) tmpPlanet.owner != player_num)
	{
		return;
	}
	/* Add this command to the queue */
	sprintf(workBuf, "name %lup\n%s\n", LastPlanet, plName);
	(void) addCmd(workBuf, QUE_IMMEDIATE);
}

/*
 * CheckpointUpdate_cb - updates the database with the checkpoint requested
 */
void CheckpointUpdate_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	char workBuf[256], plCode[PL_PSWD_LEN];
	FePlanet_t tmpPlanet;

	/* Read the planet in */
	(void) readPlanet(&tmpPlanet, LastPlanet);
	/* Copy out the value of the field */
	strncpy(plCode, fl_get_input(fd_PlanetDetailForm->Checkpoint), (PL_PSWD_LEN - 1) * sizeof(char));
	plCode[PL_PSWD_LEN]='\0';
	/* Put the name into the planet struct */
	strcpy(tmpPlanet.chkpoint, plCode);
	/* Write it back to the database */
	(void) writePlanet(&tmpPlanet, LastPlanet);
	/* If we don't own the planet, no point in sending command to host */
	if ((USHORT) tmpPlanet.owner != player_num)
	{
		return;
	}
	return; /* Seems to be broken for now XXX */
	/* Add these command to the queue */
	/* XXX Note this will break if they aren't done sequentially! */
	/*@notreached@*/sprintf(workBuf, "checkpoint %lu\n%s\n%s\n", LastPlanet, game_player_pswd,
			plCode);
	/*@notreached@*/(void) addCmd(workBuf, QUE_IMMEDIATE);
}

void PlDetOk_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	fl_hide_form(fd_PlanetDetailForm->PlanetDetailForm);
}



/* callbacks for form PowerRptForm */


/* callbacks for form MinerCensusForm */


/* callbacks for form DebugOutputForm */


/* callbacks for form LocalEditorForm */


/* callbacks for form HelpForm */


/* callbacks for form Dummy3Form */


/* callbacks for form Dummy4Form */


/* callbacks for form Dummy5Form */

/* callbacks used by many forms */

/*
 * Does nothing
 */
void dummy_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
  /* Yawn... */
}

/* Callbacks for form CommandForm */
void CmdsHelpMenu_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
  /* fill-in code for callback */
}

void CmdsDoneBtn_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	fl_hide_form(fd_CommandForm->CommandForm);
}

void CmdsPauseBtn_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	if (StopQueue)
	{
		StopQueue=FALSE;
		fl_set_object_lcol(fd_ImpFeMain->QueueDisp, FL_BLACK);
		fl_set_object_color(fd_ImpFeMain->QueueDisp, FL_WHEAT, FL_MCOL);
	}
	else
	{
		StopQueue=TRUE;
		fl_set_object_lcol(fd_ImpFeMain->QueueDisp, FL_RED);
		fl_set_object_color(fd_ImpFeMain->QueueDisp, FL_BLACK, FL_MCOL);
	}
}

void CmdsKillBtn_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	int tmp;
	char line[95];

	/* Try and get selected line */
	tmp=fl_get_browser(fd_CommandForm->CmdsBrowser);
	/* Make sure they have a line selected */
	if (tmp == 0)
	{
		return;
	}
	/* Copy out the line contents */
	strcpy(line, fl_get_browser_line(fd_CommandForm->CmdsBrowser, tmp));
	tmp = atoi(line);
	(void) cancelCmd(tmp);
}

/*
 * CmdsBrowser_cb - called when they click on a line in the commands
 *                  browser
 */
void CmdsBrowser_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	int tmp, prior, len, pos;
	char line[95], cmnd[259];

	/* Try and get selected line */
	tmp=fl_get_browser(fd_CommandForm->CmdsBrowser);
	/* Make sure they have a line selected */
	if (tmp == 0)
	{
		return;
	}
	/* Copy out the line contents */
	strcpy(line, fl_get_browser_line(fd_CommandForm->CmdsBrowser, tmp));
	tmp = atoi(line);
	/* Speed up output */
	fl_freeze_form(fd_CommandForm->CommandForm);
	strcpy(cmnd, getCmd(tmp));
	prior = getPrior(tmp);
	len=(int) strlen(cmnd);
	/* Convert the NL characters */
	for (pos=0; pos < len; pos++)
	{
		if (cmnd[pos] == '\n')
		{
			cmnd[pos]='~';
		}
	}
	fl_set_input(fd_CommandForm->CmdsCmd, cmnd);
	fl_set_choice(fd_CommandForm->CmdsPriorBtn, prior);
	fl_unfreeze_form(fd_CommandForm->CommandForm);
}

void CmdsAddBtn_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	int len, pos;
	char cmnd[259];

	strcpy(cmnd, fl_get_input(fd_CommandForm->CmdsCmd));
	len=(int) strlen(cmnd);
	/* Convert the NL characters */
	for (pos=0; pos < len; pos++)
	{
		if (cmnd[pos] == '~')
		{
			cmnd[pos]='\n';
		}
	}
	(void) addCmd(cmnd, fl_get_choice(fd_CommandForm->CmdsPriorBtn));
}

void CmdsModBtn_cb(/*@unused@*/FL_OBJECT *ob, /*@unused@*/long data)
{
	int tmp;
	char line[95];

	/* Try and get selected line */
	tmp=fl_get_browser(fd_CommandForm->CmdsBrowser);
	/* Make sure they have a line selected */
	if (tmp == 0)
	{
		return;
	}
	/* Copy out the line contents */
	strcpy(line, fl_get_browser_line(fd_CommandForm->CmdsBrowser, tmp));
	tmp = atoi(line);
	(void) modCmd(tmp, fl_get_input(fd_CommandForm->CmdsCmd),
		   fl_get_choice(fd_CommandForm->CmdsPriorBtn));
}
