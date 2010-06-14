######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 9 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMake Speect INI loader                                                          ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


#------------------------------------------------------------------------------------#
#               Configure the ini.c file (loads speect.conf on startup)              #
#------------------------------------------------------------------------------------#

# Note that the SPCT_BUILD_INI_FILE and SPCT_INSTALL_INI_FILE variables in
# ini.c.in are set in speect/cmake/spctSettings.cmake

configure_file(${CMAKE_SOURCE_DIR}/engine/config/ini.c.in 
  ${CMAKE_BINARY_DIR}/engine/src/main/ini.c)
