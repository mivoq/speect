######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 9 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMake uninstall target. From http://www.cmake.org/Wiki/CMake_FAQ                 ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


CONFIGURE_FILE(
  "${CMAKE_SOURCE_DIR}/engine/config/cmake_uninstall.cmake.in"
  "${CMAKE_BINARY_DIR}/engine/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
