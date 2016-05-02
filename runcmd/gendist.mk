# Generated, distributed files
#
# These files are generated in the development enviroment and  distributed
# along with other source files so that they are readily available in the 
# building enviroment (where it may not be possible or may not make sense 
# to regenerate them).

# Rationales
#
# When buiding the project in the development enviroment, we want to generated
# some source files, such as automatically produced headers and configure 
# scripts. We, however, do want to to add  these files in the distrubtion 
# since the tools and resources needed to regenerate them in the building 
# enviroment may not be available. This makefile contains the rules to build 
# those generated & distributed files and is included by the main project's
# Makefile. So far soo good. The problem is that the main project's Makefile is
# packed into the distribution and we do not want the rules in this makefile to
# be triggered in the  building enviroment.  To thad end, what we do here is not
# to include this makefile in the distribution and add a rule in the  main 
# makefile to generated it if not found.  The trick is to cause the main 
# makefile to produce an empty file in the build enviroment.           

# In this subproject (runcmd) we want to reuse some header files of the main 
# project (posixeg) . We, however, do not want to add the source header path 
# to the preprocessor because the subproject's makedist rule shall not reach 
# beyond the subproject's directory (runcmd.tgz contains only the subdiretory
# posixeg/runcmd, not /posixeg). We then have a rule rule to copy the reused 
# headers from their original location in the main project's tree to the 
# current build directory every time the main project's header is modified. 
# Since the generated header files (debug.h) is pakced into the distribution
# (runcmd.tgz), this rule will not be needed in the building enviroment.


SRCDIR = ../..

UTILSFILES = debug.h COPYING AUTHORS

$(UTILSFILES) : % : $(SRCDIR:%=%)/utils/% 
	@echo "copying $@ from $<"
	cp $< .

Makefile : $(SRCDIR:%=%)/utils/Makefine/Makefile
	@echo "copying $@ from $<"
	cp $< .
