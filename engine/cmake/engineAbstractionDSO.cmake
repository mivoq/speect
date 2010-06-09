######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 8 June 2010                                                            ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Speect Engine source files                                                       ##
## Dynamic shared object implementations abstraction                                ##
##                                                                                  ##
######################################################################################

# SPCT_DSO_SPECIFIC_IMPL_INCLUDES : list of implementation include files
# SPCT_DSO_SPECIFIC_IMPL_DIR      : implementation directory


set(SPCT_DSO_SUPPORT 0)

#------------------------------------------------------------------------------------#
#                              POSIX implementation                                  #
#------------------------------------------------------------------------------------#

set(SPCT_SRC_POSIX_DSO_FILES
  posix/posix_dll.c
)

set(SPCT_INCLUDE_POSIX_DSO_FILES
  posix/posix_dll.h
)

# add sources to library
if(SPCT_UNIX OR SPCT_MACOSX)
  if(SPCT_HAVE_DLFCN_H)    
    set(SPCT_DSO_SPECIFIC_IMPL_INCLUDES "${SPCT_INCLUDE_POSIX_DSO_FILES}")
    set(SPCT_DSO_SPECIFIC_IMPL_DIR "posix")
    list(APPEND SPCT_LIBRARY_SOURCES "${CMAKE_SOURCE_DIR}/engine/src/pluginmanager/platform/${SPCT_SRC_POSIX_DSO_FILES}")
    list(APPEND SPCT_PLATFORM_LIBS dl)
    
    set(SPCT_DSO_SUPPORT 1)
  elseif(SPCT_HAVE_DLFCN_H)
    message(FATAL_ERROR "Unknown dynamic shared object support.")    
  endif(SPCT_HAVE_DLFCN_H)
endif(SPCT_UNIX OR SPCT_MACOSX)


#------------------------------------------------------------------------------------#
#                              WIN32 implementation                                  #
#------------------------------------------------------------------------------------#

set(SPCT_SRC_WIN32_DSO_FILES
  win32/win32_dll.c
)

set(SPCT_INCLUDE_WIN32_DSO_FILES
  win32/win32_dll.h
)


if(SPCT_WIN32)
  set(SPCT_DLL_SPECIFIC_IMPL_INCLUDES "${SPCT_INCLUDE_WIN32_DSO_FILES}")
  set(SPCT_DLL_SPECIFIC_IMPL_DIR "win32")
  list(APPEND SPCT_LIBRARY_SOURCES "${CMAKE_SOURCE_DIR}/engine/src/pluginmanager/platform/${SPCT_SRC_WIN32_DSO_FILES}")

  set(SPCT_DSO_SUPPORT 1)
endif(SPCT_WIN32)


#------------------------------------------------------------------------------------#
#                               engine header                                        #
#------------------------------------------------------------------------------------#

configure_file(${CMAKE_SOURCE_DIR}/engine/config/dynamic_loading_impl.h.in 
  ${CMAKE_BINARY_DIR}/engine/src/pluginmanager/platform/dynamic_loading_impl.h)


#------------------------------------------------------------------------------------#
#                               installation                                         #
#------------------------------------------------------------------------------------#

install(FILES ${CMAKE_SOURCE_DIR}/engine/src/pluginmanager/platform/${SPCT_DSO_SPECIFIC_IMPL_INCLUDES}
        DESTINATION include/speect/engine/pluginmanager/platform/${SPCT_DSO_SPECIFIC_IMPL_DIR}/)

install(FILES ${CMAKE_BINARY_DIR}/engine/src/pluginmanager/platform/dynamic_loading_impl.h
        DESTINATION include/speect/engine/pluginmanager/platform)


#------------------------------------------------------------------------------------#
#                          Error if no implementation                                #
#------------------------------------------------------------------------------------#

if(NOT SPCT_DSO_SUPPORT)
  message(FATAL_ERROR "Unknown dynamic shared object support.")    
endif(NOT SPCT_DSO_SUPPORT)
