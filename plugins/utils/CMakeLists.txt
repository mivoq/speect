
#------------------------------------------------------------------------------------#
#                        Plug-in directories                                         #
#------------------------------------------------------------------------------------#
add_subdirectory(arrays)
add_subdirectory(matrices)
add_subdirectory(tracks)
add_subdirectory(tokenizer)
add_subdirectory(viterbi)
add_subdirectory(cost_function)

if(SPEECT_WRAP_PYTHON)
  add_subdirectory(python_object)
endif(SPEECT_WRAP_PYTHON)
