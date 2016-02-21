/*
* IceBreaker
* Copyright (c) 2000-2001 Matthew Miller <mattdm@mattdm.org>
*  http://www.mattdm.org/
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
typedef enum { CURSORDEFAULT, CURSORCLICK, CURSORARROW, CURSORHORIZONTAL, CURSORVERTICAL } CursorType; 

#ifdef _TINSPIRE
typedef struct
{
	SDL_Surface *image;
	SDL_Surface *bgsave;
	SDL_Rect geom;
} CursorGraphic;

extern CursorGraphic createCursor();
extern void updateCursor(CursorGraphic* c);
extern void saveBehindCursor(CursorGraphic* c);
extern void drawCursor(CursorGraphic* c);
extern void eraseCursor(CursorGraphic* c);
extern void moveCursor(CursorGraphic* c, int nX, int nY);
#endif

extern void setcursor(CursorType c);
extern void clearCursor();

extern SDL_Surface* GetCursorSurface();
