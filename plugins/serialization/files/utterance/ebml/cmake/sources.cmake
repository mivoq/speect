######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 5 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Source files for SEbmlUttFile plug-in                                            ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


######## source files ##################

speect_plugin_sources(
  src/plugin.c
  src/utt_ebml.c
  src/read.c
  src/write.c
  )
 

######## header files ##################

speect_plugin_headers(
  src/utt_ebml.h
  src/element_id.h
  )

