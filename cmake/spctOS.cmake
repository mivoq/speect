######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 7 June 2010                                                            ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Speect operating system tests                                                    ##
##                                                                                  ##
##                                                                                  ##
######################################################################################
 

#------------------------------------------------------------------------------------#
#                              Windows OS                                            #
#------------------------------------------------------------------------------------#

if(WIN32)

  # True on windows systems, including win64.
  set(SPCT_WIN32 1)
  set(SPCT_OS "SPCT_WIN32")
endif(WIN32)


#------------------------------------------------------------------------------------#
#                                  UNIX                                              #
#             (and UNIX like operating systems, excluding APPLE/MACOSX)              #
#------------------------------------------------------------------------------------#

if(UNIX AND NOT APPLE)
  
  # Linux/Unix/CYGWIN/..
  set(SPCT_UNIX 1)
  set(SPCT_OS "SPCT_UNIX")
endif(UNIX AND NOT APPLE)
  
#------------------------------------------------------------------------------------#
#                                  APPLE                                             #
#------------------------------------------------------------------------------------#

if(UNIX AND APPLE)
  
  # Apple Mac OSX. 
  set(SPCT_MACOSX 1)
  set(SPCT_OS "SPCT_MACOSX")
  
  # This flag is required on some versions of Mac OS X to avoid linker
  # problems with global variables which are not explicitly initialised.
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-common")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-common")
endif(UNIX AND APPLE)