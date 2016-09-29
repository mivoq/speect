######################################################################################
##                                                                                  ##
## AUTHOR  : Federico Rossetto                                                      ##
## DATE    : 8 September 2016                                                       ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for Words from Phrase End feature processor plug-in                  ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#------------------------------------------------------------------------------------#
#                             Define plug-in                                         #
#------------------------------------------------------------------------------------#

speect_plugin_definition(Words_From_Utterance_End_FeatProc "SWordsFromUtteranceEndFeatProc" 1 0 0)


#------------------------------------------------------------------------------------#
#                        Configure plugin_info.h.in                                  #
#------------------------------------------------------------------------------------#

set(description "The number of words from the beginning of the phrase")

# Minimum required Speect Engine version
set(major_min 1)
set(minor_min 0)

speect_plugin_configure_info(${description} ${major_min} ${minor_min})


#------------------------------------------------------------------------------------#
#                               Source files                                         #
#------------------------------------------------------------------------------------#

# Lists of all the source files
include(sources)


#------------------------------------------------------------------------------------#
#                        Plug-in shared object                                       #
#------------------------------------------------------------------------------------#

speect_plugin_create()

