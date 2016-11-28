######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : June 2012                                                              ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## String implementations abstraction                                               ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

# SPCT_STRINGS_SPECIFIC_IMPL_INCLUDES : list of implementation include files
# SPCT_STRINGS_SPECIFIC_IMPL_DIR      : implementation directory

set(SPCT_STRINGS_SUPPORT 0)

#------------------------------------------------------------------------------------#
#                             WIN32 implementation                                   #
#------------------------------------------------------------------------------------#

set(SPCT_SRC_WIN32_STRINGS_FILES
  win32/win32_strings.c
)

set(SPCT_INCLUDE_WIN32_STRINGS_FILES
  win32/win32_strings.h
)

if(SPCT_WIN32)
  set(SPCT_STRINGS_SPECIFIC_IMPL "${SPCT_INCLUDE_WIN32_STRINGS_FILES}")
  set(SPCT_STRINGS_SPECIFIC_IMPL_DIR "win32")

  list(APPEND SPCT_LIBRARY_SOURCES "${CMAKE_SOURCE_DIR}/engine/src/base/strings/platform/${SPCT_SRC_WIN32_STRINGS_FILES}")

  set(SPCT_STRINGS_SUPPORT 1)
endif(SPCT_WIN32)
  

#------------------------------------------------------------------------------------#
#                  no specific implementations  (must be last)                       #
#------------------------------------------------------------------------------------#

set(SPCT_INCLUDE_NO_STRINGS_FILES
  strings_none.h
)

if(NOT SPCT_STRINGS_SUPPORT)
  set(SPCT_STRINGS_SPECIFIC_IMPL "${SPCT_INCLUDE_NO_STRINGS_FILES}")
  set(SPCT_STRINGS_SPECIFIC_IMPL_DIR "")
endif(NOT SPCT_STRINGS_SUPPORT)


#------------------------------------------------------------------------------------#
#                               engine header                                        #
#------------------------------------------------------------------------------------#

configure_file(${CMAKE_SOURCE_DIR}/engine/config/strings_impl.h.in 
  ${CMAKE_BINARY_DIR}/engine/src/base/strings/platform/strings_impl.h)


#------------------------------------------------------------------------------------#
#                               installation                                         #
#------------------------------------------------------------------------------------#

install(FILES ${CMAKE_SOURCE_DIR}/engine/src/base/strings/platform/${SPCT_STRINGS_SPECIFIC_IMPL}
  DESTINATION include/speect/engine/base/strings/platform/${SPCT_STRINGS_SPECIFIC_IMPL_DIR}/)

install(FILES ${CMAKE_BINARY_DIR}/engine/src/base/strings/platform/strings_impl.h
  DESTINATION include/speect/engine/base/strings/platform)


