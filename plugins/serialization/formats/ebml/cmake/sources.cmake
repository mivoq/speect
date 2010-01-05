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
  src/ebml_read.c
  src/ebml_write.c
  src/plugin.c
  )
 

######## header files ##################

set(${PLUGIN_SONAME}_INCLUDE_FILES
  src/ebml_read.h
  src/ebml_write.h
  src/ebml_def.h
  src/ebml.h
  src/ebml_id.h
  )

