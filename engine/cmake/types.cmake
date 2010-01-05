######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 6 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMake Speect platform specific types support                                     ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

if(SPCT_WIN32)
  set(SPCT_TYPES_SPECIFIC_IMPL "${SPCT_INCLUDE_WIN32_TYPES_FILES}")
  set(SPCT_TYPES_SPECIFIC_IMPL_DIR "win32")
else(SPCT_WIN32)
  set(SPCT_TYPES_SPECIFIC_IMPL "${SPCT_INCLUDE_POSIX_TYPES_FILES}")
  set(SPCT_TYPES_SPECIFIC_IMPL_DIR "posix")
endif(SPCT_WIN32)


#
# Types implementation include
#
configure_file(${CMAKE_SOURCE_DIR}/config/types_impl.h.in 
  ${CMAKE_BINARY_DIR}/src/base/utils/platform/types_impl.h)

install(FILES ${CMAKE_SOURCE_DIR}/src/base/utils/platform/${SPCT_TYPES_SPECIFIC_IMPL}
        DESTINATION include/spct/engine/base/utils/platform/${SPCT_TYPES_SPECIFIC_IMPL_DIR}/)

install(FILES ${CMAKE_BINARY_DIR}/src/base/utils/platform/types_impl.h
        DESTINATION include/spct/engine/base/utils/platform)

