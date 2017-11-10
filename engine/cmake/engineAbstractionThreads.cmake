######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 8 June 2010                                                            ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Speect Engine source files                                                       ##
## Threads implementations abstraction                                              ##
##                                                                                  ##
######################################################################################

# SPCT_THREADS_SPECIFIC_IMPL_INCLUDES : list of implementation include files
# SPCT_THREADS_SPECIFIC_IMPL_DIR      : implementation directory


set(SPCT_THREADS_SUPPORT 0)

#------------------------------------------------------------------------------------#
#                          pthreads implementation                                   #
#------------------------------------------------------------------------------------#

set(SPCT_SRC_PTHREADS_FILES
  pthreads/pthreads_threads.c
)

set(SPCT_INCLUDE_PTHREADS_FILES
  pthreads/pthreads_threads.h
)


if(SPCT_UNIX OR SPCT_MACOSX)
  if(WANT_THREADS)
    find_package(Threads REQUIRED)
    set(SPCT_THREADS_SPECIFIC_IMPL "${SPCT_INCLUDE_PTHREADS_FILES}")
    set(SPCT_THREADS_SPECIFIC_IMPL_DIR "pthreads")

    list(APPEND SPCT_LIBRARY_SOURCES "${CMAKE_SPEECT_SOURCE_DIR}/engine/src/base/threads/platform/${SPCT_SRC_PTHREADS_FILES}")
    list(APPEND SPCT_PLATFORM_LIBS pthread)

    set(SPCT_THREADS_SUPPORT 1)

    # activate thread support macros
    set(SPCT_USE_THREADS 1)

  endif(WANT_THREADS)
endif(SPCT_UNIX OR SPCT_MACOSX)


#------------------------------------------------------------------------------------#
#                             WIN32 implementation                                   #
#------------------------------------------------------------------------------------#

set(SPCT_SRC_WIN32_THREADS_FILES
  win32/win32_threads.c
)

set(SPCT_INCLUDE_WIN32_THREADS_FILES
  win32/win32_threads.h
)

if(SPCT_WIN32)
  if(WANT_THREADS)
    set(SPCT_THREADS_SPECIFIC_IMPL "${SPCT_INCLUDE_WIN32_THREADS_FILES}")
    set(SPCT_THREADS_SPECIFIC_IMPL_DIR "win32")

    list(APPEND SPCT_LIBRARY_SOURCES "${CMAKE_SPEECT_SOURCE_DIR}/engine/src/base/threads/platform/${SPCT_SRC_WIN32_THREADS_FILES}")

    set(SPCT_THREADS_SUPPORT 1)

    # activate thread support macros
    set(SPCT_USE_THREADS 1)
  endif(WANT_THREADS)
endif(SPCT_WIN32)
  

#------------------------------------------------------------------------------------#
#                                  no threads                                        #
#------------------------------------------------------------------------------------#

set(SPCT_INCLUDE_NO_THREADS_FILES
  threads_none.h
)

if(NOT WANT_THREADS)
  set(SPCT_THREADS_SPECIFIC_IMPL "${SPCT_INCLUDE_NO_THREADS_FILES}")
  set(SPCT_THREADS_SPECIFIC_IMPL_DIR "")
endif(NOT WANT_THREADS)


#------------------------------------------------------------------------------------#
#                               engine header                                        #
#------------------------------------------------------------------------------------#

configure_file(${CMAKE_SPEECT_SOURCE_DIR}/engine/config/threads_impl.h.in 
  ${CMAKE_SPEECT_BINARY_DIR}/engine/src/base/threads/platform/threads_impl.h)


#------------------------------------------------------------------------------------#
#                               installation                                         #
#------------------------------------------------------------------------------------#

install(FILES ${CMAKE_SPEECT_SOURCE_DIR}/engine/src/base/threads/platform/${SPCT_THREADS_SPECIFIC_IMPL}
  DESTINATION include/speect/engine/base/threads/platform/${SPCT_THREADS_SPECIFIC_IMPL_DIR}/)

install(FILES ${CMAKE_SPEECT_BINARY_DIR}/engine/src/base/threads/platform/threads_impl.h
  DESTINATION include/speect/engine/base/threads/platform)


#------------------------------------------------------------------------------------#
#                          Error if no implementation                                #
#------------------------------------------------------------------------------------#

if(WANT_THREADS)
  if(NOT SPCT_THREADS_SUPPORT)
    message(FATAL_ERROR "Unknown thread support.")
  endif(NOT SPCT_THREADS_SUPPORT)
endif(WANT_THREADS)
