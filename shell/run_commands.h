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
#ifndef RUNCOMMANDS_H
#define RUNCOMMANDS_H
#include "tparse.h"

int run_commands_from_string(const char *);

int run_commands_from_file(const char *);

int run_pipe(char *cmd1, char *cmd2, int fg, int *io);

int run_pipes(pipeline_t *p, int fg);

#endif
