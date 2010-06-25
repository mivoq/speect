######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : June 2010                                                              ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for EBML Utterance loading/saving examples                             ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

speect_example(save_utt)
speect_example(load_utt)

if(NOT "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/save_utt.py
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
endif(NOT "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")



