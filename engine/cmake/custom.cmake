######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 9 May 2009                                                             ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMake custom functions                                                           ##
## Adapted from allegro 4.9.9.1                                                     ##
##                                                                                  ##
######################################################################################

# 
# function sanitize_cmake_link_flags
#
function(sanitize_cmake_link_flags ...)
   SET(return)
   foreach(lib ${ARGV})
      # Watch out for -framework options (OS X)
      IF (NOT lib MATCHES "-framework.*" AND NOT lib MATCHES ".*framework")
         # Remove absolute path.
         string(REGEX REPLACE "/.*/(.*)" "\\1" lib ${lib})
   
         # Remove .a/.so/.dylib.
         string(REGEX REPLACE "lib(.*)\\.a" "\\1" lib ${lib})
         string(REGEX REPLACE "lib(.*)\\.so" "\\1" lib ${lib})
         string(REGEX REPLACE "lib(.*)\\.dylib" "\\1" lib ${lib})
   
         # Remove -l prefix if it's there already.
         string(REGEX REPLACE "-l(.*)" "\\1" lib ${lib})
         
         # Make sure we don't include our own libraries.
         # FIXME: Use a global list instead of a very unstable regexp.
         IF(NOT lib MATCHES "spct_.*" AND NOT lib STREQUAL "spct") 
            set(return "${return} -l${lib}")
         ENDIF()
      ENDIF(NOT lib MATCHES "-framework.*" AND NOT lib MATCHES ".*framework")  
   endforeach(lib)
   set(return ${return} PARENT_SCOPE)
endfunction(sanitize_cmake_link_flags)


#
# function add_our_library
# 
function(add_our_library target sources extra_flags link_with)
  if(NOT BUILD_SHARED_LIBS)
    set(target ${target}-static)
    set(static_flag "-DSPCT_STATICLINK")
  endif(NOT BUILD_SHARED_LIBS)
  
  # Suppress errors about _mangled_main_address being undefined
  # on Mac OS X.
  if(APPLE)
    set(LIBRARY_LINK_FLAGS "-flat_namespace -undefined suppress")
  endif(APPLE)

  # BUILD_SHARED_LIBS controls whether this is a shared or static library.
  add_library(${target} ${sources})

#  sanitize_cmake_link_flags(${link_with})
  
  set_target_properties(${target}
    PROPERTIES
    VERSION "${SPCT_VERSION_MAJOR}.${SPCT_VERSION_MINOR}.${SPCT_VERSION_PATCH}"
    SOVERSION "${SPCT_VERSION_MAJOR}"
    COMPILE_FLAGS "${extra_flags} ${static_flag}"
    LINK_FLAGS "${LIBRARY_LINK_FLAGS}"
    OUTPUT_NAME ${target}
    DEBUG_POSTFIX "-debug"
    PROFILE_POSTFIX "-profile"
    )

  # Specify a list of libraries to be linked into the specified target.
  # Library dependencies are transitive by default.  Any target which links
  # with this target will therefore pull in these dependencies automatically.
  target_link_libraries(${target} ${link_with})

  install(TARGETS ${target}
          DESTINATION "lib${LIB_SUFFIX}"
	  EXPORT myproj 
	  )
endfunction(add_our_library)


#
# function add_our_executable
#
# Arguments after nm should be source files or libraries.  Source files must
# end with .c or .cpp.  If no source file was explicitly specified, we assume
# an implied C source file.
# 
# Free variable: EXECUTABLE_TYPE
#
function(add_our_executable nm)
  set(srcs)
  set(libs)
  set(regex "[.](c|cpp)$")
  foreach(arg ${ARGN})
    if("${arg}" MATCHES "${regex}")
      list(APPEND srcs ${arg})
    else("${arg}" MATCHES "${regex}")
      list(APPEND libs ${arg})
    endif("${arg}" MATCHES "${regex}")
  endforeach(arg ${ARGN})

  if(NOT srcs)
    set(srcs "${nm}.c")
  endif(NOT srcs)

  add_executable(${nm} ${EXECUTABLE_TYPE} ${srcs})
  target_link_libraries(${nm} ${libs})
  if(NOT BUILD_SHARED_LIBS)
    set_target_properties(${nm} PROPERTIES COMPILE_FLAGS "-DSPCT_STATICLINK")
  endif(NOT BUILD_SHARED_LIBS)
endfunction(add_our_executable)


#
# function copy_data_dir_to_build
#
# Recreate data directory for out-of-source builds.
# Note: a symlink is unsafe as make clean will delete the contents
# of the pointed-to directory.
#
# Files are only copied if they don't are inside a .svn folder so we
# won't end up with read-only .svn folders in the build folder.
function(copy_data_dir_to_build target name)
  if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    return()
  endif()

  file(GLOB_RECURSE allfiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/${name}/*)

  set(files)
  # Filter out files inside .svn folders.
  foreach(file ${allfiles})
    string(REGEX MATCH .*\\.svn.* is_svn ${file})
    if("${is_svn}" STREQUAL "")
      list(APPEND files ${file})
    endif()
  endforeach(file)
  
  add_custom_target(${target} ALL DEPENDS ${files})

  foreach(file ${files})
    add_custom_command(
      OUTPUT ${file}
      COMMAND "${CMAKE_COMMAND}" -E copy
      "${CMAKE_CURRENT_SOURCE_DIR}/${file}" ${file}
      )
  endforeach(file)
endfunction(copy_data_dir_to_build)

