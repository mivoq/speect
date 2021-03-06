######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 5 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for EBML Matrix_Int formatter plug-in                                ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#------------------------------------------------------------------------------------#
#                             Define plug-in                                         #
#------------------------------------------------------------------------------------#

speect_plugin_definition(Matrix_Int_EBML "SEbmlMatrixInt" 1 0 0)


#------------------------------------------------------------------------------------#
#                        Configure plugin_info.h.in                                  #
#------------------------------------------------------------------------------------#

set(description "SSerializedObjectClass implementation for integer matrix data types in EBML format")

# Minimum required Speect Engine version 
set(major_min 1)
set(minor_min 0)

speect_plugin_configure_info(${description} ${major_min} ${minor_min})


#------------------------------------------------------------------------------------#
#                               Source files                                         #
#------------------------------------------------------------------------------------#

# Lists of all the source files
include(sources)


#------------------------------------------------------------------------------------#
#                     Include matrix_int and ebml plug-in                            #
#------------------------------------------------------------------------------------#

include(matrix_int)
include(ebml)


#------------------------------------------------------------------------------------#
#                            Plug-in shared object                                   #
#------------------------------------------------------------------------------------#

speect_plugin_create()


#------------------------------------------------------------------------------------#
#                              SWIG wrappers                                         #
#------------------------------------------------------------------------------------#

# Speect SWIG interface
if(NOT NO_SWIG)
  add_subdirectory(swig)
endif(NOT NO_SWIG)

