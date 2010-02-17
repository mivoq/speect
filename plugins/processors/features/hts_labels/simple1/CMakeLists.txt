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

message(STATUS "cmake for project \"HTS Labels (simple1) Feature Processor\"")
project(speect_hts_labels_simple1_featproc_plugin C)

# Plug-in shared object name
set(PLUGIN_SONAME hts_labels_simple1_featproc)

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
#                          Include phoneset                                          #
#------------------------------------------------------------------------------------#

include(phoneset)

#------------------------------------------------------------------------------------#
#                        Plug-in shared object                                       #
#------------------------------------------------------------------------------------#

# Library build configuration
include(plugin)

# add dependencies
add_dependencies(${PLUGIN_SONAME}_plugin phoneset_plugin)


#------------------------------------------------------------------------------------#
#                                   examples                                         #
#------------------------------------------------------------------------------------#

#if(WANT_EXAMPLES)
#  add_subdirectory(examples)
#endif(WANT_EXAMPLES)

