######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 09 May 2009                                                            ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMake Speect memory mapped file support                                          ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


if(SPCT_UNIX)
  if(SPCT_HAVE_SYS_TYPES_H AND SPCT_HAVE_SYS_STAT_H AND SPCT_HAVE_SYS_MMAN_H
      AND SPCT_HAVE_FCNTL_H AND SPCT_HAVE_UNISTD_H)
    set(SPCT_MMAP_SPECIFIC_IMPL_INCLUDES "${SPCT_INCLUDE_MMAP_POSIX_FILES}")
    set(SPCT_MMAP_SPECIFIC_IMPL_DIR "posix")
  elseif(SPCT_HAVE_SYS_TYPES_H AND SPCT_HAVE_SYS_STAT_H AND SPCT_HAVE_SYS_MMAN_H
      AND SPCT_HAVE_FCNTL_H AND SPCT_HAVE_UNISTD_H)
    message(FATAL_ERROR "Unknown memory mapped file support")    
  endif(SPCT_HAVE_SYS_TYPES_H AND SPCT_HAVE_SYS_STAT_H AND SPCT_HAVE_SYS_MMAN_H
      AND SPCT_HAVE_FCNTL_H AND SPCT_HAVE_UNISTD_H)
elseif(SPCT_WIN32)
  set(SPCT_MMAP_SPECIFIC_IMPL_INCLUDES "${SPCT_INCLUDE_MMAP_WIN32_FILES}")
  set(SPCT_MMAP_SPECIFIC_IMPL_DIR "win32")
else(SPCT_WIN32)
  message(FATAL_ERROR "Unknown memory mapped file support")   
endif(SPCT_UNIX)

#
# Memory mapped files include
#
configure_file(${CMAKE_SOURCE_DIR}/config/mmapfile_impl.h.in 
  ${CMAKE_BINARY_DIR}/src/datasources/platform/mmapfile_impl.h)

install(FILES ${CMAKE_SOURCE_DIR}/src/datasources/platform/${SPCT_MMAP_SPECIFIC_IMPL_INCLUDES}
        DESTINATION include/spct/engine/datasources/platform/${SPCT_MMAP_SPECIFIC_IMPL_DIR}/)

install(FILES ${CMAKE_BINARY_DIR}/src/datasources/platform/mmapfile_impl.h
        DESTINATION include/spct/engine/datasources/platform)
