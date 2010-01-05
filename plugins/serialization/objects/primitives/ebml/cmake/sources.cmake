######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 5 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Template CMake source files for Speect plug-ins                                  ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


######## source files ##################

set(${PLUGIN_SONAME}_SRC_FILES
  src/read.c
  src/write.c
  src/serialized_primitives.c
  src/plugin.c
  )
 

######## header files ##################

set(${PLUGIN_SONAME}_INCLUDE_FILES
  src/serialized_primitives.h
  )

