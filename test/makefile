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
#SW = -g -Wextra -Wall -pedantic -std=c++14 -Wno-unused-parameter \
#	-Werror -Wno-unused-local-typedefs -Wno-sign-compare \
#	-Wno-error=parentheses \
#	-Wno-error=ignored-qualifiers -Wno-error=unused-variable \
#	-Wno-error=pedantic #&> error.txt
SW = -g -std=c++17 -ferror-limit=1

# production switches
#SW = -O3 -DNDEBUG -std=c++14

# compiler
#CC = g++-8
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

# LukFiBlock
LukFiSDR = ../
include $(LukFiSDR)makefile

# main module (linking phase) - - - - - - - - - - - - - - - - - - - - - - - -

# object files
MOBJ =  $(LukFiOBJ) $(SMS++OBJ)

# libreries
MLIB =  $(SMS++LIB) $(LukFiLIB)

$(NAME): $(MOBJ) $(DIR)main.o 
	$(CC) -o $(NAME) $(DIR)main.o $(MOBJ) $(MLIB) $(SW)

# dependencies: every .o from its .C + every recursively included .h- - - - -

# include directives
MINC =  $(SMS++INC) $(LukFiINC)

# includes
MH =    $(SMS++H) $(LukFiH)

# compile command

$(DIR)main.o: $(DIR)main.cpp $(MH) 
	$(CC) -c $*.cpp -o $@ $(MINC) $(SW)

############################ End of makefile ################################

