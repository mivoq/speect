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
# SPCT_PATH_SPECIFIC_IMPL_VARIANT  : implementation identifier

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

install(FILES ${CMAKE_SPEECT_SOURCE_DIR}/engine/src/base/utils/platform/${SPCT_INCLUDE_POSIX_PATH_FILES}
  DESTINATION include/speect/engine/base/utils/platform/posix/)


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

install(FILES ${CMAKE_SPEECT_SOURCE_DIR}/engine/src/base/utils/platform/${SPCT_INCLUDE_WIN32_PATH_FILES}
  DESTINATION include/speect/engine/base/utils/platform/win32/)

#------------------------------------------------------------------------------------#
#                              Default implementation                                #
#------------------------------------------------------------------------------------#

if(SPCT_WIN32)
  set(SPCT_PATH_SPECIFIC_IMPL "${SPCT_INCLUDE_WIN32_PATH_FILES}")
  set(SPCT_PATH_SPECIFIC_IMPL_VARIANT "win32")
else()
  set(SPCT_PATH_SPECIFIC_IMPL "${SPCT_INCLUDE_POSIX_PATH_FILES}")
  set(SPCT_PATH_SPECIFIC_IMPL_VARIANT "posix")
endif()

configure_file(${CMAKE_SPEECT_SOURCE_DIR}/engine/config/path_impl.h.in 
  ${CMAKE_SPEECT_BINARY_DIR}/engine/src/base/utils/platform/path_impl.h)

install(FILES ${CMAKE_SPEECT_BINARY_DIR}/engine/src/base/utils/platform/path_impl.h
  DESTINATION include/speect/engine/base/utils/platform)

