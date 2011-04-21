######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : April 2011                                                             ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Speect Python settings                                                           ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


#------------------------------------------------------------------------------------#
#                    Speect Python logging configuration file                        #
#------------------------------------------------------------------------------------#

set(SPCT_PYTHON_LOG_CONFIG_FILE "${CMAKE_BINARY_DIR}/spct_python_log.conf")
mark_as_advanced(SPCT_PYTHON_LOG_CONFIG_FILE)


#------------------------------------------------------------------------------------#
#      Configure the log_config.c file (loads spct_python_log.conf for logging)      #
#------------------------------------------------------------------------------------#

configure_file(${CMAKE_SOURCE_DIR}/engine/swig/python/config/log_config.c.in 
  ${CMAKE_BINARY_DIR}/engine/swig/python/log_config.c)


#------------------------------------------------------------------------------------#
#                Copy logging configuration file to home directory                   #
#------------------------------------------------------------------------------------#

speect_file_copy(${CMAKE_SOURCE_DIR}/engine/swig/python/spct_python_log.conf ${CMAKE_BINARY_DIR}/spct_python_log.conf)
 
