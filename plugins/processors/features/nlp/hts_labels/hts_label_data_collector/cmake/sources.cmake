######################################################################################
##                                                                                  ##
## AUTHOR  : Simone Daminato                                                        ##
## DATE    : 22 December 2015                                                       ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Source files for HTS Labels data collector feature processor plug-in             ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


######## source files ##################

speect_plugin_sources(
  src/plugin.c
  src/hts_data_collector.c
  )


######## header files ##################

speect_plugin_headers(
  src/hts_data_collector.h
  )

