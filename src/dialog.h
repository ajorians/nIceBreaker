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

// FIX -- make this be enum MenuReturnType
#define POPUPMENUDONOTHING 0
#define POPUPMENUEXITMENU 1
#define POPUPMENUQUITGAME 2
#define POPUPMENUNEWGAME 3
#define POPUPMENUREDRAWME 4


extern int gethighusername(int highest);

extern int popupmenu(void);
extern int popuphelp(void);
extern int popuphighscores(void);
extern int popupoptionsmenu(void);

extern int waitforuser(void);


