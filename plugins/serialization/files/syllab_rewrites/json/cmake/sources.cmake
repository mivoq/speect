######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : May 2011                                                               ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Source files for Syllabification rewrites JSON formatter plug-in                 ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


######## source files ##################

speect_plugin_sources(
  src/plugin.c
  src/serialized_syllabification.c
  src/read.c
  )
 

######## header files ##################

speect_plugin_headers(
  src/serialized_syllabification.h
  )

