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

message(STATUS "cmake for project \"Python Object\"")
project(speect_python_object_plugin C)

# Plug-in shared object name
set(PLUGIN_SONAME python_object)

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
#                        Plug-in shared object                                       #
#------------------------------------------------------------------------------------#

# Library build configuration
include(plugin)


#------------------------------------------------------------------------------------#
#                        Create a plug-in cmake config file                          #
#------------------------------------------------------------------------------------#

include(plugin_conf)


#------------------------------------------------------------------------------------#
#                         Add Python libraries and includes                          #
#------------------------------------------------------------------------------------#

# find Python 
find_package(PythonLibs REQUIRED) # Note, currently PythonLibs does not look for Python 3

# add Python to includes
include_directories(${PYTHON_INCLUDE_PATH})


#------------------------------------------------------------------------------------#
#                                   examples                                         #
#------------------------------------------------------------------------------------#

#if(WANT_EXAMPLES)
#  add_subdirectory(examples)
#endif(WANT_EXAMPLES)


#------------------------------------------------------------------------------------#
#                              SWIG wrappers                                         #
#------------------------------------------------------------------------------------#

add_subdirectory(swig)
