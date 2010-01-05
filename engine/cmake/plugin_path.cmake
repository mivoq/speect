######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 9 Novermber 2009                                                       ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMake look for Speect plug-in path                                               ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

if(NOT SPCT_FOUND_PLUGIN_PATH)
  set(SPCT_FOUND_PLUGIN_PATH 1 CACHE BOOL "Plug-in path has been looked for")
  mark_as_advanced(SPCT_FOUND_PLUGIN_PATH)

  find_path(SPCT_PLUGIN_PATH clean_files "${CMAKE_SOURCE_DIR}/../plugins")
  if(NOT SPCT_PLUGIN_PATH)
    set(SPCT_PLUGIN_PATH ${CMAKE_BINARY_DIR})
    message(STATUS "Warning: plug-in path not found, guessing a path. Please check PLUGIN_PATH variable")
  endif(NOT SPCT_PLUGIN_PATH)
  mark_as_advanced(SPCT_PLUGIN_PATH)
endif(NOT SPCT_FOUND_PLUGIN_PATH)