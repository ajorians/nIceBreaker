/*
* IceBreaker
* Copyright (c) 2000-2001 Matthew Miller <mattdm@mattdm.org> 
*   http://www.mattdm.org/
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the Free
* Software Foundation; either version 2 of the License, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
* for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc., 59
* Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
*/

#include "SDL/SDL.h"

#include "icebreaker.h"
#include "text.h"
#include "globals.h"
#include "hiscore.h"
#include "laundry.h"
#include "status.h"

void updatestatuslives(int lives)
{
	//FIX == shouldn't need to recalc color every time.

	SDL_Rect tmprect;
	char tmptext[30]; // should be plenty big.
	
	//lives=99; //debug
	
	tmprect.h=CHARHEIGHT*2; tmprect.w=LIVESW;
	tmprect.x=LIVESX; tmprect.y=STATUSY;
	
	SDL_FillRect(screen,&tmprect,SDL_MapRGB(screen->format,  0x00, 0x40, 0x80));
	
#ifdef _TINSPIRE
	char buffer[10];
	itoa(lives, buffer, 10);
	strcpy(tmptext, "LIVES: ");
	strcat(tmptext, buffer);
#else
	snprintf(tmptext,30,"LIVES: %d",lives);
#endif
	puttext(LIVESX,STATUSY,2,SDL_MapRGB(screen->format,  0xFF, 0xFF, 0xFF),tmptext);
	
	soil(tmprect);
}

void updatestatuscleared(int cleared)
{
	//FIX == shouldn't need to recalc color every time.

	SDL_Rect tmprect;
	char tmptext[30]; // should be plenty big.
	
	//cleared=99; // debug
	
	tmprect.h=CHARHEIGHT*2; tmprect.w=CLEAREDW;
	tmprect.x=CLEAREDX; tmprect.y=
#ifdef _TINSPIRE
BOTTOMSTATUSY
#else
STATUSY
#endif
;
	
	
	SDL_FillRect(screen,&tmprect,SDL_MapRGB(screen->format,  0x00, 0x40, 0x80));

#ifdef _TINSPIRE
	char buffer[10];
	itoa(cleared, buffer, 10);
	strcpy(tmptext, "CLEARED: ");
	strcat(tmptext, buffer);
	strcat(tmptext, "%");
#else	
	snprintf(tmptext,30, "CLEARED: %d%%",cleared);
#endif
	//printf("Writting at %d, %d: (%s)\n", CLEAREDX, BOTTOMSTATUSY, tmptext);
	puttext(CLEAREDX,
#ifdef _TINSPIRE
BOTTOMSTATUSY
#else
STATUSY
#endif
,2,SDL_MapRGB(screen->format,  0xFF, 0xFF, 0xFF),tmptext);
	
	
	soil(tmprect);
}

void updatestatusscore(long score)
{
	//FIX == shouldn't need to recalc color every time.

	SDL_Rect tmprect;
	char tmptext[30]; // should be plenty big.
	
	//score=99999; // debug
	
	tmprect.h=CHARHEIGHT*2; tmprect.w=SCOREW;
	tmprect.x=SCOREX; tmprect.y=STATUSY;
	
	SDL_FillRect(screen,&tmprect,SDL_MapRGB(screen->format,  0x00, 0x40, 0x80));

#ifdef _TINSPIRE	
	char buffer[10];
        itoa(score, buffer, 10);
        strcpy(tmptext, "SCORE: ");   
        strcat(tmptext, buffer);
#else
	snprintf(tmptext,30, "SCORE: %ld",score);
#endif
	puttext(SCOREX,STATUSY,2,SDL_MapRGB(screen->format,  0xFF, 0xFF, 0xFF),tmptext);

	soil(tmprect);
}



extern void updatehiscorebox()
{ // FIX -- this might belong in hiscore.c, but maybe not
#ifndef AJTODO
	printf("updatehiscorebox\n");
	SDL_Rect tmprect;
	
	char tmptext[40]; // should be plenty big.
	
	tmprect.x=LIVESX; tmprect.y=BOTTOMSTATUSY;
	tmprect.h=CHARHEIGHT*3;; tmprect.w=WIDTH-(CHARWIDTH*2*4)-MARGINRIGHT-4-LIVESX-2;
	
	
	SDL_FillRect(screen,&tmprect,SDL_MapRGB(screen->format,  0x00, 0x40, 0x80));
	
	snprintf(tmptext,40,"HIGH SCORE: %ld (%s)",hiscoreval[0],hiscorename[0]);
	puttext(tmprect.x,tmprect.y+3,2,SDL_MapRGB(screen->format,  0xFF, 0xFF, 0xFF),tmptext);	
	
	soil(tmprect);
	printf("end updatehiscorebox\n");
#endif
}
