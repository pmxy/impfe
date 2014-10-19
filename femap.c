#ifndef LINT
static char rcsid[]="$Id: femap.c,v 1.1.1.1 2000/05/17 19:22:13 marisa Exp $";
#endif
/*
 * ImpFE
 *
 * $Id: femap.c,v 1.1.1.1 2000/05/17 19:22:13 marisa Exp $
 *
 * This file contains routines for managing the map window
 *
 * $Log: femap.c,v $
 * Revision 1.1.1.1  2000/05/17 19:22:13  marisa
 * First CVS checkin
 *
 * Revision 1.1.1.2  1997/04/24 04:46:04  marisag
 * patch1: Many changes
 *
 * Revision 1.1.1.1  1997/04/17 06:51:19  marisag
 * patch1: Was using planc.xpm instead of pland.xpm
 *
 * Revision 1.1  1997/04/16 03:08:31  marisag
 * Initial revision
 *
 *
 */

#include "config.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
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

/* Include our Pixmaps */
#include "btile.xpm"
#include "unknown.xpm"
#include "genplan.xpm"
#include "star.xpm"
#include "bhole.xpm"
#include "snova.xpm"
#include "plana.xpm"
#include "planb.xpm"
#include "planc.xpm"
#include "pland.xpm"
#include "planh.xpm"
#include "planm.xpm"
#include "plann.xpm"
#include "plano.xpm"
#include "planq.xpm"

/*
 * setPm - sets the Pixmap for a given object
 */
void setPm(FL_OBJECT *ob, const char **bits)
{
	fl_free_pixmapbutton_pixmap(ob);
	fl_set_pixmapbutton_data(ob, (unsigned char **)bits);
}

/*
 * buildMapDisp - Builds up the map display based on LastX and LastY
 *                Used only for initial building, scrollFeMap() does
 *                the maintenance once the window is open
 *                Depends on LastX & LastY never being within 9 of the
 *                edge!
 */
void buildMapDisp(void)
{
    int row, col, tmp;
    const char **PixMap;
	char posit[25];

	/* Loop for each row */
    for (row=0; row<7; row++)
    {
		/* Set the row coord field */
		switch (row)
		{
			case 0:
				sprintf(posit, "%04u", row + LastY);
				fl_set_object_label(fd_MapForm->Row1, posit);
				break;
			case 1:
				sprintf(posit, "%04u", row + LastY);
				fl_set_object_label(fd_MapForm->Row2, posit);
				break;
			case 2:
				sprintf(posit, "%04u", row + LastY);
				fl_set_object_label(fd_MapForm->Row3, posit);
				break;
			case 3:
				sprintf(posit, "%04u", row + LastY);
				fl_set_object_label(fd_MapForm->Row4, posit);
				break;
			case 4:
				sprintf(posit, "%04u", row + LastY);
				fl_set_object_label(fd_MapForm->Row5, posit);
				break;
			case 5:
				sprintf(posit, "%04u", row + LastY);
				fl_set_object_label(fd_MapForm->Row6, posit);
				break;
			case 6:
				sprintf(posit, "%04u", row + LastY);
				fl_set_object_label(fd_MapForm->Row7, posit);
				break;
		}

		/* Loop for each column */
		for (col=0; col<10; col++)
		{
			/* If doing the first row, also set the column label */
			if (row == 0)
			{
				/* Set the row coord field */
				switch (col)
				{
					case 0:
						sprintf(posit, "%04u", col + LastX);
						fl_set_object_label(fd_MapForm->Col1, posit);
						break;
					case 1:
						sprintf(posit, "%04u", col + LastX);
						fl_set_object_label(fd_MapForm->Col2, posit);
						break;
					case 2:
						sprintf(posit, "%04u", col + LastX);
						fl_set_object_label(fd_MapForm->Col3, posit);
						break;
					case 3:
						sprintf(posit, "%04u", col + LastX);
						fl_set_object_label(fd_MapForm->Col4, posit);
						break;
					case 4:
						sprintf(posit, "%04u", col + LastX);
						fl_set_object_label(fd_MapForm->Col5, posit);
						break;
					case 5:
						sprintf(posit, "%04u", col + LastX);
						fl_set_object_label(fd_MapForm->Col6, posit);
						break;
					case 6:
						sprintf(posit, "%04u", col + LastX);
						fl_set_object_label(fd_MapForm->Col7, posit);
						break;
					case 7:
						sprintf(posit, "%04u", col + LastX);
						fl_set_object_label(fd_MapForm->Col8, posit);
						break;
					case 8:
						sprintf(posit, "%04u", col + LastX);
						fl_set_object_label(fd_MapForm->Col9, posit);
						break;
					case 9:
						sprintf(posit, "%04u", col + LastX);
						fl_set_object_label(fd_MapForm->ColA, posit);
						break;
				}
			}
			/* Get the sector type */
			tmp = (int) BitMap->coord[row + LastY][col + LastX].type;
			/* Set the pointer based on the sector type */
			switch (tmp)
			{
				case BM_UNKNOWN:
					PixMap=unknown_xpm;
					break;
				case BM_PLAN:
					PixMap=genplan_xpm;
					break;
				case BM_STAR:
					PixMap=star_xpm;
					break;
				case BM_BH:
					PixMap=bhole_xpm;
					break;
				case BM_SN:
					PixMap=snova_xpm;
					break;
				case BM_PC_A:
					PixMap=plana_xpm;
					break;
				case BM_PC_B:
					PixMap=planb_xpm;
					break;
				case BM_PC_C:
					PixMap=planc_xpm;
					break;
				case BM_PC_D:
					PixMap=pland_xpm;
					break;
				case BM_PC_M:
					PixMap=planm_xpm;
					break;
				case BM_PC_N:
					PixMap=plann_xpm;
					break;
				case BM_PC_O:
					PixMap=plano_xpm;
					break;
				case BM_PC_Q:
					PixMap=planq_xpm;
					break;
				case BM_PC_H:
					PixMap=planh_xpm;
					break;
				case BM_EMPTY:
					PixMap=btile_xpm;
					break;
				default:
					fprintf(stderr, "*** Unknown sector type\n");
					PixMap=unknown_xpm;
					break;
			}
			/* God this is messy */
			/* Now update the pixmap reference */
			switch (row)
			{
				case 0: /* Row "A" */
					switch(col)
					{
						case 0:
							setPm(fd_MapForm->A1, PixMap);
							break;
						case 1:
							setPm(fd_MapForm->A2, PixMap);
							break;
						case 2:
							setPm(fd_MapForm->A3, PixMap);
							break;
						case 3:
							setPm(fd_MapForm->A4, PixMap);
							break;
						case 4:
							setPm(fd_MapForm->A5, PixMap);
							break;
						case 5:
							setPm(fd_MapForm->A6, PixMap);
							break;
						case 6:
							setPm(fd_MapForm->A7, PixMap);
							break;
						case 7:
							setPm(fd_MapForm->A8, PixMap);
							break;
						case 8:
							setPm(fd_MapForm->A9, PixMap);
							break;
						case 9:
							setPm(fd_MapForm->AA, PixMap);
							break;
					}
					break;
				case 1: /* Row "B" */
					switch(col)
					{
						case 0:
							setPm(fd_MapForm->B1, PixMap);
							break;
						case 1:
							setPm(fd_MapForm->B2, PixMap);
							break;
						case 2:
							setPm(fd_MapForm->B3, PixMap);
							break;
						case 3:
							setPm(fd_MapForm->B4, PixMap);
							break;
						case 4:
							setPm(fd_MapForm->B5, PixMap);
							break;
						case 5:
							setPm(fd_MapForm->B6, PixMap);
							break;
						case 6:
							setPm(fd_MapForm->B7, PixMap);
							break;
						case 7:
							setPm(fd_MapForm->B8, PixMap);
							break;
						case 8:
							setPm(fd_MapForm->B9, PixMap);
							break;
						case 9:
							setPm(fd_MapForm->BA, PixMap);
							break;
					}
					break;
				case 2: /* Row "C" */
					switch(col)
					{
						case 0:
							setPm(fd_MapForm->C1, PixMap);
							break;
						case 1:
							setPm(fd_MapForm->C2, PixMap);
							break;
						case 2:
							setPm(fd_MapForm->C3, PixMap);
							break;
						case 3:
							setPm(fd_MapForm->C4, PixMap);
							break;
						case 4:
							setPm(fd_MapForm->C5, PixMap);
							break;
						case 5:
							setPm(fd_MapForm->C6, PixMap);
							break;
						case 6:
							setPm(fd_MapForm->C7, PixMap);
							break;
						case 7:
							setPm(fd_MapForm->C8, PixMap);
							break;
						case 8:
							setPm(fd_MapForm->C9, PixMap);
							break;
						case 9:
							setPm(fd_MapForm->CA, PixMap);
							break;
					}
					break;
				case 3: /* Row "D" */
					switch(col)
					{
						case 0:
							setPm(fd_MapForm->D1, PixMap);
							break;
						case 1:
							setPm(fd_MapForm->D2, PixMap);
							break;
						case 2:
							setPm(fd_MapForm->D3, PixMap);
							break;
						case 3:
							setPm(fd_MapForm->D4, PixMap);
							break;
						case 4:
							setPm(fd_MapForm->D5, PixMap);
							break;
						case 5:
							setPm(fd_MapForm->D6, PixMap);
							break;
						case 6:
							setPm(fd_MapForm->D7, PixMap);
							break;
						case 7:
							setPm(fd_MapForm->D8, PixMap);
							break;
						case 8:
							setPm(fd_MapForm->D9, PixMap);
							break;
						case 9:
							setPm(fd_MapForm->DA, PixMap);
							break;
					}
					break;
				case 4: /* Row "E" */
					switch(col)
					{
						case 0:
							setPm(fd_MapForm->E1, PixMap);
							break;
						case 1:
							setPm(fd_MapForm->E2, PixMap);
							break;
						case 2:
							setPm(fd_MapForm->E3, PixMap);
							break;
						case 3:
							setPm(fd_MapForm->E4, PixMap);
							break;
						case 4:
							setPm(fd_MapForm->E5, PixMap);
							break;
						case 5:
							setPm(fd_MapForm->E6, PixMap);
							break;
						case 6:
							setPm(fd_MapForm->E7, PixMap);
							break;
						case 7:
							setPm(fd_MapForm->E8, PixMap);
							break;
						case 8:
							setPm(fd_MapForm->E9, PixMap);
							break;
						case 9:
							setPm(fd_MapForm->EA, PixMap);
							break;
					}
					break;
				case 5: /* Row "F" */
					switch(col)
					{
						case 0:
							setPm(fd_MapForm->F1, PixMap);
							break;
						case 1:
							setPm(fd_MapForm->F2, PixMap);
							break;
						case 2:
							setPm(fd_MapForm->F3, PixMap);
							break;
						case 3:
							setPm(fd_MapForm->F4, PixMap);
							break;
						case 4:
							setPm(fd_MapForm->F5, PixMap);
							break;
						case 5:
							setPm(fd_MapForm->F6, PixMap);
							break;
						case 6:
							setPm(fd_MapForm->F7, PixMap);
							break;
						case 7:
							setPm(fd_MapForm->F8, PixMap);
							break;
						case 8:
							setPm(fd_MapForm->F9, PixMap);
							break;
						case 9:
							setPm(fd_MapForm->FA, PixMap);
							break;
					}
					break;
				case 6: /* Row "G" */
					switch(col)
					{
						case 0:
							setPm(fd_MapForm->G1, PixMap);
							break;
						case 1:
							setPm(fd_MapForm->G2, PixMap);
							break;
						case 2:
							setPm(fd_MapForm->G3, PixMap);
							break;
						case 3:
							setPm(fd_MapForm->G4, PixMap);
							break;
						case 4:
							setPm(fd_MapForm->G5, PixMap);
							break;
						case 5:
							setPm(fd_MapForm->G6, PixMap);
							break;
						case 6:
							setPm(fd_MapForm->G7, PixMap);
							break;
						case 7:
							setPm(fd_MapForm->G8, PixMap);
							break;
						case 8:
							setPm(fd_MapForm->G9, PixMap);
							break;
						case 9:
							setPm(fd_MapForm->GA, PixMap);
							break;
					}
					break;
			}
		}
    }
	sprintf(posit, "%4u", LastY);
	fl_set_input(fd_MapForm->JmRow, posit);
	sprintf(posit, "%4u", LastX);
	fl_set_input(fd_MapForm->JmCol, posit);
}

/*
 * scrollFeMap - Scrolls the map via the given stepX and stepY,
 *               and updates LastX and LastY
 *               Will not let LastX & LastY never be within 9 of the
 *               bottom/right edge!
 */
void scrollFeMap(int stepX, int stepY)
{
	BOOL doMove=FALSE;

	/* Handle simple cases */
	if ((stepX == 0) && (stepY == 0))
	{
		/* Do nothing */
		return;
	}
	if (stepX != 0)
	{
		if (abs(stepX) < 10)
		{
			/* Moving X < 10 */
			if (stepX < 0)
			{
				if (LastX > 0)
				{
					LastX--;
					doMove=TRUE;
				}
			}
			else
			{
				if (LastX < ((game_sizex - 1) * 10))
				{
					LastX++;
					doMove=TRUE;
				}
				else
				{
					LastX=(game_sizex - 1) * 10;
				}
			}
		}
		else
		{
			/* Moving X >= 10 */
			if (stepX < 0)
			{
				if (LastX > 9)
				{
					LastX -= 10;
					doMove=TRUE;
				}
				else
				{
					LastX = 0;
					doMove=TRUE;
				}
			}
			else
			{
				if (LastX < ((game_sizex - 2) * 10))
				{
					LastX += 10;
					doMove=TRUE;
				}
				else
				{
					LastX = (game_sizex - 1) * 10;
					doMove=TRUE;
				}
			}
		}
	}

	/* Now handle Y movement */
	if (stepY != 0)
	{
		if (abs(stepY) < 7)
		{
			/* Moving Y < 7 */
			if (stepY < 0)
			{
				if (LastY > 0)
				{
					LastY--;
					doMove=TRUE;
				}
			}
			else
			{
				if (LastY < ((game_sizey * 10) - 7))
				{
					LastY++;
					doMove=TRUE;
				}
			}
		}
		else
		{
			/* Moving Y >= 7 */
			if (stepY < 0)
			{
				if (LastY > 7)
				{
					LastY -= 7;
					doMove=TRUE;
				}
				else
				{
					LastY = 0;
					doMove=TRUE;
				}
			}
			else
			{
				if (LastY < (((game_sizey - 1) * 10) - 7))
				{
					LastY += 7;
					doMove=TRUE;
				}
				else
				{
					LastY = ((game_sizey * 10) - 7);
					doMove=TRUE;
				}
			}
		}
	}

	/* Show the new map display */
	if (doMove)
	{
		buildMapDisp();
	}
}

/*
 * updateMaps - This function gets called when another function thinks
 *              the map(s) might need to be updated.
 */
void updateMaps(void)
{
	if (fl_form_is_visible(fd_MapForm->MapForm))
	{
		buildMapDisp();
	}
}

