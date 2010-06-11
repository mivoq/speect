######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : June 2010                                                              ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Source files for Viterbi plug-in                                                 ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


######## source files ##################

speect_plugin_sources(
  src/plugin.c
  src/vit_candidate.c
  src/viterbi.c
  src/vit_path.c
  src/vit_point.c
  )
 

######## header files ##################

speect_plugin_headers(
  src/vit_candidate.h
  src/viterbi.h
  src/vit_path.h
  src/vit_point.h
  )

