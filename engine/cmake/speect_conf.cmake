######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 10 November 2009                                                       ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Create Speect configuration files that create a SPCT_LIBRARY_TARGET target that  ##
## can then be included in other CMake projects                                     ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

configure_file(${CMAKE_SOURCE_DIR}/config/SpeectConf.cmake.in 
    ${CMAKE_BINARY_DIR}/SpeectConf.cmake @ONLY)

configure_file(${CMAKE_SOURCE_DIR}/config/speect_conf_install.cmake.in 
  ${CMAKE_SOURCE_DIR}/cmake/speect_conf_install.cmake @ONLY)

export(TARGETS SPCT_LIBRARY_TARGET 
  FILE spctConfig.cmake)

install(SCRIPT ${CMAKE_SOURCE_DIR}/cmake/speect_conf_install.cmake)