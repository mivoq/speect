######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 8 June 2010                                                            ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Speect Engine source files                                                       ##
## File system path implementations abstraction                                     ##
##                                                                                  ##
######################################################################################

# SPCT_PATH_SPECIFIC_IMPL_INCLUDES : list of implementation include files
# SPCT_PATH_SPECIFIC_IMPL_DIR      : implementation directory


set(SPCT_PATH_SUPPORT 0)

#------------------------------------------------------------------------------------#
#                              POSIX implementation                                  #
#------------------------------------------------------------------------------------#

set(SPCT_SRC_POSIX_PATH_FILES
  posix/posix_path.c
)

set(SPCT_INCLUDE_POSIX_PATH_FILES
  posix/posix_path.h
)

if(SPCT_UNIX OR SPCT_MACOSX)
  set(SPCT_PATH_SPECIFIC_IMPL "${SPCT_INCLUDE_POSIX_PATH_FILES}")
  set(SPCT_PATH_SPECIFIC_IMPL_DIR "posix")
  list(APPEND SPCT_LIBRARY_SOURCES "${CMAKE_SOURCE_DIR}/engine/src/base/utils/platform/${SPCT_SRC_POSIX_PATH_FILES}")

  set(SPCT_PATH_SUPPORT 1)
endif(SPCT_UNIX OR SPCT_MACOSX)


#------------------------------------------------------------------------------------#
#                              WIN32 implementation                                  #
#------------------------------------------------------------------------------------#

set(SPCT_SRC_WIN32_PATH_FILES
  win32/win32_path.c
)

set(SPCT_INCLUDE_WIN32_PATH_FILES
  win32/win32_path.h
)


if(SPCT_WIN32)
  set(SPCT_PATH_SPECIFIC_IMPL "${SPCT_INCLUDE_WIN32_PATH_FILES}")
  set(SPCT_PATH_SPECIFIC_IMPL_DIR "win32")
  list(APPEND SPCT_LIBRARY_SOURCES "${CMAKE_SOURCE_DIR}/engine/src/base/utils/platform/${SPCT_SRC_WIN32_PATH_FILES}")

  set(SPCT_PATH_SUPPORT 1)
endif(SPCT_WIN32)


#------------------------------------------------------------------------------------#
#                               engine header                                        #
#------------------------------------------------------------------------------------#

configure_file(${CMAKE_SOURCE_DIR}/engine/config/path_impl.h.in 
  ${CMAKE_BINARY_DIR}/engine/src/base/utils/platform/path_impl.h)


#------------------------------------------------------------------------------------#
#                               installation                                         #
#------------------------------------------------------------------------------------#

if(SPCT_UNIX)
  install(FILES ${CMAKE_SOURCE_DIR}/engine/src/base/utils/platform/${SPCT_PATH_SPECIFIC_IMPL}
    DESTINATION include/speect/engine/base/utils/platform/${SPCT_PATH_SPECIFIC_IMPL_DIR}/)

  install(FILES ${CMAKE_BINARY_DIR}/engine/src/base/utils/platform/path_impl.h
    DESTINATION include/speect/engine/base/utils/platform)
endif(SPCT_UNIX)


#------------------------------------------------------------------------------------#
#                          Error if no implementation                                #
#------------------------------------------------------------------------------------#

if(NOT SPCT_PATH_SUPPORT)
  message(FATAL_ERROR "Unknown file system paths support.")    
endif(NOT SPCT_PATH_SUPPORT)
