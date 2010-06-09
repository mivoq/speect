######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 8 June 2010                                                            ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for Speect Engine                                                      ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#------------------------------------------------------------------------------------#
#                                Project name                                        #
#------------------------------------------------------------------------------------#

message(STATUS "cmake for project \"Speect Engine\"")
project(speect_engine C)

# version 
set(SPCT_VERSION_RELEASE "Beryllium" CACHE STRING "Speect version codename" FORCE)
set(SPCT_VERSION_MAJOR 1 CACHE STRING "Speect major version number" FORCE)
set(SPCT_VERSION_MINOR 0 CACHE STRING "Speect minor version number" FORCE)
set(SPCT_VERSION_PATCH 0 CACHE STRING "Speect patch number" FORCE)
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

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


include(engineInfoFiles)

# Lists of all the source files
include(engineSources)

# Version information
include(engineVersion)

# Create Speect initialization file (INI)
include(engineIniFile)

# Initialization file loader
include(engineIniLoader)


#------------------------------------------------------------------------------------#
#    Speect Engine library variables  (used in platform abstraction configuration)   #
#------------------------------------------------------------------------------------#

# Libraries that we always need to link against on this platform.
# Platform abstractions can add extra libraries to this list.
set(SPCT_PLATFORM_LIBS)

# List of source files need to compile Speect Engine in this configuration on
# this platform. Platform abstractions can add source files to this list.
set(SPCT_LIBRARY_SOURCES)


#------------------------------------------------------------------------------------#
#                             Platform abstraction                                   #
#------------------------------------------------------------------------------------#

# Types
include(engineAbstractionTypes)

# File system paths 
include(engineAbstractionPaths)

# Threads support
include(engineAbstractionThreads)

# Dynamic shared objects support
include(engineAbstractionDSO)

# Memory mapped files support
include(engineAbstractionMMap)


#------------------------------------------------------------------------------------#
#                      Speect Engine library include config file                     #
#------------------------------------------------------------------------------------#

# All relevant variables must be set before here
configure_file(${CMAKE_SOURCE_DIR}/engine/config/spctconfig.h.in 
  ${CMAKE_BINARY_DIR}/engine/src/include/spctconfig.h)


#------------------------------------------------------------------------------------#
#                              Speect Engine library                                 #
#------------------------------------------------------------------------------------#

# Library build configuration
include(engineLibrary)

# Addons/Plug-ins and examples can link with the SPCT_LIBRARIES target.


#------------------------------------------------------------------------------------#
#                          Speect Engine cmake build config                          #
#------------------------------------------------------------------------------------#

# 
# Create Speect configuration files that create a SPCT_LIBRARY_TARGET target that
# can then be included in other CMake projects.
# Other projects can just include the SpeectConf.cmake file.
#
include(engineCMakeConf)

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

