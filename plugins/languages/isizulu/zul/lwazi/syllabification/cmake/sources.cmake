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
  src/plugin.c
  src/zul_lwazi.c
  )
 

######## header files ##################

set(${PLUGIN_SONAME}_INCLUDE_FILES
  src/zul_lwazi.h 
  )

