/*
* IceBreaker
* Copyright (c) 2000 Matthew Miller <mattdm@mattdm.org> http://www.mattdm.org/
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
#ifndef _TINSPIRE
#include <stdlib.h>
#endif
#include "icebreaker.h"
#include "globals.h"
#include "laundry.h"

static SDL_Rect laundrylist[MAXDIRTY];
static int laundrycount;

void initlaundry()
{
	laundrycount=0;
}


void soil(SDL_Rect r)
{ // makes stuff dirty, of course.
	if (laundrycount<MAXDIRTY) // it's good to have this check here, but
	{                          // since this is the most-used function in
	                           // the whole program, it might be worth removing
	                           // for production builds...
		if( r.x > SCREEN_WIDTH ) {
			printf("correcting x\n");
			r.x = SCREEN_WIDTH;
		}
		if( r.y > SCREEN_HEIGHT ) {
			printf("correcting y\n");
			r.y = SCREEN_HEIGHT;
		}
		if( (r.y + r.h) > SCREEN_HEIGHT ) {
			printf("correcting height\n");
			r.h = 0;
		}
		if( (r.x + r.w) > SCREEN_WIDTH ) {
			printf("correcting width\n");
			r.w = 0;
		}
		laundrylist[laundrycount] = r;
		laundrycount++;
	}
	else
	{
		fprintf(stderr, "Too much dirty laundry!\n");
		exit(1);
	}
}

void clean()
{
//#ifdef _TINSPIRE
//	SDL_UpdateRect(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
//#else
	//printf("start clean\n");
	SDL_UpdateRects(screen, laundrycount, laundrylist);
	//SDL_UpdateRect(screen,0,0,0,0);
//#endif
	laundrycount=0;
	//printf("finish clean\n");
}
