######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : June 2010                                                              ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Source files for Tokenizer plug-in                                               ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


######## source files ##################

speect_plugin_sources(
  src/plugin.c
  src/token.c
  src/tokenizer.c
  src/tokenizer_file.c
  src/tokenizer_string.c
  )
 

######## header files ##################

speect_plugin_headers(
  src/token.h
  src/tokenizer.h
  src/tokenizer_file.h
  src/tokenizer_string.h
  )

