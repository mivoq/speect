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

message(STATUS "cmake for project \"LexLookup Utterance Processor\"")
project(speect_lexlookup_uttproc_plugin C)

# Plug-in shared object name
set(PLUGIN_SONAME lexlookup_uttproc)

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
#                               Include phoneset                                     #
#------------------------------------------------------------------------------------#

include(phoneset)


#------------------------------------------------------------------------------------#
#                            Include syllabification                                 #
#------------------------------------------------------------------------------------#

include(syllabification)


#------------------------------------------------------------------------------------#
#                               Include lexicon                                      #
#------------------------------------------------------------------------------------#

include(lexicon)


#------------------------------------------------------------------------------------#
#                               Include addendum                                     #
#------------------------------------------------------------------------------------#

include(addendum)


#------------------------------------------------------------------------------------#
#                               Include g2p                                          #
#------------------------------------------------------------------------------------#

include(g2p)


#------------------------------------------------------------------------------------#
#                        Plug-in shared object                                       #
#------------------------------------------------------------------------------------#

# Library build configuration
include(plugin)

# add dependencies
add_dependencies(${PLUGIN_SONAME}_plugin phoneset_plugin)
add_dependencies(${PLUGIN_SONAME}_plugin syllabification_plugin)
add_dependencies(${PLUGIN_SONAME}_plugin lexicon_plugin)
add_dependencies(${PLUGIN_SONAME}_plugin addendum_plugin)
add_dependencies(${PLUGIN_SONAME}_plugin g2p_plugin)


#------------------------------------------------------------------------------------#
#                                   examples                                         #
#------------------------------------------------------------------------------------#

if(WANT_EXAMPLES)
  add_subdirectory(examples)
endif(WANT_EXAMPLES)


