/*
* IceBreaker
* Copyright (c) 2000 Matthew Miller <mattdm@mattdm.org> and
*   Enrico Tassi <f.tassi@mo.nettuno.it>
*
* <http://www.mattdm.org/icebreaker/>
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


typedef enum { SOUNDON,SOUNDOFF } SoundSettingType;
typedef enum { AUTOPAUSEON,AUTOPAUSEOFF } AutoPauseType;
typedef enum { NORMAL, EASY, HARD } GameDifficultyType;
typedef enum { FULLSCREENOFF, FULLSCREENON, FULLSCREENALWAYS } FullscreenSettingType;

#define MAXTHEMELENGTH 10

typedef struct
{
	SoundSettingType sound;
	AutoPauseType autopause;
	GameDifficultyType difficulty;
	FullscreenSettingType fullscreen;
        char theme[MAXTHEMELENGTH];
} GameOptionsType;

extern GameOptionsType options;

void setdefaultoptions(void);
extern int readoptions(void);
extern int writeoptions(void);

