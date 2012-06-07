######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : June 2010                                                              ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Speect plug-in options                                                           ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


#------------------------------------------------------------------------------------#
#                                   HTS Engine                                       #
#------------------------------------------------------------------------------------#

# HAVE_HTS used in speect/plugins/processors/features/nlp/CMakeLists.txt
unset(HAVE_HTS CACHE)
mark_as_advanced(HAVE_HTS)

# 102
set(HTS_ENGINE_INCLUDE_102 CACHE PATH "Path to HTS Engine (1.02) include directory")
set(HTS_ENGINE_LIB_102 CACHE FILEPATH "HTS Engine (1.02) library location (full path and library)")
if(HTS_ENGINE_INCLUDE_102 AND HTS_ENGINE_LIB_102)
  set(HAVE_HTS "on" CACHE BOOL "Variable to check if hts_engine has been defined")
endif(HTS_ENGINE_INCLUDE_102 AND HTS_ENGINE_LIB_102)

# 103
set(HTS_ENGINE_INCLUDE_103 CACHE PATH "Path to HTS Engine (1.03) include directory")
set(HTS_ENGINE_LIB_103 CACHE FILEPATH "HTS Engine (1.03) library location (full path and library)")
if(HTS_ENGINE_INCLUDE_103 AND HTS_ENGINE_LIB_103)
  set(HAVE_HTS "on" CACHE BOOL "Variable to check if hts_engine has been defined")
endif(HTS_ENGINE_INCLUDE_103 AND HTS_ENGINE_LIB_103)

# 104
set(HTS_ENGINE_INCLUDE_104 CACHE PATH "Path to HTS Engine (1.04) include directory")
set(HTS_ENGINE_LIB_104 CACHE FILEPATH "HTS Engine (1.04) library location (full path and library)")
if(HTS_ENGINE_INCLUDE_104 AND HTS_ENGINE_LIB_104)
  set(HAVE_HTS "on" CACHE BOOL "Variable to check if hts_engine has been defined")
endif(HTS_ENGINE_INCLUDE_104 AND HTS_ENGINE_LIB_104)

# 105
set(HTS_ENGINE_INCLUDE_105 CACHE PATH "Path to HTS Engine (1.05) include directory")
set(HTS_ENGINE_LIB_105 CACHE FILEPATH "HTS Engine (1.05) library location (full path and library)")
if(HTS_ENGINE_INCLUDE_105 AND HTS_ENGINE_LIB_105)
  set(HAVE_HTS "on" CACHE BOOL "Variable to check if hts_engine has been defined")
endif(HTS_ENGINE_INCLUDE_105 AND HTS_ENGINE_LIB_105)

# 105 (mixed excitation)
set(HTS_ENGINE_ME_INCLUDE_105 CACHE PATH "Path to HTS Engine (1.05 mixed excitation) include directory")
set(HTS_ENGINE_ME_LIB_105 CACHE FILEPATH "HTS Engine (1.05 mixed excitation) library location (full path and library)")
if(HTS_ENGINE_ME_INCLUDE_105 AND HTS_ENGINE_ME_LIB_105)
  set(HAVE_HTS "on" CACHE BOOL "Variable to check if hts_engine (mixed excitation) has been defined")
endif(HTS_ENGINE_ME_INCLUDE_105 AND HTS_ENGINE_ME_LIB_105)

# 106
set(HTS_ENGINE_INCLUDE_106 CACHE PATH "Path to HTS Engine (1.06) include directory")
set(HTS_ENGINE_LIB_106 CACHE FILEPATH "HTS Engine (1.06) library location (full path and library)")
if(HTS_ENGINE_INCLUDE_106 AND HTS_ENGINE_LIB_106)
  set(HAVE_HTS "on" CACHE BOOL "Variable to check if hts_engine has been defined")
endif(HTS_ENGINE_INCLUDE_106 AND HTS_ENGINE_LIB_106)
