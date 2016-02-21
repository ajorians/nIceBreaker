/*
* IceBreaker
* Copyright (c) 2001 Matthew Miller <mattdm@mattdm.org> and
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

#include "SDL/SDL.h"
#ifndef _TINSPIRE
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#endif
#include "icebreaker.h"
#include "globals.h"
#include "icebreaker.h"
#include "options.h"

GameOptionsType options;


void setdefaultoptions(void)
{
        options.sound=SOUNDON;
	options.autopause=AUTOPAUSEOFF;	
        options.difficulty=NORMAL;
        options.fullscreen=FULLSCREENOFF;
        strncpy(options.theme,"linux",MAXTHEMELENGTH);
}


int readoptions(void)
{
	FILE * optionfile;
	char linebuf[50];
	char filename[255];
	char optbuf[20];
	char valbuf[10];
	int i;

	setdefaultoptions();
	
	snprintf(filename,255,"%s/%s",homedir,OPTIONFILE);
	
	optionfile=fopen(filename,"r");
	if (optionfile==NULL)
	{
		fprintf(stderr, OPTIONFILE " doesn't exist.\nWelcome to IceBreaker.\n");
		return true;
	}
	
	while(fgets(linebuf,50,optionfile))
	{
		for (i=0;i<50;i++)
		{
			if (linebuf[i]=='\0') break;
			linebuf[i]=tolower(linebuf[i]);
		}
			
		if (sscanf(linebuf,"%20s %10s",optbuf,valbuf)==2)
		{
			if (!strcmp(optbuf,"sound"))
			{
				if (!strcmp(valbuf,"on"))
					options.sound=SOUNDON;
				else if (!strcmp(valbuf,"off"))
					options.sound=SOUNDOFF;
			}
			else if (!strcmp(optbuf,"autopause"))
			{
				if (!strcmp(valbuf,"on"))
					options.autopause=AUTOPAUSEON;
				else if (!strcmp(valbuf,"off"))
					options.autopause=AUTOPAUSEOFF;
			}
			else if (!strcmp(optbuf,"fullscreen"))
			{
				if (!strcmp(valbuf,"off"))
					options.fullscreen=FULLSCREENOFF;
				else if (!strcmp(valbuf,"on"))
					options.fullscreen=FULLSCREENON;
				else if (!strcmp(valbuf,"always"))
					options.fullscreen=FULLSCREENALWAYS;
			}
			else if (!strcmp(optbuf,"difficulty"))
			{
				if (!strcmp(valbuf,"normal"))
					options.difficulty=NORMAL;
				else if (!strcmp(valbuf,"hard"))
					options.difficulty=HARD;
				else if (!strcmp(valbuf,"easy"))
					options.difficulty=EASY;
			}
			// FIX: add username
		}
	}
	
	fclose(optionfile);

	return false;
}

int writeoptions(void)
{
	FILE * optionfile;
	char filename[255];
	snprintf(filename,255,"%s/%s",homedir,OPTIONFILE);
	
	optionfile=fopen(filename,"w");
	if (optionfile==NULL)
	{
		fprintf(stderr, "Can't write to " OPTIONFILE ".\n");
		return true;
	}
	
	fprintf(optionfile,"# Icebreaker config file 1.0\n#\n");
	fprintf(optionfile,"# Separate keywords from values by whitespace. Not case sensitive.\n#\n");
	fprintf(optionfile,"# %s/" OPTIONFILE " will be overwritten automatically.\n#\n",homedir);
	
	fprintf(optionfile,"\n# Change this if the crashing noise annoys your neighbors.\n");
	if (options.sound==SOUNDON)
		fprintf(optionfile,"Sound On\n");
	else if (options.sound==SOUNDOFF)
		fprintf(optionfile,"Sound Off\n");
		
	fprintf(optionfile,"\n# AutoPause makes the game pause when the window is out of focus.\n");
	if (options.autopause==AUTOPAUSEON)
		fprintf(optionfile,"AutoPause On\n");
	else if (options.autopause==AUTOPAUSEOFF)
		fprintf(optionfile,"AutoPause Off\n");

	fprintf(optionfile,"\n# Set FullScreen to Always if you want it that way every time.\n");
	fprintf(optionfile,"# On will use full screen mode once, but then change back to Off.\n");
#ifdef HIDEFULLSCREEN // FIX -- put back fullscreen
	fprintf(optionfile,"# This is experimental here, so you have to change it here -- there's nothing\n");
	fprintf(optionfile,"# on the options menu. A future version will have complete fullscreen support.\n");
#endif
	if (options.fullscreen==FULLSCREENOFF || options.fullscreen==FULLSCREENON)
		fprintf(optionfile,"FullScreen Off\n");
	else if (options.fullscreen==FULLSCREENALWAYS)
		fprintf(optionfile,"FullScreen Always\n");

	fprintf(optionfile,"\n# Normal is the best way to play. Easy is okay to get started,\n");
	fprintf(optionfile,"# but you won't get very high scores. Hard is for those who really\n");
	fprintf(optionfile,"# want a challenge, but scores only slightly higher than normal.\n");
	

	if (options.difficulty==NORMAL)
		fprintf(optionfile,"Difficulty Normal\n");
	else if (options.difficulty==HARD)
		fprintf(optionfile,"Difficulty Hard\n");
	else if (options.difficulty==EASY)
		fprintf(optionfile,"Difficulty Easy\n");

	
	fclose(optionfile);
	
	return false;
}
