######################################################################################
##                                                                                  ##
## AUTHOR  : Federico Rossetto                                                               ##
## DATE    : 5 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Source files for Syllable break feature processor plug-in                        ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


######## source files ##################

speect_plugin_sources(
  src/plugin.c
  src/syl_break_marytts.c
  )
 

######## header files ##################

speect_plugin_headers(
  src/syl_break_marytts.h
  )

