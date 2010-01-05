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
  src/g2p_rewrites.c
  src/g2p_rewrites_rule.c
  src/read.c
  src/serialized_g2p_rewrites.c
  )
 

######## header files ##################

set(${PLUGIN_SONAME}_INCLUDE_FILES
  src/g2p_rewrites.h
  src/g2p_rewrites_rule.h
  src/serialized_g2p_rewrites.h
  )

