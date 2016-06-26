/*
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
#include <unistd.h>
#include <string.h>
#include <debug.h>
#include <fcntl.h>
#include "runcmd.h"
#include "run_commands.h"
#include "tparse.h"

extern char pwd[MAX_FILENAME];

int run_commands_from_string(const char *str)
{
	printf("%s\n", str);
	return 0;
}

int run_commands_from_file(const char *path)
{
	int fd, go_on, aux, i, j, error, result;
	int *io = NULL;
	buffer_t *command_line;
	pipeline_t *pipeline;
	char cmd[RCMD_MAXARGS];

	fd = open(path, O_RDONLY);
	if(fd < 0)
	{
		printf("Error: %s\n", strerror(errno));
		return 1;
	}

	command_line = new_command_line();
	pipeline = new_pipeline();
	error = 0;
	go_on = 1;
	while(go_on)
	{
		aux = read_command_line_from_file(fd, command_line, &error);
		if(error) {
			printf("Error: %s\n", strerror(errno));
			break;
		}
		if(aux < 0) {
			/* EOF */
			go_on = 0;
		}

		/* skip comments */
		if(command_line->buffer[0] == '#') {
			continue;
		}

		if (!parse_command_line(command_line, pipeline))
		{
			if(pipeline->ncommands > 0)
			{
				if (!strcmp(pipeline->command[0][0], "exit") || !strcmp(pipeline->command[0][0], "quit"))
				{
					go_on = 0; /* exit file */
				}
				else if(!strcmp(pipeline->command[0][0], "cd"))
				{
					if (pipeline->narguments[0] == 1)
					{
						printf("Error: missing path argument\n");
						break;
					}
					else
					{
						if(!chdir(pipeline->command[0][1])) {
							/* success */
							getcwd(pwd, MAX_FILENAME);
						}
						else { /* fail */
							printf("Error: %s\n", strerror(errno));
							break;
						}
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

					runcmd(cmd, &result, io);
				}
			}
		}
	}
	release_command_line(command_line);
	release_pipeline(pipeline);
	close(fd);
	return 0;
}
