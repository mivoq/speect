######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 8 June 2010                                                            ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Speect Engine information files                                                  ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#------------------------------------------------------------------------------------#
#                       Speect Engine initialization file                            #
#------------------------------------------------------------------------------------#

set(SPCT_INI_FILE_PATH "${CMAKE_BINARY_DIR}/engine" CACHE STRING "Speect Engine initialization file path (speect.ini)")
mark_as_advanced(SPCT_INI_FILE_PATH)


#------------------------------------------------------------------------------------#
#             Plug-in path (for Speect Engine initialization file)                   #
#------------------------------------------------------------------------------------#

set(SPCT_PLUGIN_PATH "${CMAKE_BINARY_DIR}/plugins/lib" CACHE STRING "Plug-in path")
mark_as_advanced(SPCT_PLUGIN_PATH)


#------------------------------------------------------------------------------------#
#               Speect Engine Debug & Error Logging files                            #
#------------------------------------------------------------------------------------#

set(SPCT_ERROR_LOG_FILE "${CMAKE_BINARY_DIR}/engine/spct_error.log" CACHE STRING "Error logging file.")
mark_as_advanced(SPCT_ERROR_LOG_FILE)

set(SPCT_DEBUG_LOG_FILE "${CMAKE_BINARY_DIR}/engine/spct_debug.log" CACHE STRING "Debug logging file.")
mark_as_advanced(SPCT_DEBUG_LOG_FILE)


#------------------------------------------------------------------------------------#
#                        Speect Engine SWIG common files                             #
#------------------------------------------------------------------------------------#

set(SPCT_SWIG_COMMON_DIR "${CMAKE_SOURCE_DIR}/engine/swig/common" CACHE STRING "Speect Engine SWIG common files")
mark_as_advanced(SPCT_SWIG_COMMON_DIR)

