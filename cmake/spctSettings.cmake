######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 8 June 2010                                                            ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Speect settings                                                                  ##
##                                                                                  ##
##                                                                                  ##
######################################################################################



#------------------------------------------------------------------------------------#
#         Plug-in path (see speect/engine/cmake/enginePluginPathLoader.cmake)        #
#------------------------------------------------------------------------------------#

if(SPCT_UNIX)
  set(SPCT_BUILD_PLUGIN_PATH "${CMAKE_BINARY_DIR}/plugins/lib")
  mark_as_advanced(SPCT_BUILD_PLUGIN_PATH)

  set(SPCT_INSTALL_PLUGIN_PATH "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/speect/plugins")
  mark_as_advanced(SPCT_INSTALL_PLUGIN_PATH)
else(SPCT_UNIX)
  set(SPCT_PLUGIN_PATH "${CMAKE_BINARY_DIR}/plugins/lib")
  mark_as_advanced(SPCT_PLUGIN_PATH)
endif(SPCT_UNIX)


#------------------------------------------------------------------------------------#
#                             Speect object casting                                  #
#------------------------------------------------------------------------------------#

# SAFE_CAST is a CMake option in speect/cmake/spctOptions.cmake
if(SAFE_CAST)
  set(SPCT_DO_SAFE_CAST 1)
else(SAFE_CAST)
  unset(SPCT_DO_SAFE_CAST)
endif(SAFE_CAST)


#------------------------------------------------------------------------------------#
#                             Speect Error handling                                  #
#------------------------------------------------------------------------------------#

# ERROR_ABORT_FATAL is a CMake option in speect/cmake/spctOptions.cmake
if(ERROR_ABORT_FATAL)
  set(SPCT_ERROR_ABORT_FATAL 1)
else(ERROR_ABORT_FATAL)
  unset(SPCT_ERROR_ABORT_FATAL)
endif(ERROR_ABORT_FATAL)


# ERROR_HANDLING is a CMake option in speect/cmake/spctOptions.cmake
if(ERROR_HANDLING)
  set(SPCT_ERROR_HANDLING 1)
else(ERROR_HANDLING)
  unset(SPCT_ERROR_HANDLING)
endif(ERROR_HANDLING)