######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : October 2011                                                           ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Make a "dist" target, which creates a project archive.                           ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

set(SPCT_ARCHIVE_NAME ${CMAKE_PROJECT_NAME}${SPCT_VERSION})
add_custom_target(dist
		  COMMAND git archive --prefix=${SPCT_ARCHIVE_NAME}/ HEAD
		  | gzip > ${CMAKE_BINARY_DIR}/${SPCT_ARCHIVE_NAME}.tar.gz
		  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})