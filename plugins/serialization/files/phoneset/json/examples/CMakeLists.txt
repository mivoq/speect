######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : June 2010                                                              ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for JSON Phoneset examples                                             ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

speect_example(load_phoneset)

if(NOT "${CMAKE_SPEECT_SOURCE_DIR}" STREQUAL "${CMAKE_SPEECT_BINARY_DIR}")
  speect_file_copy(${CMAKE_CURRENT_SOURCE_DIR}/phoneset_example.txt
    ${CMAKE_CURRENT_BINARY_DIR}/)
endif(NOT "${CMAKE_SPEECT_SOURCE_DIR}" STREQUAL "${CMAKE_SPEECT_BINARY_DIR}")


