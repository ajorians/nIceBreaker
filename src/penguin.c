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
#ifdef _TINSPIRE
#else
#include <stdlib.h>
#endif
#include "icebreaker.h"
#include "globals.h"
#include "penguin.h"
#include "grid.h"
#include "line.h"
#include "level.h"
#include "options.h" 

Penguin createpenguin()
{
	return(createpenguinxy(BORDERLEFT + (
#ifdef _TINSPIRE
rand()
#else
random()
#endif
 % (PLAYWIDTH-BLOCKWIDTH)),BORDERTOP + (
#ifdef _TINSPIRE
rand()
#else
random()
#endif
 % (PLAYHEIGHT-BLOCKWIDTH))));
}

Penguin createpenguinxy(int x, int y)
{
	Penguin p;
	//double h;
	p.image = penguinimage;
	
	
	/*
	h = ((random()/(double)RAND_MAX)*3.1415926536*2);
	
	p.xdelta= (int)rint(cos(h) * PENGUINSPEED);
	p.ydelta= (int)rint(sin(h) * PENGUINSPEED);
	if (p.xdelta==0) // no straight lines. too easy!
	{
		if (random()%2) { p.xdelta=-1; } else { p.xdelta=1; }
	}
	if (p.ydelta==0)
	{
		if (random()%2) { p.ydelta=-1; } else { p.ydelta=1; }
	}
	*/
	
	// FIX -- PENGUINSPEED should be variable, not a define. (set in preferences, of course) 
	switch (
#ifdef _TINSPIRE
rand()
#else
random()
#endif
 % 4)
	{
		case 0:
			p.xdelta= PENGUINSPEED;p.ydelta= PENGUINSPEED;
		break;
		case 1:
			p.xdelta= PENGUINSPEED;p.ydelta=-PENGUINSPEED;
		break;
		case 2:
			p.xdelta=-PENGUINSPEED;p.ydelta= PENGUINSPEED;
		break;
		case 3:
			p.xdelta=-PENGUINSPEED;p.ydelta=-PENGUINSPEED;
		break;
	}
	
	// For debugging only, of course.
	//p.xdelta=0; p.ydelta=0;
	
	p.speedslower=0;
			
	p.geom.w=p.image->w;
	p.geom.h=p.image->h;	
			
	//p.geom.x=BORDERLEFT + (PLAYWIDTH +BLOCKWIDTH )/2;
	//p.geom.y=BORDERTOP  + (PLAYHEIGHT+BLOCKHEIGHT)/2;
			
	p.geom.x=x;
	p.geom.y=y;

	
#ifdef _TINSPIRE	
	p.bgsave = SDL_CreateRGBSurface(SDL_HWSURFACE,p.geom.w,p.geom.h,16,0,0,0,0);
#else
	p.bgsave = SDL_CreateRGBSurface(SDL_HWSURFACE,p.geom.w,p.geom.h,32,0,0,0,0);
#endif

	return(p);
}


void savebehindpenguin(Penguin * p)
{
	SDL_BlitSurface(screen, &(p->geom), p->bgsave, NULL);
}	

void drawpenguin(Penguin * p)
{
	SDL_BlitSurface(p->image, NULL, screen, &(p->geom));
}

void erasepenguin(Penguin * p)
{
	SDL_BlitSurface(p->bgsave, NULL, screen, &(p->geom));
}

void movepenguin(Penguin * p)
{
	int newx, newy;
	int checkx,checky;
	int movex=0,movey=0;

	switch (options.difficulty)
	{
		case NORMAL:
			if (p->speedslower)
				{ movex=p->xdelta/2; movey=p->ydelta/2; }
			else
				{ movex=p->xdelta; movey=p->ydelta; }
			p->speedslower=!p->speedslower;
		break;
		case HARD:
			movex=p->xdelta; movey=p->ydelta;
		break;
		case EASY:
			movex=p->xdelta/2; movey=p->ydelta/2;
		break;
	}
		
	newx=p->geom.x+movex;
	newy=p->geom.y+movey;

	
	markgrid(p->geom.x,p->geom.y,BLOCKWIDTH,BLOCKHEIGHT,' ');
	//markgrid(p->geom.x+1,p->geom.y+1,BLOCKWIDTH-1,BLOCKHEIGHT-1,' ');
	
	if (movex>0) checkx = newx+BLOCKWIDTH;
	else checkx = newx;

	if (grid[checkx][p->geom.y]==' ' && grid[checkx][p->geom.y+BLOCKHEIGHT-1]==' ')
	{
		p->geom.x+=movex;
	}
	else if (grid[checkx][p->geom.y]=='1' || grid[checkx][p->geom.y+BLOCKHEIGHT-1]=='1')
	{
		line1.dead=true;
		p->geom.x+=movex;	
	}
	else if (grid[checkx][p->geom.y]=='2' || grid[checkx][p->geom.y+BLOCKHEIGHT-1]=='2')
	{
		line2.dead=true;
		p->geom.x+=movex;	
	}
	else if ((grid[checkx][p->geom.y]=='w' || grid[checkx][p->geom.y]==' ' ) 
	      && (grid[checkx][p->geom.y+BLOCKHEIGHT-1]=='w' || grid[checkx][p->geom.y+BLOCKHEIGHT-1]==' '))
	{
		// this is used in the intro. maybe some place else too in the future.
		// should it be merged into the first line above? maybe.
		p->geom.x+=movex;
	}
	else
	{
		p->xdelta=-p->xdelta;
	}

	if (movey>0) checky = newy+BLOCKHEIGHT;
	else checky = newy;
	
	if (grid[p->geom.x][checky]==' ' && grid[p->geom.x+BLOCKWIDTH-1][checky]==' ')
	{
		p->geom.y+=movey;
	}
	else if (grid[p->geom.x][checky]=='1' || grid[p->geom.x+BLOCKWIDTH-1][checky]=='1')
	{
		//printf("Hit 1\n");
		line1.dead=true;
		p->geom.y+=movey;		
	}
	else if (grid[p->geom.x][checky]=='2' || grid[p->geom.x+BLOCKWIDTH-1][checky]=='2')
	{
		//printf("Hit 2\n");
		line2.dead=true;
		p->geom.y+=movey;		
	}
	else if ((grid[p->geom.x][checky]=='w' || grid[p->geom.x][checky]==' ') 
	      && (grid[p->geom.x+BLOCKWIDTH-1][checky]=='w' || grid[p->geom.x+BLOCKWIDTH-1][checky]==' '))
	{
		// this is used in the intro. maybe some place else too in the future.
		// should it be merged into the first line above? maybe.
		p->geom.y+=movey;
	}

	else
	{
		p->ydelta=-p->ydelta;
	}

	markgrid(p->geom.x,p->geom.y,BLOCKWIDTH,BLOCKHEIGHT,'*');
	//markgrid(p->geom.x+1,p->geom.y+1,BLOCKWIDTH-1,BLOCKHEIGHT-1,'*');
}
