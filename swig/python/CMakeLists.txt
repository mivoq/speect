######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : June 2010                                                              ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList to copy Speect Python examples and other files to CMAKE_BINARY_DIR     ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


#------------------------------------------------------------------------------------#
#                              Generate __init__.py                                  #
#------------------------------------------------------------------------------------#

configure_file(${CMAKE_SOURCE_DIR}/swig/python/config/__init__.py.in
  ${CMAKE_BINARY_DIR}/swig/python/speect/__init__.py @only)


#------------------------------------------------------------------------------------#
#                          Copy examples and other files                             #
#------------------------------------------------------------------------------------#

if(NOT "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  
  # client.py, server.py
  speect_file_copy(${CMAKE_SOURCE_DIR}/swig/python/client.py ${CMAKE_BINARY_DIR}/swig/python)
  speect_file_copy(${CMAKE_SOURCE_DIR}/swig/python/server.py ${CMAKE_BINARY_DIR}/swig/python)
  
  # examples
  file(GLOB python_examples "${CMAKE_SOURCE_DIR}/swig/python/examples/*.py")
  speect_file_copy(${python_examples} ${CMAKE_BINARY_DIR}/swig/python/examples)

  # README
  speect_file_copy(${CMAKE_SOURCE_DIR}/swig/python/README ${CMAKE_BINARY_DIR}/swig/python)
  
  # server.conf.example
  speect_file_copy(${CMAKE_SOURCE_DIR}/swig/python/server.conf.example ${CMAKE_BINARY_DIR}/swig/python)

endif(NOT "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
