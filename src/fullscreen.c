/*
* IceBreaker
* Copyright (c) 2001 Matthew Miller <mattdm@mattdm.org> http://www.mattdm.org/
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


int isfullscreen=false;

int makefullscreen()
{
	printf("makefullscreen\n");
#ifdef _TINSPIRE
	SDL_Surface * fullscreensave=SDL_CreateRGBSurface(SDL_SWSURFACE,WIDTH,HEIGHT,16,0,0,0,0);
#else
	SDL_Surface * fullscreensave=SDL_CreateRGBSurface(SDL_SWSURFACE,WIDTH,HEIGHT,32,0,0,0,0);
#endif

	clean();
	SDL_BlitSurface(screen, NULL, fullscreensave, NULL);
	
	//screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE | SDL_FULLSCREEN );
#ifdef _TINSPIRE
	screen = SDL_SetVideoMode(WIDTH, HEIGHT, 16, SDL_HWSURFACE | SDL_FULLSCREEN );
#else
	screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_FULLSCREEN );
#endif
	if (screen == NULL)
	{
		fprintf(stderr, "Couldn't switch to full screen mode.\n"
		                "SDL error: "
		                "%s\n\n", SDL_GetError());
		exit(1);
	}	
	
	SDL_BlitSurface(fullscreensave, NULL, screen, NULL);
	SDL_UpdateRect(screen,0,0,0,0);
	SDL_FreeSurface(fullscreensave);
	return 0;
}

int makewindowed()
{
	printf("makewindowed\n");
#ifdef _TINSPIRE
	SDL_Surface * fullscreensave=SDL_CreateRGBSurface(SDL_SWSURFACE,WIDTH,HEIGHT,16,0,0,0,0);
#else
	SDL_Surface * fullscreensave=SDL_CreateRGBSurface(SDL_SWSURFACE,WIDTH,HEIGHT,32,0,0,0,0);
#endif
	clean();
	SDL_BlitSurface(screen, NULL, fullscreensave, NULL);
	
	printf("about to setvideomode\n");
	screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE);
	if (screen == NULL)
	{
		fprintf(stderr, "Couldn't switch to windowed screen mode.\n"
		                "SDL error: "
		                "%s\n\n", SDL_GetError());
		exit(1);
	}	
	
	SDL_BlitSurface(fullscreensave, NULL, screen, NULL);
	SDL_UpdateRect(screen,0,0,0,0);
	SDL_FreeSurface(fullscreensave);
	return 0;
}
