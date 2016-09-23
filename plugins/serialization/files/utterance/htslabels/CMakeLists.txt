######################################################################################
##                                                                                  ##
## AUTHOR  : Matteo Lisotto                                                         ##
## DATE    : 21 September 2015                                                      ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for Speect Utterance MaryXML Plug-in                                   ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#------------------------------------------------------------------------------------#
#                             Include libxml2 library                                #
#------------------------------------------------------------------------------------#

find_package(LibXml2)
if(LIBXML2_FOUND)
include_directories(${LIBXML2_INCLUDE_DIR})
add_definitions(${LIBXML2_DEFINITIONS})



#------------------------------------------------------------------------------------#
#                             Define plug-in                                         #
#------------------------------------------------------------------------------------#

speect_plugin_definition(Utt_HTSLabelsExport "SUttHTSLabelsExport" 1 0 0)


#------------------------------------------------------------------------------------#
#                        Configure plugin_info.h.in                                  #
#------------------------------------------------------------------------------------#

set(description "Save speect utterance's hts labels in XML format to files")

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
#                            Plug-in shared object                                   #
#------------------------------------------------------------------------------------#

speect_plugin_create(${LIBXML2_LIBRARIES})


#------------------------------------------------------------------------------------#
#                              SWIG wrappers                                         #
#------------------------------------------------------------------------------------#

# Speect SWIG interface
if(NOT NO_SWIG)
  add_subdirectory(swig)
endif(NOT NO_SWIG)


endif(LIBXML2_FOUND)
