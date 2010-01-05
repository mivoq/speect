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
# Warnings.
#
if(STRICT_WARN)
    if(COMPILER_GCC)
        set(WFLAGS "-W -Wall -Werror -ansi")
        set(WFLAGS_C_ONLY "-Wmissing-declarations -Wstrict-prototypes")
    endif(COMPILER_GCC)
    if(COMPILER_MSVC)
        set(WFLAGS "/W4 -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE")
    endif(COMPILER_MSVC)
else(STRICT_WARN)
    if(COMPILER_GCC)
      set(WFLAGS "-W -Wall -ansi")
    endif(COMPILER_GCC)
    if(COMPILER_MSVC)
        set(WFLAGS "/W3 -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE")
    endif(COMPILER_MSVC)
endif(STRICT_WARN)


if(WIN32 AND COMPILER_GCC)
    # Helps to ensure the Windows port remains compatible with MSVC.
    set(WFLAGS_C_ONLY "${WFLAGS_C_ONLY} -Wdeclaration-after-statement")
endif(WIN32 AND COMPILER_GCC)


if(WARN_DECL_AFTER_STMT)
    if(COMPILER_GCC)
        # This is useful in the Windows port to check compatibility with MSVC.
        set(WFLAGS_C_ONLY "${WFLAGS_C_ONLY} -Wdeclaration-after-statement")
    endif(COMPILER_GCC)
endif(WARN_DECL_AFTER_STMT)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WFLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WFLAGS}")


#
# Debugging.
#
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSPCT_DEBUGMODE=1")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSPCT_DEBUGMODE=1")


#
# Profiling.
#
list(APPEND CMAKE_BUILD_TYPES Profile)
if(COMPILER_GCC)
    set(CMAKE_C_FLAGS_PROFILE "-pg"
        CACHE STRING "profiling flags")
    set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_C_FLAGS_PROFILE}"
        CACHE STRING "profiling flags")
    set(CMAKE_EXE_LINKER_FLAGS_PROFILE "-pg"
        CACHE STRING "profiling flags")
    mark_as_advanced(
        CMAKE_C_FLAGS_PROFILE
        CMAKE_CXX_FLAGS_PROFILE
        CMAKE_EXE_LINKER_FLAGS_PROFILE
        )
endif(COMPILER_GCC)


#
# Errors and debugging
#

# Fatal errors
if(ERROR_ABORT_FATAL)
  set(SPCT_ERROR_ABORT_FATAL 1)
endif(ERROR_ABORT_FATAL)

# Log error file name and line number
if(ERROR_VERBOSE)
  set(SPCT_ERROR_VERBOSE 1)
endif(ERROR_VERBOSE)

# Safe cast
if(SAFE_CAST)
  set(SPCT_DO_SAFE_CAST 1)
endif(SAFE_CAST)

