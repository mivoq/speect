######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : June 2010                                                              ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMake custom functions for Speect plug-ins SWIG interfaces                       ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


#------------------------------------------------------------------------------------#
#                    Set the Speect Engine SWIG common interface                     #
#------------------------------------------------------------------------------------#

set(SPCT_SWIG_COMMON_INTERFACE ${CMAKE_BINARY_DIR}/engine/swig/common/speect_common.i)

# get the list of files that above depends on, provides SPCT_SWIG_PYTHON_SRC_FILES
include(${CMAKE_BINARY_DIR}/engine/swig/common/cmake/engineSWIGcommon.cmake)

# set SPCT_ENGINE_SWIG_COMMON_INTERFACE dependencies
set_source_files_properties(SPCT_SWIG_COMMON_INTERFACE
  PROPERTIES 
  OBJECT_DEPENDS SPCT_SWIG_COMMON_SRC_FILES
  GENERATED TRUE
)


#------------------------------------------------------------------------------------#
#                             SWIG Python functions                                  #
#------------------------------------------------------------------------------------#

if(WANT_PYTHON_WRAPPER)
  # include plug-in SWIG Python functions
  include(${CMAKE_SOURCE_DIR}/swig/python/cmake/swigPythonFunctions.cmake)
endif(WANT_PYTHON_WRAPPER)
