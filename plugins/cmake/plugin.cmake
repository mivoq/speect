######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 5 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Template CMake plug-in shared object build                                       ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


################################## Variables #########################################

# Libraries that we always need to link against on this platform.
set(PLATFORM_LIBS)


######################################################################################

# Search for C header files in these directories.
list(APPEND ${PLUGIN_SONAME}_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${SPEECT_INCLUDE_PATH}
  )

include_directories(${${PLUGIN_SONAME}_INCLUDE_DIRS})


######################################################################################
 
# Put libraries into `lib'.
set(LIBRARY_OUTPUT_PATH "${PLUGINS_ENV_VAR}/lib")


######################################################################################

# List of source files need to compile plug-in in this configuration on
# this platform.
set(LIBRARY_SOURCES
    ${${PLUGIN_SONAME}_SRC_FILES}
    ${${PLUGIN_SONAME}_INCLUDE_FILES}
    )


######################################################################################

# For intellisense.
if(COMPILER_MSVC)
    list(APPEND LIBRARY_SOURCES
        ${${PLUGIN_SONAME}_SRC_INCLUDE_FILES}
	${${PLUGIN_SONAME}_SRC_INTERNAL_INCLUDE_FILES}
	${${PLUGIN_SONAME}_COMMON_INCLUDE_FILES}
        )
endif(COMPILER_MSVC)


####################################### UNIX's #######################################

#if(UNIX) # not MACOSX
  #   list(APPEND LIBRARY_SOURCES ${PLUGIN_SRC_UNIX_FILES})
  # list(APPEND PLATFORM_LIBS m)
#endif(UNIX)


###################################### WIN32 #########################################

#if(WIN32)
#    list(APPEND LIBRARY_SOURCES ${PLUGIN_SRC_WIN32_FILES})
#    # TODO this still needs to be produced with misc/fixdll.sh
#    if(MINGW AND SHARED)
#        list(APPEND LIBRARY_SOURCES lib/mingw32/spct.def)
#    endif(MINGW AND SHARED)
#    if(COMPILER_MSVC AND SHARED)
#        list(APPEND LIBRARY_SOURCES lib/msvc/spct.def)
#    endif(COMPILER_MSVC AND SHARED)
#    list(APPEND PLATFORM_LIBS
#        kernel32 user32 gdi32 comdlg32 ole32 dxguid winmm psapi
#        )
#    list(APPEND PLATFORM_LIBS ${DINPUT_LIBRARIES})
#    if(MINGW AND NOT SHARED)
#        list(APPEND PLATFORM_LIBS stdc++)
#    endif(MINGW AND NOT SHARED)
#endif(WIN32)


###################################### MACOSX ########################################

#if(MACOSX)
#    list(APPEND LIBRARY_SOURCES ${PLUGIN_SRC_MACOSX_FILES})
#    find_library(APPKIT_LIBRARY AppKit)
#    find_library(IOKIT_LIBRARY IOKit)
#    list(APPEND PLATFORM_LIBS ${APPKIT_LIBRARY})
#    list(APPEND PLATFORM_LIBS ${IOKIT_LIBRARY})
#endif(MACOSX)


############################ Plugin library ##########################################

set(${PLUGIN_SONAME}_plugin ${PLUGIN_SONAME})

add_library(${PLUGIN_SONAME}_plugin ${LIBRARY_SOURCES})

set_target_properties(${PLUGIN_SONAME}_plugin
  PROPERTIES
  VERSION "${${PLUGIN_SONAME}_VERSION_MAJOR}.${${PLUGIN_SONAME}_VERSION_MINOR}.${${PLUGIN_SONAME}_VERSION_PATCH}"
  SOVERSION "${${PLUGIN_SONAME}_VERSION_MAJOR}"
  COMPILE_FLAGS "${LIBRARY_CFLAGS}"
  LINK_FLAGS "${LIBRARY_LINK_FLAGS}"
  OUTPUT_NAME ${${PLUGIN_SONAME}_plugin}
  PREFIX ""
  SUFFIX ".spi"
  )

# link with Speect library
target_link_libraries(${PLUGIN_SONAME}_plugin SPCT_LIBRARY_TARGET)

set(${PLUGIN_SONAME}_TARGET_NAME "${${PLUGIN_SONAME}_plugin}.spi")


######################################################################################

# Install header files.

#install(FILES ${PLUGIN_COMMON_INCLUDE_FILES}
#        DESTINATION include
#        )
#install(FILES ${PLUGIN_SRC_INCLUDE_FILES}
#        DESTINATION include/spct/src/base
#        )
#install(FILES ${PLUGIN_SRC_INTERNAL_INCLUDE_FILES}
#        DESTINATION include/spct/internal
#        )


######################################################################################
