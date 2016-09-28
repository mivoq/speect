######################################################################################
##                                                                                  ##
## AUTHOR  : Federico Rossetto                                                      ##
## DATE    : 8 September 2016                                                       ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Source files for syllable to next accent feature processor plug-in               ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


######## source files ##################

speect_plugin_sources(
  src/plugin.c
  src/syls_from_prev_acc.c
  )


######## header files ##################

speect_plugin_headers(
  src/syls_from_prev_acc.h
  )

