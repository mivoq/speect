######################################################################################
##                                                                                  ##
## AUTHOR  : Federico Rossetto                                                      ##
## DATE    : October 2016                                                           ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for CrfSuite POS utterance processor plug-in                           ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#------------------------------------------------------------------------------------#
#                             Define plug-in                                         #
#------------------------------------------------------------------------------------#

speect_plugin_definition(CrfSuitePOSTagger_UttProc "SCrfSuiteUttProc" 1 0 0)


#------------------------------------------------------------------------------------#
#                        Configure plugin_info.h.in                                  #
#------------------------------------------------------------------------------------#

find_package(crfsuite REQUIRED)

set(description "A part-of-speech Utterance Processor using the crfsuite CRF tagger.")

# Minimum required Speect Engine version
set(major_min 1)
set(minor_min 0)

speect_plugin_configure_info(${description} ${major_min} ${minor_min})

speect_plugin_include_dirs(${CRFSUITE_INCLUDE_DIRS})


#------------------------------------------------------------------------------------#
#                               Source files                                         #
#------------------------------------------------------------------------------------#

# Lists of all the source files
include(sources)


#------------------------------------------------------------------------------------#
#                        Plug-in shared object                                       #
#------------------------------------------------------------------------------------#

speect_plugin_create(${CRFSUITE_LIBRARIES} "m" "dl")

