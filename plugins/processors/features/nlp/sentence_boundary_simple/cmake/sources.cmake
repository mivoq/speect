######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : June 2012                                                              ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Source files for simple sentence boundary detection feature processor plug-in    ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


######## source files ##################

speect_plugin_sources(
  src/plugin.c
  src/sentence_boundary_simple.c
  )
 

######## header files ##################

speect_plugin_headers(
  src/sentence_boundary_simple.h
  )

