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
#include "globals.h"
#include "line.h"
#include "level.h"
#include "text.h"
#include "cursor.h"
#include "laundry.h"
#include "hiscore.h"
#include "dialog.h"

// FIX -- it'd be better to draw text once and then scroll that graphic,
// of course.

static int scrolltext(char * firsttext, int firstx, Uint32 firstcolor, char * secondtext, int secondx, Uint32 secondcolor);

int intermission(ScoreSheet * levelscore, int nextlevel)
{
	printf("intermission\n");
	return false;
	int done=false; int quit=false;
	SDL_Event event;
	SDL_Rect scorerect, bonusrect;
	char scoretext[30];
	char bonustext[30];

	snprintf(scoretext,30,"SCORE: %d",levelscore->basescore);
	snprintf(bonustext,30, "BONUS: %d",levelscore->clearbonus + levelscore->lifebonus);

	// FIX -- play some truimphant sound.
	
	// clear any pending events
	SDL_Delay(10); // needed? probably not.
#ifndef _TINSPIRE
	while (SDL_PollEvent(&event)) if (event.type == SDL_QUIT) { done=true; quit=true; }
#endif

	// get background
	SDL_BlitSurface(screen, NULL, screensave, NULL);
	
	setcursor(CURSORCLICK);

	scorerect.x=BORDERLEFT+PLAYWIDTH/2-80;
	//scorerect.y=HEIGHT-CHARHEIGHT*4;
	scorerect.y=HEIGHT-CHARHEIGHT*5;
	scorerect.h=CHARHEIGHT*4;
	scorerect.w=CHARWIDTH*4*12;
	
	bonusrect.x=BORDERLEFT+PLAYWIDTH/2-80;
	bonusrect.y=HEIGHT-CHARHEIGHT*4;
	bonusrect.h=CHARHEIGHT*4;
	bonusrect.w=CHARWIDTH*4*12;
	
	// wait for click, scroll score
	while (!done) 
	{
		while(SDL_PollEvent(&event));
		{
			if (event.type == SDL_QUIT)
			{
				done=true; quit=true;
			}
			else if (event.type==SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button==1)
				{
						done=true;
						//printf("Click\n");
				}
			}
		}
		
		
		if (scorerect.y>(HEIGHT/2-30))
		{
			scorerect.y--; 
			puttext(scorerect.x,scorerect.y,4,SDL_MapRGB(screen->format, 0x00, 0x00, 0x00),scoretext);
			soil(scorerect);
			clean();		
			SDL_BlitSurface(screensave,&scorerect, screen, &scorerect);
			//soil(scorerect);
		}
		else if (scorerect.y==(HEIGHT/2-30))
		{
			// un-clean. :)
			scorerect.y--; 
			puttext(scorerect.x,scorerect.y,4,SDL_MapRGB(screen->format, 0x00, 0x00, 0x00),scoretext);			
			soil(scorerect);
			clean();
		}
		else if (bonusrect.y>(HEIGHT/2))
		{
			bonusrect.y--;
			puttext(bonusrect.x,bonusrect.y,4,SDL_MapRGB(screen->format, 0xC0, 0x00, 0x40),bonustext);
			soil(bonusrect);
			clean();
			SDL_BlitSurface(screensave,&bonusrect, screen, &bonusrect);
			//soil(bonusrect);
		}	
		else if (bonusrect.y==(HEIGHT/2))
		{
			// un-clean
			bonusrect.y--;
			puttext(bonusrect.x,bonusrect.y,4,SDL_MapRGB(screen->format, 0xC0, 0x00, 0x40),bonustext);
			soil(bonusrect);
			clean();
		}	
		else
		{
			clean();
		}
		
		SDL_Delay(10);
	}

	// and clear any more events, for good luck.
#ifndef _TINSPIRE
	while (SDL_PollEvent(&event)) if (event.type == SDL_QUIT) quit=true;
#endif
			
	//printf("Level %d completed. ",level);	
	return(quit);
}

int gameover(long finalscore)
{
	printf("gameover\n");
	int done=false; int quit=false;
#ifndef _TINSPIRE
	SDL_Event event;
#else
	SDL_Event event = {0};
#endif

	SDL_Rect loserrect, finalrect;

	char finaltext[30];

	// FIX -- play some truimphant sound.

#ifdef _TINSPIRE
	char buffer[10];
	itoa(finalscore, buffer, 10);
	strcpy(finaltext, "FINAL SCORE: ");
	strcat(finaltext, buffer);
#else
	snprintf(finaltext,30,"FINAL SCORE: %ld",finalscore);
#endif
	
	// clear any pending events
	SDL_Delay(10); // needed? probably not.
//#ifndef _TINSPIRE
	while (SDL_PollEvent(&event)) if (event.type == SDL_QUIT) { done=true; quit=true; }
//#endif

	// get background
	SDL_BlitSurface(screen, NULL, screensave, NULL);
	
#ifndef _TINSPIRE
	if (!checkhiscore(finalscore))
		setcursor(CURSORCLICK);
	else 
		setcursor(CURSORARROW);
#endif

	loserrect.x=BORDERLEFT;
        loserrect.y=BORDERTOP;
        loserrect.h=CHARHEIGHT*4;
        loserrect.w=CHARWIDTH*4*12;

        finalrect.x=BORDERLEFT;
        finalrect.y=BORDERTOP;
        finalrect.h=CHARHEIGHT*4;
        finalrect.w=CHARWIDTH*4*12;

//	loserrect.x=BORDERLEFT+PLAYWIDTH/2-85;
//	loserrect.y=HEIGHT-CHARHEIGHT*4;
//	loserrect.h=CHARHEIGHT*4;
//	loserrect.w=CHARWIDTH*4*12;
	
//	finalrect.x=BORDERLEFT+PLAYWIDTH/2-130;
//	finalrect.y=HEIGHT-CHARHEIGHT*4;
//	finalrect.h=CHARHEIGHT*4;
//	finalrect.w=WIDTH-(BORDERLEFT+PLAYWIDTH/2-130);

	// wait for click, scroll score
	while (!done) 
	{
		while(SDL_PollEvent(&event));
		{
			if (event.type == SDL_QUIT)
			{
				printf("Quit\n");
				done=true; quit=true;
			}
			else if (event.type==SDL_MOUSEBUTTONDOWN)
			{
				printf("mousebuttondown\n");
				if (event.button.button==1)
				{
						done=true;
				}
			}
			else if (event.type == SDL_KEYDOWN)
                        {
				printf("keydown\n");
                                switch(event.key.keysym.sym)
                                {
                                        default:
                                        break;

                                        case SDLK_ESCAPE:
						printf("key escape\n");
                                                done = true; quit = true;
                                        break;

					case SDLK_r:
                                        case SDLK_DELETE:
                                        case SDLK_BACKSPACE:
					//case SDLK_RETURN:
						done = true;
                                        break;
				}
			}
		}
		
		if (loserrect.y>(HEIGHT/2-30))
		{
			loserrect.y--; 
			puttext(loserrect.x,loserrect.y,4,SDL_MapRGB(screen->format, 0xC0, 0x00, 0x40),"GAME OVER");
			soil(loserrect);
			clean();		
			SDL_BlitSurface(screensave,&loserrect, screen, &loserrect);
			//soil(loserrect);			
		}
		else if (loserrect.y==(HEIGHT/2-30))
		{
			// un-clean. :)
			loserrect.y--; 
			puttext(loserrect.x,loserrect.y,4,SDL_MapRGB(screen->format, 0xC0, 0x00, 0x40),"GAME OVER");			
			soil(loserrect);
			clean();
		}
		else if (finalrect.y>(HEIGHT/2))
		{
			finalrect.y--;
			puttext(finalrect.x,finalrect.y,4,SDL_MapRGB(screen->format, 0x00, 0x00, 0x00),finaltext);
			soil(finalrect);
			clean();
			SDL_BlitSurface(screensave,&finalrect, screen, &finalrect);
			//soil(finalrect);
		}
		else if (finalrect.y==(HEIGHT/2))
		{
			// un-clean. :)
			finalrect.y--;
			puttext(finalrect.x,finalrect.y,4,SDL_MapRGB(screen->format, 0x00, 0x00, 0x00),finaltext);
			soil(finalrect);
			if (checkhiscore(finalscore)) done=true;
			clean();
		}
		else
		{
			clean();
		}		
		
		SDL_Delay(10);
	}
	printf("Game over here\n");
		
	if (checkhiscore(finalscore))
	{
		quit=gethighusername(finalscore>hiscoreval[0]);
		addhiscore(username,finalscore);

		// jump text to top
		loserrect.y=(HEIGHT/2-31);
		puttext(loserrect.x,loserrect.y,4,SDL_MapRGB(screen->format, 0xC0, 0x00, 0x40),"GAME OVER");			
		soil(loserrect);
		finalrect.y=(HEIGHT/2);
		puttext(finalrect.x,finalrect.y,4,SDL_MapRGB(screen->format, 0x00, 0x00, 0x00),finaltext);
		soil(loserrect);
		clean();
	}

			
	// and clear any more events, for good luck.
//#ifndef _TINSPIRE
	while (SDL_PollEvent(&event)) if (event.type == SDL_QUIT) quit=true;
//#endif
			
	printf("Game over.\n");	
	return(quit);
}


int scrolltext(char * firsttext, int firstx, Uint32 firstcolor, char * secondtext, int secondx, Uint32 secondcolor)
{
	printf("scrolltext\n");
	int quit=false;
#if 0 /* FIX -- use this instead of the above two ugly functions */
	int done=false;
	SDL_Event event;
	SDL_Rect firstrect, secondrect;

	// get background
	SDL_BlitSurface(screen, NULL, screensave, NULL);
	
	firstrect.x=firstx;
	firstrect.y=HEIGHT-CHARHEIGHT*4;
	firstrect.h=CHARHEIGHT*4;
	firstrect.w=CHARWIDTH*4*12;
	
	secondrect.x=secondx;
	secondrect.y=HEIGHT-CHARHEIGHT*4;
	secondrect.h=CHARHEIGHT*4;
	secondrect.w=WIDTH-(BORDERLEFT+PLAYWIDTH/2-130);

	// wait for click, scroll score
	while (!done) 
	{
		while(SDL_PollEvent(&event));
		{
			if (event.type == SDL_QUIT)
			{
				done=true; quit=true;
			}
			else if (event.type==SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button==1)
				{
						done=true;
				}
			}
		}
		
		if (firstrect.y>(HEIGHT/2-30))
		{
			firstrect.y--; 
			puttext(firstrect.x,firstrect.y,4,SDL_MapRGB(screen->format, 0xC0, 0x00, 0x40),firsttext);
			soil(firstrect);
			clean();		
			SDL_BlitSurface(screensave,&firstrect, screen, &firstrect);
			//soil(firstrect);			
		}
		else if (firstrect.y==(HEIGHT/2-30))
		{
			// un-clean. :)
			firstrect.y--; 
			puttext(firstrect.x,firstrect.y,4,SDL_MapRGB(screen->format, 0xC0, 0x00, 0x40),firsttext);			
			soil(firstrect);
			clean();
		}
		else if (secondrect.y>(HEIGHT/2))
		{
			secondrect.y--;
			puttext(secondrect.x,secondrect.y,4,SDL_MapRGB(screen->format, 0x00, 0x00, 0x00),secondtext);
			soil(secondrect);
			clean();
			SDL_BlitSurface(screensave,&secondrect, screen, &secondrect);
			//soil(secondrect);
		}
		else if (secondrect.y==(HEIGHT/2))
		{
			// un-clean. :)
			secondrect.y--;
			puttext(secondrect.x,secondrect.y,4,SDL_MapRGB(screen->format, 0x00, 0x00, 0x00),secondtext);
			soil(secondrect);
			clean();
		}
		else
		{
			done=true;
			clean();
		}		
		
		SDL_Delay(10);
	}
#endif /* FIX! */
	return(quit);
}
