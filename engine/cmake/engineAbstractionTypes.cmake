######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 8 June 2010                                                            ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Speect Engine source files                                                       ##
## Types implementations abstraction                                                ##
##                                                                                  ##
######################################################################################

# SPCT_TYPES_SPECIFIC_IMPL_INCLUDES : list of implementation include files
# SPCT_TYPES_SPECIFIC_IMPL_DIR      : implementation directory


set(SPCT_TYPES_SUPPORT 0)

#------------------------------------------------------------------------------------#
#                              POSIX implementation                                  #
#------------------------------------------------------------------------------------#

set(SPCT_INCLUDE_POSIX_TYPES_FILES
  posix/posix_types.h
)

if(SPCT_UNIX OR SPCT_MACOSX)
  set(SPCT_TYPES_SPECIFIC_IMPL "${SPCT_INCLUDE_POSIX_TYPES_FILES}")
  set(SPCT_TYPES_SPECIFIC_IMPL_DIR "posix")
  
  list(APPEND SPCT_LIBRARY_SOURCES "${CMAKE_SPEECT_SOURCE_DIR}/engine/src/base/utils/platform/${SPCT_TYPES_SPECIFIC_IMPL}")
  set(SPCT_TYPES_SUPPORT 1)
endif(SPCT_UNIX OR SPCT_MACOSX)


#------------------------------------------------------------------------------------#
#                              WIN32 implementation                                  #
#------------------------------------------------------------------------------------#

set(SPCT_INCLUDE_WIN32_TYPES_FILES
  win32/win32_types.h
)

if(SPCT_WIN32)
  set(SPCT_TYPES_SPECIFIC_IMPL "${SPCT_INCLUDE_WIN32_TYPES_FILES}")
  set(SPCT_TYPES_SPECIFIC_IMPL_DIR "win32")

  list(APPEND SPCT_LIBRARY_SOURCES "${CMAKE_SPEECT_SOURCE_DIR}/engine/src/base/utils/platform/${SPCT_TYPES_SPECIFIC_IMPL}")
  set(SPCT_TYPES_SUPPORT 1)
endif(SPCT_WIN32)


#------------------------------------------------------------------------------------#
#                               engine header                                        #
#------------------------------------------------------------------------------------#

configure_file(${CMAKE_SPEECT_SOURCE_DIR}/engine/config/types_impl.h.in 
  ${CMAKE_SPEECT_BINARY_DIR}/engine/src/base/utils/platform/types_impl.h)


#------------------------------------------------------------------------------------#
#                               installation                                         #
#------------------------------------------------------------------------------------#

install(FILES ${CMAKE_SPEECT_SOURCE_DIR}/engine/src/base/utils/platform/${SPCT_TYPES_SPECIFIC_IMPL}
  DESTINATION include/speect/engine/base/utils/platform/${SPCT_TYPES_SPECIFIC_IMPL_DIR}/)

install(FILES ${CMAKE_SPEECT_BINARY_DIR}/engine/src/base/utils/platform/types_impl.h
  DESTINATION include/speect/engine/base/utils/platform)


#------------------------------------------------------------------------------------#
#                          Error if no implementation                                #
#------------------------------------------------------------------------------------#

if(NOT SPCT_TYPES_SUPPORT)
  message(FATAL_ERROR "Unknown types support.")    
endif(NOT SPCT_TYPES_SUPPORT)

