######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : May 2010                                                               ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMake Speect platform specific file path functions support                       ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

if(SPCT_WIN32)
  set(SPCT_PATH_SPECIFIC_IMPL "${SPCT_INCLUDE_WIN32_PATH_FILES}")
  set(SPCT_PATH_SPECIFIC_IMPL_DIR "win32")
else(SPCT_WIN32)
  set(SPCT_PATH_SPECIFIC_IMPL "${SPCT_INCLUDE_POSIX_PATH_FILES}")
  set(SPCT_PATH_SPECIFIC_IMPL_DIR "posix")
endif(SPCT_WIN32)


#
# File path functions implementation include
#
configure_file(${CMAKE_SOURCE_DIR}/config/path_impl.h.in 
  ${CMAKE_BINARY_DIR}/src/base/utils/platform/path_impl.h)

install(FILES ${CMAKE_SOURCE_DIR}/src/base/utils/platform/${SPCT_PATH_SPECIFIC_IMPL}
        DESTINATION include/spct/engine/base/utils/platform/${SPCT_PATH_SPECIFIC_IMPL_DIR}/)

install(FILES ${CMAKE_BINARY_DIR}/src/base/utils/platform/path_impl.h
        DESTINATION include/spct/engine/base/utils/platform)

