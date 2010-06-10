######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 11 June 2010                                                           ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Speect version information                                                       ##
##                                                                                  ##
##                                                                                  ##
######################################################################################
 

#------------------------------------------------------------------------------------#
#                             Speect version codename                                #
#------------------------------------------------------------------------------------#

set(SPCT_VERSION_RELEASE "Beryllium" CACHE STRING "Speect version codename" FORCE)
mark_as_advanced(SPCT_VERSION_RELEASE)


#------------------------------------------------------------------------------------#
#                           Speect major version number                              #
#------------------------------------------------------------------------------------#

set(SPCT_VERSION_MAJOR 1 CACHE STRING "Speect major version number" FORCE)
mark_as_advanced(SPCT_VERSION_MAJOR)


#------------------------------------------------------------------------------------#
#                           Speect minor version number                              #
#------------------------------------------------------------------------------------#

set(SPCT_VERSION_MINOR 0 CACHE STRING "Speect minor version number" FORCE)
mark_as_advanced(SPCT_VERSION_MINOR)


#------------------------------------------------------------------------------------#
#                               Speect patch number                                  #
#------------------------------------------------------------------------------------#

set(SPCT_VERSION_PATCH 0 CACHE STRING "Speect patch number" FORCE)
mark_as_advanced(SPCT_VERSION_PATCH)


#------------------------------------------------------------------------------------#
#                     Speect version string "major.minor.patch"                      #
#------------------------------------------------------------------------------------#

set(SPCT_VERSION "${SPCT_VERSION_MAJOR}.${SPCT_VERSION_MINOR}.${SPCT_VERSION_PATCH}"
  CACHE STRING "Speect full version number" FORCE)
mark_as_advanced(SPCT_VERSION)

