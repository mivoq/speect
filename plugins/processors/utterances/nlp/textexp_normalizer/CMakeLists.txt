######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 5 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for Textexp Normalizer utterance processor plug-in                     ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

find_package(textexp REQUIRED)

#------------------------------------------------------------------------------------#
#                             Define plug-in                                         #
#------------------------------------------------------------------------------------#

speect_plugin_definition(TextexpNormalization_UttProc "STextexpNormalizationUttProc" 1 0 0)


#------------------------------------------------------------------------------------#
#                        Configure plugin_info.h.in                                  #
#------------------------------------------------------------------------------------#

set(description "An Utterance Processor class that normalizes the tokens in the utterance Token relation and creates the Word relation with these normalized items")

# Minimum required Speect Engine version
set(major_min 1)
set(minor_min 0)

speect_plugin_configure_info(${description} ${major_min} ${minor_min})

speect_plugin_include_dirs(${TEXTEXP_INCLUDE_DIRS})


#------------------------------------------------------------------------------------#
#                               Source files                                         #
#------------------------------------------------------------------------------------#

# Lists of all the source files
include(sources)


#------------------------------------------------------------------------------------#
#                        Plug-in shared object                                       #
#------------------------------------------------------------------------------------#

speect_plugin_create(${TEXTEXP_LIBRARIES})

