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
#                                Generate setup.py                                   #
#------------------------------------------------------------------------------------#

configure_file(${CMAKE_SOURCE_DIR}/swig/python/config/setup.py.in
  ${CMAKE_BINARY_DIR}/swig/python/setup.py @only)


#------------------------------------------------------------------------------------#
#                          Copy examples and other files                             #
#------------------------------------------------------------------------------------#

if(NOT "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  
  # client.py, server.py
  speect_file_copy(${CMAKE_SOURCE_DIR}/swig/python/client.py ${CMAKE_BINARY_DIR}/swig/python)
  speect_file_copy(${CMAKE_SOURCE_DIR}/swig/python/server.py ${CMAKE_BINARY_DIR}/swig/python)
  
  # examples
  file(GLOB python_examples "${CMAKE_SOURCE_DIR}/swig/python/examples/*.py")
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/swig/python/examples)
  speect_file_copy(${python_examples} ${CMAKE_BINARY_DIR}/swig/python/examples)

  ## examples - ibibio frontend
  file(GLOB ibitio_frontend_examples "${CMAKE_SOURCE_DIR}/swig/python/examples/ibibio_frontend/*")
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/swig/python/examples/ibibio_frontend)
  speect_file_copy(${ibitio_frontend_examples} ${CMAKE_BINARY_DIR}/swig/python/examples/ibibio_frontend)

  # README
  speect_file_copy(${CMAKE_SOURCE_DIR}/swig/python/README ${CMAKE_BINARY_DIR}/swig/python)
  
  # server.conf.example
  speect_file_copy(${CMAKE_SOURCE_DIR}/swig/python/server.conf.example ${CMAKE_BINARY_DIR}/swig/python)

  # spct_python_log.conf Python logger configuration example
  speect_file_copy(${CMAKE_SOURCE_DIR}/swig/python/spct_python_log.conf ${CMAKE_BINARY_DIR}/swig/python)

  # speect/modules
  file(GLOB python_modules "${CMAKE_SOURCE_DIR}/swig/python/speect/modules/*.py")
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/swig/python/speect/modules)
  speect_file_copy(${python_modules} ${CMAKE_BINARY_DIR}/swig/python/speect/modules)

endif(NOT "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
