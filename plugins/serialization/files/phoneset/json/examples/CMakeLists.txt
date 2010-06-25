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

if(NOT "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/phoneset_example.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
endif(NOT "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")


