######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : May 2011                                                               ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for JSON Syllabification rewrites examples                             ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

speect_example(load_syllab_rewrites_json)

if(NOT "${CMAKE_SPEECT_SOURCE_DIR}" STREQUAL "${CMAKE_SPEECT_BINARY_DIR}")
  speect_file_copy(${CMAKE_CURRENT_SOURCE_DIR}/syllabification.json
    ${CMAKE_CURRENT_BINARY_DIR}/)
endif(NOT "${CMAKE_SPEECT_SOURCE_DIR}" STREQUAL "${CMAKE_SPEECT_BINARY_DIR}")


