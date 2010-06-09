######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 7 June 2010                                                            ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Speect system tests                                                              ##
##                                                                                  ##
##                                                                                  ##
######################################################################################
 

#------------------------------------------------------------------------------------#
#                         Include CMake system checking macros                       #
#------------------------------------------------------------------------------------#

include(CheckIncludeFiles)
include(CheckTypeSize)
include(TestBigEndian)


#------------------------------------------------------------------------------------#
#                         Check for system include files                             #
#------------------------------------------------------------------------------------#

# sys/types.h
check_include_files(sys/types.h SPCT_HAVE_SYS_TYPES_H)

# sys/stat.h
check_include_files(sys/stat.h SPCT_HAVE_SYS_STAT_H)

# unistd.h
check_include_files(unistd.h SPCT_HAVE_UNISTD_H)

# limits.h
check_include_files(limits.h SPCT_HAVE_LIMITS_H)

# fcntl.h
check_include_files(fcntl.h SPCT_HAVE_FCNTL_H)

# sys/mman.h
check_include_files(sys/mman.h SPCT_HAVE_SYS_MMAN_H)

# dlfcn.h
check_include_files(dlfcn.h SPCT_HAVE_DLFCN_H)


#------------------------------------------------------------------------------------#
#                             Test system endianess                                  #
#------------------------------------------------------------------------------------#
  
# Test endianess
test_big_endian(SPCT_BIG_ENDIAN)
if(NOT SPCT_BIG_ENDIAN)
  set(SPCT_LITTLE_ENDIAN 1)
endif(NOT SPCT_BIG_ENDIAN)


#------------------------------------------------------------------------------------#
#                                    Types                                           #
#------------------------------------------------------------------------------------#

# Check for long double 
check_type_size("long double" SPCT_LONG_DOUBLE)

