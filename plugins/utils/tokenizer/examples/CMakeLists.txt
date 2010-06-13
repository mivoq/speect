######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : June 2010                                                              ##
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
  file(COPY ${CMAKE_SOURCE_DIR}/plugins/utils/tokenizer/examples/test_file.txt
    DESTINATION ${CMAKE_BINARY_DIR}/plugins/utils/tokenizer/examples)
endif(NOT "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
