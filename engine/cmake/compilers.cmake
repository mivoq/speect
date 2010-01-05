######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 09 May 2009                                                            ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMake compilers                                                                  ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

include(CheckCSourceCompiles)

set(INSTALL_PREFIX "")

#
# gcc
#
if(CMAKE_COMPILER_IS_GNUCC)
    set(COMPILER_GCC 1)
    set(SPCT_GCC 1)
endif(CMAKE_COMPILER_IS_GNUCC)


#
# Visual C++
#
if(MSVC)
    set(COMPILER_MSVC 1)
    set(SPCT_MSVC 1)
    set(SPCT_WIN32 1)

    # Guess VCINSTALLDIR from the value of CMAKE_C_COMPILER if it's not set.
    if("$ENV{VCINSTALLDIR}" STREQUAL "")
        string(REGEX REPLACE "/bin/[^/]*$" "" VCINSTALLDIR "${CMAKE_C_COMPILER}")
        message(STATUS "Guessed MSVC directory: ${VCINSTALLDIR}")
    else("$ENV{VCINSTALLDIR}" STREQUAL "")
        file(TO_CMAKE_PATH "$ENV{VCINSTALLDIR}" VCINSTALLDIR)
        message(STATUS "Using VCINSTALLDIR: ${VCINSTALLDIR}")
    endif("$ENV{VCINSTALLDIR}" STREQUAL "")

    # Install in VCINSTALLDIR by default
    if(INSTALL_PREFIX STREQUAL "")
      set(CMAKE_INSTALL_PREFIX ${VCINSTALLDIR})
    else(INSTALL_PREFIX STREQUAL "")
      set(CMAKE_INSTALL_PREFIX ${INSTALL_PREFIX})
    endif(INSTALL_PREFIX STREQUAL "")

    set(EXECUTABLE_TYPE "WIN32")
endif(MSVC)


#
# Min GW
#
if(MINGW)
    set(SPCT_MINGW32 1)
    set(SPCT_WIN32 1)

    # Guess MINGDIR from the value of CMAKE_C_COMPILER if it's not set.
    if("$ENV{MINGDIR}" STREQUAL "")
        string(REGEX REPLACE "/bin/[^/]*$" "" MINGDIR "${CMAKE_C_COMPILER}")
        message(STATUS "Guessed MinGW directory: ${MINGDIR}")
    else("$ENV{MINGDIR}" STREQUAL "")
        file(TO_CMAKE_PATH "$ENV{MINGDIR}" MINGDIR)
        message(STATUS "Using MINGDIR: ${MINGDIR}")
    endif("$ENV{MINGDIR}" STREQUAL "")

    # Search in MINGDIR for headers and libraries.
    set(CMAKE_PREFIX_PATH "${MINGDIR}")

    # Install to MINGDIR
    if(INSTALL_PREFIX STREQUAL "")
      set(CMAKE_INSTALL_PREFIX ${MINGDIR})
    else(INSTALL_PREFIX STREQUAL "")
      set(CMAKE_INSTALL_PREFIX ${INSTALL_PREFIX})
    endif(INSTALL_PREFIX STREQUAL "")

    # Check for a common problem (at the time of writing).
    check_c_source_compiles("
        #include <windows.h>
        int main(void)
        {
            int x = DM_POSITION;
            return 0;
        }"
        HAVE_DM_POSITION)
    if(NOT HAVE_DM_POSITION)
        message(FATAL_ERROR
            "Missing DM_POSITION. Please update your MinGW"
            "w32api package, delete CMakeCache.txt and try again.")
    endif(NOT HAVE_DM_POSITION)
endif(MINGW)


#
# Unix
#
if(UNIX AND NOT APPLE)
    set(SPCT_UNIX 1)
endif(UNIX AND NOT APPLE)


#
# Apple
#
if(APPLE)
    set(MACOSX 1)
    set(SPCT_MACOSX 1)
    set(SPCT_UNIX 0)
 
    # This flag is required on some versions of Mac OS X to avoid linker
    # problems with global variables which are not explicitly initialised.
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-common")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-common")
endif(APPLE)


#
# Borland C++
#
if(BORLAND)
    set(SPCT_BCC32 1)
    set(SPCT_WIN32 1) # not sure
endif(BORLAND)
