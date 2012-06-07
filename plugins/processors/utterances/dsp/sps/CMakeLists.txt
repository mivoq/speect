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

# Also see speect/plugins/cmake/pluginOptions.cmake

#------------------------------------------------------------------------------------#
#                        Plug-in directories                                         #
#------------------------------------------------------------------------------------#

if(HTS_ENGINE_INCLUDE_102 AND HTS_ENGINE_LIB_102)
  add_subdirectory(hts_engine_102)
endif(HTS_ENGINE_INCLUDE_102 AND HTS_ENGINE_LIB_102)

if(HTS_ENGINE_INCLUDE_103 AND HTS_ENGINE_LIB_103)
  add_subdirectory(hts_engine_103)
endif(HTS_ENGINE_INCLUDE_103 AND HTS_ENGINE_LIB_103)

if(HTS_ENGINE_INCLUDE_104 AND HTS_ENGINE_LIB_104)
  add_subdirectory(hts_engine_104)
endif(HTS_ENGINE_INCLUDE_104 AND HTS_ENGINE_LIB_104)

if(HTS_ENGINE_INCLUDE_105 AND HTS_ENGINE_LIB_105)
  add_subdirectory(hts_engine_105)
endif(HTS_ENGINE_INCLUDE_105 AND HTS_ENGINE_LIB_105)

# mixed excitation
if(HTS_ENGINE_ME_INCLUDE_105 AND HTS_ENGINE_ME_LIB_105)
  add_subdirectory(hts_engine_me_105)
endif(HTS_ENGINE_ME_INCLUDE_105 AND HTS_ENGINE_ME_LIB_105)

if(HTS_ENGINE_INCLUDE_106 AND HTS_ENGINE_LIB_106)
  add_subdirectory(hts_engine_106)
endif(HTS_ENGINE_INCLUDE_106 AND HTS_ENGINE_LIB_106)

