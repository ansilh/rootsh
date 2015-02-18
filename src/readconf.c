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
#include <stdlib.h>
#include <string.h>
#include "config.h"
extern char *readconf(void);
char *readconf()
{
        FILE *stream;
        char *user = NULL;
        char *shell = NULL;
        char *shell_name;
        char *user_name;
        int c,usr_nm_flg,size;
        int close_err=2,i=0;
        char *default_shell;
        char config_file[]="/etc/rootsh.conf";
        default_shell  = (char *)malloc((sizeof(char) * strlen(DEFAULTSHELL)) + 1);
        default_shell = DEFAULTSHELL;
        /*
        // We will use /etc/rootsh.conf as configuration file
        // The Configuration file that contains user_name:/path/to/shell format
        */
        usr_nm_flg=0;
        size=10;
        /*
        // Initialize memory for Shell and User variables
        // That are going to be read from config file
        */

        /*
        // Get the environmental varibles SHELL_CLIENT and USER
        // The values will be stored for logging to syslog as well as custom
        */
        shell_name=getenv("SSH_CLIENT") != NULL ? getenv("SSH_CLIENT"):"local";
        user_name=getenv("LOGNAME") != NULL ? getenv("LOGNAME") : "noUser";
        /*
        // The file is opened in read-only mode
        */
        stream = fopen(config_file,"r");
                if(!stream)
                {
                        fprintf(stderr,"Unable to read config file\n");
                        return default_shell;
                }
        while((c = getc(stream)) != EOF )
        {
        /*
        // Initialize the shell and user memory variables,
        */
        shell=(char *)malloc( sizeof(char) * size);
        user=(char *)malloc( sizeof(char) * size);


        /*
        // Read each charecter
        */
                while( c != '\n' && c != EOF)
                        {



                                        /*
                                        // Check for the delimiter :
                                        */
                                        if( c == ':')
                                        {
                                                /*
                                                // If we found more than on occurance of delimiter ':'
                                                // Error in file syntax
                                                */
                                                if(usr_nm_flg == 1)
                                                {
                                                        fprintf(stderr,"More than one USER/SHELL part\n");
                                                        return default_shell;
                                                }
                                                /*
                                                // The current charecter need to be skipped because
                                                // It contains the delimitter ':'
                                                */
                                                c = getc(stream);
                                                /*
                                                // Syntax error in file NO SHELL Part
                                                */
                                                if(c == '\n' && usr_nm_flg == 0)
                                                {
                                                        fprintf(stderr,"We didn't find the shell entry in file\n");
                                                        return default_shell;
                                                }
                                                /*
                                                // End the user varible with a Null charecter so that
                                                // we can user sting manipulation functions on user variable
                                                */
                                                user[i] = '\0';


                                                /*
                                                // Set a Flag for user name ,thet means the next loop will be stared for
                                                // Shell variable */

                                                usr_nm_flg=1;
                                                i=0;
                                                size=1;
                                        }

                                        if(usr_nm_flg != 1)
                                        {
                                                /* Append the charecter to user variable */
                                                user[i] = c;
                                                /*If we are out of memmory ,then re allocate memmory */
                                                if( i >= size)
                                                {
                                                        size=size+10;
                                                        user = (char *)realloc(user,sizeof(char) * size);
                                                }
                                        }
                                        /*
                                        // If we got a : means the user part is ended and
                                        // path to shell is starting
                                        */
                                        else
                                        {
                                                if(c != 10 )
                                                {
                                                /*Append the charecter to shell variable*/
                                                        shell[i] = c;
                                                /*If we are out of memmory ,then re allocate memmory */
                                                        if( i >= size)
                                                        {
                                                                size=size+10;
                                                                shell = (char *)realloc(shell,sizeof(char) * size);
                                                        }
                                                }
                                        }
                                        /*Read the next charecter*/
                                        c = getc(stream);
                                        /*
                                        // Syntax error in file NO USER Part
                                        */
                                         if(  user[0] == '\0' && usr_nm_flg == 1)
                                        {
                                                fprintf(stderr,"We didn't find the user entry in file\n");
                                                return default_shell;
                                        }
                                        i++;
                        }

                        /*
                        // End the shell varible with a Null charecter so that
                        // we can user sting manipulation functions on user variable
                        */
                        shell[i]= '\0';
                        /*
                        // Compare the user name that we got from config file and
                        // the value got from SHELL environment variable
                        */
                        if(!strcmp(user,user_name))
                        {
                                /* We will break here because we found the user*/
                                break;
                        }
                        else
                        {

                                /*
                                // We didn't find the user and there is more lined to read
                                // Free the user and shell memmory data
                                // Change the size varible to original value,
                                // Turn off user name flag
                                */
                                i=0;
                                size=1;
                                free(shell);
                                free(user);
                                usr_nm_flg=0;
                        }
        }
        /*
        // Close the file
        */
        close_err = fclose(stream);
        /*
        // Usual Error handling while closing the file
        */

        if(close_err != 0)
        {
                fprintf(stderr,"File clould not be closed\n");
                return default_shell;
        }
        /*
        // The user is not found in configuration file so use default
        */
        if(strlen(user) == 0 && strlen(shell) == 0)
        {
                fprintf(stderr,"No user found on configuration file So using default\n");
                return default_shell;
        }
        if(strcmp(user,user_name))
        {
                fprintf(stderr,"No shell defined,using default\n");
                return default_shell;

        }

        /*printf("User is %s and shell is %s\n",user,shell);*/
        return shell;
}

