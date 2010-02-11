######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 5 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Template CMake build options for Speect plug-ins                                 ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#
# Shared libraries
#
set(BUILD_SHARED_LIBS 1)  # actual CMake variable, not really an option


#
# On some 64-bit platforms, libraries should be installed into `lib64'
# instead of `lib'.  Set this to 64 to do that.
#
set(LIB_SUFFIX "" CACHE STRING "Suffix for 'lib' directories, e.g. '64'")



#
# For developers.
#
option(STRICT_WARN "Halt at warnings" off)
option(WARN_DECL_AFTER_STMT "Warn about declarations after statements (GCC)" off)


#
# Speect path
#
set(SPEECT_DIR "/home/aby/Development/speect/engine" CACHE STRING "Path to Speect Engine")


#
# Examples.
#
option(WANT_EXAMPLES "Include the example suite in the compilation" off)
