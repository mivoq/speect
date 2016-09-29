######################################################################################
##                                                                                  ##
## AUTHOR  : Federico Rossetto                                                      ##
## DATE    : 8 September 2016                                                       ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Source files for Phrase num words feature processor plug-in                      ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


######## source files ##################

speect_plugin_sources(
  src/plugin.c
  src/words_from_prev_punc.c
  )


######## header files ##################

speect_plugin_headers(
  src/words_from_prev_punc.h
  )

