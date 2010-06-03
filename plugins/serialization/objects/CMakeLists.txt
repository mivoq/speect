
#------------------------------------------------------------------------------------#
#                        Plug-in directories                                         #
#------------------------------------------------------------------------------------#
add_subdirectory(arrays)
add_subdirectory(matrices)
add_subdirectory(primitives)
add_subdirectory(tracks)

if(SPEECT_WRAP_PYTHON)
  add_subdirectory(python_object)
endif(SPEECT_WRAP_PYTHON)
