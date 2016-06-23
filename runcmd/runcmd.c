/* runcmd.c - Execute a command as a subprocess.

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

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

#include <runcmd.h>
#include <debug.h>


/* Executes 'command' in a subprocess. Information on the subprocess execution
   is stored in 'result' after its completion, and can be inspected with the
   aid of macros made available for this purpose. Argument 'io' is a pointer
   to an integer vector where the first, second and third positions store
   file descriptors to where standard input, output and error, respective,
   shall be redirected; if NULL, no redirection is performed. On
   success, returns subprocess' pid; on error, returns 0. */
int runcmd (const char *command, int *result, int *io) /* ToDO: const char* */
{
    int pid, status;
    int aux, i, tmp_result;
    char *args[RCMD_MAXARGS], *p, *cmd;
    tmp_result = 0;

    /* Parse arguments to obtain an argv vector. */
    cmd = malloc ((strlen (command) + 1) * sizeof(char));
    sysfail (!cmd, -1);
    p = strcpy (cmd, command);

    i = 0;
    args[i++] = strtok (cmd, RCMD_DELIM);
    while ((i < RCMD_MAXARGS) && (args[i++] = strtok (NULL, RCMD_DELIM)));
    i--;

    /* Create a subprocess. */
    pid = fork();
    sysfail (pid < 0, -1);

    if (pid > 0)      /* Caller process (parent). */
    {
        aux = wait(&status);
        sysfail (aux < 0, -1);

        /* Collect termination mode. */
        if (WIFEXITED(status))
            tmp_result |= NORMTERM;
    }
    else        /* Subprocess (child) */
    {
        execvp(args[0], args);
        printf("Error: %s\n", strerror(errno));
        exit (EXECFAILSTATUS);
    }

    if (result)
        *result = tmp_result;
    free (p);
    return pid;     /* Only parent reaches this point. */
}

/* If function runcmd is called in non-blocking mode, then function
   pointed by rcmd_onexit is asynchronously evoked upon the subprocess
   termination. If this variable points to NULL, no action is performed.
*/
void (*runcmd_onexit)(void) = NULL;

int run_commands_from_string(const char *str)
{

}

int run_commands_from_file(const char *file)
{

}






