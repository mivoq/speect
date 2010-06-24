######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 5 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Source files for RELP synthesis plug-in                                          ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


######## source files ##################

speect_plugin_sources(
  src/plugin.c
  src/concat.c
  src/mapping.c
  src/relp.c
  src/window.c
  src/synthesis.c
  )
 

######## header files ##################

speect_plugin_headers(
  src/relp.h
  )

