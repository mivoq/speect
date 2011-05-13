######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : June 2010                                                              ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for SSyllabification Python interface                                  ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


#------------------------------------------------------------------------------------#
#                          Set the interface variables                               #
#------------------------------------------------------------------------------------#

# SWIG includes
set(swig_includes "typemaps.i;exception.i")

# C header files (speect.h is automatically included)
set(swig_c_includes "syllabification.h")

# Load the plug-in on initialization ? 
set(swig_load_plugin 1)

# Include Python docstrings
set(swig_python_docstrings "syllabification_docs.c")

# Include SWIG common interface definition.
set(swig_common "syllabification_common.c")

# Include SWIG Python interface definition.
set(swig_python "syllabification_python.c")


#------------------------------------------------------------------------------------#
#                          Create the interface file                                 #
#------------------------------------------------------------------------------------#

speect_plugin_swig_python_interface()


#------------------------------------------------------------------------------------#
#                        Generate the Python wrapper                                 #
#------------------------------------------------------------------------------------#

speect_plugin_swig_python_wrapper()