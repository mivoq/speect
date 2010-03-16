######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 5 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Top level CMakeList for Speect Plug-ins                                          ##
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
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)


#------------------------------------------------------------------------------------#
#                                Project name                                        #
#------------------------------------------------------------------------------------#

message(STATUS "cmake for project \"Speect Plug-ins\"")
project(speect_plugins C)


#------------------------------------------------------------------------------------#
#                 Plug-ins common cmake files location                               #
#------------------------------------------------------------------------------------#

set(PLUGINS_ENV_VAR_SET FALSE)
set(PLUGINS_ENV_VAR "$ENV{SPEECT_PLUGINS}")
string(COMPARE NOTEQUAL PLUGINS_ENV_VAR "" PLUGINS_ENV_VAR_SET)
if(NOT PLUGINS_ENV_VAR_SET)
  message(FATAL_ERROR "The environment variable \"SPEECT_PLUGINS\" is not set. Please set it to the root of the Speect plug-ins directory.")
endif(NOT PLUGINS_ENV_VAR_SET)

set(PLUGINS_COMMON_CMAKE_PATH ${PLUGINS_ENV_VAR}/cmake/)
list(APPEND CMAKE_MODULE_PATH ${PLUGINS_COMMON_CMAKE_PATH})
list(APPEND CMAKE_MODULE_PATH ${PLUGINS_COMMON_CMAKE_PATH}/config)


#------------------------------------------------------------------------------------#
#                            Speect Engine location                                  #
#------------------------------------------------------------------------------------#

set(SPEECT_ENGINE_ENV_VAR_SET FALSE)
set(SPEECT_ENGINE_ENV_VAR "$ENV{SPEECT_ENGINE}")
string(COMPARE NOTEQUAL SPEECT_ENGINE_ENV_VAR "" SPEECT_ENGINE_ENV_VAR_SET)
if(NOT SPEECT_ENGINE_ENV_VAR_SET)
  message(FATAL_ERROR "The environment variable \"SPEECT_ENGINE\" is not set. Please set it to the root of the Speect Engine directory.")
endif(NOT SPEECT_ENGINE_ENV_VAR_SET)

set(SPEECT_DIR ${SPEECT_ENGINE_ENV_VAR})


#------------------------------------------------------------------------------------#
#                             Get all CMake modules                                  #
#------------------------------------------------------------------------------------#

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Our own CMake macros and functions.
include(custom)

# Build options
include(options)

# include Speect
include(Speect)

# Compiler support
include(compilers)

# Build types (warnings, debugging, profiling, compiler flags)
include(build)


#------------------------------------------------------------------------------------#
#                        Plug-in directories                                         #
#------------------------------------------------------------------------------------#

add_subdirectory(utils)
add_subdirectory(linguistic)
add_subdirectory(acoustic)
add_subdirectory(serialization)
add_subdirectory(processors)
add_subdirectory(languages)
