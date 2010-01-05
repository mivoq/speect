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
  src/halfphone_db_ebml.c
  src/serialized_halfphone_db.c
  src/read.c
  )
 

######## header files ##################

set(${PLUGIN_SONAME}_INCLUDE_FILES
  src/halfphone_db_ebml.h
  src/serialized_halfphone_db.h
  src/element_id.h
  )

