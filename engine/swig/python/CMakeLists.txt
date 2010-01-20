######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : December 2009                                                          ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for Speect Engine Python interface                                     ##
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
include_directories(${PYTHON_INCLUDE_DIRS})

# add Speect Engine includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# clear SWIG flags, we don't need anything here
set(CMAKE_SWIG_FLAGS "")

# clear C flags, we don't want the same stuff as Speect
set(CMAKE_C_FLAGS "")

# add flags for Python Speect Engine wrapper generation
set_source_files_properties(speect.i 
  PROPERTIES 
  SWIG_FLAGS "-Wall"
)

# add flags for Python Speect Engine wrapper compilation
set_source_files_properties(speectPYTHON_wrap.c 
  PROPERTIES 
  COMPILE_FLAGS "-fno-strict-aliasing -fwrapv -Wstrict-prototypes -Wall"
  GENERATED TRUE
)

# set ouput directory
#message(STATUS "CMAKE_CURRENT_BIN_DIR = ${CMAKE_BINARY_DIR}")
set(CMAKE_SWIG_OUTDIR ${CMAKE_BINARY_DIR}/../swig/python)

# create the Python Speect Engine wrapper (C and Python files)
swig_add_module(speect python speect.i)

# link with Python and Speect Engine
swig_link_libraries(speect ${PYTHON_LIBRARIES} ${SPCT_LINK_WITH})

# set the output directory
set_target_properties(${SWIG_MODULE_speect_REAL_NAME}
  PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../swig/python
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../swig/python
)

