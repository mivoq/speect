######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 11 June 2010                                                           ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for dsp statistical parametric synthesis utterance processors plug-ins ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#------------------------------------------------------------------------------------#
#                        Plug-in directories                                         #
#------------------------------------------------------------------------------------#

if(HTS_ENGINE_INCLUDE AND HTS_ENGINE_LIB)
  add_subdirectory(hts_engine)
endif(HTS_ENGINE_INCLUDE AND HTS_ENGINE_LIB)

if(HTS_ENGINE_INCLUDE_102 AND HTS_ENGINE_LIB_102)
  add_subdirectory(hts_engine_102)
endif(HTS_ENGINE_INCLUDE_102 AND HTS_ENGINE_LIB_102)

if(HTS_ENGINE_INCLUDE_103 AND HTS_ENGINE_LIB_103)
  add_subdirectory(hts_engine_103)
endif(HTS_ENGINE_INCLUDE_103 AND HTS_ENGINE_LIB_103)

