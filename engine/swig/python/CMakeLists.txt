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

#------------------------------------------------------------------------------------#
#                Speect SWIG Python native interface library                         #
#------------------------------------------------------------------------------------#

add_subdirectory(native)

# include Speect SWIG Python native headers
include_directories(${SPCT_PYTHON_NATIVE_INCLUDE_DIRS})


#------------------------------------------------------------------------------------#
#             Speect Engine SWIG Python source files list                            #
#------------------------------------------------------------------------------------#

# generate a source file list, pointing to the absolute path of the sources so
# that we can use the CMake variable SPCT_SWIG_PYTHON_SRC_FILES when the file
# is included in other projects

configure_file(${CMAKE_SOURCE_DIR}/engine/swig/python/config/engineSWIGpython.cmake.in 
  ${CMAKE_BINARY_DIR}/engine/swig/python/cmake/engineSWIGpython.cmake)


#------------------------------------------------------------------------------------#
#         Create a list of dependencies of the SWIG Python interface                 #
#------------------------------------------------------------------------------------#

# provides the source list, SPCT_SWIG_COMMON_SRC_FILES
include(${CMAKE_BINARY_DIR}/engine/swig/common/cmake/engineSWIGcommon.cmake)

# provides the source list, SPCT_SWIG_PYTHON_SRC_FILES
include(${CMAKE_BINARY_DIR}/engine/swig/python/cmake/engineSWIGpython.cmake)

list(APPEND SPCT_SWIG_PYTHON_DEPS
  ${SPCT_SWIG_COMMON_SRC_FILES} 
  ${SPCT_SWIG_PYTHON_SRC_FILES})


#------------------------------------------------------------------------------------#
#                       Find/include required packages                               #
#------------------------------------------------------------------------------------#

###################################### SWIG ##########################################

include(${SWIG_USE_FILE})  # UseSWIG.cmake, provides swig functions


#################################### Python ##########################################

# has speect_include_python_directories macro
include(${CMAKE_SOURCE_DIR}/swig/python/cmake/swigPythonFunctions.cmake)

find_package(PythonLibs REQUIRED) # Note, CMake does not look for Python 3
mark_as_advanced(FORCE PYVERSIONS_EXE)

# add Python to includes
speect_include_python_directories()


#################################### Speect ##########################################

include_directories(${SPCT_INCLUDE_DIRS})

#################################### SPyObjectModule #################################

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/python_object)


#------------------------------------------------------------------------------------#
#                              Generate speect.i                                     #
#------------------------------------------------------------------------------------#

configure_file(${CMAKE_SOURCE_DIR}/engine/swig/python/config/speect.i.in 
  ${CMAKE_BINARY_DIR}/engine/swig/python/speect.i) 


#------------------------------------------------------------------------------------#
#                                  C flags                                           #
#------------------------------------------------------------------------------------#

# clear C flags, we don't want the same stuff as Speect
set(CMAKE_C_FLAGS "")


#------------------------------------------------------------------------------------#
#                                speect.i properties                                 #
#------------------------------------------------------------------------------------#

list(APPEND SPCT_SWIG_FLAGS -Wall -Werror)

# add flags for Python Speect Engine wrapper generation
set_source_files_properties(${CMAKE_BINARY_DIR}/engine/swig/python/speect.i 
  PROPERTIES 
  SWIG_FLAGS "${SPCT_SWIG_FLAGS}"
  GENERATED TRUE
)


#------------------------------------------------------------------------------------#
#                              speectPYTHON_wrap.c flags                             #
#------------------------------------------------------------------------------------#

# add flags for Python Speect Engine wrapper compilation
set_source_files_properties(${CMAKE_BINARY_DIR}/engine/swig/python/speectPYTHON_wrap.c 
  PROPERTIES 
  COMPILE_FLAGS ${SWIG_C_COMPILE_FLAGS}
  GENERATED TRUE
)


#------------------------------------------------------------------------------------#
#                                 CMAKE_SWIG_OUTDIR                                  #
#------------------------------------------------------------------------------------#

set(CMAKE_SWIG_OUTDIR ${CMAKE_BINARY_DIR}/swig/python/speect)


#------------------------------------------------------------------------------------#
#                           Speect Engine Python wrapper                             #
#------------------------------------------------------------------------------------#

# set dependencies
set(SWIG_MODULE_speect_EXTRA_DEPS "${SPCT_SWIG_PYTHON_DEPS}")

# create the Python Speect Engine wrapper (C and Python files)
swig_add_module(speect python ${CMAKE_BINARY_DIR}/engine/swig/python/speect.i)

# link with Python and Speect Engine
swig_link_libraries(speect 
  ${PYTHON_LIBRARIES} 
  ${SPCT_LIBRARIES_TARGET} 
  ${SPCT_PYTHON_NATIVE_LIBRARY_TARGET})

# set the output directory
set_target_properties(${SWIG_MODULE_speect_REAL_NAME}
  PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/swig/python/speect/lib
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/swig/python/speect/lib
)
