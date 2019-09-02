##############################################################################
################################ makefile ####################################
##############################################################################
#                                                                            #
#   makefile of test (for core SMS++)                                        #
#                                                                            #
#                                VERSION 2.00                                #
#                               12 - 11 - 2018                               #
#                                                                            #
#                              Implementation by:                            #
#                                                                            #
#                              Antonio Frangioni                             #
#                           Operations Research Group                        #
#                          Dipartimento di Informatica                       #
#                              Universita' di Pisa                           #
#                                                                            #
##############################################################################

# module name
NAME = LukFiMain

# basic directory
DIR = ./

# debug switches
SW =  -g3 -glldb -fno-inline -std=c++17 -ferror-limit=1

# production switches
#SW = -O3 -DNDEBUG -std=c++14  

# compiler
#CC = /usr/local/Cellar/gcc/8.3.0_2/bin/g++-8 
CC = clang++ 

# default target- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

all default: $(NAME)

# clean - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

clean::
	rm -f $(DIR)*.o $(DIR)*~ $(NAME)

# define & include the necessary modules- - - - - - - - - - - - - - - - - - -
# if a module is not used in the current configuration, just comment out the
# corresponding include line
# each module outputs some macros to be used here:
# *OBJ is the final object(s) / library
# *LIB is the external libreries + -L<libdirs>
# *H   is the list of all include files
# *INC is the -I<include directories>

# core SMS++
SMS++SDR = ../../SMS++/
include $(SMS++SDR)lib/makefile-inc

# BundeSolver
BNDSLVSDR = ../../BundleSolver/
include $(BNDSLVSDR)makefile-c

# LukFiBlock
LukFiSDR = ../
include $(LukFiSDR)makefile

# main module (linking phase) - - - - - - - - - - - - - - - - - - - - - - - -

# object files
MOBJ = $(LukFiOBJ) $(SMS++OBJ) $(BNDSLVOBJ)

# libreries
MLIB = $(SMS++LIB) $(BNDSLVLIB) $(LukFiLIB)

$(NAME): $(MOBJ) $(DIR)main.o 
	$(CC) -o $(NAME) $(DIR)main.o $(MOBJ) $(MLIB) $(SW)

# dependencies: every .o from its .C + every recursively included .h- - - - -

# include directives
MINC = $(SMS++INC) $(BNDSLVINC) $(LukFiINC)

# includes
MH = $(SMS++H) $(BNDSLVH) $(LukFiH) 

# compile command

$(DIR)main.o: $(DIR)main.cpp $(MH) 
	$(CC) -c $*.cpp -o $@ $(MINC) $(SW)

############################ End of makefile ################################

