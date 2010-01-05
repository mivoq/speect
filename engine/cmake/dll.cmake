######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 09 May 2009                                                            ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMake Speect dynamic shared objects support                                      ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


if(SPCT_UNIX)
  if(SPCT_HAVE_DLFCN_H)    
    set(SPCT_DLL_SPECIFIC_IMPL_INCLUDES "${SPCT_INCLUDE_DL_DLL_FILES}")
    set(SPCT_DLL_SPECIFIC_IMPL_DIR "dl")
  elseif(SPCT_HAVE_DLFCN_H)
    message(FATAL_ERROR "Unknown dynamic shared object support")    
  endif(SPCT_HAVE_DLFCN_H)
elseif(SPCT_WIN32)
  set(SPCT_DLL_SPECIFIC_IMPL_INCLUDES "${SPCT_INCLUDE_WIN32_DLL_FILES}")
  set(SPCT_DLL_SPECIFIC_IMPL_DIR "win32")
else(SPCT_WIN32)
  message(FATAL_ERROR "Unknown dynamic shared object support")    
endif(SPCT_UNIX)

#
# Dynamic shared objects include
#
configure_file(${CMAKE_SOURCE_DIR}/config/dynamic_loading_impl.h.in 
  ${CMAKE_BINARY_DIR}/src/pluginmanager/platform/dynamic_loading_impl.h)

install(FILES ${CMAKE_SOURCE_DIR}/src/pluginmanager/platform/${SPCT_DLL_SPECIFIC_IMPL_INCLUDES}
        DESTINATION include/spct/engine/pluginmanager/platform/${SPCT_DLL_SPECIFIC_IMPL_DIR}/)

install(FILES ${CMAKE_BINARY_DIR}/src/pluginmanager/platform/dynamic_loading_impl.h
        DESTINATION include/spct/engine/pluginmanager/platform)
