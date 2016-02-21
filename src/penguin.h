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

typedef struct
{
	SDL_Surface *image;
	SDL_Surface *bgsave;
	SDL_Rect geom;
	int xdelta;
	int ydelta;
	int speedslower;
} Penguin;



extern Penguin createpenguin();
extern Penguin createpenguinxy(int x, int y);
extern void savebehindpenguin(Penguin * p);
extern void drawpenguin(Penguin * p);
extern void erasepenguin(Penguin * p);
extern void movepenguin(Penguin * p);

