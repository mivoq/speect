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

# find Python 
find_package(PythonLibs REQUIRED) # Note, currently PythonLibs does not look for Python 3

# include Python
include_directories(${PYTHON_INCLUDE_DIRS})  # PYTHON_INCLUDE_PATH (deprecated)

# link with Speect SWIG Python native library and Python
speect_plugin_create(${SPCT_PYTHON_NATIVE_LIBRARY_TARGET} ${PYTHON_LIBRARIES})
  

#------------------------------------------------------------------------------------#
#                              SWIG wrappers                                         #
#------------------------------------------------------------------------------------#

add_subdirectory(swig)
