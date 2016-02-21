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
#ifndef _TINSPIRE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#ifndef WIN32
	#include <pwd.h>
#endif
#include <string.h>
#include <sys/types.h>
#endif
              

#include "icebreaker.h"
#include "penguin.h"
#include "line.h"
#include "grid.h"
#include "sound.h"
#include "globals.h"
#include "level.h"
#include "intro.h"
#include "text.h"
#include "transition.h"
#include "hiscore.h"
#include "dialog.h"
#include "options.h"
#include "fullscreen.h"
#include "cursor.h"
#ifdef _TINSPIRE
#include "PenguinGraphic.h"
#endif

// global
SDL_Surface * screen;
SDL_Surface * screensave;
SDL_Surface * penguinimage;

char grid[WIDTH][HEIGHT];

char username[50]; // FIX -- move this into the options struct?
char homedir[255];

#ifndef _TINSPIRE
SDL_Surface * penguinicon;
#endif

// functions

int setup(void);
void cleanup(void);

/************************************************************************/
#define SCREEN_BIT_DEPTH        (16)
#define SCREEN_VIDEO_MODE (SDL_SWSURFACE|SDL_FULLSCREEN|SDL_HWPALETTE)

int setup(void)
{
	int newuser = false;
#ifndef _TINSPIRE
	struct passwd * userinfo;

	srandom(time(NULL));	
	
	
	//stupid buffers
	setvbuf(stdout,(char *)NULL, _IOLBF, 0);
	
	userinfo = getpwuid(getuid()); // FIX -- make this part of the options struct; and maybe save in options file
	strncpy(username,userinfo->pw_name,50); // not like it's gonna be fifty characters. but y'know.
	strncpy(homedir,userinfo->pw_dir,255);
#endif

#ifndef AJTODO
	readhiscores();
	newuser=readoptions();	
#endif	

	if (SDL_Init(SDL_INIT_VIDEO))
	{
		fprintf(stderr, "Hey. We're gonna need some graphics.\n"
		                "SDL error: " 
		                "%s\n\n", SDL_GetError());
		exit(1);
	}

#ifndef _TINSPIRE	
	//atexit(SDL_Quit);
	atexit(cleanup);
#endif

#ifndef _TINSPIRE
	penguinicon = SDL_LoadBMP(
#ifndef _TINSPIRE
DATAPREFIX "/" 
#endif
PENGUINICONFILE);
	if (penguinicon==NULL) fprintf(stderr, "Icon not loaded!\n");
	SDL_WM_SetIcon(penguinicon,NULL);
	
	SDL_WM_SetCaption("IceBreaker","IceBreaker");
#endif

#ifdef _TINSPIRE	
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BIT_DEPTH, SCREEN_VIDEO_MODE);
#else
	if (options.fullscreen!=FULLSCREENOFF)
	{
		screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_FULLSCREEN);	
	}
	else
	{
		screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE);
	}
#endif
	if (screen == NULL)
	{
		fprintf(stderr, "Help! Couldn't get a window.\n"
		                "SDL error: " 
		                "%s\n\n", SDL_GetError());
		exit(1);
	}

#ifdef _TINSPIRE	
	screensave = SDL_CreateRGBSurface(SDL_SWSURFACE,screen->w,screen->h,16,0,0,0,0);
#else
	screensave = SDL_CreateRGBSurface(SDL_SWSURFACE,screen->w,screen->h,32,0,0,0,0);
#endif

	// FIX -- need "initpenguin" routine. and some error checking!
#ifdef _TINSPIRE
	penguinimage = nSDL_LoadImage(image_penguin);
	SDL_SetColorKey(penguinimage, SDL_SRCCOLORKEY, SDL_MapRGB(penguinimage->format, 0xFF, 0x00, 0x00));
#else
	penguinimage = SDL_LoadBMP(DATAPREFIX "/" PENGUINBMPFILE);
	SDL_SetColorKey(penguinimage, SDL_SRCCOLORKEY, SDL_MapRGB(penguinimage->format, 0xFF, 0x00, 0x00));
#endif	

#ifndef _TINSPIRE
	// FIX -- need preference.
	initsound();
#endif

	inittext();


	return newuser;
}

void cleanup()
{
	//writehiscores(); // now written only when hi score is actually achieved
#ifndef _TINSPIRE
	quitsound();
#endif
	SDL_Quit();
	writeoptions();
}



int main(int argc,char *argv[])
{
#ifdef _TINSPIRE
	enable_relative_paths(argv);
#endif
	int done = false;
	int level=0;
	ScoreSheet levelscore;
	long totalscore=0;
	char windowtitle[35];
	LevelExitType levelresult;
	int newuser=false;
	
	
	newuser=setup();

	done=intro();	

	if (!done && newuser)
	{ // no options file; using the default
		setcursor(CURSORCLICK);
		popuphelp();	
		setcursor(CURSORARROW);
	}

	
	SDL_EnableKeyRepeat(10, SDL_DEFAULT_REPEAT_INTERVAL);	
 	while(!done)
	{
		level++;
		if (level>=MAXPENGUINS) level=MAXPENGUINS-1;

#ifndef AJTODO
		switch (options.difficulty)
		{
			case NORMAL:
				snprintf(windowtitle,35,"IceBreaker -- Level %d",level);
			break;
			case HARD:
				snprintf(windowtitle,35,"IceBreaker -- Level %d (Hard)",level);
			break;
			case EASY:
				snprintf(windowtitle,35,"IceBreaker -- Level %d (Easy)",level);
			break;
		}		
#endif
		SDL_WM_SetCaption(windowtitle,"IceBreaker");
		
		levelresult=playlevel(level,totalscore,&levelscore);
		
  		SDL_WM_SetCaption("IceBreaker","IceBreaker");
		
		totalscore+= levelscore.basescore + levelscore.clearbonus + levelscore.lifebonus;
		
		if (levelresult == QUIT)
		{
			done=true;
		}
		else if (levelresult == DEAD)
		{
			done=gameover(totalscore);
			
			// hooray! modifying the index variable in the loop!
			// good coding practice at its finest!
			level=0; 
			totalscore=0; 
		}
		else if (levelresult == ERROR)
		{
			fprintf(stderr,"Level error -- this should never happen.\n");
		}
		else
		{
			// level completed successfully
			done=intermission(&levelscore,level+1);
		}

	}
	//printf("===========================================================\n");
	//printf("\nFinal Score: %ld\n",totalscore);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	clearCursor();	
	
	return(0);
}
