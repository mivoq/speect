######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 2 Novermber 2009                                                       ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMake Speect version information                                                 ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#
# version information
#
configure_file(${CMAKE_SOURCE_DIR}/engine/config/version.h.in 
  ${CMAKE_BINARY_DIR}/engine/src/include/version.h)
