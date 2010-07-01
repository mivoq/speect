######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : June 2010                                                              ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for JSON Lexicon examples                                              ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

speect_example(load_lexicon)

if(NOT "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  speect_file_copy(${CMAKE_CURRENT_SOURCE_DIR}/lexicon_example.txt
    ${CMAKE_CURRENT_BINARY_DIR}/)
endif(NOT "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")


