######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : April 2011                                                             ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMake custom functions for Speect Python plug-ins SWIG interfaces                ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


#------------------------------------------------------------------------------------#
#                        Include SWIG Python cmake files                             #
#------------------------------------------------------------------------------------#

# include plug-in SWIG Python settings
include(${CMAKE_SOURCE_DIR}/swig/python/cmake/swigPythonSettings.cmake)

# include plug-in SWIG Python functions
include(${CMAKE_SOURCE_DIR}/swig/python/cmake/swigPythonFunctions.cmake)
