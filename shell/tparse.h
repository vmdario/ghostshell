/* tparse.h - A trivial command line parser example.

   Copyright (c) 2015, Monaco F. J. <moanco@icmc.usp.br>

   This file is part of POSIXeg.

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


#ifndef TPARSE_H
#define TPARSE_H

#define MAX_COMMANDS  512  /* Number of commands in a pipeline. */
#define MAX_ARGUMENTS 512  /* Number of arguments per command in a pipeline. */

#define MAX_FILENAME 1024  /* Maximum length of a filename IO redirection).*/

/* Struct to read the command line. */

typedef struct buffer_t
{
  int size;			/* Current buffer size. */
  int length;			/* Current string lenght. */
  char *buffer;			/* String. */
} buffer_t;

/* Return a pointer to a newly allocated and properly initialized 
   buffer_t block or NULL on error. */

buffer_t * new_command_line (void);

/* Release all the memory used by a buffer_t block. */

void release_command_line (buffer_t *);

/* Read a line from STDIN into a buffer_t block. The buffer
   grows as needed in steps of BUFFER_STEP bytes. Buffer does not
   shrinks autmatically.*/

int read_command_line (buffer_t *); 

#define FOREGROUND 0		/* Run in foregroud. */
#define BACKGROUND 1		/* Run in background. */

/* Struct representing a pipeline. */

typedef struct pipeline_t
{
  char ***command;		/* Command line (grows unboundedly). */
  char file_in[MAX_FILENAME];	/* Redirect input from this file. */
  char file_out[MAX_FILENAME];	/* Redirect output to this file. */
  int ground;			/* Either FOREGROUND or BACKGROUND. */
  int ncommands;		/* Number of commands */
  int narguments[MAX_COMMANDS];	/* Number of arguments in each command. */
} pipeline_t;


/* Return a pointer to a newly allocated and properly initialized
   pipeline_t block, or NULL on error.*/

pipeline_t *new_pipeline (void);

/* Release memory used by a pipeline_t block. */

void release_pipeline (pipeline_t *);

/* Parse a command line stored in a buffer_t struct and fill in the
   corresponding fields of the target pipeline_t struct. */

int parse_command_line (buffer_t*, pipeline_t*);

/* Handy macros to check execution mode. */

#define RUN_FOREGROUND(pipeline) (pipeline->ground == FOREGROUND)
#define RUN_BACKGROUND(pipeline) (pipeline->ground == BACKGROUND)
#define REDIRECT_STDIN(pipeline) (pipeline->file_in[0] != '\0')
#define REDIRECT_STDOUT(pipeline) (pipeline->file_out[0] != '\0')

/* Output information of pipeline for debugging purposes. */

void pripeline_info (pipeline_t *pipeline);


char * stringdup (const char *);


#endif	/* TPARSE_H */
