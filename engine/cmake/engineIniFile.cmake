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


#------------------------------------------------------------------------------------#
#                   Configure the speect.conf (INI) file                             #
#------------------------------------------------------------------------------------#

configure_file(${CMAKE_SOURCE_DIR}/engine/config/speect.ini.in 
  ${CMAKE_BINARY_DIR}/speect.conf @ONLY)


#------------------------------------------------------------------------------------#
#                  Configure the speect.conf installation script                     #
#------------------------------------------------------------------------------------#

configure_file(${CMAKE_SOURCE_DIR}/engine/config/speect_ini_install.cmake.in 
  ${CMAKE_BINARY_DIR}/engine/cmake/speect_ini_install.cmake @ONLY)


#------------------------------------------------------------------------------------#
#                   Set the installation script (UNIX only)                          #
#------------------------------------------------------------------------------------#

if(SPCT_UNIX)
  install(SCRIPT ${CMAKE_BINARY_DIR}/engine/cmake/speect_ini_install.cmake)
endif(SPCT_UNIX)