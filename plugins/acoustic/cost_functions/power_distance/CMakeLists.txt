######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 5 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Template CMakeList for Speect Plug-ins                                           ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#------------------------------------------------------------------------------------#
#                                Project name                                        #
#------------------------------------------------------------------------------------#

message(STATUS "cmake for project \"Power distance cost function\"")
project(speect_pow_dist_cost_function_plugin C)

# Plug-in shared object name
set(PLUGIN_SONAME score_pow_dist)

set(${PLUGIN_SONAME}_VERSION_MAJOR 0)
set(${PLUGIN_SONAME}_VERSION_MINOR 9)
set(${PLUGIN_SONAME}_VERSION_PATCH 0)
set(${PLUGIN_SONAME}_VERSION "${${PLUGIN_SONAME}_VERSION_MAJOR}.${${PLUGIN_SONAME}_VERSION_MINOR}.${${PLUGIN_SONAME}_VERSION_PATCH}")


#------------------------------------------------------------------------------------#
#                             Get all CMake modules                                  #
#------------------------------------------------------------------------------------#

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Lists of all the source files
include(sources)


#------------------------------------------------------------------------------------#
#                     Include base cost function class                               #
#------------------------------------------------------------------------------------#

include(cost_function)

#------------------------------------------------------------------------------------#
#                        Include matrix float class                                  #
#------------------------------------------------------------------------------------#

include(matrix-float)


#------------------------------------------------------------------------------------#
#                        Plug-in shared object                                       #
#------------------------------------------------------------------------------------#

# Library build configuration
include(plugin)

# add dependencies
add_dependencies(${PLUGIN_SONAME}_plugin matrix-float_plugin)
add_dependencies(${PLUGIN_SONAME}_plugin cost_function_plugin)
 

#------------------------------------------------------------------------------------#
#                        Create a plug-in cmake config file                          #
#------------------------------------------------------------------------------------#

include(plugin_conf)

