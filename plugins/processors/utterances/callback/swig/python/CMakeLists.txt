######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : December 2009                                                          ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for Utterance Processor Python callback interface                      ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


# find swig
find_package(SWIG REQUIRED)

# include swig
include(${SWIG_USE_FILE})

# find Python interpreter
find_package(PythonLibs REQUIRED) # Note, currently PythonInterp does not look for Python 3

# add Python to includes
include_directories(${PYTHON_INCLUDE_PATH})

# add Speect Engine includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# clear SWIG flags, we don't need anything here
set(CMAKE_SWIG_FLAGS "")

# clear C flags, we don't want the same stuff as Speect
set(CMAKE_C_FLAGS "")

# add flags for Python Speect Engine wrapper generation
set_source_files_properties(uttproc_callback.i 
  PROPERTIES 
  SWIG_FLAGS "-Wall"
  SWIG_FLAGS "-I${SPEECT_PYTHON_WRAPPER_INCLUDE_PATH}"
)

# add flags for Python Speect Engine wrapper compilation
set_source_files_properties(uttproc_callbackPYTHON_wrap.c 
  PROPERTIES 
  COMPILE_FLAGS "-fno-strict-aliasing -fwrapv -Wstrict-prototypes -Wall"
  GENERATED TRUE
)

# set ouput directory
#message(STATUS "CMAKE_CURRENT_BIN_DIR = ${CMAKE_BINARY_DIR}")
set(CMAKE_SWIG_OUTDIR "${PLUGINS_ENV_VAR}/../swig/python")

# create the Python Speect Engine wrapper (C and Python files)
swig_add_module(uttproc_callback python uttproc_callback.i)

# link with Python and Speect Engine
swig_link_libraries(uttproc_callback ${PYTHON_LIBRARIES} SPCT_LIBRARY_TARGET)

# set the output directory
set_target_properties(${SWIG_MODULE_uttproc_callback_REAL_NAME}
  PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${PLUGINS_ENV_VAR}/../swig/python"
  RUNTIME_OUTPUT_DIRECTORY "${PLUGINS_ENV_VAR}/../swig/python"
)

#configure_file(${CMAKE_SOURCE_DIR}/swig/python/config/setup.py.in 
#  ${CMAKE_BINARY_DIR}/swig/python/setup.py @only)

