#ifndef LINT
static char rcsid[]="$Id: feconfig.c,v 1.4 2000/05/26 23:38:55 marisa Exp $";
#endif
/*
 * $Id: feconfig.c,v 1.4 2000/05/26 23:38:55 marisa Exp $
 *
 * $Log: feconfig.c,v $
 * Revision 1.4  2000/05/26 23:38:55  marisa
 * No longer add "change compr" for new databases
 *
 * Revision 1.3  2000/05/26 17:29:12  marisa
 * Now activates menu options properly when creating a new config
 *
 * Revision 1.2  2000/05/24 21:51:38  marisa
 * Fix bugs with TRUE/FALSE testing
 *
 * Revision 1.1.1.1  2000/05/17 19:22:13  marisa
 * First CVS checkin
 *
 * Revision 1.1.1.5  1997/05/03 09:42:51  marisag
 * patch1: Daily checkin
 *
 * Revision 1.1.1.4  1997/04/25 06:39:30  marisag
 * patch1: Code for the planet & ship detail forms
 *
 * Revision 1.1.1.3  1997/04/24 04:46:03  marisag
 * patch1: Many changes
 *
 * Revision 1.1.1.2  1997/04/16 03:07:37  marisag
 * patch1: Daily checkin
 *
 * Revision 1.1.1.1  1997/04/12 16:49:06  marisag
 * patch1: Much changes - new fields added, handle saving data on exit...
 *
 * Revision 1.1  1997/04/10 06:28:30  marisag
 * Initial revision
 *
 *
 */

#include "config.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#ifdef I_STRING
#include <string.h>
#else
#include <strings.h>
#endif
#include "forms.h"
#include "impfe.h"
#include "ImpFeGui.h"
#include "ImpFeMess.h"
#include "gui.h"
#include "version.h"

static char ConfigBuf[2048];
static char PickedFile[256];
static char KeyWord[80];
static char KeyValue[1025];

static FILE *CfgHndl=NULL;
static BOOL CfgParseErr=FALSE;

/*
 * strdecode - Like strcpy(), except _ is converted to spaces
 */
void strdecode(char *dest, char *src)
{
	while (*src != '\0')
	{
	    if (*src == '_')
	    {
			dest[0] = ' ';
	    }
	    else
	    {
			dest[0] = *src;
	    }
	    dest++;
	    src++;
	}
	dest[0]='\0'; /* Be safe */
}

/*
 * strencode - Like strcpy(), except spaces/tabs are converted to "_"
 */
void strencode(char *dest, char *src)
{
	while (*src != '\0')
	{
	    if ((*src == ' ') || (*src == '	'))
	    {
			dest[0] = '_';
	    }
	    else
	    {
			dest[0] = *src;
	    }
	    dest++;
	    src++;
	}
	dest[0]='\0'; /* Be safe */
}

/*
 * handleLine - parses the current KeyWord and KeyValue pair
 */

void handleLine(void)
{
	int tmpVal, tmpVer;
	unsigned long tmpLong;

	/* This is kind of yucky, but C doesn't have string-based
	 * switch() statements..
	 */
	if (strcmp(KeyWord, KEYWORD_GAME_VERSION) == 0)
	{
	    tmpVal=atoi(KeyValue);
	    tmpVer=atoi(FE_CFG_VER);
	    if (tmpVal < tmpVer)
	    {
			fe_puts("*** Configuration file is for an older ImpFE - Attempting to convert it\n");
	    }
	    return;
	}
	if (strcmp(KeyWord, KEYWORD_GAME_HOST) == 0)
	{
	    strcpy(game_host, KeyValue);
	    return;
	}
	if (strcmp(KeyWord, KEYWORD_GAME_PORT) == 0)
	{
	    strcpy(game_port, KeyValue);
	    return;
	}
	if (strcmp(KeyWord, KEYWORD_GAME_PLAYER) == 0)
	{
	    strdecode(game_player, KeyValue);
	    return;
	}
	if (strcmp(KeyWord, KEYWORD_GAME_PLAYER_PSWD) == 0)
	{
	    strdecode(game_player_pswd, KeyValue);
	    return;
	}
	if (strcmp(KeyWord, KEYWORD_GAME_DESC) == 0)
	{
	    strdecode(game_desc, KeyValue);
	    return;
	}
	if (strcmp(KeyWord, KEYWORD_GAME_SIZEX) == 0)
	{
	    tmpVal=atoi(KeyValue);
	    if ((tmpVal < 10) || (tmpVal > 255))
	    {
			fe_puts("Invalid setting for " KEYWORD_GAME_SIZEX " in config file\n");
			CfgParseErr=TRUE;
	    }
	    else
	    {
			game_sizex=(unsigned short int) tmpVal;
	    }
	    return;
	}
	if (strcmp(KeyWord, KEYWORD_GAME_SIZEY) == 0)
	{
	    tmpVal=atoi(KeyValue);
	    if ((tmpVal < 10) || (tmpVal > 255))
	    {
			fe_puts("Invalid setting for " KEYWORD_GAME_SIZEY " in config file\n");
			CfgParseErr=TRUE;
	    }
	    else
	    {
			game_sizey=(unsigned short int) tmpVal;
	    }
	    return;
	}
	if (strcmp(KeyWord, KEYWORD_HIGH_SHIP) == 0)
	{
	    if(sscanf(KeyValue, "%lu", &tmpLong) != 1)
	    {
			fe_puts("Invalid setting for " KEYWORD_HIGH_SHIP " in config file\n");
			CfgParseErr=TRUE;
	    }
	    else
	    {
			next_ship=tmpLong;
	    }
	    return;
	}
	if (strcmp(KeyWord, KEYWORD_HIGH_PLANET) == 0)
	{
	    if(sscanf(KeyValue, "%lu", &tmpLong) != 1)
	    {
			fe_puts("Invalid setting for " KEYWORD_HIGH_PLANET " in config file\n");
			CfgParseErr=TRUE;
	    }
	    else
	    {
			next_planet=tmpLong;
	    }
	    return;
	}
	if (strcmp(KeyWord, KEYWORD_HIGH_ITEM) == 0)
	{
	    if(sscanf(KeyValue, "%lu", &tmpLong) != 1)
	    {
			fe_puts("Invalid setting for " KEYWORD_HIGH_ITEM " in config file\n");
			CfgParseErr=TRUE;
	    }
	    else
	    {
			next_item=tmpLong;
	    }
	    return;
	}
	if (strcmp(KeyWord, KEYWORD_HOME_PLAN) == 0)
	{
	    if(sscanf(KeyValue, "%lu", &tmpLong) != 1)
	    {
			fe_puts("Invalid setting for " KEYWORD_HOME_PLAN " in config file\n");
			CfgParseErr=TRUE;
	    }
	    else
	    {
			home_planet=tmpLong;
	    }
	    return;
	}
	if (strcmp(KeyWord, KEYWORD_PLAYER_NUM) == 0)
	{
	    if(sscanf(KeyValue, "%u", &tmpVal) != 1)
	    {
			fe_puts("Invalid setting for " KEYWORD_PLAYER_NUM " in config file\n");
			CfgParseErr=TRUE;
	    }
	    else
	    {
			player_num=(unsigned short)tmpVal;
	    }
	    return;
	}
	CfgParseErr=TRUE;
	fl_addto_browser_chars(fd_ImpFeMain->MainBrowser, "Unknown config file keyword '");
	fl_addto_browser_chars(fd_ImpFeMain->MainBrowser, KeyWord);
	fl_addto_browser_chars(fd_ImpFeMain->MainBrowser, "'\n");
}

/*
 * parseConfig - reads in a configuration file and returns TRUE if
 *               no config errors, else returns FALSE
 */
BOOL parseConfig(void)
{
	/* Loop for each line in the file */
	while (fgets(ConfigBuf, 2047, CfgHndl) != NULL)
	{
	    /* Check for the comment character */
	    if (ConfigBuf[0] == '#')
	    {
			continue;
	    }
	    if (sscanf(ConfigBuf, "%s = %s", KeyWord, KeyValue) == 2)
	    {
			handleLine();
	    }
	    else
	    {
			if (!CfgParseErr)
			{
				CfgParseErr=TRUE;
				fe_puts("*** Invalid configuration line found");
			}
	    }
	}
	if (CfgParseErr)
	{
		return(FALSE);
	}
	return(TRUE);
}

/*
 * handleOpenCfg - Handles the user selecting the "file->open" menu item
 */
void handleOpenCfg(void)
{
	const char *selFile; /* File name they selected */
	FePlanet_t tmpPlanet;
    int res;

	fl_set_fselector_title("Open ImpFE Database");
	if ((selFile = fl_show_fselector("Select a file to open", "", "*.fe",
									 DEF_CFG_NAME)) == NULL)
	{
		/* They pressed cancel... */
		return;
	}
	/* Set the directory */
	res = chdir(fl_get_directory());
	/* Copy out the file name */
	strncpy(PickedFile, selFile, 255);
	/* Try to open the config file */
	if ((CfgHndl=fopen(PickedFile, "r")) == NULL)
	{
	    /* Open failed */
	    fl_addto_browser_chars(fd_ImpFeMain->MainBrowser, "*** Open of config file ");
	    fl_addto_browser_chars(fd_ImpFeMain->MainBrowser, PickedFile);
	    fl_addto_browser_chars(fd_ImpFeMain->MainBrowser, " for reading failed\n");
	    return;
	}
	if (parseConfig())
	{
	    if (openFeDBs(FALSE))
	    {
			fl_set_menu_item_mode(fd_ImpFeMain->MainFileMenu,
								  MAIN_FILE_MENU_OPEN, FL_PUP_GREY);
			fl_set_menu_item_mode(fd_ImpFeMain->MainFileMenu,
								  MAIN_FILE_MENU_NEW, FL_PUP_GREY);
			fl_set_menu_item_mode(fd_ImpFeMain->MainFileMenu,
								  MAIN_FILE_MENU_SAVE, FL_PUP_NONE);
			fl_set_menu_item_mode(fd_ImpFeMain->MainFileMenu,
								  MAIN_FILE_MENU_CLOSE, FL_PUP_NONE);
			/* Server menu options */
			fl_set_menu_item_mode(fd_ImpFeMain->MainServerMenu,
								  MAIN_SERVER_MENU_OPEN, FL_PUP_NONE);
			/* View menu options */
			fl_set_menu_item_mode(fd_ImpFeMain->MainViewMenu,
								  MAIN_VIEW_MENU_MAP, FL_PUP_NONE);
			fl_set_menu_item_mode(fd_ImpFeMain->MainViewMenu,
								  MAIN_VIEW_MENU_PLANET, FL_PUP_NONE);
			fl_set_menu_item_mode(fd_ImpFeMain->MainViewMenu,
								  MAIN_VIEW_MENU_SHIP, FL_PUP_NONE);
			fl_set_menu_item_mode(fd_ImpFeMain->MainViewMenu,
								  MAIN_VIEW_MENU_SCRIPTS, FL_PUP_NONE);
			fl_set_menu_item_mode(fd_ImpFeMain->MainViewMenu,
								  MAIN_VIEW_MENU_CMDS, FL_PUP_NONE);
			/* Configmenu options */
			fl_set_menu_item_mode(fd_ImpFeMain->MainConfigMenu,
								  MAIN_CONFIG_MENU_HOST, FL_PUP_NONE);

			loadCmdQueue();
			fe_puts("*** Configuration file loaded\n");
			if (player_num == 0)
			{
				addCmd("race\n", QUE_IMMEDIATE);
				addCmd("player\n", QUE_IMMEDIATE);
			}
			if (home_planet == 0)
			{
				addCmd("status\n", QUE_IMMEDIATE);
			}
			else
			{
				(void) readPlanet(&tmpPlanet, home_planet);
				LastX = tmpPlanet.pl_col;
				HomeCol = tmpPlanet.pl_col;
				LastY = tmpPlanet.pl_row;
				HomeRow = tmpPlanet.pl_row;
				if (LastX > 5)
				{
					LastX -= 5;
				}
				if (LastY > 3)
				{
					LastY -= 3;
				}
			}
			if ((game_sizex == 10) || (game_sizey == 10))
			{
				addCmd("info\n", QUE_IMMEDIATE);
			}
			if (next_ship == 0)
			{
				addCmd("ship status\n", QUE_HIGH);
				addCmd("ship big\n", QUE_HIGH);
			}
	    }
	}
	fclose(CfgHndl);
	CfgHndl=NULL;
}

/*
 * saveConfig - writes the configuration to the open config file
 */
void saveConfig(void)
{
	/* Write the header */
	fputs("#\n"
		  "# ImpFE v" FE_VER "." FE_REV "." FE_PATCH " configuration file\n"
		  "#\n"
		  "# This file generated by ImpFE - If you edit this file by hand\n"
		  "# you MUST know what you are doing, and all comments will\n"
		  "# disappear the next time you select File->Save !!!\n#\n", CfgHndl);
	/* Write out the configuration lines */
	/* Note we always force this to the current version! */
	fprintf(CfgHndl, "%s = %s\n", KEYWORD_GAME_VERSION, FE_CFG_VER);
	fprintf(CfgHndl, "%s = %s\n", KEYWORD_GAME_HOST, game_host);
	fprintf(CfgHndl, "%s = %s\n", KEYWORD_GAME_PORT, game_port);
	strencode(ConfigBuf, game_player);
	fprintf(CfgHndl, "%s = %s\n", KEYWORD_GAME_PLAYER, ConfigBuf);
	strencode(ConfigBuf, game_player_pswd);
	fprintf(CfgHndl, "%s = %s\n", KEYWORD_GAME_PLAYER_PSWD, ConfigBuf);
	strencode(ConfigBuf, game_desc);
	fprintf(CfgHndl, "%s = %s\n", KEYWORD_GAME_DESC, ConfigBuf);
	fprintf(CfgHndl, "%s = %u\n", KEYWORD_GAME_SIZEX, game_sizex);
	fprintf(CfgHndl, "%s = %u\n", KEYWORD_GAME_SIZEY, game_sizey);
	fprintf(CfgHndl, "%s = %lu\n", KEYWORD_HIGH_SHIP, next_ship);
	fprintf(CfgHndl, "%s = %lu\n", KEYWORD_HIGH_PLANET, next_planet);
	fprintf(CfgHndl, "%s = %lu\n", KEYWORD_HIGH_ITEM, next_item);
	fprintf(CfgHndl, "%s = %lu\n", KEYWORD_HOME_PLAN, home_planet);
	fprintf(CfgHndl, "%s = %u\n", KEYWORD_PLAYER_NUM, player_num);
}

/*
 * handleSaveCfg - Handles the user selecting the "file->save" menu item
 */
void handleSaveCfg(void)
{
	const char *selFile; /* File name they selected */
    int res;

	/* Try to see if they have been doing anything */
	if (PickedFile[0] == '\0')
	{
	    return;
	}
	/* Set the directory */
	res = chdir(fl_get_directory());
	/* Try to open the config file */
	if ((CfgHndl=fopen(PickedFile, "w")) == NULL)
	{
	    /* Open failed */
	    fl_addto_browser_chars(fd_ImpFeMain->MainBrowser, "*** Open of config file ");
	    fl_addto_browser_chars(fd_ImpFeMain->MainBrowser, PickedFile);
	    fl_addto_browser_chars(fd_ImpFeMain->MainBrowser, " for writing failed\n");
	    return;
	}
	saveConfig();
	fclose(CfgHndl);
	CfgHndl=NULL;
	flushCache();
}

/*
 * handleNewCfg - Handles the user selecting the "file->new" menu item
 */
void handleNewCfg(void)
{
	const char *selFile; /* File name they selected */
    int res;

	fl_set_fselector_title("Create a NEW ImpFE Database");
	if ((selFile = fl_show_fselector("Enter a file name to create. End it in .fe", "", "*.fe", "default.fe")) == NULL)
	{
		/* They pressed cancel... */
		return;
	}
	/* Set the directory */
	res = chdir(fl_get_directory());
	/* Copy out the file name */
	strncpy(PickedFile, selFile, 255);
	/* Try to open the config file */
	if ((CfgHndl=fopen(PickedFile, "w")) == NULL)
	{
	    /* Open failed */
	    fl_addto_browser_chars(fd_ImpFeMain->MainBrowser, "*** Open of config file ");
	    fl_addto_browser_chars(fd_ImpFeMain->MainBrowser, PickedFile);
	    fl_addto_browser_chars(fd_ImpFeMain->MainBrowser, " for writing failed\n");
	    return;
	}
	/* Note we do not care if there were any file contents in here */
	setDefaults();
	saveConfig();
	fclose(CfgHndl);
	CfgHndl=NULL;
	/* Attempt to create & open the database files */
	if (openFeDBs(TRUE))
	{
		fl_set_menu_item_mode(fd_ImpFeMain->MainFileMenu,
							  MAIN_FILE_MENU_OPEN, FL_PUP_GREY);
		fl_set_menu_item_mode(fd_ImpFeMain->MainFileMenu,
							  MAIN_FILE_MENU_NEW, FL_PUP_GREY);
		fl_set_menu_item_mode(fd_ImpFeMain->MainFileMenu,
							  MAIN_FILE_MENU_SAVE, FL_PUP_NONE);
		fl_set_menu_item_mode(fd_ImpFeMain->MainFileMenu,
							  MAIN_FILE_MENU_CLOSE, FL_PUP_NONE);
		/* Server menu options */
		fl_set_menu_item_mode(fd_ImpFeMain->MainServerMenu,
							  MAIN_SERVER_MENU_OPEN, FL_PUP_NONE);
		/* View menu options */
		fl_set_menu_item_mode(fd_ImpFeMain->MainViewMenu,
							  MAIN_VIEW_MENU_MAP, FL_PUP_NONE);
		fl_set_menu_item_mode(fd_ImpFeMain->MainViewMenu,
							  MAIN_VIEW_MENU_PLANET, FL_PUP_NONE);
		fl_set_menu_item_mode(fd_ImpFeMain->MainViewMenu,
							  MAIN_VIEW_MENU_SHIP, FL_PUP_NONE);
		fl_set_menu_item_mode(fd_ImpFeMain->MainViewMenu,
							  MAIN_VIEW_MENU_SCRIPTS, FL_PUP_NONE);
		fl_set_menu_item_mode(fd_ImpFeMain->MainViewMenu,
							  MAIN_VIEW_MENU_CMDS, FL_PUP_NONE);
		/* Configmenu options */
		fl_set_menu_item_mode(fd_ImpFeMain->MainConfigMenu,
							  MAIN_CONFIG_MENU_HOST, FL_PUP_NONE);

		addCmd("info\n", QUE_IMMEDIATE);
		addCmd("race\n", QUE_IMMEDIATE);
		addCmd("player\n", QUE_IMMEDIATE);
		addCmd("status\n", QUE_IMMEDIATE);
		addCmd("ship status\n", QUE_HIGH);
		addCmd("ship big\n", QUE_HIGH);
	}
}

/*
 * handleCloseCfg - Handles the user selecting the "file->close" menu item
 */
void handleCloseCfg(void)
{
	saveCmdQueue();
	closeFeDBs();
	fl_set_menu_item_mode(fd_ImpFeMain->MainFileMenu,
						  MAIN_FILE_MENU_OPEN, FL_PUP_NONE);
	fl_set_menu_item_mode(fd_ImpFeMain->MainFileMenu,
						  MAIN_FILE_MENU_NEW, FL_PUP_NONE);
	fl_set_menu_item_mode(fd_ImpFeMain->MainFileMenu,
						  MAIN_FILE_MENU_SAVE, FL_PUP_GREY);
	fl_set_menu_item_mode(fd_ImpFeMain->MainFileMenu,
						  MAIN_FILE_MENU_CLOSE, FL_PUP_GREY);
	/* Server menu options */
	fl_set_menu_item_mode(fd_ImpFeMain->MainServerMenu,
						  MAIN_SERVER_MENU_OPEN, FL_PUP_GREY);
	fl_set_menu_item_mode(fd_ImpFeMain->MainServerMenu,
						  MAIN_SERVER_MENU_CLOSE, FL_PUP_GREY);
	/* View menu options */
	fl_set_menu_item_mode(fd_ImpFeMain->MainViewMenu,
						  MAIN_VIEW_MENU_MAP, FL_PUP_GREY);
	fl_set_menu_item_mode(fd_ImpFeMain->MainViewMenu,
						  MAIN_VIEW_MENU_PLANET, FL_PUP_GREY);
	fl_set_menu_item_mode(fd_ImpFeMain->MainViewMenu,
						  MAIN_VIEW_MENU_SHIP, FL_PUP_GREY);
	fl_set_menu_item_mode(fd_ImpFeMain->MainViewMenu,
						  MAIN_VIEW_MENU_SCRIPTS, FL_PUP_GREY);
	fl_set_menu_item_mode(fd_ImpFeMain->MainViewMenu,
						  MAIN_VIEW_MENU_CMDS, FL_PUP_GREY);
	/* Config menu */
	fl_set_menu_item_mode(fd_ImpFeMain->MainConfigMenu,
						  MAIN_CONFIG_MENU_HOST, FL_PUP_GREY);
	fl_set_menu_item_mode(fd_ImpFeMain->MainConfigMenu,
						  MAIN_CONFIG_MENU_SETTINGS, FL_PUP_GREY);
	if (fl_form_is_visible(fd_MapForm->MapForm))
	{
		fl_hide_form(fd_MapForm->MapForm);
	}
}

