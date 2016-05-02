# Project configuration

# This file has been modified by Victor M. Dario <victor.munidario@gmail.com>
# as part of GhostShell project.
#
# GhostShell is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

PROJECT = runcmd

# Name your project's binaires, eg.
# bin = foo bar

# For each item in $(bin), name the objects needed to build it, e.g.
# foo_obj = main_foo.o 
# bar_obj = main_bar.o 

# For each item in $(bin), name the project's libraries to which it shall be linked, e.g.
# foo_lib = libfoobar
# bar_lib = libfoobar libm

# For each item in $(bin), name the other libraries to which it shall be linked, e.g.
# foo_lib_other = libfoobar
# bar_lib_other = libfoobar libm


# Name your project's libraries (to be installed), e.g.
# lib = libfoobar
lib = libruncmd

# For each item in $(lib), name the objects needed to build it, e.g.
# libfoobar_obj = foo.o bar.o
libruncmd_obj = runcmd.o

# For each item in $(lib), name the headers your project installs, e.g.
#libfoobar_h = foos.h
libruncmd_h = runcmd.h

# Name your project's auxuliary binaries (not to be installed), e.g.
# noinst_bin = example
noinst_bin = test1

# For each item in $(noinst_bin), name the objects needed to build it, e.g.
# example_obj = example.o 
test1_obj = test1.o

# For each item in $(noinst_bin), name the libraries to which it's to be linked, e.g.
# example_lib = libfoobar
test1_lib = libruncmd

# Extra files to go in the distribution (add as needed)

EXTRA_DIST =

# List the automatically generated, distributed source files of any kind.
# These files are generated in the development environment but should
# be already available in the building enviroment. E.g. config scripts.

gendist_files = debug.h Makefile

# Installation prefix

PREFIX=./local

# Customize the building environemnt (modify as desired)

CC = gcc
CPP_FLAGS = -Wall --ansi --pedantic-errors -D_POSIX_C_SOURCE=200809L 
C_FLAGS = 
LD_FLAGS = 
MAKE = make
AR = ar

# Housekeeping (name temporary files to clean)
# EXTRA_GARBAGE = *~
EXTRA_GARBAGE = *~ \#*

# Name your tests

tests-bin = test1

