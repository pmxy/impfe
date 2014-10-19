
/*
 * $Id: gui.h,v 1.1.1.1 2000/05/17 19:22:13 marisa Exp $
 *
 * Defines for the menu items for the main form
 *
 * $Log: gui.h,v $
 * Revision 1.1.1.1  2000/05/17 19:22:13  marisa
 * First CVS checkin
 *
 * Revision 1.1.1.6  1997/05/03 09:42:46  marisag
 * patch1: Daily checkin
 *
 * Revision 1.1.1.5  1997/04/24 04:47:07  marisag
 * patch1: Many changes
 *
 * Revision 1.1.1.4  1997/04/14 02:52:33  marisag
 * patch1: Many changes
 *
 * Revision 1.1.1.3  1997/04/12 16:52:54  marisag
 * patch1: Menu changes
 *
 * Revision 1.1.1.2  1997/04/10 06:31:40  marisag
 * patch1: Added in defs for the database stuff
 *
 * Revision 1.1.1.1  1997/04/09 03:21:57  marisag
 * patch1: New changes...
 *
 * Revision 1.1  1997/04/08 07:16:20  marisag
 * Initial revision
 *
 *
 */

#define MAIN_FILE_MENU "Open DB|Close DB|Save DB|New DB|Quit"
#define MAIN_FILE_MENU_OPEN	1
#define MAIN_FILE_MENU_CLOSE	2
#define MAIN_FILE_MENU_SAVE	3
#define MAIN_FILE_MENU_NEW	4
#define MAIN_FILE_MENU_EXIT	5

#define MAIN_SERVER_MENU "Open Connection|Close Connection"
#define MAIN_SERVER_MENU_OPEN	1
#define MAIN_SERVER_MENU_CLOSE	2

#define MAIN_VIEW_MENU "Map|Planet|Ship|Scripts|Commands|Clear Display"
#define MAIN_VIEW_MENU_MAP	1
#define MAIN_VIEW_MENU_PLANET	2
#define MAIN_VIEW_MENU_SHIP	3
#define MAIN_VIEW_MENU_SCRIPTS  4
#define MAIN_VIEW_MENU_CMDS	5
#define MAIN_VIEW_MENU_CLEAR	6

#define MAIN_CONFIG_MENU "Host|Settings"
#define MAIN_CONFIG_MENU_HOST	1
#define MAIN_CONFIG_MENU_SETTINGS	2

#define MAIN_HELP_MENU "Index|About"
#define MAIN_HELP_MENU_INDEX	1
#define MAIN_HELP_MENU_ABOUT	2

/* Form Defs */
#ifndef BUILDING_MAIN
extern
#endif
   FD_ImpFeMain *fd_ImpFeMain;
#ifndef BUILDING_MAIN
extern
#endif
   FD_ImpFeAbout *fd_ImpFeAbout;
#ifndef BUILDING_MAIN
extern
#endif
   FD_MapForm *fd_MapForm;
#ifndef BUILDING_MAIN
extern
#endif
   FD_TasksForm *fd_TasksForm;
#ifndef BUILDING_MAIN
extern
#endif
   FD_ServCfgForm *fd_ServCfgForm;
#ifndef BUILDING_MAIN
extern
#endif
   FD_ShipCensusForm *fd_ShipCensusForm;
#ifndef BUILDING_MAIN
extern
#endif
   FD_PlanetCensusForm *fd_PlanetCensusForm;
#ifndef BUILDING_MAIN
extern
#endif
   FD_ShipDetailForm *fd_ShipDetailForm;
#ifndef BUILDING_MAIN
extern
#endif
   FD_PlanetDetailForm *fd_PlanetDetailForm;
#ifndef BUILDING_MAIN
extern
#endif
   FD_PowerRptForm *fd_PowerRptForm;
#ifndef BUILDING_MAIN
extern
#endif
   FD_MinerCensusForm *fd_MinerCensusForm;
#ifndef BUILDING_MAIN
extern
#endif
   FD_DebugOutputForm *fd_DebugOutputForm;
#ifndef BUILDING_MAIN
extern
#endif
   FD_LocalEditorForm *fd_LocalEditorForm;
#ifndef BUILDING_MAIN
extern
#endif
   FD_HelpForm *fd_HelpForm; /* The Dummy* forms should be renamed as */
#ifndef BUILDING_MAIN
extern
#endif
   FD_CommandForm *fd_CommandForm; /* they are used - this is easier than   */
#ifndef BUILDING_MAIN
extern
#endif
   FD_Dummy3Form *fd_Dummy3Form; /* just adding new ones sine I can do a  */
#ifndef BUILDING_MAIN
extern
#endif
   FD_Dummy4Form *fd_Dummy4Form; /* global search & replace               */
#ifndef BUILDING_MAIN
extern
#endif
   FD_Dummy5Form *fd_Dummy5Form;

