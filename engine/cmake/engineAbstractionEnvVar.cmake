######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : June 2012                                                              ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Speect Engine source files                                                       ##
## File system environment variable implementations abstraction                     ##
##                                                                                  ##
######################################################################################

# SPCT_ENVVAR_SPECIFIC_IMPL_INCLUDES : list of implementation include files
# SPCT_ENVVAR_SPECIFIC_IMPL_DIR      : implementation directory


set(SPCT_ENVVAR_SUPPORT 0)

#------------------------------------------------------------------------------------#
#                              POSIX implementation                                  #
#------------------------------------------------------------------------------------#

set(SPCT_SRC_POSIX_ENVVAR_FILES
  posix/posix_envvar.c
)

set(SPCT_INCLUDE_POSIX_ENVVAR_FILES
  posix/posix_envvar.h
)

if(SPCT_UNIX OR SPCT_MACOSX)
  set(SPCT_ENVVAR_SPECIFIC_IMPL "${SPCT_INCLUDE_POSIX_ENVVAR_FILES}")
  set(SPCT_ENVVAR_SPECIFIC_IMPL_DIR "posix")
  list(APPEND SPCT_LIBRARY_SOURCES "${CMAKE_SOURCE_DIR}/engine/src/base/utils/platform/${SPCT_SRC_POSIX_ENVVAR_FILES}")

  set(SPCT_ENVVAR_SUPPORT 1)
endif(SPCT_UNIX OR SPCT_MACOSX)


#------------------------------------------------------------------------------------#
#                              WIN32 implementation                                  #
#------------------------------------------------------------------------------------#

set(SPCT_SRC_WIN32_ENVVAR_FILES
  win32/win32_envvar.c
)

set(SPCT_INCLUDE_WIN32_ENVVAR_FILES
  win32/win32_envvar.h
)


if(SPCT_WIN32)
  set(SPCT_ENVVAR_SPECIFIC_IMPL "${SPCT_INCLUDE_WIN32_ENVVAR_FILES}")
  set(SPCT_ENVVAR_SPECIFIC_IMPL_DIR "win32")
  list(APPEND SPCT_LIBRARY_SOURCES "${CMAKE_SOURCE_DIR}/engine/src/base/utils/platform/${SPCT_SRC_WIN32_ENVVAR_FILES}")

  set(SPCT_ENVVAR_SUPPORT 1)
endif(SPCT_WIN32)


#------------------------------------------------------------------------------------#
#                               engine header                                        #
#------------------------------------------------------------------------------------#

configure_file(${CMAKE_SOURCE_DIR}/engine/config/envvar_impl.h.in 
  ${CMAKE_BINARY_DIR}/engine/src/base/utils/platform/envvar_impl.h)


#------------------------------------------------------------------------------------#
#                               installation                                         #
#------------------------------------------------------------------------------------#

if(SPCT_UNIX)
  install(FILES ${CMAKE_SOURCE_DIR}/engine/src/base/utils/platform/${SPCT_ENVVAR_SPECIFIC_IMPL}
    DESTINATION include/speect/engine/base/utils/platform/${SPCT_ENVVAR_SPECIFIC_IMPL_DIR}/)

  install(FILES ${CMAKE_BINARY_DIR}/engine/src/base/utils/platform/envvar_impl.h
    DESTINATION include/speect/engine/base/utils/platform)
endif(SPCT_UNIX)


#------------------------------------------------------------------------------------#
#                          Error if no implementation                                #
#------------------------------------------------------------------------------------#

if(NOT SPCT_ENVVAR_SUPPORT)
  message(FATAL_ERROR "Unknown file system environment variables support.")    
endif(NOT SPCT_ENVVAR_SUPPORT)
