/*

readconf.c contains codes for reading configuration file to set a shell

Copyright (C) 2011 Ansil Hameed

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
static struct termios old, new;
void initTermios(void);
void resetTermios(void);
extern int readuser(char **);

int  readuser(char **UserNamePassed)
{
/* A temporary heap area to store charecters */
char *UserNameLocal = NULL;


int size = 1;
char c;
int i = 0;

/* Initialize the heap */

UserNameLocal=(char *)malloc(sizeof(char) * size);

/* Make terminal to non buffred mode
// The terminal driver is line bufferd by default
*/

/*system("stty -icanon"); */

initTermios();

        /* Continue reading charecter until an ENTER/new line */
        while ((c =getchar()) != '\n')
        {

                /* If the number of charecter enterd is more than index
                // then reallocate memmory to hold new charecters
                */
                if( i <= size )
                {

                        size++;

                        /* Re-allocating memory*/

                        UserNameLocal = (char *)realloc(UserNameLocal,(sizeof(char *) * size) + 1);

                        /* Check for heap overflow*/

                        if(UserNameLocal == NULL)
                        {
                                fprintf(stderr,"Memmory allocation failed\n");
                                return 1;
                        }
                }
                if( c == 127 )
                {

                        if(i != 0)
                        {
                                i--;
                                UserNameLocal[i] == '\0';
                                continue;
                        }


                }
                UserNameLocal[i] = c;
                i++;

        }
        /* Add a NULL charecter to make it as string */
         UserNameLocal[i]='\0';

/*Reset the terminal to line buffered mode
//system("stty icanon");
*/
resetTermios();
/*Swap the offset address of temporary heap to the pointer address of refernced pointer*/
*UserNamePassed=UserNameLocal;
return 0;
}
void initTermios()
{
        /* grab old terminal i/o settings */
    tcgetattr(0, &old);
        /* make new settings same as old settings */
    new = old;
        /* disable buffered i/o */
    new.c_lflag &= ~(ECHOCTL|ICANON);
    /*new.c_cc[VERASE] = 127;
    // new.c_lflag &= echo ? ECHO : ~ECHO;  set echo mode or password mode
    // use these new terminal i/o settings now r
    */
    tcsetattr(0, TCSANOW, &new);
}
void resetTermios()
{
    tcsetattr(0, TCSANOW, &old);
}

