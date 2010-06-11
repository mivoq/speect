######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 9 Novermber 2009                                                       ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMake Speect initialization file                                                 ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#
# configuration file
#
configure_file(${CMAKE_SOURCE_DIR}/engine/config/speect.ini.in 
  ${CMAKE_BINARY_DIR}/speect.conf @ONLY)


configure_file(${CMAKE_SOURCE_DIR}/engine/config/speect_ini_install.cmake.in 
  ${CMAKE_BINARY_DIR}/engine/cmake/speect_ini_install.cmake @ONLY)

if(SPCT_UNIX)
  install(SCRIPT ${CMAKE_BINARY_DIR}/engine/cmake/speect_ini_install.cmake)
endif(SPCT_UNIX)