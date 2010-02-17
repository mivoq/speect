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

message(STATUS "cmake for project \"HTS Engine synthesis utterance Processor\"")
project(speect_hts_engine_synth_uttproc_plugin C)

# Plug-in shared object name
set(PLUGIN_SONAME hts_engine_synth_uttproc)

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
#                                   Includes                                         #
#------------------------------------------------------------------------------------#

include(audio)

list(APPEND ${PLUGIN_SONAME}_INCLUDE_DIRS
  ${HTS_ENGINE_INCLUDE}
  )


#------------------------------------------------------------------------------------#
#                        Plug-in shared object                                       #
#------------------------------------------------------------------------------------#

# Library build configuration
include(plugin)

target_link_libraries(${PLUGIN_SONAME}_plugin ${HTS_ENGINE_LIB})

# add dependencies
add_dependencies(${PLUGIN_SONAME}_plugin audio_plugin)

#------------------------------------------------------------------------------------#
#                                   examples                                         #
#------------------------------------------------------------------------------------#

#if(WANT_EXAMPLES)
#  add_subdirectory(examples)
#endif(WANT_EXAMPLES)


