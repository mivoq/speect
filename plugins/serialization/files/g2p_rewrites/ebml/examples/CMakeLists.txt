######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : June 2010                                                              ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for EBML G2P Rewrites examples                                         ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

speect_example(load_g2p)

if(NOT "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  speect_file_copy(${CMAKE_CURRENT_SOURCE_DIR}/lwazi_english_g2p.spct
    ${CMAKE_CURRENT_BINARY_DIR}/)
endif(NOT "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")


