######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : June 2012                                                              ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for Tokenizer examples                                                 ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

speect_example(file_tokenizer)
speect_example(string_tokenizer)

# test file for file_tokenizer example
if(NOT "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  speect_file_copy(${CMAKE_SOURCE_DIR}/engine/examples/utils/test_file.txt
    ${CMAKE_BINARY_DIR}/engine/examples/utils)
endif(NOT "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
