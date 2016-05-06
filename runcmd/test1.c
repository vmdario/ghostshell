/* test1.c - Test file for libruncmd library
   Copyright (c) 2016, Victor Municelli Dario <victor.munidario@gmail.com>

   This file is part of GhostShell

   This program is free software: you can redistribute it and/or modify
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
#include <stdio.h>
#include <stdlib.h>
#include <runcmd.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int result;
	/* Commands separated by spaces  */
	char command[RCMD_MAXARGS] = {0};

	{  /* Arguments from argv are passed into command array */
		int i;
		for (i = 1; i < argc; ++i) {
			strcat(command, argv[i]);
			strcat(command, " ");
		}
		/* printf("Commands: %s\n", command); */
	}

	result = 0;
	runcmd(command, &result, NULL);

	printf("-----------------------------------------------------------------------\n"
		"Subprocess:\n- Terminated normally: %d\n"
		"- Nonblocking mode: %d\n"
		"- Command executed effectively: %d\n", IS_NORMTERM(result), IS_NONBLOCK(result), IS_NORMTERM(result));

	return 0;
}