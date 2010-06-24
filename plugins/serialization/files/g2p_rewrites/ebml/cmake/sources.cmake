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
  src/g2p_rewrites.c
  src/g2p_rewrites_rule.c
  src/read.c
  src/serialized_g2p_rewrites.c
  )
 

######## header files ##################

speect_plugin_headers(
  src/g2p_rewrites.h
  src/g2p_rewrites_rule.h
  src/serialized_g2p_rewrites.h
  )

