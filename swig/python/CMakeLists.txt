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



if(NOT "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  
  
  # __init__.py
  file(COPY ${CMAKE_SOURCE_DIR}/swig/python/speect/__init__.py
    DESTINATION ${CMAKE_BINARY_DIR}/swig/python/speect)

  # client.py, server.py
  file(COPY ${CMAKE_SOURCE_DIR}/swig/python/client.py
    DESTINATION ${CMAKE_BINARY_DIR}/swig/python)

  file(COPY ${CMAKE_SOURCE_DIR}/swig/python/server.py
    DESTINATION ${CMAKE_BINARY_DIR}/swig/python)

  # examples
  file(GLOB python_examples "${CMAKE_SOURCE_DIR}/swig/python/examples/*.py")
  file(COPY ${python_examples}
    DESTINATION ${CMAKE_BINARY_DIR}/swig/python/examples)

  # README
  file(COPY ${CMAKE_SOURCE_DIR}/swig/python/README
    DESTINATION ${CMAKE_BINARY_DIR}/swig/python)
  
  # server.conf.example
  file(COPY ${CMAKE_SOURCE_DIR}/swig/python/server.conf.example
    DESTINATION ${CMAKE_BINARY_DIR}/swig/python)


endif(NOT "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
