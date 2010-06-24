######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 5 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Source files for EBML Array_Float formatter plug-in                              ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


######## source files ##################

speect_plugin_sources(
  src/read.c
  src/write.c
  src/serialized_float_array.c
  src/plugin.c
  )
 

######## header files ##################

speect_plugin_headers(
  src/serialized_float_array.h
  src/element_id.h
  )

