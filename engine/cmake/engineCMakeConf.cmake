######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 10 November 2009                                                       ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Create Speect configuration files that create a SPCT_LIBRARIES target that       ##
## can then be included in other CMake projects                                     ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

configure_file(${CMAKE_SOURCE_DIR}/engine/config/SpeectConf.cmake.in 
    ${CMAKE_BINARY_DIR}/engine/SpeectConf.cmake @ONLY)

configure_file(${CMAKE_SOURCE_DIR}/engine/config/speect_conf_install.cmake.in 
  ${CMAKE_SOURCE_DIR}/engine/cmake/speect_conf_install.cmake @ONLY)

export(TARGETS SPCT_LIBRARIES
  FILE spctConfig.cmake)

install(SCRIPT ${CMAKE_SOURCE_DIR}/engine/cmake/speect_conf_install.cmake)