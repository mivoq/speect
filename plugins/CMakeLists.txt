######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 11 June 2010                                                           ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for Speect Plug-ins                                                    ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#------------------------------------------------------------------------------------#
#                                Project name                                        #
#------------------------------------------------------------------------------------#

message(STATUS "cmake for project \"Speect Plug-ins\"")
project(speect_plugins C)


#------------------------------------------------------------------------------------#
#                             Get all CMake modules                                  #
#------------------------------------------------------------------------------------#

# append cmake directory to cmake module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# directory where plug-in configuration files are generated #
set(SPCT_PLUGINS_CMAKECONF_DIR ${CMAKE_BINARY_DIR}/plugins/cmakeconf)

# append plug-in configuration directory to cmake module path
list(APPEND CMAKE_MODULE_PATH ${SPCT_PLUGINS_CMAKECONF_DIR})

# Functions to create plug-ins
include(pluginFunctions)

# Options for plug-ins
include(pluginOptions)

# Variables that contain information about plug-ins. Use during installation
# and to check that no duplicate plug-in names are created.
set(SPCT_PLUGIN_LIST "" CACHE INTERNAL "List of Speect plug-ins and their include directories" FORCE)
set(SPCT_PLUGIN_NAMES_LIST "" CACHE INTERNAL "List of lowercase Speect plug-in names" FORCE)


#------------------------------------------------------------------------------------#
#                           SWIG CMake modules/functions                             #
#------------------------------------------------------------------------------------#

if(NOT NO_SWIG)
  # append plug-in SWIG Python functions
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/swig/cmake)

  # Functions, also includes SWIG langauge specific functions
  include(swigCommonFunctions)
endif(NOT NO_SWIG)


#------------------------------------------------------------------------------------#
#                        Speect Plug-in directories                                  #
#------------------------------------------------------------------------------------#

add_subdirectory(utils)
add_subdirectory(linguistic)
add_subdirectory(acoustic)
add_subdirectory(serialization)
add_subdirectory(processors)
add_subdirectory(languages)


#------------------------------------------------------------------------------------#
#                            Installation scripts                                    #
#------------------------------------------------------------------------------------#

configure_file(${CMAKE_SOURCE_DIR}/plugins/config/pluginInstall.cmake.in 
  ${CMAKE_BINARY_DIR}/plugins/cmake/pluginInstall.cmake @ONLY)

install(SCRIPT ${CMAKE_BINARY_DIR}/plugins/cmake/pluginInstall.cmake)
