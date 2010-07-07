######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : December 2009                                                          ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for Speect Wrappers                                                    ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#------------------------------------------------------------------------------------#
#                   Find SWIG and mark as advanced in CMakecache                     #
#------------------------------------------------------------------------------------#

if(NOT NO_SWIG)
  find_package(SWIG REQUIRED)
  mark_as_advanced(SWIG_DIR)
  mark_as_advanced(SWIG_EXECUTABLE)
  mark_as_advanced(SWIG_VERSION)
endif(NOT NO_SWIG)


#------------------------------------------------------------------------------------#
#                   Speect Engine SWIG common wrappers                               #
#------------------------------------------------------------------------------------#

# Provides SPCT_SWIG_COMMON_SRC_FILES, a list of sources.
add_subdirectory(common)


#------------------------------------------------------------------------------------#
#                   Include wrappers for different languages                         #
#------------------------------------------------------------------------------------#

if(WANT_PYTHON_WRAPPER)
  add_subdirectory(python)
endif(WANT_PYTHON_WRAPPER)
