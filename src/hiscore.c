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

#include "SDL/SDL.h"
#ifndef _TINSPIRE
#include <stdio.h>
#include <stdlib.h>
#endif
#include "icebreaker.h"
#include "globals.h"
#include "hiscore.h"


char hiscorename[HISCORENUM][50];
long hiscoreval[HISCORENUM];

static char temphiscorename[HISCORENUM+1][50]; //used for sorting
static long temphiscoreval[HISCORENUM+1];

static int cmpscore(int * a, int * b);

void readhiscores()
{
#ifdef _TINSPIRE
	return;
#endif
	printf("Beginning readhiscores\n");
	FILE *hiscorefile;
	char linebuf[50];

	int arrayindex[HISCORENUM];
	int i;

	// fill the "helper" array. 
	for (i=0;i<HISCORENUM;i++)
		arrayindex[i]=i;
		


	// make sure all entries are zeroed out to start.
	for (i=0;i<HISCORENUM;i++)
	{
		printf("about to call snprintf\n");
		snprintf(temphiscorename[i],7,"Nobody");
		temphiscoreval[i]=100; //100 is better than 0. :)
	}

	printf("Shouldn't do this next part\n");
#ifndef AJTODO
	printf("Highscore part\n");
	hiscorefile=fopen(HISCOREPREFIX "/" HISCOREFILE,"r");
	if (hiscorefile==NULL)
	{
		// It's writing we need to worry about, really, so don't
		// complain here.
		//fprintf(stderr,"Can't read high score file; continuing anyway.\nYou may want to ask your sysadmin to make sure this program can access\n " HISCOREPREFIX "/" HISCOREFILE "\n");
	}
	else
	{
		for (i=0;i<HISCORENUM;i++)
		{
			if (fgets(linebuf,50,hiscorefile))
			{
				sscanf(linebuf,"%12s %30ld",temphiscorename[i],&temphiscoreval[i]);
			}
		}
		fclose(hiscorefile);
		
		// sort arrayindex based on the corresponding hiscoreval
		// really, the array should already be sorted. but you never know.	
		qsort(arrayindex, HISCORENUM, sizeof(int), (int (*)(const void*,const void*))cmpscore);

	}
	
	// ok, so now, we can copy things over in the proper sorted order
	for (i=0;i<HISCORENUM;i++)
	{
		snprintf(hiscorename[i],50,temphiscorename[arrayindex[i]]);
		hiscoreval[i]=temphiscoreval[arrayindex[i]];
	}
#endif
}


int checkhiscore(long score)
{
#ifdef _TINSPIRE
	return 0;
#endif
	printf("checkhiscore\n");
	// need to re-read from disk in case another user has obtained
	// a better score in the meantime...
	readhiscores();
	// check to see if score is better than the lowest high score
	return (score>hiscoreval[HISCORENUM-1]);
}

void addhiscore(char * username, long score)
{
#ifdef _TINSPIRE:
	return;
#endif
	printf("addhiscore\n");
	int arrayindex[HISCORENUM+1]; // note the +1 -- we're including the new score
	FILE *hiscorefile;
	int i;


	// make sure the temp array contains the right data
	for (i=0;i<HISCORENUM;i++)
	{
		snprintf(temphiscorename[i],50,hiscorename[i]);
		temphiscoreval[i]=hiscoreval[i];
	}
	
	// and toss in the new data 
	//(this is why these arrays are size HISCORENUM+1)
	snprintf(temphiscorename[HISCORENUM],50,username);
	temphiscoreval[HISCORENUM]=score;

	// fill the "helper" array. 
	for (i=0;i<HISCORENUM+1;i++)
		arrayindex[i]=i;

	
	// ok, now sort those 
	qsort(arrayindex, HISCORENUM+1, sizeof(int), (int (*)(const void*,const void*))cmpscore);

	// and take the top ones back.
	for (i=0;i<HISCORENUM;i++)
	{
		snprintf(hiscorename[i],50,temphiscorename[arrayindex[i]]); 
		hiscoreval[i]=temphiscoreval[arrayindex[i]];
	}

	// writehiscores:

	hiscorefile=fopen(HISCOREPREFIX "/" HISCOREFILE,"w");
	if (hiscorefile==NULL)
	{
		fprintf(stderr,"Can't save high scores.\n");
		fprintf(stderr,"You may want to ask your sysadmin to make sure this program can write to\n<" HISCOREPREFIX "/" HISCOREFILE ">.\n");
	}
	else
	{
		// FIX -- make this go. :)
		//for (i=0;hiscorename[i]!='\0';i++)
		//	if (hiscorename[i]==' ') hiscorename[i]='_';
			
		for (i=0;i<HISCORENUM;i++)
		{
			fprintf(hiscorefile,"%s %ld\n",hiscorename[i],hiscoreval[i]);
		}
		fclose(hiscorefile);
	}
}

int cmpscore(int * a, int * b)
{
	return(temphiscoreval[*b] - temphiscoreval[*a]);
}


