######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 11 June 2010                                                           ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for object serialization plug-ins                                      ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#------------------------------------------------------------------------------------#
#                        Plug-in directories                                         #
#------------------------------------------------------------------------------------#

add_subdirectory(arrays)
add_subdirectory(matrices)
add_subdirectory(primitives)
add_subdirectory(tracks)

if(WANT_PYTHON_WRAPPER)
  add_subdirectory(pyobject)
endif(WANT_PYTHON_WRAPPER)
