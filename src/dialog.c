/*
* IceBreaker
* Copyright (c) 2000-2001 Matthew Miller <mattdm@mattdm.org> and
*   Enrico Tassi <f.tassi@mo.nettuno.it>
*
* <http://www.mattdm.org/icebreaker/>
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
#include <string.h>
#endif
#include "icebreaker.h"
#include "globals.h"
#include "text.h"
#include "laundry.h"
#include "dialog.h"
#include "sound.h"
#include "hiscore.h"
#include "options.h"
#include "fullscreen.h"

static int menuhandler(SDL_Rect menurect, int menulength, char ** menuitems, int (**menufunctions)(char *), int menuvaluetextwidth );

static int menuitem_newgame(char * notused);
static int menuitem_options(char * notused);
static int menuitem_highscores(char * notused);
static int menuitem_help(char * notused);
static int menuitem_quit(char * notused);

static int menuitem_sound(char * val);
static int menuitem_autopause(char * val);
static int menuitem_fullscreen(char * val);
static int menuitem_difficulty(char * val);
static int menuitem_theme(char * val);


#define MAXMENUITEMS 10
#define MAXMENUVALUELENGTH 10

int gethighusername(int highest)
{
	SDL_Rect tmprect,namerect,okayrect;
	SDL_Event event;
	int done=false;
	int quit=false;
	int okayglow=false;
	int okaypressed=false;
	int clearname=true;
	
	char keypressed;
	int insertionpoint=0;
	
	// save behind
	SDL_BlitSurface(screen, NULL, screensave, NULL);
	
	tmprect.w=282;
	tmprect.h=92;
	tmprect.x=(WIDTH/2)-(tmprect.w/2)-1;
	tmprect.y=(HEIGHT/2)+31;
	SDL_FillRect(screen,&tmprect,SDL_MapRGB(screen->format, 0xC0, 0xC0, 0xC0));

	tmprect.w-=2; tmprect.h-=2;
	tmprect.x++; tmprect.y++;
	SDL_FillRect(screen,&tmprect,SDL_MapRGB(screen->format, 0x00, 0x40, 0x80));

	if (highest)
		puttext(tmprect.x+7,tmprect.y+7,2,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF),"NEW TOP SCORE!");
	else
		puttext(tmprect.x+7,tmprect.y+7,2,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF),"NEW HIGH SCORE.");
	puttext(tmprect.x+7,tmprect.y+7+16,2,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF),"ENTER YOUR NAME:");
	
	namerect.w=268;
	namerect.h=29;
	namerect.x=tmprect.x+6;
	namerect.y=tmprect.y+7+32;
	
	SDL_FillRect(screen,&namerect,SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
	puttext(namerect.x+4,namerect.y+5,4,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF),username);

	okayrect.x=tmprect.x+tmprect.w-32;
	okayrect.y=tmprect.y+7+64;
	okayrect.h=CHARHEIGHT*2+3;
	okayrect.w=CHARWIDTH*2*2+1;

	//SDL_FillRect(screen,&okayrect,SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
	puttext(okayrect.x+3,okayrect.y+3,2,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF),"OK");


	SDL_UpdateRect(screen,0,0,0,0);	


	SDL_EnableUNICODE(1);
	
	clean();
	do
	{
		SDL_WaitEvent(NULL); // no new CPU cooler needed. :)
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				done=true;
				quit=true;
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{  // fix -- make this left button only
				if (event.button.x>=okayrect.x
				 && event.button.y>=okayrect.y
				 && event.button.x<okayrect.x+okayrect.w
				 && event.button.y<okayrect.y+okayrect.h)
				okaypressed=true;
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				if (okaypressed
				 && event.button.x>=okayrect.x
				 && event.button.y>=okayrect.y
				 && event.button.x<okayrect.x+okayrect.w
				 && event.button.y<okayrect.y+okayrect.h)
				{
					if (username[0]!='\0')
					{
						done=true;
					}
					//FIX: else beep
				}
				else if (okayglow)
				{
					okayglow=false;
					SDL_FillRect(screen,&okayrect,SDL_MapRGB(screen->format, 0x00, 0x40, 0x80));
					puttext(okayrect.x+3,okayrect.y+3,2,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF),"OK");
					soil(okayrect);
				}
				okaypressed=false;
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				if (event.motion.x>=okayrect.x
				 && event.motion.y>=okayrect.y
				 && event.motion.x<okayrect.x+okayrect.w
				 && event.motion.y<okayrect.y+okayrect.h)
				 {
					if (!okayglow)
					{
						okayglow=true;
						SDL_FillRect(screen,&okayrect,SDL_MapRGB(screen->format, 0xF0, 0xF0, 0xF0));
						puttext(okayrect.x+3,okayrect.y+3,2,SDL_MapRGB(screen->format, 0x00, 0x40, 0x80),"OK");
						soil(okayrect);
					}
					
				}
				else
				{
					if (okayglow && !okaypressed)
					{
						okayglow=false;
						SDL_FillRect(screen,&okayrect,SDL_MapRGB(screen->format, 0x00, 0x40, 0x80));
						puttext(okayrect.x+3,okayrect.y+3,2,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF),"OK");
						soil(okayrect);
					}
				}
			
			}
			else if (event.type==SDL_KEYDOWN)
			{

				if (event.key.keysym.sym==SDLK_BACKSPACE ||
				    event.key.keysym.sym==SDLK_DELETE ||
				    event.key.keysym.sym==SDLK_LEFT)
				{
					if (clearname)
					{
						insertionpoint=0;
						username[0]='\0'; // null out username
					}
					else if (insertionpoint>0)
					{
						insertionpoint--;	
						username[insertionpoint]='\0';
					}
					//FIX: else beep
					clearname=false;
				}	
				else if (event.key.keysym.sym==SDLK_RETURN || event.key.keysym.sym==SDLK_KP_ENTER)
				{
					if (username[0]!='\0')
					{
						done=true;
					}
					//FIX: else beep
					clearname=false;
				}
				else if (event.key.keysym.sym==SDLK_ESCAPE || event.key.keysym.sym==SDLK_CLEAR)
				{
					insertionpoint=0;
					username[0]='\0'; // null out username
					clearname=false;
				}
				else if ((event.key.keysym.unicode & 0xFF80) == 0) // make sure it's ascii
				{
					keypressed=event.key.keysym.unicode & 0x7F;
					if (keypressed==32) keypressed=95;
					if (keypressed>32 && keypressed<127)
					{
						if (clearname)
						{
							insertionpoint=0;
							clearname=false;
						}
						if (insertionpoint<12)
						{
							username[insertionpoint]=keypressed;
							username[insertionpoint+1]='\0';
							insertionpoint++;
						}
						//FIX: else beep
					}			
				}
				SDL_FillRect(screen,&namerect,SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
				puttext(namerect.x+4,namerect.y+5,4,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF),username);
				soil(namerect);	
			}
		}
		clean();
	} while (!done);

	// and restore the background
	SDL_BlitSurface(screensave, NULL, screen, NULL);
	SDL_UpdateRect(screen,0,0,0,0);	
	
	SDL_EnableUNICODE(0);	
	return(quit);
}

#define MAINMENULENGTH 5
#if 0
int nRelocatedGlobals = 0;
#endif
int popupmenu()
{
	printf("popupmenu\n");
	SDL_Rect menurect;

	char * mainmenu[MAINMENULENGTH] = { "NEW GAME", "OPTIONS", "HIGH SCORES", "HELP","QUIT" };
	int (*mainmenufunctions[MAINMENULENGTH])(char *) = { &menuitem_newgame, &menuitem_options, &menuitem_highscores, &menuitem_help, &menuitem_quit };
#if 0
	if( nRelocatedGlobals == 0 )
        {
                nl_relocdata((unsigned*)mainmenu, 5);
                nl_relocdata((unsigned*)mainmenufunctions, 5);
                nRelocatedGlobals = 1;
        }
#endif
	
	menurect.w=126;
	menurect.h=5+(MAINMENULENGTH*(CHARHEIGHT*2+3));
	menurect.x=BORDERRIGHT-menurect.w+5;
	menurect.y=BORDERBOTTOM-menurect.h+9;

	return menuhandler(menurect, MAINMENULENGTH, mainmenu, mainmenufunctions, 0);

}

int menuitem_newgame(char * notused)
{
	return POPUPMENUNEWGAME;
}


int menuitem_options(char * notused)
{
	SDL_Event event;
	
	int rc=popupoptionsmenu();

	if (rc!=POPUPMENUEXITMENU)
	{
		return rc;
	}
	else
	{
		// add a fake event so "Options" gets un-highlighted if need be.
		SDL_GetMouseState((int *)&event.motion.x,(int *)&event.motion.y);
		event.type = SDL_MOUSEMOTION;
		SDL_PushEvent(&event);
		return POPUPMENUDONOTHING;
	}
}


int menuitem_highscores(char * notused)
{
	int rc;
	SDL_Event event;
	rc=popuphighscores();
	// add a fake event so menuitem gets un-highlighted if need be.
	SDL_GetMouseState((int *)&event.motion.x,(int *)&event.motion.y);
	event.type = SDL_MOUSEMOTION;
	SDL_PushEvent(&event);
	return rc;
}


int menuitem_help(char * notused)
{
	int rc;
	SDL_Event event;
	rc=popuphelp();
	// add a fake event so menuitem gets un-highlighted if need be.
	SDL_GetMouseState((int *)&event.motion.x,(int *)&event.motion.y);
	event.type = SDL_MOUSEMOTION;
	SDL_PushEvent(&event);

	return rc;
}


int menuitem_quit(char * notused)
{
	return POPUPMENUQUITGAME;
}

#ifndef HIDEFULLSCREEN // FIX -- put back fullscreen
  #define OPTIONSMENULENGTH 4
#else
  #define OPTIONSMENULENGTH 3
#endif

int popupoptionsmenu()
{
	SDL_Rect menurect;
	GameDifficultyType originaldifficulty=options.difficulty;
	int rc;

	// FIX -- Themes!
 	//char * optionsmenu[OPTIONSMENULENGTH] = { "SOUND", "AUTO PAUSE", "FULL SCREEN", "DIFFICULTY", "THEME" };
	//int (*optionsmenufunctions[OPTIONSMENULENGTH])(char *) = { &menuitem_sound, &menuitem_autopause, &menuitem_fullscreen, &menuitem_difficulty, &menuitem_theme };
	
#ifndef HIDEFULLSCREEN // FIX -- put back fullscreen
	char * optionsmenu[OPTIONSMENULENGTH] = { "SOUND", "AUTO PAUSE", "FULL SCREEN", "DIFFICULTY" };
	int (*optionsmenufunctions[OPTIONSMENULENGTH])(char *) = { &menuitem_sound, &menuitem_autopause, &menuitem_fullscreen, &menuitem_difficulty };
#else	
	char * optionsmenu[OPTIONSMENULENGTH] = { "SOUND", "AUTO PAUSE", "DIFFICULTY" };
	int (*optionsmenufunctions[OPTIONSMENULENGTH])(char *) = { &menuitem_sound, &menuitem_autopause, &menuitem_difficulty };
#endif
	
	menurect.w=212;
	menurect.h=5+(OPTIONSMENULENGTH*(CHARHEIGHT*2+3));
	menurect.x=BORDERRIGHT-menurect.w+10;
	menurect.y=BORDERBOTTOM-menurect.h+9-((CHARHEIGHT*2+4)*4)-2;

	
	rc=menuhandler(menurect, OPTIONSMENULENGTH, optionsmenu, optionsmenufunctions, 69);
	
	// FIX -- prompt if user really wants to do this mid-game
	if (rc==POPUPMENUEXITMENU && options.difficulty != originaldifficulty) rc=POPUPMENUNEWGAME;

	//writeoptions(); // probably no need to do this until exit
	
	return rc;
}


int menuitem_sound(char * val)
{
#ifndef _TINSPIRE
	if (strlen(val)==0)
	{
		if (soundsystemworks)
		{
			if (options.sound==SOUNDON)
				strncpy(val,"on",MAXMENUVALUELENGTH);
			else
				strncpy(val,"off",MAXMENUVALUELENGTH);
		}
		else
		{
			strncpy(val,"n/a",MAXMENUVALUELENGTH);
		}
		return POPUPMENUDONOTHING;
	}
	else
	{
		if (!soundsystemworks) return POPUPMENUDONOTHING;
		if (options.sound==SOUNDON)
		{
			options.sound=SOUNDOFF;
			strncpy(val,"off",MAXMENUVALUELENGTH);
		}
		else
		{
			options.sound=SOUNDON;	
			strncpy(val,"on",MAXMENUVALUELENGTH);
		}
		return POPUPMENUREDRAWME;
	}
#endif
}

int menuitem_autopause(char * val)
{
	if (strlen(val)==0)
	{
		if (options.autopause==AUTOPAUSEON)
			strncpy(val,"on",MAXMENUVALUELENGTH);
		else
			strncpy(val,"off",MAXMENUVALUELENGTH);
		return POPUPMENUDONOTHING;
	}
	else
	{
		if (options.autopause==AUTOPAUSEON)
		{
			options.autopause=AUTOPAUSEOFF;
			strncpy(val,"off",MAXMENUVALUELENGTH);
		}
		else
		{
			options.autopause=AUTOPAUSEON;	
			strncpy(val,"on",MAXMENUVALUELENGTH);
		}
		return POPUPMENUREDRAWME;
	}
}

int menuitem_fullscreen(char * val)
{
	if (strlen(val)==0)
	{
		switch (options.fullscreen)
		{
			case FULLSCREENOFF:
				strncpy(val,"off",MAXMENUVALUELENGTH);
			break;
			case FULLSCREENON:
				strncpy(val,"on",MAXMENUVALUELENGTH);
			break;
			case FULLSCREENALWAYS:
				strncpy(val,"always",MAXMENUVALUELENGTH);
			break;
		}
		return POPUPMENUDONOTHING;
	}
	else
	{
		switch (options.fullscreen)
		{
			case FULLSCREENOFF:
				options.fullscreen=FULLSCREENON;
				strncpy(val,"on",MAXMENUVALUELENGTH);
				makefullscreen();
			break;
			case FULLSCREENON:
				options.fullscreen=FULLSCREENALWAYS;
				strncpy(val,"always",MAXMENUVALUELENGTH);
			break;
			case FULLSCREENALWAYS:
				options.fullscreen=FULLSCREENOFF;
				strncpy(val,"off",MAXMENUVALUELENGTH);
				makewindowed();
			break;
		}
		return POPUPMENUREDRAWME;
	}
}


int menuitem_difficulty(char * val)
{
	if (strlen(val)==0)
	{
		switch (options.difficulty)
		{
			case NORMAL:
				strncpy(val,"normal",MAXMENUVALUELENGTH);
			break;
			case EASY:
				strncpy(val,"easy",MAXMENUVALUELENGTH);
			break;
			case HARD:
				strncpy(val,"hard",MAXMENUVALUELENGTH);
			break;
		}
		return POPUPMENUDONOTHING;
	}
	else
	{
		switch (options.difficulty)
		{
			case NORMAL:
				options.difficulty=EASY;
				strncpy(val,"easy",MAXMENUVALUELENGTH);
			break;
			case EASY:
				options.difficulty=HARD;
				strncpy(val,"hard",MAXMENUVALUELENGTH);
			break;
			case HARD:
				options.difficulty=NORMAL;
				strncpy(val,"normal",MAXMENUVALUELENGTH);
			break;
		}
		return POPUPMENUREDRAWME;
	}
}

int menuitem_theme(char * val)
{
	if (strlen(val)==0)
		strncpy(val,"linux",MAXMENUVALUELENGTH);


	return POPUPMENUDONOTHING;
}


int popuphighscores()
{
	SDL_Rect scorelistrect,borderrect;
	SDL_Surface * scorelistsave;
	int quit=POPUPMENUDONOTHING;
	int i;
	char buf[30]; // plenty big. :)

	scorelistrect.w=PLAYWIDTH-(BLOCKWIDTH*11);
	scorelistrect.h=PLAYHEIGHT-(BLOCKHEIGHT*4);
	scorelistrect.x=BORDERLEFT+PLAYWIDTH/2-scorelistrect.w/2+BLOCKWIDTH*4;
	scorelistrect.y=BORDERTOP+PLAYHEIGHT/2-scorelistrect.h/2+BLOCKHEIGHT/2;
	
	borderrect.w=scorelistrect.w+2;
	borderrect.h=scorelistrect.h+2;
	borderrect.x=scorelistrect.x-1;
	borderrect.y=scorelistrect.y-1;

#ifdef _TINSPIRE
	scorelistsave = SDL_CreateRGBSurface(SDL_SWSURFACE,borderrect.w,borderrect.h,16,0,0,0,0);
#else
	scorelistsave = SDL_CreateRGBSurface(SDL_SWSURFACE,borderrect.w,borderrect.h,32,0,0,0,0);
#endif
	SDL_BlitSurface(screen, &borderrect, scorelistsave, NULL);

	SDL_FillRect(screen,&borderrect,SDL_MapRGB(screen->format,  0xC0, 0xC0, 0xC0));
	SDL_FillRect(screen,&scorelistrect,SDL_MapRGB(screen->format,  0x00, 0x40, 0x80));

	puttext(scorelistrect.x+(scorelistrect.w/2-(CHARWIDTH*2*9)),scorelistrect.y+BLOCKHEIGHT,4,SDL_MapRGB(screen->format, 0xC0, 0x00, 0x00),"HIGH SCORES");

	// just in case they've changed...
	readhiscores();

	for (i=0;i<HISCORENUM;i++)
	{
		snprintf(buf,4,"%d.",i+1);
		puttext(scorelistrect.x+BLOCKWIDTH,scorelistrect.y+45+i*(CHARHEIGHT*2+5),2,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF),buf);		
		puttext(scorelistrect.x+BLOCKWIDTH*4,scorelistrect.y+45+i*(CHARHEIGHT*2+5),2,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF),hiscorename[i]);		
		snprintf(buf,30,"%ld",hiscoreval[i]);		
		puttext(scorelistrect.x+scorelistrect.w-(BLOCKWIDTH*5),scorelistrect.y+45+i*(CHARHEIGHT*2+5),2,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF),buf);
	}
	
	soil(borderrect);	

	clean();
		
	if (waitforuser()) quit=POPUPMENUQUITGAME;

	SDL_BlitSurface(scorelistsave, NULL, screen, &borderrect);
	soil(borderrect);
	clean();

	SDL_FreeSurface(scorelistsave);	 	

	return(quit);
}

int popuphelp()
{
	SDL_Rect helprect,borderrect;
	SDL_Surface * helpsave;
	int quit=POPUPMENUDONOTHING;
	int i=0;
	char buf[80];

	char helptext[13][50]={
	     "Ok, so there's a bunch of penguins on an",
	     "iceberg in Antarctica. You must catch them",
	     "so they can be shipped to Finland. The",
	     "smaller the area in which they're caught,",
	     "the lower the shipping fees and the higher",
	     "your score - but don't take too long: the",
	     "clock is ticking. Once 80% of the 'berg is",
	     "cleared, it's on to the next shipment.",
	     "",
	     "The left mouse button starts lines; right",
	     "button toggles direction. You'll catch on.",
	     "",
	     "Check the README file for more info."
	      };

	helprect.w=PLAYWIDTH-(BLOCKWIDTH*2)+2;
	helprect.h=PLAYHEIGHT-(BLOCKHEIGHT*3);
	helprect.x=BORDERLEFT+PLAYWIDTH/2-helprect.w/2;
	helprect.y=BORDERTOP+PLAYHEIGHT/2-helprect.h/2;
	
	borderrect.w=helprect.w+2;
	borderrect.h=helprect.h+2;
	borderrect.x=helprect.x-1;
	borderrect.y=helprect.y-1;

#ifdef _TINSIRE
	helpsave = SDL_CreateRGBSurface(SDL_SWSURFACE,borderrect.w,borderrect.h,16,0,0,0,0);
#else
	helpsave = SDL_CreateRGBSurface(SDL_SWSURFACE,borderrect.w,borderrect.h,32,0,0,0,0);
#endif
	SDL_BlitSurface(screen, &borderrect, helpsave, NULL);

	SDL_FillRect(screen,&borderrect,SDL_MapRGB(screen->format,  0xC0, 0xC0, 0xC0));
	SDL_FillRect(screen,&helprect,SDL_MapRGB(screen->format,  0x00, 0x40, 0x80));
	
	for (i=0;i<13;i++)
	{
		puttext(helprect.x+BLOCKWIDTH/2,helprect.y+BLOCKHEIGHT/2+i*(CHARHEIGHT*2+4),2,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF),helptext[i]);		
	}
	snprintf(buf,80,"v%d.%d.%d   %s",VERMAJOR,VERMINOR,VERSUB,"Copyright (c) 2000-2001 Matthew Miller. Released under the GPL.");
	puttext(helprect.x+BLOCKWIDTH/2,helprect.y+helprect.h-CHARHEIGHT*3,1,SDL_MapRGB(screen->format, 0xC0, 0xC0, 0xC0),buf);		
	puttext(helprect.x+BLOCKWIDTH/2,helprect.y+helprect.h-CHARHEIGHT*1-3,1,SDL_MapRGB(screen->format, 0xC0, 0xC0, 0xC0),"Thanks to my wonderful wife Karen for inspiration (and for patience)!");		
	
	soil(borderrect);	

	clean();

	if (waitforuser()) quit=POPUPMENUQUITGAME;

	SDL_BlitSurface(helpsave, NULL, screen, &borderrect);
	soil(borderrect);
	clean();

	SDL_FreeSurface(helpsave);	 	

	return(quit);
}


/* Wait for the user to hit a key or button or quit; returns true if quit */
int waitforuser()
{
	int done=false;
	int quit=false;
	SDL_Event event;
	
	do
	{
		SDL_WaitEvent(NULL);
		while(SDL_PollEvent(&event))
		{
			if ( event.type == SDL_QUIT)
			{
				done=true;
				quit=true;
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				done=true;
			}
			else if (event.type == SDL_KEYUP) // fix -- add keyboard support
			{
				switch (event.key.keysym.sym)
                                {
					default:
					break;

                                        case SDLK_ESCAPE:
                                                done = true;
                                        break;
				}
			}
		}
	} while (!done);
	return(quit);		
}




/* displays a menu; takes a rect for position and size, the length of the
 * menu, an array of menu items, an array of pointers to functions for
 * each menu item (functions should return type MenuReturnType
 * indicating what to do when the item is clicked) and a flag telling whether
 * menu items are option/value pairs.
 */
int menuhandler(SDL_Rect menurect, int menulength, char ** menuitems, int (**menufunctions)(char *), int menuvaluetextwidth)
{
	SDL_Rect borderrect;
	SDL_Rect menuitemrect[MAXMENUITEMS];
	char menuvalues[MAXMENUITEMS][MAXMENUVALUELENGTH];
	int menuitemglow=-1;
	int menuitempressed=false;
	SDL_Event event;
	SDL_Surface * menusave;
	int quit=POPUPMENUDONOTHING;

	int i;
	
	borderrect.w=menurect.w+2;
	borderrect.h=menurect.h+2;
	borderrect.x=menurect.x-1;
	borderrect.y=menurect.y-1;

#ifdef _TINSPIRE
	menusave = SDL_CreateRGBSurface(SDL_SWSURFACE,borderrect.w,borderrect.h,16,0,0,0,0);
#else
	menusave = SDL_CreateRGBSurface(SDL_SWSURFACE,borderrect.w,borderrect.h,32,0,0,0,0);
#endif
	SDL_BlitSurface(screen, &borderrect, menusave, NULL);

	SDL_FillRect(screen,&borderrect,SDL_MapRGB(screen->format,  0xC0, 0xC0, 0xC0));
	SDL_FillRect(screen,&menurect,SDL_MapRGB(screen->format,  0x00, 0x40, 0x80));
	
	for (i=0;i<menulength;i++)
	{
		menuitemrect[i].x=menurect.x;
		menuitemrect[i].y=menurect.y+2+(i*(CHARHEIGHT*2+3));
		menuitemrect[i].w=menurect.w;
		menuitemrect[i].h=(CHARHEIGHT*2)+3;
		puttext(menuitemrect[i].x+5,menuitemrect[i].y+3,2,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF),menuitems[i]);

		strncpy(menuvalues[i],"",MAXMENUVALUELENGTH);
		if (menuvaluetextwidth)
		{
			if (menufunctions!=NULL && menufunctions[i]!=NULL)
				(*menufunctions[i])(menuvalues[i]);
			puttext(menuitemrect[i].x+menuitemrect[i].w-menuvaluetextwidth,menuitemrect[i].y+3,2,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF),menuvalues[i]);
		}
	}		
	
	soil(borderrect);	
	
	clean();
	
	
	// find options
	
	do
	{
		SDL_WaitEvent(NULL); // no new CPU cooler needed. :)
		while (SDL_PollEvent(&event))
		{
			if (event.type==SDL_KEYDOWN) // FIX -- add keyboard support
                        {
                                switch (event.key.keysym.sym)
                                {
					default:
					break;

                                        case SDLK_ESCAPE:
                                                quit=POPUPMENUEXITMENU;
                                        break;

                                }
                        }

			if (event.type == SDL_QUIT)
			{
				quit=POPUPMENUQUITGAME;
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{  // fix -- make this left button only
				if (event.button.x>menurect.x &&
				    event.button.y>menurect.y &&
				    event.button.x<menurect.x + menurect.w &&
				    event.button.y<menurect.y + menurect.h)
				{
					menuitempressed=true;
				}
				else
				{
					quit=POPUPMENUEXITMENU;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				if (menuitempressed)
				{
					// in area, button was down
					if (event.motion.x>menurect.x &&
					    event.motion.y>(menurect.y + 2) &&
					    event.motion.x<menurect.x + menurect.w &&
					    event.motion.y<menurect.y + menurect.h - 2)
					{
						if (menufunctions!=NULL && menufunctions[menuitemglow]!=NULL)
							quit=(*menufunctions[menuitemglow])(menuvalues[menuitemglow]);
					}					
					menuitempressed=false;
				}
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				// are we in the menu area?
				if (event.motion.x>menurect.x &&
				    event.motion.y>(menurect.y + 2) &&
				    event.motion.x<menurect.x + menurect.w &&
				    event.motion.y<menurect.y + menurect.h - 2)
				{
					// FIX -- should check for overflow, just to be sure.
					menuitemglow=(event.motion.y-(menurect.y+3))/(CHARHEIGHT*2+3);
					// FIX -- this is inefficient.
					SDL_FillRect(screen,&menurect,SDL_MapRGB(screen->format,  0x00, 0x40, 0x80));
					for (i=0;i<menulength;i++)
					{
						if (i == menuitemglow)
						{
							SDL_FillRect(screen,&menuitemrect[i],SDL_MapRGB(screen->format,  0xF0, 0xF0, 0xF0));
							puttext(menuitemrect[i].x+5,menuitemrect[i].y+3,2,SDL_MapRGB(screen->format, 0x00, 0x40, 0x80),menuitems[i]);
							if (menuvaluetextwidth) puttext(menuitemrect[i].x+menuitemrect[i].w-menuvaluetextwidth,menuitemrect[i].y+3,2,SDL_MapRGB(screen->format, 0x00, 0x40, 0x80),menuvalues[i]);
						}
						else
						{
							puttext(menuitemrect[i].x+5,menuitemrect[i].y+3,2,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF),menuitems[i]);
							if (menuvaluetextwidth) puttext(menuitemrect[i].x+menuitemrect[i].w-menuvaluetextwidth,menuitemrect[i].y+3,2,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF),menuvalues[i]);
						}
					}		
					soil(menurect);
				}
				else
				{
					if (menuitemglow != -1 && !menuitempressed)
					{
						menuitemglow=-1;
						SDL_FillRect(screen,&menurect,SDL_MapRGB(screen->format,  0x00, 0x40, 0x80));
						for (i=0;i<menulength;i++)
						{
							puttext(menuitemrect[i].x+5,menuitemrect[i].y+3,2,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF),menuitems[i]);
							if (menuvaluetextwidth) puttext(menuitemrect[i].x+menuitemrect[i].w-menuvaluetextwidth,menuitemrect[i].y+3,2,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF),menuvalues[i]);
						}
						soil(menurect);
					}
				}
			
			}
		}
		
		if (quit==POPUPMENUREDRAWME)
		{	
			quit=POPUPMENUDONOTHING;
			// add a fake event so "Options" gets un-highlighted if need be.
			SDL_GetMouseState((int *)&event.motion.x,(int *)&event.motion.y);
			event.type = SDL_MOUSEMOTION;
			SDL_PushEvent(&event);
		}
		
		clean();
	} while (quit==POPUPMENUDONOTHING);


	// and restore the background
	if (quit != POPUPMENUQUITGAME) // of course, if we're quitting, don't waste time
	{
		SDL_BlitSurface(menusave, NULL, screen, &borderrect);
		soil(borderrect);
		clean();
	}

	SDL_FreeSurface(menusave);	
	

	// a fake event update things that depend on mouse position
	SDL_GetMouseState((int *)&event.motion.x,(int *)&event.motion.y);
	event.type = SDL_MOUSEMOTION;
	printf("about to push a motion\n");
	SDL_PushEvent(&event);
	printf("did that\n");

	return(quit);

}
