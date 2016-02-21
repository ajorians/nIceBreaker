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

extern void printboard(void);
extern void printwholegrid(void);
extern void printwholemaskgrid();

extern void markgrid(int x, int y, int w, int h, char fillchar);
extern long countcleared(void);
extern void checkempty(int x, int y);
extern int penguinsearch(int i, int j);
extern void floodfill(int x, int y);
extern void squarefill(int x, int y);

