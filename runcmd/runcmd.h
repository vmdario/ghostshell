/* runcmd.h - Header of libruncmd.

   Copyright (c) 2014, Francisco José Monaco <moanco@icmc.usp.br>

   This file is part of POSIXeg

   POSIXeg is free software: you can redistribute it and/or modify
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

#ifndef RUNCMD_H
#define RUNCMD_H

/* Definitions for the command line parser. */

#define RCMD_MAXARGS   1024	/* Max number of arguments. */
#define RCMD_DELIM    " \n\t\r" /* Command line delimiters.   */
#define RCMD_NONBLOCK '&'	/* Command line nonblock sign.*/

/* Bit mask for interpreting command execution results. */

#define NORMTERM    (1 <<  8)	/* 256 */
#define EXECOK      (1 <<  9)	/* 1024 */
#define NONBLOCK    (1 << 10)	/* 2048 */
#define RETSTATUS   (0xFF)	/* 255 */

/*  
 *   This is the user API.
 */

/* Macros to obtain information on command execution results
   reported by 'runcmd' call.

   IS_NORMTERM(result) returns true if the subprocess has terminated 
                       normally; false otherwise.
   IS_NONBLOCK(result) returns true if the subprocess has been executed
                       in nonblocking mode; false otherwise.
   IS_EXECOK(result)   returns true if 'command' has been effectively 
		       executed; false otherwise.
                       the subprocess; false otherwise.
   EXITSTATUS(result)  returns the exit status returned by the
                       subproccess.

*/

#define IS_NORMTERM(result) ((result & NORMTERM) && 1) 
#define IS_NONBLOCK(result) ((result & NONBLOCK) && 1) 
#define EXITSTATUS(result)  ( result & RETSTATUS)
#define IS_EXECOK(result)   ((result & EXECOK) && 1)

/* Subprocess' exit status upon exec failure.*/

#define EXECFAILSTATUS 127

/* Run a command in a subprocess. */

int runcmd (const char *command, int *result, int *io);

/* Hanlder for SIGCHLD in nonblock mode. */

extern void (*runcmd_onexit)(void);

#endif	/* RUNCMD_H */
