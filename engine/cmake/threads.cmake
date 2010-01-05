######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 09 May 2009                                                            ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMake Speect multi-threaded support                                              ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

if(WANT_THREADS)
  set(SPCT_USE_THREADS 1)
  if (SPCT_UNIX)
    find_package(Threads REQUIRED)
    set(SPCT_USE_POSIX_THREADS 1)
    set(SPCT_THREADS_SPECIFIC_IMPL "${SPCT_INCLUDE_PTHREADS_FILES}")
    set(SPCT_THREADS_SPECIFIC_IMPL_DIR "pthreads")
  elseif(SPCT_WIN32)
    set(SPCT_USE_WIN32_THREADS 1)
    set(SPCT_THREADS_SPECIFIC_IMPL "${SPCT_INCLUDE_WIN32_THREADS_FILES}")
    set(SPCT_THREADS_SPECIFIC_IMPL_DIR "win32")
  else(SPCT_WIN32)
    message(FATAL_ERROR
      "Unknown thread support")
  endif(SPCT_UNIX)
else(WANT_THREADS)
  set(SPCT_THREADS_SPECIFIC_IMPL "${SPCT_INCLUDE_NO_THREADS_FILES}")
  set(SPCT_THREADS_SPECIFIC_IMPL_DIR "")
endif(WANT_THREADS)


#
# Thread implementation include
#
configure_file(${CMAKE_SOURCE_DIR}/config/threads_impl.h.in 
  ${CMAKE_BINARY_DIR}/src/base/threads/platform/threads_impl.h)

install(FILES ${CMAKE_SOURCE_DIR}/src/base/threads/platform/${SPCT_THREADS_SPECIFIC_IMPL}
        DESTINATION include/spct/engine/base/threads/platform/${SPCT_THREADS_SPECIFIC_IMPL_DIR}/)

install(FILES ${CMAKE_BINARY_DIR}/src/base/threads/platform/threads_impl.h
        DESTINATION include/spct/engine/base/threads/platform)
