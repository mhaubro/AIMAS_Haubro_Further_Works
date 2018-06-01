#
#Should include and execute the makefile for the searchclient, as well as maketable,
#prober etc., as well as make clean for all of them, tidying up things
#
#

CPP = g++
CPP_FLAGS = -std=c++14 -g -O0#Using -O3 to aggressively optimize. Use -g to use with gdb
#gdb requires hacks for now.

.PHONY: all clean allclean alltools
#-include $(SOURCEDIR)/Makefile.mk
Root_Directory =  ${strip $(CURDIR)}
SCDIR = $(Root_Directory)/searcher
SOURCEDIR = $(SCDIR)/src
BINDIR = $(Root_Directory)/bin
OBJECT_DIR = $(SCDIR)/objects
AUX_DIR = $(Root_Directory)/prober_presenter

#Creating directories for .o as in https://stackoverflow.com/questions/1950926/create-directories-using-make-file
#MKDIR = mkdir -p
#directories: ${OUT_DIR}

#${OUT_DIR}:
#	${MKDIR_P} ${OUT_DIR}

#DIRECTORIES = $(sort $(dir $(wildcard $(SOURCEDIR)/*/)))


$(OBJECT_DIR)/%.cpp.o: $(SCDIR)/%.cpp
	mkdir -p $(@D)
	$(CPP) $(CPP_FLAGS) $(INCLUDE_DIRS) -c $< -o $@


#Grabs all .cpp in the search client folder
CPP_FILES = $(wildcard $(SOURCEDIR)/*.cpp)
CPPSHORT = $(subst $(SCDIR),,$(CPP_FILES))
OBJS = $(foreach d, $(CPPSHORT), $(OBJECT_DIR)$(d).o)
#%.cpp.o:	%.cpp
#	$(CPP) $(CPP_FLAGS) $(INCLUDE_DIRS) $< -o $@

#OBJ_SC = $(foreach d, $(basename $(CPP_FILES)), $(d).cpp.o)

searcher: $(OBJS)
	$(CPP) -o $(BINDIR)/searcher $^

EXECUTABLESSEARCHER += $(Root_Directory)/searcher

#-I marks that headers can be found in a directory
INCLUDE_TEMP += $(SCDIR)/include

alltools: searcher prober maketable
all: searcher

testCompile:
	@echo $(CPP_FILES)
	@echo $(CPPSHORT)
	@echo $(OBJ_FOLDERS)

default: searcher

prober: $(AUX_DIR)/prober.cpp
	$(CPP) $(CPP_FLAGS) $< -o $(BINDIR)/prober

maketable: $(AUX_DIR)/maketable.cpp
	$(CPP) $(CPP_FLAGS) $< -o $(BINDIR)/maketable


#Takes all of include_temp, and puts on -I flag.
INCLUDE_DIRS = $(foreach dir, $(basename $(INCLUDE_TEMP)), -I$(dir))

#EXECUTABLES += $(Root_Directory)/prober $(Root_Directory)/maketable $(Root_Directory)/searcher

clean:
	rm $(EXECUTABLESEARCHER) $(OBJS)

allclean:
	rm $(EXECUTABLES) $(OBJS)


export INCLUDE_DIRS
