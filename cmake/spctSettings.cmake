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
