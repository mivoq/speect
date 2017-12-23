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

list(APPEND SPCT_LIBRARY_SOURCES "${CMAKE_SPEECT_SOURCE_DIR}/engine/src/base/utils/platform/${SPCT_SRC_POSIX_PATH_FILES}")

if(SPCT_UNIX OR SPCT_MACOSX)
  set(SPCT_PATH_SPECIFIC_IMPL "${SPCT_INCLUDE_POSIX_PATH_FILES}")
  set(SPCT_PATH_SPECIFIC_IMPL_DIR "posix")

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

list(APPEND SPCT_LIBRARY_SOURCES "${CMAKE_SPEECT_SOURCE_DIR}/engine/src/base/utils/platform/${SPCT_SRC_WIN32_PATH_FILES}")

if(SPCT_WIN32)
  set(SPCT_PATH_SPECIFIC_IMPL "${SPCT_INCLUDE_WIN32_PATH_FILES}")
  set(SPCT_PATH_SPECIFIC_IMPL_DIR "win32")

  set(SPCT_PATH_SUPPORT 1)
endif(SPCT_WIN32)


#------------------------------------------------------------------------------------#
#                               engine header                                        #
#------------------------------------------------------------------------------------#

configure_file(${CMAKE_SPEECT_SOURCE_DIR}/engine/config/path_impl.h.in 
  ${CMAKE_SPEECT_BINARY_DIR}/engine/src/base/utils/platform/path_impl.h)


#------------------------------------------------------------------------------------#
#                               installation                                         #
#------------------------------------------------------------------------------------#

install(FILES ${CMAKE_SPEECT_SOURCE_DIR}/engine/src/base/utils/platform/${SPCT_INCLUDE_POSIX_PATH_FILES}
  DESTINATION include/speect/engine/base/utils/platform/posix/)

install(FILES ${CMAKE_SPEECT_SOURCE_DIR}/engine/src/base/utils/platform/${SPCT_INCLUDE_WIN32_PATH_FILES}
  DESTINATION include/speect/engine/base/utils/platform/win32/)

install(FILES ${CMAKE_SPEECT_BINARY_DIR}/engine/src/base/utils/platform/path_impl.h
  DESTINATION include/speect/engine/base/utils/platform)

#------------------------------------------------------------------------------------#
#                          Error if no implementation                                #
#------------------------------------------------------------------------------------#

if(NOT SPCT_PATH_SUPPORT)
  message(FATAL_ERROR "Unknown file system paths support.")    
endif(NOT SPCT_PATH_SUPPORT)
