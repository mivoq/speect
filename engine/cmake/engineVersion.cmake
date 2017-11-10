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


#------------------------------------------------------------------------------------#
#                        Configure the version.h file                                #
#------------------------------------------------------------------------------------#

# Note that the version variables are set in speect/cmake/spctVersion.cmake

configure_file(${CMAKE_SPEECT_SOURCE_DIR}/engine/config/version.h.in 
  ${CMAKE_SPEECT_BINARY_DIR}/engine/src/include/version.h)
