######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 19 November 2009                                                       ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for Speect primitive data types ebml format plug-in                    ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#------------------------------------------------------------------------------------#
#                                Project name                                        #
#------------------------------------------------------------------------------------#

message(STATUS "cmake for project \"Speect Primitives Ebml\"")
project(speect_primitives_ebml_plugin C)

# Plug-in shared object name
set(PLUGIN_SONAME prim-ebml)

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
#                               Include ebml                                         #
#------------------------------------------------------------------------------------#

include(ebml)

#------------------------------------------------------------------------------------#
#                        Plug-in shared object                                       #
#------------------------------------------------------------------------------------#

# Library build configuration
include(plugin)

# add dependency
add_dependencies(${PLUGIN_SONAME}_plugin ebml_plugin)
  

#------------------------------------------------------------------------------------#
#                                   examples                                         #
#------------------------------------------------------------------------------------#

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/examples/prim_example_read.c.in 
  ${CMAKE_CURRENT_BINARY_DIR}/examples/prim_example_read.c @ONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/examples/prim_example_write.c.in 
  ${CMAKE_CURRENT_BINARY_DIR}/examples/prim_example_write.c @ONLY)

if(WANT_EXAMPLES)
  add_subdirectory(examples)
endif(WANT_EXAMPLES)
