######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : September 2011                                                         ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMake Speect Plug-in Path finder                                                 ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


#------------------------------------------------------------------------------------#
#         Configure the plugin_path.c file (finds plug-in path on startup)           #
#------------------------------------------------------------------------------------#

# Note that the SPCT_INSTALL_PLUGIN_PATH and SPCT_BUILD_PLUGIN_PATH variables in
# plugin_path.c.in are set in speect/cmake/spctSettings.cmake

configure_file(${CMAKE_SOURCE_DIR}/engine/config/plugin_path.c.in 
  ${CMAKE_BINARY_DIR}/engine/src/main/plugin_path.c)
