######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : June 2010                                                              ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for Speect<->Python native functions/classes                           ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#------------------------------------------------------------------------------------#
#                                Project name                                        #
#------------------------------------------------------------------------------------#

message(STATUS "cmake for project \"Speect<->Python native functions/classes\"")
project(speect_swig_python_native C)


#------------------------------------------------------------------------------------#
#                        Generate SWIG runtime header                                #
#------------------------------------------------------------------------------------#

find_package(SWIG REQUIRED)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/swigpyrun.h 
  COMMAND ${SWIG_EXECUTABLE} -python -external-runtime 
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating SWIG run-time functions header"
  VERBATIM
  )


#------------------------------------------------------------------------------------#
#                         Add Python libraries and includes                          #
#------------------------------------------------------------------------------------#

# find Python 
find_package(PythonLibs REQUIRED) # Note, currently PythonLibs does not look for Python 3

# add Python to includes
include_directories(${PYTHON_INCLUDE_PATH})

# include Speect Engine
include_directories(${SPCT_INCLUDE_DIRS})

# Speect<->Python native includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})


#------------------------------------------------------------------------------------#
#                                  C flags                                           #
#------------------------------------------------------------------------------------#

# clear C flags, we don't want the same stuff as Speect
set(CMAKE_C_FLAGS "")


#------------------------------------------------------------------------------------#
#                              Source files                                          #
#------------------------------------------------------------------------------------#

list(APPEND SPCT_SWIG_PYTHON_NATIVE_SOURCES 
  "${CMAKE_CURRENT_SOURCE_DIR}/py_object.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/py_funcs.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/py_native.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/py_list.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/py_list_itr.c"
  "${CMAKE_CURRENT_BINARY_DIR}/swigpyrun.h"
  )

add_library(SPCT_SWIG_PYTHON_NATIVE_LIBRARY ${SPCT_SWIG_PYTHON_NATIVE_SOURCES})

set_target_properties(SPCT_SWIG_PYTHON_NATIVE_LIBRARY
  PROPERTIES
  VERSION "${SPCT_VERSION_MAJOR}.${SPCT_VERSION_MINOR}.${SPCT_VERSION_PATCH}"
  SOVERSION "${SPCT_VERSION_MAJOR}"
  COMPILE_FLAGS "-fno-strict-aliasing -fwrapv -Wstrict-prototypes -DSPCT_SRC -DSPCT_LIBRARIES_EXPORTS -DSWIG_TYPE_TABLE=speect ${static_flag}"
  LINK_FLAGS "${SPCT_LIBRARY_LINK_FLAGS}"
  OUTPUT_NAME ${SPCT_SWIG_PYTHON_NATIVE_LIBRARY}
  LIBRARY_OUTPUT_DIRECTORY ${SPCT_LIBRARY_OUTPUT_PATH}
  RUNTIME_OUTPUT_DIRECTORY ${SPCT_LIBRARY_OUTPUT_PATH}
#  DEBUG_POSTFIX "-debug"
#  PROFILE_POSTFIX "-profile"
  )

target_link_libraries(SPCT_SWIG_PYTHON_NATIVE_LIBRARY 
  ${SPCT_LIBRARIES_TARGET} 
  ${PYTHON_LIBRARIES})
