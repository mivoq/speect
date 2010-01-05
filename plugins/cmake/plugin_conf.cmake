######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 19 November 2009                                                       ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Create cmake configuration file that creates a target that other plug-ins can    ##
## include                                                                          ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

set(tmp_name ${${PLUGIN_SONAME}_INCLUDE_DIRS})
configure_file(${PLUGINS_COMMON_CMAKE_PATH}/PluginConf.cmake.in 
     ${PLUGINS_COMMON_CMAKE_PATH}/config/${PLUGIN_SONAME}.cmake @ONLY)

