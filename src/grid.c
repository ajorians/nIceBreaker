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
#include <string.h>
#endif

#include "icebreaker.h"
#include "laundry.h"
#include "grid.h"
#include "penguin.h"
#include "globals.h"

static char maskgrid[WIDTH][HEIGHT];

// kludge-o-rama
static long rcount;
#define MAXRCOUNT 80000



void markgrid(int x, int y, int w, int h, char fillchar)
{
	int i;
	for (i=x;i<x+w;i++)
		memset(&grid[i][y],fillchar,h); // FIX -- do the pointer math ourselves; it's a teensy bit faster, and this function gets called *a lot*
	
	/*
	int i, j;
	for (j=y;j<y+h;j++)
		for (i=x;i<x+w;i++)
			grid[i][j]=fillchar;		
	*/	
}

long countcleared()
{
	int i, j;
	long c;
	c=0;
	for (i=BORDERLEFT;i<BORDERRIGHT;i++)
		for (j=BORDERTOP;j<BORDERBOTTOM;j++)
			if (grid[i][j] == ' ' || grid[i][j] == '*')
				c++;
	//return(100-(c*100/(PLAYWIDTH*PLAYHEIGHT)));
	return(c);
	
}

#ifdef DEBUG
void printboard()
{
	int i, j;
	
	for (j=BLOCKWIDTH/2;j<HEIGHT;j+=BLOCKHEIGHT)
	{
		for (i=BLOCKWIDTH/2;i<WIDTH;i+=BLOCKWIDTH)
		{
			printf("%c ",grid[i][j]);
		}
		printf("\n");
	}
}
#endif

#ifdef DEBUG
void printwholegrid()
{
	int i, j;
	
	printf ("grid:\n");
	for (j=0;j<HEIGHT;j++)
	{
		for (i=0;i<WIDTH;i++)
		{
			printf("%c ",grid[i][j]);
		}
		printf("\n");
	}
}
#endif

#ifdef DEBUG
void printwholemaskgrid()
{
	int i, j;
	
	printf ("maskgrid:\n");
	for (j=0;j<HEIGHT;j++)
	{
		for (i=0;i<WIDTH;i++)
		{
			printf("%c ",maskgrid[i][j]);
		}
		printf("\n");
	}
}
#endif

void checkempty(int x, int y)
{
	//int i,j;
	SDL_Rect tmprect;
	

	// if square isn't empty, just return....
	if (grid[x][y]!=' ') {  return; }


	// it'd be nice to find a way to keep this longer...
	memcpy(maskgrid,grid,WIDTH*HEIGHT);
	

	// penguinsearch at that spot...
	rcount=0;
	if (!penguinsearch(x,y)) // area is clear!
	{
		//printwholemaskgrid();
		

		
		//floodfill(x,y);
		
		// this makes sure x and y are the top left corners of blocks.
		// since the area is empty of penguins, it should be completely
		// safe to use this isntead of floodfill here. really. :)
		squarefill( (((x-BORDERLEFT)/BLOCKWIDTH ) * BLOCKWIDTH ) +BORDERLEFT, (((y-BORDERTOP)/BLOCKHEIGHT) * BLOCKHEIGHT) +BORDERTOP);

		tmprect.w=BLOCKWIDTH; tmprect.h=BLOCKHEIGHT;
		for (tmprect.x=BORDERLEFT;tmprect.x<BORDERRIGHT;tmprect.x+=BLOCKWIDTH)
			for (tmprect.y=BORDERTOP;tmprect.y<BORDERBOTTOM;tmprect.y+=BLOCKHEIGHT)
				if (grid[tmprect.x][tmprect.y]=='.') // clear it!)
				{
					SDL_FillRect(screen,&tmprect,SDL_MapRGB(screen->format, 0x00, 0x40, 0x80));
					soil(tmprect);
				}
		//printwholegrid();
	}
	/*
	for (j=0;j<HEIGHT;j+=BLOCKHEIGHT)
	{
		for (i=0;i<WIDTH;i+=BLOCKWIDTH)
		{
			printf("%c ",maskgrid[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	*/
}


int penguinsearch(int i, int j)
{
	int searchval=0;

	rcount++; 
	// kludge! FIX! BAD!
	if (rcount>MAXRCOUNT) // bail
	{
		fprintf(stderr,"Damn. Ran out of recursions.\n");
		return(2);
	}
	
	
	// shouldn't need to check bounds because we're only painting in the
	// middle. and we call this function so much that the time saved
	// is worth it
	//if (i<0 || j<0 || i>=WIDTH || j>=HEIGHT)
	//{
	//	fprintf(stderr,"That shouldn't have happened (penguinsearch)! (%d,%d)\n",i,j);
	//	exit(1);
	//}

          
	if (maskgrid[i][j]==' '
	    || maskgrid[i][j]=='1' || maskgrid[i][j]=='2' || maskgrid[i][j]=='w')  // Ah ha! The nefarious "instant melting ice" bug solved!  NOTE: if more lines are added to the game, add them here too!
	{
		maskgrid[i][j]=',';
		
		searchval=penguinsearch(i+BLOCKWIDTH, j);
		if (!searchval) searchval=penguinsearch(i-BLOCKWIDTH, j);
		if (!searchval) searchval=penguinsearch(i, j-BLOCKHEIGHT);
		if (!searchval) searchval=penguinsearch(i, j+BLOCKHEIGHT);
		
	}
	else if (maskgrid[i][j]=='*') // found a penguin!
	{
		searchval=1;	
	}
	return(searchval);
}


void floodfill(int x, int y)
{
	// shouldn't need to check bounds because we're only painting in the
	// middle.
	//if (x<0 || y<0 || x>WIDTH || y>HEIGHT)
	//{
	//	fprintf(stderr,"That shouldn't have happened! (%d,%d)\n",x,y);
	//	exit(1);
	//}
	if (grid[x][y]==' ' || grid[x][y]=='1' || grid[x][y]=='2' || grid[x][y]=='w')
	{
		grid[x][y]='.';
		floodfill(x+1, y);
		floodfill(x-1, y);
		floodfill(x, y+1);
		floodfill(x, y-1);
	}
}

void squarefill(int x, int y)
{
	// x and y must be the top left corner of a square, or else this
	// will look silly. and there's no bounds checking!

	if (grid[x][y]==' ' || grid[x][y]=='1' || grid[x][y]=='2' || grid[x][y]=='w')
	{
		markgrid(x,y,BLOCKWIDTH,BLOCKHEIGHT,'.');
		squarefill(x+BLOCKWIDTH, y);
		squarefill(x-BLOCKWIDTH, y);
		squarefill(x, y+BLOCKHEIGHT);
		squarefill(x, y-BLOCKHEIGHT);
	}
}

