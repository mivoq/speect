######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 11 June 2010                                                           ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for utils plug-ins                                                     ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#------------------------------------------------------------------------------------#
#                        Plug-in directories                                         #
#------------------------------------------------------------------------------------#
add_subdirectory(arrays)
add_subdirectory(matrices)
add_subdirectory(tracks)
add_subdirectory(tokenizer)
add_subdirectory(viterbi)
add_subdirectory(cost_function)

#if(WANT_PYTHON_WRAPPER)
#  add_subdirectory(python_object)
#endif(WANT_PYTHON_WRAPPER)
