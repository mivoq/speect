######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 5 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Source files for Audio RIFF formatter plug-in                                    ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


######## source files ##################

speect_plugin_sources(
  src/plugin.c
  src/serialized_audio.c
  src/write.c
  )
 

######## header files ##################

speect_plugin_headers(
  src/serialized_audio.h
  )

