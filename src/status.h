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

extern void updatestatuslives(int lives);
extern void updatestatuscleared(int cleared);
extern void updatestatusscore(long score);

extern void updatehiscorebox();

#define STATUSY (BORDERTOP-(4*CHARHEIGHT))

#define LIVESX   (BORDERLEFT)
#ifdef _TINSPIRE
#define CLEAREDX (LIVESX+(SCREEN_WIDTH/4))
#else
#define CLEAREDX (LIVESX+165)
#endif
#define SCOREX   (BORDERRIGHT-120)

#define LIVESW (CLEAREDX - LIVESX)
#ifdef _TINSPIRE
#define CLEAREDW (WIDTH - CLEAREDX)
#else
#define CLEAREDW (SCOREX - CLEAREDX)
#endif
#define SCOREW  (WIDTH - SCOREX)

#ifdef _TINSPIRE
#define BOTTOMSTATUSY (MARGINTOP+PLAYHEIGHT+CHARHEIGHT)
#else
#define BOTTOMSTATUSY (MARGINTOP+PLAYHEIGHT+CHARHEIGHT*2)
#endif

