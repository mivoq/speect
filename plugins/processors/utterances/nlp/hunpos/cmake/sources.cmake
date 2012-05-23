######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 23 May 2012                                                            ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Source files for Hunpos POS utterance processor plug-in                          ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


######## source files ##################

speect_plugin_sources(
  src/plugin.c
  src/hunpos_proc.c
  src/hunposwrap.c
  )
 

######## header files ##################

speect_plugin_headers(
  src/hunpos_proc.h
  src/hunpos.h
  )

