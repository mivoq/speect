######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 8 June 2010                                                            ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Speect Engine source files                                                       ##
## Memory mapped file implementations abstraction                                   ##
##                                                                                  ##
######################################################################################

# SPCT_MMAP_SPECIFIC_IMPL_INCLUDES : list of implementation include files
# SPCT_MMAP_SPECIFIC_IMPL_DIR      : implementation directory


set(SPCT_MMAP_SUPPORT 0)

#------------------------------------------------------------------------------------#
#                              POSIX implementation                                  #
#------------------------------------------------------------------------------------#

set(SPCT_SRC_POSIX_MMAP_FILES
  posix/posix_mmapfile.c
)

set(SPCT_INCLUDE_POSIX_MMAP_FILES
  posix/posix_mmapfile.h
)


if(SPCT_UNIX OR SPCT_MACOSX)
  if(SPCT_HAVE_SYS_TYPES_H AND SPCT_HAVE_SYS_STAT_H AND SPCT_HAVE_SYS_MMAN_H
      AND SPCT_HAVE_FCNTL_H AND SPCT_HAVE_UNISTD_H)
    set(SPCT_MMAP_SPECIFIC_IMPL_INCLUDES "${SPCT_INCLUDE_POSIX_MMAP_FILES}")
    set(SPCT_MMAP_SPECIFIC_IMPL_DIR "posix")
    list(APPEND SPCT_LIBRARY_SOURCES "${CMAKE_SOURCE_DIR}/engine/src/datasources/platform/${SPCT_SRC_POSIX_MMAP_FILES}")

    set(SPCT_MMAP_SUPPORT 1)
  elseif(SPCT_HAVE_SYS_TYPES_H AND SPCT_HAVE_SYS_STAT_H AND SPCT_HAVE_SYS_MMAN_H
      AND SPCT_HAVE_FCNTL_H AND SPCT_HAVE_UNISTD_H)
    message(FATAL_ERROR "Unknown memory mapped file support.")    
  endif(SPCT_HAVE_SYS_TYPES_H AND SPCT_HAVE_SYS_STAT_H AND SPCT_HAVE_SYS_MMAN_H
      AND SPCT_HAVE_FCNTL_H AND SPCT_HAVE_UNISTD_H)
endif(SPCT_UNIX OR SPCT_MACOSX)
 

#------------------------------------------------------------------------------------#
#                              WIN32 implementation                                  #
#------------------------------------------------------------------------------------#

set(SPCT_SRC_WIN32_MMAP_FILES
  win32/win32_mmapfile.c
)

set(SPCT_INCLUDE_WIN32_MMAP_FILES
  win32/win32_mmapfile.h
)

if(SPCT_WIN32)
  set(SPCT_MMAP_SPECIFIC_IMPL_INCLUDES "${SPCT_INCLUDE_WIN32_MMAP_FILES}")
  set(SPCT_MMAP_SPECIFIC_IMPL_DIR "win32")
  list(APPEND SPCT_LIBRARY_SOURCES "${CMAKE_SOURCE_DIR}/engine/src/datasources/platform/${SPCT_SRC_WIN32_MMAP_FILES}")

  set(SPCT_MMAP_SUPPORT 1)
endif(SPCT_WIN32)


#------------------------------------------------------------------------------------#
#                               engine header                                        #
#------------------------------------------------------------------------------------#

configure_file(${CMAKE_SOURCE_DIR}/engine/config/mmapfile_impl.h.in 
  ${CMAKE_BINARY_DIR}/engine/src/datasources/platform/mmapfile_impl.h)


#------------------------------------------------------------------------------------#
#                               installation                                         #
#------------------------------------------------------------------------------------#

install(FILES ${CMAKE_SOURCE_DIR}/engine/src/datasources/platform/${SPCT_MMAP_SPECIFIC_IMPL_INCLUDES}
  DESTINATION include/speect/engine/datasources/platform/${SPCT_MMAP_SPECIFIC_IMPL_DIR}/)

install(FILES ${CMAKE_BINARY_DIR}/engine/src/datasources/platform/mmapfile_impl.h
  DESTINATION include/speect/engine/datasources/platform)


#------------------------------------------------------------------------------------#
#                          Error if no implementation                                #
#------------------------------------------------------------------------------------#

if(NOT SPCT_MMAP_SUPPORT)
  message(FATAL_ERROR "Unknown memory mapped file support.")    
endif(NOT SPCT_MMAP_SUPPORT)
