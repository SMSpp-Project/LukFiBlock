##############################################################################
################################ makefile ####################################
##############################################################################
#                                                                            #
#   makefile of [Simple/Structured]MILPBlock                                 #
#                                                                            #
#   Note that $(SMS++INC) is assumed to include any -I directive             #
#   corresponding to external libraries needed by SMS++, at least to the     #
#   extent in which they are needed by the parts of SMS++ used by            #
#   LukFiBlock.                                                              #
#                                                                            #
#   Input:  $(CC)       = compiler command                                   #
#           $(SW)       = compiler options                                   #
#           $(SMS++INC) = the -I$( core SMS++ directory )                    #
#           $(SMS++OBJ) = the libSMS++ library itself                        #
#           $(LukFiSDR) = the directory where the source is                  #
#                                                                            #
#   Output: $(LukFiOBJ) = the final object(s) / library                      #
#           $(LukFiH)   = the .h files to include                            #
#           $(LukFiINC) = the -I$( source directory )                        #
#                                                                            #
#                              Antonio Frangioni                             #
#                         Dipartimento di Informatica                        #
#                             Universita' di Pisa                            #
#                                                                            #
##############################################################################


# macros to be exported - - - - - - - - - - - - - - - - - - - - - - - - - - -

LukFiOBJ = $(LukFiSDR)/obj/LukFiBlock.o

LukFiINC = -I$(LukFiSDR)

LukFiH   = $(LukFiSDR)/LukFiBlock.h

# clean - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

clean::
	rm -f $(LukFiOBJ) $(MILPBSDR)/*~

# dependencies: every .o from its .cpp + every recursively included .h- - - -

$(LukFiSDR)/obj/LukFiBlock.o: $(LukFiSDR)/src/LukFiBlock.cpp \
	$(LukFiSDR)/include/LukFiBlock.h $(SMS++OBJ)
	$(CC) -c $*.cpp -o $@ $(LukFiINC) $(SMS++INC) $(SW)

########################## End of makefile ###################################
