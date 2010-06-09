######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : June 2010                                                              ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for Speect Engine SWIG common source files                             ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


#------------------------------------------------------------------------------------#
#             Speect Engine SWIG common source files list                            #
#------------------------------------------------------------------------------------#

# generate a source file list, pointing to the absolute path of the sources so
# that we can use the CMake variable SPCT_SWIG_COMMON_SRC_FILES when the file
# is included in other projects

configure_file(${CMAKE_SOURCE_DIR}/engine/swig/common/config/engineSWIGcommon.cmake.in 
  ${CMAKE_BINARY_DIR}/engine/swig/common/cmake/engineSWIGcommon.cmake)


#------------------------------------------------------------------------------------#
#             Speect Engine SWIG common interface file                               #
#------------------------------------------------------------------------------------#

# generate the interface file

configure_file(${CMAKE_SOURCE_DIR}/engine/swig/common/config/speect_common.i.in 
  ${CMAKE_BINARY_DIR}/engine/swig/common/speect_common.i)

