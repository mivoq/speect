######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : June 2010                                                              ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMake custom functions/macros for Speect plug-ins:                               ##
##                                                                                  ##
##   speect_plugin_definition       (Define a plug-in)                              ## 
##   speect_plugin_sources          (Add plug-in source files)                      ##
##   speect_plugin_headers          (Add plug-in header files)                      ##
##   speect_plugin_include_dirs     (Add plug-in include directories)               ##
##   speect_plugin_create           (Create the plug-in shared object)              ##
##   speect_plugin_configure_info   (Configure a plug-in header with information)   ##
##   speect_include_plugin          (Include another plug-in in the build)          ##
##                                                                                  ##
## See the documentation at each function/macro below.                              ##
##                                                                                  ##
######################################################################################


#------------------------------------------------------------------------------------#
#                             Define a plug-in                                       #
#------------------------------------------------------------------------------------#
#
# speect_plugin_definition(name classname version_major version_minor version_patch)
#
# Define a plug-in. The plug-in name is lowercased and the project name is 
# set to "speect_lowercase_name_plugin" with the C language. The plug-in library's
# name will be "lowercase_name.spi" with links to:
#                  "lowercase_name.spi.version_major"
#                  "lowercase_name.spi.version_major.version_minor.version_patch"
#
# The class name is used in the documentation strings 
# (for example "SArrayFloat plug-in initialization")
# 

# The plug-in's "cmake" directory will be added to CMAKE_MODULE_PATH
#
# :param name: The name of the plug-in.
# :type name: string
# :param name: The name of the class that the plug-in implements
# :type name: string
# :param version_major: The major version number of the plug-in.
# :type version_major: int
# :param version_minor: The minor version number of the plug-in.
# :type version_minor: int
# :param version_patch: The patch number of the plug-in.
# :type version_patch: int
#
# For example: speect_plugin_definition(Viterbi "SViterbi" 0 9 5)
# will create a plug-in viterbi.spi that points to viterbi.spi.0 which in turn
# points to viterbi.spi.0.9.5
#

macro(speect_plugin_definition name classname version_major version_minor version_patch)
  message(STATUS "cmake for project \"${name}\"")

  string(TOLOWER "${name}" plugin_lowercase_name)

  # used in plugin_info.h.in if required
  string(TOUPPER "${name}" plugin_uppercase_name)

  # look if a plug-in with the same name is in the list of
  # plug-in names
  list(FIND SPCT_PLUGIN_NAMES_LIST "${plugin_lowercase_name}" plugin_exists)
  if(NOT plugin_exists EQUAL -1)
    message(FATAL_ERROR "A plug-in with the given lowercase name \"${plugin_lowercase_name}\" already exists, please rename your plug-in")
  endif(NOT plugin_exists EQUAL -1)

  # OK, add plugin_lowercase_name to SPCT_PLUGIN_NAMES_LIST
  set(tmplistnames ${SPCT_PLUGIN_NAMES_LIST})
  list(APPEND tmplistnames ${plugin_lowercase_name})
  set(SPCT_PLUGIN_NAMES_LIST ${tmplistnames} CACHE INTERNAL "List of lowercase Speect plug-in names" FORCE)

  project(speect_${plugin_lowercase_name}_plugin C)
  
  # Plug-in shared object name
  set(${plugin_lowercase_name}_SONAME ${plugin_lowercase_name})
  set(${plugin_lowercase_name}_VERSION_MAJOR ${version_major})
  set(${plugin_lowercase_name}_VERSION_MINOR ${version_minor})
  set(${plugin_lowercase_name}_VERSION_PATCH ${version_patch})
  set(${plugin_lowercase_name}_VERSION "${version_major}.${version_minor}.${version_patch}")

  # class name
  set(plugin_class_name ${classname})

  # for SWIG if needed
  set(swig_classname ${plugin_class_name})

  list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
endmacro(speect_plugin_definition)


#------------------------------------------------------------------------------------#
#                          Add plug-in source files                                  #
#------------------------------------------------------------------------------------#
#
# speect_plugin_sources(source_list)
#
# Appends the given list of sources to the plug-in source list variable, lowercase_name_SRC_FILES
#
# :param source_list: A list of source files to append to lowercase_name_SRC_FILES
# :type source_list: CMake list
#

macro(speect_plugin_sources)
  foreach(arg ${ARGN})
    list(APPEND ${plugin_lowercase_name}_SRC_FILES ${arg})
  endforeach(arg ${ARGN})
endmacro(speect_plugin_sources)


#------------------------------------------------------------------------------------#
#                          Add plug-in header files                                  #
#------------------------------------------------------------------------------------#
#
# speect_plugin_headers(headers_list)
#
# Appends the given list of headers to the plug-in header list variable, lowercase_name_INCLUDE_FILES
# These headers will be installed to CMAKE_INSTALL_PREFIX/include/speect/plugins/<plugin_dir>
#
# :param headers_list: A list of header files to append to lowercase_name_INCLUDE_FILES
# :type headers_list: CMake list
#

macro(speect_plugin_headers)
  foreach(arg ${ARGN})
    list(APPEND ${plugin_lowercase_name}_INCLUDE_FILES ${arg})
  endforeach(arg ${ARGN})
endmacro(speect_plugin_headers)
    

#------------------------------------------------------------------------------------#
#                          Add plug-in include directories                           #
#------------------------------------------------------------------------------------#
#
# speect_plugin_include_dirs(directories)
#
# Appends the given list of directories to the plug-in include directories variable, 
# lowercase_name_INCLUDE_DIRS.
#
# :param directories: A list of directories to append to lowercase_name_INCLUDE_DIRS
# :type headers_list: CMake list
#

macro(speect_plugin_include_dirs)
  foreach(arg ${ARGN})
    list(APPEND ${plugin_lowercase_name}_INCLUDE_DIRS ${arg})
  endforeach(arg ${ARGN})
endmacro(speect_plugin_include_dirs)
    

#------------------------------------------------------------------------------------#
#                   Create the plug-in shared object                                 #
#------------------------------------------------------------------------------------#
#
# speect_plugin_create([link_lib_list = NULL])
#
# Create the plug-in shared object. The shared object will be linked against the 
# Speect Engine library. Build options will be as set at top level Speect CMakeLists.txt
# The Speect Engine directories will be included, as well as all headers in the plug-in's
# source (src) directory. Optional parameters, which are libraries to link with,
# can be given in a list form. Installation paths for UNIX system types will be set and
# the binary path output will be CMAKE_BINARY_DIR/plugins/lib
#
# A plug-in CMake configuration file will also be created. This configuration file can be
# included in other plug-in CMakeLists.txt to include their header files.
#
# :param link_lib_list: Extra libraries that the plug-in must be linked with.
# :type link_lib_list: CMake list
#

function(speect_plugin_create)

  # Libraries that we always need to link against on this platform.
  set(PLUGIN_PLATFORM_LIBS)

  # include Speect headers
  include_directories(${SPCT_INCLUDE_DIRS})

  # include Plug-in headers
  list(APPEND ${plugin_lowercase_name}_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src 
    ${CMAKE_CURRENT_BINARY_DIR}/src)

  # reverse the list to make sure that the plug-ins own
  # directories are included first
  list(REVERSE ${plugin_lowercase_name}_INCLUDE_DIRS)

  include_directories(${${plugin_lowercase_name}_INCLUDE_DIRS})
 
  # extra libraries to link with
  foreach(arg ${ARGN})
    list(APPEND PLUGIN_PLATFORM_LIBS ${arg})
  endforeach(arg ${ARGN})

  ###################################### UNIX #########################################
  if(SPCT_UNIX) # not MACOSX
    list(APPEND PLUGIN_PLATFORM_LIBS m) # math library
  endif(SPCT_UNIX)

  ###################################### WIN32 #########################################
  if(SPCT_WIN32)
    list(APPEND PLUGIN_PLATFORM_LIBS kernel32 user32 winmm)
  endif(SPCT_WIN32)
  
  ###################################### MACOSX ########################################
  if(SPCT_MACOSX)
    find_library(APPKIT_LIBRARY AppKit)
    find_library(IOKIT_LIBRARY IOKit)
    list(APPEND PLUGIN_PLATFORM_LIBS ${APPKIT_LIBRARY})
    list(APPEND PLUGIN_PLATFORM_LIBS ${IOKIT_LIBRARY})
    set(PLUGIN_LIBRARY_LINK_FLAGS "-flat_namespace -undefined suppress")
  endif(SPCT_MACOSX)
  
  ############################ Plugin library ##########################################
  set(${plugin_lowercase_name}_plugin ${plugin_lowercase_name}_SONAME)

  add_library(${plugin_lowercase_name}_plugin ${${plugin_lowercase_name}_SRC_FILES})

  set_target_properties(${plugin_lowercase_name}_plugin
    PROPERTIES
    VERSION "${${plugin_lowercase_name}_VERSION}"
    SOVERSION "${${plugin_lowercase_name}_VERSION_MAJOR}"
    COMPILE_FLAGS "${SPCT_LIBRARY_CFLAGS}"
    LINK_FLAGS "${SPCT_LIBRARY_LINK_FLAGS}"
    OUTPUT_NAME ${plugin_lowercase_name}
    PREFIX ""
    SUFFIX ".spi"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/lib"
    )

  # link with Speect library
  target_link_libraries(${plugin_lowercase_name}_plugin
    ${SPCT_LIBRARIES_TARGET}
    ${PLUGIN_PLATFORM_LIBS}
    )

  set(${plugin_lowercase_name}_TARGET ${plugin_lowercase_name}_plugin)


  ################################# Installation #######################################
  
  if(SPCT_UNIX)
 
    # get plugin directory structure
    string(LENGTH ${CMAKE_SOURCE_DIR}/plugins/ length_plugin_main)
    string(LENGTH ${CMAKE_CURRENT_SOURCE_DIR} length_plugin_current)
    math(EXPR length_plugin_name "${length_plugin_current} - ${length_plugin_main}")
    string(SUBSTRING ${CMAKE_CURRENT_SOURCE_DIR}  
      ${length_plugin_main} ${length_plugin_name} plugin_dir)
    
    # Install header files.  
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/"
      DESTINATION include/speect/plugins/${plugin_dir}
      FILES_MATCHING PATTERN "*.h"
      PATTERN "platform" EXCLUDE)

    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/src/"
      DESTINATION include/speect/plugins/${plugin_dir}
      FILES_MATCHING PATTERN "*.h"
      PATTERN "platform" EXCLUDE)
  
    # Install plug-in library
    install(TARGETS ${plugin_lowercase_name}_plugin
      DESTINATION "lib${LIB_SUFFIX}/speect/plugins"
      )

    # For Config file installation
    list(LENGTH ${plugin_lowercase_name}_INCLUDE_DIRS num_dirs)
    list(APPEND PLUGIN_INFO 
      ${num_dirs} 
      ${plugin_lowercase_name} 
      ${${plugin_lowercase_name}_VERSION_MAJOR}
      ${${plugin_lowercase_name}_VERSION_MINOR}
      ${${plugin_lowercase_name}_INCLUDE_DIRS}
      )

    set(tmplist ${SPCT_PLUGIN_LIST})
    list(APPEND tmplist ${PLUGIN_INFO})
    set(SPCT_PLUGIN_LIST ${tmplist} CACHE INTERNAL "Speect Engine include directories")
    

  endif(SPCT_UNIX)

  ################################# Config file ########################################

  set(tmp_name ${${plugin_lowercase_name}_INCLUDE_DIRS})
  set(tmp_version_major ${${plugin_lowercase_name}_VERSION_MAJOR})
  set(tmp_version_minor ${${plugin_lowercase_name}_VERSION_MINOR})

  list(REMOVE_DUPLICATES tmp_name)
  configure_file(${CMAKE_SOURCE_DIR}/plugins/config/pluginConf.cmake.in 
    ${CMAKE_BINARY_DIR}/plugins/cmakeconf/${plugin_lowercase_name}.cmake @ONLY)

 
endfunction(speect_plugin_create)


#------------------------------------------------------------------------------------#
#                   Configure a plug-in header with information                      #
#------------------------------------------------------------------------------------#
#
# speect_plugin_configure_info(description major_min minor_min)
#
# Configure the plug-in information header, plugin_info.h, with the provided information.
#
# :param description: A short description of the plug-in.
# :type description: string
# :param major_min: The minimum major version of the Speect Engine with which this
#                   plug-in will work.
# :type major_min: int
# :param minor_min: The minimum minor version of the Speect Engine with which this
#                   plug-in will work.
# :type minor_min: int
#

macro(speect_plugin_configure_info description major_min minor_min)

  # plug-in description
  set(plugin_description "${description}")

  # Speect Engine minimum versions
  set(plugin_speect_major_min ${major_min})
  set(plugin_speect_minor_min ${minor_min})

  # plug-in versions
  set(plugin_major ${${plugin_lowercase_name}_VERSION_MAJOR})
  set(plugin_minor ${${plugin_lowercase_name}_VERSION_MINOR})
  
  configure_file(${CMAKE_SOURCE_DIR}/plugins/config/plugin_info.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/src/plugin_info.h @ONLY)
endmacro(speect_plugin_configure_info)


#------------------------------------------------------------------------------------#
#                   Include another plug-in in the build                             #
#------------------------------------------------------------------------------------#
#
# speect_include_plugin(name major_min minor_min)
#
# Include another plug-in in the build of this one. This macro will load the plug-in's
# CMake configuration, which contains the header files to include, as well as version info.
# A fatal error will be thrown if the given major and minor version numbers are greater 
# than the requested plug-in's versions.
#
# :param name: The name of the requested plug-in to include.
# :type description: string
# :param major_min: The minimum major version of the requested plug-in with which 
#                   this plug-in will work.
# :type major_min: int
# :param minor_min: The minimum minor version of the requested plug-in with which 
#                   this plug-in will work.
# :type minor_min: int
#

macro(speect_include_plugin name major_min minor_min)

  string(TOLOWER "${name}" include_lowercase_name)
  include(${include_lowercase_name})
  
  math(EXPR min_required "${major_min}*1000 + ${minor_min}")
  math(EXPR have_version "${${include_lowercase_name}_VERSION_MAJOR}*1000 + ${${include_lowercase_name}_VERSION_MINOR}")

  if(min_required GREATER have_version)
    message(FATAL_ERROR "Requested plug-in's (${name}) version, ${${include_lowercase_name}_VERSION_MAJOR}.${${include_lowercase_name}_VERSION_MINOR}, is less than minimum required, ${major_min}.${minor_min}")
  endif(min_required GREATER have_version)

endmacro(speect_include_plugin)