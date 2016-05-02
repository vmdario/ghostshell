# Makefile - A Makefile template by Makefine vr. 0.9
#
# Copyright (c) 2015, Monaco F. J. <moanco@icmc.usp.br>
#
# This file is part of POSIXeg.
#
# POSIXeg is free software: you can redistribute it and/or modify
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


# Directions: 
#
# make		    builds everything
# make clean	    clean up the mess
# make dist	    pack files to distribute
# make distcheck    make dist and check package
# make install      install the libraries
# make uninstall    uninstall the libraries
# make test         perform all runtime tests


# This is generic Makefile template to build multiple binaries and libraries 
# out from a C source code projet. You won't probably need to modify bellow 
# this line, unless you want to modify this template. If you only want to add
# new rules or to modify existing ones, you may edit local.mk, instead.

include config.mk

lib_static = $(lib:%=%.a)              # For building
lib_shared = $(lib:%=%.so)             # For building

# Used for installation
lib_all = $(lib_static) $(lib_shared)  # For installing

inst_headers = $(foreach i, $(lib:%=%_h), $($i))  # For installing

proj_src += $(foreach i, $(lib:%=%_obj), $($i:%.o=%.c))  # For distribution
proj_src += $(foreach i, $(bin:%=%_obj), $($i:%.o=%.c))
proj_src += $(foreach i, $(noinst_bin:%=%_obj), $($i:%.o=%.c))

proj_dep = $(proj_src:%.c=%.d)  # For building

proj_headers += $(filter %.h, $(shell $(CC) $(CPP_FLAGS) $(CPPFLAGS) -I. -MM -MG -MT " " $(proj_src)) )
proj_headers += $(inst_headers) # For distribution

EXTRA_DIST += Makefile config.mk local.mk test.mk README COPYING AUTHORS 

#
# Rules
#

# Build everything


all static : $(gendist_files) $(lib_all) $(bin) $(noinst_bin) 

.SECONDEXPANSION:

# Pattern rules for building libraries

$(lib_static) : %.a : $$($$*_obj)
	$(AR) rcs $@ $^

$(lib_shared) : %.so : $$($$*_obj:.o=-pic.o)
	$(CC) -shared $(LD_FLAGS) $(LDFLAGS) -L. $^ -o $@


# Pattern rules for building binaires (uses secondary expansion).
# By default, both static and shared libraries are built and installed. 
# The binary is dynalically linked. If the executables are to be linked 
# against the static libraries, the target'static' produces this result. 
# This does not prevent the building and installation of the shared 
# libraries, anyway.

ifneq (,$(filter static, $(MAKECMDGOALS))) 
#
# Statically linked
#

$(bin) : % : $$($$*_obj)  | $$(foreach i, $$($$*_lib), $$i.a)
	$(CC) $(LD_FLAGS) $(LDFLAGS) -L.  $^ -Wl,-Bstatic $(|:lib%.a=-l%) -Wl,-Bdynamic $(patsubst lib%, -l%, $($*_lib_other))  -o $(@:%=.%)
	echo "#!/bin/sh\n./$(@:%=.%)" > $@
	chmod u+x $@

$(noinst_bin): % : $$($$*_obj)  | $$(foreach i, $$($$*_lib), $$i.a)
	$(CC) $(LD_FLAGS) $(LDFLAGS) -L.  $^ -Wl,-Bstatic $(|:lib%.a=-l%) -Wl,-Bdynamic $(patsubst lib%, -l%, $($*_lib_other))  -o $@
	chmod u+x $@

else
#
# Dynamically linked
#

$(bin) : % : $$($$*_obj)  | $$(foreach i, $$($$*_lib), $$i.so)
#	if ! -d $(PREFIX) ; then install -d $(PREFIX) ; fi 
#	$(CC) $(LD_FLAGS) $(LDFLAGS) -L. -Wl,-rpath=$(realpath $(PREFIX))/lib $^ $(|:lib%.so=-l%) $(patsubst lib%, -l%, $($*_lib_other))  -o $(@:%=.%)
	$(CC) $(LD_FLAGS) $(LDFLAGS) -L. -Wl,-rpath='$$ORIGIN/../lib' $^ $(|:lib%.so=-l%) $(patsubst lib%, -l%, $($*_lib_other))  -o $(@:%=.%)
	echo "#!/bin/sh\nLD_LIBRARY_PATH=. ./$(@:%=.%)" > $@
	chmod u+x $@

$(noinst_bin): % : $$($$*_obj)  | $$(foreach i, $$($$*_lib), $$i.so)
#	if ! -d $(PREFIX) ; then install -d $(PREFIX) ; fi 
	$(CC) $(LD_FLAGS) $(LDFLAGS) -L. -Wl,-rpath=. $^ $(|:lib%.so=-l%) $(patsubst lib%, -l%, $($*_lib_other))  -o $@
	chmod u+x $@


endif

#
# New scheme
#


# Pattern rules sufix-rules fror object-to-source dependency

%.o : %.c
	$(CC) $(CPP_FLAGS) $(CPPFLAGS) -I. $(C_FLAGS) $(CFLAGS) -c  $< -o $@

%-pic.o : %.c
	$(CC) $(CPP_FLAGS) $(CPPFLAGS) -I. $(C_FLAGS) $(CFLAGS) -c  -fPIC $< -o $@

# Atuomatically calculate header dependences

%.d : %.c
	$(CC) $(CPP_FLAGS) $(CPPFLAGS) -I. -MM -MG -MT '$(<:.c=.o) $@' $< > $@

%-pic.d : %.c
	$(CC) $(CPP_FLAGS) $(CPPFLAGS) -I. -MM -MG -MT '$(<:.c=-pic.o) $@' $< > $@

ifeq (,$(filter clean uninstall, $(MAKECMDGOALS))) # Don't do it if not needed
-include $(proj_dep)
endif


.PHONY: clean dist distcheck install uninstall

clean: tests_cleanup
	rm -f *.o *.d $(lib_static) $(lib_shared) $(bin) $(noinst_bin) $(bin:%=.%) $(noinst_bin:%=.%)
	rm -f $(EXTRA_GARBAGE) 
	rm -rf $(PROJECT) $(PROJECT).tgz


# Distribution

dist: $(PROJECT).tgz

$(PROJECT).tgz : $(proj_src) $(proj_headers) $(EXTRA_DIST) $(gendist_files)
	rm -rf $(PROJECT)
	mkdir $(PROJECT)
	cp $^ $(PROJECT)
	tar zcvf $(PROJECT).tgz $(PROJECT)

distcheck: dist
	if $(MAKE) -C $(PROJECT) ; then echo "\n$(PROJECT).tgz ready for distribution"; \
	else echo "\nSomething wrong with $(PROJECT).tgz"; fi; echo 

# Install, uninstall

install: INSTALL=yes
install : $(lib_all) $(bin) $(inst_headers) 
	install -d $(PREFIX)/lib
	install -d $(PREFIX)/include
	install -d $(PREFIX)/bin
	cp $(lib_all) $(PREFIX)/lib/
	cp $(inst_headers) $(PREFIX)/include/
	for i in $(bin:%=.%); do if test -f $$i ; then cp $$i $(PREFIX)/bin/$${i#.}; fi; done

uninstall: 
	for i in $(lib_all) ; do rm -f $(PREFIX)/lib/$$i; done
	for i in $(inst_headers) ; do rm  -f $(PREFIX)/include/$$i; done
	for i in $(bin) ; do rm -f $(PREFIX)/bin/$${i#.}; done


# Run tests,  if any

test:	all tests_setup
	@echo "\n----------------------------------------------------------------------"
	@echo "\nRunning tests\n"
	@total=0;\
	for i in $(tests-bin); do echo;\
	./$$i; nerrors=$$?; total=$$(( $$total + $$nerrors)); done; \
	echo; \
	if test $$total -eq 0 ; then echo -n "Good! All tests ok!"; else echo -n "Found $$total error(s). Some tests failed."; fi;
	@echo; echo

.PONY : tests_setup


# Submakefiles

#-include gendist.mk # Distributed produced files (see gendist.mk)

-include local.mk  # Project-specific rules, if any.

-include test.mk  # Set up for tests, if any.

local.mk : 
	touch $@

#gendist.mk local.mk : 
#	touch $@
