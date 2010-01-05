######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 5 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMake custom functions for Speect plug-ins                                       ##
## Adapted from allegro 4.9.9.1                                                     ##
##                                                                                  ##
######################################################################################


#
# function add_our_library
# 
function(add_our_library target name_suffix sources extra_flags link_with)
  # Construct the output name.
  set(output_name ${target})
  append_lib_type_suffix(output_name)
  set(output_name ${output_name}${name_suffix})
  
   
  # Suppress errors about _mangled_main_address being undefined
  # on Mac OS X.
  if(APPLE)
    set(LIBRARY_LINK_FLAGS "-flat_namespace -undefined suppress")
  endif(APPLE)

  add_library(${target} ${sources})
 
  set_target_properties(${target}
    PROPERTIES
    COMPILE_FLAGS "${extra_flags} ${static_flag}"
    LINK_FLAGS "${LIBRARY_LINK_FLAGS}"
    OUTPUT_NAME ${output_name}
    )

  # Specify a list of libraries to be linked into the specified target.
  # Library dependencies are transitive by default.  Any target which links
  # with this target will therefore pull in these dependencies automatically.
  target_link_libraries(${target} ${link_with})

  install(TARGETS ${target}
          DESTINATION "lib${LIB_SUFFIX}"
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

