######################################################################################
##                                                                                  ##
## AUTHOR  :                                                                        ##
## DATE    :                                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for SYour Python interface                                             ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

# see TODO: sections

#------------------------------------------------------------------------------------#
#                          Set the interface variables                               #
#------------------------------------------------------------------------------------#

# SWIG includes
set(swig_includes "typemaps.i;exception.i")

# C header files (speect.h is automatically included)
set(swig_c_includes "source.h") # TODO: add includes

# Load the plug-in on initialization ? 
set(swig_load_plugin 1)

# Include Python docstrings
set(swig_python_docstrings "source_docs.c")  # TODO: add docs

# Include Python typemaps
set(swig_python_typemaps)

# Include SWIG common interface definition.
set(swig_common "source_common.c")  # TODO: add c functions and interface

# Include SWIG Python interface definition.
set(swig_python "source_python.c")  # TODO: add pure Python functions and interface


#------------------------------------------------------------------------------------#
#                          Create the interface file                                 #
#------------------------------------------------------------------------------------#

speect_plugin_swig_python_interface()


#------------------------------------------------------------------------------------#
#                        Generate the Python wrapper                                 #
#------------------------------------------------------------------------------------#

speect_plugin_swig_python_wrapper()