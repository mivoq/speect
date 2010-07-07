######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 7 June 2010                                                            ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Speect build types (compiler build flags)                                        ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


#------------------------------------------------------------------------------------#
#        Strict warnings options (see spctOptions.cmake::Developers options)         #
#------------------------------------------------------------------------------------#

if(STRICT_WARN)
  
  # Strict warnings options are ON 
  if(SPCT_GCC)
    set(WFLAGS "-W -Wall -Werror -ansi")
    set(WFLAGS_C_ONLY "-Wmissing-declarations -Wstrict-prototypes")
  endif(SPCT_GCC)
  if(SPCT_MSVC)
    set(WFLAGS "/W4 -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE")
  endif(SPCT_MSVC)

else(STRICT_WARN)

  # Strict warnings options are OFF 
  if(SPCT_GCC)
    set(WFLAGS "-W -Wall -ansi")
  endif(SPCT_GCC)
  if(SPCT_MSVC)
    set(WFLAGS "/W3 -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE")
  endif(SPCT_MSVC)

endif(STRICT_WARN)


#------------------------------------------------------------------------------------#
#                                WIN32 compatibility                                 #
#------------------------------------------------------------------------------------#

if(SPCT_WIN32 AND SPCT_GCC)

    # Helps to ensure the Windows port remains compatible with MSVC.
    set(WFLAGS_C_ONLY "${WFLAGS_C_ONLY} -Wdeclaration-after-statement")
endif(SPCT_WIN32 AND SPCT_GCC)


#------------------------------------------------------------------------------------#
#Warn about declarations after statements (see spctOptions.cmake::Developers options)#
#------------------------------------------------------------------------------------#

if(WARN_DECL_AFTER_STMT)
  if(SPCT_GCC)

    # This is useful in the Windows port to check compatibility with MSVC.
    set(WFLAGS_C_ONLY "${WFLAGS_C_ONLY} -Wdeclaration-after-statement")
  endif(SPCT_GCC)
endif(WARN_DECL_AFTER_STMT)


#------------------------------------------------------------------------------------#
#                            C Flags: Standard build                                 #
#------------------------------------------------------------------------------------#

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WFLAGS}")


#------------------------------------------------------------------------------------#
#                            C Flags: Debugging                                      #
#------------------------------------------------------------------------------------#

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSPCT_DEBUGMODE=1")


#------------------------------------------------------------------------------------#
#                            C Flags: Profiling                                      #
#------------------------------------------------------------------------------------#

list(APPEND CMAKE_BUILD_TYPES Profile)
if(SPCT_GCC)
    set(CMAKE_C_FLAGS_PROFILE "-pg"
        CACHE STRING "profiling flags")
    set(CMAKE_EXE_LINKER_FLAGS_PROFILE "-pg"
        CACHE STRING "profiling flags")
    mark_as_advanced(
        CMAKE_C_FLAGS_PROFILE
        CMAKE_EXE_LINKER_FLAGS_PROFILE
        )
endif(SPCT_GCC)


#------------------------------------------------------------------------------------#
#                           SWIG C compilation flags                                 #
#------------------------------------------------------------------------------------#

if(SPCT_GCC)
  set(SWIG_C_COMPILE_FLAGS "-fno-strict-aliasing -fwrapv -Wstrict-prototypes -Wall -DSWIG_TYPE_TABLE=speect")
elseif(SPCT_MSVC)
  set(SWIG_C_COMPILE_FLAGS "-DSWIG_TYPE_TABLE=speect")
endif(SPCT_GCC)