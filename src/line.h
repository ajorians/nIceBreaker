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

typedef enum { UP, DOWN, LEFT, RIGHT } LineDir;	
typedef enum { HORIZONTAL, VERTICAL } LineType;

typedef struct
{
	int on;
	SDL_Rect geom;
	SDL_Rect mark;
	LineDir dir;
	Uint32 color;
	char id;
	int dead;
	int speedslower;
	int stuckcount;
} Line;


extern Line createline(char i, Uint8 r, Uint8 g, Uint8 b);
extern void startline(Line * l, LineDir d, int x, int y);
extern int moveline(Line * l);
extern void finishline(Line * l);
extern void killline(Line * l);

