######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : May 2012                                                               ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for HTS Engine (mixed excitation) synthesis utterance                  ##
## processor plug-in                                                                ##
##                                                                                  ##
######################################################################################

#------------------------------------------------------------------------------------#
#                             Define plug-in                                         #
#------------------------------------------------------------------------------------#

speect_plugin_definition(HTS_Engine_ME_Synth_UttProc_105 "SHTSEngineMESynthUttProc105" 1 0 0)


#------------------------------------------------------------------------------------#
#                        Configure plugin_info.h.in                                  #
#------------------------------------------------------------------------------------#

set(description "An utterance processor to do HTS Engine (mixed excitation) synthesis of a segment relation stream.")

# Minimum required Speect Engine version 
set(major_min 1)
set(minor_min 0)

speect_plugin_configure_info(${description} ${major_min} ${minor_min})


#------------------------------------------------------------------------------------#
#                        Include audio and HTS Engine                                #
#------------------------------------------------------------------------------------#

include(audio)

speect_plugin_include_dirs(${HTS_ENGINE_ME_INCLUDE_105})


#------------------------------------------------------------------------------------#
#                               Source files                                         #
#------------------------------------------------------------------------------------#

# Lists of all the source files
include(sources)


#------------------------------------------------------------------------------------#
#                        Plug-in shared object                                       #
#------------------------------------------------------------------------------------#

speect_plugin_create(${HTS_ENGINE_ME_LIB_105})


