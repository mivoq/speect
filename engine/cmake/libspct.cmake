######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 09 May 2009                                                            ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMake Speect library build                                                       ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


################################## Variables #########################################

# Libraries that we always need to link against on this platform.
set(PLATFORM_LIBS)


######################################################################################

# Search for C header files in these directories.
list(APPEND SPCT_INCLUDE_DIRS
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_BINARY_DIR}/src)

include_directories(${SPCT_INCLUDE_DIRS})


######################################################################################
 
# Put libraries into `lib'.
set(SPCT_LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)


######################################################################################

# List of source files need to compile Speect in this configuration on
# this platform.
set(LIBRARY_SOURCES
    ${SPCT_SRC_FILES}
    ${SPCT_SRC_INCLUDE_FILES}
    )


######################################################################################

# System types, set in types.cmake
list(APPEND LIBRARY_SOURCES "${CMAKE_SOURCE_DIR}/src/base/utils/platform/${SPCT_TYPES_SPECIFIC_IMPL}")


######################################################################################

# Threads support

# header files, already set for different platforms in threads.cmake
if(SPCT_USE_THREADS)
  list(APPEND LIBRARY_SOURCES "${CMAKE_SOURCE_DIR}/src/base/threads/platform/${SPCT_THREADS_SPECIFIC_IMPL}")
endif(SPCT_USE_THREADS)

# source files
if(SPCT_UNIX) # not MACOSX
  if(SPCT_USE_THREADS)
    if(SPCT_USE_POSIX_THREADS)
      list(APPEND LIBRARY_SOURCES "src/base/threads/platform/${SPCT_SRC_PTHREADS_FILES}")
      list(APPEND PLATFORM_LIBS pthread)
    endif(SPCT_USE_POSIX_THREADS)
  endif(SPCT_USE_THREADS)
endif(SPCT_UNIX)

if(SPCT_WIN32)
  if(SPCT_USE_THREADS)
    list(APPEND LIBRARY_SOURCES "src/base/threads/platform/${SPCT_SRC_WIN32_THREADS_FILES}")
 endif(SPCT_USE_THREADS)
endif(SPCT_WIN32)
  

######################################################################################

# Dynamic shared objects support, same as dll.cmake

# header files, already set for different platforms in dll.cmake
list(APPEND LIBRARY_SOURCES "${CMAKE_SOURCE_DIR}/src/pluginmanager/platform/${SPCT_DLL_SPECIFIC_IMPL_INCLUDES}")

# source files
if(SPCT_UNIX)
  if(SPCT_HAVE_DLFCN_H)  
    list(APPEND LIBRARY_SOURCES "src/pluginmanager/platform/${SPCT_SRC_DL_DLL_FILES}")
    list(APPEND PLATFORM_LIBS dl)
  endif(SPCT_HAVE_DLFCN_H)
endif(SPCT_UNIX)

if(SPCT_WIN32)
  list(APPEND LIBRARY_SOURCES "src/pluginmanager/platform/${SPCT_SRC_WIN32_DLL_FILES}")
endif(SPCT_WIN32)


######################################################################################

# Memory mapped files support, same as mmap.cmake

# header files, already set for different platforms in mmap.cmake
list(APPEND LIBRARY_SOURCES "${CMAKE_SOURCE_DIR}/src/datasources/platform/${SPCT_MMAP_SPECIFIC_IMPL_INCLUDES}")

# source files
if(SPCT_UNIX)
  if(SPCT_HAVE_SYS_TYPES_H AND SPCT_HAVE_SYS_STAT_H AND SPCT_HAVE_SYS_MMAN_H
      AND SPCT_HAVE_FCNTL_H AND SPCT_HAVE_UNISTD_H)
    list(APPEND LIBRARY_SOURCES "src/datasources/platform/${SPCT_SRC_MMAP_POSIX_FILES}")
  endif(SPCT_HAVE_SYS_TYPES_H AND SPCT_HAVE_SYS_STAT_H AND SPCT_HAVE_SYS_MMAN_H
      AND SPCT_HAVE_FCNTL_H AND SPCT_HAVE_UNISTD_H)
endif(SPCT_UNIX)

if(SPCT_WIN32)
  list(APPEND LIBRARY_SOURCES "src/datasources/platform/${SPCT_SRC_MMAP_WIN32_FILES}")
endif(SPCT_WIN32)


####################################### UNIX's #######################################

if(SPCT_UNIX) # not MACOSX
  #   list(APPEND LIBRARY_SOURCES ${SPCT_SRC_UNIX_FILES})
  list(APPEND PLATFORM_LIBS m)
endif(SPCT_UNIX)


###################################### WIN32 #########################################

if(SPCT_WIN32)
  list(APPEND PLATFORM_LIBS kernel32 user32 winmm)
endif(SPCT_WIN32)
  
 
###################################### MACOSX ########################################

#if(MACOSX)
#    list(APPEND LIBRARY_SOURCES ${SPCT_SRC_MACOSX_FILES})
#    find_library(APPKIT_LIBRARY AppKit)
#    find_library(IOKIT_LIBRARY IOKit)
#    list(APPEND PLATFORM_LIBS ${APPKIT_LIBRARY})
#    list(APPEND PLATFORM_LIBS ${IOKIT_LIBRARY})
#endif(MACOSX)

#if(APPLE)
#  set(LIBRARY_LINK_FLAGS "-flat_namespace -undefined suppress")
#endif(APPLE)


############################### Speect Engine library ################################

set(SPCT_LIBRARY_TARGET spct)

if(NOT BUILD_SHARED_LIBS)
  set(SPCT_LIBRARY_TARGET ${SPCT_LIBRARY_TARGET}-static)
  set(static_flag "-DSPCT_STATICLINK")
endif(NOT BUILD_SHARED_LIBS)

add_library(SPCT_LIBRARY_TARGET ${LIBRARY_SOURCES})

set_target_properties(SPCT_LIBRARY_TARGET
  PROPERTIES
  VERSION "${SPCT_VERSION_MAJOR}.${SPCT_VERSION_MINOR}.${SPCT_VERSION_PATCH}"
  SOVERSION "${SPCT_VERSION_MAJOR}"
  COMPILE_FLAGS "${LIBRARY_CFLAGS} -DSPCT_SRC ${static_flag}"
  LINK_FLAGS "${LIBRARY_LINK_FLAGS}"
  OUTPUT_NAME ${SPCT_LIBRARY_TARGET}
  LIBRARY_OUTPUT_DIRECTORY ${SPCT_LIBRARY_OUTPUT_PATH}
  RUNTIME_OUTPUT_DIRECTORY ${SPCT_LIBRARY_OUTPUT_PATH}
#  DEBUG_POSTFIX "-debug"
#  PROFILE_POSTFIX "-profile"
  )

target_link_libraries(SPCT_LIBRARY_TARGET ${PLATFORM_LIBS})

# Addons and examples should link with this target.
set(SPCT_LINK_WITH SPCT_LIBRARY_TARGET)


############################# Installation ###########################################


# Install header files.  
if(SPCT_UNIX)
  install(DIRECTORY src/ 
    DESTINATION include/spct/engine
    FILES_MATCHING PATTERN "*.h"
    PATTERN "platform" EXCLUDE)
  
  install(TARGETS SPCT_LIBRARY_TARGET
    DESTINATION "lib${LIB_SUFFIX}/spct/engine"
    EXPORT spctConfig 
    )

  install(EXPORT spctConfig DESTINATION include/spct/engine)
endif(SPCT_UNIX)

# Also installations in 
# - threads.cmake
# - dll.cmake
# - mmap.cmake
# - types.cmake


######################################################################################
