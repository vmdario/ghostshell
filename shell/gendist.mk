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
# those generated distributed files and is included by the main project's 
# Makefile. So far soo good. The problem is that the main project's Makefile is
# packed into the distribution and we do not want the rules in this makefile to
# be triggered in the  building enviroment.  To thad end, what we do here is not
# to include this makefile in the distribution and add a rule in the  main 
# makefile to generated it if not found.  The trick is to cause the main 
# makefile to produce an empty file in the build enviroment.           

# Example
EXERNALFILES = debug.h
EXTERNALDIR = ../../utils

$(EXERNALFILES) : % : $(EXTERNALDIR:%/=%)/% 
	@echo "copying $@ from $<"
	cp $< .



