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
#                          Speect initialization file                                #
#------------------------------------------------------------------------------------#

######### UNIX ###########

if(SPCT_UNIX)
  
  set(SPCT_BUILD_INI_FILE "${CMAKE_BINARY_DIR}/speect.conf")
  mark_as_advanced(SPCT_BUILD_INI_FILE)

  set(SPCT_INSTALL_INI_FILE "/etc/speect.conf")
  mark_as_advanced(SPCT_INSTALL_INI_FILE)
endif(SPCT_UNIX)


#------------------------------------------------------------------------------------#
#             Plug-in path (for Speect Engine initialization file)                   #
#------------------------------------------------------------------------------------#

set(SPCT_PLUGIN_PATH "${CMAKE_BINARY_DIR}/plugins/lib")
mark_as_advanced(SPCT_PLUGIN_PATH)

#------------------------------------------------------------------------------------#
#               Speect Engine Debug & Error Logging files                            #
#------------------------------------------------------------------------------------#

set(SPCT_ERROR_LOG_FILE "${CMAKE_BINARY_DIR}/spct_error.log")
mark_as_advanced(SPCT_ERROR_LOG_FILE)

set(SPCT_DEBUG_LOG_FILE "${CMAKE_BINARY_DIR}/spct_debug.log")
mark_as_advanced(SPCT_DEBUG_LOG_FILE)


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

# FIXME, for now this must be set otherwise we get undefined behaviour
set(SPCT_ERROR_HANDLING 1)

#if(ERROR_HANDLING)
#  set(SPCT_ERROR_HANDLING 1)
#else(ERROR_HANDLING)
#  unset(SPCT_ERROR_HANDLING)
#endif(ERROR_HANDLING)