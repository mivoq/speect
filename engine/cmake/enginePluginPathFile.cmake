######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : September 2011                                                         ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMake Speect Plug-in Path location holder file                                   ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


#------------------------------------------------------------------------------------#
#                         Copy the plugin.lib file                                   #
#------------------------------------------------------------------------------------#

# Copy plugin.lib file to binary dir
speect_file_copy(${CMAKE_SOURCE_DIR}/engine/config/plugin.lib ${CMAKE_BINARY_DIR}/plugins/lib/plugin.lib)


#------------------------------------------------------------------------------------#
#                  Configure the speect.conf installation script                     #
#------------------------------------------------------------------------------------#

configure_file(${CMAKE_SOURCE_DIR}/engine/config/plugin_path_install.cmake.in 
  ${CMAKE_BINARY_DIR}/engine/cmake/plugin_path_install.cmake  @ONLY)


#------------------------------------------------------------------------------------#
#                   Set the installation script (UNIX only)                          #
#------------------------------------------------------------------------------------#

if(SPCT_UNIX)
  install(SCRIPT ${CMAKE_BINARY_DIR}/engine/cmake/plugin_path_install.cmake)
endif(SPCT_UNIX)