######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 5 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for Speect primitive data types ebml format plug-in                    ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#------------------------------------------------------------------------------------#
#                             Define plug-in                                         #
#------------------------------------------------------------------------------------#

speect_plugin_definition(PyObject_EBML "SEbmlPyObject" 1 0 0)


#------------------------------------------------------------------------------------#
#                        Configure plugin_info.h.in                                  #
#------------------------------------------------------------------------------------#

set(description "SSerializedObjectClass implementation for Python objects data types in EBML format")

# Minimum required Speect Engine version 
set(major_min 1)
set(minor_min 0)

speect_plugin_configure_info(${description} ${major_min} ${minor_min})


#------------------------------------------------------------------------------------#
#                               Source files                                         #
#------------------------------------------------------------------------------------#

# Lists of all the source files
include(sources)


#------------------------------------------------------------------------------------#
#                              Include ebml plug-in                                  #
#------------------------------------------------------------------------------------#

include(ebml)


#------------------------------------------------------------------------------------#
#                            Plug-in shared object                                   #
#------------------------------------------------------------------------------------#

# include Speect SWIG Python native headers
include_directories(${SPCT_PYTHON_NATIVE_INCLUDE_DIRS})

# has speect_find_python macro
include(${CMAKE_SOURCE_DIR}/swig/python/cmake/swigPythonFunctions.cmake)

# find Python
speect_find_python()

# has speect_include_python_directories macro
include(${CMAKE_SOURCE_DIR}/swig/python/cmake/swigPythonFunctions.cmake)

# include Python
speect_include_python_directories()

# link with Speect SWIG Python native library and Python
speect_plugin_create(${SPCT_PYTHON_NATIVE_LIBRARY_TARGET} ${PYTHON_LIBRARIES})
  

#------------------------------------------------------------------------------------#
#                              SWIG wrappers                                         #
#------------------------------------------------------------------------------------#

add_subdirectory(swig)
