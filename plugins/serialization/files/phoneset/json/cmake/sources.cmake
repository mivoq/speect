######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 5 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Source files for Phoneset JSON formatter plug-in                                 ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


######## source files ##################

speect_plugin_sources(
  src/plugin.c
  src/phoneset_json.c
  src/serialized_phoneset.c
  src/read.c
  )
 

######## header files ##################

speect_plugin_headers(
  src/phoneset_json.h
  src/serialized_phoneset.h
  )

