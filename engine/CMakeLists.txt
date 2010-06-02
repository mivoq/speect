######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 9 May 2009                                                             ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Main CMakeList for Speect                                                        ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


#------------------------------------------------------------------------------------#
#                                Build type                                          #
#------------------------------------------------------------------------------------#

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING  #  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel Profile."
      FORCE)
endif()

# Restrict configuration types to the selected build type.
# Note: This needs to be done before the project command
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_BUILD_TYPE}" CACHE INTERNAL "internal")


#------------------------------------------------------------------------------------#
#                               CMake settings                                       #
#------------------------------------------------------------------------------------#

message(STATUS "setting cmake flags")
#set(CMAKE_VERBOSE_MAKEFILE "TRUE")
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)


#------------------------------------------------------------------------------------#
#                                Project name                                        #
#------------------------------------------------------------------------------------#

message(STATUS "cmake for project \"Speect Engine\"")
project(speect_engine C)

# version 
set(SPCT_VERSION_RELEASE "Lithium" CACHE STRING "Speect version codename" FORCE)
set(SPCT_VERSION_MAJOR 0 CACHE STRING "Speect major version number" FORCE)
set(SPCT_VERSION_MINOR 9 CACHE STRING "Speect minor version number" FORCE)
set(SPCT_VERSION_PATCH 5 CACHE STRING "Speect patch number" FORCE)
set(SPCT_VERSION "${SPCT_VERSION_MAJOR}.${SPCT_VERSION_MINOR}.${SPCT_VERSION_PATCH}"
  CACHE STRING "Speect full version number" FORCE)
mark_as_advanced(SPCT_VERSION_RELEASE)
mark_as_advanced(SPCT_VERSION_MAJOR)
mark_as_advanced(SPCT_VERSION_MINOR)
mark_as_advanced(SPCT_VERSION_PATCH)
mark_as_advanced(SPCT_VERSION)


#------------------------------------------------------------------------------------#
#                             Get all CMake modules                                  #
#------------------------------------------------------------------------------------#

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Try to find the plug-in path
include(plugin_path)

# Our own CMake macros and functions
include(custom)

# Lists of all the source files
include(sources)

# Build options
include(options)

# Compiler support
include(compilers)

# System tests
include(system)

# Thread support
include(threads)

# File path functions support
include(path)

# Dynamic shared objects support
include(dll)

# Memory mapped files support
include(mmap)

# Platform specific types support
include(types)

# Version information
include(version)

# Create Speect initialization file (INI)
include(inifile)

# Initialization file loader
include(main_ini)

# Uninstall target
include(uninstall)

# Build types (warnings, debugging, profiling, compiler flags)
include(build)


#------------------------------------------------------------------------------------#
#                      Speect Engine library include config file                     #
#------------------------------------------------------------------------------------#

# All relevant variables must be set before here
configure_file(${CMAKE_SOURCE_DIR}/config/spctconfig.h.in 
  ${CMAKE_BINARY_DIR}/src/include/spctconfig.h)


#------------------------------------------------------------------------------------#
#                                 Main library                                       #
#------------------------------------------------------------------------------------#

# Library build configuration
include(libspct)


#------------------------------------------------------------------------------------#
#                          Speect Engine cmake build config                          #
#------------------------------------------------------------------------------------#

# 
# Create Speect configuration files that create a SPCT_LIBRARY_TARGET target that
# can then be included in other CMake projects.
# Other projects can just include the SpeectConf.cmake file.
#
include(speect_conf)

#------------------------------------------------------------------------------------#
#                               examples/tests                                       #
#------------------------------------------------------------------------------------#

if(WANT_EXAMPLES)
  add_subdirectory(examples)
endif(WANT_EXAMPLES)

if(WANT_TESTS)
  add_subdirectory(tests)
endif(WANT_TESTS)


#------------------------------------------------------------------------------------#
#                              SWIG wrappers                                         #
#------------------------------------------------------------------------------------#

add_subdirectory(swig)