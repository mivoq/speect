######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 09 May 2009                                                            ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Speect Engine library build                                                      ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


#------------------------------------------------------------------------------------#
#                           C header files path                                      #
#------------------------------------------------------------------------------------#

# Search for C header files in these directories.
list(APPEND SPCT_INCLUDE_DIRS_TMP
  ${CMAKE_SOURCE_DIR}/engine/src
  ${CMAKE_BINARY_DIR}/engine/src)

include_directories(${SPCT_INCLUDE_DIRS_TMP})

# set SPCT_INCLUDE_DIRS_TMP as SPCT_INCLUDE_DIRS in cache
set(SPCT_INCLUDE_DIRS ${SPCT_INCLUDE_DIRS_TMP} 
  CACHE INTERNAL "Speect Engine include directories" FORCE)


#------------------------------------------------------------------------------------#
#                           Libraries output path                                    #
#------------------------------------------------------------------------------------#

# Put libraries into `lib'.
set(SPCT_LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/engine/lib)


#------------------------------------------------------------------------------------#
#                              Source files                                          #
#------------------------------------------------------------------------------------#

list(APPEND SPCT_LIBRARY_SOURCES ${SPCT_SRC_FILES})


#------------------------------------------------------------------------------------#
#                     Extra platform libraries to link against                       #
#------------------------------------------------------------------------------------#

###################################### UNIX #########################################

if(SPCT_UNIX) # not MACOSX
  list(APPEND SPCT_PLATFORM_LIBS m) # math library
endif(SPCT_UNIX)


###################################### WIN32 #########################################

if(SPCT_WIN32)
  list(APPEND SPCT_PLATFORM_LIBS kernel32 user32 winmm)
endif(SPCT_WIN32)
  
 
###################################### MACOSX ########################################

if(SPCT_MACOSX)
  find_library(APPKIT_LIBRARY AppKit)
  find_library(IOKIT_LIBRARY IOKit)
  list(APPEND SPCT_PLATFORM_LIBS ${APPKIT_LIBRARY})
  list(APPEND SPCT_PLATFORM_LIBS ${IOKIT_LIBRARY})
  set(SPCT_LIBRARY_LINK_FLAGS "-flat_namespace -undefined suppress")
endif(SPCT_MACOSX)


#------------------------------------------------------------------------------------#
#                              Speect Engine library                                 #
#------------------------------------------------------------------------------------#

set(SPCT_LIBRARIES spct)

if(NOT BUILD_SHARED_LIBS)
  set(SPCT_LIBRARIES ${SPCT_LIBRARIES}-static)
  set(static_flag "-DSPCT_STATICLINK")
endif(NOT BUILD_SHARED_LIBS)

add_library(SPCT_LIBRARIES ${SPCT_LIBRARY_SOURCES})

set_target_properties(SPCT_LIBRARIES
  PROPERTIES
  VERSION "${SPCT_VERSION_MAJOR}.${SPCT_VERSION_MINOR}.${SPCT_VERSION_PATCH}"
  SOVERSION "${SPCT_VERSION_MAJOR}"
  COMPILE_FLAGS "${SPCT_LIBRARY_CFLAGS} -DSPCT_SRC ${static_flag}"
  LINK_FLAGS "${SPCT_LIBRARY_LINK_FLAGS}"
  OUTPUT_NAME ${SPCT_LIBRARIES}
  LIBRARY_OUTPUT_DIRECTORY ${SPCT_LIBRARY_OUTPUT_PATH}
  RUNTIME_OUTPUT_DIRECTORY ${SPCT_LIBRARY_OUTPUT_PATH}
#  DEBUG_POSTFIX "-debug"
#  PROFILE_POSTFIX "-profile"
  )

target_link_libraries(SPCT_LIBRARIES ${SPCT_PLATFORM_LIBS})

# plug-ins and examples should link with SPCT_LIBRARIES_TARGET.
set(SPCT_LIBRARIES_TARGET SPCT_LIBRARIES
  CACHE INTERNAL "Speect Engine library target for internal plug-ins" FORCE)


#------------------------------------------------------------------------------------#
#                                Installation                                        #
#------------------------------------------------------------------------------------#

###################################### UNIX #########################################

if(SPCT_UNIX)

  # Install header files.  
  install(DIRECTORY "${CMAKE_SOURCE_DIR}/engine/src/"
    DESTINATION include/speect/engine
    FILES_MATCHING PATTERN "*.h"
    PATTERN "platform" EXCLUDE)

  install(DIRECTORY "${CMAKE_BINARY_DIR}/engine/src/"
    DESTINATION include/speect/engine
    FILES_MATCHING PATTERN "*.h"
    PATTERN "platform" EXCLUDE)
  
  # Install library
  install(TARGETS SPCT_LIBRARIES
    DESTINATION "lib${LIB_SUFFIX}/speect/engine"
    EXPORT spctConfig 
    )

  # export CMake config
  install(EXPORT spctConfig DESTINATION include/speect/engine)
endif(SPCT_UNIX)
