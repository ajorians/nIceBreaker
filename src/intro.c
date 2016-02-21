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
#include "sound.h"
#include "laundry.h"
#include "globals.h"
#include "level.h"
#include "status.h"
#include "text.h"
#include "hiscore.h"

#define LXPOS(x) (BORDERLEFT+(BLOCKWIDTH*(x)))
#define LYPOS(y) (BORDERTOP+(BLOCKHEIGHT*(y)))

static void setupintro(void);

void setupintro()
{
	int x,y;
	int c;
	SDL_Rect tmprect;

	setcursor(CURSORARROW);
	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format, 0x00, 0x40, 0x80));
	
	tmprect.x=BORDERLEFT; tmprect.y=BORDERTOP;
	tmprect.w=PLAYWIDTH; tmprect.h=PLAYHEIGHT;
	SDL_FillRect(screen,&tmprect,SDL_MapRGB(screen->format, 0xC0, 0xC0, 0xC0));

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
				grid[x][y]='w';
		}
	
	/// Here's the fun part where we set up the letters.
	// I
	markgrid(LXPOS(1),LYPOS(2),BLOCKWIDTH*3,BLOCKHEIGHT  ,' ');
	markgrid(LXPOS(1),LYPOS(7),BLOCKWIDTH*3,BLOCKHEIGHT  ,' ');
	markgrid(LXPOS(2),LYPOS(2),BLOCKWIDTH  ,BLOCKHEIGHT*6,' ');
	
	//C
	markgrid(LXPOS(5),LYPOS(2),BLOCKWIDTH*4,BLOCKHEIGHT  ,' ');
	markgrid(LXPOS(5),LYPOS(3),BLOCKWIDTH  ,BLOCKHEIGHT*5,' ');	
	markgrid(LXPOS(6),LYPOS(7),BLOCKWIDTH*3,BLOCKHEIGHT,  ' ');
	
	//E
	markgrid(LXPOS(10),LYPOS(2),BLOCKWIDTH*4,BLOCKHEIGHT  ,' ');
	markgrid(LXPOS(10),LYPOS(3),BLOCKWIDTH  ,BLOCKHEIGHT*5,' ');	
	markgrid(LXPOS(11),LYPOS(7),BLOCKWIDTH*3,BLOCKHEIGHT,  ' ');
	markgrid(LXPOS(11),LYPOS(4),BLOCKWIDTH*2,BLOCKHEIGHT  ,' ');
	
}



int intro(void)
{
	// FIX -- hey, here's an idea. how about moving some of this crap
	// to separate functions?


	int penguincount=0;
	int frozen=true;
	int i;
	int x,y;
	int letterstep=0;
	int hiscoreindex=0; int hiscorescroll=0;
	int hiscorestep=false;
	int linedone1=false; int linedone2=false;

#ifndef _TINSPIRE
	char scoretext[40];
	SDL_Rect labelrect;
	SDL_Rect scrollrect;
	SDL_Rect bigrect;
	SDL_Rect rightmarginrect;
	SDL_Rect leftmarginrect;
#endif
	
	int done = false;
	
	Penguin flock[100];
	Penguin tux;
	
	SDL_Event event;

	setupintro();
	
	
#ifndef _TINSPIRE
	labelrect.x=BORDERLEFT;
	labelrect.y=BORDERTOP-(4*CHARHEIGHT);
	labelrect.h=CHARHEIGHT*2;
	labelrect.w=CHARWIDTH*21;
			
	scrollrect.x=BORDERLEFT+CHARWIDTH*21;
	scrollrect.y=labelrect.y;
	scrollrect.w=BORDERRIGHT-(BORDERLEFT+CHARWIDTH*21);
	scrollrect.h=labelrect.h;

	rightmarginrect.x=BORDERRIGHT;
	rightmarginrect.y=labelrect.y;
	rightmarginrect.w=MARGINRIGHT;
	rightmarginrect.h=labelrect.h;
	
	leftmarginrect.x=0;
	leftmarginrect.y=labelrect.y;
	leftmarginrect.w=BORDERLEFT;
	leftmarginrect.h=labelrect.h;
	
	bigrect.x=labelrect.x;
	bigrect.y=labelrect.y;
	bigrect.w=WIDTH-(BORDERLEFT);
	bigrect.h=labelrect.h;

	hiscoreindex=HISCORENUM-1; hiscorescroll=200;
#endif

#ifndef _TINSPIRE
	puttext(labelrect.x,labelrect.y,2,SDL_MapRGB(screen->format,  0xFF, 0xFF, 0xFF),"HIGH SCORES:");
	soil(labelrect);
#endif


	line1=createline('1',0x00, 0x00, 0x00);
	line2=createline('2',0xC0, 0x00, 0x40);

	tux = createpenguin();

	printf("some intro\n");	
	do 
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				return(true);
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				done=true;
			}
			//FIX -- add keyboard support
			else if(event.type == SDL_KEYDOWN )
			{
				switch(event.key.keysym.sym)
				{
					default:
					break;
					case SDLK_ESCAPE:
						return(true);
					break;

					case SDLK_RETURN:
					case SDLK_LCTRL:
						done = true;
					break;
				}
			}
		}
		if (letterstep < 10)
		{
			switch (letterstep)
			{
				// I
				case 0:
					if (!linedone1 && !line1.on) startline(&line1,LEFT,LXPOS(2),LYPOS(2));
					if (!linedone2 && !line2.on) startline(&line2,RIGHT,LXPOS(2),LYPOS(2));
				break;
				case 1:
					if (!linedone1 && !line1.on) startline(&line1,LEFT,LXPOS(2),LYPOS(7));
					if (!linedone2 && !line2.on) startline(&line2,RIGHT,LXPOS(2),LYPOS(7));
				break;
				case 2:
					if (!linedone1 && !line1.on) startline(&line1,UP,LXPOS(2),LYPOS(5));
					if (!linedone2 && !line2.on) startline(&line2,DOWN,LXPOS(2),LYPOS(5));
				break;
				// C
				case 3:
					if (!linedone1 && !line1.on) startline(&line1,LEFT,LXPOS(7),LYPOS(2));
					if (!linedone2 && !line2.on) startline(&line2,RIGHT,LXPOS(7),LYPOS(2));
				break;
				case 4:
					if (!linedone1 && !line1.on) startline(&line1,UP,LXPOS(5),LYPOS(5));
					if (!linedone2 && !line2.on) startline(&line2,DOWN,LXPOS(5),LYPOS(5));
				break;
				case 5:
					if (!linedone1 && !line1.on) startline(&line1,LEFT,LXPOS(7),LYPOS(7));
					if (!linedone2 && !line2.on) startline(&line2,RIGHT,LXPOS(7),LYPOS(7));
				break;
				// E
				case 6:
					if (!linedone1 && !line1.on) startline(&line1,LEFT,LXPOS(11),LYPOS(2));
					if (!linedone2 && !line2.on) startline(&line2,RIGHT,LXPOS(11),LYPOS(2));
				break;
				case 7:
					if (!linedone1 && !line1.on) startline(&line1,UP,LXPOS(10),LYPOS(5));
					if (!linedone2 && !line2.on) startline(&line2,DOWN,LXPOS(10),LYPOS(5));
				break;
				case 8:
					if (!linedone1 && !line1.on) startline(&line1,LEFT,LXPOS(13),LYPOS(7));
					if (!linedone2 && !line2.on) startline(&line2,RIGHT,LXPOS(13),LYPOS(7));
				break;
				case 9:
					if (!linedone1 && !line1.on) startline(&line1,LEFT,LXPOS(12),LYPOS(4));
					if (!linedone2 && !line2.on) startline(&line2,RIGHT,LXPOS(12),LYPOS(4));
				break;
			}
				
		}
		else if (letterstep==10)
		{
			printf("letterstop 10\n");
			for (x=0;x<WIDTH;x++)
				for (y=0;y<HEIGHT;y++)
					if (grid[x][y]=='w')
					{
						grid[x][y]=' ';
					}
					// FIX: known bug -- if a line stops partway through a grid cell,
					// the space left over is not cleared properly later. this isn't 
					// an issue in the game, as lines aways stop on cell boundaries, but
					// sometimes shows up in the intro. this is a kludge
					// which should fix it.
					else if (((x-BORDERLEFT) % BLOCKWIDTH)==0 && ((y-BORDERTOP) % BLOCKHEIGHT)==0)
					{
						if (grid[x+BLOCKWIDTH-1][y+BLOCKHEIGHT-1]==' ')
							grid[x][y]=' ';
					}
			letterstep++;
		}
		else if (letterstep==11)
		{
			printf("Letterstop 11\n");
			//B
			flock[penguincount]=createpenguinxy(LXPOS(0),LYPOS(11)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(0),LYPOS(12)); penguincount++; 
			flock[penguincount]=createpenguinxy(LXPOS(0),LYPOS(13)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(0),LYPOS(14)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(0),LYPOS(15)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(0),LYPOS(16)); penguincount++;

			flock[penguincount]=createpenguinxy(LXPOS(1),LYPOS(11)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(2),LYPOS(11)); penguincount++;
			
			flock[penguincount]=createpenguinxy(LXPOS(1),LYPOS(13)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(2),LYPOS(13)); penguincount++;
			
			flock[penguincount]=createpenguinxy(LXPOS(1),LYPOS(16)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(2),LYPOS(16)); penguincount++;
			
			flock[penguincount]=createpenguinxy(LXPOS(3),LYPOS(12)); penguincount++;
			
			flock[penguincount]=createpenguinxy(LXPOS(3),LYPOS(14)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(3),LYPOS(15)); penguincount++;
			
			//R
			flock[penguincount]=createpenguinxy(LXPOS(5),LYPOS(11)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(5),LYPOS(12)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(5),LYPOS(13)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(5),LYPOS(14)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(5),LYPOS(15)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(5),LYPOS(16)); penguincount++;
			
			flock[penguincount]=createpenguinxy(LXPOS(6),LYPOS(11)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(7),LYPOS(11)); penguincount++;
			
			flock[penguincount]=createpenguinxy(LXPOS(6),LYPOS(13)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(7),LYPOS(13)); penguincount++;
			
			flock[penguincount]=createpenguinxy(LXPOS(8),LYPOS(12)); penguincount++;
			
			flock[penguincount]=createpenguinxy(LXPOS(8),LYPOS(14)); penguincount++;
 			flock[penguincount]=createpenguinxy(LXPOS(8),LYPOS(15)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(8),LYPOS(16)); penguincount++;
			
			//E
			flock[penguincount]=createpenguinxy(LXPOS(10),LYPOS(11)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(10),LYPOS(12)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(10),LYPOS(13)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(10),LYPOS(14)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(10),LYPOS(15)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(10),LYPOS(16)); penguincount++;
			
			flock[penguincount]=createpenguinxy(LXPOS(11),LYPOS(11)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(12),LYPOS(11)); penguincount++;
			
			flock[penguincount]=createpenguinxy(LXPOS(11),LYPOS(13)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(12),LYPOS(13)); penguincount++;
			
			flock[penguincount]=createpenguinxy(LXPOS(11),LYPOS(16)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(12),LYPOS(16)); penguincount++;
			
			//A
			flock[penguincount]=createpenguinxy(LXPOS(14),LYPOS(12)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(14),LYPOS(13)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(14),LYPOS(14)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(14),LYPOS(15)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(14),LYPOS(16)); penguincount++;
			
			flock[penguincount]=createpenguinxy(LXPOS(17),LYPOS(12)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(17),LYPOS(13)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(17),LYPOS(14)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(17),LYPOS(15)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(17),LYPOS(16)); penguincount++;

			flock[penguincount]=createpenguinxy(LXPOS(15),LYPOS(11)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(16),LYPOS(11)); penguincount++;

			flock[penguincount]=createpenguinxy(LXPOS(15),LYPOS(13)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(16),LYPOS(13)); penguincount++;

			//K
			flock[penguincount]=createpenguinxy(LXPOS(19),LYPOS(11)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(19),LYPOS(12)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(19),LYPOS(13)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(19),LYPOS(14)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(19),LYPOS(15)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(19),LYPOS(16)); penguincount++;
			
			flock[penguincount]=createpenguinxy(LXPOS(20),LYPOS(13)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(21),LYPOS(12)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(22),LYPOS(11)); penguincount++;
			
			flock[penguincount]=createpenguinxy(LXPOS(21),LYPOS(14)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(22),LYPOS(15)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(22),LYPOS(16)); penguincount++;

			//E
			flock[penguincount]=createpenguinxy(LXPOS(24),LYPOS(11)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(24),LYPOS(12)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(24),LYPOS(13)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(24),LYPOS(14)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(24),LYPOS(15)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(24),LYPOS(16)); penguincount++;
			
			flock[penguincount]=createpenguinxy(LXPOS(25),LYPOS(11)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(26),LYPOS(11)); penguincount++;
			
			flock[penguincount]=createpenguinxy(LXPOS(25),LYPOS(13)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(26),LYPOS(13)); penguincount++;
			
			flock[penguincount]=createpenguinxy(LXPOS(25),LYPOS(16)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(26),LYPOS(16)); penguincount++;

			//R
			flock[penguincount]=createpenguinxy(LXPOS(28),LYPOS(11)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(28),LYPOS(12)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(28),LYPOS(13)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(28),LYPOS(14)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(28),LYPOS(15)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(28),LYPOS(16)); penguincount++;
			
			flock[penguincount]=createpenguinxy(LXPOS(29),LYPOS(11)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(30),LYPOS(11)); penguincount++;
			
			flock[penguincount]=createpenguinxy(LXPOS(29),LYPOS(13)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(30),LYPOS(13)); penguincount++;
			
			flock[penguincount]=createpenguinxy(LXPOS(31),LYPOS(12)); penguincount++;
			
			flock[penguincount]=createpenguinxy(LXPOS(31),LYPOS(14)); penguincount++;
 			flock[penguincount]=createpenguinxy(LXPOS(31),LYPOS(15)); penguincount++;
			flock[penguincount]=createpenguinxy(LXPOS(31),LYPOS(16)); penguincount++;

			if (penguincount>=MAXPENGUINS) { fprintf(stderr,"Too many penguins!\n"); }

			for (i=0;i<penguincount;i++)
			{
				savebehindpenguin(&flock[i]);
				markgrid(flock[i].geom.x,flock[i].geom.y,BLOCKWIDTH,BLOCKHEIGHT,'*');	
				drawpenguin(&flock[i]);
				soil(flock[i].geom);
			}
			
			
			letterstep++;
		}
		else if (letterstep<300)
		{
			letterstep++;
		}
		else if (letterstep==300)
		{
			printf("letter stop 300\n");
			frozen=false;
			for (i=0;i<penguincount;i++)
			{
				markgrid(flock[i].geom.x,flock[i].geom.y,BLOCKWIDTH,BLOCKHEIGHT,' ');
				erasepenguin(&flock[i]);
				soil(flock[i].geom);
			}
			letterstep++;
			setcursor(CURSORCLICK);
		}
		else if (letterstep<400)
		{
			letterstep++;
		}
		else if (letterstep==400)
		{
			if (penguincount)
			{
				penguincount--;
				markgrid(flock[penguincount].geom.x,flock[penguincount].geom.y,BLOCKWIDTH,BLOCKHEIGHT,' ');
				erasepenguin(&flock[penguincount]);
				soil(flock[penguincount].geom);
			}
			else
			{
				letterstep++;
			}
		}
		else if (letterstep<800)
		{
			if (!line1.on && !line2.on)
			{
#ifdef _TINSPIRE
				x=LXPOS(rand() % 32);
				y=LYPOS(rand() % 20);
				if (rand()%2)
#else
				x=LXPOS(random() % 32);
                                y=LYPOS(random() % 20);
                                if (random()%2)
#endif
				{
					startline(&line1,UP,x,y);
					startline(&line2,DOWN,x,y);
				}
				else
				{
					startline(&line1,LEFT,x,y);
					startline(&line2,RIGHT,x,y);
				}

			}			
		}
		//else if (letterstep<2000)
		//{
		//	letterstep++;
		//}
		//else
		//{
		//	done=true;
		//}

		// high score stuff
		// FIX: it'd be *much* more efficient to draw the text to
		// surfaces and then scroll the surfaces, since the text-draw
		// routine is so slow.
		
		if (letterstep>0) // that's a bit kludgy...
		{
#ifndef _TINSPIRE
			SDL_FillRect(screen,&bigrect,SDL_MapRGB(screen->format,  0x00, 0x40, 0x80));

			// 1
			printf("about to snprintf\n");
			snprintf(scoretext,40,"#%d. %s: %ld",hiscoreindex+1,hiscorename[hiscoreindex],hiscoreval[hiscoreindex]);
			puttext(scrollrect.x-hiscorescroll,scrollrect.y,2,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF), scoretext);
	
			// 2			
			snprintf(scoretext,40,"#%d. %s: %ld",((hiscoreindex+1)%HISCORENUM)+1,hiscorename[(hiscoreindex+1)%HISCORENUM],hiscoreval[(hiscoreindex+1)%HISCORENUM]);
			puttext(scrollrect.x-hiscorescroll+250,scrollrect.y,2,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF), scoretext);
						
			// 3!
			snprintf(scoretext,40,"#%d. %s: %ld",((hiscoreindex+2)%HISCORENUM)+1,hiscorename[(hiscoreindex+2)%HISCORENUM],hiscoreval[(hiscoreindex+2)%HISCORENUM]);
			puttext(scrollrect.x-hiscorescroll+500,scrollrect.y,2,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF), scoretext);
			printf("Done with that text stuff\n");
				
			// blank left margin
			SDL_FillRect(screen,&leftmarginrect,SDL_MapRGB(screen->format,  0x00, 0x40, 0x80));

			// label
			SDL_FillRect(screen,&labelrect,SDL_MapRGB(screen->format,  0x00, 0x40, 0x80));
			puttext(labelrect.x,labelrect.y,2,SDL_MapRGB(screen->format,  0xFF, 0xFF, 0xFF),"HIGH SCORES:");	
				
			// blank right margin
			SDL_FillRect(screen,&rightmarginrect,SDL_MapRGB(screen->format,  0x00, 0x40, 0x80));

			soil(bigrect);
	
			if (hiscorestep) hiscorescroll++;
			hiscorestep=!hiscorestep;
			if (hiscorescroll>=250)
			{
				hiscorescroll=0;
				hiscoreindex=(hiscoreindex+1)%HISCORENUM;
			}
#endif
		}	
		
		
		// move split-lines
		if (line1.on) 
		{
			if (moveline(&line1))
			{
				linedone1=true;
			}
			if (line1.dead) // need a second check because moveline could change this
			{
				//no sound in intro?
				//playsound(SNDBREAK);
				killline(&line1);
				linedone1=false;
			}
		}
		if (line2.on)
		{
		
			if (moveline(&line2))
			{
				linedone2=true;
				
			}
			if (line2.dead)
			{ 
				// no sound in intro?
				//playsound(SNDBREAK);
				killline(&line2);
				linedone2=false;
			}
		}
		if (linedone1 && linedone2) { letterstep++; linedone1=false; linedone2=false; }
		

		// move (and get old background)
		
		if (!frozen)
		{
			for (i=0;i<penguincount;i+=2)
			{
				soil(flock[i].geom); // mark the penguin's old position as dirty
				movepenguin(&flock[i]);
				soil(flock[i].geom); // mark the penguin's new position as dirty too (it will be soon...)
				savebehindpenguin(&flock[i]);
			}
		}
		soil(tux.geom); // mark the penguin's old position as dirty
		movepenguin(&tux);
		soil(tux.geom); // mark the penguin's new position as dirty too (it will be soon...)
		savebehindpenguin(&tux);
		
		// actually draw
		if (!frozen)
		{
			for (i=0;i<penguincount;i+=2)
			{
				drawpenguin(&flock[i]);
			}
		}
		drawpenguin(&tux);
		
		// update screen
		clean();

		// clear for next update
		if (!frozen)
		{
			for (i=0;i<penguincount;i+=2)
			{
				erasepenguin(&flock[i]);
			}
		}
		erasepenguin(&tux);
		
		SDL_Delay(10);
		
	} while (!done);

	clean();
	printf("Return\n");
	return(false);
}

