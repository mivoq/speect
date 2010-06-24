######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 5 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Source files for EBML serialization plug-in                                      ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


######## source files ##################

speect_plugin_sources(
  src/ebml_read.c
  src/ebml_write.c
  src/plugin.c
  )
 

######## header files ##################

speect_plugin_headers(
  src/ebml_read.h
  src/ebml_write.h
  src/ebml_def.h
  src/ebml.h
  src/ebml_id.h
  )

