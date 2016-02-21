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

#define VERSION 1.2.1
#define VERMAJOR 1
#define VERMINOR 2
#define VERSUB 1


#ifndef HISCOREPREFIX
#define HISCOREPREFIX "/var/lib/games"
#endif

#define HISCOREFILE "icebreaker.scores"

#ifndef DATAPREFIX 
#define DATAPREFIX "/usr/local/share/icebreaker"
#endif

#ifndef OPTIONFILE
  #ifdef WIN32
    #define OPTIONFILE "icebreaker.cfg"
  #else
    #define OPTIONFILE ".icebreaker"
  #endif
#endif  

#define SNDFILEOUCH "ouch.wav"
#define SNDFILEBREAK "crash.wav"
#ifdef _TINSPIRE
#define PENGUINBMPFILE "penguin.bmp.tns"
#else
#define PENGUINBMPFILE "penguin.bmp"
#endif

#ifdef _TINSPIRE
  #define PENGUINICONFILE "icebreaker_48.bmp.tns"
#elif WIN32
  #define PENGUINICONFILE "penguinicon_32.bmp"
#else
  #define PENGUINICONFILE "icebreaker_48.bmp"
#endif

#define BLOCKWIDTH 9
#define BLOCKHEIGHT 8

#define COLS 32
#define ROWS 20

#define PLAYWIDTH (COLS*BLOCKWIDTH)
#define PLAYHEIGHT (ROWS*BLOCKHEIGHT)

#define MARGINTOP 30
#define MARGINBOTTOM 30
#define MARGINLEFT 16
#define MARGINRIGHT 16

/* Centered in 640x480: 
// #define MARGINTOP 100
// #define MARGINBOTTOM 100
// #define MARGINLEFT  96
// #define MARGINRIGHT 96 
*/

#define WIDTH (MARGINLEFT+PLAYWIDTH+MARGINRIGHT)
#define HEIGHT (MARGINTOP+PLAYHEIGHT+MARGINBOTTOM)

#define BORDERTOP    MARGINTOP
#define BORDERBOTTOM (MARGINTOP+PLAYHEIGHT)
#define BORDERLEFT   MARGINLEFT
#define BORDERRIGHT  (MARGINLEFT+PLAYWIDTH)


#define MAXPENGUINS 100

#define PENGUINSPEED 2

#define LINESPEED 2

#define LINEMAXSTUCK 750

#define PERCENTREQUIRED 75
#define PERCENTBONUS 75
#define PERCENTEXTRABONUS 80

#ifndef true
#define true -1
#endif

#ifndef false
#define false 0
#endif

#ifdef WIN32
	#include "win32_compatibility.h"
#endif

#ifdef WIN32
	#define HIDEFULLSCREEN
#endif

