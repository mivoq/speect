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
project(speect_python_native C)


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

# has speect_include_python_directories and speect_find_python macros
include(${CMAKE_SPEECT_SOURCE_DIR}/swig/python/cmake/swigPythonFunctions.cmake)

# find Python 
speect_find_python()

# add Python to includes
speect_include_python_directories()


#------------------------------------------------------------------------------------#
#                             Add Speect includes                                    #
#------------------------------------------------------------------------------------#

# include Speect Engine
include_directories(${SPCT_INCLUDE_DIRS})


#------------------------------------------------------------------------------------#
#                    Speect<->Python native functions/classes includes               #
#------------------------------------------------------------------------------------#

list(APPEND SPCT_PYTHON_NATIVE_INCLUDE_DIRS_TMP
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR})

include_directories(${SPCT_PYTHON_NATIVE_INCLUDE_DIRS_TMP})

# set SPCT_PYTHON_NATIVE_INCLUDE_DIRS_TMP as SPCT_PYTHON_NATIVE_INCLUDE_DIRS in cache
set(SPCT_PYTHON_NATIVE_INCLUDE_DIRS ${SPCT_PYTHON_NATIVE_INCLUDE_DIRS_TMP} 
  CACHE INTERNAL "Speect Python native include directories" FORCE)


#------------------------------------------------------------------------------------#
#                                  C flags                                           #
#------------------------------------------------------------------------------------#

# clear C flags, we don't want the same stuff as Speect
set(CMAKE_C_FLAGS "")


#------------------------------------------------------------------------------------#
#                              Header files                                          #
#------------------------------------------------------------------------------------#

list(APPEND SPCT_PYTHON_NATIVE_INCLUDES 
  "${CMAKE_CURRENT_SOURCE_DIR}/py_object.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/py_funcs.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/py_native.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/py_list.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/py_list_itr.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/py_map.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/py_map_itr.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/py_logger.h"
  )


#------------------------------------------------------------------------------------#
#                              Source files                                          #
#------------------------------------------------------------------------------------#

list(APPEND SPCT_PYTHON_NATIVE_SOURCES 
  "${CMAKE_CURRENT_SOURCE_DIR}/py_object.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/py_funcs.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/py_native.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/py_list.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/py_list_itr.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/py_map.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/py_map_itr.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/py_logger.c"
  "${CMAKE_CURRENT_BINARY_DIR}/swigpyrun.h"
  )


#------------------------------------------------------------------------------------#
#                               Compile flags                                        #
#------------------------------------------------------------------------------------#

if(SPCT_GCC)
  set(SWIG_C_COMPILE_FLAGS "-fno-strict-aliasing -fwrapv -Wstrict-prototypes -DSPCT_SRC -DSPCT_LIBRARIES_EXPORTS -DSWIG_TYPE_TABLE=speect ${static_flag}")
elseif(SPCT_MSVC)
  set(SWIG_C_COMPILE_FLAGS "-DSPCT_SRC -DSPCT_LIBRARIES_EXPORTS -DSWIG_TYPE_TABLE=speect ${static_flag}")
endif(SPCT_GCC)


#------------------------------------------------------------------------------------#
#                          Library definition                                        #
#------------------------------------------------------------------------------------#

set(SPCT_PYTHON_NATIVE_LIBRARY spct-py-native)

add_library(SPCT_PYTHON_NATIVE_LIBRARY ${SPCT_LIB_TYPE} ${SPCT_PYTHON_NATIVE_SOURCES})

set_target_properties(SPCT_PYTHON_NATIVE_LIBRARY
  PROPERTIES
#  VERSION "${SPCT_VERSION_MAJOR}.${SPCT_VERSION_MINOR}.${SPCT_VERSION_PATCH}"
  SOVERSION "${SPCT_VERSION_MAJOR}.${SPCT_VERSION_MINOR}"
  COMPILE_FLAGS "${SWIG_C_COMPILE_FLAGS}"
  LINK_FLAGS "${SPCT_LIBRARY_LINK_FLAGS}"
  OUTPUT_NAME ${SPCT_PYTHON_NATIVE_LIBRARY}
  LIBRARY_OUTPUT_DIRECTORY ${SPCT_LIBRARY_OUTPUT_PATH}
  RUNTIME_OUTPUT_DIRECTORY ${SPCT_LIBRARY_OUTPUT_PATH}
#  DEBUG_POSTFIX "-debug"
#  PROFILE_POSTFIX "-profile"
  )

target_link_libraries(SPCT_PYTHON_NATIVE_LIBRARY 
  ${SPCT_LIBRARIES_TARGET} 
  ${PYTHON_LIBRARIES})


#------------------------------------------------------------------------------------#
#                           link target for other libraries                          #
#------------------------------------------------------------------------------------#

# plug-ins and examples should link with SPCT_PYTHON_NATIVE_LIBRARY_TARGET.
set(SPCT_PYTHON_NATIVE_LIBRARY_TARGET SPCT_PYTHON_NATIVE_LIBRARY
  CACHE INTERNAL "Speect Python native library target for internal plug-ins" FORCE)


#------------------------------------------------------------------------------------#
#                               examples/tests                                       #
#------------------------------------------------------------------------------------#

if(WANT_EXAMPLES)
  add_subdirectory(examples)
endif(WANT_EXAMPLES)


#------------------------------------------------------------------------------------#
#                                Installation                                        #
#------------------------------------------------------------------------------------#

###################################### UNIX #########################################

if(SPCT_UNIX)

  # Install header files.  
  file(GLOB tmpfiles "${CMAKE_CURRENT_SOURCE_DIR}/py*.h")
  install(FILES ${tmpfiles}
    DESTINATION include/speect/engine/python/native
    )

  file(GLOB tmpfiles "${CMAKE_CURRENT_BINARY_DIR}/py*.h")
  install(FILES ${tmpfiles}
    DESTINATION include/speect/engine/python/native
    )
  
  # Install library
  install(TARGETS SPCT_PYTHON_NATIVE_LIBRARY 
    DESTINATION "lib${LIB_SUFFIX}/"
    EXPORT spctPythonNativeConfig 
    )

  # export CMake config
#  install(EXPORT spctPythonNativeConfig DESTINATION include/speect/engine/python)
endif(SPCT_UNIX)
