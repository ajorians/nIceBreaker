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

#include "SDL/SDL.h"
#include "icebreaker.h"
#include "text.h"
#include "string.h"
#include "globals.h"

static char * letterindex;
static char * letterdata[6];


void inittext()
{
	letterindex = " AAAAA BBBBB CCCCC DDDDD EEEEE FFFFF GGGGG HHHHH III JJJJJ KKKK LLLLL MMMMM NNNNN OOOOO PPPPP QQQQQ RRRRR SSSSS TTTTT UUUUU VVVVV WWWWW XXXXX YYYYY ZZZZZ aaaaa bbbbb cccc ddddd eeeee fffff ggggg hhhhh i jjjj kkkk lll mmmmm nnnnn ooooo ppppp qqqqq rrrr sssss tttt uuuuu vvvvv wwwww xxxx yyyyy zzzz 00000 11 22222 33333 44444 55555 66666 77777 88888 99999 ~~~~~~    ``     !    @@@@   #####  $$$$   %%%%%  ^^^^^  &&&&&  *****    ((   ))    _____  -----  +++++  =====   {{{     [[[   }}}   ]]]      |     :     ;;      ''   <<<<<<   ,,   >>>>>>    .    ????   /////    \"\"\" \\\\\\\\\\";
	letterdata[0]="   #   ####   ###  ####  ##### #####  #### #   # ###   ### #  # #     #   # #   #  ###  ####   ###  ####   #### ##### #   # #   # #   # #   # #   # #####       #              #         ###       #     #    # #    ##                                            #                                      ###   # ####  ####     ## #####  ###  #####  ###   ###    #       #      #     ##     # #    ###   ##  #    #     ##      #       #   #                     #              #     ###   #     ###      #                    #       ##        ##             ###        #    # # #    ";
	letterdata[1]="  # #  #   # #   # #   # #     #     #     #   #  #      # # #  #     ## ## ##  # #   # #   # #   # #   # #       #   #   # #   # # # #  # #   # #     #   #### ####   ###  ####  ###   #     #### ####         # ##  #  ## #  # ##   ###  ####   #### # ##  #### #### #   # #   # #   # #  # #   # #### #   # ##     #     #   # # #     #         # #   # #   #  # # #     #     #    # ##   #####  # #    ## #    # #   #      # # #    #     #                    #    #####    #      #      #      #      #     #      #      #      ##            ##              #      #     # #  #   ";
	letterdata[2]=" #   # ####  #     #   # ####  ####  #  ## #####  #      # ##   #     # # # # # # #   # ####  #   # ####   ###    #   #   #  # #  # # #   #     #     #   #   # #   # #    #   # ## ## ####  #   # #   # #    # ##    #  # # # ##  # #   # #   # #   # ##   ###    #   #   #  # #  # # #  ##  #   #   #  #   #  #  ###   ###   #  # ####  ####     #   ###   ####     #            #    # ##    # #    ###     #    #   #   ## #   ###     #     #           #####  #####          ##      #      ##     #      #                        ##                ##          ##      #            #  ";
	letterdata[3]=" ##### #   # #   # #   # #     #     #   # #   #  #  #   # # #  #     # # # #  ## #   # #     #   # #  #      #   #   #   #  # #  ## ##  # #    #    #    #  ## #   # #    #   # ###    #     #### #   # #    # # #   #  # # # #   # #   # #   # #   # #      ###  #   #  ##  # #  # # #  ##   ####  #   #   #  # #         # #####     # #   #   #   #   #     #                       #      #####    # #   # ##         #  #   # # #    #     #                    #    #####    #      #      #      #      #     #      #             ##      #     ##      #            #              # ";
	letterdata[4]=" #   # ####   ###  ####  ##### #      #### #   # ###  ###  #  # ##### #   # #   #  ###  #      ###  #   # ####    #    ###    #   #   # #   #   #   #####  ## # ####   ###  ####  ###   #        # #   # # #  # #  # ### #   # #   #  ###  ####   #### #    ####    ##  ## #   #    # #  #  #     # ####  ###   # ##### ####      # ####   ###   #     ###   ###                   #     ##     # #    ###   #  ##          ## #    #       #   #                     #              #     ###   #     ###      #            #               ##    #   ##        #     #     #                #";
        letterdata[5]="                                                                                                 ##                                                                                           ###           ##                             #         #                                         ###                                                                                                                                                    #####                                                                 #                     #                                            ";
}


// FIX -- should return actual width of box
void puttext(int x, int y, int size, Uint32 color, char * text)
{
	SDL_Rect tmprect;
	unsigned int l;
	int i,j;
	char * indexpointer;
	unsigned char c;
	int w,startx;
	
	if (size>1)
	{
		tmprect.h=size/2;
		tmprect.w=size/2;
	}
	else
	{
		tmprect.h=1; tmprect.w=1;
	}
	startx=x;
		
	for (l=0; l<strlen(text); l++)
	{
		c=*(text+l);
		indexpointer=
#ifdef _TINSPIRE
strchr
#else
index
#endif
			(letterindex,c);
		if (indexpointer!=NULL)
		{
			w=0;
			for (i=indexpointer-letterindex; *(letterindex+i)==c; i++)
			{ // scan through index for location of letter.
			 // FIX -- deal with spaces properly
				for (j=0;j<CHARHEIGHT;j++)
				{
					if (*(letterdata[j]+i) != ' ')
					{
						tmprect.x=startx+w;
						tmprect.y=y+j*size;
						SDL_FillRect(screen,&tmprect,color);
					}	
				}
				w+=size;
			}
			startx+=w+2;
		}
	}
}

