#ifndef LINT
static char rcsid[]="$Id: impcon.c,v 1.2 2000/05/24 21:51:39 marisa Exp $";
#endif
/*************************************************************
 *  $Id: impcon.c,v 1.2 2000/05/24 21:51:39 marisa Exp $
 *************************************************************/

#include "config.h"
#include <sys/types.h>
#include <rpc/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <sys/errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#ifdef I_STRING
#include <string.h>
#else
#include <strings.h>
#endif
#include <ctype.h>
#include "forms.h"
#include "impfe.h"
#include "ImpFeGui.h"
#include "gui.h"

static unsigned int MyCond=FL_READ;
static BOOL DoingLogin=FALSE;
static BOOL LoseNextChar=FALSE;

/* These get set from our config files
char game_host[]="localhost";
int game_port=DEF_PORT;
char game_player[]="unknown";
char game_player_pswd[]="unknown";
char game_desc[]="unknown";
int game_time=0;
int game_btus=0; */

/*
 * call_cocket - Attempts to connect to a remote socket
 *               returns the socket number if succeeds, or -1 if fails
 */
int call_socket(char *hostname, int portnum)
{
	struct sockaddr_in sa;
	struct hostent *hp;
	int a, s;

	if ((hp=gethostbyname(hostname)) == NULL)
	{
		errno=ECONNREFUSED;
		return(-1);
	}

	bzero(&sa, sizeof(sa));
	bcopy(hp->h_addr, (char *)&sa.sin_addr, hp->h_length * sizeof(char));
	sa.sin_family=hp->h_addrtype;
	sa.sin_port=htons((unsigned short) portnum);

	if ((s=socket(hp->h_addrtype, SOCK_STREAM, 0)) < 0) /* Get socket */
	{
		return(-1);
	}
	if (connect(s, &sa, sizeof(sa)) < 0)
	{
		close(s);
		return(-1);
	}
	return(s);
}

/*
 * write_data - writes data to the socket...
 */
int write_data(int sock, const char *buf, int len)
{
	int bcount; /* Bytes written */
	int bw;     /* Bytes written this pass */

	bcount = 0;
	bw=0;
	while (bcount < len)
	{
		if ((bw=write(sock, buf, len-bcount)) > 0)
		{
			bcount += bw;
			buf += bw;
		}
		if (bw < 0)
		{
			return(-1);
		}
	}
	return(bcount);
}

/*
 * write_str - writes a C string to the game socket
 */
void write_str(const char *buf)
{
    if (write_data(GameSocket, buf, strlen(buf) * sizeof(char)) == -1)
    {
        fe_puts("*** error returned from write_data\n");
    }
}

/*
 * read_data - reads data from the game socket
 */
int read_data(int sock, char *buf, int len)
{
	int bcount; /* Bytes read */
	int br;     /* Bytes read this pass */

	bcount = 0;
	br=0;
	while (bcount < len)
	{
		if ((br=read(sock, buf, len-bcount)) > 0)
		{
			bcount += br;
			buf += br;
		}
		if (br < 0)
		{
			return(-1);
		}
	}
	return(bcount);
}

#define SERV_BUF_LEN	2048
static char ServBuf[SERV_BUF_LEN+1];
void handleServInp(int socket, void *data)
{
	int br;
	char locBuf[]="\0\0";

	if ((br=read(socket, &locBuf[0], 1)) == 1)
	{
		if (LoseNextChar)
		{
			LoseNextChar=FALSE;
			return;
		}
		strcat(&ServBuf[0], &locBuf[0]);
		/* Check for Newline */
		if (locBuf[0] == '\n')
		{
		    /* Parse it */
		    fe_parse(ServBuf);
		    ServBuf[0]='\0';
		}
		else {
			/* Check for a prompt line */
			if (DoingLogin)
			{
				if ((locBuf[0] == ':') && (ServBuf[0] == ':') &&
					(ServBuf[1] != '\0'))
				{
					fl_addto_browser_chars(fd_ImpFeMain->MainBrowser, ServBuf);
					if (strcmp(ServBuf, ":Enter player name:") == 0)
					{
						LoseNextChar=TRUE;
						if (game_player[0] != '\0')
						{
							write_str("!");
							write_str(game_player);
							write_str("\n");
							fl_addto_browser_chars(fd_ImpFeMain->MainBrowser,
												   game_player);
							fl_addto_browser_chars(fd_ImpFeMain->MainBrowser,
												   "\n");
						}
					}
					else if (strcmp(ServBuf, ":Enter player password:") == 0)
					{
						LoseNextChar=TRUE;
						if (game_player_pswd[0] != '\0')
						{
							write_str(game_player_pswd);
							write_str("\n");
							fl_addto_browser_chars(fd_ImpFeMain->MainBrowser,
												   game_player_pswd);
							fl_addto_browser_chars(fd_ImpFeMain->MainBrowser,
												   "\n");
						}
						DoingLogin=FALSE;
					}
					ServBuf[0]='\0';
				}
			}
		}
	}
	else
	{
		/* Remove the callback */
		fl_remove_io_callback(GameSocket, MyCond, handleServInp);
		/* Close the socket */
		close(GameSocket);
		GameSocket=-1;
		fl_set_menu_item_mode(fd_ImpFeMain->MainServerMenu,
							  MAIN_SERVER_MENU_CLOSE, FL_PUP_GREY);
		fl_set_menu_item_mode(fd_ImpFeMain->MainServerMenu,
							  MAIN_SERVER_MENU_OPEN, FL_PUP_NONE);
		fl_set_object_label(fd_ImpFeMain->StatusBox, "Offline");
		fe_puts("*** Connection to server terminated...\n");
		AtMainPrompt=FALSE;
	}
}

/*
 * cmd_connect
 */
BOOL cmd_connect(BOOL kill_it)
{
	/* Are we trying to stop the server? */
	if (kill_it)
	{
		/* Make sure the socket was valid */
		if (GameSocket != -1)
		{
			/* Remove the callback */
			fl_remove_io_callback(GameSocket, MyCond, handleServInp);
			/* Close the socket */
			close(GameSocket);
			GameSocket=-1;
			fe_puts("*** Connection to server closed\n");
		        AtMainPrompt=FALSE;
		}
		return(TRUE);
	}
	fl_addto_browser_chars(fd_ImpFeMain->MainBrowser, "*** Attempting to connect to ");
	fl_addto_browser_chars(fd_ImpFeMain->MainBrowser, game_host);
	fl_addto_browser_chars(fd_ImpFeMain->MainBrowser, ", port ");
	fl_addto_browser_chars(fd_ImpFeMain->MainBrowser, game_port);
	fl_addto_browser_chars(fd_ImpFeMain->MainBrowser, "\n");
	if ((GameSocket=call_socket(game_host, atoi(game_port))) == -1)
	{
		fe_puts("**** Connection failed\n");
		AtMainPrompt=FALSE;
		return(FALSE);
	}
	/* Add in the callback for the socket */
	fl_add_io_callback(GameSocket, MyCond, handleServInp, NULL);
	DoingLogin=TRUE;
	return(TRUE);
}
