#ifndef LINT
static char rcsid[]="$Id: planet.c,v 1.2 2000/05/24 20:40:39 marisa Exp $";
#endif
/*
 * ImpFE
 *
 * $Id: planet.c,v 1.2 2000/05/24 20:40:39 marisa Exp $
 *
 * This file contains routines for handling planet data manipulation
 *
 * $Log: planet.c,v $
 * Revision 1.2  2000/05/24 20:40:39  marisa
 * Disable some menu options until config file created or opened.
 *
 * Revision 1.1.1.1  2000/05/17 19:22:13  marisa
 * First CVS checkin
 *
 * Revision 1.1.1.1  1997/04/27 16:34:28  marisag
 * patch1: Fix browser bug
 *
 * Revision 1.1  1997/04/25 06:41:04  marisag
 * Initial revision
 *
 *
 */

#include "config.h"
#include <sys/types.h>
#include <time.h>
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
#include "gui.h"

/*
 * buildPlCensus - Clears the existing contents of the planet census
 *                 window and redraws it based on current known
 *                 planets.
 */
void buildPlCensus(void)
{
	unsigned int plNum, crew;
	USHORT eff;
	FePlanet_t tmpPlanet;
	char workBuf[90], nameBuf[10], plClass, location[15], crewBuf[6],
		tBuff[28], plName[10];

	/* Speed up output */
	fl_freeze_form(fd_PlanetCensusForm->PlanetCensusForm);
	/* Clear the existing contents, if any */
	fl_clear_browser(fd_PlanetCensusForm->PlBrowse);

	/* Loop for each known planet */
	for (plNum=0; plNum < next_planet; plNum++)
	{
//		/* Make sure the ship has been seen */
		if (readPlanet(&tmpPlanet, plNum))
		{
			if (tmpPlanet.last_seen != 0)
			{
			    if (tmpPlanet.owner != 0)
			    {
				    strncpy(nameBuf, Player[tmpPlanet.owner].name, 9);
				    nameBuf[9] = '\0';
			    }
			    else
			    {
				    strcpy(nameBuf, "???");
			    }
			    strncpy(plName, tmpPlanet.name, 9);
			    plName[9]='\0';
			    if (tmpPlanet.class != 0)
			    {
				    plClass = tmpPlanet.class;
			    }
			    else
			    {
				    plClass = '?';
			    }
			    crew = tmpPlanet.quant[it_civilians] +
				    tmpPlanet.quant[it_military] +
				    tmpPlanet.quant[it_scientists] +
				    tmpPlanet.quant[it_officers];
			    if (crew < 1000)
			    {
				        snprintf(crewBuf, 6, "%4u ", crew);
			    }
			    else if (crew < 10000)
			    {
                    unsigned int_part = (crew / 1000);
                    unsigned dec_part = (crew - int_part) / 10;
				    snprintf(crewBuf, 6, "%2u.%01uk", (crew / 1000), int_part, dec_part);
			    }
			    else
			    {
				        snprintf(crewBuf, 6, "%4uk", crew / 1000);
			    }
			    sprintf(location, "%4u, %4u", tmpPlanet.pl_row, tmpPlanet.pl_col);
			    strcpy(tBuff, ctime(&tmpPlanet.last_seen));
			    tBuff[19]='\0';
			        sprintf(workBuf, "%8u  %8s   %8s     %c  %s %s  %3u  %3u %3u %3u %3u %3u  %s", 
                            plNum,
					        plName, nameBuf, plClass, location, crewBuf, tmpPlanet.eff,
					        tmpPlanet.gas, tmpPlanet.water, tmpPlanet.minr, tmpPlanet.gold, tmpPlanet.polut, &tBuff[4]);
			    fl_add_browser_line(fd_PlanetCensusForm->PlBrowse, workBuf);
		    }
		}
	}
	fl_unfreeze_form(fd_PlanetCensusForm->PlanetCensusForm);
}

/*
 * buildPlDetDisp - Clears the existing contents of the planet detail
 *                 window and redraws it based on current known
 *                 planets.
 */
void buildPlDetDisp(void)
{
	ULONG itNum;
	FePlanet_t tmpPlanet;
	FeItem_t tmpItem;
	char workBuf[90], nameBuf[10], itType, location[15], tBuff[28];

	/* Speed up output */
	fl_freeze_form(fd_PlanetDetailForm->PlanetDetailForm);
	/* Clear the existing contents, if any */
	fl_clear_browser(fd_PlanetDetailForm->PlProdBrowse);
	fl_clear_browser(fd_PlanetDetailForm->PlDetItem);
	/* read the planet in */
	(void) readPlanet(&tmpPlanet, LastPlanet);
	/* Loop for each production type */
	for (itNum=0; itNum < (PPROD_LAST + 1); itNum++)
	{
		sprintf(workBuf, "%11s   %5u   %3u", PPROD_NAME[itNum],
				tmpPlanet.prod[itNum], tmpPlanet.workPer[itNum]);
		fl_add_browser_line(fd_PlanetDetailForm->PlProdBrowse, workBuf);
	}
	/* Loop for each known big item */
	for (itNum=0; itNum < next_item; itNum++)
	{
		/* Make sure the item has been seen and is on the planet */
		if (!readItem(&tmpItem, itNum))
		{
			continue;
		}
		if ((tmpItem.last_seen != 0) && (tmpItem.where == LastPlanet) &&
			(tmpItem.flags & BF_ONPLAN))
		{
			if (tmpItem.owner != 0)
			{
				strncpy(nameBuf, Player[tmpItem.owner].name, 9);
				nameBuf[9] = '\0';
			}
			else
			{
				strcpy(nameBuf, "???");
			}
			if (tmpItem.type != 0)
			{
				itType=tmpItem.type;
			}
			else
			{
				itType='?';
			}
			sprintf(workBuf, "%8u  %c  %3u  %3u  %5u", itNum,
					itType, tmpItem.it_tf, tmpItem.efficiency,
					tmpItem.weight);
			fl_add_browser_line(fd_PlanetDetailForm->PlDetItem, workBuf);
		}
	}
	sprintf(workBuf, "%u", tmpPlanet.size);
	fl_set_object_label(fd_PlanetDetailForm->Size, workBuf);
	sprintf(workBuf, "%u", tmpPlanet.eff);
	fl_set_object_label(fd_PlanetDetailForm->Effic, workBuf);
	sprintf(workBuf, "%u", tmpPlanet.polut);
	fl_set_object_label(fd_PlanetDetailForm->Polut, workBuf);
	sprintf(workBuf, "%u", tmpPlanet.minr);
	fl_set_object_label(fd_PlanetDetailForm->Minerals, workBuf);
	sprintf(workBuf, "%u", tmpPlanet.gold);
	fl_set_object_label(fd_PlanetDetailForm->Gold, workBuf);
	sprintf(workBuf, "%u", tmpPlanet.water);
	fl_set_object_label(fd_PlanetDetailForm->Water, workBuf);
	sprintf(workBuf, "%u", tmpPlanet.gas);
	fl_set_object_label(fd_PlanetDetailForm->Gas, workBuf);
	sprintf(workBuf, "%u", tmpPlanet.mobil);
	fl_set_object_label(fd_PlanetDetailForm->Mobil, workBuf);
	sprintf(workBuf, "%u", tmpPlanet.TF);
	fl_set_object_label(fd_PlanetDetailForm->TF, workBuf);
	switch(tmpPlanet.xfer)
	{
		case PT_PEACEFULL:
			fl_set_object_label(fd_PlanetDetailForm->Transfer, "New Owner");
			break;
		case PT_TRADE:
			fl_set_object_label(fd_PlanetDetailForm->Transfer, "Trade");
			break;
		case PT_HOME:
			fl_set_object_label(fd_PlanetDetailForm->Transfer, "Home Planet");
			break;
		case PT_HOSTILE:
			fl_set_object_label(fd_PlanetDetailForm->Transfer, "HOSTILE");
			break;
		default:
			fl_set_object_label(fd_PlanetDetailForm->Transfer, "IMPOSSIBLE");
			break;
	}
	if (tmpPlanet.lstOwn != 0)
	{
		strcpy(workBuf, Player[tmpPlanet.lstOwn].name);
	}
	else
	{
		strcpy(workBuf, "NONE");
	}
	fl_set_object_label(fd_PlanetDetailForm->PrevOwn, workBuf);
	if (tmpPlanet.plgStg != 0)
	{
		fl_set_object_label(fd_PlanetDetailForm->Plague, "YES");
	}
	else
	{
		fl_set_object_label(fd_PlanetDetailForm->Plague, "NO");
	}
	sprintf(workBuf, "%u", tmpPlanet.RF);
	fl_set_object_label(fd_PlanetDetailForm->RF, workBuf);
	sprintf(workBuf, "%u", tmpPlanet.ResLv);
	fl_set_object_label(fd_PlanetDetailForm->Research, workBuf);
	sprintf(workBuf, "%u", tmpPlanet.TechLv);
	fl_set_object_label(fd_PlanetDetailForm->Tech, workBuf);
	sprintf(workBuf, "%u", tmpPlanet.pl_btu);
	fl_set_object_label(fd_PlanetDetailForm->BTU, workBuf);
	sprintf(workBuf, "%u", tmpPlanet.quant[it_civilians]);
	fl_set_object_label(fd_PlanetDetailForm->Civ, workBuf);
	sprintf(workBuf, "%u", tmpPlanet.quant[it_military]);
	fl_set_object_label(fd_PlanetDetailForm->Mil, workBuf);
	sprintf(workBuf, "%u", tmpPlanet.quant[it_scientists]);
	fl_set_object_label(fd_PlanetDetailForm->Sci, workBuf);
	sprintf(workBuf, "%u", tmpPlanet.quant[it_officers]);
	fl_set_object_label(fd_PlanetDetailForm->Ofc, workBuf);
	sprintf(workBuf, "%u", tmpPlanet.quant[it_missiles]);
	fl_set_object_label(fd_PlanetDetailForm->Misl, workBuf);
	sprintf(workBuf, "%u", tmpPlanet.quant[it_planes]);
	fl_set_object_label(fd_PlanetDetailForm->Plane, workBuf);
	sprintf(workBuf, "%u", tmpPlanet.quant[it_ore]);
	fl_set_object_label(fd_PlanetDetailForm->Ore, workBuf);
	sprintf(workBuf, "%u", tmpPlanet.quant[it_bars]);
	fl_set_object_label(fd_PlanetDetailForm->Bars, workBuf);
	sprintf(workBuf, "%u", tmpPlanet.quant[it_airTanks]);
	fl_set_object_label(fd_PlanetDetailForm->AirTnk, workBuf);
	sprintf(workBuf, "%u", tmpPlanet.quant[it_fuelTanks]);
	fl_set_object_label(fd_PlanetDetailForm->FuelTnk, workBuf);
	sprintf(workBuf, "%u", tmpPlanet.PF);
	fl_set_object_label(fd_PlanetDetailForm->PF, workBuf);
	sprintf(workBuf, "%u", tmpPlanet.PopPct);
	fl_set_object_label(fd_PlanetDetailForm->PopPct, workBuf);
	strcpy(workBuf, ctime(&tmpPlanet.last_seen));
	workBuf[24]='\0'; /* strip off NL */
	fl_set_object_label(fd_PlanetDetailForm->Seen, workBuf);
	sprintf(workBuf, "%u", LastPlanet);
	fl_set_object_label(fd_PlanetDetailForm->PlNum, workBuf);
	sprintf(workBuf, "%u,%u", tmpPlanet.pl_row, tmpPlanet.pl_col);
	fl_set_object_label(fd_PlanetDetailForm->RowCol, workBuf);
	if (tmpPlanet.class != 0)
	{
		sprintf(workBuf, "%c", tmpPlanet.class);
		fl_set_object_label(fd_PlanetDetailForm->Class, workBuf);
	}
	else
	{
		fl_set_object_label(fd_PlanetDetailForm->Class, "?");
	}
	if (tmpPlanet.owner != 0)
	{
		strcpy(workBuf, Player[tmpPlanet.owner].name);
	}
	else
	{
		strcpy(workBuf, "???");
	}
	if (tmpPlanet.owner == player_num)
	{
		fl_set_object_label(fd_PlanetDetailForm->OwnerRel, "N/A");
	}
	else
	{
		if (tmpPlanet.owner != 0)
		{
			switch (Player[tmpPlanet.owner].relation)
			{
				case relation_default:
					/* Check the race relation */
					switch (Race[Player[tmpPlanet.owner].race].relation)
					{
						case relation_allied:
							strcpy(workBuf, "Def/Allied");
							break;
						case relation_war:
							strcpy(workBuf, "Def/*WAR*");
							break;
						case relation_default:
							strcpy(workBuf, "Def/Neutral");
							break;
						default:
							strcpy(workBuf, "Def/???");
							break;
					}
					break;
				case relation_allied:
					strcpy(workBuf, "Allied");
					break;
				case relation_war:
					strcpy(workBuf, "*WAR*");
					break;
				default:
					strcpy(workBuf, "???");
					break;
			}
		}
		else
		{
			strcpy(workBuf, "???");
		}
		fl_set_object_label(fd_PlanetDetailForm->OwnerRel, workBuf);
	}
	fl_set_object_label(fd_PlanetDetailForm->Owner, workBuf);
	fl_set_input(fd_PlanetDetailForm->Name, tmpPlanet.name);
	fl_set_input(fd_PlanetDetailForm->Checkpoint, tmpPlanet.chkpoint);

	fl_unfreeze_form(fd_PlanetDetailForm->PlanetDetailForm);
}

