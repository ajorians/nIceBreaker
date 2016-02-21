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
#endif

#include "icebreaker.h"
#include "cursor.h"
#include "penguin.h"
#include "line.h"
#include "grid.h"
#include "laundry.h"
#include "sound.h"
#include "globals.h"
#include "level.h"
#include "status.h"
#include "text.h"
#include "dialog.h"
#include "options.h"

#define min(a,b) (a < b) ? a : b
#define max(a,b) (a > b) ? a : b

Line line1;
Line line2;

static void setuplevel(void);

void setuplevel()
{
	int x,y;
	int c;
	SDL_Rect tmprect;

	setcursor(CURSORARROW);
	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format, 0x00, 0x40, 0x80));
	
	tmprect.x=BORDERLEFT; tmprect.y=BORDERTOP;
	tmprect.w=PLAYWIDTH; tmprect.h=PLAYHEIGHT;
	SDL_FillRect(screen,&tmprect,SDL_MapRGB(screen->format, 0xC0, 0xC0, 0xC0));

	//tmprect.w=BLOCKWIDTH-1; tmprect.h=BLOCKHEIGHT-1;
	//for (tmprect.x=BORDERLEFT;tmprect.x<BORDERRIGHT;tmprect.x+=BLOCKWIDTH)
	//	for (tmprect.y=BORDERTOP;tmprect.y<BORDERBOTTOM;tmprect.y+=BLOCKHEIGHT)
	//		SDL_FillRect(screen,&tmprect,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));

	tmprect.w=BLOCKWIDTH-1; tmprect.h=BLOCKHEIGHT-1;
	for (tmprect.x=BORDERLEFT;tmprect.x<BORDERRIGHT;tmprect.x+=BLOCKWIDTH)
		for (tmprect.y=BORDERTOP;tmprect.y<BORDERBOTTOM;tmprect.y+=BLOCKHEIGHT)
			{
				c = (
#ifdef _TINSPIRE
rand()
#else
random() 
#endif
				% 32)+224;
				SDL_FillRect(screen,&tmprect,SDL_MapRGB(screen->format, c, c, c));
			}
	SDL_BlitSurface(screen, NULL, screensave, NULL);


	
	SDL_UpdateRect(screen,0,0,0,0);

	for (x=0;x<WIDTH;x++)
		for (y=0;y<HEIGHT;y++)
		{
			if (x<BORDERLEFT || x>=BORDERRIGHT || y <BORDERTOP || y>=BORDERBOTTOM) 	
				grid[x][y]='X';
			else
				grid[x][y]=' ';
		}
	

}



LevelExitType playlevel(int level, long oldscore, ScoreSheet * levelscore)
{
	int penguincount=level+1;
	int lives=level+1;

	LevelExitType returncode=ERROR;
	int i,x,y,xdif,ydif;

#ifndef _TINSPIRE
	SDL_Rect menubuttonrect;
	int menubuttonglow=false;
	int menubuttonpressed=false;
#endif
	int paused=false;

	int clear=0;
	int linedoneflag=false;
	LineType linetype;
	float scoremod=1;
	float bonusmod=1;
	
	int tick=0;
	int timepenalty=0;
	int timepenaltyinterval=0;
	
	int domenuflag = false;
	
	int done = false;

#ifdef _TINSPIRE
	int nMouseX = SCREEN_WIDTH/2, nMouseY = SCREEN_HEIGHT/2;
#endif
	
	// FIX -- this is a good candidate for dynamic memory allocation...
	//Penguin flock[penguincount];
	Penguin flock[MAXPENGUINS];
	CursorGraphic cur;

#ifndef _TINSPIRE	
	SDL_Event event;
#else
	SDL_Event event = {0};
#endif

	switch(options.difficulty)
	{
		case NORMAL:
			scoremod=(level+1)/2.0;
			bonusmod=(level+1)/2.0;
			timepenaltyinterval=100;
		break;
		case EASY:
			scoremod=(level+1)/5.0;
			bonusmod=(level+1)/7.0;
			timepenaltyinterval=200;
		break;
		case HARD:
			scoremod=(level+1)/1.75;
			bonusmod=(level+1)/1.5;
			timepenaltyinterval=75;
		break;
		default:
			fprintf(stderr,"Unknown difficulty -- that can't happen!\n");
		break;
	}

	levelscore->basescore=0;
	levelscore->clearbonus=0;
	levelscore->lifebonus=0;
	
	setuplevel();
	
	setcursor(CURSORVERTICAL); linetype=VERTICAL; 

/*	printf("===========================================================\n"
	       "Starting level %d.\n"
	       "Lives: %d\n",
	       level,lives);
*/	       

#ifndef _TINSPIRE
	menubuttonrect.x=WIDTH-(CHARWIDTH*2*4)-MARGINRIGHT-4;
	menubuttonrect.y=BOTTOMSTATUSY;
	menubuttonrect.w=CHARWIDTH*2*4+3;
	menubuttonrect.h=CHARHEIGHT*2+3;
	puttext(menubuttonrect.x+3,menubuttonrect.y+3,2,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF),"MENU");
	soil(menubuttonrect);
#endif

	line1=createline('1',0x00, 0x00, 0x00);
	//line1=createline('1',0x00, 0x40, 0x80);
	line2=createline('2',0xC0, 0x00, 0x40);

	for (i=0;i<penguincount;i++)
	{
		flock[i] = createpenguin();
	}
	cur = createCursor();

	updatestatuslives(lives);
	updatestatuscleared(clear);
	updatestatusscore(oldscore);
	
	updatehiscorebox();	
	clean();

	touchpad_info_t* ptad_info = touchpad_getinfo();
	touchpad_report_t tpad_report;
	int nMouseCenterX = -1, nMouseCenterY = -1;
	
	do 
	{
		int nMouseMove = false, nMouseDown = false, nMouseButton = 0;

		// FIX -- this is way too messy. time to split it up into
		// neat little functions or something. Especially the menubutton stuff.
		while (SDL_PollEvent(&event))
		{
			/*if (event.type == SDL_QUIT)
			{
				lives=0; // is this the right way?
				done = true;
				return QUIT;
			}
			else*/ if (nMouseMove == false && event.type == SDL_KEYDOWN)
			{
				switch(event.key.keysym.sym)
				{
					default:
					break;

					case SDLK_ESCAPE:
						return QUIT;
					break;

                                        case SDLK_LSHIFT:
					case SDLK_RSHIFT:
                                        case SDLK_5:
					case SDLK_f:
						nMouseDown = true;
						nMouseButton = 2;
					break;

					case SDLK_RETURN:
					case SDLK_LCTRL:
					case SDLK_RCTRL:
						nMouseDown = true;
						nMouseButton = 1;
					break;

					case SDLK_LEFT:
                                        case SDLK_4:
						nMouseMove = true;
						nMouseX-=BLOCKWIDTH;
						nMouseX = max(0, nMouseX);
                                                break;

                                        case SDLK_RIGHT:
                                        case SDLK_6:
						nMouseMove = true;
						nMouseX+=BLOCKWIDTH;
						nMouseX = min(nMouseX, SCREEN_WIDTH);
                                                break;

					case SDLK_UP:
                                        case SDLK_8:
						nMouseMove = true;
                                                nMouseY-=BLOCKHEIGHT;
						nMouseY = max(0, nMouseY);
                                                break;

                                        case SDLK_DOWN:
                                        case SDLK_2:
						nMouseMove = true;
                                                nMouseY+=BLOCKHEIGHT;
						nMouseY = min(nMouseY, SCREEN_HEIGHT);
                                                break;

					case SDLK_p:
						paused = !paused;
						break;
				}
			}
		}
		
		if( ptad_info )
                {
                        touchpad_scan(&tpad_report);
                        if( tpad_report.proximity > 0 && !tpad_report.pressed )
                        {
                              int nX = tpad_report.x * SCREEN_WIDTH / 2328;
                              int nY = (1691-tpad_report.y) * SCREEN_HEIGHT / 1691;
                              nX = min(SCREEN_WIDTH, max(0, nX));
                              nY = min(SCREEN_HEIGHT, max(0, nY));
                              if( nMouseCenterX == -1 && nMouseCenterY == -1 ) {
                                     nMouseCenterX = nX;
                                     nMouseCenterY = nY;
                              }
                              else {
                                     int nMX = (nMouseX + (nX-nMouseCenterX));
                                     int nMY = (nMouseY + (nY-nMouseCenterY));
                                     nMX = min(SCREEN_WIDTH, max(0, nMX));
                                     nMY = min(SCREEN_HEIGHT, max(0, nMY));
                                     nMouseMove = true;
                                     nMouseX = nMX, nMouseY = nMY;
                                     nMouseCenterX = nX, nMouseCenterY = nY;

                              }
                        }
                        else
                        {
                                nMouseCenterX = nMouseCenterY = -1;
                        }
                 }

		if( nMouseMove == true )
		{
			//if (grid[event.motion.x][event.motion.y] == ' ' || grid[event.motion.x][event.motion.y] == '*')
			if (nMouseX>BORDERLEFT && nMouseX<BORDERRIGHT && nMouseY>BORDERTOP && nMouseY<BORDERBOTTOM)	
			{
				switch (linetype)
				{
					case HORIZONTAL:
						setcursor(CURSORHORIZONTAL);
					break;
					case VERTICAL:
						setcursor(CURSORVERTICAL);
					break;
				}
				
			}			
			else  // we're somewhere outside of the playing area
			{
				setcursor(CURSORARROW);
			}
		
#ifndef _TINSPIRE	
			if (nMouseX>=menubuttonrect.x && nMouseY>=menubuttonrect.y && nMouseX<menubuttonrect.x+menubuttonrect.w && nMouseY<menubuttonrect.y+menubuttonrect.h)
			{ // over the menu button
				if (!menubuttonglow)
				{
					menubuttonglow=true;
					SDL_FillRect(screen,&menubuttonrect,SDL_MapRGB(screen->format, 0xF0, 0xF0, 0xF0));
					puttext(menubuttonrect.x+3,menubuttonrect.y+3,2,SDL_MapRGB(screen->format, 0x00, 0x40, 0x80),"MENU");
				}

			}
			else 
			{
				if (menubuttonglow && !menubuttonpressed)
				{
					menubuttonglow=false;
					SDL_FillRect(screen,&menubuttonrect,SDL_MapRGB(screen->format, 0x00, 0x40, 0x80));
					puttext(menubuttonrect.x+3,menubuttonrect.y+3,2,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF),"MENU");
				}
			
			}
			soil(menubuttonrect);	
#endif
		}
		else if (nMouseDown == true)
		{
			if (nMouseButton==1) //left
			{
				// in game area?
				if (nMouseX>BORDERLEFT && nMouseX<BORDERRIGHT && nMouseY>BORDERTOP && nMouseY<BORDERBOTTOM)
				{
					x=(((nMouseX-BORDERLEFT)/BLOCKWIDTH ) * BLOCKWIDTH ) +BORDERLEFT;
					y=(((nMouseY-BORDERTOP)/BLOCKHEIGHT) * BLOCKHEIGHT) +BORDERTOP;
					xdif=nMouseX-x; ydif=nMouseY-y;

					if (grid[x][y] == '*' || grid[nMouseX][nMouseY] == '*') 
					{ // a little bit of grace if the player clicks directly on the penguin
#ifndef _TINSPIRE
						playsound(SNDOUCH);
#endif
					}
					else if (grid[x][y] == ' ')
					{

						switch (linetype)
						{
							case VERTICAL:
								if (!line1.on) (ydif<BLOCKHEIGHT/2) ? startline(&line1,UP,x,y) : startline(&line1,UP,x,y+BLOCKHEIGHT);
								if (!line2.on) (ydif<BLOCKHEIGHT/2) ? startline(&line2,DOWN,x,y) : startline(&line2,DOWN,x,y+BLOCKHEIGHT);
							break;
							case HORIZONTAL:
								if (!line1.on) (xdif<BLOCKWIDTH/2) ? startline(&line1,LEFT,x,y) : startline(&line1,LEFT,x+BLOCKWIDTH,y);
								if (!line2.on) (xdif<BLOCKWIDTH/2) ? startline(&line2,RIGHT,x,y) : startline(&line2,RIGHT,x+BLOCKWIDTH,y);
							break;
						}
					
					}
				}
#ifndef _TINSPIRE
				else if (event.button.x>=menubuttonrect.x && event.button.y>=menubuttonrect.y && event.button.x<menubuttonrect.x+menubuttonrect.w && event.button.y<menubuttonrect.y+menubuttonrect.h)
				{
					menubuttonpressed=true;
				}
#endif
					  
			}
			//else if (event.button.button==3) // for testing!
			//{
			//	printwholegrid();  				
			//}
			else //middle or right
			{
				switch (linetype)
				{
					case VERTICAL:
						linetype=HORIZONTAL;
						setcursor(CURSORHORIZONTAL);
						updateCursor(&cur); 
					break;
					case HORIZONTAL:
						linetype=VERTICAL;
						setcursor(CURSORVERTICAL);
						updateCursor(&cur);
					break;
				}
			}
#ifndef _TINSPIRE
		}
		else if (event.type == SDL_MOUSEBUTTONUP)
		{
#endif

#ifndef _TINSPIRE
			if (menubuttonpressed && nMouseX>=menubuttonrect.x && nMouseY>=menubuttonrect.y && nMouseX<menubuttonrect.x+menubuttonrect.w && nMouseY<menubuttonrect.y+menubuttonrect.h)
			{
				menubuttonglow=true;
				
				SDL_FillRect(screen,&menubuttonrect,SDL_MapRGB(screen->format, 0xF0, 0xF0, 0xF0));
				puttext(menubuttonrect.x+3,menubuttonrect.y+3,2,SDL_MapRGB(screen->format, 0x00, 0x40, 0x80),"MENU");
				soil(menubuttonrect);

				domenuflag=true;
			}
			else if (menubuttonglow && menubuttonpressed)
			{
				menubuttonglow=false;
				SDL_FillRect(screen,&menubuttonrect,SDL_MapRGB(screen->format, 0x00, 0x40, 0x80));
				puttext(menubuttonrect.x+3,menubuttonrect.y+3,2,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF),"MENU");
				soil(menubuttonrect);	
			}
			menubuttonpressed=false;
#endif
		}
#ifndef _TINSPIRE
		else if (event.type == SDL_ACTIVEEVENT && ((options.autopause == AUTOPAUSEON) || (event.active.state & SDL_APPACTIVE)))
		{   
			if (event.active.gain==0)
			{ // iconified / lost focus
				paused=true;
			}
			else // event.active.gain==1
			{ // restored /got focus
				paused=false;
			}
		}
#endif
			// FIX -- add keyboard support, handle other events.
		if (paused) 
		{
			printf("paused\n");
			SDL_WaitEvent(NULL);
			continue;
		}
		
		
		// move split-lines
		if (line1.on)
		{
			// kludgy crap to implement speed = 1 1/2
			linedoneflag=moveline(&line1);
			if (!linedoneflag && line1.speedslower)
			{
				line1.speedslower=false;
				linedoneflag=moveline(&line1);
			}
			else
			{
				line1.speedslower=true;
			}
				
			if (linedoneflag)
			{
				clear=(100-(countcleared()*100/(PLAYWIDTH*PLAYHEIGHT)));
				levelscore->basescore=(int)(clear*scoremod)-timepenalty;
				tick=0; // reset this to keep score from seeming to flicker around a lot. and to be nice. :)
				
				updatestatuscleared(clear);
				updatestatusscore(oldscore+levelscore->basescore);
				//printf("Cleared: %d%%\n",clear);				
			}

			if (line1.dead) 
			{
#ifndef _TINSPIRE
				playsound(SNDBREAK);
#endif
				killline(&line1);
				lives--;
				if (lives<0) lives=0;
				updatestatuslives(lives);
			}
		}
		if (line2.on)
		{
			linedoneflag=moveline(&line2);
			if (!linedoneflag && line2.speedslower)
			{
				line2.speedslower=false;
				linedoneflag=moveline(&line2);
			}
			else
			{
				line2.speedslower=true;
			}

			if (linedoneflag)
			{
				clear=(100-(countcleared()*100/(PLAYWIDTH*PLAYHEIGHT)));
				levelscore->basescore=(int)(clear*scoremod)-timepenalty;
				tick=0; // reset this to keep score from seeming to flicker around a lot. and to be nice. :)

				updatestatuscleared(clear);
				updatestatusscore(oldscore+levelscore->basescore);
			}

			if (line2.dead)
			{
#ifndef _TINSPIRE 
				playsound(SNDBREAK);
#endif
				killline(&line2);
				lives--;
				if (lives<0) lives=0;
				updatestatuslives(lives);
				
			}
		}
		
		// move (and get old background)
		for (i=0;i<penguincount;i++)
		{
			
			soil(flock[i].geom); // mark the penguin's old position as dirty
			movepenguin(&flock[i]);
		
			soil(flock[i].geom); // mark the penguin's new position as dirty too (it will be soon...)
			savebehindpenguin(&flock[i]);
		}
		soil(cur.geom);
		moveCursor(&cur, nMouseX, nMouseY);
		soil(cur.geom);
		saveBehindCursor(&cur);
		

		// actually draw
		for (i=0;i<penguincount;i++)
		{
			drawpenguin(&flock[i]);
		}
		drawCursor(&cur);

#ifndef _TINSPIRE
		if (domenuflag)
		{
			switch (popupmenu())
			{
				case POPUPMENUQUITGAME:
					printf("Quit game\n");
					lives=0; // is this the right way?
					done = true;
					return QUIT;
				break;
				case POPUPMENUNEWGAME:
					printf("new game\n");
					// hmmm... maybe this could be done better
					done = true;
					returncode=DEAD;
				break;
			}
			
			domenuflag=false;
		}
#endif

		
		// update clock
		tick++;
		if (tick>timepenaltyinterval)
		{
			tick=0;
			if (levelscore->basescore>0)
			{
				timepenalty++;
//				levelscore->basescore--;
				updatestatusscore(oldscore+levelscore->basescore);
			}
		}

		// update screen
		clean();


		// clear for next update
		for (i=0;i<penguincount;i++)
		{
			erasepenguin(&flock[i]);
		}
		eraseCursor(&cur);
		
		if (lives<=0) // game over
		{
			done = true;
			returncode=DEAD;
		} 
		else if (!line1.on && !line2.on && clear>=PERCENTREQUIRED) // success!
		{ 
			done = true;
			levelscore->basescore=(int)(clear*scoremod)-timepenalty;
			returncode=PASS;


			levelscore->clearbonus=0;
			// bonuses for clearing lots
			if (clear>PERCENTBONUS)
				levelscore->clearbonus+=(int)((clear-PERCENTBONUS)*bonusmod);
			if (clear>PERCENTEXTRABONUS)
				levelscore->clearbonus+=(int)((clear-PERCENTEXTRABONUS)*(clear-PERCENTEXTRABONUS)*bonusmod);
				
			// bonuses for leftover lives
			levelscore->lifebonus=(int)((lives-1)*3*bonusmod);
		}


		//printboard();
		
		// oh, if only we could sleep for less than 10ms on 
		// intel. too bad alpha systems suck so much in other
		// ways -- they can sleep with 1ms resolution.
		// (we could on intel with nanosleep, if we were suid root...)
		SDL_Delay(10);
		
	} while (!done);

	// make sure visible penguins are actually in the screen memory.
	for (i=0;i<penguincount;i++)
	{
		drawpenguin(&flock[i]);
	}
	drawCursor(&cur);

	clean();
	return returncode;
}

