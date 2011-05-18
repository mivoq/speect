######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 5 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Source files for SG2PRewrites plug-in                                            ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


######## source files ##################

speect_plugin_sources(
  src/plugin.c
  src/syllab_rewrites.c
  src/syllab_rewrites_rule.c
  )
 

######## header files ##################

speect_plugin_headers(
  src/syllab_rewrites.h
  src/syllab_rewrites_rule.h
  )

