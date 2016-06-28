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
#include <signal.h>
#include <fcntl.h>
#include <tparse.h>
#include <debug.h>
#include <runcmd.h>
#include "run_commands.h"
#include "job.h"

#define PROMPT "@:"

void show_version();
int open_io(pipeline_t *p, int *io);
void close_io(pipeline_t *p, int *io);

list_t *jobs = NULL; /* process list */
int go_on = 1; /* This variable controls the main loop. */
char pwd[MAX_FILENAME]; /* current directory */

int main(int argc, char **argv)
{
    buffer_t *command_line;
    int i, j, k, aux, pid, result;
    int io[3];
    char cmd[RCMD_MAXARGS];
    pipeline_t *pipeline;
    struct sigaction sigact;

    setpgid(0, 0);
    signal (SIGINT, SIG_IGN);
    signal (SIGQUIT, SIG_IGN);

    getcwd(pwd, MAX_FILENAME);
    io[0] = 0;
    io[1] = 1;
    io[2] = 2;
    jobs = new_list(free);

    /*---------------- Checking if options or arguments are passed ------------------*/
    if (argc > 1)
    {
        /* Checking for options */
        if (!strcmp(argv[1], "--version"))
        {
            show_version();
            return 0;
        }
        else if(!strcmp(argv[1], "-c") || !strcmp(argv[1], "--command"))
        {
            if (argc < 3)
            {
                /* Argument missing: gsh -c 'expr' */
                printf("Error: argument missing\n");
                return 1;
            }
            /* Run string */
            return run_commands_from_string(argv[2]);
        }
        else
        {
            /* Run a file */
            return run_commands_from_file(argv[1]);
        }
    }
    
    command_line = new_command_line();
    pipeline = new_pipeline();

    /* This is the main loop for parsing and runing commands */
    while (go_on)
    {
        update_jobs_status();
		printf("%s%s ", pwd, PROMPT);
		fflush(stdout);
        aux = read_command_line(command_line);
        sysfatal(aux < 0);

        if (!parse_command_line(command_line, pipeline))
        {
            if (pipeline->ncommands > 0)
            {
                /* Built-in commands go here */
                if (!strcmp(pipeline->command[0][0], "exit") || !strcmp(pipeline->command[0][0], "quit"))
                {
                    go_on = 0; /* exit shell */
                }
                else if(!strcmp(pipeline->command[0][0], "cd"))
                {
                    if (pipeline->narguments[0] == 1)
                    {
                        printf("Error: missing path argument\n");
                    }
                    else
                    {
                        if(!chdir(pipeline->command[0][1])) {
                            /* success */
                            getcwd(pwd, MAX_FILENAME);
                        }
                        else { /* fail */
                            printf("Error: %s\n", strerror(errno));
                        }
                    }
                }
                else if(!strcmp(pipeline->command[0][0], "fg"))
                {
                    list_node_t *n = jobs->first;
                    if(!n) {
                        printf("No such job\n");
                    }
                    else {
                        if(pipeline->command[0][1]) {
                            int num = atoi(pipeline->command[0][1]);
                            if(num < 0) {
                                printf("Error: number must be positive\n");
                                continue;
                            }
                            i = 1;
                            while(n && i < num)
                            {
                                n = n->next;
                                i++;
                            }
                        }
                        if(n) {
                            set_job_foreground(n);
                        }
                    }
                }
                else if(!strcmp(pipeline->command[0][0], "bg"))
                {
                    list_node_t *n = jobs->first;
                    if(!n) {
                        printf("No such job\n");
                    }
                    else {
                        if(pipeline->command[0][1]) {
                            int num = atoi(pipeline->command[0][1]);
                            if(num < 0) {
                                printf("Error: number must be positive\n");
                                continue;
                            }
                            i = 1;
                            while(n && i < num)
                            {
                                n = n->next;
                                i++;
                            }
                        }
                        if(n != NULL) {
                            set_job_background(n);
                        }
                    }
                }
                else if(!strcmp(pipeline->command[0][0], "jobs"))
                {
                    list_node_t *n = jobs->first;
                    i = 1;
                    while(n)
                    {
                        printf("[%d]\t%d\t%s\n", i, n->value->pid, n->value->name);
                        n = n->next;
                        i++;
                    }
                }
                else
                {
                    /* Run pipe if pipeline->ncommands > 1 */
                    if(pipeline->ncommands > 1)
                    {
                        char *cmd1, cmd2[RCMD_MAXARGS];
                        int pfd[2];
                        cmd1 = &cmd[0];
                        io[0] = 0;
                        for(i = 0; i < pipeline->ncommands; i += 2)
                        {
                            memset(cmd1, 0, sizeof (cmd1)); /* cleaning cmd buffers */
                            memset(cmd2, 0, sizeof (cmd2));

                            for (j = 0; pipeline->command[i][j]; j++)
                            {
                                strcat(cmd1, pipeline->command[i][j]);
                                strcat(cmd1, " ");
                            }
                            for (j = 0; pipeline->command[i+1][j]; j++)
                            {
                                strcat(cmd2, pipeline->command[i+1][j]);
                                strcat(cmd2, " ");
                            }

                            /* creating pipe */
                            if( pipe(pfd) < 0 ) {
                                printf("Error: %s\n", strerror(errno));
                                return -1; /* error */
                            }

                            if(open_io(pipeline, io) < 0) {
                                printf("Error: %s\n", strerror(errno));
                                continue;
                            }
                            run_pipe(pfd, cmd1, cmd2, RUN_FOREGROUND(pipeline), io);
                            close_io(pipeline, io);
                        }
                    }
                    else
                    {
                        memset(cmd, 0, sizeof (cmd)); /* cleaning cmd buffer */
                        for (i = 0; pipeline->command[i][0]; i++)
                        {
                            for (j = 0; pipeline->command[i][j]; j++)
                            {
                                strcat(cmd, pipeline->command[i][j]);
                                strcat(cmd, " ");
                            }
                        }
                        
                        if(open_io(pipeline, io) < 0) {
                            printf("Error: %s\n", strerror(errno));
                            continue;
                        }
                        runcmd(cmd, RUN_FOREGROUND(pipeline), &result, io);
                        close_io(pipeline, io);
                    }
                }
            }
        }
    }
    release_command_line(command_line);
    release_pipeline(pipeline);
    release_list(jobs);
    return EXIT_SUCCESS;
}

void show_version()
{
    printf("GhostShell v%s\n", "0.1");
}

int open_io(pipeline_t *pipeline, int *io)
{
    /* Redirect input */
    if (REDIRECT_STDIN(pipeline))
    {
        io[0] = open(pipeline->file_in, O_RDONLY);
        if (io[0] < 0) {
            return -1;
        }
    }
    /* Redirect output */
    if (REDIRECT_STDOUT(pipeline))
    {
        io[1] = open(pipeline->file_out, O_CREAT|O_TRUNC|O_WRONLY, 0644);
        if (io[1] < 0) {
            return -1;
        }
    }
    return 1; /* sucesso */
}

void close_io(pipeline_t *pipeline, int *io)
{
    if(REDIRECT_STDIN(pipeline)) {
        close(io[0]);
        io[0] = -1;
    }
    if(REDIRECT_STDOUT(pipeline)) {
        close(io[1]);
        io[1] = -1;
    }
}
