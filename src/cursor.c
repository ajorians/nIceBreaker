/*
* IceBreaker
* Copyright (c) 2000-2001 Matthew Miller <mattdm@mattdm.org>
* except the init_system_cursor function, which was lovingly stolen from
* the SDL documentation
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
#include "cursor.h"
#include "icebreaker.h"
#ifdef _TINSPIRE
#include "globals.h"
#endif

static SDL_Cursor *init_system_cursor(const char *image[]);

/* XPM */
static const char *cursorarrow[] = {
/* width height num_colors chars_per_pixel */
"    32    32        3            1",
/* colors */
"X c #000000",
". c #ffffff",
"  c None",
/* pixels */
"X                               ",
"XX                              ",
"X.X                             ",
"X..X                            ",
"X...X                           ",
"X....X                          ",
"X.....X                         ",
"X......X                        ",
"X.......X                       ",
"X........X                      ",
"X.....XXXXX                     ",
"X..X..X                         ",
"X.X X..X                        ",
"XX  X..X                        ",
"X    X..X                       ",
"     X..X                       ",
"      X..X                      ",
"      X..X                      ",
"       XX                       ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"0,0"
};


/* XPM */
static const char *cursorhorizontal[] = {
/* width height num_colors chars_per_pixel */
"    32    32        3            1",
/* colors */
"X c #000000",
". c #ffffff",
"  c None",
/* pixels */
"        X X                     ",
"       XX XX                    ",
"      X.X X.X                   ",
"     X..X X..X                  ",
"    X...X X...X                 ",
"   X....X X....X                ",
"  X.....XXX.....X               ",
" X...............X              ",
"X.................X             ",
" X...............X              ",
"  X.....XXX.....X               ",
"   X....X X....X                ",
"    X...X X...X                 ",
"     X..X X..X                  ",
"      X.X X.X                   ",
"       XX XX                    ",
"        X X                     ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"9,8"
};

/* XPM */
static const char *cursorvertical[] = {
/* width height num_colors chars_per_pixel */
"    32    32        3            1",
/* colors */
"X c #000000",
". c #ffffff",
"  c None",
/* pixels */
"        X                       ",
"       X.X                      ",
"      X...X                     ",
"     X.....X                    ",
"    X.......X                   ",
"   X.........X                  ",
"  X...........X                 ",
" X.............X                ",
"XXXXXXX...XXXXXXX               ",
"      X...X                     ",
"XXXXXXX...XXXXXXX               ",
" X.............X                ",
"  X...........X                 ",
"   X.........X                  ",
"    X.......X                   ",
"     X.....X                    ",
"      X...X                     ",
"       X.X                      ",
"        X                       ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"8,9"
};

/* XPM */
static const char *cursorclick[] = {
/* width height num_colors chars_per_pixel */
"    32    32        3            1",
/* colors */
"X c #000000",
". c #ffffff",
"  c None",
/* pixels */
" XXXX  XXX   XXX  XXXX  XXX  XX ",
"X....X X.X   X.X X....X X.X X.X ",
"X.XXX  X.X   X.X X.XXX  X.XX.X  ",
"X.X    X.X   X.X X.X    X...X   ",
"X.X    X.X   X.X X.X    X.XX.X  ",
"X.XXX  X.XXX X.X X.XXX  X.X X.X ",
"X....X X...X X.X X....X X.X X.X ",
" XXXX  XXXXX XXX  XXXX  XXX XXX ",
"                                ",
"                                ",
"        XXXXXXX  XXXXX          ",
"        X.....X X.....X         ",
"        XXX.XXX X.XXX.X         ",
"          X.X   X.X X.X         ",
"          X.X   X.X X.X         ",
"          X.X   X.XXX.X         ",
"          X.X   X.....X         ",
"          XXX    XXXXX          ",
"                                ",
"                                ",
" XXXX  XXXXXX  X    XXXX  XXXXXX",
"X....X X....X X.X   X...X X....X",
"X.XXX  XX.XXX X..X  X.XX.X X.XXX",
"X....X  X.X  X.XX.X X...X  X.X  ",
" XXX.X  X.X X.....X X.X.X  X.X  ",
" XXX.X  X.X X.XXX.X X.XX.X X.X  ",
"X....X  X.X X.X X.X X.XX.X X.X  ",
" XXXX   XXX XXX XXX XXX XX XXX  ",
"                                ",
"                                ",
"                                ",
"                                ",
"15,15"
};


static SDL_Cursor *init_system_cursor(const char *image[])
{
	printf("init_system_cursor\n");
	int i, row, col;
	Uint8 data[4*32];
	Uint8 mask[4*32];
	int hot_x, hot_y;

	i = -1;
	for ( row=0; row<32; ++row )
	{
		for ( col=0; col<32; ++col ) 
		{
			if ( col % 8 ) 
			{
		        	data[i] <<= 1;
				mask[i] <<= 1;
			} 
			else
			{
				++i;
				data[i] = mask[i] = 0;
			}
			switch (image[4+row][col])
			{
				case '.':
					data[i] |= 0x01;
					mask[i] |= 0x01;
				break;
				case 'X':
					mask[i] |= 0x01;
				break;
				case ' ':
				break;
			}
		}
	}
#ifdef AJTODO:
	printf("about to sscanf\n");
	//sscanf(image[4+row], "%d,%d", &hot_x, &hot_y);
	hot_x = hot_y = 15;
	printf("hot_x: %d, hot_y: %d\n", hot_x, hot_y);
#endif
	return SDL_CreateCursor(data, mask, 32, 32, hot_x, hot_y);
}

#ifdef _TINSPIRE
#include "CursorGraphic.h"
#include "SplitLRGraphic.h"
#include "SplitTBGraphic.h"
SDL_Surface* g_pCursor = NULL;

SDL_Surface* GetCursorSurface()
{
   return g_pCursor;
}
#endif

void setcursor(CursorType c)
{
	// FIX -- store cursors rather than converting each time!!!
	
	static CursorType current=CURSORDEFAULT;
	if (c != current)
	{
		clearCursor();
		switch (c)
		{
			case CURSORDEFAULT: //falls through
			case CURSORARROW:
				g_pCursor = nSDL_LoadImage(image_cursor);
				SDL_SetColorKey(g_pCursor, SDL_SRCCOLORKEY, SDL_MapRGB(g_pCursor->format, 0, 0, 0));
				//SDL_SetCursor(init_system_cursor(cursorarrow));
			break;
			case CURSORHORIZONTAL:
				g_pCursor = nSDL_LoadImage(image_Split_LR);
                                SDL_SetColorKey(g_pCursor, SDL_SRCCOLORKEY, SDL_MapRGB(g_pCursor->format, 255, 255, 255));
				//SDL_SetCursor(init_system_cursor(cursorhorizontal));
			break;
			case CURSORVERTICAL:
				g_pCursor = nSDL_LoadImage(image_Split_TB);
                                SDL_SetColorKey(g_pCursor, SDL_SRCCOLORKEY, SDL_MapRGB(g_pCursor->format, 255, 255, 255));
				//SDL_SetCursor(init_system_cursor(cursorvertical));
			break;
			case CURSORCLICK:
				g_pCursor = nSDL_LoadImage(image_cursor);
                                SDL_SetColorKey(g_pCursor, SDL_SRCCOLORKEY, SDL_MapRGB(g_pCursor->format, 0, 0, 0));
				//SDL_SetCursor(init_system_cursor(cursorclick));
			break;
		}
		current=c;
	}
	
	
}

void clearCursor()
{
	if( g_pCursor )
        {
               printf("free surface\n");
               SDL_FreeSurface(g_pCursor);
        }
}

#ifdef _TINSPIRE
CursorGraphic createCursor()
{
	CursorGraphic c;
	c.image = GetCursorSurface();

	c.geom.x = SCREEN_WIDTH/2;
	c.geom.y = SCREEN_HEIGHT/2;
	c.geom.w = 16;
	c.geom.h = 16;

	c.bgsave = SDL_CreateRGBSurface(SDL_HWSURFACE,c.geom.w,c.geom.h,16,0,0,0,0);

	return c;
}

void updateCursor(CursorGraphic* c)
{
	c->image = GetCursorSurface();
}

void saveBehindCursor(CursorGraphic* c)
{
	SDL_BlitSurface(screen, &(c->geom), c->bgsave, NULL);
}

void drawCursor(CursorGraphic* c)
{
	SDL_BlitSurface(c->image, NULL, screen, &(c->geom));
}

void eraseCursor(CursorGraphic* c)
{
	SDL_BlitSurface(c->bgsave, NULL, screen, &(c->geom));
}

void moveCursor(CursorGraphic* c, int nX, int nY)
{
	c->geom.x = nX-8;
	c->geom.y = nY-8;
}

#endif

