######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 7 June 2010                                                            ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Speect compiler options/tests                                                    ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


#------------------------------------------------------------------------------------#
#                               Value of INSTALL_PREFIX                              #
#------------------------------------------------------------------------------------#

set(INSTALL_PREFIX "")


#------------------------------------------------------------------------------------#
#                               Compiler is GCC                                      #
#------------------------------------------------------------------------------------#

if(CMAKE_COMPILER_IS_GNUCC)
  set(SPCT_GCC 1)
endif(CMAKE_COMPILER_IS_GNUCC)


#------------------------------------------------------------------------------------#
#                         Compiler is Microsoft Visual C                             #
#------------------------------------------------------------------------------------#

if(MSVC)
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


#------------------------------------------------------------------------------------#
#                                Compiler is MinGW                                   #
#------------------------------------------------------------------------------------#

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
endif(MINGW)


#------------------------------------------------------------------------------------#
#                                Compiler is Borland C++                             #
#------------------------------------------------------------------------------------#

if(BORLAND)
    set(SPCT_BCC32 1)
    set(SPCT_WIN32 1) # not sure
endif(BORLAND)
