/* main.c

         Copyright (c) 2016, Victor Municelli Dario <vmunidario@usp.br>

         This file is part of GhostShell.

         GhostShell is free software: you can redistribute it and/or modify
         it under the terms of the GNU General Public License as published by
         the Free Software Foundation, either version 3 of the License, or
         (at your option) any later version.

         This program is distributed in the hope that it will be useful,
         but WITHOUT ANY WARRANTY; without even the implied warranty of
         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
         GNU General Public License for more details.

         You should have received a copy of the GNU General Public License
         along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <tparse.h>
#include <debug.h>
#include <runcmd.h>

#define PROMPT "@:"

/* void test(void); */

int go_on = 1; /* This variable controls the main loop. */

int main(int argc, char **argv)
{
    buffer_t *command_line;
    int i, j, aux, pid, result;
    int *io = NULL;
    char cmd[RCMD_MAXARGS];
    char pwd[MAX_FILENAME];
    pipeline_t *pipeline;
    
    command_line = new_command_line();
    pipeline = new_pipeline();
    getcwd(pwd, MAX_FILENAME);

    /* This is the main loop. */
    while (go_on) {
        /* Prompt. */
        printf("%s%s ", pwd, PROMPT);
        fflush(stdout);
        aux = read_command_line(command_line);
        sysfatal(aux < 0);

        if (!parse_command_line(command_line, pipeline)) {
            
            if (pipeline->ncommands > 0)
            {
                /*
                    Built-in commands go here
                */
                if (!strcmp(pipeline->command[0][0], "exit") || !strcmp(pipeline->command[0][0], "quit")) {
                    go_on = 0; /* exit shell */
                }
                else if(!strcmp(pipeline->command[0][0], "cd")) {
                    if (pipeline->narguments[0] == 1) {
                        printf("Error: missing path argument\n");
                    }
                    else {
                        if(!chdir(pipeline->command[0][1])) {
                            /* success */
                            getcwd(pwd, MAX_FILENAME);
                        }
                        else { /* fail */
                            printf("Error: %s\n", strerror(errno));
                        }
                    }
                }
                else if(!strcmp(pipeline->command[0][0], "fg")) {
                    
                }
                else if(!strcmp(pipeline->command[0][0], "bg")) {
                    
                }
                else if(!strcmp(pipeline->command[0][0], "jobs")) {
                    
                }
                else {
                    memset(cmd, 0, sizeof (cmd)); /* cleaning cmd buffer */
                    for (i = 0; pipeline->command[i][0]; i++) {
                        for (j = 0; pipeline->command[i][j]; j++) {
                            strcat(cmd, pipeline->command[i][j]);
                            strcat(cmd, " ");
                        }
                    }
                    /*printf("DBG - COMMAND: %s-----------------------------------------------------\n", cmd);*/

                    runcmd(cmd, &result, io);
                    if( result == -1 )
                        printf("Error: %d\n", errno);
                }
            }
        }
    }
    release_command_line(command_line);
    release_pipeline(pipeline);
    return EXIT_SUCCESS;
}
