######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 11 June 2010                                                           ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Speect version information                                                       ##
##                                                                                  ##
##                                                                                  ##
######################################################################################
 

#------------------------------------------------------------------------------------#
#                             Speect version codename                                #
#------------------------------------------------------------------------------------#

set(SPCT_VERSION_RELEASE "Beryllium" CACHE STRING "Speect version codename" FORCE)
mark_as_advanced(SPCT_VERSION_RELEASE)


#------------------------------------------------------------------------------------#
#        Get Speect version info from, git or file or default                        #
#------------------------------------------------------------------------------------#

if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  # this is a git repo, get version info from git
  execute_process(COMMAND git describe --abbrev=4 HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE TMP_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  # write info to file
  file(WRITE "${CMAKE_SOURCE_DIR}/VERSION" ${TMP_VERSION})
elseif(EXISTS "${CMAKE_SOURCE_DIR}/VERSION")
  # this is a release tarball, read version from file
  file(READ "${CMAKE_SOURCE_DIR}/VERSION" TMP_VERSION LIMIT 64)
else(EXISTS "${CMAKE_SOURCE_DIR}/VERSION")
  # use default
  set(TMP_VERSION "v1.0.0_pre-260-g796b" CACHE STRING "Speect default version" FORCE)
endif(EXISTS "${CMAKE_SOURCE_DIR}/.git")

if(TMP_VERSION MATCHES "^v(.*)")
  string(REGEX REPLACE "^v(.*)" "\\1" TMP_VERSION ${TMP_VERSION})
elseif(TMP_VERSION MATCHES "^upstream/(.*)")
  string(REGEX REPLACE "^upstream/(.*)" "\\1" TMP_VERSION ${TMP_VERSION})
else(TMP_VERSION MATCHES "^upstream/(.*)")
  message(FATAL_ERROR "Failed to match the version regex to: ${TMP_VERSION}")
endif(TMP_VERSION MATCHES "^v(.*)")
mark_as_advanced(TMP_VERSION)


#------------------------------------------------------------------------------------#
#                           Speect major version number                              #
#------------------------------------------------------------------------------------#

string(REGEX REPLACE "^(.*)\\..*\\..*" "\\1" SPCT_VERSION_MAJOR ${TMP_VERSION})
mark_as_advanced(SPCT_VERSION_MAJOR)


#------------------------------------------------------------------------------------#
#                           Speect minor version number                              #
#------------------------------------------------------------------------------------#

string(REGEX REPLACE "^.*\\.(.*)\\..*" "\\1" SPCT_VERSION_MINOR ${TMP_VERSION})
mark_as_advanced(SPCT_VERSION_MINOR)


#------------------------------------------------------------------------------------#
#                               Speect patch number                                  #
#------------------------------------------------------------------------------------#

string(REGEX REPLACE "^.*\\..*\\.(.*)" "\\1" SPCT_VERSION_PATCH ${TMP_VERSION})
mark_as_advanced(SPCT_VERSION_PATCH)


#------------------------------------------------------------------------------------#
#                     Speect version string "major.minor.patch"                      #
#------------------------------------------------------------------------------------#

set(SPCT_VERSION "${SPCT_VERSION_MAJOR}.${SPCT_VERSION_MINOR}.${SPCT_VERSION_PATCH}"
  CACHE STRING "Speect full version number" FORCE)
mark_as_advanced(SPCT_VERSION)

