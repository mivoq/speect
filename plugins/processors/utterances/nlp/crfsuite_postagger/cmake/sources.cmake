######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 23 May 2012                                                            ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Source files for CrfSuite POS utterance processor plug-in                          ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


######## source files ##################

speect_plugin_sources(
  src/plugin.c
  src/crfsuite_proc.c
  )
 

######## header files ##################

speect_plugin_headers(
  src/crfsuite_proc.h
  )

