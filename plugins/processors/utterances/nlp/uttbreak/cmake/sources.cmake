######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 5 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Source files for Tokenizer utterance processor plug-in                           ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


######## source files ##################

speect_plugin_sources(
  src/plugin.c
  src/uttbreak_proc.c
  )
 

######## header files ##################

speect_plugin_headers(
  src/uttbreak_proc.h
  )

