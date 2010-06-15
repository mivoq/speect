######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : June 2010                                                              ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for STrackFloat Python interface                                       ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


#------------------------------------------------------------------------------------#
#                          Set the interface variables                               #
#------------------------------------------------------------------------------------#

# SWIG includes
set(swig_includes "typemaps.i;exception.i")

# C header files (speect.h is automatically included)
set(swig_c_includes "track_float.h")

# Load the plug-in on initialization ? 
set(swig_load_plugin 1)

# Include Python docstrings
set(swig_python_docstrings "track_float_docs.c")

# Include Python typemaps
set(swig_python_typemaps "track_float_py_typemap.i")

# Include SWIG common interface definition.
set(swig_common "track_float_common.c")

# Include SWIG Python interface definition.
set(swig_python)


#------------------------------------------------------------------------------------#
#                          Create the interface file                                 #
#------------------------------------------------------------------------------------#

speect_plugin_swig_python_interface()


#------------------------------------------------------------------------------------#
#                        Generate the Python wrapper                                 #
#------------------------------------------------------------------------------------#

speect_plugin_swig_python_wrapper()