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
if(NOT "${CMAKE_SPEECT_SOURCE_DIR}" STREQUAL "${CMAKE_SPEECT_BINARY_DIR}")
  speect_file_copy(${CMAKE_SPEECT_SOURCE_DIR}/engine/examples/utils/test_file.txt
    ${CMAKE_SPEECT_BINARY_DIR}/engine/examples/utils)
endif(NOT "${CMAKE_SPEECT_SOURCE_DIR}" STREQUAL "${CMAKE_SPEECT_BINARY_DIR}")
