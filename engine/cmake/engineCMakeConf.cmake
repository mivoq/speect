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


#------------------------------------------------------------------------------------#
#                   Configure the SpeectConf.cmake file                              #
#------------------------------------------------------------------------------------#

# this is the file included by other projects
configure_file(${CMAKE_SOURCE_DIR}/engine/config/SpeectConf.cmake.in 
    ${CMAKE_BINARY_DIR}/engine/SpeectConf.cmake @ONLY)


#------------------------------------------------------------------------------------#
#             Configure the SpeectConf.cmake installation script                     #
#------------------------------------------------------------------------------------#

configure_file(${CMAKE_SOURCE_DIR}/engine/config/speect_conf_install.cmake.in 
  ${CMAKE_BINARY_DIR}/engine/cmake/speect_conf_install.cmake @ONLY)


#------------------------------------------------------------------------------------#
#               Create the Speect Engine library export cmake file                   #
#------------------------------------------------------------------------------------#

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  export(TARGETS SPCT_LIBRARIES
    FILE spctConfig.cmake)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")


#------------------------------------------------------------------------------------#
#                   Set the installation script (UNIX only)                          #
#------------------------------------------------------------------------------------#

if(SPCT_UNIX)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    install(SCRIPT ${CMAKE_BINARY_DIR}/engine/cmake/speect_conf_install.cmake)
  endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
endif(SPCT_UNIX)