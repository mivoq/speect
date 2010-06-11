######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : June 2010                                                              ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Custom CMake Speect functions                                                    ##
##                                                                                  ##
##                                                                                  ##
######################################################################################
 
#------------------------------------------------------------------------------------#
#                    Add an example to be compiled as an executable                  #
#------------------------------------------------------------------------------------#
#
# speect_example(name [link_libs = NULL])
#
# Add an example that will be compiled as an executable.
#
# :param name: The name of the c file (excluding ".c"), 
#              will also be the name of the executable.
# :type name: string
# :param link_libs: Libraries that the executable must link with (excluding Speect)
# :type link_libs: string
#
# From allegro cmake functions
#
function(speect_example name)
  set(srcs)
  set(libs)
  set(regex "[.](c)$")
  foreach(arg ${ARGN})
    if("${arg}" MATCHES "${regex}")
      list(APPEND srcs ${arg})
    else("${arg}" MATCHES "${regex}")
      list(APPEND libs ${arg})
    endif("${arg}" MATCHES "${regex}")
  endforeach(arg ${ARGN})

  # add Speect Engine lib
  list(APPEND libs ${SPCT_LIBRARIES_TARGET})

  if(NOT srcs)
    set(srcs "${name}.c")
  endif(NOT srcs)

  add_executable(${name} ${srcs})
  target_link_libraries(${name} ${libs})

endfunction(speect_example)

