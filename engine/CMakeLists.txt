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


#------------------------------------------------------------------------------------#
#                             Get all CMake modules                                  #
#------------------------------------------------------------------------------------#

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


# Lists of all the source files
include(engineSources)

# Version information
include(engineVersion)

# Create Speect plug-in path loader
include(enginePluginPathLoader)

# Create Speect plug-in path file holder
include(enginePluginPathFile)

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

# Environment Variables
include(engineAbstractionEnvVar)

# Platform specific string support
include(engineAbstractionStrings)


#------------------------------------------------------------------------------------#
#                      Speect Engine library include config file                     #
#------------------------------------------------------------------------------------#

# All relevant variables must be set before here
configure_file(${CMAKE_SPEECT_SOURCE_DIR}/engine/config/spctconfig.h.in 
  ${CMAKE_SPEECT_BINARY_DIR}/engine/src/include/spctconfig.h)


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

if(NOT NO_SWIG)
  add_subdirectory(swig)
endif(NOT NO_SWIG)

