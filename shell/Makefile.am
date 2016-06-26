##   Makefile.am - Automake script for project's libraries.
##
##   Copyright (c) 2014, Francisco Jose Monaco  <moanco@icmc.usp.br>
##
##   This file is part of POSIXeg.
##
##   POSIXeg is free software: you can redistribute it and/or modify
##   it under the terms of the GNU General Public License as published by
##   the Free Software Foundation, either version 3 of the License, or
##   (at your option) any later version.
##
##   This program is distributed in the hope that it will be useful,
##   but WITHOUT ANY WARRANTY; without even the implied warranty of
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##   GNU General Public License for more details.
##
##   You should have received a copy of the GNU General Public License
##   along with this program.  If not, see <http://www.gnu.org/licenses/>.

AM_CPPFLAGS = -I$(srcdir)/shell -I$(top_srcdir)/runcmd
AM_CFLAGS =   @C_FLAGS@ @PTHREAD_CFLAGS@ 
AM_LDFLAGS =  @LD_FLAGS@ 

CC = @PTHREAD_CC@
LADD = @PTHREAD_LIBS@

bin_PROGRAMS = ghostshell

ghostshell_SOURCES = main.c list.c parser.c run_commands.c debug.h list.h tparse.h run_commands.h

ghostshell_LDADD = ../runcmd/libruncmd.la $(LIBOBJS) 


ghostshell_LIBS = @PTHREAD_LIBS@
