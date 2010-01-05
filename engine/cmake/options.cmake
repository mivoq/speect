######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 09 May 2009                                                            ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMake build options                                                              ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#
# Shared libraries
#
option(SHARED "Build shared libraries" on)
set(BUILD_SHARED_LIBS ${SHARED})  # actual CMake variable


#
# On some 64-bit platforms, libraries should be installed into `lib64'
# instead of `lib'.  Set this to 64 to do that.
#
set(LIB_SUFFIX "" CACHE STRING "Suffix for 'lib' directories, e.g. '64'")


#
# Multi threaded support
#
option(WANT_THREADS "Enable multi threaded support" off)


#
# Documentation.
#
option(WANT_DOCS "Generate documentation" on)
option(WANT_DOCS_HTML "Generate HTML documentation" on)
option(WANT_DOCS_PDF "Generate PDF document (requires pdflatex)" off)


#
# For developers.
#
option(STRICT_WARN "Halt at warnings" off)
option(ERROR_ABORT_FATAL "Abort on fatal errors" off)
option(ERROR_VERBOSE "Include funtion names, file names and line numbers in error logs" on)
option(SAFE_CAST "Do safe cast of objects, slower than non-safe cast" on)
option(WARN_DECL_AFTER_STMT "Warn about declarations after statements (GCC)" off)


#
# Speect Logging
#
set(ERROR_LOG_FILE "${CMAKE_BINARY_DIR}/spct_error.log" CACHE STRING "Error logging file")
set(DEBUG_LOG_FILE "${CMAKE_BINARY_DIR}/spct_debug.log" CACHE STRING "Debug logging file")


#
# Speect plug-in path
#
set(PLUGIN_PATH "${SPCT_PLUGIN_PATH}/lib" CACHE STRING "Plug-in path")


#
# Speect initialization file 
#
set(INI_FILE_PATH "${CMAKE_BINARY_DIR}" CACHE STRING "Speect initialization file path (speect.ini)")


#
# Tests/Examples.
#
option(WANT_TESTS "Include the test suite in the compilation" off)
option(WANT_EXAMPLES "Include the example suite in the compilation" off)


#
# SWIG interfaces
#
option(SPEECT_WRAP_PYTHON "Create Speect Engine Python wrappers" on)

