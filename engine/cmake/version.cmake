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
# logging facilities
#
configure_file(${CMAKE_SOURCE_DIR}/config/version.h.in 
  ${CMAKE_BINARY_DIR}/src/include/version.h)
