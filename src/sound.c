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

#ifndef _TINSPIRE
#include <SDL.h>
#include <SDL_mixer.h>
#include "icebreaker.h"
#include "sound.h"
#include "options.h"

int soundsystemworks=false;

static Mix_Chunk *sndouch = NULL;
static Mix_Chunk *sndbreak = NULL;


void initsound()
{
	if (SDL_InitSubSystem(SDL_INIT_AUDIO))
	{	
		soundsystemworks=false;
		fprintf(stderr, "Can't start sound subsystem: %s\nContinuing without it.\n", SDL_GetError());
	}	
	else if  (Mix_OpenAudio(11025, (Uint16)AUDIO_U8, 2, 256))
	{
		soundsystemworks=false;
		fprintf(stderr, "Can't open audio: %s\nContinuing without it.\n", SDL_GetError());
	}
	else
	{
		soundsystemworks=true;
		if ( (sndouch=Mix_LoadWAV(DATAPREFIX "/" SNDFILEOUCH)) == NULL)
			fprintf(stderr, "Error loading " DATAPREFIX "/" SNDFILEOUCH "\n%s\n", SDL_GetError());
		if ( (sndbreak=Mix_LoadWAV(DATAPREFIX "/" SNDFILEBREAK)) == NULL)
			fprintf(stderr, "Error loading " DATAPREFIX "/" SNDFILEBREAK "\n%s\n", SDL_GetError());
	}
}


void quitsound()
{
	while (Mix_Playing(-1)); 
	
	if (sndouch) { Mix_FreeChunk(sndouch); sndouch = NULL; }
	if (sndbreak) { Mix_FreeChunk(sndbreak); sndbreak = NULL; }
	
	if (soundsystemworks) Mix_CloseAudio();	
}


void playsound(SoundSample s)
{
	if (soundsystemworks && options.sound==SOUNDON)
	{
		switch (s)
		{
			case SNDOUCH:
				Mix_PlayChannel(-1, sndouch, 0);	
			break;
			case SNDBREAK:
				Mix_PlayChannel(-1, sndbreak, 0);	
			break;
		}
	}
}

#endif
