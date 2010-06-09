######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 9 Novermber 2009                                                       ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMake Speect initialization file                                                 ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#
# configuration file
#
configure_file(${CMAKE_SOURCE_DIR}/engine/config/speect.ini.in 
  ${SPCT_INI_FILE_PATH}/speect.conf)
